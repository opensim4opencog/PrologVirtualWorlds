package com.jamud.communications;


import jamud.Jamud;
import jamud.channel.*;
import jamud.command.*;
import jamud.plugin.*;
import java.util.*;
import net.n3.nanoxml.IXMLElement;


public class ChannelsPlugin extends JamudPlugin {


    private static final String PARAM_PRIORITY = "PRIORITY";


    public final String getName() {
	return "Channels Plugin";
    }

    public final String getVersion() {
	return "1.0";
    }

    public final String getAuthor() {
	return "siege <siege@jamud.com>";
    }

    public final String getInfo() {
	return "Channels and related commands.";
    }


    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state;
    }


    public boolean isActive() {
	return (this.state == STATE_INITIALIZED);
    }


    private ArrayList channels;

    private AbstractInterpreter commands;



    public ChannelsPlugin() {
	super();
	this.channels = new ArrayList();
    }



    public synchronized boolean initialize() {
	if(this.state > STATE_TERMINATED) {
	    return false;
	} else {
	    this.state = STATE_INITIALIZING;
	}

	Jamud j = Jamud.currentInstance();

	// add all our channels
	ChannelManager cm = j.channelManager();
	synchronized( cm ) {
	    for(int I = this.channels.size(); I-- > 0; ) {
		cm.addChannel( (Channel) this.channels.get(I) );
	    }
	}

	// add all our channel commands
	InterpreterManager im = j.interpreterManager();
	im.addInterpreter( this.commands );

	this.state = STATE_INITIALIZED;
	return true;
    }


    public synchronized boolean terminate() {
	if(this.state < STATE_INITIALIZED) {
	    return false;
	} else {
	    this.state = STATE_TERMINATING;
	}

	Jamud j = Jamud.currentInstance();

	// remove all our channel commands
	InterpreterManager im = j.interpreterManager();
	im.removeInterpreter( this.commands );

	// remove all our channels
	ChannelManager cm = Jamud.currentInstance().channelManager();
	synchronized( cm ) {
	    for(int I = this.channels.size(); I-- > 0; ) {
		cm.removeChannel( (Channel) this.channels.get(I) );
	    }
	}

	this.state = STATE_TERMINATED;
	return true;
    }


    public void load(IXMLElement xml) throws Exception {
	super.load( xml );

	final int priority = parameters().getAttribute(PARAM_PRIORITY, 1);
	this.commands = new PlayerInterpreter(getName(), priority, this);

	for(Enumeration enum = xml.getChildrenNamed(Channel.MARKUP).
		elements(); enum.hasMoreElements(); ) {

	    IXMLElement nxt = (IXMLElement) enum.nextElement();
	    Channel chan = Channel.loadChannel( nxt );

	    if(chan.getName() != null) {
		this.channels.add(chan);

		ChannelCommand com = new ChannelCommand( chan );
		this.commands.addCommand( com );
	    }
	}
    }

}
