package logicmoo.cmd;


import logicmoo.*;
import logicmoo.obj.*;
import logicmoo.util.*;
import java.util.Enumeration;
import net.n3.nanoxml.*;


public class Social {

	public static final String MARKUP = "SOCIAL";

	private static final String
		PROPERTY_NAME = "NAME",
		PROPERTY_ALLOW = "ALLOW",
		PROPERTY_DENY = "DENY",
		PROPERTY_RACE = "RACE",
		PROPERTY_GENDER = "GENDER",
		PROPERTY_JOB = "JOB";

	public static final int
		ACTOR_NO_TARGET = 0,
		PEERS_NO_TARGET = 1,
		ACTOR_TARGET_IS_INVALID = 2,
		ACTOR_ACTOR_IS_TARGET = 3,
		PEERS_ACTOR_IS_TARGET = 4,
		ACTOR = 5,
		TARGET = 6,
		PEERS = 7;									

	public static final String[] PART = {
		"ACT_NOTARG",		// untargeted, actor echo
		"PER_NOTARG",		// untargeted, local echo
		"ACT_BADTARG",		// bad target, actor echo
		"ACT_ACTTARG",		// actor targeted, actor echo
		"PER_ACTTARG",		// actor targeted, local echo
		"ACTOR",				//	actor echo
		"TARGET",			// target echo
		"PEERS" };			// local echo

	private static final String
		MARK_ACTOR_NAME = "%n",
		MARK_ACTOR_PRON = "%e",
		MARK_ACTOR_NOM = "%m",
		MARK_ACTOR_POSS = "%s",
		MARK_ACTOR_RACE_ADJ = "%r";

	private static final String
		MARK_TARGET_NAME = "%N",
		MARK_TARGET_PRON = "%E",
		MARK_TARGET_NOM = "%M",
		MARK_TARGET_POSS = "%S",
		MARK_TARGET_RACE_ADJ = "%R";


	private FlagList allowGender = new FlagList();
	private FlagList allowRace = new FlagList();
	private FlagList allowJob = new FlagList();
	private FlagList denyGender = new FlagList();
	private FlagList denyRace = new FlagList();
	private FlagList denyJob = new FlagList();

   private String name;
      public String getName() { return name; }
      public void setName(String name) { this.name = name; }

	String[] parts;
		public String[] getParts() { return parts; }
		public String getPart(int which) { return parts[which]; }
		public void setPart(int which, String part) { parts[which] = part; }



   public Social(String name) {
		this();
      this.name = name;
   }

   public Social() {
		parts = new String[PART.length];
   }



	public boolean allow(AgentGeneric actor) {
		return (
			allow(actor.getGender().getName(), allowGender, denyGender)
			&& allow(actor.getRace().getName(), allowRace, denyRace)
			&&	allow(actor.getJob().getName(), allowJob, denyJob)
		);
	}


	private boolean allow(String flag, FlagList yes, FlagList no) {
		if(no.contains(flag)) return false;		//explicitly denied?
		else if(yes.size() == 0) return true;	//implicitly allowed?
		else return (yes.contains(flag));		//explicitly allowed?
	}


	public void execute(AgentGeneric actor) {
		GeographicalRegion room = actor.getGeographicalRegion();

		String A = Social.parse(getPart(Social.ACTOR_NO_TARGET), actor, null);
		String R = Social.parse(getPart(Social.PEERS_NO_TARGET), actor, null);

		if(A == null)
			actor.println("To whom?");
		else
			room.println(actor, A, R);
	}


	public void execute(AgentGeneric actor, AgentGeneric target) {
		GeographicalRegion room = actor.getGeographicalRegion();
		
		if(target==null) {

         String A = getPart(Social.ACTOR_TARGET_IS_INVALID);
         A = ((A != null && A.length() > 0) ? Social.parse(A, actor, null) :
				"But they aren't here." );
      	actor.println(A);

		}
		else if(target==actor) {

         String A = Social.parse(getPart(Social.ACTOR_ACTOR_IS_TARGET), actor, target);
         String R = Social.parse(getPart(Social.PEERS_ACTOR_IS_TARGET), actor, target);

         if(A == null)
				actor.println("You aren't that flexible.");
         else
				room.println(actor, A, R);

      }
      else {

         String A = Social.parse(getPart(Social.ACTOR), actor, target);
         String T = Social.parse(getPart(Social.TARGET), actor, target);
         String R = Social.parse(getPart(Social.PEERS), actor, target);

         if(T == null)
				actor.println("You reconsider, that would be rude.");
         else
				room.println(actor, A, target, T, R);

      }
	}


