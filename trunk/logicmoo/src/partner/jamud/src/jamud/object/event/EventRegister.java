package jamud.object.event;


import java.lang.ref.*;
import java.util.*;


public class EventRegister implements JamudEventTrigger {
    
    
    // contains RegisteredEvents
    private Set register;
    
    // used to clean GC'd listeners
    private ReferenceQueue rq = new ReferenceQueue();
    
    
    
    public EventRegister() {
	register = Collections.synchronizedSet(new HashSet());
    }
    
    
    
    private void clean() {
	while(rq.poll() != null) {
	    try {
		Reference ref = rq.remove( (long)10 );
		for(Iterator i = register.iterator(); i.hasNext(); ) {
		    RegisteredEvent n = (RegisteredEvent) i.next();
		    if( ref == n.listenerReference ) {
			i.remove();
			break;
		    }
		}
	    } catch(InterruptedException ie) {
		; //nuttin' in ref, so don't do anything
	    }
	}
    }
    

    public boolean hasListener(JamudEventListener listener,
					 Class eventClass) {

	RegisteredEvent re = new RegisteredEvent(listener, eventClass);
	for(Iterator i = register.iterator(); i.hasNext(); ) {
	    if(re.equals( (RegisteredEvent)i.next() )) {
		return true;
	    }
	}
	
	return false;
    }


    public boolean hasListener(Class eventClass) {
	for(Iterator i = register.iterator(); i.hasNext(); ) {
	    RegisteredEvent re = (RegisteredEvent) i.next();
	    if( re.eventClass() == eventClass ) {
		return true;
	    }
	}

	return false;
    }

    
    public boolean addListener(JamudEventListener listener,
					 Class eventClass) {

	RegisteredEvent re = new RegisteredEvent(listener, eventClass);
	final boolean r = register.add(re);
	clean();
	return r;
    }
    
    
    public boolean removeListener(JamudEventListener listener) {
	boolean found = false;
	for(Iterator i = register.iterator(); i.hasNext(); ) {
	    RegisteredEvent r = (RegisteredEvent) i.next();
	    if( listener == r.listenerReference.get() ) {
		i.remove();
		found = true;
	    }
	}
	return found;
    }
    
    
    public boolean removeListener(JamudEventListener listener,
				  Class eventClass) {

	RegisteredEvent r = new RegisteredEvent(listener, eventClass);
	    
	for(Iterator i = register.iterator(); i.hasNext(); ) {
	    if( r.equals( (RegisteredEvent)i.next() ) ) {
		i.remove();
		return true;
	    }
	}
	return false;
    }
    
    
    public boolean removeListeners() {
	final boolean found = (register.size() > 0);
	register.clear();
	return found;
    }
    
    
    public boolean removeListeners(Class eventClass) {
	boolean found = false;
	for(Iterator i = register.iterator(); i.hasNext(); ) {
	    RegisteredEvent r = (RegisteredEvent) i.next();
	    if(r.eventClass() == eventClass) {
		i.remove();
		found = true;
	    }
	}
	return found;
    }
    
    
    public void triggerListeners(JamudEvent jEvent) {
	if(jEvent == null) {
	    System.out.println("a NULL event was triggered");
	}

	Class e = jEvent.getClass();
	
	for(Iterator i = register.iterator(); i.hasNext(); ) {
	    RegisteredEvent re = (RegisteredEvent) i.next();
	    if( re.eventClass == e ) {
		JamudEventListener jel = re.listener();
		if(jel != null) {
		    jel.eventTriggered(jEvent);
		}
	    }
	}
    }
    
    
    
    //inner class for pairing a listener with the event they want
    class RegisteredEvent {
	
	WeakReference listenerReference;
	Class eventClass;

	Class eventClass() {
	    return eventClass;
	}
	
	JamudEventListener listener() {
	    return (JamudEventListener) listenerReference.get();
	}
	
	
	RegisteredEvent(JamudEventListener listener, Class eventClass) {
	    this.listenerReference = new WeakReference(listener, rq);
	    this.eventClass = eventClass;
	}
	
	
	public boolean equals(Object o) {
	    if( o instanceof RegisteredEvent) {
		return this.equals( (RegisteredEvent)o );
	    } else {
		return (o == this);
	    }
	}
	
	public boolean equals(RegisteredEvent re) {
	    return (
		    re.eventClass == this.eventClass &&
		    re.listenerReference.get() == this.listenerReference.get()
		    );
	}
	
    }
    
}
