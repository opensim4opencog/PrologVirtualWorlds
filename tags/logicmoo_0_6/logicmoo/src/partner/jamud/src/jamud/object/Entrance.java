package jamud.object;

import jamud.Jamud;
import jamud.util.Attributes;
import java.io.IOException;
import net.n3.nanoxml.*;


public class Entrance extends MudObjectViewable implements Cloneable {


    /** <code>MARKUP = "ENTRANCE"</code> */
    public static final String
	MARKUP = "ENTRANCE";
    
    
    private static final String
	PROPERTY_CLASS = "CLASS",
	PROPERTY_TYPE = "TYPE",
	PROPERTY_STATE = "STATE",
	PROPERTY_TARGET = "TARGET",
	PROPERTY_MESSAGE_ORIGIN = "OMSG",
	PROPERTY_MESSAGE_DESTINATION = "DMSG";



    /** Entrance is always in an open state.<br><br>
	<code>TYPE_OPEN = 0</code>
     */
    public static final int TYPE_OPEN = 0;

    /** Entrance may be in an open or closed state, and
	may be blocked.<br><br>
	<code>TYPE_DOOR = 1</code>
     */
    public static final int TYPE_DOOR = 1;

    /** Entrance may be in an open or closed state, and
	may be blocked or locked.<br><br>
	<code>TYPE_LOCK = 2</code>
     */
    public static final int TYPE_LOCK = 2;


    /** Entrance is available to be traversed<br><br>
	<code>STATE_OPEN = 1</code>
     */
    public static final int STATE_OPEN = 1;

    /** Entrance is locked.
	Entrance requires being unlocked before STATE_OPEN may be changed<br>
	A door may be locked in either the open or closed position.<br><br>
	<code>STATE_LOCKED = 2</code>
     */
    public static final int STATE_LOCKED = 2;

    /** Entrance is blocked.
	Entrance requires being unblocked before STATE_OPEN may be changed<br>
	A door may be blocked in either the open or closed position, and may
	or may not be locked. eg: a rusty door being jammed open.<br><br>
	<code>STATE_BLOCKED = 4</code>
     */
    public static final int STATE_BLOCKED = 4;



    // the Template ID of the MudObject whose innards
    // we link to
    private String tid;

    public String getDestinationTid() {
	return tid;
    }

    public void setDestinationTid(String tid) {
	this.tid = tid;
	if(tid != null) {
	    MudObject t = MudObject.getMudObjectTemplate(tid);
	    if(t != null) {
		this.destination = t.childContainer();
		return;
	    }
	}
	this.destination = null;
    }


    // where we're going
    private MudObjectContainer destination;

    public MudObjectContainer getDestination() {
	if(this.destination == null && this.tid != null) {
	    MudObject t = MudObject.getMudObjectTemplate(tid);
	    if(t != null) {
		this.destination = t.childContainer();
	    }
	}
	return this.destination;
    }

    public void setDestination(MudObjectContainer dest) {
	this.destination = dest;
	this.tid = dest.getTemplateId();
    }


    // The Type of Entrance
    private int type = TYPE_OPEN;

    public int getType() {
	return this.type;
    }

    public void setType(int type) {
	this.type = type;
    }


    // The State of the Entrance
    private int state = STATE_OPEN;

    public int getState() {
	return this.state;
    }

    public void setState(int state) {
	this.state = state;
    }


    // The Destination Message
    private String destmsg;

    /** The messsage intended to be printed when exit is used to
     * enter a room (MudObjectContainer)
     */
    public String getDestinationMessage() {
	return this.destmsg;
    }

    public void setDestinationMessage(String msg) {
	this.destmsg = msg;
    }

    
    // The Origin Message
    private String frommsg;

    /** The message intended to be printed when exit is used to leave
     * a room (MudObjectContainer)
     */
    public String getOriginMessage() {
	return this.frommsg;
    }

    public void setOriginMessage(String msg) {
	this.frommsg = msg;
    }



    // Constructors
    public Entrance() {
	//empty, like my heart
    }

    public Entrance(String tid) {
	setDestinationTid(tid);
    }

    public Entrance(MudObjectContainer dest) {
	setDestination(dest);
    }



    public void traverse(MudObject mover,
			 boolean ignoreState) throws EntranceException {

	if(ignoreState || (state & STATE_OPEN) > 0 ) {
	    _traverse(mover);
	} else {
	    throw new EntranceException(this, mover);
	}
    }


    public void traverse(MudObject mover) throws EntranceException {
	if( (state & STATE_OPEN) > 0 ) {
	    _traverse(mover);
	} else {
	    throw new EntranceException(this, mover);
	}
    }


    private void _traverse(MudObject mover) {
	if(getDestination() != null) {

	    if(frommsg != null && frommsg.length() > 0 ) {
		mover.getParentContainer().printAction(mover, frommsg);
	    }
	    
	    mover.getParentContainer().removeChildObject(mover);
	    
	    destination.addChildObject(mover);
	    
	    if(destmsg != null && destmsg.length() > 0 ) {
		destination.printAction(mover, destmsg);
	    }
	}
    }


    public static Entrance loadEntrance(IXMLElement xml) throws Exception {
	String cl = xml.getAttribute(PROPERTY_CLASS, null);
	Entrance te = null;

	if(cl != null) {
	    Object o;

	    if(cl.equalsIgnoreCase("North")) {
		o = new North();
	    } else if(cl.equalsIgnoreCase("South")) {
		o = new South();
	    } else if(cl.equalsIgnoreCase("East")) {
		o = new East();
	    } else if(cl.equalsIgnoreCase("West")) {
		o = new West();
	    } else if(cl.equalsIgnoreCase("Up")) {
		o = new Up();
	    } else if(cl.equalsIgnoreCase("Down")) {
		o = new Down();
	    } else {
		Class c = Class.forName( cl );
		o = c.newInstance();
	    }

	    if(o instanceof Entrance) {
		te = (Entrance) o;
	    } else {
		throw new Exception( cl + " not an instance of" +
				     " jamud.object.Entrance" );
	    }
	} else {
	    te = new Entrance();
	}

	te.load(xml);
	return te;
    }


