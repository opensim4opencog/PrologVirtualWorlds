package logicmoo.obj;


import logicmoo.*;
import logicmoo.cmd.*;
import logicmoo.net.PlyrConnection;
import logicmoo.obj.event.*;
import logicmoo.util.*;
import java.io.FileWriter;
import java.util.*;
import net.n3.nanoxml.*;


/**
 * The representation of a Engineer in the mud. 
 *
 *@author Siege
 */
public class Engineer extends PlayerMask implements Flagged, TickListener, ChannelListener {

   public static final String MARKUP = "PLAYER";

   public static final long DEFAULT_LAG = 250;
	public static final int DEFAULT_SCROLL = 23;
   public static final String DEFAULT_PROMPT = "[%n@logicmoo /]$ ";

   public static final String
      PROPERTY_NAME = "NAME",
      PROPERTY_PASSWORD = "PASSWORD",
      PROPERTY_BORN = "BORN",
      PROPERTY_TITLE = "TITLE",
      PROPERTY_TRUST = "TRUST",
      PROPERTY_PROMPT = "PROMPT",
		PROPERTY_SCROLL = "SCROLL",
      PROPERTY_CHANNELS = "CHANNELS",
		PROPERTY_COMMANDS = "COMMANDS",
      PROPERTY_NATIVE_BODY = "BODY",
      PROPERTY_CURRENT_BODY = "cBODY",
      PROPERTY_LOCATION = "LOCATION",
      PROPERTY_LOCATION_VNUM = "VNUM",
      PROPERTY_EMAIL = "EMAIL",
      PROPERTY_EMAIL_FWD = "FWD",
      PROPERTY_CURRENT_NOTE = "NOTE",
      PROPERTY_LAST_COMMAND = "CMD",
      PROPERTY_POOF_IN = "POOFIN",
      PROPERTY_POOF_OUT = "POOFOUT",
      PROPERTY_ALIASLIST = "ALIASLIST";

   public static final String
      PROPERTY_FLAGS_ALIAS = "ALIAS",
      PROPERTY_FLAGS_COLOUR = "COLOUR",
      PROPERTY_FLAGS_DEAF = "DEAF",
      PROPERTY_FLAGS_MUTE = "MUTE",
      PROPERTY_FLAGS_AFK = "AFK",
      PROPERTY_FLAGS_TELL = "TELL",
      PROPERTY_FLAGS_AUTOFIGHT = "AUTO_FIGHT";

   public static final int
      LOGIN_SUCCESS = 0,
      LOGIN_ERR_ALREADY_THERE = 1,
      LOGIN_ERR_BAD_PASSWORD = 2,
      LOGIN_ERR_NO_SUCH_CHARACTER = 3;

	public static final String
		MESSAGE_WRITE = "player.write",
		MESSAGE_BADPASS = "player.badpass";

	private long lastCommandAt = System.currentTimeMillis();
		public long getLastCommandAt() { return lastCommandAt; }

   private Hashtable properties = new Hashtable();
      public Hashtable getProperties() { return this.properties; }
      public Object getProperty(String key) { return this.properties.get(key); }
      public void putProperty(String key, Object o) { this.properties.put(key, o); }

   private boolean colour = true;
      public boolean getColour() { return colour; }
      public void setColour(boolean colour) {
         this.colour = colour;
         if(colour) flags.add(PROPERTY_FLAGS_COLOUR);
         else flags.rem(PROPERTY_FLAGS_COLOUR);
      }

   private boolean deaf = false;
      public boolean getDeaf() { return deaf; }
      public void setDeaf(boolean deaf) {
         this.deaf = deaf;
         if(deaf)	flags.add(PROPERTY_FLAGS_DEAF);
         else flags.rem(PROPERTY_FLAGS_DEAF);
      }

   private boolean mute = false;
      public boolean getMute() { return mute; }
      public void setMute(boolean mute) {
         this.mute = mute;
         if(mute) flags.add(PROPERTY_FLAGS_MUTE);
         else flags.rem(PROPERTY_FLAGS_MUTE);
      }

