package logicmoo;


import logicmoo.net.*;
import logicmoo.obj.*;
import logicmoo.obj.event.JamudEventManager;
import logicmoo.cmd.*;
import logicmoo.plugin.JamudPlugin;
import logicmoo.script.ScriptLanguage;
import logicmoo.util.*;
import java.util.*;
import net.n3.nanoxml.*;


/**
 * The main class of the mud. Contains the main(String[]) method used
 * to start everything up. Should only be instantiated ONCE per JVM
 * as the rest of the mud relies heavily on static calls to this class
 * to interconnect.
 *
 * @author siege
 */
public class Jamud implements TickListener {


    public static final String
    VER_MAJOR = "alpha 0",
    VER_MINOR = "4.4",
    BUILD_VERSION = "20010523";


    private static final String MARKUP = "JAMUD";

    private static final String DEFAULT_INI = "jamud.xml";

    private static final String
    PROPERTY_CHANNELS = "CHANNELS",
    PROPERTY_COMMANDS = "COMMANDS",
    PROPERTY_GENDERS = "GENDERS",
    PROPERTY_JOBS = "JOBS",
    PROPERTY_MESSAGES = "MESSAGES",
    PROPERTY_MESSAGE = "MESSAGE",
    PROPERTY_MESSAGE_NAME = "NAME",
    PROPERTY_MESSAGE_SRC = "SRC",
    PROPERTY_PLUGINS = "PLUGINS",
    PROPERTY_RACES = "RACES",
    PROPERTY_SCRIPTS = "SCRIPTMODS",
    PROPERTY_SKILLS = "SKILLS",
    PROPERTY_SOCIALS = "SOCIALS";

    public static final String
    MESSAGE_MOTD = "logicmoo.motd",
    MESSAGE_OPEN = "logicmoo.openscreen",
    MESSAGE_CLOSE = "logicmoo.closescreen";

    private static Jamud instance;
    public static Jamud currentInstance() { return instance;}

    private static Ticker ticker = null;

    private static Random rand = new Random(System.currentTimeMillis());
    public static int randInt() { return Math.abs(rand.nextInt());}
    public static long randLong() { return Math.abs(rand.nextLong());}

    private static String title;
    public static String getTitle() { return title;}

    private static String id;
    public static String getId() { return id;}

    private static JamudEventManager eventMan = new JamudEventManager();
    public static JamudEventManager getEventManager() { return eventMan;}

    private static Vector plugins = new Vector();
    public static Vector getPlugins() { return plugins;}
    public static void addPlugin(JamudPlugin plugin) {
	if ( ! plugins.contains(plugin) )
	    plugins.add(plugin);
    }
    public static void removePlugin(JamudPlugin plugin) {
	if ( plugin.isActive() ) {
	    try { 
		plugin.terminate();
	    } catch ( Exception e ) {	 /* empty */
	    }
	}
	plugins.remove(plugin);
    }

    private static Vector scripts = new Vector();
    public static Vector getScriptLanguages() { return scripts;}
    public static ScriptLanguage getScriptLanguage(String name) {
	for ( Enumeration enum = scripts.elements(); enum.hasMoreElements(); ) {
	    ScriptLanguage sl = (ScriptLanguage) enum.nextElement();
	    if ( sl.getName().equalsIgnoreCase(name) )
		return sl;
	}
	return null;
    }

    private static PlayerManager playerMan;
    public static PlayerManager getPlayerManager() { return playerMan;}

    private static PostManager postMan;
    public static PostManager getPostManager() { return postMan;}

    private static AreaManager areaMan;
    public static AreaManager getAreaManager() { return areaMan;}

    //		private static AffectManager affMan;
    //			public static AffectManager getAffectManager() { return affMan; }

    private static HelpManager helpMan;
    public static HelpManager getHelpManager() { return helpMan;}

    private static Vector channels = new Vector();
    public static Vector getChannelManager() { return channels;}

    private static Vector socials = new Vector();
    public static Vector getSocialManager() { return socials;}