    public void load(IXMLElement xml) throws Exception {
	// Viewable, Named
	super.load(xml);

	// Random Attribute Stuff
	Attributes at = new Attributes(xml.getAttributes());
	this.type = at.getAttribute( PROPERTY_TYPE, TYPE_OPEN );
	this.state = at.getAttribute( PROPERTY_STATE, STATE_OPEN );
	this.tid = at.getAttribute( PROPERTY_TARGET, null );
	at = null;

	IXMLElement nxt;

	// Destination Message
	nxt = xml.getFirstChildNamed( PROPERTY_MESSAGE_ORIGIN );
	if( nxt != null ) {
	    this.frommsg = nxt.getContent();
	}

	// Origin Message
	nxt = xml.getFirstChildNamed( PROPERTY_MESSAGE_DESTINATION );
	if( nxt != null ) {
	    this.destmsg = nxt.getContent();
	}
    }


    public IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }


    public void toXMLElement(IXMLElement xml) {
	// Viewable, Named
	super.toXMLElement(xml);
	
	// Random Attribute Stuff
	xml.setAttribute( PROPERTY_TYPE, Integer.toString(this.type) );
	xml.setAttribute( PROPERTY_STATE, Integer.toString(this.state) );
	xml.setAttribute( PROPERTY_TARGET, this.tid );

	IXMLElement nxt;
	String i;

	// Destination Message
	i = this.frommsg;
	if( i != null && i.length() > 0 ) {
	    nxt = new XMLElement( PROPERTY_MESSAGE_ORIGIN );
	    nxt.setContent(i);
	    xml.addChild(nxt);
	}

	// Origin Message
	i = this.destmsg;
	if( i != null && i.length() > 0 ) {
	    nxt = new XMLElement( PROPERTY_MESSAGE_DESTINATION );
	    nxt.setContent(i);
	    xml.addChild(nxt);
	}
    }


    public IXMLElement save() {
	if(this.destination != null &&
	   this.destination.isTemplate()) {
	    return toXMLElement();
	} else {
	    return null;
	}
    }


    public void save(IXMLElement xml) {
	if(this.destination != null &&
	   this.destination.isTemplate()) {
	    toXMLElement(xml);
	}
    }


    public Object clone() {
	Entrance mo = new Entrance();
	mo.copycat(this);
	return (Object) mo;	
    }


    public void copycat(Entrance e) {
	//MudObjectViewable
	super.copycat( e );

	//MudObjectEntrance stuff
	this.setDestination( e.getDestination() );
	this.type = e.getType();
	this.state = e.getState();
	this.destmsg = e.getDestinationMessage();
	this.frommsg = e.getOriginMessage();
    }


    public final boolean equals(Object o) {
	return (this == o);
    }


    
    public static class North extends DefaultEntrance {
	public final String defaultName() {
	    return "North";
	}

	public final String defaultDestinationMessage() {
	    return "$n arrives from the south.";
	}

	public final String defaultOriginMessage() {
	    return "$n leaves north.";
	}

	public North() {
	    // super();
	}

	public Object clone() {
	    return new North();
	}
    }


    public static class South extends DefaultEntrance {
	public final String defaultName() {
	    return "South";
	}

	public final String defaultDestinationMessage() {
	    return "$n arrives from the north.";
	}

	public final String defaultOriginMessage() {
	    return "$n leaves south.";
	}

	public South() {
	    // super();
	}

	public Object clone() {
	    return new South();
	}
    }


    public static class East extends DefaultEntrance {
	public final String defaultName() {
	    return "East";
	}

	public final String defaultDestinationMessage() {
	    return "$n arrives from the west.";
	}

	public final String defaultOriginMessage() {
	    return "$n leaves east.";
	}


	public East() {
	    // super();
	}

	public Object clone() {
	    return new East();
	}
    }


    public static class West extends DefaultEntrance {
	public final String defaultName() {
	    return "West";
	}

	public final String defaultDestinationMessage() {
	    return "$n arrives from the east.";
	}

	public final String defaultOriginMessage() {
	    return "$n leaves west.";
	}


	public West() {
	    // super();
	}

	public Object clone() {
	    return new West();
	}
    }


    public static class Up extends DefaultEntrance {
	public final String defaultName() {
	    return "Up";
	}

	public final String defaultDestinationMessage() {
	    return "$n arrives from below.";
	}

	public final String defaultOriginMessage() {
	    return "$n leaves upwards";
	}


	public Up() {
	    // super();
	}

	public Object clone() {
	    return new Up();
	}
    }


    public static class Down extends DefaultEntrance {
	public final String defaultName() {
	    return "Down";
	}

	public final String defaultDestinationMessage() {
	    return "$n arrives from above.";
	}

	public final String defaultOriginMessage() {
	    return "$n leaves downwards.";
	}


	public Down() {
	    // super();
	}

	public Object clone() {
	    return new Down();
	}
    }


    /*
    public class NorthWest extends DefaultEntrance {

    }


    public class NorthEast extends DefaultEntrance {

    }


    public class SouthWest extends DefaultEntrance {

    }


    public class SouthEast extends DefaultEntrance {

    }
    */

}
