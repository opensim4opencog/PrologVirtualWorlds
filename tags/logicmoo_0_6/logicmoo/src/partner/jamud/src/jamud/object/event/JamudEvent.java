package jamud.object.event;


import java.lang.ref.WeakReference;


/** Describes the most basic level of functionality for an
 * event within Jamud.
 */
public class JamudEvent {


    // who triggered the event
    private WeakReference src;

    public JamudEventTrigger getTrigger() {
	return (JamudEventTrigger) this.src.get();
    }

    
    // string representation of the event
    private String event;

    public String getEventString() {
	return this.event;
    }
    
    

    public JamudEvent(JamudEventTrigger trigger, String eventString) {
	this.src = new WeakReference(trigger);
	this.event = event;
    }
    
    

    public void finalize() {
	this.src.clear();
	this.event = null;
    }

}
