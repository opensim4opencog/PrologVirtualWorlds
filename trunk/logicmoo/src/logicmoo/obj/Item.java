package logicmoo.obj;


import java.util.Enumeration;
import java.util.Iterator;
import logicmoo.Jamud;
import logicmoo.obj.event.*;
import logicmoo.util.*;
import net.n3.nanoxml.*;


public class Item extends PartiallyTangible implements Flagged, GuidEnabled, JamudEventTrigger {

    public static final String MARKUP = "ITEM";

    private static final String
    PROPERTY_AREA = "AREA",
    PROPERTY_TYPE = "TYPE",
    PROPERTY_STATLIST = "STATLIST",
    PROPERTY_WEARLOCATIONS = "WEARLOCATIONS";

    public static final String
    FLAG_ANTI_EVIL = "ANTI_EVIL",
    FLAG_ANTI_GOOD = "ANTI_GOOD",
    FLAG_ANTI_NEUT = "ANTI_NEUT",
    FLAG_CLOTH = "CLOTH",
    FLAG_CONTAIN = "CONTAIN",
    FLAG_DRINK = "DRINK",
    FLAG_EAT = "EAT",
    FLAG_ENTER = "ENTER",
    FLAG_EVIL = "EVIL",
    FLAG_FLAME = "FLAME",
    FLAG_FLOAT = "FLOAT",
    FLAG_FLY = "FLY",
    FLAG_GLOW = "GLOW",
    FLAG_GOOD = "GOOD",
    FLAG_HIDE = "HIDE",
    FLAG_HUMM = "HUMM",
    FLAG_ICE = "ICE",
    FLAG_INVIS = "INVIS",
    FLAG_KEY = "KEY",
    FLAG_MAGIC = "MAGIC",
    FLAG_MELT_DROP = "MELT_DROP",
    FLAG_METAL = "METAL",
    FLAG_NEUT = "NEUT",
    FLAG_NO_DROP = "NO_DROP",
    FLAG_NO_REMOVE = "NO_REMOVE",
    FLAG_NULLIFY = "NULLIFY",
    FLAG_REST_AT = "REST_AT",
    FLAG_REST_IN = "REST_IN",
    FLAG_REST_ON = "REST_ON",
    FLAG_RIDE_IN = "RIDE_IN",
    FLAG_RIDE_ON = "RIDE_ON",
    FLAG_SHOCK = "SHOCK",
    FLAG_SIT_AT = "SIT_AT",
    FLAG_SIT_IN = "SIT_IN",
    FLAG_SIT_ON = "SIT_ON",
    FLAG_SLEEP_AT = "SLEEP_AT",
    FLAG_SLEEP_IN = "SLEEP_IN",
    FLAG_SLEEP_ON = "SLEEP_ON",
    FLAG_TAKE = "TAKE",
    FLAG_VORPAL = "VORPAL",
    FLAG_WEAR = "WEAR",
    FLAG_WOOD = "WOOD";

    
    public FlagList getWearLocations() { return getSlotValueFlagList(getMt(),getCycFort(),"mudWearable","MudBodyPart"); }
    public void setWearLocations(FlagList list) { assertSlotValue(getMt(),getCycFort(),"mudWearable",list.enumerate(),"MudBodyPart",true); }
    public boolean isWearableOn(String location) { return getWearLocations().contains(location);}



//    public String getShortDesc() {  return ""+getSlotValueObject(getMt(),getCycFort(),"mudShortDescription");}
//    public void setShortDesc(String desc) { assertSlotValue(getMt(),getCycFort(),"mudShortDescription",desc,true);}


    private StatTable skills = new StatTable();
    public StatTable getStats() { return skills;}
    public void setStats(StatTable st) { skills = st;}
    public void setStats(String skill, int level) { skills.set(skill, level);}
    public int getSkillLevel(String skill) { return skills.stat(skill, 0);}


    public void setItems(Inventory list) { assertSlotValue(getMt(),getCycFort(),"mudObjectFoundInLocation",list.elements(),"MudItem",true); }
    
    public Inventory getItems() { return getSlotValueInventory(getMt(),getCycFort(),"mudObjectFoundInLocation","MudItem");}
    public Inventory getBodies() { return getSlotValueInventory(getMt(),getCycFort(),"mudObjectFoundInLocation","MudBody");}

    public Item getItem(String name) {
	return getItems().getItem(name);
    }

    public Item fetchItem(String name) {
	return getItems().fetchItem(name);
    }

    public String itemsView(Body from) {
	if ( getItems().size() > 0 )
	    return "\n" + getItems().getShortDescView(from, true);
	else
	    return "";
    }

    public Item() {
	//empty
    }

    public Item(String stringfort) {
	setCycFortString(stringfort);
    }

    public PartiallyTangible getViewable(String name) {
	Item i = getItem(name);
	if ( i == null ) {
	    Body b = getBody(name);
	    return(PartiallyTangible) b;
	} else
	    return(PartiallyTangible) i;
    }


    public Body getBody(String name) {
	return(Body) getBodies().getViewable(name);
    }


    public Body fetchBody(String name) {
	return(Body) getBodies().fetchViewable(name);
    }

    public String getView(Body from) {
	return "{&" + getName() + "\n{0" + 
	getDesc() + "\n" + 
	"{$" + itemsView(from) + "{^" + 
	bodiesView(from) + "{0";
    }

