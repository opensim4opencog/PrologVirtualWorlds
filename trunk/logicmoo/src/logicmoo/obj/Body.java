package logicmoo.obj;


import java.util.*;
import logicmoo.*;
import logicmoo.cmd.*;
import logicmoo.obj.event.*;
import logicmoo.plugin.Fight;
import logicmoo.script.*;
import logicmoo.util.*;
import net.n3.nanoxml.*;


public class Body extends Item implements Flagged, GuidEnabled, JamudEventTrigger {

    public static final String MARKUP = "BODY";

    private static final String
    PROPERTY_STATLIST = "STATLIST",
    PROPERTY_SKILLS = "SKILLS",
    PROPERTY_JOB = "JOB",
    PROPERTY_RACE = "RACE",
    PROPERTY_GENDER = "GENDER",
    PROPERTY_SCRIPT = "SCRIPT",
    PROPERTY_WORN = "WORN",
    PROPERTY_WEAR_LOCATION = "WEAR";

    public static final String
    STAT_INT = "INT",
    STAT_WIS = "WIS",
    STAT_DEX = "DEX",
    STAT_STR = "STR",
    STAT_CON = "CON",
    STAT_LEVEL = "LEVEL",
    STAT_HP_MAX = "HPMAX",
    STAT_MANA_MAX = "MAMAX",
    STAT_MOVE_MAX = "MVMAX",
    STAT_HP = "HP",
    STAT_MANA ="MA",
    STAT_MOVE = "MV",
    STAT_ARMOR_BASH = "A_BASH",
    STAT_ARMOR_PIERCE = "A_PIERCE",
    STAT_ARMOR_SLASH = "A_SLASH",
    STAT_ARMOR_MAGIC = "A_MAGIC",
    STAT_SAVES = "SAVES",
    STAT_EXP = "EXP",
    STAT_EXP_TNL = "EXPTNL",
    STAT_ALIGNMENT = "ALIGN",
    STAT_WIMPY = "WIMPY",
    STAT_GOLD = "GOLD",
    STAT_POSITION = "POSITION",
    STAT_VISIBLE = "VISIBLE";

    public static final String
    FLAG_ETHOS_CHAOTIC = "CHAOTIC",
    FLAG_ETHOS_LAWFUL = "LAWFUL",
    FLAG_ETHOS_NEUTRAL = "NEUTRAL";

    public static final int
    POSITION_STANDING = 0,
    POSITION_SITTING = 1,
    POSITION_SLEEPING = 2;

    public static final int
    VIS_HIDDEN = 1,
    VIS_INVIS = 2,
    VIS_WIZINVIS = 4;

    private Room room = Jamud.defaultRoom;
    public Room getRoom() { return room;}
    public void setRoom(Room room) { this.room = room;}

    private Player player;
    public Player getPlayer() { return player;}
    public void setPlayer(Player player) { this.player = player;}

    private Vector scripts = new Vector();
    public Vector getScripts() { return scripts;}

    private Hashtable wornonbody = new Hashtable();
    public Hashtable getWorn() { return wornonbody;}
    public Item getWornOn(String location) { return(Item) wornonbody.get(location.toUpperCase());}
    public void setWornOn(String location, Item i) {
	if ( i == null )
	    wornonbody.remove(location.toUpperCase());
	else
	    wornonbody.put(location.toUpperCase(), i);
    }

    private Fight fight;
    public Fight getFight() { return fight;}
    public void setFight(Fight fight) { this.fight = fight;}

    private Gender gender;
    public Gender getGender() { return gender;}
    public void setGender(Gender gender) { this.gender = gender;}
    public void setGender(String gString) { this.gender = Gender.getGender( gString );}

    private Race race;
    public Race getRace() { return race;}
    public void setRace(Race race) { this.race = race;}
    public void setRace(String rString) { this.race = Race.getRace( rString );}

    private Job job;
    public Job getJob() { return job;}
    public void setJob(Job job) { this.job = job;}
    public void setJob(String rString) { this.job = Job.getJob( rString );}

    public Body() {
	//empty
    }



    /*
    public void refreshAffects() {
	synchronized(affectStats) {
	if(affectStats == null) {
	    if(affects == null || affects.size() == 0)
	    return;
	    else
	    affectStats = new StatTable();
	}
	else
	    affectStats.clear();
    
	for(Enumeration enum = affects.elements(); enum.hasMoreElements(); )
	    ((BodyAffect)enum.nextElement()).affectRefresh();
	}
    }
    */


    public boolean enact(String command) {
	return enact(command, false);
    }


    public boolean enact(String command, boolean fromFight) {
	if ( command == null )
	    command = "";

	String[] sp = Util.split(command, " ");
	return BodyInterpreter.execute(this, sp[0], sp[1], fromFight);
    }


    public boolean enact(String command, String args) {
	return enact(command, args, false);
    }


