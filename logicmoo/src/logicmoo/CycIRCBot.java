package logicmoo;

import bsh.*;
import bsh.util.*;
import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;
import java.awt.*;
import java.net.*;

import jamud.*;

import jamud.command.*;
import jamud.object.*;
import jamud.object.event.*;
import jamud.util.*;
import com.jamud.commands.*;
import com.jamud.connections.*;
import com.jamud.communications.*;
import com.jamud.creation.*;
import net.n3.nanoxml.*;
import org.opencyc.webserver.WebServer;
import org.opencyc.util.*;
import org.opencyc.api.*;
import org.opencyc.chat.*;
import org.opencyc.cycobject.*;
import ViolinStrings.*;


public class CycIRCBot extends Thread  implements ChatSender {

    protected Socket ircServerSocket =null;
    protected InputStream ircInputStream = null;
    protected OutputStream ircOutputStream = null;
    protected BufferedReader ircInputReader = null;
    protected BufferedWriter ircOutputWriter = null;
    protected static CycMooAccess cycAccess = null;
    protected static bsh.Interpreter bshInterpeter = null;

    /**
     * reference to ChatterBot
     */

    protected org.opencyc.chat.ChatterBot chatterBot = null;

    public boolean running;

    // bot details
    public String ircNick = "Cycbot";
    public String ircComment = "http://logicmoo.sourceforge.net";
    public String ircChannel = "#opencyc";
    public String ircServer = "irc.openprojects.net";
    public int ircPort = 6667;

    public String ircDestination = "#opencyc";
    public String ircDebug = "#cycbot";

    /**
     * A Basic IRC Bot
     */
    public CycIRCBot() {
    }

    /**
     * A Full IRC Bot
     */
    public CycIRCBot(CycMooAccess access, String nick, String comment, String server, int port, String channel) {
	// set the bot's nickname and description
	ircNick = nick;
	ircComment = comment;
	ircServer = server;
	ircPort = port;
	cycAccess = access;
	ircConnect();
	if ( channel!=null ) ircJoin(channel);
    }


    /**
     * Provides a main function to launch the ConsoleChat application.  The
     * args are not used.
     *
     * @param args not used
     */
    public static void main(String[] args) {

    }

    public void ircJoin(String channel) {
	serverSend("JOIN "+channel);
    }

    public void ircPart(String channel) {
	serverSend("PART "+channel);
    }

    /**
     * Connects to an IRC server
     */

    public void ircConnect() {
	Log.makeLog();
	try {
	    ircServerSocket = new Socket(ircServer, ircPort);
	} catch ( Exception e ) {
	    System.err.println("error Connecting to IRC server");
	    e.printStackTrace();
	}

	try {
	    ircInputStream = ircServerSocket.getInputStream();    
	    ircOutputStream = ircServerSocket.getOutputStream();
	} catch ( Exception e ) {
	    System.err.println("error opening streams to IRC server");
	    e.printStackTrace();                                   
	}

	ircInputReader = new BufferedReader(new InputStreamReader(ircInputStream));
	ircOutputWriter = new BufferedWriter(new OutputStreamWriter(ircOutputStream));  

	try {
	    // send user info
	    ircOutputWriter.write("user " + ircNick + " logicmoo irc :" + ircComment);
	    ircOutputWriter.newLine();
	    ircOutputWriter.write("nick " + ircNick);
	    ircOutputWriter.newLine();
	    ircOutputWriter.flush();
	} catch ( Exception e ) {
	    System.out.println("ircLogOn error: " + e);
	}
	ircJoin(ircChannel);
	ircJoin(ircDebug);
	ircJoin("#ai");
	this.running    = true;
	startChatterBot();
	if ( bshInterpeter==null ) {
	    // Interpreter(java.io.Reader in, java.io.PrintStream out, java.io.PrintStream err, boolean interactive) 

	    bshInterpeter = new bsh.Interpreter(new InputStreamReader(System.in),(PrintStream)System.out ,(PrintStream)System.out,false);
	    bshInterpeter.run();
	}
	return;

    }
    /**
     * Disconnct from an IRC server
     */
    public void ircDisconnect() {
	try {
	    chatterBot.finalize();
	    chatterBot = null;
	    ircOutputWriter.write("QUIT ircDisconnect()");
	    ircOutputWriter.newLine();
	    ircOutputWriter.flush();
	} catch ( Exception e ) {
	    System.out.println("ircLogOff error: " + e);
	}

	// close the IO streams to the IRC server
	try {
	    ircInputReader.close();
	    ircOutputWriter.close();
	    ircInputReader = null;
	    ircOutputWriter = null;
	    ircServerSocket.close();
	    ircServerSocket = null;

	} catch ( IOException e ) {
	    System.err.println("Error ircDisconnecting from IRC server");
	    e.printStackTrace();
	}

    }
    /**
     * Sends a raw string to the IRC server
     */
    public boolean serverSend(String message) {

	System.out.println("irc: '" + message + "'");

	try {
	    ircOutputWriter.write(message);
	    ircOutputWriter.newLine();
	    ircOutputWriter.flush();
	} catch ( IOException e ) {
	    return false;
	}
	return true;
    }

