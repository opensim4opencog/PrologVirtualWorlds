package com.jamud.commands;


import jamud.command.*;
import jamud.object.*;
import jamud.util.*;
import java.util.*;
import net.n3.nanoxml.*;


class Social implements Command {

    
    public static final String
	MARKUP = "SOCIAL";

    
    private static final String
	PROPERTY_NAME = "NAME",
	PROPERTY_ALLOW = "ALLOW",
	PROPERTY_DENY = "DENY",
	PROPERTY_RACE = "RACE",
	PROPERTY_GENDER = "GENDER",
	PROPERTY_JOB = "JOB";

    
    private static final int
	ACTOR_NO_TARGET = 0,
	PEERS_NO_TARGET = 1,
	ACTOR_TARGET_IS_INVALID = 2,
	ACTOR_ACTOR_IS_TARGET = 3,
	PEERS_ACTOR_IS_TARGET = 4,
	ACTOR = 5,
	TARGET = 6,
	PEERS = 7;
    
    private static final String[] PART = {
	"ACT_NOTARG",	// untargeted, actor echo
	"PER_NOTARG",	// untargeted, local echo
	"ACT_BADTARG",	// bad target, actor echo
	"ACT_ACTTARG",	// actor targeted, actor echo
	"PER_ACTTARG",	// actor targeted, local echo
	"ACTOR",	// actor echo
	"TARGET",	// target echo
	"PEERS" };	// local echo
    

    // allow and deny lists
    private FlagList
	allowGender,
	allowRace, 
	allowJob, 
	denyGender,
	denyRace,
	denyJob;


    // String list of parts as indexed by the PART[]
    String[] parts;

    public String[] getParts() {
	String[] p = new String[parts.length];
	System.arraycopy(parts, 0, p, 0, p.length);
	return p;
    }

    public String getPart(int which) {
	return parts[which];
    }

    public void setPart(int which, String part) {
	this.parts[which] = part;
    }
    

    private String name;

    public String getName() {
	return this.name;
    }


    
    public Social(String name) {
	this();
	this.name = name;
    }

    
    private Social() {
	//parts
	this.parts = new String[PART.length];
	
	// allow and deny
	this.allowGender = new FlagList();
	this.allowRace = new FlagList();
	this.allowJob = new FlagList();
	this.denyGender = new FlagList();
	this.denyRace = new FlagList();
	this.denyJob = new FlagList();
    }
    
    
    
    // returns true if the actor passes all the allow and deny rules
    public boolean allows(Actor actor) {
	if(actor != null) {
	    if(actor instanceof Player) {
		if((actor = ((Player)actor).getBody()) != null ) {
		    return allows( (MudObject)actor );
		}
	    } else if(actor instanceof MudObject) {
		return allows( (MudObject)actor );
		
	    }
	}

	return false;
    }


    private boolean allows(MudObject actor) {
	return (allows(actor.getGender().getName(), allowGender, denyGender) &&
		allows(actor.getRace().getName(), allowRace, denyRace) &&
		allows(actor.getJob().getName(), allowJob, denyJob) );
    }
    
    
    private boolean allows(String flag, FlagList yes, FlagList no) {
	if(no.contains(flag)) return false;    //explicitly denied?
	else if(yes.size() == 0) return true;  //implicitly allowed?
	else return (yes.contains(flag));      //explicitly allowed?
    }


    private MudObject getPeer(MudObject s, String keyword) {
	MudObjectContainer room = s.getParentContainer();
	if( room == null ) {
	    return null;
	}

	for(Iterator i = room.childObjects(keyword); i.hasNext(); ) {
	    MudObject m = (MudObject) i.next();
	    if( m != s && m.matchKeyword(keyword) ) {
		return m;
	    }
	}

	if(keyword.equalsIgnoreCase("self") ||
	   keyword.equalsIgnoreCase("me")) {
	    return s;
	} else {
	    return null;
	}
    }


    public void execute(Actor actor, String args) throws CommandException {
	if(actor == null) {
	    throw new CommandException(this, actor, "Actor is null");

	} else if(actor instanceof Player) {
	    if((actor = ((Player)actor).getBody()) != null ) {
		MudObject o = (MudObject)actor;
		o.println( execute(o, args) );
		
	    }

	} else if(actor instanceof MudObject) {
	    MudObject o = (MudObject)actor;
	    o.println( execute(o, args) );

	} else {
	    throw new CommandException(this, actor,
				       "Actor is not a MudObject");
	}
    }


