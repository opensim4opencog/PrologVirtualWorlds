package jamud.object.event;


import jamud.object.MudObject;
import jamud.object.MudObjectContainer;
import java.lang.ref.WeakReference;


public class DepartEvent extends JamudEvent {

    public static final String
	EVENT_STRING = "onDepart";


    private WeakReference origin;

    public MudObjectContainer getOrigin() {
	return (MudObjectContainer) this.origin.get();
    }



    public DepartEvent(MudObject trigger, MudObjectContainer origin) {
	super(trigger, DepartEvent.EVENT_STRING);
	this.origin = new WeakReference(origin);
    }



    public void finalize() {
	super.finalize();
	this.origin.clear();
    }

}
