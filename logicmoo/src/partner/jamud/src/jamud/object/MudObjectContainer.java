package jamud.object;


import jamud.Initializable;
import jamud.object.event.*;
import java.io.*;
import java.util.*;
import net.n3.nanoxml.*;


public class MudObjectContainer extends MudObjectViewable
    implements JamudEventTrigger, Initializable {


    /** <code>MARKUP = "INSIDE"</code> */
    public static final String MARKUP = "INSIDE";


    // Initialized
    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state;
    }


    // The Event Register
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

    public void triggerListeners(JamudEvent event) {
	if(this.state == STATE_INITIALIZED) {
	    events.triggerListeners(event);
	}
    }


    // The MudObject outside this
    private MudObject parent;

    public MudObject parentObject() {
	return this.parent;
    }


    // forward the method to the parent
    public String getTemplateId() {
	if(this.parent != null) {
	    return this.parent.getTemplateId();
	} else {
	    return null;
	}
    }

    public long getLoadId() {
	if(this.parent != null) {
	    return this.parent.getLoadId();
	} else {
	    return -1;
	}
    }

    public boolean isTemplate() {
	if(this.parent != null) {
	    return this.parent.isTemplate();
	} else {
	    return false;
	}
    }


    // The MudObjects inside this
    private Set children;

    public synchronized Iterator childObjects() {
	return children.iterator();
    }

    public synchronized Iterator childObjects(String keyword) {
	MudObject[] r = new MudObject[children.size()];
	int I = 0;

	for(Iterator i = children.iterator(); i.hasNext(); ) {
	    MudObject o = (MudObject) i.next();
	    if(o.matchKeyword(keyword)) {
		r[I++] = o;
	    }
	}

	if(I > 0) {
	    MudObject[] ret = new MudObject[I];
	    System.arraycopy(r, 0, ret, 0, I);
	    return _ChildObjects(ret);

	} else {
	    return _ChildObjects(new MudObject[0]);

	}
    }

    private Iterator _ChildObjects(final MudObject[] o) {

	Iterator e = new Iterator() {
		Object[] oa = o;
		int count;

		public boolean hasNext() {
		    return (count < oa.length);
		}

		public Object next() {
		    return (count < oa.length) ?
			oa[count++] :
			null;
		}

		public void remove() {
		    if(count > 0) {
			children.remove( oa[count-1] );
		    }
		}
	    };

	return e;
    }

    public synchronized MudObject getChildObject(String keyword) {
	return this.getChildObject(0, keyword);
    }
    
    public synchronized MudObject getChildObject(int offset,
						 String keyword) {

	if( (keyword != null) && (keyword.length() > 0) ) {
	    for(Iterator i = children.iterator(); i.hasNext(); ) {
		MudObject o = (MudObject) i.next();
		if( o.matchKeyword(keyword) && (offset-- == 0) ) {
		    return o;
		}
	    }
	}
	return null;
    }
    
    /** Adds a MudObject to this container, and sets the ParentContainer
     * property of the child.
     */
    public synchronized boolean addChildObject(MudObject child) {
	if( child != null &&
	    children.add( child )) {
		
	    if(child.parent != null) {
		child.parent.children.remove(child);
	    }
	    
	    child.parent = this;
	    return true;
	}
	
	return false;
    }

    /** If this container contains child, child is removed and the
     * ParentContainer property of child is set to null.
     */
    public synchronized boolean removeChildObject(MudObject child) {
	if( children.remove(child) ) {
	    child.parent = null;
	    return true;
	} else {
	    return false;
	}
    }


    // Exits is a list of Entrance objects.
    private Set exits;

    /**
     * Obtains an Entrance by name.<br>
     * <br>
     * "Which is a kind of integrity, if you look upon every exit as
     * merely an entrance, someplace else" -<i>Rosencrantz &amp;
     * Guildenstern are Dead</i>, Tom Stoppard
     */
    public Entrance getExit(String name, boolean cases) {
	synchronized( exits ) {
		for(Iterator i = exits.iterator(); i.hasNext(); ) {
			Entrance e = (Entrance) i.next();
			if(e.getName().equals(name) || cases == true && e.getName().equalsIgnoreCase(name)) {
				return e;
			} 
		}
	return null;
	}
    }

    /**
     * new version of getExit that calls the real one with default args
     * this defaults to a case sensitive search
     */
    public Entrance getExit(String name) {
	return this.getExit(name,false);
    }

    /**
     * Obtains the <i>n</i>th Entrance by name, where <i>n</n> is the
     * <code>offset</code>. 0 returns the first, 1 returns the second...
     */
    public Entrance getExit(int offset, String name) {
	synchronized( exits ) {
	    int n = 0;
	    for(Iterator i = exits.iterator(); i.hasNext(); ) {
		Entrance e = (Entrance) i.next();
		if( e.getName().equals(name) && n++ == offset ) {
		    return e;
		}
	    }
	    return null;
	}
    }

    public boolean addExit(Entrance e) {
	return this.exits.add( e );
    }

    public boolean removeExit(Entrance e) {
	return this.exits.remove( e );
    }

    public boolean removeExit(String name) {
	synchronized( exits ) {
	    for(Iterator i = exits.iterator(); i.hasNext(); ) {
		Entrance e = (Entrance) i.next();
		if(e.getName().equals(name)) {
		    i.remove();
		    return true;
		}
	    }
	    return false;
	}
    }

    public boolean removeExit(int offset, String name) {
	synchronized( exits ) {
	    int n = 0;
	    for(Iterator i = exits.iterator(); i.hasNext(); ) {
		Entrance e = (Entrance) i.next();
		if(e.getName().equals(name) && n++ == offset) {
		    i.remove();
		    return true;
		}
	    }
	    return false;
	}
    }

    public Iterator exits() {
	return this.exits.iterator();
    }



    // Default Constructor
    public MudObjectContainer(MudObject parent) {
	this.parent = parent;

	this.children = Collections.synchronizedSet(new HashSet());
	this.exits = Collections.synchronizedSet(new HashSet());
    }



    public void load(IXMLElement xml) throws Exception {
	System.out.println("begin: MudObjectContainer.load(xml)");

	// Viewable, Named
	super.load(xml);

	for(Enumeration enum = xml.enumerateChildren();
	    enum.hasMoreElements(); ) {

	    IXMLElement nxt = (XMLElement) enum.nextElement();

	    // Children, Exits
	    if(nxt.getName().equals(MudObject.MARKUP)) {

		addChildObject( MudObject.loadMudObject(nxt) );

	    } else if(nxt.getName().equals(Entrance.MARKUP)) {

		exits.add( Entrance.loadEntrance(nxt) );

	    }
	}

	System.out.println("end: MudObjectContainer.load(xml)");
    }


    public IXMLElement save() throws Exception {
	System.out.println("begin: MudObjectContainer.save() ");
	IXMLElement xml = new XMLElement(MARKUP);
	save(xml);
	System.out.println("end: MudObjectContainer.save()");
	return xml;
    }


    public void save(IXMLElement xml) throws Exception {
	// Viewable
	super.toXMLElement( xml );

	IXMLElement nxt;
	
	// Children
	for(Iterator i = children.iterator(); i.hasNext(); ) {
	    nxt = ((MudObject)i.next()).save();
	    if(nxt != null) {
		xml.addChild( nxt );
	    }
	}

	// Exits
	for(Iterator i = exits.iterator(); i.hasNext(); ) {
	    nxt = ((Entrance)i.next()).save();
	    if(nxt != null) {
		xml.addChild( nxt );
	    }
	}
    }


    public IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }


    public void toXMLElement(IXMLElement xml) {
	// Viewable
	super.toXMLElement(xml);

	// Children
	for(Iterator i = children.iterator(); i.hasNext(); ) {
	    xml.addChild( ((MudObject)i.next()).toXMLElement() );
	}

	// Exits
	for(Iterator i = exits.iterator(); i.hasNext(); ) {
	    xml.addChild( ((Entrance)i.next()).toXMLElement() );
	}
    }


    public void copycat(MudObjectContainer mirror) {
	// Viewable, Named
	super.copycat(mirror);

	// Exits
	try {
	    synchronized(mirror.exits) {
		synchronized(exits) {
		    exits.clear();
		    for(Iterator i = mirror.exits.iterator(); i.hasNext(); ) {
			Entrance c = (Entrance) i.next();
			exits.add( c.clone() );
		    }
		}
	    }
	} catch(Exception e) {
	    ; // clone not supported?? wtf??
	}

    }


    public void print(String text) {
	for(Iterator i = children.iterator(); i.hasNext(); ) {
	    ((MudObject)i.next()).print( text );
	}

	triggerListeners( new PrintEvent(this, null, text) );
    }


    public void println(String text) {
	for(Iterator i = children.iterator(); i.hasNext(); ) {
	    ((MudObject)i.next()).println( text );
	}

	triggerListeners( new PrintEvent(this, null, text) );
    }
    

    public void printAction(MudObject src, String text) {
	for(Iterator i = children.iterator(); i.hasNext(); ) {
	    MudObject o = (MudObject) i.next();
	    if(o != src) {
		o.printAction(src, text);
	    }
	}

	if( this.state == STATE_INITIALIZED &&
	    this.events.hasListener(PrintEvent.class) ) {

	    text = MudObject.parseAction( src, text );
	    events.triggerListeners( new PrintEvent(this, src, text) );
	}
    }


    public void printAction(MudObject src, MudObject target, String text) {
	for(Iterator i = children.iterator(); i.hasNext(); ) {
	    MudObject o = (MudObject) i.next();
	    if(o != src && o != target) {
		o.printAction(src, text);
	    }
	}

	if( this.state == STATE_INITIALIZED &&
	    this.events.hasListener(PrintEvent.class) ) {

	    text = MudObject.parseAction( src, target, text );
	    events.triggerListeners( new PrintEvent(this, src, text) );
	}
    }


    public void printAction(MudObject src, MudObject target,
			    String targetText, String defaultText) {
	
	for(Iterator i = children.iterator(); i.hasNext(); ) {
	    MudObject o = (MudObject) i.next();
	    if(o != src) {
		if(o == target) {
		    o.printAction(src, target, targetText);
		} else {
		    o.printAction(src, target, defaultText);
		}
	    }
	}

	if( this.state == STATE_INITIALIZED &&
	    this.events.hasListener(PrintEvent.class) ) {

	    defaultText = MudObject.parseAction(src, target, defaultText);
	    events.triggerListeners( new PrintEvent(this, src, defaultText) );
	}
    }


    public synchronized boolean initialize() {
	if(this.state > STATE_TERMINATED) {
	    return false;
	} else {
	    this.state = STATE_INITIALIZING;
	}

	// enables activity

	this.state = STATE_INITIALIZED;
	return true;
    }


    public synchronized boolean terminate() {
	if(this.state < STATE_INITIALIZED) {
	    return false;
	} else {
	    this.state = STATE_TERMINATING;
	}

	// disables activity
	
	this.state = STATE_TERMINATED;
	return true;
    }


    public void tick() {
	triggerListeners( new TickEvent(this) );

	for(Iterator i = children.iterator(); i.hasNext(); ) {
	    ((MudObject)i.next()).tick();
	}
    }

}
