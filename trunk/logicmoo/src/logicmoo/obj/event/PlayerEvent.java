package logicmoo.obj.event;

import logicmoo.obj.*;

/**
*	Representation of an event triggered from a Player, or the PlayerManager. The available events are
*	<ul>
*	<li>onLogIn - when this Player logs in
*	<li>onLogOut - when this Player logs out
*	<li>onDataIn - when data is sent from the Player
*	<li>onDataOut - when data is sent to the Player
*	</ul>
*/
public class PlayerEvent extends JamudEvent {

	public static final int
		EVENT_LOGIN = 1,
		EVENT_LOGOUT = 2,
		EVENT_DATAIN = 3,
		EVENT_DATAOUT = 4;

	public static final String
		EVENT_NAME_LOGIN = "onLogIn",
		EVENT_NAME_LOGOUT = "onLogOut",
		EVENT_NAME_DATAIN = "onDataIn",
		EVENT_NAME_DATAOUT = "onDataOut";

	public PlayerEvent(Player source, int event, Object[] info) {
		super(source, event, info);
	}

	public PlayerEvent(PlayerManager source, int event, Object[] info) {
		super(source, event, info);
	}

}