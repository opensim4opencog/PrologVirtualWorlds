package logicmoo.net;

import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;


public class TelnetProxyServer extends Thread {

    public boolean listening = true;
    private ServerSocket serverSocket = null;
    private int serverPort = 3666;
    private int clientPort = 3601;
    public static ArrayList clients = new ArrayList();

    public static void main(String[] arg) {
	try {
	    TelnetProxyServer tps = new TelnetProxyServer(new Integer(arg[0]).intValue(),new Integer(arg[1]).intValue());
	    tps.run();
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
    }

    public TelnetProxyServer() throws IOException {
	serverSocket = new ServerSocket(serverPort);
    }

    public TelnetProxyServer(int port, int toport) throws IOException {
	serverSocket = new ServerSocket(port);
    }

    public void run() {
	try {
	    while ( listening && !interrupted() ) {
		try {
		    clients.add(new SockToSock(serverSocket.accept(),new Socket("localhost",clientPort)));
		} catch ( Exception e ) {
		    e.printStackTrace();
		}
	    }
	    serverSocket.close();
	} catch ( Exception ee ) {
	}
    }

    private class SockToSock {
	private InToOut s1;
	private InToOut s2;
	public SockToSock(Socket left, Socket right)  throws IOException {
	    //System.out.println("" + left + "->" + left);
	    s1 = new InToOut(left,right);
	    s2 = new InToOut(right,left);
	    s1.start();
	    s2.start();
	}
    }

    private class InToOut extends Thread {
	private InputStream ins;
	private OutputStream outs;

	public InToOut(Socket in, Socket out) throws IOException {
	    ins = in.getInputStream();
	    outs = out.getOutputStream();
	}

	public void run() {
	    byte[] c={0};
	    System.out.println("" + ins + "->" + outs);
	    try {
		while ( !this.interrupted() ) {
		    ins.read(c);
		    outs.write(c);
		}
	    } catch ( Exception e ) {
		e.printStackTrace();
		try {
		    ins.close();
		    outs.close();
		} catch ( IOException ee ){
		    ee.printStackTrace();
		}
	    }
	}
    }
}

    
