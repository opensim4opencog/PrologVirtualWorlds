package jamud.object;


import jamud.object.event.*;
import jamud.util.*;
import java.util.*;
import net.n3.nanoxml.*;


public class PlayerManager implements JamudEventTrigger {

    
    public static final String
	MARKUP = "PLAYERMANAGER";
    

    private static final String
	PROPERTY_PATH = "PATH",
	PROPERTY_TIMEOUT = "TIMEOUT";


    private String path;

    public String getPath() {
	return this.path;
    }


    private long timeout;

    public long getTimeout() {
	return this.timeout;
    }


    
    // collection of PlayerMasks
    private Set masks;
    
    public PlayerMask getPlayerMask(String name) {
	if(name == null || name.length() == 0) {
	    return null;
	}

	for(Iterator i = masks.iterator(); i.hasNext(); ) {
	    PlayerMask p = (PlayerMask) i.next();
	    if( p.getName().equalsIgnoreCase(name) ) {
		return p;
	    }
	}
	return null;
    }

    public boolean addPlayerMask(PlayerMask p) {
	return masks.add(p);
    }

    public boolean removePlayerMask(String name) {
	if(name == null || name.length() == 0) {
	    return false;
	}

	for(Iterator i = masks.iterator(); i.hasNext(); ) {
	    PlayerMask p = (PlayerMask) i.next();
	    if(p.getName().equalsIgnoreCase(name)) {
		i.remove();
		return true;
	    }
	}
	return false;
    }

    public boolean removePlayerMask(PlayerMask p) {
	return masks.remove(p);
    }


    // The Event Register (for JamudEventTrigger)
    private EventRegister events = new EventRegister();

    public boolean addListener(JamudEventListener listener,
			    Class eventClass) {
	return events.addListener(listener, eventClass);
    }

    public boolean removeListener(JamudEventListener listener) {
	return events.removeListener(listener);
    }

    public boolean removeListener(JamudEventListener listener,
			       Class eventClass) {
	return events.removeListener(listener, eventClass);
    }

    public boolean removeListeners() {
	return events.removeListeners();
    }

    public boolean removeListeners(Class eventClass) {
	return events.removeListeners(eventClass);
    }

    public void triggerListeners(JamudEvent event) {
	events.triggerListeners(event);
    }



    public PlayerManager() {
	this.masks = Collections.synchronizedSet(new HashSet());
    }



    public boolean logPlayerMaskIn(PlayerMask p) {
	if(masks.add(p)) {
	    JamudEvent ev = new PlayerMaskLoginEvent(p);
	    events.triggerListeners(ev);
	    return true;
	} else {
	    return false;
	}
    }


    public boolean logPlayerMaskOut(PlayerMask p, int reason) {
	if(masks.remove(p)) {
	    JamudEvent ev = new PlayerMaskLogoutEvent(p);
	    events.triggerListeners(ev);
	    return true;
	} else {
	    return false;
	}
    }


    public void logOutInactives() {
	long nt = System.currentTimeMillis();

	for(Iterator i = masks.iterator(); i.hasNext(); ) {
	    PlayerMask pm = (PlayerMask) i.next();
	    if(pm.isLocal() && pm instanceof Player) {
		Player p = (Player) pm;
		if( p.getConnection() == null &&
		    nt - p.lastCommandAt() >= this.timeout )
		    p.terminate(Player.LOGOUT_LINKDEATH);
	    }
	}
    }


    public void load(IXMLElement xml) {
	Attributes a = new Attributes( xml.getAttributes() );
	
	String p = a.getAttribute( PROPERTY_PATH, "/" );
	final char c = p.charAt( p.length() - 1 );
	if( c != '/' ) {
	    p = p.concat( "/" );
	}
	this.path = p;

	this.timeout = a.getAttribute( PROPERTY_TIMEOUT, (long)60000 );
    }


    public IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement( xml );
	return xml;
    }


    public void toXMLElement(IXMLElement xml) {
	xml.setAttribute( PROPERTY_PATH, this.path );
	xml.setAttribute( PROPERTY_TIMEOUT, Long.toString(this.timeout) );
    }
    
}