   private boolean parseAlias = true;
      public boolean getParseAlias() { return parseAlias; }
      public void setParseAlias(boolean parse) {
         this.parseAlias = parse;
         if(parse) flags.add(PROPERTY_FLAGS_ALIAS);
         else flags.rem(PROPERTY_FLAGS_ALIAS);
      }

   private int parseLen = DEFAULT_SCROLL;
      public void killParseAlias(int len) {
         this.parseAlias = false;
         this.parseLen = len;
      }

	private int scrollLength = 0;
		public int getScrollLength() { return scrollLength; }
		public void setScrollLength(int lines) { this.scrollLength = lines; }

   private boolean loggedOn = false;
      public boolean getLoggedOn() { return loggedOn; }
      public void setLoggedOn(boolean loggedOn) { this.loggedOn = loggedOn; }

   private PlyrConnection connection;
      public PlyrConnection getConnection() { return this.connection; }
      public void setConnection(PlyrConnection connection) { this.connection = connection; }
      public void setConnection(PlyrConnection connection, int reason) {
         if (this.connection != null) this.connection.logOut(reason);
         this.connection = connection;
         this.connection.setPlayer(this);
      }

   private FlagList channels = new FlagList();
      public FlagList getChannels() { return channels; }
      public void setChannels(FlagList channels) { this.channels = channels; }

   private FlagList flags = new FlagList();
      public FlagList getFlags() { return flags; }
      public void setFlags(FlagList flags) { this.flags = flags; }
		public boolean isFlagged(String flag) { return flags.contains(flag); }

   private Hashtable aliases = new Hashtable();
      public Hashtable getAliases() { return aliases; }
      public Alias getAlias(String name) { return (Alias) aliases.get(name); }

   private FlagList commands = new FlagList();
      public FlagList getGranted() { return commands; }

   private Queue cmdBuffer = null;

   private Ticker ticker = null;
      public Ticker getTicker() { return this.ticker; }
      public long getLag() {
         if(ticker == null) return 0;
         return ticker.getInterval();
      }
      public void setLag(long lag) {
         if(ticker != null) ticker.setInterval(lag);
      }



   public Engineer() {
      super.setPlayer(this);
		//provides a reverse reference in PlayerMask
   }



   public void encueCommand(String cmd) {
		Object[] o = { cmd };
		Jamud.getEventManager().triggerListeners(new EngineerEvent(this, EngineerEvent.EVENT_DATAIN, o));

      if(cmdBuffer != null)
			cmdBuffer.put(cmd);
      else
			Interpreter.execute(this, cmd, false);
   }


	public void channelOut(Channel chan, PlayerMask pm, String text) {
		if(pm != this) {
			String o = Util.replace(chan.getDisplay(), "%n", pm.getName());
			o = Util.replace(o, "%t", text);
			println(o);
		}
	}


   public void tick(Ticker src) {
      try {
	      String cmd = (String) cmdBuffer.get();
			if(cmd != null) {
	         if(parseLen >= 0) {
	            if(--parseLen < 0) {
	               this.parseAlias = true;
	               parseLen = 0;
	            }
	         }
				lastCommandAt = System.currentTimeMillis();
	         Interpreter.execute(this, cmd, this.parseAlias);
			}
      }
      catch(Exception e) {
         System.err.println("Exception in Engineer.tick");
			e.printStackTrace();
         connection.logOut(PlyrConnection.LOGOUT_ERROR);
      }
   }


   public void tickError(Ticker src, Exception e) {
      System.err.println("tickError in Engineer");
      System.err.println(e);

		if(ticker == src)
			ticker = new Ticker(this, src.getInterval());
   }


	public void triggerOnTick() {
		Jamud.getEventManager().triggerListeners(new EngineerEvent(this, JamudEvent.EVENT_TICK, new Object[0]));
	}


