package  logicmoo.obj;


import java.util.*;
import logicmoo.*; 
import logicmoo.util.*;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public class MicrotheoryManager  {

    public static final String MARKUP = "MICROTHEORYMANAGER";

    public static final CycConstant DEFAULT_GEOGRAPHICALREGION_VNUM = CycMoo.makeCycConstantSafe("midga_0001");

    private Hashtable areas = new Hashtable();
    public Microtheory get(CycFort areaPrefix) { return(Microtheory) areas.get(areaPrefix);}
    public void put(Microtheory microtheory) { areas.put(microtheory.getCycFort(), microtheory);}
    public void remove(CycFort areaPrefix) { areas.remove(areaPrefix);}
    public void remove(Microtheory microtheory) { areas.remove(microtheory.getCycFort());}
    public Enumeration elements() { return areas.elements();}
    public static Microtheory masterMicrotheory;


    public MicrotheoryManager(XMLElement xml) throws Exception {
	loadMicrotheories(xml);
	applyExits();
    }



    private void loadMicrotheories(XMLElement xml) throws Exception {
	areas.clear();

    }


    private void applyExits() throws Exception {
	for ( Enumeration enum = elements(); enum.hasMoreElements(); ) {
	    Microtheory microtheory = (Microtheory) enum.nextElement();

	    for ( Enumeration renum = microtheory.getGeographicalRegions().elements(); renum.hasMoreElements(); ) {
		GeographicalRegion rm = (GeographicalRegion) renum.nextElement();

		for ( Enumeration exnum = rm.enumerateExits(); exnum.hasMoreElements(); )
		    applyExit(rm, (Exit) exnum.nextElement());
	    }
	}
    }


    public void applyExit(GeographicalRegion src, Exit ex) throws Exception {
	CycFort t = ex.getTargetCycFort();
	GeographicalRegion T = getGeographicalRegion(src.getMicrotheory(), t);
	if ( T==null )
	    throw new Exception("GeographicalRegion VNUM does not exist: " + t);
	else
	    ex.setTargetGeographicalRegion(T);
    }


    public GeographicalRegion getDefaultGeographicalRegion() {
	return getGeographicalRegion(null, DEFAULT_GEOGRAPHICALREGION_VNUM);
    }

    /*public GeographicalRegion getGeographicalRegion(Microtheory origin, String name) {
	return getGeographicalRegion(origin,CycMoo.makeCycFort(name));
    } */

    public GeographicalRegion getGeographicalRegion(Microtheory origin, CycFort cycFort) {
	if (origin==null) origin=masterMicrotheory;
	return((GeographicalRegion) (origin.getGeographicalRegions().get(cycFort)));
    }

    /*
   public Artifact getArtifact(Microtheory origin, CycFort cycFort) {
   }

   public Mobile getMobile(Microtheory origin, CycFort cycFort) {
   }

   public ForwardRule getForwardRule(Microtheory origin, CycFort cycFort) {
   }
    */


}