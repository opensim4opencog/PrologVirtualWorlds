package logicmoo.obj;


import logicmoo.*;
import logicmoo.obj.*;
import logicmoo.util.Util;
import java.util.Enumeration;
import net.n3.nanoxml.*;


public class Job extends CycThing {

	/*
		a job should contain...
		- creation cost
		- inherant skills
		- inherant spells
	*/

	public static final String MARKUP = "JOB";

	private static final String
		PROPERTY_ADJ = "ADJ",
		PROPERTY_ABBREV = "ABBREV";

	public static Job DEFAULT = new Job("Tourist");

	private String adjective,
		abbreviation;

		public String getAdj() { return adjective; }
		public String getAbbreviation() { return abbreviation; }
		private void setAbbreviation( String abbreviation ) {
			this.abbreviation = Util.fixLength( abbreviation, 3 );
		}



	public Job(String name) {
		setName(name.toLowerCase());
		this.adjective = name + "ish";
		setAbbreviation(name);
	}

	public Job() {
		//empty
	}



	public static Job getJob( String rString ) {
		rString = rString.toLowerCase();

		for(Enumeration enum = Jamud.getJobManager().elements(); enum.hasMoreElements(); ) {
			Job r = (Job)enum.nextElement();
			if(r.getName().startsWith(rString))
				return r;
		}

		return new Job(rString);
	}

	
	public static Job loadJob(XMLElement xml) throws Exception {
		if (! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in Job.loadJob");

		Job r = new Job(xml.getAttribute(PROPERTY_NAME));

		for( Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
			XMLElement nxt = (XMLElement) enum.nextElement();
			String nom = nxt.getName();

			if ( nom.equals(PROPERTY_ADJ) )
				r.adjective = nxt.getContent();
			else if ( nom.equals(PROPERTY_ABBREV ))
				r.setAbbreviation(nxt.getContent());
		}

		return r;
	}


	public XMLElement toXMLElement() {
		//job
		XMLElement xml = new XMLElement();
		xml.setName(MARKUP);
		xml.setAttribute(CycThing.PROPERTY_NAME, getName());

		XMLElement nxt;

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

		return xml;
	}

}
