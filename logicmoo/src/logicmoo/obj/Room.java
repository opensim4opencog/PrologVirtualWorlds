package logicmoo.obj;


import logicmoo.*;
import logicmoo.obj.event.*;
import logicmoo.plugin.Fight;
import logicmoo.script.*;
import logicmoo.util.*;
import java.util.*;
import net.n3.nanoxml.*;


public class Room extends Item implements GuidEnabled, Flagged, JamudEventTrigger {

    public static final String MARKUP = "ROOM";

    private static final String
    PROPERTY_TYPE = "TYPE",
    PROPERTY_SCRIPT = "SCRIPT";

    public static final String
    TYPE_INDOOR = "INDOOR",
    TYPE_STREET = "STREET",
    TYPE_PLAIN = "PLAIN",
    TYPE_WATER = "WATER",
    TYPE_AIR = "AIR",
    TYPE_FOREST = "FOREST",
    TYPE_MOUNTAIN = "MOUNTAIN",
    TYPE_DESERT = "DESERT",
    TYPE_SWAMP = "SWAMP",
    TYPE_VOID = "VOID";

    public static final String
    FLAG_SAFE = "SAFE",
    FLAG_NORECALL = "NO_RECALL",
    FLAG_DARK = "DARK",
    FLAG_NOENTER = "NO_ENTER",
    FLAG_NOEXIT = "NO_EXIT",
    FLAG_EVIL = "EVIL",
    FLAG_GOOD = "GOOD",
    FLAG_NEUTRAL = "NEUTRAL",
    FLAG_ANTI_EVIL = "ANTI-EVIL",
    FLAG_ANTI_GOOD = "ANTI-GOOD",
    FLAG_ANTI_NEUTRAL = "ANTI-NEUTRAL";

    private Vector exits = new Vector();
    public Enumeration enumerateExits() { return exits.elements();}
    public Vector getExits() { return exits;}
    public void addExit(Exit exit) {
	if ( ! exits.contains(exit) )
	    exits.add(exit);
    }
    public void removeExit(Exit exit) { exits.remove(exit);}
    public void removeExit(String dir) {
	Exit e = getExit(dir);
	if ( e != null )
	    removeExit(e);
    }

    private Vector scripts = new Vector();
    public Vector getScripts() { return scripts;}

    //private Area area;
    //public Area getArea() { return area;}

    private Vector fights;
    public Vector getFights() {
	if ( fights == null ) {
	    synchronized(fights) {
		if ( fights == null )
		    fights = new Vector();
	    }
	}
	return fights;
    }
    public int getFightCount() {
	if ( fights != null )
	    return fights.size();
	else
	    return 0;
    }



    public Room(Area area) {
	//this.area = area;
    }



    public void enter(Body who) {
	enter(who, who.getName() + " has arrived.");
    }


    public void enter(Body who, String msg) {
	who.setRoom(this);
	getBodies().add(who);

	if ( who.getPlayer() != null )
	    who.println(getView(who));

	if ( msg != null )
	    println(who, null, msg);

	// trigger onEnter event on room
	triggerOnEnter(who);

	// trigger onEnter event on bodies
	for ( Enumeration enum = getBodies().elements(); enum.hasMoreElements(); ) {
	    Body body = (Body) enum.nextElement();
	    if ( body != who )
		body.triggerOnEnter(who);
	}

	// trigger onArrive event on who
	who.triggerOnArrive(this);

    }


    public void triggerOnEnter(Body who) {
	Object[] o = { who};
	Jamud.getEventManager().triggerListeners(new RoomEvent(this, RoomEvent.EVENT_ENTER, o));
    }


    public void triggerOnExit(Body who) {
	Object[] o = { who};
	Jamud.getEventManager().triggerListeners(new RoomEvent(this, RoomEvent.EVENT_EXIT, o));
    }


    public void exit(Body who) {
	exit(who, who.getName() + " leaves.");
    }


    public void exit(Body who, String msg) {
	if ( msg != null )
	    println(who, null, msg);

	// trigger onExit event on room
	triggerOnExit(who);

	// trigger onExit event on bodies
	for ( Enumeration enum = getBodies().elements(); enum.hasMoreElements(); ) {
	    Body body = (Body) enum.nextElement();
	    if ( body != who )
		body.triggerOnExit(who);
	}

	// trigger onDepart event on who
	who.triggerOnDepart(this);

	getBodies().remove(who);
	who.setRoom(null);
    }


