package  logicmoo.plugin;


import logicmoo.util.*;
import java.util.*;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public abstract class JamudPlugin {

	public static final String MARKUP = "PLUGIN";

	private static final String
		PROPERTY_AUTOMATIC = "AUTO",
		PROPERTY_CLASS = "CLASS",
		PROPERTY_JAR = "JAR",
		PROPERTY_PARAM = "PARAM",
		PROPERTY_PARAM_NAME = "NAME",
		PROPERTY_PARAM_VALUE = "VALUE";

	public static final int
		STATE_INITIALIZED = 0,
		STATE_INITIALIZING = 1,
		STATE_TERMINATED = 2,
		STATE_TERMINATING = 3;

	public abstract int getPluginState();		//what state is the plugin in

	public abstract String getName();			//The title of the Plugin
	public abstract String getVersion();		//The version of the Plugin
	public abstract String getAuthor();			//The author(s) of the Plugin
	public abstract String getInfo();			//Additional info for the Plugin

	public abstract void initialize() throws Exception;	//Start the Plugin up
	public abstract void terminate() throws Exception;		//Stop the Plugin

	public abstract boolean isActive();			//Is it active?

	private boolean autoStart = false;
		public boolean isAutomatic() { return autoStart; }

	private Hashtable ini = new Hashtable();
		public String getParam(String key) { return (String) ini.get(key); }



	public JamudPlugin(Hashtable ini) {
		this.ini = ini;
	}



	public static final JamudPlugin loadJamudPlugin(XMLElement xml) throws Exception {
		if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in JamudPlugin.loadJamudPlugin");

		// load params
		Hashtable ini = new Hashtable();
		for(Enumeration enum = xml.getChildrenNamed(PROPERTY_PARAM).elements(); enum.hasMoreElements(); ) {
			XMLElement nxt = (XMLElement) enum.nextElement();
			String a = nxt.getAttribute(PROPERTY_PARAM_NAME, null),
				b = nxt.getAttribute(PROPERTY_PARAM_VALUE, null);
			if(a != null && b != null)
				ini.put(a, b);
		}		

		// find the plugin's class
		String jar_file = xml.getAttribute(PROPERTY_JAR, null),
			class_name = xml.getAttribute(PROPERTY_CLASS, null);

		Class pc;
		if(jar_file != null)
			pc = (new JarClassLoader(jar_file)).loadClass(class_name, true);
		else
			pc = Class.forName(class_name);

		// cleanup- no longer needed
		jar_file = null;
		class_name = null;

		// instantiate the class
		Class[] o1 = { ini.getClass() };
		Object[] o2 = { ini };
		JamudPlugin plug = (JamudPlugin) pc.getConstructor(o1).newInstance(o2);

		// cleanup- no longer needed
		o1 = null;
		o2 = null;
		pc = null;

	       // plug.autoStart = xml.getAttribute(PROPERTY_AUTOMATIC, "true", "false", false);

		plug.autoStart = true;

		return plug;
	}


	public boolean equals(Object o) {
		return ( (Object)this == o );
	}

}
