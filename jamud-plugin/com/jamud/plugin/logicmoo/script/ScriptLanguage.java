package  logicmoo.script;

import logicmoo.obj.event.JamudEventTrigger;
import logicmoo.util.*;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;

public abstract class ScriptLanguage {

	public static final String MARKUP = "SCRIPTMOD";

	private static final String
		PROPERTY_JAR = "JAR",
		PROPERTY_CLASS = "CLASS";

	//assorted information
	public abstract String getName();
	public abstract String getVersion();
	public abstract String getAuthor();
	public abstract String getInfo();


	/** loads a scripted event from a passed SCRIPT tag onto a particular JamudEventTrigger
	*/
	public abstract EventScript loadEventScript(JamudEventTrigger trg, XMLElement xml) throws Exception;


	public static final ScriptLanguage loadScriptLanguage(XMLElement xml) throws Exception {
		if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in ScriptLanguage.loadScriptLanguage");

		String jar_file = xml.getAttribute(PROPERTY_JAR, null),
			class_name = xml.getAttribute(PROPERTY_CLASS, null);

		xml = null;

		Class pc;
		if(jar_file != null)
			pc = (new JarClassLoader(jar_file)).loadClass(class_name, true);
		else
			pc = Class.forName(class_name);

		return (ScriptLanguage) pc.newInstance();
	}
}