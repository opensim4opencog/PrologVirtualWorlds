package com.jamud.commands;


import jamud.*;
import jamud.command.*;
import jamud.plugin.*;
import java.io.*;
import java.util.*;
import net.n3.nanoxml.*;


public class SocialsPlugin extends JamudPlugin {


    private static final String
	PARAM_PRIORITY = "PRIORITY";

    private static final String
	PROPERTY_SOCIALS = "SOCIALS",
	PROPERTY_SRC = "SRC";


    private MudObjectInterpreter socials;


    public final String getName() {
	return "Social Commands (Socials)";
    }

    public final String getVersion() {
	return "1.0";
    }

    public final String getAuthor() {
	return "siege <siege@jamud.com>";
    }

    public final String getInfo() {
	return "The social commands (aka socials). Emotive emulation of"
	    .concat( " common activities." );
    }

    
    private int state = STATE_TERMINATED;

    public int initializableState() {
	return this.state;
    }    

    public boolean isActive() {
	return (this.state == STATE_INITIALIZED);
    }



    public SocialsPlugin() {
	super();
    }



    public synchronized boolean initialize() {
	System.out.println("begin: SocialCommands.initialize()");
	if(state > STATE_TERMINATED) {
	    System.out.println(" not yet terminated");
	    System.out.println("end: SocialCommands.initialize()");
	    return false;
	} else {
	    state = STATE_INITIALIZING;
	}

	Jamud.currentInstance().interpreterManager()
	    .addInterpreter(socials);
	
	state = STATE_INITIALIZED;
	System.out.println("end: SocialCommands.initialize()");
	return true;
    }


    public synchronized boolean terminate() {
	System.out.println("begin: SocialCommands.terminate()");
	if(state < STATE_INITIALIZED) {
	    System.out.println(" not yet initialized");
	    System.out.println("end: SocialCommands.terminate()");
	    return false;
	} else {
	    state = STATE_TERMINATING;
	}

	Jamud.currentInstance().interpreterManager()
	    .removeInterpreter(socials);
	
	state = STATE_TERMINATED;
	System.out.println("end: SocialCommands.terminate()");
	return true;
    }


    public void load(IXMLElement xml) throws Exception {
	super.load(xml);

	final int priority = parameters().getAttribute(PARAM_PRIORITY, 3);
	this.socials = new MudObjectInterpreter(getName(), priority, this);

	for(Enumeration enum = xml.enumerateChildren();
	    enum.hasMoreElements(); ) {

	    IXMLElement nxt = (IXMLElement) enum.nextElement();
	    String nom = nxt.getName();

	    if( nom.equals( Social.MARKUP ) ) {

		this.socials.addCommand( Social.loadSocial( nxt ) );

	    } else if( nom.equals( PROPERTY_SOCIALS ) ) {

		loadSocialsElement( nxt );
	    }
	}
    }


    public void loadSocialsElement(IXMLElement xml) throws Exception {
	String src = xml.getAttribute(PROPERTY_SRC, null);

	// If the passed xml element just points to a file
	if(src != null) {
	    IXMLParser xmp = new StdXMLParser();
	    xmp.setBuilder(new StdXMLBuilder());
	    xmp.setValidator(new NonValidator());
	    xmp.setReader(StdXMLReader.fileReader(src));

	    xml = (XMLElement) xmp.parse();
	    xmp = null;
	}	

	int count = 0;
	for(Enumeration enum = xml.getChildrenNamed(Social.MARKUP).elements();
	    enum.hasMoreElements(); count++) {
	    
	    IXMLElement nxt = (IXMLElement) enum.nextElement();
	    this.socials.addCommand( Social.loadSocial( nxt ) );
	}
    }

}
