package logicmoo.obj;


import java.util.*;
import logicmoo.*;
import logicmoo.cmd.*;
import logicmoo.obj.event.*;
import logicmoo.plugin.Fight;
import logicmoo.script.*;
import logicmoo.util.*;
import net.n3.nanoxml.*;


public class AgentGeneric extends Artifact  implements Flagged, GuidTracable, JamudEventTrigger {

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

    private Engineer player;
    public Engineer getPlayer() { return player;}
    public void setPlayer(Engineer player) { this.player = player;}


    private StatTable skills = new StatTable();
    public Hashtable getGranted() { return(Hashtable)skills;}
    public int getSkillLevel(String skill) { return skills.stat(skill, 0);}
    public void setSkillLevel(String skill, int level) { skills.set(skill, level);}

    private StatTable stats = new StatTable();
    public StatTable getStats() { return stats;}


    private Vector scripts = new Vector();
    public Vector getScripts() { return scripts;}

    private ReifiableGroup items = new ReifiableGroup();
    public ReifiableGroup getArtifacts() { return items;}

    private Hashtable worn = new Hashtable();
    public Hashtable getWorn() { return worn;}
    public Artifact getWornOn(String location) { return(Artifact) worn.get(location.toUpperCase());}
    public void setWornOn(String location, Artifact i) {
	if ( i == null )
	    worn.remove(location.toUpperCase());
	else
	    worn.put(location.toUpperCase(), i);
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



    public AgentGeneric() {
	super();
    }


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
	getGeographicalRegion().exit(this, this.getName() + " slowly fades from view.");
    }


    public void grant(BodyGranted skill, int level) {
	skills.put(skill.getName().toLowerCase(), new Integer(level));
    }


    public BodyGranted getGranted(String name) {
	name = name.toLowerCase();
	Integer in = (Integer) skills.get(name);

	if ( in != null )
	    return BodyInterpreter.grantedTable.get(name);
	else
	    for ( Enumeration enum = skills.keys(); enum.hasMoreElements(); ) {
		String ns = (String) enum.nextElement();
		if ( ns.startsWith(name) ) return BodyInterpreter.grantedTable.get(ns);
	    }

	return null;
    }


    public Artifact getArtifact(String name) {
	return items.getArtifact(name);
    }


    public Artifact fetchArtifact(String name) {
	return items.fetchArtifact(name);
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
	Jamud.getEventManager().triggerListeners(new AgentGenericEvent(this, AgentGenericEvent.EVENT_TEXT, o));
    }


    public void triggerOnArrive(GeographicalRegion where) {
	Object[] o = { where};
	Jamud.getEventManager().triggerListeners(new AgentGenericEvent(this, AgentGenericEvent.EVENT_ARRIVE, o));
    }


    public void triggerOnDepart(GeographicalRegion where) {
	Object[] o = { where};
	Jamud.getEventManager().triggerListeners(new AgentGenericEvent(this, AgentGenericEvent.EVENT_DEPART, o));
    }


    public void triggerOnEnter(AgentGeneric who) {
	Object[] o = { who};
	Jamud.getEventManager().triggerListeners(new AgentGenericEvent(this, AgentGenericEvent.EVENT_ENTER, o));
    }


    public void triggerOnExit(AgentGeneric who) {
	Object[] o = { who};
	Jamud.getEventManager().triggerListeners(new AgentGenericEvent(this, AgentGenericEvent.EVENT_EXIT, o));
    }


    public void triggerOnTick() {
	Jamud.getEventManager().triggerListeners(new AgentGenericEvent(this, JamudEvent.EVENT_TICK, new Object[0]));
    }


    public void tick() {

	//trigger our event
	triggerOnTick();

	//trigger our items' events
	for ( Enumeration enum = items.elements(); enum.hasMoreElements(); )
	    ((Artifact)enum.nextElement()).tick();

    }


    public AgentGeneric getSemiClone() {

	AgentGeneric n = new AgentGeneric();

	//PartiallyTangible
	n.setName(getName());
	n.setComment(getComment());
	n.setPrettyName(getPrettyName());
	n.setKeywords(new FlagList(getKeywords()));

	//Flagged
	n.flags = (new FlagList(flags));

	//Vnumbered
	n.cycFort = cycFort;

	n.area = area;
	n.gender = gender;
	n.race = race;
	n.job = job;
	n.stats = ((StatTable) stats.clone());
	n.skills = ((StatTable) skills.clone());

	//Scripts
	for ( Enumeration enum = scripts.elements(); enum.hasMoreElements(); )
	    n.scripts.add( ((EventScript)enum.nextElement()).getSemiClone(n) );

	return n;
    }

}