    public Exit getExit(String name) {
	if ( name == null || name.length() == 0 )
	    return null;

	name = name.toLowerCase();
	for ( Enumeration enum = exits.elements(); enum.hasMoreElements(); ) {
	    Exit i = (Exit) enum.nextElement();
	    if ( i.getName().toLowerCase().startsWith(name) )
		return i;
	}
	return null;
    }

    private String exitsView() {
	String r = "Exits: {&[{0";

	if ( exits.size() == 0 )
	    r += " none";
	else
	    for ( Enumeration enum = exits.elements(); enum.hasMoreElements(); )
		r += " " + ((Exit)enum.nextElement()).getName().toLowerCase();

	r += " {&]{0";

	return r;
    }

    public String getView(Body from) {
	return "{&" + getName() + "\n{0" + 
	getDesc() + "\n" + 
	exitsView() +
	"{$" + itemsView(from) + "{^" + 
	bodiesView(from) + "{0";
    }

    public PartiallyTangible getViewable(String name) {
	Item i = getItem(name);
	if ( i == null ) {
	    Body b = getBody(name);
	    if ( b == null ) {
		Exit e = getExit(name);
		return(PartiallyTangible) e;
	    } else
		return(PartiallyTangible) b;
	} else
	    return(PartiallyTangible) i;
    }


    public void triggerOnTick() {
	Jamud.getEventManager().triggerListeners(new RoomEvent(this, JamudEvent.EVENT_TICK, new Object[0]));
    }

    //public void tick() {}

    public static Room loadRoom(Area area, XMLElement xml) throws Exception {
	if ( ! xml.getName().equals(MARKUP) )
	    throw new Exception("non-" + MARKUP + " element in Room.loadRoom");

	if ( area==null ) area = Jamud.defaultArea;

	Room room = new Room(area);

	room.setName(xml.getAttribute(PartiallyTangible.PROPERTY_NAME));
	room.setCycFortString(xml.getAttribute(GuidEnabled.PROPERTY_VNUM));
	room.setType(xml.getAttribute(PROPERTY_TYPE, TYPE_INDOOR));

	for ( Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
	    XMLElement nxt = (XMLElement) enum.nextElement();
	    String nom = nxt.getName(); nxt.getContent();

	    if ( nom.equals(PartiallyTangible.PROPERTY_DESC) ) {
		room.setDesc(Util.format(nxt.getContent(), 78, true));
		//room.setDesc(nxt.getContent());
		room.setShortDesc(nxt.getAttribute(PartiallyTangible.PROPERTY_SHORTDESC));
	    } else if ( nom.equals(Exit.MARKUP) )
		room.addExit(Exit.loadExit(nxt));
	    else if ( nom.equals(PROPERTY_SCRIPT) ) {
		EventScript es = EventScript.loadEventScript(room, nxt);
		if ( es != null )
		    room.scripts.add(es);
	    } else {
		System.out.println("unknown: "+nom);
	    }
	}

	return room;
    }


    public XMLElement toXMLElement() {
	//room
	XMLElement xml = new XMLElement();
	xml.setName(MARKUP);
	xml.setAttribute(PartiallyTangible.PROPERTY_NAME, getName());
	xml.setAttribute(GuidEnabled.PROPERTY_VNUM, getCycFortString());
	xml.setAttribute(PROPERTY_TYPE, getType());

	XMLElement nxt;

	//room.desc
	nxt = new XMLElement();
	nxt.setName(PartiallyTangible.PROPERTY_DESC);
	nxt.setAttribute(PartiallyTangible.PROPERTY_SHORTDESC, getShortDesc());
	nxt.setContent(getDesc());
	xml.addChild(nxt);

	//room.script
	//bla

	nxt = null;

	//room.exits
	for ( Enumeration enum = exits.elements(); enum.hasMoreElements(); )
	    xml.addChild( ((Exit)enum.nextElement()).toXMLElement() );

	return xml;
    }


    public boolean equals(Object o) {
	return(o == (Object)this);
    }

    public void saveCyc() {
//	super.saveCyc();
      //  Exit.saveExits(getMt(),getCycFort(),"mudExit",getExits());
    }



}
