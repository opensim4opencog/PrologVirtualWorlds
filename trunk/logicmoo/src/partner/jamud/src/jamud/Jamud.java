package jamud;


import jamud.board.BoardManager;
import jamud.channel.ChannelManager;
import jamud.command.InterpreterManager;
import jamud.object.*;
import jamud.plugin.JamudPlugin;
import jamud.util.*;
import java.io.*;
import java.util.*;
import net.n3.nanoxml.*;


/**
 * The main class of the mud. Contains the main(String[]) method used
 * to start everything up. Use currentInstance to obtain a reference
 * to the rest of the currently running MUD.
 *
 * @author siege
 */
public class Jamud implements Initializable {


    /** <code>MARKUP = "JAMUD"</code> */
    public static final String
	MARKUP = "JAMUD";

    
    /** DEFAULT_INI = "jamud.xml" */
    private static final String
	DEFAULT_INI = "jamud.xml";
    

    private static final String
	PROPERTY_TIMEFILE = "TIMEFILE",
	PROPERTY_MESSAGE = "MESSAGE",
	PROPERTY_MESSAGE_NAME = "NAME",
	PROPERTY_MESSAGE_SRC = "SRC";
    

    public static final String
	MESSAGE_TITLE = "jamud.title",
	MESSAGE_ID = "jamud.id",
	MESSAGE_MOTD = "jamud.motd",
	MESSAGE_OPEN = "jamud.openscreen",
	MESSAGE_CLOSE = "jamud.closescreen";



    // the Singleton
    private static Jamud instance;

    public synchronized static final Jamud currentInstance() {

	if(Jamud.instance == null) {
	    Jamud.instance = new Jamud();
	}

	return Jamud.instance;
    }


    
    // the Ticker
    private Ticker ticker;

    
    // a Random seed thingy, just because
    private Random rand;

    public final int randInt() {
	return Math.abs(rand.nextInt());
    }

    public final long randLong() {
	return Math.abs(rand.nextLong());
    }


    // creation manager
    private Creation creation;

    public Creation getCreation() {
	return this.creation;
    }

    public void setCreation(Creation creation) {
	this.creation = creation;
    }


    // the Plugin Manager
    private Set plugins;

    public Iterator plugins() {
	return this.plugins.iterator();
    }

    public void addPlugin(JamudPlugin plugin) {
	plugins.add(plugin);
    }

    public void removePlugin(JamudPlugin plugin) {
	if(plugin.isActive()) {
	    try {
		plugin.terminate();
	    } catch(Exception e) {
		; /* empty */
	    }
	}
	plugins.remove(plugin);
    }