   public static boolean trySocial(AgentGeneric actor, String cmd, String args) {

		for(Enumeration enum = Jamud.getSocialManager().elements(); enum.hasMoreElements(); ) {
			Social s = (Social) enum.nextElement();

         if(s.getName().startsWith(cmd) && s.allow(actor)) {
				if(args.length() > 0)
					s.execute(actor, actor.getGeographicalRegion().getAgentGeneric(Util.split(args, " ")[0]));
				else
					s.execute(actor);

				return true;
         }
      }

      return false;
   }


	public static String parse(String parse, AgentGeneric source, AgentGeneric target) {
		if(parse == null || parse.length() == 0)
			return null;

		parse = Util.replace(parse, MARK_ACTOR_RACE_ADJ, source.getRace().getAdj());
		parse = Util.replace(parse, MARK_ACTOR_PRON, source.getGender().getPron());
		parse = Util.replace(parse, MARK_ACTOR_NOM, source.getGender().getNom());
		parse = Util.replace(parse, MARK_ACTOR_POSS, source.getGender().getPoss());
		parse = Util.replace(parse, MARK_ACTOR_NAME, source.getName());

		if(target != null){
			parse = Util.replace(parse, MARK_TARGET_RACE_ADJ, target.getRace().getAdj());
			parse = Util.replace(parse, MARK_TARGET_PRON, target.getGender().getPron());
			parse = Util.replace(parse, MARK_TARGET_NOM, target.getGender().getNom());
			parse = Util.replace(parse, MARK_TARGET_POSS, target.getGender().getPoss());
			parse = Util.replace(parse, MARK_TARGET_NAME, target.getName());
		}

		return parse;
	}


	public static Social loadSocial(XMLElement xml) throws Exception {
		if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in Social.loadSocial");

		Social s = new Social(xml.getAttribute(PROPERTY_NAME));

		for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
			XMLElement nxt = (XMLElement) enum.nextElement();

			String nom = nxt.getName();
			if(nom.equals(PROPERTY_ALLOW)) {

				String t;
				t = nxt.getAttribute(PROPERTY_GENDER, null);
				if(t != null)
					s.allowGender = new FlagList(t);
				t = nxt.getAttribute(PROPERTY_RACE, null);
				if(t != null)
					s.allowRace = new FlagList(t);
				t = nxt.getAttribute(PROPERTY_JOB, null);
				if(t != null)
					s.allowJob = new FlagList(t);

			}
			else if(nom.equals(PROPERTY_DENY)) {

				String t;
				t = nxt.getAttribute(PROPERTY_GENDER, null);
				if(t != null)
					s.denyGender = new FlagList(t);
				t = nxt.getAttribute(PROPERTY_RACE, null);
				if(t != null)
					s.denyRace = new FlagList(t);
				t = nxt.getAttribute(PROPERTY_JOB, null);
				if(t != null)
					s.denyJob = new FlagList(t);

			}
			else {

				for(int I=0; I<PART.length; I++)
					if(nom.equals(PART[I])) {
						s.setPart(I, nxt.getContent());
						break;
					}

			}
		}

		return s;
	}


	public XMLElement toXMLElement() {
		//Social
		XMLElement xml = new XMLElement();
		xml.setName(MARKUP);
		xml.setAttribute(PROPERTY_NAME, name);

		XMLElement nxt;

		//Social.allow
		if( (allowGender.size() + allowRace.size() + allowJob.size()) > 0 ) {
			nxt = new XMLElement();
			nxt.setName(PROPERTY_ALLOW);
			if(allowGender.size() != 0)
				nxt.setAttribute(PROPERTY_GENDER, allowGender.toString());
			if(allowRace.size() != 0)
				nxt.setAttribute(PROPERTY_RACE, allowRace.toString());
			if(allowJob.size() != 0)
				nxt.setAttribute(PROPERTY_JOB, allowJob.toString());
			xml.addChild(nxt);
		}

		//Social.deny
		if( (denyGender.size() + denyRace.size() + denyJob.size()) > 0 ) {
			nxt = new XMLElement();
			nxt.setName(PROPERTY_DENY);
			if(denyGender.size() != 0)
				nxt.setAttribute(PROPERTY_GENDER, denyGender.toString());
			if(denyRace.size() != 0)
				nxt.setAttribute(PROPERTY_RACE, denyRace.toString());
			if(denyJob.size() != 0)
				nxt.setAttribute(PROPERTY_JOB, denyJob.toString());
			xml.addChild(nxt);
		}

		//Social.parts[]
		for(int I=0; I<parts.length; I++)
			if(parts[I] != null) {
				nxt = new XMLElement();
				nxt.setName(PART[I]);
				nxt.setContent(parts[I]);
				xml.addChild(nxt);
			}

		return xml;
	}


}
