package com.jamud.commands;


import jamud.command.*;
import jamud.object.Player;
import java.lang.reflect.Method;


class PlayerTrusted extends TrustedCommand {
    
    private Method method;


    private static final Class source = PlayerTrustMethods.class;


    private static final Class[] params = {
	Player.class,
	String.class
    };



    public PlayerTrusted(String name, int trust, Method method) {
	super(name, trust);
	this.method = method;
    }
    
    
    public PlayerTrusted(String name, int trust, String m_name) {
	super(name, trust);

	try {
	    this.method = this.source.getMethod(m_name, this.params);
	} catch(Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }
    

    public boolean allows(Actor a) {
	return ( a instanceof Player &&
		 super.allows(a) );
    }


    public void execute(Actor actor, String args) throws CommandException {
	if(actor != null && actor instanceof Player) {
	    try {
		Object[] pass = { (Player)actor, args};
		method.invoke(null, pass);
	    } catch(Exception e) {
		throw new CommandException(this, actor, e);
	    }
	} else {
	    throw new CommandException(this, actor, "Actor is not a Player");
	}
    }

}
