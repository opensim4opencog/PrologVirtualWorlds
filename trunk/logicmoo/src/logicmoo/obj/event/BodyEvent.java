package logicmoo.obj.event;

import logicmoo.obj.*;

/**
*	Representation of an event triggered from a Body. The available events are
*	<ul>
*	<li>onText - text passed to Body
*	<li>onArrive - when the Body arrives in a Room
*	<li>onDepart - when the Body leaves the Room
*	<li>onEnter - when another Body enters the Room this Body is in
*	<li>onExit - when another Body leaves the Room this Body is in
*	</ul>
*/
public class BodyEvent extends JamudEvent {

	public static final int
		EVENT_TEXT = 1,
		EVENT_ARRIVE = 2,
		EVENT_DEPART = 3,
		EVENT_ENTER = 4,
		EVENT_EXIT = 5;

	public static final String
		EVENT_NAME_TEXT = "onText",
		EVENT_NAME_ARRIVE = "onArrive",
		EVENT_NAME_DEPART = "onDepart",
		EVENT_NAME_ENTER = "onEnter",
		EVENT_NAME_EXIT = "onExit";

	public BodyEvent(Body source, int event, Object[] info) {
		super(source, event, info);
	}

}