package com.jamud.connections;


import jamud.Jamud;
import jamud.object.Player;
import jamud.object.PlayerManager;
import jamud.plugin.*;
import java.net.Socket;


public class StandardServer extends JamudPlugin {


    public static final int DEFAULT_PORT = 8808;

    private static final String PARAM_PORT = "PORT";


    public final String getName() {
	return "Standard Server";
    }

    public final String getVersion() {
	return "1.0";
    }

    public final String getAuthor() {
	return "siege <siege@jamud.com>";
    }

    public final String getInfo() {
	return "This is the standard player connection server.";
    }


    private StandardServerThread serv;


    private StandardServerListener servlistener;

    
    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state;
    }



    public StandardServer() {
	super();

	this.servlistener = new StandardServerListener() {
		public void serverAccept(StandardServerThread s, Socket sock) {
		    StandardConnection sc = new StandardConnection(sock);

		    new Player( sc );

		    sc.start();
		}
		public void serverError(StandardServerThread s,
					Exception e) {
		    try {
			terminate();
		    } catch(Exception f) {
			; //nuttin'
		    }
		}
	    };
    }



    public synchronized boolean initialize() {
	System.out.println( "begin: StandardServer.initialize()" );
	if(this.state > STATE_TERMINATED) {
	    System.out.println( " not yet terminated" );
	    System.out.println( "end: StandardServer.initialize()" );
	    return false;
	} else {
	    this.state = STATE_INITIALIZING;
	}

	try {	    
	    if(serv == null) {
		int ip = parameters().getAttribute( PARAM_PORT, DEFAULT_PORT );
		serv = new StandardServerThread( this.servlistener, ip );
	    }
	    serv.start();

	} catch(Exception e) {
	    e.printStackTrace();

	    this.state = STATE_TERMINATED;
	    System.out.println( "end: StandardServer.initialise()" );
	    return false;
	}

	this.state = STATE_INITIALIZED;
	System.out.println( "end: StandardServer.initialise()" );
	return true;
    }


    public synchronized boolean terminate() {
	System.out.println( "begin: StandardServer.terminate()" );
	if(this.state < STATE_INITIALIZED) {
	    System.out.println( " not yet initialized" );
	    System.out.println( "end: StandardServer.terminate()" );
	    return false;
	} else {
	    this.state = STATE_TERMINATING;
	}

	// shut it down
	serv.halt();

	this.state = STATE_TERMINATED;
	System.out.println( "end: StandardServer.terminate()" );
	return true;
    }


    public boolean isActive() {
	return (this.state == STATE_INITIALIZED);
    }

}
