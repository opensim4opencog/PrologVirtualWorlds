//<pre>
package logicmoo.agent;

import logicmoo.api.*;
import logicmoo.agent.*;
import logicmoo.util.*;
import logicmoo.obj.*;


// Java
import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;



// OpenCyc
import org.opencyc.api.*;
import org.opencyc.creation.*;
import org.opencyc.cycobject.*;
import org.opencyc.kif.*;
import org.opencyc.util.*;
import org.opencyc.chat.*;
import org.opencyc.webserver.WebServer;
import org.apache.oro.util.*;


import ViolinStrings.Strings;


public class MooIrcBot extends MooAgentThread implements IMooClient, ChatSender {

    /**
     * Creates a basic unstarted IRC Bot
     *     
     */
    public MooIrcBot() {
    }

    /**
     * Creates a full IRC Bot
    *     
    */

    public MooIrcBot(LogicMooCycAccess cycm, String nick, String comment, String server, int port, String channel) throws IOException, CycApiException{
	// set the bot's nickname and description
	ircNick = nick;
	ircComment = comment;
	ircServer = server;
	ircPort = port;
	ircChannel = channel;
	Log.makeLog();
	cyc = cycm;
	reloadInterpretor(cyc.makeCycConstant(ircNick));
    }

    public Object prompt(Object message) {
	return "";
    }

    public void end() {
	this.ircDisconnect();
	return;
    }

    public boolean receiveEvent(LogicMooEvent event) {
	if( isEventsOn ) sendMessage(ircDebug,""+event+"<br>");
	return true;
    }


    private StringBuffer sendBuffer = new StringBuffer("");

    public boolean printRaw(String message) {
	message =  Strings.change(message,"<br>","\n");
	message =  Strings.change(message,"\\n","\n");
	int crlf = message.indexOf("\n");
	if( crlf<0 ) {
	    sendBuffer.append(message);
	    return true;
	}

	sendBuffer.append(message.substring(0,crlf).trim()+" ");
	String willSend = Strings.change(sendBuffer.toString(),"\n"," ").trim();
	if( willSend.length()>0 );ircSend("privmsg " + ircDestination + " :" + willSend);
	sendBuffer = new StringBuffer(message.substring(crlf+1));
	try {
	    Thread.sleep(500);
	} catch( Exception e ) {
	}
	return true;
    }

    /**
   * IRC Bot details
   */

    // Name Bot goes by on IRC
    public String ircNick = "Cycbot";

    // WHOIS Information
    public String ircComment = "http://www.opencyc.org";

    // IRC Auto-join
    public String ircChannel = "#logicmoo";

    /**
     * IRC Sever details
     */
    public String ircServer = "irc.openprojects.net";
    public int ircPort = 6667;

    // IRC Unkown message replies sentence to
    public String ircDestination = "#logicmoo";

    // ArrayList of paraphrased writable locations
    public ArrayList paraphrased = new ArrayList();

    // IRC Debug messages sentence to ( may send to an IRC username instead of channel )
    public String ircDebug = "#logicmoo";

    /**
     * IRC Server comunication
     */
    private Socket ircServerSocket =null;
    private InputStream ircInputStream = null;
    private OutputStream ircOutputStream = null;
    private BufferedReader ircInputReader = null;
    private BufferedWriter ircOutputWriter = null;

    /**
     * Telent DCC Chat Server 
     */
    public DccServerThread dccServer;

    /**
     * reference to CycAccess (OpenCyc server)
     */


    /**
     * reference to ChatterBot
     */
    public org.opencyc.chat.ChatterBot chatterBot = null;



    public boolean running = false;


