package com.jamud.commands;


import jamud.*;
import jamud.command.*;
import jamud.plugin.*;
import net.n3.nanoxml.XMLElement;


public class PlayerCommandsPlugin extends JamudPlugin {


    private static final String PARAM_PRIORITY = "PRIORITY";


    private AbstractInterpreter pi;


    public final String getName() {
	return "Jamud Standard Player Commands";
    }

    public final String getVersion() {
	return "1.0";
    }

    public final String getAuthor() {
	return "siege <siege@jamud.com>";
    }

    public final String getInfo() {
	return "These are the standard Player commands.";
    }

    
    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state; 
    }    

    public boolean isActive() {
	return (this.state == STATE_INITIALIZED);
    }
    


    public PlayerCommandsPlugin() {
	super();
    }



    public synchronized boolean initialize() {
	System.out.println("begin: PlayerCommandsPlugin.initialize()");
	if(state > STATE_TERMINATED) {
	    System.out.println(" not yet terminated");
	    System.out.println("end: PlayerCommandsPlugin.initialize()");
	    return false;
	} else {
	    state = STATE_INITIALIZING;
	}

	if(this.pi == null) {
	    initInterpreter();
	}

	InterpreterManager im = Jamud.currentInstance().interpreterManager();
	im.addInterpreter( this.pi );
	
	state = STATE_INITIALIZED;
	System.out.println("end: PlayerCommandsPlugin.initialize()");
	return true;
    }
    
    
    public synchronized boolean terminate() {
	System.out.println("begin: PlayerCommandsPlugin.terminate()");
	if(state != STATE_INITIALIZED) {
	    System.out.println(" not yet terminated");
	    System.out.println("end: PlayerCommandsPlugin.terminate()");
	    return false;
	} else {
	    state = STATE_TERMINATING;
	}

	InterpreterManager im = Jamud.currentInstance().interpreterManager();
	im.removeInterpreter( this.pi );

	state = STATE_TERMINATED;
	System.out.println("end: PlayerCommandsPlugin.terminate()");
	return true;
    }


    public void load(XMLElement xml) throws Exception {
	super.load( xml );

	this.initInterpreter();
    }


    private void initInterpreter() {
	int priority = super.parameters().getAttribute( PARAM_PRIORITY, 0 );

	this.pi = new PlayerInterpreter( this.getName(), priority, this );

	pi.addCommand(new PlayerTrusted( "*",       0, "ENACT" ));
	pi.addCommand(new PlayerTrusted( "enact",   0, "ENACT" ));
	pi.addCommand(new PlayerTrusted( "quit",    0, "LOGOUT" ));
	pi.addCommand(new PlayerTrusted( "odump",   0, "OBJECT_DUMP" ));
	pi.addCommand(new PlayerTrusted( "color",   0, "COLOUR" ));
	pi.addCommand(new PlayerTrusted( "colour",  0, "COLOUR" ));
    }
    
}
