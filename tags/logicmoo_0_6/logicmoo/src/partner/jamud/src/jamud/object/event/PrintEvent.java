package jamud.object.event;


import jamud.object.MudObject;
import jamud.object.MudObjectContainer;
import jamud.object.Player;
import java.lang.ref.WeakReference;


public class PrintEvent extends JamudEvent {

    public static final String
	EVENT_STRING = "onPrint";


    // who printed it
    private WeakReference source;

    public MudObject getSource() {
	return (MudObject) this.source.get();
    }


    // what's printed
    private String text;

    public String getText() {
	return this.text;
    }



    public PrintEvent(MudObject trigger, MudObject source, String text) {
	super(trigger, PrintEvent.EVENT_STRING);
	this.source = new WeakReference(source);
	this.text = text;
    }

    public PrintEvent(MudObjectContainer trigger, MudObject source,
		      String text) {
	super(trigger, PrintEvent.EVENT_STRING);
	this.source = new WeakReference(source);
	this.text = text;
    }

    public PrintEvent(Player trigger, String text) {
	super(trigger, PrintEvent.EVENT_STRING);
	this.source = new WeakReference( null );
	this.text = text;
    }



    public void finalize() {
	super.finalize();

	this.source.clear();
	this.text = null;
    }
}