    private static Vector genders = new Vector();
    public static Vector getGenderManager() { return genders;}

    private static Vector races = new Vector();
    public static Vector getRaceManager() { return races;}

    private static Vector jobs = new Vector();
    public static Vector getJobManager() { return jobs;}

    private static Hashtable messages = new Hashtable();
    public static void putMessage(String msg_name, String message) { messages.put(msg_name.toLowerCase(), message);}
    public static String getMessage(String msg_name) { return(String) messages.get(msg_name.toLowerCase());}
    public static void removeMessage(String msg_name) { messages.remove(msg_name.toLowerCase());}

    protected static long time;
    protected static String timefile;



    public Jamud(String ini){
	try {
	    loadJamud(ini);

	    for ( int I=0; I<plugins.size(); I++ ) {
		JamudPlugin p = (JamudPlugin) plugins.get(I);
		try {
		    if ( p.isAutomatic() )
			p.initialize();
		} catch ( Exception e ) {
		    e.printStackTrace();
		}
	    }

	    tick();

	    ticker = new Ticker(this, 60000);
	    ticker.start();
	} catch ( Exception a ) {
	    System.err.println("Exception while initializing");
	    a.printStackTrace();
	    System.err.println("Halting...");
	    // System.exit(1);
	}
    }



    private void loadJamud(String iniFile) throws Exception {
	// Load the xml ini file
	System.out.println("Loading JaMUD from " + iniFile);

	IXMLParser xmp = new StdXMLParser();
	xmp.setBuilder(new StdXMLBuilder());
	xmp.setValidator(new NonValidator());
	xmp.setReader(StdXMLReader.fileReader(iniFile));
	iniFile = null;
	XMLElement xml = (XMLElement) xmp.parse();
	xmp = null;

	loadJamud(xml);
    }


