package jamud.object;


import jamud.object.event.*;
import jamud.util.Attributes;
import java.lang.ref.WeakReference;
import net.n3.nanoxml.*;


public class MudObjectListener implements JamudEventListener {


    public static final String MARKUP = "LISTENER";


    private static final String
	PROPERTY_BASECLASS = "CLASS",
	PROPERTY_EVENT = "EVENT",
	PROPERTY_TRIGGER = "TRIGGER",
	PROPERTY_INSIDE = "USE_INSIDE";


    private static final Class DEFAULT_EVENT_CLASS = PrintEvent.class;


    private Class event_class;


    private WeakReference target;


    private String trigger_tid;


    private WeakReference trigger;


    private boolean useTriggerContainer;


    private JamudEventListener tick_listener;
    


    public MudObjectListener(JamudEventTrigger target) {
	this.target = new WeakReference( target );
    }



    public static final MudObjectListener loadMudObjectListener(
        IXMLElement xml, JamudEventTrigger target)
	throws Exception {

	String c = xml.getAttribute(PROPERTY_BASECLASS, null);

	if(c == null) {
	    MudObjectListener mo = new MudObjectListener( target );
	    mo.load(xml);
	    return mo;
	} else {
	    Class cl = Class.forName(c);
	    return loadMudObjectListener(xml, target, cl);
	}
    }


    public static final MudObjectListener loadMudObjectListener(
        IXMLElement xml, JamudEventTrigger target, Class base)
        throws Exception {

	final Class[] cons  = { JamudEventTrigger.class };
	java.lang.reflect.Constructor c = base.getConstructor( cons );
	final Object[] pass = { target };
	Object o = c.newInstance( pass );
	
	if( !(o instanceof MudObjectListener) ) {
	    throw new Exception("class " + base.getName() +
				" not an instance of" +
				" jamud.obj.MudObjectListener");
	}

	MudObjectListener mo = (MudObjectListener) o;
	mo.load(xml);
	return mo;
    }


    public void load(IXMLElement xml) {
	Attributes at = new Attributes(xml.getAttributes());
	this.trigger_tid = at.getAttribute( PROPERTY_TRIGGER, null );
	this.useTriggerContainer = at.getAttribute( PROPERTY_INSIDE, false );
	at = null;
	_tick();
    }


    public IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement( MARKUP );
	toXMLElement( xml );
	return xml;
    }


    public void toXMLElement(IXMLElement xml) {
	if( this.event_class != DEFAULT_EVENT_CLASS ) {
	    xml.setAttribute( PROPERTY_EVENT, this.event_class.getName() );
	}

	xml.setAttribute( PROPERTY_TRIGGER, this.trigger_tid );
	xml.setAttribute( PROPERTY_INSIDE,
			  useTriggerContainer ? "TRUE" : "FALSE" );
    }


    private void _tick() {
	// all this just to find our trigger by tid, what a hullaballoo

	if( this.trigger_tid == null ) {
	    return;	   

	} else if( this.trigger.get() == null ) {
	    // try and find the template id
	    MudObject m = MudObject.getMudObjectTemplate( this.trigger_tid );

	    if( m != null ) {

		// if we found it, hooray! Our quest is over.
		if(useTriggerContainer) {
		    assignTrigger( m.childContainer() );
		} else {
		    assignTrigger( m );
		}
		_killTick();

	    } else {
		if( this.tick_listener == null ) {
		    this.tick_listener = new JamudEventListener() {
			    public void eventTriggered(JamudEvent e) {
				_tick();
			    }
			};
		    ((JamudEventTrigger) target.get())
			.addListener(this.tick_listener, TickEvent.class);
		}
	    }

	} else {
	    _killTick();

	}
    }


    private void _killTick() {
	if( this.tick_listener != null ) {

	    ((JamudEventTrigger) target.get())
		.removeListener(this.tick_listener, TickEvent.class);

	    this.tick_listener = null;
	}	
    }


    private void assignTrigger( JamudEventTrigger j ) {
	this.trigger = new WeakReference( j );
	j.addListener( this, this.event_class );
    }


    public void eventTriggered(JamudEvent event) {
	if( event.getClass() == event_class
	    && event.getTrigger() == this.trigger.get() ) {
	    
	    JamudEventTrigger t = (JamudEventTrigger) target.get();
	    t.triggerListeners( event );
	}
    }


}
