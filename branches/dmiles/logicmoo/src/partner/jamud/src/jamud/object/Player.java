package jamud.object;


import jamud.*;
import jamud.channel.*;
import jamud.command.*;
import jamud.object.event.*;
import jamud.util.*;
import java.io.*;
import java.util.Iterator;
import net.n3.nanoxml.*;


public class Player extends PlayerMask implements Initializable, Actor {


    /** MARKUP = "PLAYER" */
    public static final String MARKUP = "PLAYER";


    private static final String
	PROPERTY_TRUST = "TRUST",
	PROPERTY_INFO = "INFO",
	PROPERTY_TITLE = "TITLE",
	PROPERTY_NAME = "NAME",
	PROPERTY_FLAGS = "FLAGS",
	PROPERTY_CHANNELS = "CHANNELS",
	PROPERTY_GRANTED = "GRANTED",
	PROPERTY_PASSWORD = "PASSWORD";


    public static final String
	ATTRIB_PROMPT = "PROMPT",
	ATTRIB_EMAIL = "EMAIL",
	ATTRIB_LAG = "LAG";


    public static final int
	LOGOUT_NORMAL = 0,    // through the quit command, etc
	LOGOUT_BOOTED = 1,    // happens when manually booted
	LOGOUT_LINKDEATH = 2, // happends when the PlayerManager boots you
	LOGOUT_ERROR = 3;     // connection broken


    public static final String
	MESSAGE_LOGIN_NAME = "player.login.name",
	MESSAGE_LOGIN_PASSWORD = "player.login.password";


    // the tick listener
    private TickListener ticklistener;

    // the ticker
    private Ticker ticker;


    private final void _tick(Ticker src) {
	String cmd = readln();
	if(cmd != null) {
	    //execute the command
	    this.lastcmdat = System.currentTimeMillis();
	    this.enact( cmd );
	}
    }

    private final void _tickError(Ticker src, Exception e) {
	e.printStackTrace();
	if(this.state == STATE_TERMINATED) {
	    this.tickinit();
	}
    }


    // The Event Register (JamudEventTrigger)
    private EventRegister events = new EventRegister();

    public boolean addListener(JamudEventListener listener,
			    Class eventClass) {
	return events.addListener(listener, eventClass);
    }

    public boolean removeListener(JamudEventListener listener) {
	return events.removeListener(listener);
    }

    public boolean removeListener(JamudEventListener listener,
			       Class eventClass) {
	return events.removeListener(listener, eventClass);
    }

    public boolean removeListeners() {
	return events.removeListeners();
    }

    public boolean removeListeners(Class eventClass) {
	return events.removeListeners(eventClass);
    }

    public void triggerListeners(JamudEvent jEvent) {
	this.events.triggerListeners(jEvent);
    }    


    // source file
    // PlayerManager.getPath().concat(this.name)
    private String src;

    public String getSource() {
	return this.src;
    }

    public void setSource(String src) {
	this.src = src;
    }


    // flags
    private FlagList flags;

    public FlagList flags() {
	return this.flags;
    }

    public boolean isFlagged(String flag) {
	return this.flags.contains(flag);
    }


    // random attributes, just in case
    private Attributes attribs;

    public Attributes attributes() {
	return this.attribs;
    }


    // the mudobject using as a body
    private MudObject body;

    public MudObject getBody() {
	return this.body;
    }

    public void setBody(MudObject body) {
	this.body = body;
    }


    // native body, always the same
    // tid: this.name.concat("-body");
    private MudObject nativebody;

    public MudObject nativeBody() {
	return this.nativebody;
    }

    public synchronized MudObject obtainNativeBody() {
	if(this.nativebody == null) {
	    String tid = this.name.concat("-body");
	    MudObject b = MudObject.getMudObjectTemplate(tid);
	    if( b == null ) {
		try {
		    b = new MudObject();
		    b.setTemplateId( tid );
		    b.isTemplate( true );
		    b.setName( this.name );
		    b.getKeywords().add( this.name );
		    b.setDescription( "Look, it's ".concat(this.name) );
		    b.setShortDescription( this.name );
		    b.setSource( this.src.concat(".body") );
		} catch( TemplateConflictException tce) {
		    ; //if this happened, something is wrong.
		}
	    }
	    this.nativebody = b;
	}
	return this.nativebody;
    }