    private void loadJamud(XMLElement xml) throws Exception {

	if ( ! xml.getName().equals(MARKUP) )
	    throw new Exception("non-" + MARKUP + " element in Jamud.loadJamud");

	title = xml.getAttribute("TITLE", "The Unnamed MUD");
	id = xml.getAttribute("ID", "unknown");

	System.out.print(" Player Manager Initializing...");
	playerMan = new PlayerManager(xml.getAttribute("PLAYER"));
	System.out.println(" done");

	Vector v;

	v = xml.getChildrenNamed(PostManager.MARKUP);
	if ( v.size() > 0 ) {
	    System.out.print(" Post Manager Initializing...");
	    postMan = new PostManager( (XMLElement) v.get(0) );
	    System.out.println(" done");
	}

	v = xml.getChildrenNamed(HelpManager.MARKUP);
	if ( v.size() > 0 ) {
	    System.out.print(" Help Manager Initializing...");
	    helpMan = new HelpManager( (XMLElement) v.get(0) );
	    System.out.println(" done");
	}

	v = xml.getChildrenNamed(PROPERTY_CHANNELS);
	if ( v.size() > 0 ) {
	    System.out.print(" Loading Channels...");
	    for ( Enumeration enum = ( (XMLElement) v.get(0) ).enumerateChildren(); enum.hasMoreElements(); )
		channels.add(Channel.loadChannel((XMLElement) enum.nextElement()));
	    System.out.println(" done");
	}


	v = xml.getChildrenNamed(PROPERTY_SCRIPTS);
	if ( v.size() > 0 ) {
	    System.out.print(" Loading Script Languages...");
	    for ( Enumeration enum = ( (XMLElement) v.get(0) ).enumerateChildren(); enum.hasMoreElements(); )
		scripts.add(ScriptLanguage.loadScriptLanguage((XMLElement) enum.nextElement()));
	    System.out.println(" done");
	}

	/*
		v = xml.getChildrenNamed(PROPERTY_COMMANDS);
		if(v.size() > 0) {
			System.out.print(" Command Interpreter Loading...");
			for(Enumeration enum = ( (XMLElement) v.get(0) ).enumerateChildren(); enum.hasMoreElements(); )
			    CommandInterpreter.put(Command.loadCommand((XMLElement) enum.nextElement()));
			System.out.println(" done");			
		}
	
		v = xml.getChildrenNamed(PROPERTY_SKILLS);
		if(v.size() > 0) {
			System.out.print(" Skill Interpreter Loading...");
			for(Enumeration enum = ( (XMLElement) v.get(0) ).enumerateChildren(); enum.hasMoreElements(); )
			    SkillInterpreter.put(Skill.loadSkill((XMLElement) enum.nextElement()));
			System.out.println(" done");			
		}
	*/

	/*
		v = xml.getChildrenNamed(PROPERTY_AFFECTS);
		if(v.size() > 0) {
		      System.out.print(" Affect Manager Initializing...");
		    affMan = new AffectManager( (XMLElement) v.get(0) );
		      System.out.println(" done");		
		}
	*/

	v = xml.getChildrenNamed(PROPERTY_PLUGINS);
	if ( v.size() > 0 ) {
	    System.out.print(" Plugin Manager Initializing...");
	    for ( Enumeration enum = ( (XMLElement) v.get(0) ).enumerateChildren(); enum.hasMoreElements(); )
		addPlugin(JamudPlugin.loadJamudPlugin((XMLElement) enum.nextElement()));
	    System.out.println(" done");
	}

	v = xml.getChildrenNamed(PROPERTY_JOBS);
	if ( v.size() > 0 ) {
	    System.out.print(" Loading Jobs...");
	    for ( Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
		XMLElement nxt = (XMLElement) enum.nextElement();
		String src = nxt.getAttribute("SRC", null);
		if ( src != null ) {
		    IXMLParser xmp = new StdXMLParser();
		    xmp.setBuilder(new StdXMLBuilder());
		    xmp.setValidator(new NonValidator());
		    xmp.setReader(StdXMLReader.fileReader(src));
		    nxt = (XMLElement) xmp.parse();
		    xmp = null;
		    if ( ! nxt.getName().equals(PROPERTY_JOBS) )
			throw new Exception("non-" + PROPERTY_JOBS + " element loading jobs from " + src);
		}
		for ( Enumeration renum = nxt.enumerateChildren(); renum.hasMoreElements(); )
		    jobs.add( Job.loadJob( (XMLElement) renum.nextElement() ) );
	    }
	    System.out.println(" done");
	}

	v = xml.getChildrenNamed(PROPERTY_RACES);
	if ( v.size() > 0 ) {
	    System.out.print(" Loading Races...");
	    for ( Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
		XMLElement nxt = (XMLElement) enum.nextElement();
		String src = nxt.getAttribute("SRC", null);
		if ( src != null ) {
		    IXMLParser xmp = new StdXMLParser();
		    xmp.setBuilder(new StdXMLBuilder());
		    xmp.setValidator(new NonValidator());
		    xmp.setReader(StdXMLReader.fileReader(src));
		    nxt = (XMLElement) xmp.parse();
		    xmp = null;
		    if ( ! nxt.getName().equals(PROPERTY_RACES) )
			throw new Exception("non-" + PROPERTY_RACES + " element loading races from " + src);
		}
		for ( Enumeration renum = nxt.enumerateChildren(); renum.hasMoreElements(); )
		    races.add( Race.loadRace( (XMLElement) renum.nextElement() ) );
	    }
	    System.out.println(" done");
	}

	v = xml.getChildrenNamed(PROPERTY_GENDERS);
	if ( v.size() > 0 ) {
	    System.out.print(" Loading Genders...");
	    for ( Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
		XMLElement nxt = (XMLElement) enum.nextElement();
		String src = nxt.getAttribute("SRC", null);
		if ( src != null ) {
		    IXMLParser xmp = new StdXMLParser();
		    xmp.setBuilder(new StdXMLBuilder());
		    xmp.setValidator(new NonValidator());
		    xmp.setReader(StdXMLReader.fileReader(src));
		    nxt = (XMLElement) xmp.parse();
		    xmp = null;
		    if ( ! nxt.getName().equals(PROPERTY_GENDERS) )
			throw new Exception("non-" + PROPERTY_GENDERS + " element loading genders from " + src);
		}
		for ( Enumeration renum = nxt.enumerateChildren(); renum.hasMoreElements(); )
		    genders.add(Gender.loadGender((XMLElement)renum.nextElement()));
	    }
	    System.out.println(" done");
	}

	v = xml.getChildrenNamed(PROPERTY_SOCIALS);
	if ( v.size() > 0 ) {
	    System.out.print(" Loading Socials...");
	    for ( Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
		XMLElement nxt = (XMLElement) enum.nextElement();
		String src = nxt.getAttribute("SRC", null);
		if ( src != null ) {
		    IXMLParser xmp = new StdXMLParser();
		    xmp.setBuilder(new StdXMLBuilder());
		    xmp.setValidator(new NonValidator());
		    xmp.setReader(StdXMLReader.fileReader(src));
		    nxt = (XMLElement) xmp.parse();
		    xmp = null;
		    if ( ! nxt.getName().equals(PROPERTY_SOCIALS) )
			throw new Exception("non-" + PROPERTY_SOCIALS + " element loading socials from " + src);
		}
		for ( Enumeration renum = nxt.enumerateChildren(); renum.hasMoreElements(); )
		    socials.add( Social.loadSocial( (XMLElement) renum.nextElement() ) );
	    }
	    System.out.println(" done");
	}

	v = xml.getChildrenNamed(AreaManager.MARKUP);
	if ( v.size() > 0 ) {
	    System.out.print(" Area Manager Initializing...");
	    areaMan = new AreaManager( (XMLElement) v.get(0) );
	    System.out.println(" done");
	}

	v = xml.getChildrenNamed(PROPERTY_MESSAGES);
	if ( v.size() > 0 ) {
	    System.out.print(" Loading Messages...");
	    for ( Enumeration enum = v.elements(); enum.hasMoreElements(); )
		for ( Enumeration renum = ((XMLElement)enum.nextElement()).enumerateChildren(); renum.hasMoreElements(); ) {
		    XMLElement nxt = (XMLElement) renum.nextElement();

		    if ( ! nxt.getName().equals(PROPERTY_MESSAGE) )
			throw new Exception("non-" + PROPERTY_MESSAGE + " element loading messages");

		    String n = nxt.getAttribute(PROPERTY_MESSAGE_NAME),
			       s = nxt.getAttribute(PROPERTY_MESSAGE_SRC, null);

		    if ( n != null ) {
			if ( s == null )
			    putMessage(n, nxt.getContent());
			else
			    putMessage(n, FileAccess.loadFile(s));
		    }
		}
	    System.out.println(" done");
	}

     //   beanShell = CycMoo.getBeanShell();

    }