    public boolean enact(String command, String args, boolean fromFight) {
	if ( command == null )
	    command = "";

	if ( args == null )
	    args = "";

	return BodyInterpreter.execute(this, command, args, fromFight);
    }


    public void quit() {
	getRoom().exit(this, this.getName() + " slowly fades from view.");
    }


    public void grant(BodyGranted skill, int level) {
	getStats().put(skill.getName().toLowerCase(), new Integer(level));
    }


    public BodyGranted getStats(String name) {
	name = name.toLowerCase();
	Integer in = (Integer) getStats().get(name);

	if ( in != null )
	    return BodyInterpreter.grantedTable.get(name);
	else
	    for ( Enumeration enum = getStats().keys(); enum.hasMoreElements(); ) {
		String ns = (String) enum.nextElement();
		if ( ns.startsWith(name) ) return BodyInterpreter.grantedTable.get(ns);
	    }

	return null;
    }

    public void print(String text) {
	if ( player != null )
	    player.print(text);
	else
	    triggerOnText(text);
    }


    public void print(byte[] text) {
	if ( player != null )
	    player.print(text);
	else
	    triggerOnText(new String(text));
    }


    public void println(String text) {
	if ( player != null )
	    player.println(text);
	else
	    triggerOnText(text + "\n");
    }


    public void triggerOnText(String text) {
	Object[] o = { text};
	Jamud.getEventManager().triggerListeners(new BodyEvent(this, BodyEvent.EVENT_TEXT, o));
    }


    public void triggerOnArrive(Room where) {
	Object[] o = { where};
	Jamud.getEventManager().triggerListeners(new BodyEvent(this, BodyEvent.EVENT_ARRIVE, o));
    }


    public void triggerOnDepart(Room where) {
	Object[] o = { where};
	Jamud.getEventManager().triggerListeners(new BodyEvent(this, BodyEvent.EVENT_DEPART, o));
    }


    public void triggerOnEnter(Body who) {
	Object[] o = { who};
	Jamud.getEventManager().triggerListeners(new BodyEvent(this, BodyEvent.EVENT_ENTER, o));
    }


    public void triggerOnExit(Body who) {
	Object[] o = { who};
	Jamud.getEventManager().triggerListeners(new BodyEvent(this, BodyEvent.EVENT_EXIT, o));
    }


    public void triggerOnTick() {
	Jamud.getEventManager().triggerListeners(new BodyEvent(this, JamudEvent.EVENT_TICK, new Object[0]));
    }


    public void tick() {

	//trigger our event
	triggerOnTick();

	//trigger our getItems()' events
	for ( Enumeration enum = getItems().elements(); enum.hasMoreElements(); )
	    ((Item)enum.nextElement()).tick();

    }


    public Object getSemiClone() {

	Body i = new Body();

	//PartiallyTangible
	i.setCycFortString(getCycFortString()+"Cloned") ;
	i.setName(getName());
	i.setDesc(getDesc());
	i.setShortDesc(getShortDesc());
	i.setKeywords(new FlagList(i,i.makeCycConstant("mudKeyword"),null,getKeywords()));

	//GuidEnabled
	//i.area = area;

	//Flagged
	i.setFlags(new FlagList(i,i.makeCycConstant("mudFlag"),i.makeCycConstant("MudFlag"),getFlags()));

	i.setWearLocations(new FlagList(i,i.makeCycConstant("mudWearable"),i.makeCycConstant("MudBodyPart"),getWearLocations()));
	
	i.setStats((StatTable)getStats().clone());

	//n.area = area;
	i.gender = gender;
	i.race = race;
	i.job = job;

	//Scripts
	for ( Enumeration enum = scripts.elements(); enum.hasMoreElements(); )
	    i.scripts.add( ((EventScript)enum.nextElement()).getSemiClone(i) );

	return i;
    }


