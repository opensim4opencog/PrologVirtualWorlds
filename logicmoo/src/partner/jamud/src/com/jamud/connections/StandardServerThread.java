package com.jamud.connections;


import java.net.ServerSocket;
import java.net.Socket;
import java.io.InterruptedIOException;


class StandardServerThread extends Thread {

    
    private static final int TIMEOUT = 500;

    
    private StandardServerListener listener;


    private ServerSocket serv_sock = null;


    private int port;

    
    private boolean keepGoing;

    public boolean isAccepting() {
	return this.keepGoing;
    }


    
    public StandardServerThread(StandardServerListener listener, int port) {
	this.listener = listener;
	this.port = port;
    }
    

    public StandardServerThread(StandardServerListener listener,
				ServerSocket serv_sock) {

	this.listener = listener;
	this.serv_sock = serv_sock;

	try {
	    if(serv_sock.getSoTimeout() == 0) {
		serv_sock.setSoTimeout(TIMEOUT);
	    }
	} catch(Exception e) {
	    ; // nuttin'
	}
    }

    

    public void run() {
	this.keepGoing = true;

	try{
	    if(this.serv_sock == null) {
		this.serv_sock = new ServerSocket(this.port);
		this.serv_sock.setSoTimeout(TIMEOUT);
	    }
	    
	    while(this.keepGoing) {
		try {
		    Socket s = serv_sock.accept();
		    if(this.keepGoing) {
			this.listener.serverAccept(this, s);
		    } else {
			s.close();
		    }

		} catch(InterruptedIOException f) {
		    ; // nothing, just to keep us from waiting forever
		}
		this.yield();
	    }
	    
	    this.serv_sock.close();
	    
	} catch(Exception e) {
	    this.halt();
	    this.listener.serverError(this, e);
	}
	
	this.serv_sock = null;
    }
    

    public void halt() {
	if(this.keepGoing) {
	    try {
		this.keepGoing = false;
		if(this.serv_sock != null) {
		    this.sleep( (long) this.serv_sock.getSoTimeout() );
		}
	    } catch(Exception e) {
		; // nuttin'
	    }
	}
    }
    
}