	public void grant(PlayerGranted command) {
		commands.add(command.getName().toLowerCase());
	}


   public PlayerGranted getGranted(String name) {
		name = name.toLowerCase();
      if(commands.contains(name)) {
         return PlayerInterpreter.grantedTable.get(name);
      }
      else {
         for(Enumeration enum = commands.enumerate(); enum.hasMoreElements(); ) {
            String ns = (String) enum.nextElement();
            if(ns.startsWith(name)) return PlayerInterpreter.grantedTable.get(ns);
         }
      }
      return null;
   }


   public void logIn() {
		try {

		//print the opening screen
      print(Jamud.getMessage(Jamud.MESSAGE_OPEN));

		//try and get a good user name
      String n = connection.prompt("\nUsername : ").trim();
      if(n.length() < 1) return;
      while(! Util.verifyString(n, Util.ALPHABET)) {
         n = connection.prompt("\nInvalid! Username may contain only letters (A-Z, a-z)\nUsername : ").trim();
         if(n.length() < 1) return;
      }

		//try authenticating logIn
      if(! logIn(n)) return;

		//print the MOTD
      print("\n" + Jamud.getMessage(Jamud.MESSAGE_MOTD) + "\n");

		//put our body in the right room
      AgentGeneric body = (AgentGeneric) getProperty(PROPERTY_CURRENT_BODY);
      GeographicalRegion r = Jamud.getMicrotheoryManager().getGeographicalRegion(null, (String) this.getProperty(Engineer.PROPERTY_LOCATION));
      if(r==null)
			r = Jamud.getMicrotheoryManager().getDefaultGeographicalRegion();
      r.enter(body, getName() + " fades into view.");

		//subscribe to all our channels
		for(Enumeration enum = channels.enumerate(); enum.hasMoreElements(); ) {
			String s = (String) enum.nextElement();
			Channel c = null;
			for(Enumeration renum = Jamud.getChannelManager().elements(); renum.hasMoreElements(); ) {
				Channel tmp = (Channel) renum.nextElement();
				if(tmp.getName().equals(s)) {
					c = tmp;
					break;
				}
			}
			if(c != null)
				c.subscribe(this);
		}

		//open up the command buffer and start the ticker
      cmdBuffer = new Queue();
      ticker = new Ticker(this, DEFAULT_LAG);
      ticker.start();

		//try and save
		if(! this.save() )
         System.err.println("Unable to save file at Engineer.login for " + getName());

		//trigger LogIn event from PlayerManager
		Jamud.getPlayerManager().triggerPlayerLogIn(this);

		//trigger LogIn event from Engineer
		Jamud.getEventManager().triggerListeners(new EngineerEvent(this, EngineerEvent.EVENT_LOGIN, new Object[0]));

		//start triggering PlayerMaskUpdate events
		triggerPlayerMaskEvents(true);

		//All went well
      loggedOn = true;

		}
		catch(Exception exx) {
			exx.printStackTrace();
		}

   }


