package com.jamud.commands;


import jamud.command.*;
import jamud.object.*;
import java.lang.reflect.Method;


class MudObjectTrusted extends TrustedCommand {
    
    private Method method;


    private static final Class source = MudObjectTrustMethods.class;

    
    private static final Class[] params = {
	MudObject.class,
	String.class
    };



    public MudObjectTrusted(String name, int trust, Method method) {
	super(name, trust);
	this.method = method;
    }
    
    
    public MudObjectTrusted(String name, int trust, String m_name) {
	super(name, trust);

	try {
	    this.method = this.source.getMethod(m_name, this.params);
	} catch(Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }
    


    public void execute(Actor actor, String args) throws CommandException {
	if(actor == null) {
	    throw new CommandException(this, actor, "Actor is null");
	} else if(actor instanceof Player) {
	    if( (actor = ((Player)actor).getBody()) == null) {
		throw new CommandException(this, actor, "Actor is null");
	    }
	}
	
	if(actor instanceof MudObject) {
	    try {
		MudObject o = (MudObject)actor;
		Object[] pass = { o, args};
		String s = (String)method.invoke(null, pass);
		o.println( s );

	    } catch(Exception e) {
		throw new CommandException(this, actor, e);
	    }
	} else {
	    throw new CommandException(this, actor,
				       "Actor is not a MudObject");
	}
    }


    public boolean allows(Actor a) {
	if(a != null) {
	    if(a instanceof Player) {

		//use the Player's trust
		return (super.allows(a) &&
			((Player)a).getBody() != null );

	    } else if(a instanceof MudObject) {
		return super.allows(a);
	    }
	}
	
	return false;
    }

}