    /**
     * Send a notice to an IRC user
     * @param destination String
     * @param message String
     */
    public void sendNotice(String destination, String message) {
	serverSend("notice " + destination + " :" + message);
    }
    /**
     * Send a public message to an IRC user
     * @param destination String
     * @param message String
     */
    public boolean sendMessage(String destination, Object post) {

	if ( post==null ) return false;

	// Wait a 1/2 sec (Keeps from flooding off server)
	try {
	    Thread.sleep(500);
	} catch ( InterruptedException e ) {
	}

	if ( post instanceof Iterator ) {
	    while ( ((Iterator)post).hasNext() ) {
		try {
		    if ( ircInputReader.ready()) if  (ircInputReader.readLine().trim().endsWith(".")) return true;
		} catch ( Exception e ) {
		}
		sendMessage(destination,((Iterator)post).next());
	    }
	    return true;
	}

	String message = post.toString().trim();

	try {
	    if ( ViolinStrings.Strings.contains(message,"\n") || ViolinStrings.Strings.contains(message,"\r") ) {
		BufferedReader mr =  new BufferedReader(new StringReader(message));
		String line = null;
		while ( (line = mr.readLine()) != null ) sendMessage(destination,line);
	    } else {
		if ( message.length() > 120 ) {
		    serverSend("privmsg " + destination + " :" + message.substring(0,120));
		    return sendMessage(destination,message.substring(121));
		} else
		    return serverSend("privmsg " + destination + " :" + message);
	    }
	    return true;
	} catch ( Exception e ) {
	    System.out.println(""+e);
	    return false;
	}
    }

    public void run() {
	while ( !this.interrupted() && running )
	    try {
		this.serviceAll();
	    } catch ( Exception e ) {
		System.out.println(""+e);
	    }
    }


    /**
     * Receives and handles IRC messages
     */
    public void serviceAll() throws Exception {

	// Wait a 1/10th sec
	try {
	    Thread.sleep(100);
	} catch ( InterruptedException e ) {
	}

	// Data ?
	if ( !ircInputReader.ready() ) return;

	String message = ircInputReader.readLine();

	System.out.println(message);

	// send a pong back
	if ( message.substring(0,4).equalsIgnoreCase("ping") ) {
	    serverSend("pong " + message.substring(5));
	    return;
	}

	String prefix = null;
	String command = null;
	String params = null;
	String user = null;


	// check for the prefix
	if ( message.substring(0,1).equals(":") ) {
	    prefix = message.substring(1, message.indexOf(' '));
	    message = message.substring(message.indexOf(' ') + 1);
	}

	// extract the command
	command = message.substring(0, message.indexOf(' '));

	// get the parameters (the rest of the message)
	params = message.substring(message.indexOf(' ') + 1);

	System.out.println("prefix: '" + prefix + "' command: '" + command + "' params: '" + params + "'");

	if ( params.toLowerCase().startsWith(":closing") ) {
	    ircConnect();
	    return;
	}

	try {
	    int col = params.indexOf(':');
	    String destination = params.substring(0,col-1).trim();
	    ircDestination = destination;
	    params = params.substring(col+1).trim();

	    serviceIRCMsg(prefix.substring(0, prefix.indexOf('!')),prefix,command,destination, params);
	} catch ( Exception e ) {
	}
    }

    /**
     * Process a private message
     * @param destination String nickname of the user who sent the message
     * @param message String the command
    
    params are in the form
    <my nick> :<message> 
    or
    <my nick> <message>	
    
     */
    public void serviceIRCMsg(String from, String hostmask, String command, String destination,String params) {

	if ( hostmask.startsWith(ircNick) ) return;
	ircDestination = destination;
	if ( command.equalsIgnoreCase("PRIVMSG") )
	    servicePublicMessage(from, hostmask, destination, params.trim());
    }

