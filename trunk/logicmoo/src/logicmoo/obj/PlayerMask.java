package logicmoo.obj;

import logicmoo.Jamud;
import logicmoo.obj.event.*;
import logicmoo.util.Util;

public class PlayerMask extends CycThing implements JamudEventTrigger {

   public static final int
		TRUST_GUEST = 0,           //on a guest ID
      TRUST_NEWBIE = 1,          //anything under L15
      TRUST_PLAYER = 2,          //L15 to L100
      TRUST_HERO = 3,            //L101 and up (2nd tier, inclusive)
		TRUST_IMMORTAL = 4,			//hired player, administrator
      TRUST_GOD = 5,      		   //between owner-op and hired
      TRUST_IMPLEMENTOR = 6;     //owner-operator

   public static final String[] TRUST = {
		"guest",
      "newbie",
      "player",
      "hero",
      "immortal",
		"god",
      "implementor" };

	public static final int
		TELL_OK = 0,
		TELL_AFK = 1,
		TELL_OFF = 2,
		TELL_ERR = 3;

		//STATE_ACTIVE = 0
	public static final int
		STATE_AFK = 1,
		STATE_NOTELL = 2,
		STATE_LINKDEAD = 4;

		//VIS_VISIBLE = 0
	public static final int
		VIS_HIDDEN = 1,
		VIS_INVIS = 2,
		VIS_WIZINVIS = 4;

	private boolean triggerListenerss = false;
		public boolean triggerPlayerMaskEvents() { return triggerListenerss; }
		public void triggerPlayerMaskEvents(boolean triggerListenerss) { this.triggerListenerss = triggerListenerss; }

	public boolean isLocal() { return (player != null); }

	private Player player = null;
		public Player getPlayer() { return player; }
		public void setPlayer(Player player) { this.player = player;}


	private String title = null;
	private int level = 0, trust = 0, vis = 0, active = 0;
	private Gender gender = null;
	private Race race = null;
	private Job job = null;


	public PlayerMask() {
		//empty
	}


	public String getShortName() { return getName(); }
	public void setShortName(String name) {
		setName(name);
		if(triggerPlayerMaskEvents())
			Jamud.getEventManager().triggerListeners(new PlayerMaskEvent(this, PlayerMaskEvent.EVENT_UPDATE, new Object[0]));
	}


	public String getTitle() { return title; }
	public void setTitle(String title) {
		this.title = title;
		if(triggerPlayerMaskEvents())
			Jamud.getEventManager().triggerListeners(new PlayerMaskEvent(this, PlayerMaskEvent.EVENT_UPDATE, new Object[0]));
	}


	public Gender getGender() {
		if(isLocal()) {
			Body b = (Body) player.getProperty(Player.PROPERTY_NATIVE_BODY);
			if ( b.getGender() == null )
				return( Gender.DEFAULT );
			else 
				return( b.getGender() );
		}
		else return gender;
	}
	public void setGender(Gender gender) {
		this.gender = gender;
		if(triggerPlayerMaskEvents())
			Jamud.getEventManager().triggerListeners(new PlayerMaskEvent(this, PlayerMaskEvent.EVENT_UPDATE, new Object[0]));
	}


	public Race getRace() {
		if(isLocal()) {
			Body b = (Body) player.getProperty(Player.PROPERTY_NATIVE_BODY);
			if ( b.getRace() == null )
				return( Race.DEFAULT );
			else 
				return( b.getRace() );
		}
		else return race;
	}
	public void setRace(Race race) {
		this.race = race;
		if(triggerPlayerMaskEvents())
			Jamud.getEventManager().triggerListeners(new PlayerMaskEvent(this, PlayerMaskEvent.EVENT_UPDATE, new Object[0]));
	}


