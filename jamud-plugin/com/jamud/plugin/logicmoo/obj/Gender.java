package logicmoo.obj;


import logicmoo.*;
import logicmoo.obj.*;
import logicmoo.util.Util;
import java.util.Enumeration;
import net.n3.nanoxml.*;


/**
 * @author Nolan, Siege
 */
public class Gender extends CycThing {

	public static final String MARKUP = "GENDER";

	private static final String
		PROPERTY_INITIAL = "INITIAL",
		PROPERTY_NOM = "NOM",
		PROPERTY_POSS = "POSS",
		PROPERTY_PRON = "PRON";

	public static Gender DEFAULT = new Gender();
	
	private String initial,
		pron,
		nom,
		poss;
   
   	public String getInitial() { return initial; }
		public String getNom() { return nom; }
		public String getPoss() { return poss; }
		public String getPron() { return pron; }



	public Gender(String name) {
		this();
		this.setName(name.toLowerCase());
	}

	public Gender() {
		// reset to "default" gender
		setName("unknown");
		initial = "u";
		pron = "they";
		nom = "them";
		poss = "their";
	}



	public static Gender getGender(String gString) {
		gString = gString.toLowerCase();

		for(Enumeration enum = Jamud.getGenderManager().elements(); enum.hasMoreElements(); ) {
			Gender g = (Gender) enum.nextElement();
			if( g.getName().startsWith(gString) )
				return g;
		}

		return new Gender(gString);
	}
   

	public static Gender loadGender(XMLElement xml) throws Exception {
		if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in Gender.loadGender");

		Gender g = new Gender(xml.getAttribute(CycThing.PROPERTY_NAME));

		for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
			XMLElement nxt = (XMLElement) enum.nextElement();
			String nom = nxt.getName();

			if( nom.equals(PROPERTY_INITIAL) )
				g.initial = nxt.getContent();
			else if( nom.equals(PROPERTY_PRON) )
				g.pron = nxt.getContent();
			else if( nom.equals(PROPERTY_NOM) )
				g.nom = nxt.getContent();
			else if( nom.equals(PROPERTY_POSS) )
				g.poss = nxt.getContent();
		}

		return g;
	}


	public XMLElement toXMLElement() {
		//gender
		XMLElement xml = new XMLElement();
		xml.setName(MARKUP);
		xml.setAttribute(CycThing.PROPERTY_NAME, getName());

		XMLElement nxt;

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

		return xml;
	}

}