   public boolean logIn(String user) {
      setShortName(user);
      boolean exists = false;

      try {
			//try and load the contents of the file
	      System.out.println("attempting login for " + user + "...");
			IXMLParser xmp = new StdXMLParser();
			xmp.setBuilder(new StdXMLBuilder());
			xmp.setValidator(new NonValidator());
			xmp.setReader(StdXMLReader.fileReader(Jamud.getPlayerManager().getPath() + user.toLowerCase()));
			XMLElement xml = (XMLElement) xmp.parse();
			xmp = null;

         load(xml);
			xml = null;

			//Password check
         String pass = prompt("Password : {-").trim();
         pass = (new MD5(user.toLowerCase() + pass)).asHex();
         print("{+");
         if(! pass.equals(this.getProperty(PROPERTY_PASSWORD))) {
            System.out.println("bad password!");
            print("\nBad Password!");
            return false;
         }

         //Pfile is there, and they've passed the password check
         exists = true;
      }
		catch(java.io.FileNotFoundException f) {
			//No Pfile found, will start creation after a few...
		}
      catch(Exception e) {
         System.out.println("exception at logIn...");
         System.err.println(e);
			return false;
      }

      //check if already logged on elsewhere
      PlayerMask tst = Jamud.getPlayerManager().get(user);

      //not logged on elsewhere
      if(tst == null) {

         //player exists, so finish logging on
         if(exists) {
            loggedOn = true;
            System.out.println("successfull login.");
            return true;
         }

         //player does not exist, so attempt creation
         else {
            System.out.println("attempting creation for " + user + "...");

				//try creation
            Creation.creation(this);

				//creation was successfull
            if(loggedOn) {
               System.out.println("creation successfull.");
               return true;
            }

				//creation got screwed
            else {
               System.out.println("creation aborted.");
               return false;
            }
         }
      }

      //log on with this connection instead
      else {
         tst.getPlayer().setConnection(connection, PlyrConnection.LOGOUT_ELSEWHERE);
         String tx = (tst.getPlayer().getLoggedOn() ? "Connection relocated." : "You have reconnected.");
         tst.getPlayer().println(tx);
         System.out.println("connection relocated.");
         this.setConnection(null);
         tst.getPlayer().setLoggedOn(true);
         return false;
      }
   }


   public void logOut() {

		//save (avoiding item-load cheating)
		save();

		//unsubscribe to all our channels
		for(Enumeration enum = channels.enumerate(); enum.hasMoreElements(); ) {
			String s = (String) enum.nextElement();
			Channel c = null;
			for(Enumeration renum = Jamud.getChannelManager().elements(); renum.hasMoreElements(); ) {
				Channel tmp = (Channel) renum.nextElement();
				if(tmp.getName().equals(s)) {
					c = tmp;
					break;
				}
			}
			if(c != null) c.unsubscribe(this);
		}

		//trigger LogOut event from Engineer
		Jamud.getEventManager().triggerListeners(new EngineerEvent(this, EngineerEvent.EVENT_LOGOUT, new Object[0]));

		//trigger LogOut event from PlayerManager
		Jamud.getPlayerManager().triggerPlayerLogOut(this);

		//remove body from room
		AgentGeneric a = (AgentGeneric) getProperty(PROPERTY_CURRENT_BODY),
			b = (AgentGeneric) getProperty(PROPERTY_NATIVE_BODY);
		a.quit();
		if(a != b)
			b.quit(); 

		a = null;
		b = null;

      if(connection != null) {
			//print closing credits
			print("\n" + Jamud.getMessage(Jamud.MESSAGE_CLOSE));

			//clean up connection
         connection.logOut(0);
         connection = null;
      }

		//clean up ticker
      if(ticker != null) {
         ticker.halt();
         ticker = null;
      }

		//clean up command buffer
      if(cmdBuffer != null) {
         cmdBuffer.clear();
         cmdBuffer = null;
      }

		//release various resources
		properties.clear();
		properties = null;
		aliases.clear();
		aliases = null;

		//ensure we're clean
		Jamud.getEventManager().removeAllListeners(this);
   }


   public static Engineer loadPlayer(XMLElement xml) throws Exception {
      Engineer player = new Engineer();
      player.load(xml);
      return player;
   }


   public void load(XMLElement xml) throws Exception {

     }


   public synchronized boolean save() {
    	}


   public void ready() {
      //prints the prompt for this player
      if (connection!= null && connection.goodConnection()) {
         String p = "\n" + parsePrompt(this, (String)this.getProperty(Engineer.PROPERTY_PROMPT));
         connection.print(Colour.convert(p, colour));
			Object[] o = {p};
			Jamud.getEventManager().triggerListeners(new EngineerEvent(this, EngineerEvent.EVENT_DATAOUT, o));
      }
   }