    // home, no place like it.
    // tid: this.name.concat("-home");
    private MudObject home;

    public MudObjectContainer home() {
	if(home == null) {
	    return null;
	} else {
	    return home.childContainer();
	}
    }

    public synchronized MudObjectContainer obtainHome() {
	if(this.home == null) {
	    String tid = this.name.concat("-home");
	    MudObject m = MudObject.getMudObjectTemplate(tid);
	    if( m == null ) {
		try {
		    m = new MudObject();
		    m.setTemplateId( tid );
		    m.isTemplate( true );
		    m.setName( tid );
		    m.setDescription( tid );
		    m.setShortDescription( tid );
		    m.setSource( this.src.concat(".home") );
		} catch(TemplateConflictException tce) {
		    ; //wrong wrong wrong!
		}
	    }
	    this.home = m;
	}
	if( this.home == null ) {
	    return null;
	} else {
	    return this.home.childContainer();
	}
    }


    // PlayerMask: name
    private String name = "";

    public String getShortName() {
	return this.name;
    }

    public void setShortName(String name) {
	this.name = name;
    }


    // PlayerMask: title (similar to sdesc)
    private String title = "";

    public String getTitle() {
	return this.title;
    }

    public void setTitle(String title) {
	this.title = title;
    }


    // PlayerMask: info (similar to desc)
    private String info = "";

    public String getInfo() {
	return this.info;
    }

    public void setInfo(String info) {
	this.info = info;
    }


    // MD5-sum in hex of this.name.concat(passphrase).getBytes()
    private String pass;

    public boolean verifyPassword(String test) {
	if( test != null && this.pass != null ) {
	    MD5 m = new MD5( this.name.concat( test ) );
	    test = m.asHex();
	    m = null;
	    return test.equals( this.pass );
	} else {
	    return (test == null || test.length() == 0) && 
		(this.pass == null || this.pass.length() == 0);
	}
    }

    public boolean setPassword(String oldpass, String newpass) {
	MD5 m = new MD5();

	// first we check the oldpass for validity
	if( oldpass != null && this.pass != null ) {
	    m.Init();
	    m.Update( this.name.concat( oldpass ) );
	    oldpass = m.asHex();
	    if(! oldpass.equals( this.pass ) ) {
		return false;
	    }
	} else if( oldpass != this.pass ) {
	    return false;
	}

	// and if we make it this far, we can change the pass
	if( newpass != null ) {
	    m.Init();
	    m.Update( this.name.concat( newpass ) );
	    this.pass = m.asHex();
	} else {
	    this.pass = null;
	}

	return true;
    }


    // for ACLs, and commands
    private int trust;

    public int getTrust() {
	return this.trust;
    }

    public void setTrust(int trust) {
	this.trust = trust;
    }


