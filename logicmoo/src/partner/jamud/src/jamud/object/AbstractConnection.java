package jamud.object;


import jamud.object.event.*;


public abstract class AbstractConnection implements Connection {


    /** returns "unspecified" unless overridden */
    public String getType() {
	return "unspecified";
    }


    // the listener who catches PrintEvents on player
    private JamudEventListener printlistener;


    // player associated with this connection
    private Player player;

    public Player getPlayer() {
	return this.player;
    }

    public void setPlayer(Player a_player) {
	final Player old = this.player;

	if(a_player != old) {
	    if(old != null) {
		old.removeListener( this.printlistener );
	    }

	    if(a_player != null) {
		a_player.addListener( this.printlistener, PrintEvent.class );
	    }

	    this.player = a_player;
	}
    }


    // long set when created by system time
    private long when;

    public long connectedAt() {
	return this.when;
    }



    /** Default Constructor */
    public AbstractConnection() {
	this.when = System.currentTimeMillis();

	this.printlistener = new JamudEventListener() {
		public void eventTriggered(JamudEvent event) {
		    if( event instanceof PrintEvent ) {
			PrintEvent p = (PrintEvent) event;
			if( p.getText() != null &&
			    p.getTrigger() == player ) {
			    print( p.getText() );
			}
		    }
		}
	    };
    }


    public void finalize() {
	this.printlistener = null;
	this.player = null;
    }


    public final boolean equals(Object o) {
	return (this == o);
    }

}
