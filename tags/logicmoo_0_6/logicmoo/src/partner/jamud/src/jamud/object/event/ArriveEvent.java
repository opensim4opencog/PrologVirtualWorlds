package jamud.object.event;


import jamud.object.MudObject;
import jamud.object.MudObjectContainer;
import java.lang.ref.WeakReference;


public class ArriveEvent extends JamudEvent {

    public static final String
	EVENT_STRING = "onArrive";


    private WeakReference dest;

    public MudObjectContainer getDestination() {
	return (MudObjectContainer) this.dest.get();
    }



    public ArriveEvent(MudObject trigger,
		       MudObjectContainer destination) {
	super(trigger, ArriveEvent.EVENT_STRING);
	this.dest = new WeakReference(destination);
    }



    public void finalize() {
	super.finalize();
	this.dest.clear();
    }

}
