package logicmoo.obj.event;

import logicmoo.obj.*;

/**
*	Representation of an event triggered from a Room. The available events are
*	<ul>
*	<li>onEnter - when a Body enters the Room
*	<li>onExit - when a Body leaves the Room
*	</ul>
*/
public class RoomEvent extends JamudEvent {

	//event 0 is reserved
	public static final int
		EVENT_ENTER = 1,
		EVENT_EXIT = 2;

	public static final String
		EVENT_NAME_ENTER = "onEnter",
		EVENT_NAME_EXIT = "onExit";

	public RoomEvent(Room source, int event, Object[] info) {
		super(source, event, info);
	}

}