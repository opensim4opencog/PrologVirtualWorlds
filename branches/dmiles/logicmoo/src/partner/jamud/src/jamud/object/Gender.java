package jamud.object;


import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;
import net.n3.nanoxml.*;


/**
 * @author Nolan, Siege
 */
public class Gender {
    

    private static HashMap genders = new HashMap();

    public static final Gender getGender(String name) {
	if(name != null &&
	   (name = name.trim().toLowerCase()).length() > 0 )
	    return (Gender) genders.get(name);
	else
	    return null;
    }

    public static final Iterator genders() {
	return genders.values().iterator();
    }



    /** <code>MARKUP = "GENDER"</code> */
    public static final String
	MARKUP = "GENDER";
    

    private static final String
	PROPERTY_NAME = "NAME",
	PROPERTY_INITIAL = "INITIAL",
	PROPERTY_NOM = "NOM",
	PROPERTY_POSS = "POSS",
	PROPERTY_PRON = "PRON";
    

    public static final Gender
	DEFAULT = new Gender();

    static{
	DEFAULT.name = "thing";
	DEFAULT.initial = " ";
	DEFAULT.pron = "it";
	DEFAULT.nom = "it";
	DEFAULT.poss = "its";
    }

    
    private String
	name,
	initial,
	pron,
	nom,
	poss;
    

    public String getName() {
	return this.name;
    }

    public String getInitial() {
	return this.initial;
    }

    public String getNom() {
	return this.nom;
    }

    public String getPoss() {
	return this.poss;
    }

    public String getPron() {
	return this.pron;
    }
    
    
    
    public Gender(String name) {
	this.name = name.trim().toLowerCase();
    }

    public Gender() {
	// sploot
    }

    
    
    public static Gender loadGender(IXMLElement xml)  {
	Gender gender;

	if( xml.getChildrenCount() == 0 ) {
	    String name = xml.getAttribute(PROPERTY_NAME, "").toLowerCase();
	    gender = (Gender) genders.get( name );

	    if(gender == null) {
		gender = DEFAULT;
	    }

	} else {

	    gender = new Gender();
	    gender.load(xml);
	    genders.put( gender.name, gender );

	}

	return gender;
    }
	

    public void load(IXMLElement xml) {
	
	this.name = xml.getAttribute(PROPERTY_NAME, "").toLowerCase();

	for(Enumeration enum = xml.enumerateChildren();
	    enum.hasMoreElements(); ) {

	    IXMLElement nxt = (IXMLElement) enum.nextElement();
	    String nom = nxt.getName();
	    
	    if( nom.equals(PROPERTY_INITIAL) )
		this.initial = nxt.getContent();
	    else if( nom.equals(PROPERTY_PRON) )
		this.pron = nxt.getContent();
	    else if( nom.equals(PROPERTY_NOM) )
		this.nom = nxt.getContent();
	    else if( nom.equals(PROPERTY_POSS) )
		this.poss = nxt.getContent();
	}
	
    }


    public IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }
    
    
    public void toXMLElement(IXMLElement xml) {
	//gender
	xml.setAttribute(PROPERTY_NAME, getName());
	
	IXMLElement nxt;
	
	//gender.initial
	nxt = new XMLElement();
	nxt.setName(PROPERTY_INITIAL);
	nxt.setContent(initial);
	xml.addChild(nxt);
	
	//gender.pron
	nxt = new XMLElement();
	nxt.setName(PROPERTY_PRON);
	nxt.setContent(pron);
	xml.addChild(nxt);
	
	//gender.nom
	nxt = new XMLElement();
	nxt.setName(PROPERTY_NOM);
	nxt.setContent(nom);
	xml.addChild(nxt);
	
	//gender.poss
	nxt = new XMLElement();
	nxt.setName(PROPERTY_POSS);
	nxt.setContent(poss);
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