    private String execute(MudObject actor, String args) {

	if(args != null && args.length() > 0) {
	    String tn = InterpreterManager.commandSplit(args)[0];
	    MudObject targ = getPeer(actor, tn);
	    return execute(actor, targ);
	    
	} else {
	    return execute(actor);
	}
    }
    

    private String execute(MudObject actor) {

	String R = parts[PEERS_NO_TARGET];
	if(R != null && R.length() > 0) {
	    actor.getParentContainer().printAction(actor, R);
	}

	String A = parts[ACTOR_NO_TARGET];
	if( A == null || A.length() == 0 ) {
	    return "To whom?";
	} else {
	    return MudObject.parseAction( actor, A );
	}
    }
    
    
    private String execute(MudObject actor, MudObject target) {
	
	// target specified was not found
	if( target == null ) {

	    String A = parts[ACTOR_TARGET_IS_INVALID];
	    if( A == null || A.length() == 0 ) {
		return "But they aren't here.";
	    } else {
		return MudObject.parseAction( actor, target, A );
	    }

	// target specified was the actor
	} else if( target == actor ) {

	    String R = parts[PEERS_ACTOR_IS_TARGET];
	    if( R != null && R.length() > 0 ) {
		actor.getParentContainer().printAction(actor, target, R);
	    }

	    String A = parts[ACTOR_ACTOR_IS_TARGET];
	    if( A == null || A.length() == 0) {
		return "You aren't that flexible.";
	    } else {
		return MudObject.parseAction( actor, target, A );
	    }

	// target is valid and non-reflexive
	} else {
	    
	    String T = parts[Social.TARGET];	    
	    if(T == null || T.length() == 0) {
		return "You reconsider, that would be rude.";
	    } else {
		MudObjectContainer room = actor.getParentContainer();

		String R = parts[Social.PEERS];
		if(R == null || R.length() == 0) {
		    room.printAction(actor, target, T);
		} else {
		    room.printAction(actor, target, T, R);
		}

		String A = parts[Social.ACTOR];
		if( A == null || A.length() == 0 ) {
		    return "You do just that.";
		} else {
		    return MudObject.parseAction( actor, target, A );
		}
	    }

	}
    }


    public void load(IXMLElement xml) {

	for(Enumeration enum = xml.enumerateChildren();
	    enum.hasMoreElements(); ) {

	    this.name = xml.getAttribute(PROPERTY_NAME, "");

	    IXMLElement nxt = (IXMLElement) enum.nextElement();
	    String nom = nxt.getName();

	    if(nom.equals(PROPERTY_ALLOW)) {
		String t;

		t = nxt.getAttribute(PROPERTY_GENDER, null);
		if(t != null)
		    allowGender.refresh(t);

		t = nxt.getAttribute(PROPERTY_RACE, null);
		if(t != null)
		    allowRace.refresh(t);

		t = nxt.getAttribute(PROPERTY_JOB, null);
		if(t != null)
		    allowJob.refresh(t);

	    } else if(nom.equals(PROPERTY_DENY)) {
		String t;

		t = nxt.getAttribute(PROPERTY_GENDER, null);
		if(t != null)
		    denyGender.refresh(t);

		t = nxt.getAttribute(PROPERTY_RACE, null);
		if(t != null)
		    denyRace.refresh(t);

		t = nxt.getAttribute(PROPERTY_JOB, null);
		if(t != null)
		    denyJob.refresh(t);
		
	    } else {
		for(int I=PART.length; I-- > 0; ) {
		    if( nom.equals(PART[I]) ) {
			parts[I] =  nxt.getContent();
			break;
		    }
		}
	    }
	}
    }

    
    public static Social loadSocial(IXMLElement xml) {
	Social s = new Social();
	s.load(xml);
	return s;
    }


    public void toXMLElement(IXMLElement xml) {

	// Social name
	xml.setAttribute(PROPERTY_NAME, name);
	
	IXMLElement nxt;
	
	// allow
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
	
	// deny
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
	
	// parts
	for(int I=parts.length; I-- > 0; ) {
	    if(parts[I] != null) {
		nxt = new XMLElement();
		nxt.setName(PART[I]);
		nxt.setContent(parts[I]);
		xml.addChild(nxt);
	    }
	}

    }

  
    public IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }


    public int compareTo(Object o) {
	if(o instanceof Command) {
	    return this.compareTo( (Command)o );
	} else {
	    return -1;
	}
    }


    public int compareTo(Command p) {
	return this.name.compareTo(p.getName());
    }


    public final boolean equals(Object o) {
	return (this == o);
    }

}
