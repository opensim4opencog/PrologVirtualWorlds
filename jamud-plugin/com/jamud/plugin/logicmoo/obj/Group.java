package  logicmoo.obj;


import java.util.*;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public class Group extends ReifiableGroup {

	public static final String MARKUP = "GROUP";


	public Artifact getArtifact(CycFort key) {
		return (Artifact) getReifiable(key);
	}
	
	public Artifact getArtifact(String key) {
	    return (Artifact) getArtifact(CycMoo.nearestFort("MudArtifact", key));
	}


	public Artifact getArtifact(int offset, CycFort key) {
		return (Artifact) getReifiable(offset, key);
	}
	
	public Artifact getArtifact(int offset, String key) {
	    return (Artifact) getArtifact(offset, CycMoo.nearestFort("MudReifiable", key));
	}


	public Artifact fetchArtifact(CycFort key) {
		return (Artifact) fetchReifiable(key);
	}

	public Artifact fetchArtifact(String key) {
	    return (Artifact) fetchArtifact(CycMoo.nearestFort("MudArtifact", key));
	}


	public Artifact fetchArtifact(int offset, CycFort key) {
		return (Artifact) fetchReifiable(offset, key);
	}

	public Artifact fetchArtifact(int offset, String key) {
	    return (Artifact) fetchArtifact(offset, CycMoo.nearestFort("MudArtifact", key));
	}


	public XMLElement toXMLElement() {
		XMLElement xml = new XMLElement();
		xml.setName(MARKUP);

		for(Enumeration enum = this.elements(); enum.hasMoreElements(); )
			xml.addChild( ((Artifact)enum.nextElement()).toXMLElement() );

		return xml;
	}

}