    /**
     * Provide a command line function to launch the MooIrcBot application.  
     *     
     */
    public static void main(String[] args) {
	try {
	    MooIrcBot ircBot = new MooIrcBot();
	    ircBot.cyc = new LogicMooCycAccess();
	    if( args.length > 0 ) ircBot.ircNick = args[0];
	    if( args.length > 1 ) ircBot.ircChannel = args[1];
	    if( args.length > 2 ) ircBot.ircServer = args[2];
	    if( args.length > 3 ) ircBot.ircPort = Integer.parseInt((args[3]));
	    System.out.println("Lauching MooIrcBot: n\nick='"+ircBot.ircNick+"' \nchannel='"+ircBot.ircChannel+"' \nserver='"+ircBot.ircServer+":'"+ircBot.ircPort+"");
	    ircBot.run();
	    System.exit(0);
	} catch( Exception e ) {
	    e.printStackTrace(System.err);
	    System.exit(1);
	}
    }

    /**
     * Connects Bot to an IRC server
     *     
     */
    public void ircConnect() {
	Log.makeLog();
	try {
	    ircServerSocket = new Socket(ircServer, ircPort);
	} catch( Exception e ) {
	    System.err.println("error Connecting to IRC server");
	    e.printStackTrace();
	}

	try {
	    ircInputStream = ircServerSocket.getInputStream();    
	    ircOutputStream = ircServerSocket.getOutputStream();
	} catch( Exception e ) {
	    System.err.println("error opening streams to IRC server");
	    e.printStackTrace();                                   
	}

	ircInputReader = new BufferedReader(new InputStreamReader(ircInputStream));
	ircOutputWriter = new BufferedWriter(new OutputStreamWriter(ircOutputStream));  

	try {
	    // send user info
	    ircOutputWriter.write("user " + ircNick + " opencyc irc :" + ircComment);
	    ircOutputWriter.newLine();
	    ircOutputWriter.write("nick " + ircNick);
	    ircOutputWriter.newLine();
	    ircOutputWriter.flush();
	} catch( Exception e ) {
	    System.out.println("ircLogOn error: " + e);
	}
	ircJoin(ircChannel);
	//paraphrased.add(ircChannel);
	ircJoin(ircDebug);

	this.running = true;

	startChatterBot();
	return;
    }

    /**
     * Disconnct Bot from an IRC server
     *     
     */
    public void ircDisconnect() {
	try {
	    chatterBot.finalize();
	    chatterBot = null;
	    ircOutputWriter.write("QUIT this.ircDisconnect();");
	    ircOutputWriter.newLine();
	    ircOutputWriter.flush();
	} catch( Exception e ) {
	    System.out.println("ircLogOff error: " + e);
	    e.printStackTrace();
	}

	// close the IO streams to the IRC server
	try {
	    ircInputReader.close();
	    ircOutputWriter.close();
	    ircInputReader = null;
	    ircOutputWriter = null;
	    ircServerSocket.close();
	    ircServerSocket = null;

	} catch( IOException e ) {
	    System.err.println("Error ircDisconnecting from IRC server");
	    e.printStackTrace();
	}
    }

    public void run() {
	ircConnect();
	if( ircChannel!=null ) ircJoin(ircChannel);
	while( !this.interrupted() && running )
	    try {
		this.serviceLoop();
	    } catch( Exception e ) {
		System.out.println(""+e);
	    }
    }

    public void restartChatterBot() {
	terminateChatterBot();
	startChatterBot();
    }

    public void terminateChatterBot() {
	if( chatterBot!=null ) {
	    try {
		chatterBot.finalize();
	    } catch( Exception e ) {
		e.printStackTrace(System.err);
	    }
	    chatterBot = null;
	}
    }

    public void startChatterBot() {
	if( chatterBot==null ) {
	    try {
		chatterBot = new ChatterBot(this);
		chatterBot.initialize();
	    } catch( Exception e ) {
		e.printStackTrace(System.err);
	    }
	}
    }

    public void serviceLoop() throws Exception {
	serviceIRCServer();
    }

    /**
     * Sends a raw string to the IRC server
     */
    public boolean ircSend(String message) {
	System.out.println("irc: '" + message + "'");
	try {
	    ircOutputWriter.write(message);
	    ircOutputWriter.newLine();
	    ircOutputWriter.flush();
	} catch( IOException e ) {
	    return false;
	}
	return true;
    }

