package com.jamud.commands;


import jamud.*;
import jamud.object.*;
import jamud.util.*;
import java.util.*;


abstract class PlayerTrustMethods {
    
    
    /* - the meaty part of the interpreter -
     * These are the methods associated with various commands from
     * the PlayerInterpreter. They all need to take arguments of a
     * Player and a String (even if they don't use either actively)
     */


    public static final void LOGOUT(Player p, String args) {
	String fin = Jamud.currentInstance()
	    .getMessage( Jamud.MESSAGE_CLOSE );			  

	p.println( fin );
	p.terminate( Player.LOGOUT_NORMAL );
    }


    public static final void ENACT(Player p, String args) {
	MudObject body = p.getBody();
	if(body == null) {
	    p.println( "You don't seem to have a body..." );
	} else {
	    body.enact( args );
	}
	p.ready();
    }


    public static final void OBJECT_DUMP(Player p, String args) {
	MudObjectContainer root = Jamud.currentInstance()
	    .mudObjectRoot();
	p.println( o_dump(root, 1) );
	p.ready();
    }

    private static final String o_dump(MudObjectContainer c, int depth) {

	String indent;
	if(++depth > 0) {
	    indent = new String( new byte[depth] ).replace( '\0', ' ' );
	} else {
	    indent = "";
	}

	if(depth > 8) {
	    return indent.concat("[more...]");
	}

	Iterator i = c.childObjects();
	if( i.hasNext() ) {

	    StringBuffer sb = new StringBuffer();
	    synchronized(sb) {
		while( i.hasNext() ) {
		    MudObject next = (MudObject) i.next();
		    MudObjectContainer next_c = next.childContainer();
		    sb.append( indent );
		    sb.append( o_id(next) );
		    sb.append( "\n" );
		    sb.append( o_dump(next_c, depth) );
		}
		return sb.toString();
	    }
	} else {
	    return "";
	}
    }


    private static final String[] states = {
	" [TERMINATED]",
	" [INITIALIZING]",
	" [TERMINATING]",
	" [INITIALIZED]"
    };


    private static final String o_id(MudObject c) {
	String id = c.getTemplateId();
	String ln = Long.toString( c.getLoadId() );

	String st = states[c.initializableState()];

	if( c.isTemplate() ) {
	    return id.concat(st);
	} else {
	    if( id != null && id.length() > 0 ) {
		return id.concat(".").concat(ln).concat(st);
	    } else {
		return ln.concat(st);
	    }
	}
    }


    public static final void COLOUR(Player p, String args) {
	if( p.isFlagged( Colour.PLAYER_COLOUR_FLAG ) ) {
	    p.flags().remove( Colour.PLAYER_COLOUR_FLAG );
	    p.println("Colour off.");
	} else {
	    p.flags().add( Colour.PLAYER_COLOUR_FLAG );
	    p.println("%C%Colour on.%^^");
	}

	p.ready();
    }


}
