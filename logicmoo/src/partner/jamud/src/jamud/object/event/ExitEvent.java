package jamud.object.event;


import jamud.object.MudObject;
import jamud.object.MudObjectContainer;
import java.lang.ref.WeakReference;


public class ExitEvent extends JamudEvent {

    public static final String
	EVENT_STRING = "onExit";


    private WeakReference actor;

    public MudObject getActor() {
	return (MudObject) this.actor.get();
    }


    /** This event is triggered when a MudObject (actor) leaves the same
     * container as trigger
     */
    public ExitEvent(MudObject trigger, MudObject actor) {
	super(trigger, ExitEvent.EVENT_STRING);
	this.actor = new WeakReference(actor);
    }

    /** This event is triggered when a MudObject (actor) leaves the
     * container (trigger)
     */
    public ExitEvent(MudObjectContainer trigger, MudObject actor) {
	super(trigger, ExitEvent.EVENT_STRING);
	this.actor = new WeakReference(actor);
    }



    public void finalize() {
	super.finalize();
	this.actor.clear();
    }

}
