package logicmoo.net;

import logicmoo.*;

// Java
import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;
import logicmoo.*;


public class CycWebServer  extends Thread {
    public boolean listening = true;
    private org.opencyc.webserver.WebServer webserver;

    public CycWebServer() throws IOException {
	webserver = new org.opencyc.webserver.WebServer();
    }

    public void run() {
	webserver.run();
    }

    public synchronized boolean receiveEvent(Object event) {
	return true;
    }

}