    public static Body loadBody(XMLElement xml) throws Exception {
	if ( ! xml.getName().equals(MARKUP) )
	    throw new Exception("non-" + MARKUP + " element in Body.loadBody");

	Body b = new Body();

	b.setCycFortString(xml.getAttribute(GuidEnabled.PROPERTY_VNUM));
	b.setName(xml.getAttribute(CycThing.PROPERTY_NAME));
	b.gender = Gender.getGender(xml.getAttribute(PROPERTY_GENDER, ""));
	b.race = Race.getRace(xml.getAttribute(PROPERTY_RACE, ""));
	b.job = Job.getJob(xml.getAttribute(PROPERTY_JOB, ""));

	for ( Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
	    XMLElement nxt = (XMLElement) enum.nextElement();
	    String nom = nxt.getName(); nxt.getContent();

	    if ( nom.equals(PartiallyTangible.PROPERTY_KEYWORDS) )
		b.setKeywords(new FlagList(nxt.getContent()));

	    else if ( nom.equals(PartiallyTangible.PROPERTY_DESC) ) {
		b.setShortDesc(nxt.getAttribute(PartiallyTangible.PROPERTY_SHORTDESC));
		b.setDesc(Util.format(nxt.getContent(), 70, true));
	    }

	    else if ( nom.equals(PROPERTY_STATLIST) )
		b.setStats( new StatTable(nxt.getContent()));

	    else if ( nom.equals(PROPERTY_SKILLS) )
		b.setStats( new StatTable(nxt.getContent()));

	    else if ( nom.equals(PROPERTY_SCRIPT) ) {
		EventScript es = EventScript.loadEventScript(b, nxt);
		if ( es != null )
		    b.scripts.add(es);
	    }

	    else if ( nom.equals(Inventory.MARKUP) )
		b.setItems(Inventory.loadInventory(b.getMt(),b.getCycFort(),makeCycConstant("mudObjectFoundInLocation"),makeCycConstant("MudItem"),xml));

	    else if ( nom.equals(PROPERTY_WORN) ) {
		for ( Enumeration renum = nxt.enumerateChildren(); renum.hasMoreElements(); ) {
		    XMLElement x = (XMLElement) renum.nextElement();

		    if ( ! x.getName().equals(PROPERTY_WEAR_LOCATION) )
			throw new Exception("non-" + PROPERTY_WEAR_LOCATION + " element loading wornonbody inventory");

		    String n = x.getAttribute(PROPERTY_NAME, null);
		    if ( n != null )
			for ( Enumeration xnum = x.enumerateChildren(); xnum.hasMoreElements(); )
			    b.wornonbody.put(n, Item.loadItem((XMLElement) xnum.nextElement()));
		}
	    }

	}

	return b;
    }


    public XMLElement toXMLElement() {

	System.out.println("body");
	XMLElement xml = new XMLElement();
	xml.setName(MARKUP);
	xml.setAttribute(GuidEnabled.PROPERTY_VNUM, getCycFortString());
	xml.setAttribute(CycThing.PROPERTY_NAME, getName());
	xml.setAttribute(Body.PROPERTY_RACE, getRace().getName());
	xml.setAttribute(Body.PROPERTY_GENDER, getGender().getName());
	xml.setAttribute(Body.PROPERTY_JOB, getJob().getName());

	XMLElement nxt;

	System.out.println("body.keywords");
	nxt = new XMLElement();
	nxt.setName(PartiallyTangible.PROPERTY_KEYWORDS);
	nxt.setContent(getKeywords().toString());
	xml.addChild(nxt);

	System.out.println("body.desc");
	nxt = new XMLElement();
	nxt.setName(PartiallyTangible.PROPERTY_DESC);
	nxt.setAttribute(PartiallyTangible.PROPERTY_SHORTDESC, getShortDesc());
	nxt.setContent(getDesc());
	xml.addChild(nxt);

	System.out.println("body.stats");
	nxt = new XMLElement();
	nxt.setName(PROPERTY_STATLIST);
	nxt.setContent(getStats().toString());
	xml.addChild(nxt);

	System.out.println("body.getStats()");
	nxt = new XMLElement();
	nxt.setName(PROPERTY_SKILLS);
	nxt.setContent(getStats().toString());
	xml.addChild(nxt);

	System.out.println("body.wornonbody");
	nxt = new XMLElement();
	nxt.setName(PROPERTY_WORN);
	for ( Enumeration enum = wornonbody.keys(); enum.hasMoreElements(); ) {
	    String k = (String) enum.nextElement();
	    Item i = (Item) wornonbody.get(k);
	    if ( i != null ) {
		XMLElement x = new XMLElement();
		x.setName(PROPERTY_WEAR_LOCATION);
		x.setAttribute(PROPERTY_NAME, k);
		x.addChild(i.toXMLElement());
		nxt.addChild(x);
	    }
	}
	xml.addChild(nxt);

	nxt = null;

	System.out.println("body.getItems()");
	xml.addChild(getItems().toXMLElement());

	return xml;
    }
	 /*
    public void saveCyc() {
	super.saveCyc();
	Inventory.saveInventory(getMt(),getItems(),"mudObjectFoundInObject",getCycFort());
	CycMoo.assertSlotValue(getMt(),getCycFort(),"mudRace",getRace().getCycFort(),true);
	CycMoo.assertSlotValue(getMt(),getCycFort(),"mudGender",getGender().getCycFort(),true);
	CycMoo.assertSlotValue(getMt(),getCycFort(),"mudJob",getJob().getCycFort(),true);
	CycMoo.assertSlotValue(getMt(),getCycFort(),"mudObjectFoundInLocation",getRoom().getCycFort(),true);
	//FlagList.assertList(getMt(),getCycFort(),"mudObjectFoundWarnByObject",getWorn().containsKey() );
			    
    }
			*/

}