    public static bsh.Interpreter beanShell =null;


    public void tick() {
	//1. tick all the areas
	for ( Enumeration enum = areaMan.elements(); enum.hasMoreElements(); )
	    ((Area) enum.nextElement()).tick();

	//2. check for player inactivity
	long nt = System.currentTimeMillis();
	for ( Enumeration enum = playerMan.localElements(); enum.hasMoreElements(); ) {
	    Player p = ((PlayerMask) enum.nextElement()).getPlayer();
	    if ( p.getConnection() == null && (nt - p.getLastCommandAt()) >= ticker.getInterval() )
		p.logOut();
	}

	//3. recalc the time
	/* Haven't got this yet. Plan on something nifty configurable */
    }


    public static Room defaultRoom = null;
    public static Area defaultArea = null;

    public void tick(Ticker source) {
	tick();
    }


    public void tickError(Ticker source, Exception e) {
	System.err.println("Ticker exception:");
	System.err.println(e);
	System.exit(1);
    }



    public static void main(String[] args) {

	System.setErr(System.out);    // only while in ALPHA. This is for debugging
	// only so we have an easy way to redirect
	// error output to a file

	if ( args.length == 0 )
	    instance = new Jamud(Jamud.DEFAULT_INI);
	else
	    instance = new Jamud(args[0]);

    }


}
