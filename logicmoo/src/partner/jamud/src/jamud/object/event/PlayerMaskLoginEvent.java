package jamud.object.event;


import jamud.object.PlayerMask;


public class PlayerMaskLoginEvent extends JamudEvent {

    public static final String
	EVENT_STRING = "onLogin";


    public PlayerMask getPlayerMask() {
	return (PlayerMask) super.getTrigger();
    }



    public PlayerMaskLoginEvent(PlayerMask whom) {
	super(whom, PlayerMaskLoginEvent.EVENT_STRING);
    }

}
