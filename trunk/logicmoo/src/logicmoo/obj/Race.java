package logicmoo.obj;


import logicmoo.*;
import logicmoo.obj.*;
import logicmoo.util.*;
import java.util.Enumeration;
import net.n3.nanoxml.*;


public class Race extends CycThing {

	/*
		a race should contain...
		- creation cost
		- base language
		- inherant skills
		- inherant spells
		- inherant flags (resists, vuln, affects, etc)
		- keywords
		- bodyparts
		- base stats
	*/

	public static final String MARKUP = "RACE";

	private static final String
		PROPERTY_NAME = "NAME",
		PROPERTY_ADJ = "ADJ",
		PROPERTY_ABBREV = "ABBREV",
		PROPERTY_WEARLOCATIONS = "WEARLOCATIONS";

	public static Race DEFAULT = new Race("Unknown");

	private FlagList bodyparts = new FlagList();
		public FlagList getWearLocations() { return bodyparts; }

	private String adjective,
		abbreviation;

		public String getAdj() { return adjective; }
		public String getAbbreviation() { return abbreviation; }
		private void setAbbreviation( String abbreviation ) {
			this.abbreviation = Util.fixLength( abbreviation, 5 );
		}



	public Race(String name) {
		setName(name.toLowerCase());
		this.adjective = name + "ish";
		setAbbreviation(name);
	}

	public Race() {
		//empty
	}



	public static Race getRace( String rString ) {
		rString = rString.toLowerCase();
		for(Enumeration enum = Jamud.getRaceManager().elements(); enum.hasMoreElements(); ) {
			Race r = (Race)enum.nextElement();
			if( r.getName().startsWith(rString) )
				return r;
		}
		return new Race(rString);
	}


	public static Race loadRace(XMLElement xml) throws Exception {
		if( !xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in Race.loadRace");

		Race r = new Race(xml.getAttribute(CycThing.PROPERTY_NAME));

		for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
			XMLElement nxt = (XMLElement) enum.nextElement();
			String nom = nxt.getName(); nxt.getContent();

			if( nom.equals(PROPERTY_ADJ) )
				r.adjective = nxt.getContent();
			else if( nom.equals(PROPERTY_ABBREV) )
				r.setAbbreviation(nxt.getContent());
			else if( nom.equals(PROPERTY_WEARLOCATIONS) )
				r.bodyparts = new FlagList(nxt.getContent());
		}

		return r;
	}


	public XMLElement toXMLElement() {
		//race
		XMLElement xml = new XMLElement();
		xml.setName(MARKUP);
		xml.setAttribute(CycThing.PROPERTY_NAME, getName());

		XMLElement nxt;

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
		nxt.setName(PROPERTY_WEARLOCATIONS);
		nxt.setContent(bodyparts.toString());
		xml.addChild(nxt);

		return xml;
	}

}
