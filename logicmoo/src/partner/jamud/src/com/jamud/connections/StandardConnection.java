package com.jamud.connections;


import jamud.object.*;
import jamud.object.event.*;
import jamud.util.*;
import java.io.*;
import java.net.Socket;


public class StandardConnection extends AbstractConnection
    implements Runnable {


    // our thread
    private Thread thread;


    // our continuation state
    private boolean cont;


    // for keeping track of null lines
    private int agg, aggCount;


    /** Method of identifying what sort of connection this is.
     * This implementation returns "Standard Connection"
     */
    public String getType() {
	return "Standard Connection";
    }


    private Socket socket;


    private OutputStream netout;


    private BufferedReader netin;


    private String remoteaddress;

    public String getRemoteAddress() {
	return this.remoteaddress;
    }


    private int remoteport;

    public int getRemotePort() {
	return this.remoteport;
    }


    private int localport;

    public int getLocalPort() {
	return this.localport;
    }



    public StandardConnection(Socket socket) {
	this.socket = socket;
	this.remoteaddress = new String(socket.getInetAddress().getAddress());
	this.remoteport = socket.getPort();
	this.localport = socket.getLocalPort();
	this.thread = new Thread(this);
    }



    public void start() {
	this.thread.start();
    }


    public void run() {

	System.out.println("begin: StandardConnection.run()");

	try {
	    this.netout = this.socket.getOutputStream();
	    this.netin = new BufferedReader(new
		InputStreamReader( this.socket.getInputStream() ));

	} catch(Exception e1) {
	    e1.printStackTrace();
	    this.setPlayer(null);
	    this.disconnect();
	    return;
	}

	// fork part 1, initialize
	Runnable r = new Runnable() {
		public void run() {
		    if(! getPlayer().initialize()) {
			getPlayer().terminate();
		    }
		}
	    };
	Thread t = new Thread( r );
	t.start();

	// fork part 2, read from socket
	for(this.cont = true; this.cont; ) {
	    try {

		String in = this.netin.readLine();
		if(in == null) {

		    if(this.cont) {
			this.cont = false;
			Player p = this.getPlayer();
			if(p != null) {
			    p.terminate(Player.LOGOUT_ERROR);
			}
		    }

		} else {

		    final int l = in.length();
		    if( l + this.agg == 0 ) {
			this.aggCount++;
		    } else if( this.agg == 0 ) {
			this.aggCount = 0;
		    }
		    
		    if(this.aggCount < 2 && this.cont) {
			in = Util.cleanBackSpace( in );
			this.getPlayer().enqueueCommand( in );
			this.agg = l;
			this.thread.yield();
		    }
		}

	    } catch(Exception e2) {
		if(this.cont) {
		    this.cont = false;

		    e2.printStackTrace();

		    Player p = this.getPlayer();
		    if(p != null) {
			p.terminate(Player.LOGOUT_ERROR);
		    }
		}
	    }
	}

	this.setPlayer(null);

	System.out.println("end: StandardConnection.run()" );
	
    }


    public synchronized void print(String text) {
	if(this.cont) {
	    this.aggCount = 0;

	    try {
		byte[] t = text.getBytes();

		if(this.getPlayer().isFlagged( Colour.PLAYER_COLOUR_FLAG )) {
		    t = AnsiColour.parseColourMarkup( t );
		} else {
		    t = Colour.wipeColour( t );
		    t = Util.convertCrLf( t );
		}

		netout.write( t );
		netout.write( (byte)0 );
		netout.flush();

	    } catch(Exception e) {
		e.printStackTrace();
		disconnect();
	    }
	}
    }


    public synchronized void disconnect() {
	System.out.println( "begin: StandardConnection.disconnect()" );

	this.cont = false;
	if(this.socket != null) {
	    try {
		this.socket.shutdownInput();
		this.socket.shutdownOutput();
		this.socket.close();
	    } catch(Exception e) {
		e.printStackTrace();
	    }
	    this.socket = null;
	}
	System.out.println( "end: StandardConnection.disconnect()" );
    }


    public void finalize() {
	System.out.println( "begin: StandardConnection.finalize()" );
	this.thread = null;
	this.netout = null;
	this.netin = null;
	System.out.println( "end: StandardConnection.finalize()" );
    }

}