    public void restartChatterBot() {
	terminateChatterBot();
	startChatterBot();
    }

    public void terminateChatterBot() {
	if ( chatterBot!=null ) {
	    try {
		chatterBot.finalize();
	    } catch ( Exception e ) {
		e.printStackTrace(System.err);
	    }
	    chatterBot = null;
	}
    }

    public void startChatterBot() {
	if ( chatterBot==null ) {
	    try {
		chatterBot = new ChatterBot(this);
		chatterBot.initialize();
	    } catch ( Exception e ) {
		e.printStackTrace(System.err);
	    }
	}
    }

    public void servicePublicMessage(String from, String hostmask, String returnpath,String params) {
	if ( params.toLowerCase().startsWith("hello") ) {
	    sendMessage(returnpath, "hello " + from);
	    return;
	}
	if ( params.toLowerCase().startsWith("time") ) {
	    sendMessage(returnpath, "the time is " + (new Date()).toString());
	    return;
	}
	if ( params.toLowerCase().startsWith("restart") ) {
	    restartChatterBot();
	    //sendMessage(returnpath,"Restarted " + chatterBot);
	    return;
	}

	try {
	    if ( chatterBot!=null )
		chatterBot.receiveChatMessage(from,from,params);
	} catch ( Exception e ) {
	    e.printStackTrace(System.out);
	    sendDebug(""+e);
	}

	int ccol = params.indexOf(':');

	if ( ccol>1 ) {
	    String token = params.substring(0,ccol).trim();
	    params = params.substring(ccol+1).trim();
	    servicePublicMessage(from, hostmask, returnpath, token,params);      
	}
	return;
    }

    public void servicePublicMessage(String from, String hostmask, String returnpath,String token,String params) {
	System.out.println("token: '" + token + "' params: '" + params + "'");
	if ( token.equalsIgnoreCase("echo") ) {
	    sendMessage(returnpath, params);
	    return;
	}
	if ( token.equalsIgnoreCase("cyclify") ) {
	    sendMessage(returnpath, toCycListString(params));
	    return;
	}
	if ( token.equalsIgnoreCase("subl") ) {
	    serviceSubL(from,returnpath, params);
	    return;
	}
	if ( token.equalsIgnoreCase("debug") ) {
	    ircDebug = params;
	    return;
	}
	if ( token.equalsIgnoreCase("query") ) {
	    serviceQuery(from,returnpath, params);
	    return;
	}
	if ( token.equalsIgnoreCase("prove") ) {
	    serviceProve(from,returnpath, params);
	    return;
	}
	if ( token.equalsIgnoreCase("ask") ||  token.equalsIgnoreCase("?") ) {
	    serviceQueryUser(from,returnpath, params);
	    return;
	}
	if ( token.equalsIgnoreCase("assert") || token.equalsIgnoreCase("+") ) {
	    serviceAssert(from,returnpath, params);
	    return;
	}
	if ( token.equalsIgnoreCase("bsh") ) {
	    try {
		bshInterpeter.eval(params);
	    } catch ( Exception e ) {
		sendMessage(returnpath,""+e);
	    }
	    return;
	}
	if ( token.equalsIgnoreCase("join") ) {
	    ircJoin(params);
	    return;
	}

	if ( token.equalsIgnoreCase("part") ) {
	    ircPart(params);
	    return;
	}
	if ( token.equalsIgnoreCase("putserv") ) {
	    serverSend(params);
	    return;
	}

    }

    public void serviceSubL(String cyclist,String returnpath, String subl) {
	try {
	    sendAnswers(returnpath,cycAccess.converseObject(subl));
	} catch ( Exception e ) {
	    sendMessage(returnpath, ""+ e    /*  + "\" " + " trying to eval \"" + subl + "\" " + "from \"" + cyclist + "\" with returnpath \""+returnpath + "\""*/ );
	}
    }

    public void serviceQuery(String cyclist,String returnpath, String query) {
	try {
	    sendAnswers(returnpath, cycAccess.converseObject( "(cyc-query '" +toCycListString(query) + " #$InferencePSC)"));
	} catch ( Exception e ) {
	    sendMessage(returnpath, ""+ e);
	}
    }
    public void serviceProve(String cyclist,String returnpath, String query) {
	try {
	    sendAnswers(returnpath, cycAccess.converseObject( "(fi-prove '" +toCycListString(query) + " #$InferencePSC)"));
	} catch ( Exception e ) {
	    sendMessage(returnpath, ""+ e);
	}
    }