   public void print(String text) {
      if (text!=null && connection!= null && connection.goodConnection()) {
         connection.print(Colour.convert(text, colour));
			if(loggedOn) {
				Object[] o = {text};
				Jamud.getEventManager().triggerListeners(new EngineerEvent(this, EngineerEvent.EVENT_DATAOUT, o));
			}
      }
   }


   public void print(byte[] text) {
      if (connection!= null && connection.goodConnection()) {
         connection.print(Colour.convert(text, colour));
			if(loggedOn) {
				Object[] o = {new String(text)};
				Jamud.getEventManager().triggerListeners(new EngineerEvent(this, EngineerEvent.EVENT_DATAOUT, o));
			}
      }
   }


   public void println(String text) {
      if (text!=null && connection!= null && connection.goodConnection()) {

			String p = "";
         if(loggedOn) {
	         p = "\n" + parsePrompt(this, (String)this.getProperty(Engineer.PROPERTY_PROMPT));
			}

			if(scrollLength > 0 && text.length() > 1) {
				//print only scrollLength lines at a time

				//turn text into an arraylist
				ArrayList al = new ArrayList();
				for(String sp[] = Util.split(text, '\n'); (sp[0].length() + sp[1].length() > 0); sp = Util.split(sp[1], '\n')) {
					al.add(sp[0]);
				}

				if(al.size() < scrollLength) {

					//print it all at once
					al = null;
					text += ((text.length() > 0) ? "\n" : "");
					text += p;

					connection.print(Colour.convert(text, colour));
					if(loggedOn) {
						Object[] ob = {text};
						Jamud.getEventManager().triggerListeners(new EngineerEvent(this, EngineerEvent.EVENT_DATAOUT, ob));
					}
				}
				else {

					//print out scrollList lines at a time
					for(int I=0; I<al.size(); ) {
						String o = "";
						for(int J=0; J<scrollLength && I<al.size(); J++) {
							o += (String) al.get(I++) + "\n";
						}

						connection.print(Colour.convert(o, colour));
						if(loggedOn) {
							Object[] ob = {o};
							Jamud.getEventManager().triggerListeners(new EngineerEvent(this, EngineerEvent.EVENT_DATAOUT, ob));
						}

						if(I >= al.size() || prompt("{*[press enter to continue]{0").length() > 0) {
							if(loggedOn) {
								connection.print(Colour.convert(p, colour));
								Object[] ob = {o};
								Jamud.getEventManager().triggerListeners(new EngineerEvent(this, EngineerEvent.EVENT_DATAOUT, ob));
							}
							return;
						}

					}

				}

			}
			else {

				//print it all at once
				text += ((text.length() > 0) ? "\n" : "");
				text += p;
				connection.print(Colour.convert(text, colour));
				if(loggedOn) {
					Object[] ob = {text};
					Jamud.getEventManager().triggerListeners(new EngineerEvent(this, EngineerEvent.EVENT_DATAOUT, ob));
				}

			}
      }
   }


   public String prompt(String msg) {
      if(cmdBuffer != null) {
         print(msg);
         return (String) cmdBuffer.get();
      }
      else {
         if(connection != null && connection.goodConnection()) {
            return connection.prompt(Colour.convert(msg, colour));
         }
         else {
            return null;
         }
      }
   }


   public static String parsePrompt(Engineer src, String pr) {
      pr = Util.replace(pr, "%n", src.getName());
      return pr;
   }


   public String getText(String prompt) {
      print(prompt + "\n");
      return getText();
   }


   public String getText() {
      boolean okay = true;
      String o = "";
      while(okay) {
         String i = (String) cmdBuffer.get();
         if(!i.equalsIgnoreCase(".")) {
            o = o + i + "\n";
         }
         else {
            o = o.substring(0, o.length()-1);
            okay = false;
         }
      }
      return o;
   }


   public void finalize() throws Throwable {
//      System.out.println(" player finalized (" + getName() + ")");
      super.finalize();
   }

}