package  logicmoo.obj;


import logicmoo.*; 
import logicmoo.obj.event.*;
import logicmoo.script.*;
import logicmoo.util.*;
import java.util.*;
import net.n3.nanoxml.*; 

import org.opencyc.cycobject.*; import logicmoo.*;


public class GeographicalRegion extends PartiallyTangible implements GuidTracable, Flagged, JamudEventTrigger {

    public static final String MARKUP = "GEOGRAPHICALREGION";

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
    public Enumeration enumerateExits() { return getExits().elements();}
    public Vector getExits() { return exits;}
    public void addExit(Exit exit) {
	if ( ! getExits().contains(exit) )
	    getExits().add(exit);
    }
    public void removeExit(Exit exit) { getExits().remove(exit);}
    public void removeExit(String dir) {
	Exit e = getExit(dir.toString());
	if ( e != null )
	    removeExit(e);
    }

    private Vector scripts = new Vector();
    public Vector getScripts() { return scripts;}

    private Group artifacts = new Group();
    public Group getArtifacts() { return artifacts;}

    private ReifiableGroup agents = new ReifiableGroup();
    public ReifiableGroup getAgentGenerics() { return agents;}

    public GeographicalRegion(Microtheory microtheory) {
	this.microtheory = microtheory;
    }

    public void enter(AgentGeneric who) {
	enter(who, who.getName() + " has arrived.");
    }


    public void enter(AgentGeneric who, String msg) {
	who.setGeographicalRegion(this);
	getAgentGenerics().add(who);

	if ( who.getEngineer() != null )
	    who.println(getView(who));

	if ( msg != null )
	    println(who, null, msg);

	// trigger onEnter event on room
	triggerOnEnter(who);

	// trigger onEnter event on agents
	for ( Enumeration enum = getAgentGenerics().elements(); enum.hasMoreElements(); ) {
	    AgentGeneric agent = (AgentGeneric) enum.nextElement();
	    if ( agent != who )
		agent.triggerOnEnter(who);
	}

	// trigger onArrive event on who
	who.triggerOnArrive(this);

    }


    public void triggerOnEnter(AgentGeneric who) {
	Object[] o = { who};
	Jamud.getEventManager().triggerListeners(new GeographicalRegionEvent(this, GeographicalRegionEvent.EVENT_ENTER, o));
    }


    public void triggerOnExit(AgentGeneric who) {
	Object[] o = { who};
	Jamud.getEventManager().triggerListeners(new GeographicalRegionEvent(this, GeographicalRegionEvent.EVENT_EXIT, o));
    }


    public void exit(AgentGeneric who) {
	exit(who, who.getName() + " leaves.");
    }


    public void exit(AgentGeneric who, String msg) {
	if ( msg != null )
	    println(who, null, msg);

	// trigger onExit event on room
	triggerOnExit(who);

	// trigger onExit event on agents
	for ( Enumeration enum = getAgentGenerics().elements(); enum.hasMoreElements(); ) {
	    AgentGeneric agent = (AgentGeneric) enum.nextElement();
	    if ( agent != who )
		agent.triggerOnExit(who);
	}

	// trigger onDepart event on who
	who.triggerOnDepart(this);

	getAgentGenerics().remove(who);
	who.setGeographicalRegion(null);
    }

    public PartiallyTangible getReifiable(String name) {
	return getReifiable(CycMoo.nearestFort("MudReifiable",name));
    }

    public PartiallyTangible getReifiable(CycFort name) {
	Artifact i = getArtifact(name);
	if ( i == null ) {
	    AgentGeneric b = getAgentGeneric(name);
	    if ( b == null ) {
		Exit e = getExit(name.toString());
		return(PartiallyTangible) e;
	    } else
		return(PartiallyTangible) b;
	} else
	    return(PartiallyTangible) i;
    }


    public Artifact getArtifact(String name) {
	return getArtifact(CycMoo.nearestFort("MudArtifact",name));
    }
    
