package logicmoo.obj.event;

import logicmoo.obj.*;

public class AreaEvent extends JamudEvent {

	public static final int
		EVENT_ENTER = 1,
		EVENT_EXIT = 2;

	public static final String
		EVENT_NAME_ENTER = "onEnter",
		EVENT_NAME_EXIT = "onExit";

	public AreaEvent(Area source, int event, Object[] info) {
		super(source, event, info);
	}

}