    public void serviceQueryUser(String cyclist,String returnpath, String query) {
	try {
	    sendAnswers(returnpath, cycAccess.converseObject( "(cyc-query '" +toCycListString(query) + " " + mtForUser(cyclist).stringApiValue() + ")"));
	} catch ( Exception e ) {
	    sendMessage(returnpath, ""+ e);
	}
    }

    public void serviceAssert(String cyclist,String returnpath, String sent) {
	serviceSubL(cyclist,returnpath, "(cyc-assert '" +toCycListString(sent) + " " + mtForUser(cyclist).stringApiValue() + ")");
    }

    public CycFort mtForUser(String cyclist) {
	try {
	    CycConstant mt = cycAccess.makeCycConstant("#$"+cyclist+"ChatMt");
	    cycAccess.assertIsa(mt,cycAccess.microtheory,cycAccess.baseKB);
	    return(CycFort)mt;
	} catch ( Exception e ) {
	    return cycAccess.jamudMt; 
	}
    }

    public String toCycListString(String sentence) {
	try {
	    return(((CycList)((new CycListKifParser(cycAccess)).read(sentence))).cyclify());
	} catch ( Exception e ) {
	    return null;
	}

    }

    public void sendAnswers(String returnpath, Object in) {
	if ( in instanceof CycList ) {
	    CycList answers = (CycList) in;
	    int size = answers.size();
	    if ( size > 5 && size < 51 ) {
		sendMessage(returnpath,answers.iterator());
		return;
	    }
	    if ( size>50 ) {
		sendMessage(returnpath,"Your question returned " + size + " answers .. please refine. ");
		return;
	    }
	}
	sendMessage(returnpath,in);

    }

    /**
     * Sends the chat message from Cyc into the chat system.
     */
    public void sendChatMessage(String chatMessage) {
	sendMessage(ircDestination,chatMessage);
    }

    /**
     * Receives chat messages from the user.
     */
    protected String receiveChatMessage() throws IOException {
	System.out.print("user> ");
	return "foo";
    }

    public void recievedConsoleMsg(DccClientHandlerThread client,String message) {
	System.out.print("recievedConsoleMsg " + client + ": " + message);

    }

    public void sendDebug(String message) {
	sendMessage(ircDebug ,message);
    }

    public DccServerThread dccServer;

    public void listenForConnections(int port) {
	try {
	    dccServer = new DccServerThread(this,port);
	} catch ( Exception e ) {
	}
    }

    public class DccServerThread extends Thread {
	public boolean listening = true;
	protected ServerSocket serverSocket = null;
	protected int serverPort = 4444;
	protected CycIRCBot cycIRCBot = null;
	public HashMap clients = null;

	public DccServerThread(CycIRCBot ircBot, int port) throws IOException {
	    cycIRCBot =  ircBot;
	    serverPort = port;
	    clients = new HashMap();
	    serverSocket = new ServerSocket(port);
	    this.start();
	}

	public void run() {
	    try {
		while ( listening ) {
		    Socket thisClient = serverSocket.accept();
		    String username = getLogin(thisClient);
		    DccClientHandlerThread clientThread = new DccClientHandlerThread(cycIRCBot, thisClient);
		    clientThread.run();
		    clients.put(username,clientThread);
		}

		serverSocket.close();
	    } catch ( Exception e ) {
	    }

	}

	public String getLogin(Socket thisClient) {
	    return thisClient.toString();
	}
    }

    public class DccClientHandlerThread extends Thread {
	private Socket socket = null;
	private CycIRCBot cycIRCBot = null;
	private PrintWriter out = null;
	private BufferedReader in = null;

	public DccClientHandlerThread(CycIRCBot ircBot,Socket socket) {
	    super("DccClientThread");
	    this.socket = socket;
	}

	public void println(String message) {
	    out.println(message);
	}

	public void run() {
	    String inputLine = null;

	    try {
		out = new PrintWriter(socket.getOutputStream(), true);
		in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

		while ( (inputLine = in.readLine()) != null ) {
		    cycIRCBot.recievedConsoleMsg(this,inputLine);
		}

	    } catch ( IOException e ) {
		e.printStackTrace();
	    }
	}
	public void disconnect() {

	    try {
		out.close();
		in.close();
		socket.close();

	    } catch ( IOException e ) {
		e.printStackTrace();
	    }
	}
    }



}


