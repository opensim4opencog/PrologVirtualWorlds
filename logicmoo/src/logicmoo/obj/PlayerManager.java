package logicmoo.obj;


import logicmoo.*;
import logicmoo.net.*;
import logicmoo.obj.event.*;
import logicmoo.util.FileAccess;
import java.util.*;
import java.net.Socket;
import net.n3.nanoxml.*;


public class PlayerManager implements ServerListener, JamudEventTrigger {

   public static final int
		VALIDATE_OK = 0,
      VALIDATE_BADPASS = 1,
      VALIDATE_NOFILE = 2;


   private String path;
		public String getPath() { return path; }
		public void setPath(String path) {
			if(path == null || path.length() == 0)
				return;

			String e = path.substring(path.length()-1);
			if(e.equals("\\") || e.equals("/"))
				this.path = path;
			else
				this.path = path + "/";
		}

	private Hashtable players = new Hashtable();
	private Hashtable localPlayers = new Hashtable();
		public PlayerMask get(String n) { return (PlayerMask) players.get(n.toLowerCase()); }
		public void put(PlayerMask p) {
			players.put(p.getName().toLowerCase(), p);
			if(p.isLocal())
				localPlayers.put(p.getName().toLowerCase(), p);
		}
		public void remove(String n) {
			players.remove(n.toLowerCase());
			localPlayers.remove(n.toLowerCase());
		}
		public void remove(PlayerMask p) { remove(p.getName()); }
		public Enumeration elements() { return players.elements(); }
		public Enumeration localElements() { return localPlayers.elements(); }



	public PlayerManager(String path) {
		setPath(path);
	}


	public void triggerPlayerLogIn(Player p) {
		//first, add the player for our use
		this.put(p);

		//trigger registered listeners
		Object[] o = { p };
		Jamud.getEventManager().triggerListeners(new PlayerEvent(this, PlayerEvent.EVENT_LOGIN, o));
	}


	public void triggerPlayerLogOut(Player p) {
		//first, remove him from us
		this.remove(p.getName());

		//trigger registered listeners (for us)
		Object[] o = { p };
		Jamud.getEventManager().triggerListeners(new PlayerEvent(this, PlayerEvent.EVENT_LOGOUT, o));
	}


   public void localMudOut(String text) {
      for(Enumeration j = elements(); j.hasMoreElements(); ) {
         Player nxt = ((PlayerMask) j.nextElement()).getPlayer();
         if(nxt != null && nxt.getLoggedOn())
				nxt.println(text);
      }
   }


	public PlayerMask getPlayerMask(String playerName) {
		if(playerName == null || playerName.length() == 0)
			return null;

		playerName = playerName.toLowerCase();

		PlayerMask p = (PlayerMask) get(playerName);
		if(p == null) {
			for(Enumeration enum = elements(); enum.hasMoreElements(); ) {
				PlayerMask tmp = (PlayerMask) enum.nextElement();
				if(tmp.getName().toLowerCase().startsWith(playerName)) {
					p = tmp;
					break;
				}
			}
		}
		return p;
	}


	public Player findPlayer(String playerName) {
		if(playerName == null || playerName.length() == 0)
			return null;

		playerName = playerName.toLowerCase();

		PlayerMask pl = (PlayerMask) get(playerName);
		if(pl == null)
			try {
				IXMLParser xmp = new StdXMLParser();
				xmp.setBuilder(new StdXMLBuilder());
				xmp.setValidator(new NonValidator());
				xmp.setReader(StdXMLReader.fileReader(path + playerName));
				XMLElement xml = (XMLElement) xmp.parse();
				xmp = null;
				return Player.loadPlayer(xml);
			}
			catch(Exception e) {
				return null;
			}
		else
			return pl.getPlayer();
	}


   public void serverAccept(ServerThread source, Socket socket) {
      Player p = new Player();
      p.setConnection(new StdConnection(socket), 0);
   }


   public void serverError(ServerThread source, Exception e) {
		System.err.println("Exception in PlayerManager accepting connections");
      System.err.println(e);
   }

}