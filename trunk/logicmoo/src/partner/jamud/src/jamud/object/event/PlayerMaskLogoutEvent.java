package jamud.object.event;


import jamud.object.PlayerMask;


public class PlayerMaskLogoutEvent extends JamudEvent {

    public static final String
	EVENT_STRING = "onLogout";


    public PlayerMask getPlayerMask() {
	return (PlayerMask) super.getTrigger();
    }



    public PlayerMaskLogoutEvent(PlayerMask whom) {
	super(whom, PlayerMaskLogoutEvent.EVENT_STRING);
    }

}