    public void ircJoin(String channel) {
	ircSend("JOIN "+channel);
    }

    public void ircPart(String channel) {
	ircSend("PART "+channel);
    }
    /**
     * Send a notice to an IRC user
     * @param destination String
     * @param message String
     */
    public void sendNotice(String destination, String message) {
	ircSend("notice " + destination + " :" + message);
    }

    /**
     * Send a public message to an IRC user
     * @param destination String
     * @param message String
     */
    public boolean sendMessage(String destination, Object post) {
	ircDestination = destination;
	return printFormat(post);
    }

    /**
     * Receives and parses IRC Server messages
     */
    public void serviceIRCServer() throws Exception {

	// Wait a 1/10th sec
	try {
	    Thread.sleep(100);
	} catch( InterruptedException e ) {
	}

	// Data ?
	if( !ircInputReader.ready() ) return;

	String message = ircInputReader.readLine();

	//System.out.println(message);

	// send a pong back
	if( message.substring(0,4).equalsIgnoreCase("ping") ) {
	    ircSend("pong " + message.substring(5));
	    return;
	}
	serviceIRCSession(message);
    }


    /**
     * Receives and parses IRC Session messages
     */
    public void serviceIRCSession(String message) {
	String prefix = null;
	String command = null;
	String params = null;
	String user = null;


	// check for the prefix
	if( message.substring(0,1).equals(":") ) {
	    prefix = message.substring(1, message.indexOf(' '));
	    message = message.substring(message.indexOf(' ') + 1);
	}

	// extract the command
	command = message.substring(0, message.indexOf(' '));

	// get the parameters (the rest of the message)
	params = message.substring(message.indexOf(' ') + 1);

	if( params.toLowerCase().startsWith(":closing") ) {
	    ircConnect();
	    return;
	}

	try {
	    int col = params.indexOf(':');
	    String destination = params.substring(0,col-1).trim();
	    ircDestination = destination;
	    params = params.substring(col+1).trim();

	    serviceIRCTransaction(prefix.substring(0, prefix.indexOf('!')),prefix,command,destination, params);
	} catch( Exception e ) {
	}
    }
    /**
     * Process an IRC Transaction
     * @param destination String nickname of the user who sentence the message
     * @param message String the command
    
    params are in the form
    <my nick> :<message> 
    or
    <my nick> <message>	
    
     */
    public void serviceIRCTransaction(String from, String hostmask, String command, String destination,String params) {

	if( hostmask.startsWith(ircNick) ) return;
	if( command.equalsIgnoreCase("PRIVMSG") )
	    servicePublicMessage(from, hostmask, destination, params.trim());
    }

    /**
     * Process a Public message (PRIVMSG to Channel)
     * @param destination String nickname of the user who sentence the message
     * @param message String the command
    
    params are in the form
    <my nick> :<message> 
    or
    <my nick> <message>	
    
     */
    public void servicePublicMessage(String from, String hostmask, String returnpath,String params) {

	String lcparams = params.toLowerCase().trim();

	int ccol = params.indexOf(':');
	if( ccol<0 ) ccol = params.indexOf(' ');

	if( ccol>1 ) {
	    String token = lcparams.substring(0,ccol).trim();
	    params = params.substring(ccol+1).trim();
	    if( serviceToken(from, hostmask, returnpath, token,params) ) return;
	} else {
	    if( serviceToken(from, hostmask, returnpath, lcparams, params) ) return;
	}

	serviceChatter(from,hostmask,params,returnpath);
    }

    /**
     * Process a Token message 
     * @param destination String nickname of the user who sentence the message
     * @param message String the command
    
    params are in the form
    <my nick> :<message> 
    or
    <my nick> <message>	
    
     */

