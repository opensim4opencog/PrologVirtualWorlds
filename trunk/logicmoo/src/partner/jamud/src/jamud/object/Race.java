package jamud.object;


import jamud.util.FlagList;
import jamud.util.Util;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;
import net.n3.nanoxml.*;


public class Race {
      

    private static HashMap races = new HashMap();

    public static final Race getRace(String name) {
	if(name != null &&
	   (name = name.trim().toLowerCase()).length() > 0 )
	    return (Race) races.get(name);
	else
	    return null;
    } 

    public static final Iterator races() {
	return races.values().iterator();
    }



    /** <code>MARKUP = "RACE"</code> */
    public static final String
	MARKUP = "RACE";
    

    private static final String
	PROPERTY_NAME = "NAME",
	PROPERTY_ADJ = "ADJ",
	PROPERTY_ABBREV = "ABBREV",
	PROPERTY_SECTIONS = "SECTIONS";
    

    public static final Race DEFAULT = new Race();

    static {
	DEFAULT.name = "thing";
	DEFAULT.adjective = "thingy";
	DEFAULT.abbreviation = "     ";
    }

    
    private FlagList sections;

    public FlagList sections() {
	return this.sections;
    }
    

    private String
	name,
	adjective,
	abbreviation;
    

    public String getName() {
	return this.name;
    }

    public String getAdj() {
	return this.adjective;
    }

    public String getAbbreviation() {
	return this.abbreviation;
    }

    
    
    public Race(String name) {
	this.sections = new FlagList();
	this.name = name.trim().toLowerCase();
	this.adjective = name + "ish";
	this.abbreviation = Util.fixLength(name,  5);
    }
    
    public Race() {
	this.sections = new FlagList();
    }
    
    
        
    public static final Race loadRace(IXMLElement xml) {
	Race race;

	if( xml.getChildrenCount() == 0 ) {
	    String name = xml.getAttribute(PROPERTY_NAME, "").toLowerCase();
	    race = (Race) races.get( name );

	    if(race == null) {
		race = DEFAULT;
	    }

	} else {

	    race = new Race();
	    race.load(xml);
	    races.put( race.name, race );

	}

	return race;
    }


    public void load (IXMLElement xml) {

	this.name = xml.getAttribute(PROPERTY_NAME, "").toLowerCase();
	
	for(Enumeration enum = xml.enumerateChildren();
	    enum.hasMoreElements(); ) {

	    IXMLElement nxt = (IXMLElement) enum.nextElement();
	    String nom = nxt.getName();
	    
	    if( nom.equals(PROPERTY_ADJ) )
		this.adjective = nxt.getContent();
	    else if( nom.equals(PROPERTY_ABBREV) )
		this.abbreviation = Util.fixLength(nxt.getContent(), 5);
	    else if( nom.equals(PROPERTY_SECTIONS) )
		this.sections.refresh(nxt.getContent());
	}

    }
    
    
    public IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }


    public void toXMLElement(IXMLElement xml) {
	//race
	xml.setAttribute(PROPERTY_NAME, this.name);
	
	IXMLElement nxt;
	
	//race.adjective
	nxt = new XMLElement();
	nxt.setName(PROPERTY_ADJ);
	nxt.setContent(adjective);
	xml.addChild(nxt);
	
	//race.abbreviation
	nxt = new XMLElement();
	nxt.setName(PROPERTY_ABBREV);
	nxt.setContent(abbreviation);
	xml.addChild(nxt);
	
	//race.wearlocations
	nxt = new XMLElement();
	nxt.setName(PROPERTY_SECTIONS);
	nxt.setContent(sections.toString());
	xml.addChild(nxt);
    }
    

    public IXMLElement toShortXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toShortXMLElement(xml);
	return xml;
    }


    public void toShortXMLElement(IXMLElement xml) {
	xml.setAttribute(PROPERTY_NAME, this.name);
    }

}
