package jamud.object;


import jamud.util.Util;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;
import net.n3.nanoxml.*;


public class Job {
    

    private static HashMap jobs = new HashMap();

    public static final Job getJob(String name) {
	if(name != null &&
	   (name = name.trim().toLowerCase()).length() > 0 )
	    return (Job) jobs.get(name);
	else
	    return null;
    } 

    public static final Iterator jobs() {
	return jobs.values().iterator();
    }



    /** <code>MARKUP = "JOB"</code> */    
    public static final String
	MARKUP = "JOB";
    

    private static final String
	PROPERTY_NAME = "NAME",
	PROPERTY_ADJ = "ADJ",
	PROPERTY_ABBREV = "ABBREV";

    
    public static final Job DEFAULT = new Job();

    static {
	DEFAULT.name = "junk";
	DEFAULT.adjective = "junky";
	DEFAULT.abbreviation = "   ";
    }    


    private String
	name,
	adjective,
	abbreviation;


    public String getName() {
	return this.name;
    }
    
    public String getAdjective() {
	return this.adjective;
    }

    public String getAbbreviation() {
	return this.abbreviation;
    }

        
    
    public Job(String name) {
	this.name = name.trim().toLowerCase();
	this.adjective = name.concat("ish");
	this.abbreviation = Util.fixLength(name, 3);
    }
    
    public Job() {
	//empty
    }
    
    
    
    public static final Job loadJob(IXMLElement xml) {
	Job job;

	if( xml.getChildrenCount() == 0 ) {
	    String name = xml.getAttribute(PROPERTY_NAME, "")
		.toLowerCase();
	    job = (Job) jobs.get( name );

	    if(job == null)
		job = DEFAULT;

	} else {

	    job = new Job();
	    job.load(xml);
	    jobs.put( job.name, job );

	}

	return job;
    }


    public void load(IXMLElement xml) {
	
	this.name = xml.getAttribute(PROPERTY_NAME, "").toLowerCase();

	for(Enumeration enum = xml.enumerateChildren();
	enum.hasMoreElements(); ) {

	    IXMLElement nxt = (IXMLElement) enum.nextElement();
	    String nom = nxt.getName();
	    
	    if( nom.equals(PROPERTY_ADJ) )
		this.adjective = nxt.getContent();
	    else if( nom.equals(PROPERTY_ABBREV ))
		this.abbreviation = Util.fixLength(nxt.getContent(), 3);
	}

    }


    public IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }
    
    
    public void toXMLElement(IXMLElement xml) {
	//job
	xml.setAttribute(PROPERTY_NAME, this.name);
	
	IXMLElement nxt;
	
	//job.adj
	nxt = new XMLElement();
	nxt.setName(PROPERTY_ADJ);
	nxt.setContent(adjective);
	xml.addChild(nxt);
	
	//job.abbrev
	nxt = new XMLElement();
	nxt.setName(PROPERTY_ABBREV);
	nxt.setContent(abbreviation);
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
