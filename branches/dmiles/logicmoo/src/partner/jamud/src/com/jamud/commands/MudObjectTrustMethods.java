package com.jamud.commands;


import jamud.*;
import jamud.object.*;
import jamud.util.*;
import java.util.*;


abstract class MudObjectTrustMethods {
    
    
    /* - the meaty part of the interpreter -
     * These are the methods associated with various commands from
     * theMudObjectInterpreter. They all need to take arguments of a
     * MudObject and a String (even if they don't use either actively)
     */

    
    public static String LOOK(MudObject src, String args) {
	if(src == null) {
	    return null;
	} else if(args != null && args.length() > 0) {
	    return LOOKAT(src, args);
	}


	MudObjectContainer room = src.getParentContainer();
	if(room == null) {
	    return "You don't see anything at all.";
	}

	String de = room.getName();
	if(de == null || de.length() == 0) {
	    de = room.getShortDescription();
	}

	StringBuffer sb = new StringBuffer( de );
	synchronized( sb ) {
	    sb.append( "\n" );
	    
	    de = room.getDescription();
	    if(de == null || de.length() == 0) {
		de = "You don't see anything special here. Oh well.";
	    }
	    sb.append( de );
	    
	    sb.append( "\n  [ " );
	    
	    boolean hadExits = false;
	    
	    for(Iterator i = room.exits(); i.hasNext(); hadExits = true) {
		Entrance e = (Entrance) i.next();
		sb.append( e.getName() );

		if( i.hasNext() ) {
		    sb.append( ", " );
		}
	    }
	    
	    if(! hadExits) {
		sb.append( "none ]" );
	    } else {
		sb.append( " ]" );
	    }
	    
	    for(Iterator i = room.childObjects(); i.hasNext(); ) {
		MudObject m = (MudObject) i.next();

		final int v = m.attributes()
		    .getAttribute(MudObject.ATTRIB_VISIBILITY,
				  MudObject.ATTRIB_VISIBILITY_SHOW);
		
		if( (v & MudObject.ATTRIB_VISIBILITY_SHOW) > 0 ) {
		    sb.append("\n");
		    sb.append(m.getShortDescription());
		}
	    }
	    
	    return sb.toString();
	}
    }


    public static final String LOOKAT(MudObject src, String args) {

	if(args == null || args.length() == 0) {
	    return "Look at what?";
	}

	MudObjectContainer room = src.getParentContainer();
	if(room == null) {
	    return "You don't see anything at all.";
	}

	//first, try and find a match in peers
	for(Iterator i = room.childObjects(); i.hasNext(); ) {
	    Viewable v = (Viewable) i.next();
	    if( v.matchKeyword( args ) ||
		v.getName().equals( args ) ) {
		return _lookat(v);
	    }
	}

	//then, try and find a match in exits
	for(Iterator i = room.exits(); i.hasNext(); ) {
	    Viewable v = (Viewable) i.next();
	    if( v.matchKeyword( args ) ||
		v.getName().equals( args ) ) {
		return _lookat(v);
	    }
	}

	return "Look at what? Where? Huh?";
    }


    private static final String _lookat(Viewable v) {
	String r, n;

	n = v.getName();
	if( n != null && n.length() > 0 ) {
	    r = n.concat("\n");
	} else {
	    r = "";
	}

	n = v.getDescription();
	if( n != null && n.length() > 0 ) {
	    return r.concat(n);
	} else {
	    return r.concat("You don't see anything interesting.");
	}	
    }


    public static final String MOVE(MudObject src, String args) {
	if(src == null) {
	    return null;
	}

	MudObjectContainer room = src.getParentContainer();
	if(room == null) {
	    return "You don't seem to be in a room... how weird.";
	}

	Entrance exit = room.getExit( args );
	if(exit == null) {
	    return "There's no such exit!";
	} else {
	    try {
		exit.traverse( src );
		return LOOK( src, null );
	    } catch(EntranceException ee) {
		return "You can't go that way";
	    }
	}

    }


    public static final String MOVE_NORTH(MudObject src, String args) {
	return MOVE(src, "North");
    }


    public static final String MOVE_SOUTH(MudObject src, String args) {
	return MOVE(src, "South");
    }


    public static final String MOVE_EAST(MudObject src, String args) {
	return MOVE(src, "East");
    }


    public static final String MOVE_WEST(MudObject src, String args) {
	return MOVE(src, "West");
    }


    public static final String MOVE_UP(MudObject src, String args) {
	return MOVE(src, "Up");
    }


    public static final String MOVE_DOWN(MudObject src, String args) {
	return MOVE(src, "Down");
    }



    public static final String SAY(MudObject src, String args) {
	if(src == null) {
	    return null;
	}
	
	args = args.trim().concat("'");
	MudObjectContainer room = src.getParentContainer();
	if(room != null) {
	    String out = ("$n says, '").concat(args);
	    room.printAction( src, out );
	}

	return "You say, '".concat(args);
    }

}
