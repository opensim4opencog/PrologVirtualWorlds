package com.jamud.communications;


import jamud.channel.Channel;
import jamud.command.*;
import jamud.object.*;


class ChannelCommand extends AbstractCommand {


    // our channel
    private Channel channel;



    public ChannelCommand(Channel channel) {
	super( channel.getName() );
	this.channel = channel;
    }



    public boolean allows(Actor a) {
	if(a instanceof Player) {
	    final int t = a.getTrust();
	    return (t >= this.channel.getAclRead() ||
		    t >= this.channel.getAclWrite() );
	} else {
	    return false;
	}
    }


    public void execute(Actor a, String args) throws CommandException {
	if(! (a instanceof Player) ) {
	    throw new CommandException(this, a, "Actor is not a Player");
	}

	Player p = (Player) a;

	String name = super.getName();

	if(args == null || args.length() == 0) {
	    if( p.subscribedChannels().contains(name) ) {
		this.channel.unsubscribe( p );
		p.println( name.concat(" : off") );
		p.ready();
	    } else {
		try {
		    this.channel.subscribe( p );
		    p.println( name.concat(" : on") );
		    p.ready();
		} catch( InsufficientTrustException ite1) {
		    p.println( "You cannot subscribe to the channel '"
			       .concat(name).concat("'") );
		    p.ready();
		}
	    }
	} else {
	    boolean nohear = false;
	    if(! p.subscribedChannels().contains(name) ) {
		try {
		    this.channel.subscribe( p );
		} catch(InsufficientTrustException ite2) {
		    nohear = true;
		}
	    }
	    try {
		this.channel.println( p, args );
		if(nohear) {
		    p.ready();
		}
	    } catch(InsufficientTrustException ite3) {
		throw new CommandException(this, p,
					   "Insufficient trust to write to "
					   .concat(name) );
	    }
	}

    }

}
