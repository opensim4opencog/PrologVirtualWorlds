package logicmoo;

import java.lang.*;
import java.io.*;
import java.util.*;
import java.net.*;

import bsh.*;
import bsh.util.*;

import org.opencyc.api.*;
import org.opencyc.chat.*;
import org.opencyc.creation.*;

// Jamud
/*
import logicmoo.*;
import logicmoo.command.*;
import logicmoo.object.*;
import logicmoo.object.event.*;
import logicmoo.util.*;
import com.logicmoo.commands.olc.*;
import com.logicmoo.commands.*;
import com.logicmoo.connections.*;
import com.logicmoo.communications.*;
import com.logicmoo.creation.*;
import net.n3.nanoxml.*;
*/
//import com.logicmoo.mud.*;



public class MooIrcBot extends IrcChat  {

    /**
     * Creates a basic unstarted IRC Bot
     *     
     */
    public MooIrcBot() {
	super();
	initPlayer();
    }

    /**
     * Creates a full running IRC Bot
    *     
    */
    private CycMoo cycMoo = null;
    //private Engineer mudPlayer = null;

    public MooIrcBot(String nick, String comment, String server, int port, String channel) throws IOException, CycApiException{
	super( (CycAccess) (new CycMoo()), nick, comment, server, port, channel);
	cycMoo = new CycMoo();
       // initPlayer();
    }

    public void initPlayer() {
	//mudPlayer = new IRCPlayer(this,cycMoo);

    }
      /*

    public class IRCPlayer extends Engineer {

	private MooIrcBot mooIrcBot = null;
	private CycMoo cycMoo = null;
	private MudObject body = null;
	private String name = null;

	public IRCPlayer(MooIrcBot amooIrcBot,CycMoo  acycMoo){
	    mooIrcBot = amooIrcBot;
	    cycMoo = acycMoo;
            body = new MudObject();
	    name = mooIrcBot.ircNick;
	    body.setName(name+"-body");
	    body.setPrettyNameription("Look, it's " + name);
	    body.setCommentription("Look, it is undoubtably " + name);
	    body.setParentContainer(cycMoo.findObject("midg_001").childContainer());
	    this.setBody(body);
	}

	public String readln() {
	    return null;
	}
*/
    	/** print the Engineer's prompt.
	 * it is important to note that the enact method itself does not call
	 * ready. Rather, the commands executed (if any) will normally choose
	 * to print the prompt when they have finished execution.
	 */
	
/*
public synchronized void ready() {
	}
	public synchronized void eventTriggered(JamudEvent event) {
	}
	public void print(String text) {
	    mooIrcBot.sendMessage(mooIrcBot.ircDestination,  text);
	}
	public void println(String text) {
	    mooIrcBot.sendMessage(mooIrcBot.ircDestination,  text);
	}
	public String prompt(String text) {
	    return null;
	}
    }
	    */



    public boolean servicePlugin(String from, String hostmask, String returnpath,String token,String params) {

	// Begin BeanShell
	if ( token.equals("bsh") ) {
	    try {
		sendMessage(returnpath, CycMoo.eval(params));
	    } catch ( Exception e ) {
		e.printStackTrace(System.out);
		sendMessage(returnpath,""+e);
	    }
	    return true;
	}
	// End BeanShell


	return false;
    }

    public void startPlugins() {
	try {
	    CycMoo.set("ircbot",this);
	} catch ( Exception e ) {
	    e.printStackTrace();
	}
	return;
    }

}

