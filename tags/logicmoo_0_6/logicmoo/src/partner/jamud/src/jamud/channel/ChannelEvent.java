package jamud.channel;


import jamud.object.PlayerMask;
import jamud.object.event.JamudEvent;
import java.lang.ref.WeakReference;


public class ChannelEvent extends JamudEvent {


    public static final String
	EVENT_STRING = "onChannelPrint";


    public Channel getChannel() {
	return (Channel) super.getTrigger();
    }


    private WeakReference source;

    public PlayerMask getSource() {
	return (PlayerMask) this.source.get();
    }


    private String text;

    public String getText() {
	return this.text;
    }



    public ChannelEvent(Channel channel, PlayerMask source, String text) {
	super(channel, ChannelEvent.EVENT_STRING);
	this.source = new WeakReference(source);
	this.text = text;
    }



    public void finalize() {
	super.finalize();

	this.source.clear();
	this.text = null;
    }
}
