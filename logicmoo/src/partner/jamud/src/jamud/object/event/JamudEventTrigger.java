package jamud.object.event;


public abstract interface JamudEventTrigger {
    
    /** subscribe listener to receive events from this event trigger */
    public abstract boolean addListener(JamudEventListener listener,
					Class eventClass);
    
    /** unsubscribe listener from all events on this trigger */
    public abstract boolean removeListener(JamudEventListener listener);
    
    /** unsubscribe listener from the indicated event */
    public abstract boolean removeListener(JamudEventListener listener,
					   Class eventClass);
    
    /** unsubscribe all listeners from all events */
    public abstract boolean removeListeners();

    /** unsubscribe all listeners from the indicated event */
    public abstract boolean removeListeners(Class eventClass);


    public abstract void triggerListeners(JamudEvent event);

}
