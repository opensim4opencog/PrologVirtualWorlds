package  logicmoo.obj;


import logicmoo.*; 
import logicmoo.obj.event.*;
import logicmoo.util.*;
import java.io.FileWriter;
import java.util.*;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public class Microtheory extends CycThing implements JamudEventTrigger {

    private static final String MARKUP = "MICROTHEORY";

    private static final String
    PROPERTY_SRC = "SRC",
    PROPERTY_PREFIX = "PREFIX",
    PROPERTY_AUTHOR = "AUTHOR",
    PROPERTY_ARTIFACTS = "ARTIFACTS",
    PROPERTY_BODIES = "BODIES",
    PROPERTY_RESETS = "RESETS",
    PROPERTY_GEOGRAPHICALREGIONS = "GEOGRAPHICALREGIONS";

    private String src;
    public String getSrc() { return src;}
    public void setSrc(String src) { this.src = src;}

    private String author;
    public String getAuthor() { return author;}
    public void setAuthor(String author) { this.author = author;}

    protected Hashtable artifacts;
    public Hashtable getArtifacts() { return artifacts;}

    protected Hashtable agents;
    public Hashtable getAgentGenerics() { return agents;}

    protected Hashtable rules;
    public Hashtable getForwardRules() { return rules;}

    protected Hashtable geographicalMicrotheorys;
    public Hashtable getGeographicalRegions() { return geographicalMicrotheorys;}



    protected Microtheory() {
	artifacts = new Hashtable();
	agents = new Hashtable();
	rules = new Hashtable();
	geographicalMicrotheorys= new Hashtable();
    }


    public void println(String text) {
	for ( Enumeration rms = geographicalMicrotheorys.elements(); rms.hasMoreElements(); )
	    ((GeographicalRegion) rms.nextElement()).println(text);
    }


    public void println(AgentGeneric from, String f_text, String text) {
	for ( Enumeration enum = geographicalMicrotheorys.elements(); enum.hasMoreElements(); )
	    ((GeographicalRegion) enum.nextElement()).println(from, f_text, text);
    }


    public void tick() {
	//apply rules
	for ( Enumeration enum = rules.elements(); enum.hasMoreElements(); )
	    ((ForwardRule) enum.nextElement()).apply();

	//tick each room
	for ( Enumeration enum = geographicalMicrotheorys.elements(); enum.hasMoreElements(); )
	    ((GeographicalRegion) enum.nextElement()).tick();
    }

}