package logicmoo.obj.event;

import logicmoo.obj.*;

/**
*	Representation of an event triggered from a PlayerMask. The available events are
*	<ul>
*	<li>onUpdate - when the PlayerMask updates its information
*	</ul>
*/
public class PlayerMaskEvent extends JamudEvent {

	public static final int
		EVENT_UPDATE = 1;

	public static final String
		EVENT_NAME_UPDATE = "onUpdate";

	public PlayerMaskEvent(PlayerMask source, int event, Object[] info) {
		super(source, event, info);
	}

}