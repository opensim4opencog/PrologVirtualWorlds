package com.jamud.communications;


import jamud.Jamud;
import jamud.board.*;
import jamud.command.*;
import jamud.plugin.*;
import java.util.*;
import net.n3.nanoxml.IXMLElement;


public class ThreadedBoardsPlugin extends JamudPlugin {


    private static final String PARAM_PRIORITY = "PRIORITY";


    public final String getName() {
	return "Threaded boards plugin";
    }

    public final String getVersion() {
	return "1.0";
    }

    public final String getAuthor() {
	return "siege <siege@jamud.com>";
    }

    public final String getInfo() {
	return "Threaded boards/posts and related commands.";
    }


    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state;
    }


    public boolean isActive() {
	return (this.state == STATE_INITIALIZED);
    }


    private ArrayList boards;

    private AbstractInterpreter commands;



    public ThreadedBoardsPlugin() {
	super();
	this.boards = new ArrayList();
    }



    public synchronized boolean initialize() {
	if(this.state > STATE_TERMINATED) {
	    return false;
	} else {
	    this.state = STATE_INITIALIZING;
	}

	// init

	this.state = STATE_INITIALIZED;
	return true;
    }


    public synchronized boolean terminate() {
	if(this.state < STATE_INITIALIZED) {
	    return false;
	} else {
	    this.state = STATE_TERMINATING;
	}

	// term

	this.state = STATE_TERMINATED;
	return true;
    }


    public void load(IXMLElement xml) throws Exception {
	super.load( xml );

	final int priority = parameters().getAttribute(PARAM_PRIORITY, 1);
	this.commands = new PlayerInterpreter(getName(), priority, this);

	for(Enumeration enum = xml.getChildrenNamed(Board.MARKUP).
		elements(); enum.hasMoreElements(); ) {
	    IXMLElement nxt = (IXMLElement) enum.nextElement();
	}
    }

}