	public Job getJob() {
		if(isLocal()) {
			Body b = (Body) player.getProperty(Player.PROPERTY_NATIVE_BODY);
			if ( b.getJob() == null )
				return( Job.DEFAULT );
			else 
				return( b.getJob() );
		}
		else return job;
	}
	public void setJob(Job job) {
		this.job = job;
		if(triggerPlayerMaskEvents())
			Jamud.getEventManager().triggerListeners(new PlayerMaskEvent(this, PlayerMaskEvent.EVENT_UPDATE, new Object[0]));
	}


	public int getLevel() {
		if(isLocal()) {
			Body b = (Body) player.getProperty(Player.PROPERTY_NATIVE_BODY);
			return b.getStats().stat(Body.STAT_LEVEL, 1);
		}
		else return level;
	}
	public void setLevel(int level) {
		this.level = level;
		if(triggerPlayerMaskEvents())
			Jamud.getEventManager().triggerListeners(new PlayerMaskEvent(this, PlayerMaskEvent.EVENT_UPDATE, new Object[0]));
	}


	public int getTrust() { return trust; }
	public void setTrust(int trust) {
		this.trust = trust;
		if(triggerPlayerMaskEvents())
			Jamud.getEventManager().triggerListeners(new PlayerMaskEvent(this, PlayerMaskEvent.EVENT_UPDATE, new Object[0]));
	}


	public int getVisible() {
		if(isLocal())
			return ((Body) player.getProperty(Player.PROPERTY_NATIVE_BODY)).getStats().stat(Body.STAT_VISIBLE, 0);
		else
			return vis;
	}
	public void setVisible(int vis) {
		this.vis = vis;
		if(triggerPlayerMaskEvents())
			Jamud.getEventManager().triggerListeners(new PlayerMaskEvent(this, PlayerMaskEvent.EVENT_UPDATE, new Object[0]));
	}


	public int getActive() { return active; }
	public void setActive(int active) {
		this.active = active;
		if(triggerPlayerMaskEvents())
			Jamud.getEventManager().triggerListeners(new PlayerMaskEvent(this, PlayerMaskEvent.EVENT_UPDATE, new Object[0]));
	}



	public void update(String title, int level, int trust, Gender gender, int vis, int active) {
		this.title = title;
		this.level = level;
		this.trust = trust;
		this.gender = gender;
		this.vis = vis;
		this.active = active;
		if(triggerPlayerMaskEvents())
			Jamud.getEventManager().triggerListeners(new PlayerMaskEvent(this, PlayerMaskEvent.EVENT_UPDATE, new Object[0]));
	}

	public String getWho() {
		/*
		System.out.println(" in getWho()");	
		System.out.println(" level = " + getLevel());
		System.out.println(" sex = " + getGender().getName());
		System.out.println(" name = " + getName());
		System.out.println(" title = " + getTitle());
		*/

		String w = "";
		w += "[" + Util.fixLength(""+getLevel(), 3, true) + "] ";
		w += getGender().getInitial().toUpperCase() + " ";
		w += getRace().getAbbreviation() + " ";
		w += getJob().getAbbreviation() + " ";
		w += getName();
		String t = title;
		w += (t != null ? " " + t : "" );
		return w;
	}

	private String tellLog = "";
	private int tellCount = 0;
	public int tell(PlayerMask from, String text) {

		if(isLocal()) {
			text = from.getName() + " tells you {4\"{$" + text + "{$\"{0";

			if(active == 0) {
				player.println(text);
				return TELL_OK;
			}
			else if((active & STATE_NOTELL) == STATE_NOTELL) {
				return TELL_OFF;
			}
			else if((active & STATE_LINKDEAD) == STATE_LINKDEAD) {
				tellLog += text + "\n";
				tellCount++;
				return TELL_ERR;
			}
			else if((active & STATE_AFK) == STATE_AFK) {
				tellLog += text + "\n";
				tellCount++;
				return TELL_AFK;
			}
		}

		//should never hit this
		return TELL_ERR;
	}

}
