package  logicmoo.net;

import java.util.Hashtable;
import logicmoo.Jamud;
import logicmoo.plugin.*;


/**
 * Default plugin for accepting connections to the mud.
 * @author siege
 */
public class StandardServer extends JamudPlugin implements ServerListener {

	private static final String
		NAME = "Jamud Standard Server",
		VERSION = "1.0.4",
		AUTHOR = "siege";

		public String getName()			{ return NAME; }
		public String getVersion()		{ return VERSION; }
		public String getAuthor()		{ return AUTHOR; }
		public String getInfo() {
			return (
				" This is the standard server for accepting sockets in JaMUD."
			);
		}

	private static final int DEFAULT_PORT = 8808;

	private static final String PROPERTY_PORT = "PORT";

	private int state = STATE_TERMINATED;
		public int getPluginState() { return state; }

	private int port;
	private ServerThread serv = null;



	public void initialize() {
		if(state == STATE_TERMINATED) {
			state = STATE_INITIALIZING;
			System.out.print("Jamud Standard Server starting on port " + this.port + "... ");

			serv = new ServerThread(this, this.port);
			serv.start();

			state = STATE_INITIALIZED;
			System.out.println("done");
		}
	}


	public void terminate(){
		if(state == STATE_INITIALIZED) {
			state = STATE_TERMINATING;
			System.out.print("Jamud Standard Server stopping... ");

			serv.halt();
			serv = null;

			state = STATE_TERMINATED;
			System.out.println("done");
		}
	}


	public boolean isActive() {
		return (state == STATE_INITIALIZED);
	}


	public void tick() {
		//empty
	}


	public StandardServer(Hashtable ini) {
		super(ini);
	       String p = getParam(PROPERTY_PORT);
	      	this.port = ((p != null) ? Integer.parseInt(p) : DEFAULT_PORT );
	}


   public void serverAccept(ServerThread source, java.net.Socket socket) {
		Jamud.getPlayerMananger().serverAccept(source, socket);
	}


   public void serverError(ServerThread source, Exception e) {
		if(isActive()) {
			this.terminate();
			Jamud.getPlayerMananger().serverError(source, e);
		}
	}


}