    public Artifact getArtifact(CycFort name) {
	return getArtifacts().getArtifact(name);
    }


    public AgentGeneric getAgentGeneric(CycFort name) {
	return(AgentGeneric) this.getAgentGenerics().getReifiable(name);
    }


    public Artifact fetchArtifact(CycFort name) {
	return getArtifacts().fetchArtifact(name);
    }


    public AgentGeneric fetchAgentGeneric(CycFort name) {
	return(AgentGeneric) getAgentGenerics().fetchReifiable(name);
    }

    public Exit getExit(String name) {
	if ( name == null )
	    return null;
	
	for ( Enumeration enum = getExits().elements(); enum.hasMoreElements(); ) {
	    Exit i = (Exit) enum.nextElement();
	    if ( i.getName().toLowerCase().startsWith(name) )
		return i;
	}
	return null;
    }


    public String getView(AgentGeneric from) {
	return "{&" + getName() + "\n{0" + 
	getComment() + "\n" + 
	exitsView() +
	"{$" + artifactsView(from) + "{^" + 
	agentsView(from) + "{0";
    }


    private String exitsView() {
	String r = "Exits: {&[{0";

	if ( getExits().size() == 0 )
	    r += " none";
	else
	    for ( Enumeration enum = getExits().elements(); enum.hasMoreElements(); )
		r += " " + ((Exit)enum.nextElement()).getName().toLowerCase();

	r += " {&]{0";

	return r;
    }


    private String artifactsView(AgentGeneric from) {
	if ( getArtifacts().size() > 0 )
	    return "\n" + getArtifacts().getPrettyNameView(from, true);
	else
	    return "";
    }


    private String agentsView(AgentGeneric from) {
	String r = "";
	for ( Enumeration enum = getAgentGenerics().elements(); enum.hasMoreElements(); ) {
	    AgentGeneric b = (AgentGeneric) enum.nextElement();
	    if ( b != from )
		r = r + "\n " + b.getPrettyName();
	}
	if ( r.length() > 1 )
	    r = "\n" + r.substring(1);
	return r;
    }


    public void println(String text) {
	for ( Enumeration enum = getAgentGenerics().elements(); enum.hasMoreElements(); )
	    ((AgentGeneric) enum.nextElement()).println(text);
    }


    public void println(AgentGeneric from, String  f_text, String text) {
	for ( Enumeration enum = getAgentGenerics().elements(); enum.hasMoreElements(); ) {
	    AgentGeneric agent = (AgentGeneric) enum.nextElement();
	    if ( agent != from )
		agent.println(text);
	    else
		agent.println(f_text);
	}
    }


    public void println(AgentGeneric from, String fromText, AgentGeneric to, String toText, String obsText) {
	for ( Enumeration enum = getAgentGenerics().elements(); enum.hasMoreElements(); ) {
	    AgentGeneric agent = (AgentGeneric) enum.nextElement();
	    if ( agent == from )
		agent.println(fromText);
	    else if ( agent == to )
		agent.println(toText);
	    else
		agent.println(obsText);
	}
    }


    public void print(AgentGeneric src, String src_txt, AgentGeneric trg, String trg_txt, String oth_txt) {
	for ( Enumeration enum = getAgentGenerics().elements(); enum.hasMoreElements(); ) {
	    AgentGeneric agent = (AgentGeneric) enum.nextElement();
	    if ( agent == src )
		src.print(src_txt);
	    else if ( agent != trg )
		trg.print(trg_txt);
	    else
		agent.print(oth_txt);
	}
    }


    public void triggerOnTick() {
	Jamud.getEventManager().triggerListeners(new GeographicalRegionEvent(this, JamudEvent.EVENT_TICK, new Object[0]));
    }


    public void tick() {

	//trigger our tick event
	triggerOnTick();

	//tick each of the contained Bodies
	for ( Enumeration enum = getAgentGenerics().elements(); enum.hasMoreElements(); )
	    ((AgentGeneric) enum.nextElement()).tick();

    }


}
