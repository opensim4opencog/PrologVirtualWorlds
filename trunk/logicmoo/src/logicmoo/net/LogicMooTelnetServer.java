package logicmoo.net;

import logicmoo.*;
import logicmoo.agent.*;



import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;


public class LogicMooTelnetServer extends Thread {

    public boolean listening = true;
    private ServerSocket serverSocket = null;
    private int serverPort = 4444;
    public static ArrayList clients = new ArrayList();

    public LogicMooTelnetServer() throws IOException {
	serverSocket = new ServerSocket(serverPort);
    }
    
    public LogicMooTelnetServer(int port) throws IOException {
	serverSocket = new ServerSocket(port);
    }

    public void run() {
	try {
	    while ( listening ) {
		Socket thisClient = serverSocket.accept();
		System.out.println(thisClient.getInetAddress().toString()+ " connected");
		MooTelnetClientHandler clientThread = new MooTelnetClientHandler(thisClient);
		System.out.println(thisClient.getInetAddress().toString()+ " connected to " +clientThread );
		clients.add(clientThread);
		clientThread.start();
	    }
	    serverSocket.close();
	} catch ( Exception e ) {
	}
    }

    public synchronized boolean receiveEvent(Object event) {
	return true;
    }


}

