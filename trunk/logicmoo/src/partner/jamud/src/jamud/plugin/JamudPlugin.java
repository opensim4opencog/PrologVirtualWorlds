package jamud.plugin;


import jamud.Initializable;
import jamud.util.*;
import java.util.*;
import net.n3.nanoxml.*;


public abstract class JamudPlugin implements Initializable {
    

    public static final String
	MARKUP = "PLUGIN";
    

    private static final String
	PROPERTY_AUTOMATIC = "AUTO",
	PROPERTY_CLASS = "CLASS",
	PROPERTY_JAR = "JAR",
	PROPERTY_PARAM = "PARAM",
	PROPERTY_PARAM_NAME = "NAME",
	PROPERTY_PARAM_VALUE = "VALUE";


    //The title of the Plugin
    public abstract String getName();


    //The version of the Plugin
    public abstract String getVersion();


    //The author(s) of the Plugin
    public abstract String getAuthor();


    //Additional info for the Plugin
    public abstract String getInfo();
    
    
    //Is it active?
    public abstract boolean isActive();

    
    //Start automatically when the mud initializes?
    private boolean autoStart = false;

    public boolean isAutomatic() {
	return this.autoStart;
    }

    public void setAutomatic(boolean autoStart) {
	this.autoStart = autoStart;
    }
    

    //Initialization Variables
    private Attributes ini;

    public Attributes parameters() {
	return this.ini;
    }
    
    
    
    // Default Constructor
    public JamudPlugin() {
	this.ini = new Attributes();
    }
    
    

    // favourite way to load a plugin
    public static final JamudPlugin loadJamudPlugin(IXMLElement xml)
	throws Exception {

	// find the plugin's class
	String jar_file = xml.getAttribute(PROPERTY_JAR, null),
	    class_name = xml.getAttribute(PROPERTY_CLASS, null);
	
	Class pc;

	if(jar_file != null) {
	    pc = (new JarClassLoader(jar_file)).loadClass(class_name, true);
	} else {
	    pc = Class.forName(class_name);
	}
	
	return loadJamudPlugin(xml, pc);
    }


    // another way to load a plugin
    public static final JamudPlugin loadJamudPlugin(IXMLElement xml,
	Class base) throws Exception {
	
	Object o = base.newInstance();
	
	if( !(o instanceof JamudPlugin) ) {
	    throw new Exception( "class ".concat( base.getName() )
				 .concat(" not an instance of ")
				 .concat("jamud.plugin.JamudPlugin") );
	} else {
	    JamudPlugin plug = (JamudPlugin) o;
	    plug.load(xml);
	    return plug;
	}
    }


    // put the params in the plugin. May be overridden for alternative use.
    public void load(IXMLElement xml) throws Exception {

	// load the SET tags as an Attributes set
	this.ini.load( xml );
	
	// autoStart
	Attributes at = new Attributes(xml.getAttributes());
	this.autoStart = at.getAttribute(PROPERTY_AUTOMATIC, false);
    }
    
    
    public final boolean equals(Object o) {
	return ( this == o );
    }
    
}
