package logicmoo.obj.event;

import logicmoo.obj.*;

public class ItemEvent extends JamudEvent {

	public static final int
		EVENT_A = 0,
		EVENT_B = 1,
		EVENT_C = 2;

	public static final String
		EVENT_NAME_A = "onA",
		EVENT_NAME_B = "onB",
		EVENT_NAME_C = "onC";

	public ItemEvent(Item source, int event, Object[] info) {
		super(source, event, info);
	}

}