    public boolean serviceToken(String from, String hostmask, String returnpath,String token,String params) {
	System.out.println("token: '" + token + "' params: '" + params + "'");
	ircDestination = returnpath;

	if( token.equals("ping") ) {
	    sendMessage(returnpath, "hello " + from);
	    return true;
	}

	if( token.equals("join") ) {
	    ircJoin(params);
	    return true;
	}
	if( token.startsWith("part") ) {
	    ircPart(params);
	    return true;
	}
	if( token.startsWith("putserv") ) {
	    ircSend(params);
	    return true;
	}

	ircDestination = returnpath;
	return enact(returnpath,from,token+ " " + params);
    }

    public void serviceChatter(String cyclist,String identity,String message, String returnpath) {
	ircDestination = returnpath;
	return;


	//	try {
	//          if ( chatterBot!=null ) chatterBot.receiveChatMessage(cyclist, cyclist    /*identity hostmask?*/,message);
	//	} catch ( Exception e ) {
	//	    e.printStackTrace(System.out);
	// sendDebug(""+e);
	//	}
    }


    public HashMap mtUser = new HashMap();

    /**
     * Returns a Mt for a user
     */
    public CycFort mtForUser(String cyclist) {

	CycConstant mt = (CycConstant) mtUser.get(cyclist);
	if( mt==null ) {
	    try {
		mt = cyc.makeCycConstant("#$"+cyclist+"ChatMt");
		cyc.assertIsa(mt,cyc.makeCycConstant("#$Microtheory"),cyc.baseKB);
	    } catch( Exception e ) {
		mt = cyc.baseKB; 
	    }
	    sendMessage(ircDestination,"Using microtheory" + mt.cyclify() + " for assertions until " + cyclist + " types \"mt <something>\"");
	    mtUser.put(cyclist,mt);
	}
	return(CycFort)mt;
    }

    public void sendDebug(String message) {
	sendMessage(ircDebug ,message);
    }


    /**
     * Sends the chat message from Cyc into the chat system.
     */
    public void sendChatMessage(String chatMessage) {
	// sendMessage(ircDestination,chatMessage);
    }

    /**
     * Receives chat messages from the user.
     */
    private String receiveChatMessage() throws IOException {
	System.out.print("user> ");
	return "foo";
    }

    public void recievedConsoleMsg(DccClientHandlerThread client,String message) {
	System.out.print("recievedConsoleMsg " + client + ": " + message);
    }

    public void listenForConnections(int port) {
	try {
	    dccServer = new DccServerThread(this,port);
	    dccServer.start();
	} catch( Exception e ) {
	}
    }

    private class DccServerThread extends Thread {
	public boolean listening = true;
	private ServerSocket serverSocket = null;
	private int serverPort = 4444;
	private MooIrcBot MooIrcBot = null;
	public HashMap clients = null;

	public DccServerThread(MooIrcBot ircBot, int port) throws IOException {
	    MooIrcBot =  ircBot;
	    serverPort = port;
	    clients = new HashMap();
	    serverSocket = new ServerSocket(port);
	    this.start();
	}

	public void run() {
	    try {
		while( listening ) {
		    Socket thisClient = serverSocket.accept();
		    String username = getLogin(thisClient);
		    DccClientHandlerThread clientThread = new DccClientHandlerThread(MooIrcBot, thisClient);
		    clientThread.run();
		    clients.put(username,clientThread);
		}

		serverSocket.close();
	    } catch( Exception e ) {
	    }

	}

	public String getLogin(Socket thisClient) {
	    return thisClient.toString();
	}
    }

    public class DccClientHandlerThread extends Thread {
	private Socket socket = null;
	private MooIrcBot MooIrcBot = null;
	private PrintWriter out = null;
	private BufferedReader in = null;

	public DccClientHandlerThread(MooIrcBot ircBot,Socket socket) {
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

		while( (inputLine = in.readLine()) != null ) {
		    MooIrcBot.recievedConsoleMsg(this,inputLine);
		}

	    } catch( IOException e ) {
		e.printStackTrace();
	    }
	}
	public void disconnect() {

	    try {
		out.close();
		in.close();
		socket.close();

	    } catch( IOException e ) {
		e.printStackTrace();
	    }
	}
    }



}


