package jamud.object;


import jamud.*;
import jamud.command.*;
import jamud.object.event.*;
import jamud.util.*;
import java.io.*;
import java.util.*;
import net.n3.nanoxml.*;


public class MudObject extends MudObjectViewable
    implements JamudEventTrigger, Initializable, Actor {


    /** <code>MARKUP = "OBJECT"</code> */
    public static final String MARKUP = "OBJECT";


    private static final String
	PROPERTY_SRC = "SRC",
	PROPERTY_BASECLASS = "CLASS",
	PROPERTY_TEMPLATE = "TEMPLATE",
	PROPERTY_TEMPLATE_ID = "TID",
	PROPERTY_SECTION = "SECTION",
	PROPERTY_SECTION_NAME = "NAME",
	PROPERTY_ATTRIBUTES = "ATTRIBUTES",
	PROPERTY_FLAGS = "FLAGS",
	PROPERTY_SCRIPTS = "SCRIPTS";


    /** The Visibility of a MudObject is considered a low-level attribute,
     * and hence is included here. Visibility at its lowest level is defined
     * by three values.<br>
     * <br>
     * <code>ATTRIB_VISIBILITY_SHOW</code> indicates that the MudObject
     * can sometimes be seen, or that there exists some point at which it
     * ought to be displayed. If this is not set as a minimum, then this
     * MudObject will NOT be displayed in-game.<br>
     * <code>ATTRIB_VISIBILITY_HIDDEN</code> indicates that the MudObject
     * is hidden. Anything in-game attempting to detect the appearence of a
     * MudObject that is hidden must make some sort of detection roll, etc.
     * in order to see it.<br>
     * <code>ATTRIB_VISIBILITY_INVIS</code> indicates that the MudObject
     * is invisible. This is a higher level version of hidden. It can still
     * be seen by some, but it is far less likely to be so.<br>
     * <br>
     * Visibility is used for example in the printAction method. MudObjects
     * which are not within the range of visibility of the "observer" will
     * show up as "someone" instead of the real name.
     */
    public static final String ATTRIB_VISIBILITY = "VISIBLE";

    public static final int
	ATTRIB_VISIBILITY_SHOW = 1,
	ATTRIB_VISIBILITY_HIDDEN = 2,
	ATTRIB_VISIBILITY_INVIS = 4;


    /** The Open State of a MudObject is considered a low-level attribute,
     * and hence is included here. The Opened State of a MudObeject at its
     * lowest level is defined by three values.<br>
     * <br>
     * <code>ATTRIB_OPENSTATE_OPEN</code> indicates that the MudObject
     * is a container, of can be entered or have other MudObjects put into
     * it at game-time. MudObjects lacking this value are not considred
     * containers.<br>
     * <code>ATTRIB_OPENSTATE_CLOSED</code> indicates that the MudObject
     * is currently closed (hence cannot be entered currently). This can be
     * changed with a simple "open" command normally.<br>
     * <code>ATTRIB_OPENSTATE_LOCKED</code> indicates that the MudObject
     * is locked in its current state and requires a key in order to change
     * that state.
     * <br>
     *
     */
    public static final String ATTRIB_OPENSTATE = "OPEN";

    public static final int
	ATTRIB_OPENSTATE_OPEN = 1,
	ATTRIB_OPENSTATE_CLOSED = 2,
	ATTRIB_OPENSTATE_LOCKED = 4;


    // static section to get a unique load id
    private static long lid_counter = 0;

    public static final synchronized long nextLoadId() {
	return ++lid_counter;
    }

    private static WeakHashMap lid_hash = new WeakHashMap();

    public static final MudObject getMudObjectByLoadId(long id) {
	synchronized( lid_hash ) {
	    return (MudObject) lid_hash.get( new Long(id) );
	}
    }


    // static section to keep templates
    private static WeakHashMap tid_hash = new WeakHashMap();

    public static final MudObject getMudObjectTemplate(String tid) {
	if( tid != null ) {
	    System.out.print("getMudObjectTemplate( \"" + tid + "\" )");

	    MudObject m;
	    synchronized( tid_hash) {
		m = (MudObject) tid_hash.get( tid );
	    }

	    if( m != null ) {
		System.out.println(" != null");
	    } else {
		System.out.println(" == null");
	    }
	    return m;
	} else {
	    System.out.println("getMudObjectTemplate( null )");
	    return null;
	}
    }


    // Initialized
    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state;
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
	if(this.state == STATE_INITIALIZED) {
	    events.triggerListeners(jEvent);
	}
    }


    // The Parent Directory
    protected MudObjectContainer parent;

    public MudObjectContainer getParentContainer() {
	return this.parent;
    }

    public synchronized void setParentContainer(MudObjectContainer p) {
	if(this.parent != p) {
	    if(p != null) {
		p.addChildObject( this );
	    } else if(this.parent != null) {
		this.parent.removeChildObject( this );
	    }
	    this.parent = p;
	}
    }


    // The Child Directory
    private MudObjectContainer child;

    public synchronized MudObjectContainer childContainer() {
	if(this.child == null) {
	    this.child = new MudObjectContainer(this);
	}
	return this.child;
    }


    // Granted Commands
    private FlagList granted;

    public FlagList grantedCommands() {
	return this.granted;
    }

    public boolean isGranted(String command) {
	return this.granted.contains( command );
    }


    // Attributes
    private Attributes attribs;

    public Attributes attributes() {
	return this.attribs;
    }


    //Gender
    private Gender gender;

    public Gender getGender() {
	return this.gender;
    }

    public void setGender(Gender gender) {
	this.gender = gender;
    }


    //Race
    private Race race;

    public Race getRace() {
	return this.race;
    }

    public void setRace(Race race) {
	this.race = race;
    }


    //Job
    private Job job;

    public Job getJob() {
	return this.job;
    }

    public void setJob(Job job) {
	this.job = job;
    }


    // Flags
    private FlagList flags;

    public FlagList flags() {
	return this.flags;
    }

    public boolean isFlagged(String flag) {
	return this.flags.contains( flag );
    }


    // Trust
    private int trust;

    public int getTrust() {
	return this.trust;
    }

    public void setTrust(int trust) {
	this.trust = trust;
    }


    // Template ID
    private String tid;

    public final String getTemplateId() {
	return this.tid;
    }

    public final void setTemplateId(String tid)
	throws TemplateConflictException {

	if(! this.istemplate) {
	    // we aren't a template, so we don't give a damn about our tid
	    this.tid = tid;

	} else if (! this.tid.equals(tid)) {
	    // if we're actually changing the tid

	    synchronized( tid_hash ) {
		// see if there's an object in the new tid space in tid_hash
		Object o = tid_hash.get(tid);

		if(o == null) {
		    // nothing in that space, so put us there and remove our
		    // old entry (if it exists)
		    tid_hash.remove(this.tid);
		    this.tid = tid;
		    tid_hash.put(this.tid, this);

		} else if(o == this) {
		    // then we were somehow in the tid_hash under two tids
		    // which is very strange. Remove the entry for our old
		    // tid, and leave us in the new tid.
		    tid_hash.remove(this.tid);
		    this.tid = tid;

		} else {
		    // then there's a different template there with the tid
		    // we are trying to be set to.
		    throw new TemplateConflictException((MudObject)o,
							this, tid);
		}
	    }
	}
    }


    private boolean istemplate;

    public final boolean isTemplate() {
	return this.istemplate;
    }

    public final void isTemplate(boolean istemplate)
	throws TemplateConflictException {

	synchronized( tid_hash ) {
	    if( this.istemplate && !istemplate) {
		tid_hash.remove( this.tid );

	    } else if( !this.istemplate && istemplate ) {
		Object o = tid_hash.get(this.tid);

		if( o == null ) {
		    tid_hash.put( this.tid, this );
		} else if( this != o ) {
		    throw new TemplateConflictException((MudObject)o,
							this, tid);
		}
	    }
	}
	this.istemplate = istemplate;
    }


    // Load ID
    private long lid;

    public final long getLoadId() {
	return this.lid;
    }


    // SRC file (if applicable)
    private String src;

    public String getSource() {
	return this.src;
    }

    public void setSource(String src) {
	this.src = src;
    }


    // Available Sections
    private FlagList sectionNames;

    public FlagList availableSections() {
	return this.sectionNames;
    }


    // Sections
    private HashMap sections;

    public MudObject getSection(String name) {
	synchronized( sections ) {
	    return (MudObject) sections.get(name);
	}
    }

    public void setSection(String name, MudObject sec) {
	if(name != null && 
	   sectionNames.contains(name = name.toLowerCase())) {

	    synchronized( sections ) {
		if(sec != null) {
		    sections.put(name, sec);
		} else {
		    sections.remove(name);
		}
	    }
	}
    }



    // Default Constructor
    public MudObject() {
	this.lid = nextLoadId();
	synchronized( lid_hash ) {
	    lid_hash.put( new Long(this.lid), this );
	}

	this.attribs = new Attributes();

	this.flags = new FlagList();
	this.sectionNames = new FlagList();
	this.sections = new HashMap();

	this.granted = new FlagList();

	this.race = Race.DEFAULT;
	this.gender = Gender.DEFAULT;
	this.job = Job.DEFAULT;
	
	this.sectionNames.refresh(race.sections());
    }

    public MudObject(Race race, Gender gender, Job job) {
	this.lid = nextLoadId();
	synchronized( lid_hash ) {
	    lid_hash.put( new Long(this.lid), this );
	}

	this.attribs = new Attributes();

	this.flags = new FlagList();
	this.sectionNames = new FlagList();
	this.sections = new HashMap();

	this.granted = new FlagList();

	this.race = race;
	this.gender = gender;
	this.job = job;

	this.sectionNames.refresh(race.sections());
    }


    
    public synchronized void load(IXMLElement xml) throws Exception {
	System.out.println("begin: MudObject.load(xml)");

	String src = xml.getAttribute(PROPERTY_SRC, null);

	// If the passed xml element just points to a file
	if(src != null) {
	    IXMLParser xmp = new StdXMLParser();
	    xmp.setBuilder(new StdXMLBuilder());
	    xmp.setValidator(new NonValidator());
	    xmp.setReader(StdXMLReader.fileReader(src));

	    xml = (IXMLElement) xmp.parse();
	    xmp = null;

	    this.src = src;
	}

	// Viewable (name, desc, sdesc, keywords)
	super.load(xml);
	
	// Template ID
	Attributes at = new Attributes(xml.getAttributes());
	tid = at.getAttribute(PROPERTY_TEMPLATE_ID, null);
	if(tid != null) {
	    this.isTemplate(at.getAttribute(PROPERTY_TEMPLATE, false));
	}
	at = null;
	
	IXMLElement nxt;

	// Race
	nxt = xml.getFirstChildNamed(Race.MARKUP);
	if(nxt != null) {
	    this.race = Race.loadRace(nxt);
	}

	// Job
	nxt = xml.getFirstChildNamed(Job.MARKUP);
	if(nxt != null) {
	    this.job = Job.loadJob(nxt);
	}

	// Gender
	nxt = xml.getFirstChildNamed(Gender.MARKUP);
	if(nxt != null) {
	    this.gender = Gender.loadGender(nxt);
	}
	
	// Flags
	nxt = xml.getFirstChildNamed(PROPERTY_FLAGS);
	if(nxt != null) {
	    String f = nxt.getContent();
	    if(f != null && f.length() > 0) {
		flags.refresh( f );
	    }
	}
	
	// Attributes
	nxt = xml.getFirstChildNamed(Attributes.MARKUP);
	if(nxt != null) {
	    this.attribs.load(nxt);
	}
	
	// Sections
	Vector v = xml.getChildrenNamed(PROPERTY_SECTION);
	for(Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
	    nxt = (IXMLElement) enum.nextElement();

	    String s = nxt.getAttribute(PROPERTY_SECTION_NAME, null);
	    if(s != null && s.length() > 0) {
		sectionNames.add(s);

		nxt = nxt.getFirstChildNamed(MARKUP);
		if(nxt != null) {
		    setSection( s, MudObject.loadMudObject(nxt) );
		}
	    }
	}
	v = null;
	
	// Child Mud Object Container
	nxt = xml.getFirstChildNamed(MudObjectContainer.MARKUP);
	if(nxt != null) {
	    childContainer().load(nxt);
	}

	// let us be initialized
	this.initialize();

	System.out.println("end: MudObject.load(xml)");
    }
    

    public static MudObject loadMudObject(IXMLElement xml) throws Exception {
	String c = xml.getAttribute(PROPERTY_BASECLASS, null);

	if(c == null) {
	    MudObject mo = new MudObject();
	    mo.load(xml);
	    return mo;
	} else {
	    Class cl = Class.forName(c);
	    return loadMudObject(xml, cl);
	}
    }


    public static MudObject loadMudObject(IXMLElement xml,
					  Class base) throws Exception {

	Object o = base.newInstance();

	if( !(o instanceof MudObject) ) {
	    throw new Exception("class " + base.getName() +
				" not an instance of jamud.obj.MudObject");
	}

	MudObject mo = (MudObject) o;
	mo.load(xml);

	return mo;
    }


    public synchronized void save(IXMLElement xml) throws Exception {
	if(this.src == null) {
	    _save(xml);
	} else {
	    IXMLElement nxt = _save();
	    XMLWriter xmp = new XMLWriter( new FileWriter(this.src) );
	    xmp.write( nxt, true );
	    toShortXMLElement(xml);
	}
    }


    public synchronized IXMLElement save() throws Exception {
	IXMLElement xml = new XMLElement(MARKUP);
	save(xml);
	return xml;
    }


    private void _save(IXMLElement xml) throws Exception {
	_toXMLElement(xml);

	IXMLElement nxt;

	// Sections
	synchronized( sections ) {
	    for(Iterator i = sections.keySet().iterator(); i.hasNext(); ) {
		String s = (String) i.next();
		MudObject o = (MudObject) sections.get(s);
	    	    
		if(o != null) {
		    nxt = new XMLElement(PROPERTY_SECTION);
		    nxt.setAttribute(PROPERTY_SECTION_NAME, s);

		    IXMLElement x = o.save();
		    if(x != null) {
			nxt.addChild( x );
			xml.addChild(nxt);
		    }
		}
	    }
	}

	// Child
	if(this.child != null) {
	    nxt = child.save();
	    xml.addChild(nxt);
	}
    }


    private IXMLElement _save() throws Exception {
	IXMLElement xml = new XMLElement(MARKUP);
	_save(xml);
	return xml;
    }

    
    public synchronized void toXMLElement(IXMLElement xml) {
	_toXMLElement( xml );

	IXMLElement nxt;

	// Sections
	synchronized( sections ) {
	    for(Iterator i = sections.keySet().iterator(); i.hasNext(); ) {
		String s = (String) i.next();
		MudObject o = (MudObject) sections.get(s);
	    
		nxt = new XMLElement(PROPERTY_SECTION);
		nxt.setAttribute(PROPERTY_SECTION_NAME, s);
	    
		if(o != null) {
		    nxt.addChild( o.toXMLElement() );
		}

		xml.addChild(nxt);
	    }
	}

	// Child
	if(this.child != null) {
	    nxt = child.toXMLElement();
	    xml.addChild(nxt);
	}
    }


    private void _toXMLElement(IXMLElement xml) {

	// Viewable, Named
	super.toXMLElement(xml);

	// Template ID
	if(tid != null) {
	    xml.setAttribute(PROPERTY_TEMPLATE_ID, tid);
	    if(istemplate) {
		xml.setAttribute(PROPERTY_TEMPLATE, "true");
	    }
	}

	IXMLElement nxt;

	// Flags
	nxt = new XMLElement(PROPERTY_FLAGS);
	nxt.setContent(flags.toString());
	xml.addChild(nxt);

	// Job, Race, Gender
	if(this.job != null) {
	    xml.addChild( this.job.toShortXMLElement() );
	}
	if(this.race != null) {
	    xml.addChild( this.race.toShortXMLElement() );
	}
	if(this.gender != null) {
	    xml.addChild( this.gender.toShortXMLElement() );
	}

	// Attributes
	if(this.attribs.size() > 0) {
	    xml.addChild( attribs.toXMLElement() );
	}
    }


    public synchronized IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }


    public synchronized void toShortXMLElement(IXMLElement xml) {
	xml.setAttribute(PROPERTY_SRC, src);
    }


    public synchronized IXMLElement toShortXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toShortXMLElement(xml);
	return xml;
    }


    public synchronized boolean initialize() {
	if(this.state > STATE_TERMINATED) {
	    return false;
	} else {
	    this.state = STATE_INITIALIZING;
	}

	// enable activity
	if(this.child != null) {
	    this.child.initialize();
	}

	this.state = STATE_INITIALIZED;
	return true;
    }


    public synchronized boolean terminate() {
	if(this.state < STATE_INITIALIZED) {
	    return false;
	} else {
	    this.state = STATE_TERMINATING;
	}

	// disable activity
	if(this.child != null) {
	    this.child.terminate();
	}
	
	this.state = STATE_TERMINATED;
	return true;
    }


    public synchronized void copycat(MudObject mirror) {
	// Viewable (name, desc, sdesc, keywords)
	super.copycat(mirror);

	// Template ID
	// doesn't get overwritten!

	// Flags
	flags.refresh(mirror.flags);

	// Attributes
	this.attribs = (Attributes) mirror.attribs.clone();

	// Sections
	// copy only the names
	sectionNames.refresh(mirror.sectionNames);

	// Child
	if(mirror.child != null) {
	    childContainer().copycat(mirror.child);
	}
    }


    public void enact(String command) {
	this.enact(this, command);
    }


    public void enact(Actor user, String command) {
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
		    c.execute( user, cmdsplit[1] );
		} catch(CommandException ce) {
		    ce.printStackTrace();
		    this.println( "Huh?" );
		}
		
	    } else {
		// TODO: look up message
		this.println( "Huh?" );
	    }
	}
    }


    public void print(String text) {
	if(this.state == STATE_INITIALIZED) {
	    events.triggerListeners( new PrintEvent(this, null, text) );
	}
    }


    public void println(String text) {
	if(this.state == STATE_INITIALIZED) {
	    PrintEvent p = new PrintEvent(this, null, text.concat("\n"));
	    events.triggerListeners( p );
	}
    }


    public void printAction(MudObject src, String text) {
	if( this.state == STATE_INITIALIZED &&
	    this.events.hasListener(PrintEvent.class) ) {

	    text = _parseAction( src, text );
	    events.triggerListeners( new PrintEvent(this, src, text) );
	}
    }


    public void printAction(MudObject src, MudObject target, String text) {
	if( this.state == STATE_INITIALIZED &&
	    this.events.hasListener(PrintEvent.class) ) {

	    text = _parseAction( src, target, text );
	    events.triggerListeners( new PrintEvent(this, src, text) );
	}
    }


    public void tick() {
	if( this.state == STATE_INITIALIZED ) {

	    events.triggerListeners( new TickEvent(this) );

	    if(this.child != null) {
		this.child.tick();
	    }
	}
    }


    private String _parseAction(MudObject source, String parse) {

	//TODO: visibility checks

	if(parse == null) {
	    return null;
	} else if(parse.length() == 0) {
	    return parse;
	}

	HashMap map = new HashMap();

	if(source != null) {
	    map.put( "r", source.getRace().getAdj() );
	    map.put( "e", source.getGender().getPron() );
	    map.put( "m", source.getGender().getNom() );
	    map.put( "s", source.getGender().getPoss() );
	    map.put( "n", source.getName() );
	}

	return Util.variableParse( map, parse );	
    }


    private String _parseAction(MudObject source, MudObject target,
				String parse) {

	//TODO: visibility checks

	if(parse == null) {
	    return null;
	} else if(parse.length() == 0) {
	    return parse;
	}

	HashMap map = new HashMap();

	if(source != null) {
	    map.put( "r", source.getRace().getAdj() );
	    map.put( "e", source.getGender().getPron() );
	    map.put( "m", source.getGender().getNom() );
	    map.put( "s", source.getGender().getPoss() );
	    map.put( "n", source.getName() );
	}

	if(target != null) {
	    map.put( "R", target.getRace().getAdj() );
	    map.put( "E", target.getGender().getPron() );
	    map.put( "M", target.getGender().getNom() );
	    map.put( "S", target.getGender().getPoss() );
	    map.put( "N", target.getName() );
	}

	return Util.variableParse( map, parse );
    }


    public static String parseAction(MudObject source, String parse) {
	if(parse == null) {
	    return null;
	} else if(parse.length() == 0) {
	    return parse;
	}

	HashMap map = new HashMap();

	if(source != null) {
	    map.put( "r", source.getRace().getAdj() );
	    map.put( "e", source.getGender().getPron() );
	    map.put( "m", source.getGender().getNom() );
	    map.put( "s", source.getGender().getPoss() );
	    map.put( "n", source.getName() );
	}

	return Util.variableParse( map, parse );
    }


    public static String parseAction(MudObject source, MudObject target,
				     String parse) {
	if(parse == null) {
	    return null;
	} else if(parse.length() == 0) {
	    return parse;
	}

	HashMap map = new HashMap();

	if(source != null) {
	    map.put( "r", source.getRace().getAdj() );
	    map.put( "e", source.getGender().getPron() );
	    map.put( "m", source.getGender().getNom() );
	    map.put( "s", source.getGender().getPoss() );
	    map.put( "n", source.getName() );
	}

	if(target != null) {
	    map.put( "R", target.getRace().getAdj() );
	    map.put( "E", target.getGender().getPron() );
	    map.put( "M", target.getGender().getNom() );
	    map.put( "S", target.getGender().getPoss() );
	    map.put( "N", target.getName() );
	}

	return Util.variableParse( map, parse );
    }

}
