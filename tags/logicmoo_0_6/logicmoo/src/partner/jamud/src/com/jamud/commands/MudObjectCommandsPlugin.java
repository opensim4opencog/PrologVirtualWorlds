package com.jamud.commands;


import jamud.*;
import jamud.command.*;
import jamud.plugin.*;


public class MudObjectCommandsPlugin extends JamudPlugin {

    private static final String PARAM_PRIORITY = "PRIORITY";


    private MudObjectInterpreter mi;
 

    public final String getName() {
	return "Jamud Standard MudObject Commands";
    }

    public final String getVersion() {
	return "1.0";
    }

    public final String getAuthor() {
	return "siege <siege@jamud.com>";
    }

    public final String getInfo() {
	return "These are the standard MudObject commands.";
    }

    
    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state; 
    }    

    public boolean isActive() {
	return (this.state == STATE_INITIALIZED);
    }
    


    public MudObjectCommandsPlugin() {
	super();
    }



    private void initInterpreter() {
	int priority = super.parameters().getAttribute( PARAM_PRIORITY, 1 );

	this.mi = new MudObjectInterpreter(this.getName(), priority, this);

	mi.addCommand(new MudObjectTrusted( "look",    0, "LOOK" ));
	mi.addCommand(new MudObjectTrusted( "lookat",  0, "LOOKAT" ));
	mi.addCommand(new MudObjectTrusted( "go",      0, "MOVE" ));
	mi.addCommand(new MudObjectTrusted( "move",    0, "MOVE" ));
	mi.addCommand(new MudObjectTrusted( "north",   0, "MOVE_NORTH" ));
	mi.addCommand(new MudObjectTrusted( "south",   0, "MOVE_SOUTH" ));
	mi.addCommand(new MudObjectTrusted( "east",    0, "MOVE_EAST" ));
	mi.addCommand(new MudObjectTrusted( "west",    0, "MOVE_WEST" ));
	mi.addCommand(new MudObjectTrusted( "up",      0, "MOVE_UP" ));
	mi.addCommand(new MudObjectTrusted( "down",    0, "MOVE_DOWN" ));
	mi.addCommand(new MudObjectTrusted( "n",       0, "MOVE_NORTH" ));
	mi.addCommand(new MudObjectTrusted( "s",       0, "MOVE_SOUTH" ));
	mi.addCommand(new MudObjectTrusted( "e",       0, "MOVE_EAST" ));
	mi.addCommand(new MudObjectTrusted( "w",       0, "MOVE_WEST" ));
	mi.addCommand(new MudObjectTrusted( "u",       0, "MOVE_UP" ));
	mi.addCommand(new MudObjectTrusted( "d",       0, "MOVE_DOWN" ));
	mi.addCommand(new MudObjectTrusted( "talk",    0, "SAY" ));
	mi.addCommand(new MudObjectTrusted( "say",     0, "SAY" ));
	mi.addCommand(new MudObjectTrusted( "'",       0, "SAY" ));
    }


    public synchronized boolean initialize() {
	System.out.println("begin: MudObjectCommandsPlugin.initialize()");
	if(state > STATE_TERMINATED) {
	    System.out.println(" not yet terminated");
	    System.out.println("end: MudObjectCommandsPlugin.initialize()");
	    return false;
	} else {
	    state = STATE_INITIALIZING;
	}

	if(this.mi == null) {
	    initInterpreter();
	}

	InterpreterManager im = Jamud.currentInstance().interpreterManager();
	im.addInterpreter( this.mi );
	
	state = STATE_INITIALIZED;
	System.out.println("end: MudObjectCommandsPlugin.initialize()");
	return true;
    }
    
    
    public synchronized boolean terminate() {
	System.out.println("begin: MudObjectCommandsPlugin.terminate()");
	if(state < STATE_INITIALIZED) {
	    System.out.println(" not yet initialized");
	    System.out.println("end: MudObjectCommandsPlugin.terminate()");
	    return false;
	} else {
	    state = STATE_TERMINATING;
	}

	InterpreterManager im = Jamud.currentInstance().interpreterManager();
	im.removeInterpreter( this.mi );

	state = STATE_TERMINATED;
	System.out.println("end: MudObjectCommandsPlugin.terminate()");
	return true;
    }

}