    public String bodiesView(Body from) {
	String r = "";
	for ( Enumeration enum = getBodies().elements(); enum.hasMoreElements(); ) {
	    Body b = (Body) enum.nextElement();
	    if ( b != from )
		r = r + "\n " + b.getShortDesc();
	}
	if ( r.length() > 1 )
	    r = "\n" + r.substring(1);
	return r;
    }


    public void println(String text) {
	for ( Enumeration enum = getBodies().elements(); enum.hasMoreElements(); )
	    ((Body) enum.nextElement()).println(text);
    }


    public void println(Body from, String f_text, String text) {
	for ( Enumeration enum = getBodies().elements(); enum.hasMoreElements(); ) {
	    Body body = (Body) enum.nextElement();
	    if ( body != from )
		body.println(text);
	    else
		body.println(f_text);
	}
    }


    public void println(Body from, String fromText, Body to, String toText, String obsText) {
	for ( Enumeration enum = getBodies().elements(); enum.hasMoreElements(); ) {
	    Body body = (Body) enum.nextElement();
	    if ( body == from )
		body.println(fromText);
	    else if ( body == to )
		body.println(toText);
	    else
		body.println(obsText);
	}
    }


    public void print(Body src, String src_txt, Body trg, String trg_txt, String oth_txt) {
	for ( Enumeration enum = getBodies().elements(); enum.hasMoreElements(); ) {
	    Body body = (Body) enum.nextElement();
	    if ( body == src )
		src.print(src_txt);
	    else if ( body != trg )
		trg.print(trg_txt);
	    else
		body.print(oth_txt);
	}
    }

    public void triggerOnTick() {
	Jamud.getEventManager().triggerListeners(new ItemEvent(this, JamudEvent.EVENT_TICK, new Object[0]));
    }

    public void tick() {
	//trigger our onTick
	triggerOnTick();

	//tick each of the contained Bodies
	for ( Enumeration enum = getBodies().elements(); enum.hasMoreElements(); )
	    ((Body) enum.nextElement()).tick();
	for ( Enumeration enum = getItems().elements(); enum.hasMoreElements(); )
	    ((Item) enum.nextElement()).tick();

    }


    public Object getSemiClone() {
	Item i = new Item();

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

	return i;
    }


    public static Item loadItem(XMLElement xml) throws Exception {
	if ( ! xml.getName().equals(MARKUP) )
	    throw new Exception("non-" + MARKUP + " element in Item.loadItem");

	Item item = new Item();
	item.setCycFortString( xml.getAttribute(GuidEnabled.PROPERTY_VNUM));
	//item.area = xml.getAttribute(PROPERTY_AREA);
	item.setName(xml.getAttribute(CycThing.PROPERTY_NAME));

	for ( Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
	    XMLElement nxt = (XMLElement) enum.nextElement();
	    String nom = nxt.getName(); nxt.getContent();

	    if ( nom.equals(PartiallyTangible.PROPERTY_DESC) ) {
		item.setShortDesc(nxt.getAttribute(PartiallyTangible.PROPERTY_SHORTDESC));
		item.setDesc(Util.format(nxt.getContent(), 70, true));
	    } else if ( nom.equals(Flagged.PROPERTY_FLAGS) )
		item.setFlags(new FlagList(item,item.makeCycConstant("mudFlag"),item.makeCycConstant("MudFlag"),nxt.getContent()));
	    else if ( nom.equals(PROPERTY_WEARLOCATIONS) )
		item.setWearLocations(new FlagList(item,item.makeCycConstant("mudWearable"),item.makeCycConstant("MudBodyPart"),nxt.getContent()));
	    else if ( nom.equals(PartiallyTangible.PROPERTY_KEYWORDS) )
		item.setKeywords(new FlagList(item,item.makeCycConstant("mudKeyword"),null,nxt.getContent()));
	    else if ( nom.equals(PROPERTY_STATLIST) ) {
		item.skills = new StatTable(nxt.getContent());
	    }
	}

	return item;
    }


    public XMLElement toXMLElement() {
	System.out.println("item");
	XMLElement xml = new XMLElement();
	xml.setName(MARKUP);
	// xml.setAttribute(PROPERTY_AREA, area);
	xml.setAttribute(GuidEnabled.PROPERTY_VNUM, getCycFortString());
	xml.setAttribute(CycThing.PROPERTY_NAME, getName());

	XMLElement nxt;

	System.out.println("item.flags");
	nxt = new XMLElement();
	nxt.setName(PROPERTY_FLAGS);
	nxt.setContent(getFlags().toString());
	xml.addChild(nxt);

	System.out.println("item.keywords");
	nxt = new XMLElement();
	nxt.setName(PartiallyTangible.PROPERTY_KEYWORDS);
	nxt.setContent(getKeywords().toString());
	xml.addChild(nxt);

	System.out.println("item.wearlocationslocations");
	nxt = new XMLElement();
	nxt.setName(PROPERTY_WEARLOCATIONS);
	nxt.setContent(getWearLocations().toString());
	xml.addChild(nxt);

	System.out.println("item.desc");
	nxt = new XMLElement();
	nxt.setName(PartiallyTangible.PROPERTY_DESC);
	nxt.setAttribute(PartiallyTangible.PROPERTY_SHORTDESC, getShortDesc());
	nxt.setContent(getDesc());
	xml.addChild(nxt);

	return xml;
    }

}
