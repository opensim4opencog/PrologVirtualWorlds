package com.jamud.commands.olc;


import jamud.*;
import jamud.command.*;
import jamud.plugin.*;
import java.io.*;
import java.util.*;
import net.n3.nanoxml.*;


public class BuilderCommandsPlugin extends JamudPlugin {
    
    private static final String
	PARAM_PRIORITY = "PRIORITY";

    private static final String
	PROPERTY_BUILDERS = "BUILDERS",
	PROPERTY_SRC = "SRC";
    
    
    public final String getName() {
	return "BuilderCommand Commands (Builders)";
    }
    
    public final String getVersion() {
	return "1.0";
    }
    
    public final String getAuthor() {
	return "neph <dmiles@teknowledge.com>";
    }
    
    public final String getInfo() {
	return "The builder commands (aka builders). Emotive emulation of"
	    .concat( " common activities." );
    }
    
    
    private int state = STATE_TERMINATED;
    
    public int initializableState() {
	return this.state;
    }    
    
    public boolean isActive() {
	return (this.state == STATE_INITIALIZED);
    }
    
    
    
    public BuilderCommandsPlugin() {
	super();

	builders = new BuilderInterpreter("The OLC Interpreter", 2);
	Command c = new BuilderCommand("@");
	builders.addCommand( c );
    }
    
    
    private BuilderInterpreter builders;
    
    
    public synchronized boolean initialize() {
	System.out.println("begin: BuilderCommandsPlugin.initialize()");

	if(state > STATE_TERMINATED) {
	    System.out.println(" not yet terminated");
	    System.out.println("end: BuilderCommandsPlugin.initialize()");
	    return false;
	} else {
	    state = STATE_INITIALIZING;
	}
	

	Jamud.currentInstance().interpreterManager().addInterpreter(builders);
	
	state = STATE_INITIALIZED;
	System.out.println("end: BuilderCommandsPlugin.initialize()");
	return true;
    }
    
    
    public synchronized boolean terminate() {
	System.out.println("begin: BuilderCommandsPlugin.terminate()");

	if(state < STATE_INITIALIZED) {
	    System.out.println(" not yet initialized");
	    System.out.println("end: BuilderCommandsPlugin.terminate()");
	    return false;

	} else {
	    state = STATE_TERMINATING;
	}
	
	Jamud.currentInstance().interpreterManager().removeInterpreter(builders);
	
	state = STATE_TERMINATED;
	System.out.println("end: BuilderCommandsPlugin.terminate()");
	return true;
    }
    
    
    public void load(IXMLElement xml) throws Exception {
	super.load(xml);
    }
    
} 
