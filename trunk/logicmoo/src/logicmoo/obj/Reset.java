package logicmoo.obj;


import java.util.Enumeration;
import net.n3.nanoxml.*;


public class Reset extends CycThing implements GuidEnabled {

    public static final String MARKUP = "RESET";

    private static final String
    PROPERTY_TARGET = "TARGET",
    PROPERTY_TYPE = "TYPE";

    public static final String
    TYPE_LOAD_ITEM = "ITEM",
    TYPE_LOAD_MOBILE = "MOBILE",
    TYPE_DOOR_STATE = "STATE";


    private String target;
    public String getTarget() { return target;}
    public void setTarget(String target) { this.target = target;}

    private String type;
    public String getType() { return type;}
    public void setType(String type) { this.type = type;}

    private String action;
    public String getAction() { return action;}
    public void setAction(String action) { this.action = action;}

    private Area source;



    public Reset(Area source) {
	this.source = source;
    }



    public void apply() {
	try {
	    Room trg = (Room) source.getRooms().get(target);
	    int I = action.indexOf(",");
	    String a = action.substring(0, I).trim(),
		       b = action.substring(I+1).trim();
	    int l;

	    if ( trg != null ) {
		if ( type.equals(TYPE_LOAD_ITEM) ) {
		    l = ((b != null && b.length() > 0) ? Integer.parseInt(b) : 0);
		    Item itm = (Item) source.getItems().get(a);
		    if ( itm != null )
			rst_LOAD_ITEM(trg, itm, l);
		} else if ( type.equals(TYPE_LOAD_MOBILE) ) {
		    l = ((b != null && b.length() > 0) ? Integer.parseInt(b) : 0);
		    Body bdy = (Body) source.getBodies().get(a);
		    if ( bdy != null )
			rst_LOAD_MOB(trg, bdy, l);
		} else if ( type.equals(TYPE_DOOR_STATE) ) {
		    Exit ext = trg.getExit(a);
		    int S = Integer.parseInt(b);
		    rst_DOOR_STATE(ext, S);
		}
	    }
	} catch ( Exception e ) {
	    System.out.println(""+ e);
	}
    }


    public void rst_LOAD_ITEM(Room trg, Item itm, int limit) {
	try {
	    int I = 0;
	    if ( limit > 0 ) {
		Object enum[] = trg.getItems().toArray();
		for ( int i=0; i<enum.length; i++ )
		    if ( ((Item) enum[i]).getCycFort().equals(itm.getCycFort()) )
			I++;
	    }
	    if ( limit > I || limit == 0 )
		trg.getItems().add(itm.getSemiClone());
	} catch ( Exception e ) {
	    System.out.println(""+ e);
//	    e.printStackTrace();
	}
    }


    public void rst_LOAD_MOB(Room trg, Body mob, int limit) {
	int I = 0;
	if ( limit > 0 ) {
	    String v = mob.getCycFortString();
	    Object ppl[] = trg.getBodies().toArray();
	    for ( int i=0; i<ppl.length; i++ ) {
		Body it = (Body) ppl[i];
		if ( it.getCycFortString().equals(v) ) I++;
	    }
	}
	if ( limit > I || limit == 0 ) {
	    Body t = (Body)mob.getSemiClone();
	    trg.enter(t, t.getName() + " has arrived.");
	}
    }


    public void rst_DOOR_STATE(Exit ext, int st) {
	if ( ext != null )
	    if ( (ext.isType(Exit.TYPE_LOCK) && st <= Exit.STATE_LOCKED) ||
		 (ext.isType(Exit.TYPE_DOOR) && st <= Exit.STATE_CLOSED) )
		ext.setState(st);
    }


    public static Reset loadReset(Area area, XMLElement xml) throws Exception {
	if ( ! xml.getName().equals(MARKUP) )
	    throw new Exception("non-" + MARKUP + " element in Reset.loadReset");

	Reset reset = new Reset(area);

	reset.setCycFortString( xml.getAttribute(GuidEnabled.PROPERTY_VNUM));
	reset.target = xml.getAttribute(PROPERTY_TARGET);
	reset.type = xml.getAttribute(PROPERTY_TYPE, TYPE_DOOR_STATE);
	reset.action = xml.getContent();

	return reset;
    }


    public XMLElement toXMLElement() {
	XMLElement xml = new XMLElement();
	xml.setName(MARKUP);
	xml.setAttribute(GuidEnabled.PROPERTY_VNUM, getCycFortString());
	xml.setAttribute(PROPERTY_TYPE, type);
	xml.setAttribute(PROPERTY_TARGET, target);
	xml.setContent(action);
	return xml;
    }

}
