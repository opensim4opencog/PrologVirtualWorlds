package logicmoo.net;

import logicmoo.*;
import logicmoo.agent.*;
import logicmoo.obj.*;



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
	    while ( listening && !interrupted() ) {
		try {
		    Socket thisClient = serverSocket.accept();
		    System.out.println("Telnet client "+thisClient.getInetAddress().toString()+ " connected");
		    MooTelnetClientHandler clientThread = new MooTelnetClientHandler(thisClient);
		    clients.add(clientThread);
		    clientThread.start();
		} catch ( Exception e ) {
		}
	    }
	    serverSocket.close();
	} catch ( Exception ee ) {
	}
    }

    public synchronized boolean receiveEvent(LogicMooEvent event) {
	return true;
    }
}

