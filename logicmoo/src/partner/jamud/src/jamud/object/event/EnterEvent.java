package jamud.object.event;


import jamud.object.MudObject;
import jamud.object.MudObjectContainer;
import java.lang.ref.WeakReference;


public class EnterEvent extends JamudEvent {

    public static final String
	EVENT_STRING = "onEnter";


    private WeakReference actor;

    public MudObject getActor() {
	return (MudObject) this.actor.get();
    }



    public EnterEvent(MudObject trigger, MudObject actor) {
	super(trigger, EnterEvent.EVENT_STRING);
	this.actor = new WeakReference(actor);
    }

    public EnterEvent(MudObjectContainer trigger, MudObject actor) {
	super(trigger, EnterEvent.EVENT_STRING);
	this.actor = new WeakReference(actor);
    }



    public void finalize() {
	super.finalize();
	this.actor.clear();
    }

}
