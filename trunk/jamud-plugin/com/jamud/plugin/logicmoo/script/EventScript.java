package  logicmoo.script;


import logicmoo.Jamud;
import logicmoo.obj.event.*;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public abstract class EventScript implements JamudEventListener {

    public static final String MARKUP = "SCRIPT";

    private static final String PROPERTY_LANGUAGE = "LANGUAGE";


    /** the XML representation of the contained scripts */
    public abstract XMLElement toXMLElement();


    public static final EventScript loadEventScript(JamudEventTrigger tr, XMLElement xml) throws Exception {
	if ( ! xml.getName().equals(MARKUP) )
	    throw new Exception("non-" + MARKUP + " element in EventScript.loadEventScript");

	try {
	    ScriptLanguage sl = Jamud.getScriptLanguage(xml.getAttribute(PROPERTY_LANGUAGE));

	    if ( sl != null )
		return sl.loadEventScript(tr, xml);
	    else
		return null;
	} catch ( Exception e ) {
	    System.err.println("Script not loaded");
	    e.printStackTrace(System.err);
	    return null;
	}
    }

    public abstract EventScript getSemiClone(JamudEventTrigger for_targ);

}