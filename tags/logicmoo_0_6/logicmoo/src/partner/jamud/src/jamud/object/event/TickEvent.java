package jamud.object.event;


public class TickEvent extends JamudEvent {


    public static final String
	EVENT_STRING = "onTick";


    public TickEvent(JamudEventTrigger src) {
	super(src, TickEvent.EVENT_STRING);
    }


}
