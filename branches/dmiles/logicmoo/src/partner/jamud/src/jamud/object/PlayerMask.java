package jamud.object;


import jamud.Jamud;
import jamud.object.event.*;
import jamud.util.FlagList;


/**
 * Representation of a player (remote or local) for interactivty
 * with other players.
 */
public abstract class PlayerMask
    implements JamudEventTrigger, JamudEventListener {

    
    public static final int
	TRUST_0GUEST = 0,       //on a guest ID
	TRUST_1NEWBIE = 1,      //early player
	TRUST_2PLAYER = 2,      //normal player
	TRUST_3HERO = 3,        //trusted player
	TRUST_4IMMORTAL = 4,    //hired player, administrator
	TRUST_5GOD = 5,         //between owner-op and hired
	TRUST_6IMPLEMENTOR = 6; //owner-operator. big-daddy
    

    public static final int
	STATUS_DISCONNECTED = 0,
	STATUS_CONNECTED = 1,
	STATUS_AWAY = 2,
	STATUS_QUIET = 3;



    /** list of subscribed channels. Note that modifying this does not
     * actually subscribe a PlayerMask to a channel's ChannelEvents.
     * Rather, subscribing to a Channel will modify this FlagList.
     */
    public abstract FlagList subscribedChannels();

    
    /** is player on THIS mud? if not overridden returns true */
    public boolean isLocal() {
	return true;
    }


    /** normally returns the name of the mud this mask is native to.
     * if not overridden returns this mud's title
     */
    public String getLocale() {
	return Jamud.currentInstance().getMessage(Jamud.MESSAGE_ID);
    }


    /** ShortName is local, LongName if not  */
    public final String getName() {
	if( this.isLocal() ) {
	    return this.getShortName();
	} else {
	    return this.getLongName();
	}
    }


    /** player's name */
    public abstract String getShortName();


    /** player's name and locale */
    public final String getLongName() {
	StringBuffer sb = new StringBuffer( this.getShortName() );
	synchronized( sb ) {
	    sb.append( "@" );
	    sb.append( this.getLocale() );
	    return sb.toString();
	}	
    }


    /** player's title */
    public abstract String getTitle();


    /** player's info */
    public abstract String getInfo();

    
    /** player's trust level */
    public abstract int getTrust();


    private int status;

    /** players availability state */
    public int getStatus() {
	return this.status;
    }

    public void setStatus(int status) {
	this.status = status;
    }

    
    public final boolean equals(Object o) {
	return (this == o);
    }

}