    // Initialized
    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state;
    }


    // Connection stuff
    private Connection con;

    public Connection getConnection() {
	return this.con;
    }


    // Granted Commands
    private FlagList granted;

    public FlagList grantedCommands() {
	return this.granted;
    }

    public boolean isGranted(String command) {
	return this.granted.contains( command );
    }


    // Subscribed Channels
    private FlagList channels;

    public FlagList subscribedChannels() {
	return this.channels;
    }


    // Player's Command Queue
    private Queue commandBuffer;

    public void enqueueCommand(String command) {
	System.out.println( "called: Player.enqueueCommand(\""
			    .concat(command).concat("\")") );
	commandBuffer.put(command);
    }


    // see: _tick(Ticker) for where this is updated
    private long lastcmdat;

    public long lastCommandAt() {
	return this.lastcmdat;
    }



    public Player() {
	this.granted = new FlagList();
	this.channels = new FlagList();
	this.commandBuffer = new Queue();
	this.flags = new FlagList();
	this.attribs = new Attributes();
    }


    public Player(Connection con) {
	this();

	this.con = con;
	con.setPlayer(this);
    }



    /** enact a command upon this Player.
     * command will be broken into command and arguments,
     * and the interpreters will be polled for a match.
     */
    public synchronized void enact(String command) {
	if(this.state < STATE_INITIALIZED) {
	    return;
	}

	if( command != null && (command=command.trim()).length() > 0 ) {

	    //split the command into command and arguments
	    String[] cmdsplit = InterpreterManager.commandSplit(command);

	    InterpreterManager im = Jamud.currentInstance()
		.interpreterManager();

	    Command c = im.completeCommand( cmdsplit[0], this );
	    if( c != null ) {
		try {
		    c.execute( this, cmdsplit[1] );
		} catch(CommandException ce) {
		    ce.printStackTrace();
		    this.println( "Huh?" );
		    this.ready();
		} catch(Exception e) {
		    //TODO: special logging should go here
		    e.printStackTrace();
		    this.println( "Huh?" );
		    this.ready();	    
		}

	    } else {
		// TODO: look up message
		this.println( "Huh?" );
		this.ready();
	    }

	} else {

	    // empty command
	    this.ready();
	}
    }


    /** print the Player's prompt.
     * it is important to note that the enact method itself does not call
     * ready. Rather, the commands executed (if any) will normally choose
     * to print the prompt when they have finished execution.
     */
    public synchronized void ready() {
	if(this.state < STATE_INITIALIZED) {
	    return;
	}

	String p = attribs.getAttribute( ATTRIB_PROMPT, null );

	if( p != null ) {
	    /* TODO: parse prompt variables */
	    //p = 

	} else {
	    p = ( ( this.name == null )
		  ? "[jamud]$ "
		  : "[".concat(this.name).concat("]$ ")
		  );
	}

	this.print( "\n".concat(p) );
    }


    private void tickinit() {
	System.out.println( "begin: Player.tickinit()");

	// kill the old ticker, if there is one
	if(this.ticker != null) {
	    this.ticker.halt();
	}

	// if there isn't a ticklistener, make one
	if(this.ticklistener == null) {
	    this.ticklistener = new TickListener() {
		    public void tick(Ticker src) {
			_tick(src);
		    }
		    public void tickError(Ticker src, Exception e) {
			_tickError(src, e);
		    }
		};
	}

	final long lag = this.attribs.getAttribute( ATTRIB_LAG, (long) 200 );
	this.ticker = new Ticker(ticklistener, lag );
	this.ticker.start();

	System.out.println( "end: Player.tickinit()");
    }


    /**
     * begin the login process for this Player. Called normally immediately
     * after construction with a working Connection.
     */
    public synchronized boolean initialize() {
	System.out.println("begin: Player.initialize()");
	
	if( this.state > STATE_TERMINATED ) {
	    System.out.println("end: Player.initialize() [false]");
	    return false;
	} else {
	    this.state = STATE_INITIALIZING;
	}

	Jamud jamud = Jamud.currentInstance();

	println( jamud.getMessage( Jamud.MESSAGE_OPEN ) );

	// temporary string, it's all over the place
	String t;

	//prompt username
	if( (t=jamud.getMessage(MESSAGE_LOGIN_NAME)) == null ) {
	    t = "Player Name: ";
	}
	this.name = this.prompt( t ).toLowerCase();

	boolean exists = true;

	try {

	    // resolve our playerfile from the path and our name
	    t = jamud.playerManager().getPath();
	    this.src = t.concat(this.name);

	    // load from playerfile
	    this.load();

	} catch(IOException ioe) {

	    // no pfile found, will run creation eventually
	    exists = false;

	} catch(Exception e) {

	    // uh oh.
	    e.printStackTrace();
	    System.out.println("end: Player.initialize() [false]");
	    return false;
	}

	// if the above block went well...
	if(exists) {

	    // prompt for password
	    if( (t=jamud.getMessage(MESSAGE_LOGIN_PASSWORD)) == null ) {
		t = "Password: ";
	    }
	    t = this.prompt( t );

	    // verify password
	    if( ! this.verifyPassword( t ) ) {
		System.out.println("end: Player.initialize() [false]");
		return false;
	    }

	    // find a previous instance of this mask by name
	    PlayerMask prev = jamud.playerManager()
		.getPlayerMask(this.getName());

	    // is this player already logged in?
	    if(prev != null && prev instanceof Player) {

		// if so, reconnect them with OUR connection
		Player p = (Player) prev;
		p.reconnect( this.con );
		this.con = null;
		this.terminate();

		System.out.println("end: Player.initialize() [true]");
		return true;

	    } else {
		// otherwise, log this one all the way in
		final boolean b = this.logIn();
		System.out.println("end: Player.initialize() "
				   .concat( b ? "[true]" : "[false]" ) );
		return b;
	    }

	} else {
	    // couldn't find the pfile, so let's make one
	    final boolean b = this.creation();
	    System.out.println("end: Player.initialize() "
			       .concat( b ? "[true]" : "[false]" ) );
	    return b;
	}
    }


    /** calls terminate(LOGOUT_NORMAL) */
    public synchronized boolean terminate() {
	System.out.println("begin: Player.terminate()");

	this.terminate( LOGOUT_NORMAL );

	System.out.println("end: Player.terminate()");
	return true;
    }


    /** terminates Player in a manner corresponding to the reason */
    public synchronized boolean terminate(int reason) {
	System.out.println( "begin: Player.terminate("
			    .concat(Integer.toString(reason))
			    .concat(")") );

	this.state = STATE_TERMINATING;

	if(reason != LOGOUT_ERROR) {

	    // remove from playermanager
	    Jamud jamud = Jamud.currentInstance();
	    jamud.playerManager().logPlayerMaskOut( this, reason );

	    try {
		this.save();
	    } catch(Exception e) {
		e.printStackTrace();
	    }

	    // stop referencing thread
	    if(this.ticker != null) {
		this.ticker.halt();
		this.ticker = null;
	    }
	    
	    // don't worry about events we're subscribed to,
	    // they'll happen on finalize()
	    
	}

	if(this.con != null) {
	    System.out.println(" disconnecting");
	    this.con.disconnect();
	    this.con = null;
	}

	this.state = STATE_TERMINATED;

	System.out.println( "end: Player.terminate("
			    .concat(Integer.toString(reason))
			    .concat(")") );
	return true;
    }


    private boolean logIn() {
	System.out.println ("begin: Player.logIn()");

	Jamud jamud = Jamud.currentInstance();

	jamud.playerManager().logPlayerMaskIn( this );

	// subscribe channels
	for(Iterator i = channels.iterator(); i.hasNext(); ) {
	    String n = (String) i.next();
	    Channel c = (Channel) jamud.channelManager().getChannel( n );
	    if(c != null) {
		try {
		    c.subscribe( this );
		} catch(InsufficientTrustException ite) {
		    channels.remove( n );
		}
	    }
	}

	// placement of body, subscription to events
	if(this.body == null) {
	    this.body = this.nativebody;
	}
	if(this.body != null) {
	    MudObjectContainer room = this.body.getParentContainer();
	    if(room == null) {
		jamud.mudObjectRoot().addChildObject( this.body );
	    } else {
		room.addChildObject( this.body );
	    }
	    this.body.addListener( this, PrintEvent.class );
	}
	
	// we're good to go
	this.state = STATE_INITIALIZED;
	
	// print the motd
	print( jamud.getMessage( Jamud.MESSAGE_MOTD ) );
	
	if(this.body != null) {
	    // take a look around
	    this.body.enact("look");
	    
	} else {
	    // print our prompt
	    ready();
	}

	// start our ticker, and we can begin command execution
	this.tickinit();

	System.out.println("end: Player.logIn()");
	return true;
    }


    private boolean creation() {
	System.out.println( "begin: Player.creation()" );

	Creation creation = Jamud.currentInstance().getCreation();
	if( creation != null) {
	    try {
		System.out.println( " enacting Creation" );
		creation.enact( this );
		try {
		    this.save();
		} catch(Exception e) {
		    throw new CreationException(e);
		}
		
		final boolean b = this.logIn();
		System.out.println( "end: Player.creation() "
				    .concat( b ? "[true]" : "[false]" ) );
		return b;

	    } catch(CreationException ce) {
		ce.printStackTrace();
		System.out.println("end: Player.creation() [false]");
		return false;
	    }

	} else {

	    // no creation
	    System.out.println(" no creation");
	    System.out.println("end: Player.creation() [false]");
	    return false;
	}
    }


    /** sets up this player to use the passed connection */
    public synchronized boolean reconnect(Connection connection) {
	System.out.println("begin: Player.reconnect(Connection)");
	this.state = STATE_INITIALIZING;

	if(this.con != null) {
	    this.con.disconnect();
	}
	connection.setPlayer(null);
	connection.setPlayer(this);
	this.con = connection;

	this.state = STATE_INITIALIZED;

	this.println(" ... reconnected");
	this.ready();

	System.out.println("end: Player.reconnect(Connection)");
	return true;
    }


    public synchronized void eventTriggered(JamudEvent event) {

	if(state < STATE_INITIALIZED) {
	    return;
	}

	if(event instanceof ChannelEvent) {

	    // channel stuff
	    ChannelEvent c = (ChannelEvent) event;
	    this.println( c.getChannel().parseDisplay( c.getSource(),
						       c.getText() ) );
	    this.ready();

	} else if(event instanceof TellEvent) {

	    // tell stuff
	    TellEvent t = (TellEvent) event;
	    this.println( t.getSource().getName().concat(" tells you, \"")
			  .concat( t.getText() ).concat("\"") );
	    this.ready();

	} else if(event instanceof PrintEvent) {

	    // just print it!
	    String text = ((PrintEvent)event).getText();
	    this.print( text );
	    this.ready();

	}
    }


    public void print(String text) {
	if(text != null) {
	    this.triggerListeners( new PrintEvent(this, text) );
	}
    }


    public void println(String text) {
	if(text != null) {
	    this.triggerListeners( new PrintEvent(this, text.concat("\n")) );
	}
    }


    public String prompt(String text) {
	if(text != null) {
	    this.triggerListeners( new PrintEvent(this, text) );
	}
	return this.readln();
    }


    public String readln() {
	if(this.commandBuffer == null) {
	    return null;
	} else {
	    return (String) this.commandBuffer.get();
	}
    }


    public synchronized void load() throws Exception {
	System.out.println("begin: Player.load()");

	IXMLParser xmp = new StdXMLParser();
	xmp.setBuilder( new StdXMLBuilder() );
	xmp.setValidator( new NonValidator() );
	xmp.setReader( StdXMLReader.fileReader( this.src ) );

	IXMLElement xml = (IXMLElement) xmp.parse();
	xmp = null;

	this.load(xml);

	System.out.println("end: Player.load()");
    }


    public synchronized void load(IXMLElement xml) throws Exception {
	System.out.println("begin: Player.load(IXMLElement)");
	
	Attributes at = new Attributes(xml.getAttributes());
	this.name = at.getAttribute( PROPERTY_NAME, this.name );
	this.trust = at.getAttribute( PROPERTY_TRUST, 1 );

	IXMLElement nxt;

	nxt = xml.getFirstChildNamed( PROPERTY_INFO );
	if(nxt != null) {
	    this.title = nxt.getAttribute( PROPERTY_TITLE, this.title );
	    String t = nxt.getContent();
	    if(t != null) {
		this.info = t;
	    }
	}

	nxt = xml.getFirstChildNamed( PROPERTY_PASSWORD );
	if(nxt != null) {
	    this.pass = nxt.getContent();
	}

	nxt = xml.getFirstChildNamed( Attributes.MARKUP );
	if(nxt != null) {
	    this.attribs.load( nxt );
	}

	nxt = xml.getFirstChildNamed( PROPERTY_FLAGS );
	if(nxt != null) {
	    String t = nxt.getContent();
	    if(t != null) {
		this.flags.refresh( t );
	    }
	}

	nxt = xml.getFirstChildNamed( PROPERTY_GRANTED );
	if(nxt != null) {
	    String t = nxt.getContent();
	    if(t != null) {
		this.granted.refresh( t );
	    }
	}

	nxt = xml.getFirstChildNamed( PROPERTY_CHANNELS );
	if(nxt != null) {
	    String t = nxt.getContent();
	    if(t != null) {
		this.channels.refresh( t );
	    }
	}

	// attempt to find Home
	MudObject m = MudObject
	    .getMudObjectTemplate( this.name.concat("-home") );
	if( m == null ) {
	    nxt = xml.getFirstChildNamed( "HOME" );
	    if(nxt != null) {
		m = MudObject.loadMudObject( nxt );
		this.home = m;
		Jamud.currentInstance().mudObjectRoot().addChildObject( m );
	    }
	} else {
	    this.home = m;
	}

	// attempt to find Native Body
	m = MudObject.getMudObjectTemplate( this.name.concat("-body") );
	if( m == null ) {
	    nxt = xml.getFirstChildNamed( "BODY" );
	    if(nxt != null) {
		m = MudObject.loadMudObject( nxt );
		this.nativebody = m;
		if(this.home != null) {
		    this.home.childContainer().addChildObject( m );
		}
	    }
	} else {
	    this.nativebody = m;
	}

	System.out.println("end: Player.load(IXMLElement)");
    }


    public synchronized void save() throws Exception {
	IXMLElement xml = this._save();
	XMLWriter xmw = new XMLWriter( new FileWriter(this.src) );
	xmw.write( xml, true );
    }


    private IXMLElement _save() {
	XMLElement xml = new XMLElement(MARKUP);
	_save(xml);
	return xml;
    }


    private void _save(IXMLElement xml) {
	_toXMLElement(xml);

	//body
	if(this.nativebody != null) {
	    try {
		IXMLElement nxt = new XMLElement( "BODY" );
		this.nativebody.save( nxt );
		xml.addChild( nxt );
	    } catch(Exception e) {
		e.printStackTrace();
	    }
	}

	//home
	if(this.home != null) {
	    try {
		IXMLElement nxt = new XMLElement( "HOME" );
		this.home.save( nxt );
		xml.addChild( nxt );
	    } catch(Exception e) {
		e.printStackTrace();
	    }
	}
    }


    public IXMLElement toXMLElement() {
	XMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }


    private void _toXMLElement(IXMLElement xml) {
	//name, trust, state
	if(this.name != null) {
	    xml.setAttribute( PROPERTY_NAME, this.name );
	}
	xml.setAttribute( PROPERTY_TRUST, Integer.toString(this.trust) );

	IXMLElement nxt;

	//info, title
	nxt = new XMLElement( PROPERTY_INFO );
	if(this.title != null) {
	    nxt.setAttribute( PROPERTY_TITLE, this.title );
	}
	nxt.setContent( this.info );
	xml.addChild( nxt );

	//password
	nxt = new XMLElement( PROPERTY_PASSWORD );
	nxt.setContent( this.pass );
	xml.addChild( nxt );

	//attribs
	nxt = attribs.toXMLElement();
	xml.addChild( nxt );

	nxt = new XMLElement( PROPERTY_FLAGS );
	nxt.setContent( flags.toString() );
	xml.addChild( nxt );

	//grants
	nxt = new XMLElement( PROPERTY_GRANTED );
	nxt.setContent( granted.toString() );
	xml.addChild( nxt );

	//channels
	nxt = new XMLElement( PROPERTY_CHANNELS );
	nxt.setContent( channels.toString() );
	xml.addChild( nxt );
    }


    public void toXMLElement(IXMLElement xml) {
	System.out.println("begin: Player.toXMLElement(IXMLElement)");

	_toXMLElement(xml);

	//body
	if(this.nativebody != null) {
	    IXMLElement nxt = new XMLElement( "BODY" );
	    this.nativebody.toXMLElement( nxt );
	    xml.addChild( nxt );
	}

	//home
	if(this.home != null) {
	    IXMLElement nxt = new XMLElement( "HOME" );
	    this.home.toXMLElement( nxt );
	    xml.addChild( nxt );
	}

	System.out.println("end: Player.toXMLElement(IXMLElement)");
    }


    public void finalize() {
	System.out.println( "begin: Player.finalize()" );
	System.out.println( " on ".concat(this.toString()) );

	// internal cleanup
	if(this.commandBuffer != null) {
	    this.commandBuffer.clear();
	    this.commandBuffer = null;
	}
	
	// kill off our listeners
	if(this.events != null) {
	    this.events.removeListeners();
	    this.events = null;
	}

	System.out.println( "end: Player.finalize()" );
    }

}
