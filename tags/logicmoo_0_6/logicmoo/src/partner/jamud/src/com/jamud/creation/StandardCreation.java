package com.jamud.creation;


import jamud.Jamud;
import jamud.object.*;
import jamud.plugin.*;
import jamud.util.*;
import java.util.*;


public class StandardCreation extends JamudPlugin {


    private static final String PARAM_HOME = "HOME";


    public final String getName() {
	return "Standard Creation Module";
    }

    public final String getVersion() {
	return "1.0";
    }

    public final String getAuthor() {
	return "siege <siege@jamud.com>";
    }

    public final String getInfo() {
	return "Provides standard player creation.";
    }

    
    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state;
    }


    private Creation creator;



    public StandardCreation() {

	this.creator = new Creation() {
		public void enact(Player p) throws CreationException {
		    doEnact(p);
		}
	    };
    }



    public synchronized boolean initialize() {
	if(this.state > STATE_TERMINATED) {
	    return false;
	} else {
	    this.state = STATE_INITIALIZING;
	}

	Jamud.currentInstance().setCreation( this.creator );

	this.state = STATE_INITIALIZED;
	return true;
    }


    public synchronized boolean terminate() {
	if(this.state < STATE_INITIALIZED) {
	    return false;
	} else {
	    this.state = STATE_TERMINATING;
	}

	Jamud jamud = Jamud.currentInstance();
	if( jamud.getCreation() == this.creator ) {
	    jamud.setCreation( null );
	}

	this.state = STATE_TERMINATED;
	return true;
    }


    public boolean isActive() {
	return (this.state == STATE_INITIALIZED);
    }



    private final void doEnact(Player p) throws CreationException {

	Jamud jamud = Jamud.currentInstance();

	String tmp = "Player not found, beginning Standard Creation";
	
	p.println( tmp );
	tmp = p.prompt( "Create new character named '"
			.concat( p.getShortName() )
			.concat( "'? [y/n]" ) );
	
	if( ! tmp.trim().toLowerCase().startsWith("y") ) {
	    throw new CreationException("Creation willingly aborted");
	}
	
	tmp = p.prompt( "new password: " );
	if( tmp.equals( p.prompt( "repeat : " ) ) ) {
	    p.setPassword( null, tmp );
	}
	
	Attributes attrib = p.attributes();
	
	tmp = p.prompt( "email address: " );
	if( tmp.trim().length() > 0 ) {
	    attrib.setAttribute( Player.ATTRIB_EMAIL, tmp ) ;
	}
	
	tmp = p.prompt( "player title: " );
	if( tmp.trim().length() > 0 ) {
	    p.setTitle( tmp );
	}
	
	tmp = p.prompt( "enter player info? [y/n]" );
	if( tmp.trim().toLowerCase().startsWith("y") ) {
	    p.println( "Write Mode. Enter lines of text. To finish enter"
		       + " a line containing only a period (.)" ); 
	    
	    StringBuffer sb = new StringBuffer();
	    synchronized(sb) {
		for(tmp = p.prompt(null); !tmp.equals(".");
		    tmp = p.prompt(null) ) {

		    sb.append( tmp );
		    sb.append( "\n" );
		}
		p.setInfo( sb.toString() );
	    }
	    p.println( "Initial player info set." );
	}
	
	String name = p.getShortName();
	
	// HOME
	MudObjectContainer c = p.obtainHome();
	c.setName( name.concat("'s Home") );
	c.setDescription( "Welcome Home" );
	c.setShortDescription( c.getName() );
	
	MudObjectContainer tc = null;
	String te_tid = parameters().getAttribute( PARAM_HOME, null );
	if(te_tid != null) {
	    MudObject t_o = MudObject.getMudObjectTemplate(te_tid);
	    if(t_o != null) {
		tc = t_o.childContainer();
	    }
	}
	if(tc == null) {
	    tc = jamud.mudObjectRoot();
	}
	Entrance te = (Entrance) new Entrance.Down();
	te.setDestination( tc );
	c.addExit( te );
	c.parentObject().initialize();
	
	Gender g = promptGender( p );
	Job j = promptJob( p );
	Race r = promptRace( p );
	
	// BODY
	MudObject b = p.obtainNativeBody();
	b.setGender( g );
	b.setJob( j );
	b.setRace( r );
	b.initialize();
	p.home().addChildObject( b );
	p.setBody( b );

	// Placement
	jamud.mudObjectRoot().addChildObject(c.parentObject());
	c.addChildObject( b );
	
	tmp = "%1%[%^^".concat(name).concat("%1%]$%^^ ");
	attrib.setAttribute( Player.ATTRIB_PROMPT, tmp );

	if(promptColour(p)) {
	    p.flags().add( Colour.PLAYER_COLOUR_FLAG );
	    p.println("%C%Colour on.%^^");
	} else {
	    p.println("Colour off.");
	}
    }    


    private static final boolean promptColour(Player p) {
	String pr = "Would you like to enable ANSI Colour? [y/n]";
	pr = p.prompt( pr );

	return ( pr.trim().toLowerCase().startsWith("y") );
    }


    private static final Gender promptGender(Player p) {
	String pr = "Please select a character gender:\n";
	StringBuffer sb = new StringBuffer( pr );
	synchronized(sb) {
	    for(Iterator i = Gender.genders(); i.hasNext(); ) {
		Gender g = (Gender) i.next();
		sb.append( g.getName() );
		sb.append( ", " );
	    }
	    sb.append( "? for help\n: " );
	    pr = sb.toString();
	}

	String tmp;

	for( tmp = p.prompt( pr ); true; tmp = p.prompt( pr ) ) {
	    if( tmp.equals("?") ) {
		p.println("Simply enter in the name of a gender in the list");
	    } else {
		Gender g = Gender.getGender( tmp );
		if( g != null) {
		    return g;
		} else {
		    p.println( "Invalid selection" );
		}
	    }
	}
    }


    private static final Job promptJob(Player p) {
	String pr = "Please select a character job:\n";
	StringBuffer sb = new StringBuffer( pr );
	synchronized(sb) {
	    for(Iterator i = Job.jobs(); i.hasNext(); ) {
		Job g = (Job) i.next();
		sb.append( g.getName() );
		sb.append( ", " );
	    }
	    sb.append( "? for help\n: " );
	    pr = sb.toString();
	}

	String tmp;

	for( tmp = p.prompt( pr ); true; tmp = p.prompt( pr ) ) {
	    if( tmp.equals("?") ) {
		p.println("Simply enter in the name of a job in the list");
	    } else {
		Job g = Job.getJob( tmp );
		if( g != null) {
		    return g;
		} else {
		    p.println( "Invalid selection" );
		}
	    }
	}
    }


    private static final Race promptRace(Player p) {
	String pr = "Please select a character race:\n";
	StringBuffer sb = new StringBuffer( pr );
	synchronized(sb) {
	    for(Iterator i = Race.races(); i.hasNext(); ) {
		Race g = (Race) i.next();
		sb.append( g.getName() );
		sb.append( ", " );
	    }
	    sb.append( "? for help\n: " );
	    pr = sb.toString();
	}

	String tmp;

	for( tmp = p.prompt( pr ); true; tmp = p.prompt( pr ) ) {
	    if( tmp.equals("?") ) {
		p.println("Simply enter in the name of a race in the list");
	    } else {
		Race g = Race.getRace( tmp );
		if( g != null) {
		    return g;
		} else {
		    p.println( "Invalid selection" );
		}
	    }
	}
    }

}
