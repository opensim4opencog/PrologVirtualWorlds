package jamud.channel;

import jamud.object.PlayerMask;
import jamud.object.event.JamudEvent;
import java.lang.ref.WeakReference;


public class TellEvent extends JamudEvent {


    public static final String
	EVENT_STRING = "onTell";


    private WeakReference source;

    public PlayerMask getSource() {
	return (PlayerMask) this.source.get();
    }


    private String text;

    public String getText() {
	return this.text;
    }



    public TellEvent(PlayerMask trigger, PlayerMask source, String text) {
	super(trigger, TellEvent.EVENT_STRING);
	this.source = new WeakReference(source);
	this.text = text;
    }



    public void finalize() {
	super.finalize();
	this.source.clear();
	this.text = null;
    }
}