    // Initializable
    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state;
    }
    
        
    // the Player Manager
    private PlayerManager players;

    public PlayerManager playerManager() {
	return this.players;
    }

    
    // the Board and Post Manager
    private BoardManager boards;

    public BoardManager boardManager() {
	return this.boards;
    }

    
    // the Root MudObject
    private MudObjectRoot root;

    public MudObjectRoot mudObjectRoot() {
	return this.root;
    }


    // the Channel Manager
    private ChannelManager channels;

    public ChannelManager channelManager() {
	return this.channels;
    }
    

    // the Interpreter Manager
    private InterpreterManager interpreters;

    public InterpreterManager interpreterManager() {
	return this.interpreters;
    }


    // the Message Table
    private HashMap messages;

    public final void setMessage(String msg_name, String message) {
	if(message == null) {
	    messages.remove(msg_name.toLowerCase());
	} else {
	    messages.put(msg_name.toLowerCase(), message);
	}
    }

    public final String getMessage(String msg_name) {
	return (String) messages.get(msg_name.toLowerCase());
    }

    public final void removeMessage(String msg_name) {
	messages.remove(msg_name.toLowerCase());
    }
    

    // the MUD's runtime in millis
    private /* me love you */ long time;

    public static long getMudTimeMillis() {
	if(Jamud.instance != null) {
	    return Jamud.instance.time;
	} else {
	    return (long) 0;
	}
    }
    

    // location of the file time is stored in
    private String timefile = ".timer";
    
    
    
    // Default Constructor
    private Jamud() {
	this.rand = new Random(System.currentTimeMillis());

	this.messages = new HashMap();

	this.plugins = Collections.synchronizedSet(new HashSet());

	this.players = new PlayerManager();
	this.channels = new ChannelManager();
	this.boards = new BoardManager();
	this.interpreters = new InterpreterManager();

	this.root = new MudObjectRoot();

	this.last_tick = System.currentTimeMillis();
    }



    public synchronized boolean initialize() {
	System.out.println( "begin: Jamud.initialize()" );
	if( this.state > STATE_TERMINATED ) {
	    System.out.println( " not yet terminated" );
	    System.out.println( "end: Jamud.initialize()" );
	    return false;
	} else {
	    this.state = STATE_INITIALIZING;
	}

	// start up plugins
	for(Iterator i = plugins.iterator(); i.hasNext(); ) {
	    JamudPlugin p = (JamudPlugin) i.next();
	    if(p.isAutomatic()) {
		if(! p.initialize() ) {
		    System.out.println(" could not initialize plugin -->"
				       .concat( p.getName() ) );
		}
	    }
	}
	
	// tick the world manually
	tick();

	// start a ticker to tick for us
	TickListener tl = new TickListener() {
		public void tick(Ticker source) {
		    Jamud.currentInstance().tick();
		}
		
		public void tickError(Ticker source, Exception e) {
		    System.err.println("Core Ticker exception:");
		    e.printStackTrace();
		    System.exit(1);
		}
	    };
	ticker = new Ticker(tl, 60000);
	ticker.start();

	// we're running
	this.state = STATE_INITIALIZED;
	System.out.println( "end: Jamud.initialize()" );
	return true;
    }


    public synchronized boolean terminate() {
	System.out.println( "begin: Jamud.terminate()" );
	if( this.state < STATE_INITIALIZED ) {
	    System.out.println( " not yet initialized" );
	    System.out.println( "end: Jamud.terminate()" );
	    return false;
	} else {
	    this.state = STATE_TERMINATING;
	}

	// kill the ticker
	ticker.halt();
	ticker = null;

	// shut down plugins
	for(Iterator i = plugins.iterator(); i.hasNext(); ) {
	    JamudPlugin p = (JamudPlugin) i.next();
	    if(p.isActive()) {
		if(! p.terminate()) {
		    System.out.println(" could not terminate plugin -->"
				       .concat( p.getName() ) );
		}
	    }
	}

	// we're stopped
	this.state = STATE_TERMINATED;
	System.out.println( "end: Jamud.terminate()" );
	return true;
    }
    
    
    private synchronized void load(String iniFile) throws Exception {
	
	IXMLParser xmp = new StdXMLParser();
	xmp.setBuilder(new StdXMLBuilder());
	xmp.setValidator(new NonValidator());
	xmp.setReader(StdXMLReader.fileReader(iniFile));

	iniFile = null;

	XMLElement xml = (XMLElement) xmp.parse();
	xmp = null;
	
	load(xml);
    }
    
    
    public synchronized void load(IXMLElement xml) throws Exception {

	String f = xml.getAttribute( PROPERTY_TIMEFILE, null );
	if(f != null ) {
	    this.timefile = f;
	}

	loadTimeFile();

	Vector v;
	IXMLElement nxt;

       	// 1 -- Message Manager
	v = xml.getChildrenNamed(PROPERTY_MESSAGE);
	if(v.size() > 0) {
	    for(Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
		nxt = (IXMLElement) enum.nextElement();
		    
		String n = nxt.getAttribute(PROPERTY_MESSAGE_NAME, null),
		    s = nxt.getAttribute(PROPERTY_MESSAGE_SRC, null);
		
		if(n != null && n.length() > 0) {
		    if(s == null) {
			setMessage(n, nxt.getContent());
		    } else {
			try{
			    File ifi = new File(s);
			    final int I = (int) ifi.length();
			    byte J[] = new byte[I];
			    FileInputStream in = new FileInputStream(ifi);
			    in.read(J, 0, I);
			    in.close();
			    setMessage( n, new String(J) );
			} catch(Exception e) {
			    System.err.println(e);
			}
		    }
		}
	    }
	}


	// 2 -- Player Manager
	nxt = xml.getFirstChildNamed(PlayerManager.MARKUP);
	if(nxt != null) {
	    players.load(nxt);
	}


	// 3 -- Plugin Manager
	v = xml.getChildrenNamed(JamudPlugin.MARKUP);
	if(v.size() > 0) {
	    for(Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
		nxt = (IXMLElement) enum.nextElement();
		JamudPlugin p = JamudPlugin.loadJamudPlugin(nxt);
		addPlugin( p );
	    }
	}


	// 4 -- Interpreter Manager
	nxt = xml.getFirstChildNamed(InterpreterManager.MARKUP);
	if(nxt != null) {
	    interpreters.load(nxt);
	}


	// 5 -- Jobs, Races, Genders
	v = xml.getChildrenNamed(Job.MARKUP);
	if(v.size() > 0) {
	    for(Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
		nxt = (IXMLElement) enum.nextElement();
		Job.loadJob(nxt);
            }
	}

	v = xml.getChildrenNamed(Race.MARKUP);
	if(v.size() > 0) {
	    for(Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
		nxt = (IXMLElement) enum.nextElement();
		Race.loadRace(nxt);
            }
	}

	v = xml.getChildrenNamed(Gender.MARKUP);
	if(v.size() > 0) {
	    for(Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
		nxt = (IXMLElement) enum.nextElement();
		Gender.loadGender(nxt);
            }
	}
	
	v = null;

		
	// 6 -- Root Mud Object
	nxt = xml.getFirstChildNamed(MudObjectRoot.MARKUP);
	if(nxt != null) {
	    root.load(nxt);
	}
	
    }


    private long last_tick;
    
    public synchronized void tick() {
	System.out.println( "tick..." );

	//1. tick the Root Mud Object
	this.root.tick();
	
	//2. check for inactive players
	this.players.logOutInactives();
	
	//3. recalc the time
	this.saveTimeFile();
	this.last_tick = System.currentTimeMillis();

	System.out.println ("tock!");
    }
    
 
    private void loadTimeFile() {
	File tf = new File( this.timefile );
	if( tf.exists() ) {
	    try {
		FileInputStream in = new FileInputStream( tf );
		final int l = (int) in.available();
		byte[] b = new byte[ l ];
		in.read( b, 0, l );
		in.close();
		String s = new String( b );
		this.time = Long.parseLong( s );
	    } catch(Exception e) {
		saveTimeFile();
	    }
	} else {
	    saveTimeFile();
	}
    }


    private void saveTimeFile() {
	final long diff = (System.currentTimeMillis() - last_tick);
	this.time += diff;

	String s = Long.toString( this.time );

	try {
	    File tf = new File( this.timefile );
	    FileOutputStream out = new FileOutputStream( tf );
	    out.write( s.getBytes() );
	    out.flush();
	    out.close();
	} catch(Exception e) {
	    e.printStackTrace();
	}
    }

    
    /** Start the MUD */
    public synchronized static void main(String[] args) throws Exception {

	String l = (args.length == 0) ?
	    Jamud.DEFAULT_INI :
	    args[0];
	
	try {
	    if(instance == null) {
		instance = new Jamud();
	    }
	    
	    //load me up!
	    instance.load( l );
	    
	    //start me up!
	    instance.initialize();
	    
	} catch(Exception e) {
	    System.err.println( "Jamud failed to load from '"
				.concat( l ).concat( "'" ) );
	    throw e;
	}
    }


    /** Start the MUD directly with the xml ini */
    public synchronized static void main(IXMLElement ini) throws Exception {

	try {
	    if(instance == null) {
		instance = new Jamud();
	    }

	    instance.load( ini );
	    instance.initialize();

	} catch(Exception e) {
	    throw e;
	}
    }
    
}
