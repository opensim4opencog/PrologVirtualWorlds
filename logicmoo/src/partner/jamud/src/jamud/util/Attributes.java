package jamud.util;


import java.io.*;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Properties;
import net.n3.nanoxml.*;


/** Similar to java.util.Properties with some overloaded methods */
public class Attributes extends Properties {


    public static final String
	MARKUP = "ATTRIBUTES";


    public Attributes() {
	super();
    }

    public Attributes(Hashtable defaults) {
	for(Enumeration enum = defaults.keys(); enum.hasMoreElements(); ) {
	    Object key = enum.nextElement();
	    this.put( key, defaults.get(key) );
	}
    }



    public synchronized String getAttribute(String key, String defaultValue) {
	Object o = this.get(key);
	if(o != null) {
	    if(o instanceof String) {
		return (String) o;
	    } else {
		return o.toString();
	    }
	} else {
	    return defaultValue;
	}
    }


    public synchronized int getAttribute(String key, int defaultValue) {
	Object o = this.get(key);
	if(o != null) {
	    if(o instanceof Number) {
		return ((Number)o).intValue();
	    } else {
		Integer n = new Integer( (String)o );
		return n.intValue();
	    }
	} else {
	    return defaultValue;
	}
    }


    public synchronized long getAttribute(String key, long defaultValue) {
	Object o = this.get(key);
	if(o != null) {
	    if(o instanceof Number) {
		return ((Number)o).longValue();
	    } else {
		Long n = new Long( (String)o );
		return n.longValue();
	    }
	} else {
	    return defaultValue;
	}
    }


    public synchronized double getAttribute(String key, double defaultValue) {
	Object o = this.get(key);
	if(o != null) {
	    if(o instanceof Number) {
		return ((Number)o).doubleValue();
	    } else {
		Double n = new Double( (String)o );
		return n.doubleValue();
	    }
	} else {
	    return defaultValue;
	}
    }


    public synchronized float getAttribute(String key, float defaultValue) {
	Object o = this.get(key);
	if(o != null) {
	    if(o instanceof Number) {
		return ((Number)o).floatValue();
	    } else {
		Float n = new Float( (String)o );
		return n.floatValue();
	    }
	} else {
	    return defaultValue;
	}
    }


    /** if the key links to "true" then this method returns true.
     * all other values are assumed to indicate false. If the
     * attribute is undefined, then the defaultValue is returned.
     */
    public synchronized boolean getAttribute(String key,
					     boolean defaultValue) {
	return getAttribute(key, "true", defaultValue);
    }


    /** if the key links to a String which matches the trueValue,
     * then this method returns true. All other String values are
     * assumed to indicate flase. If a non-String value was previously
     * set to this key, then true is returned if the integer value of
     * that number is greater than 0. If the attribute is undefined,
     * then the defaultValue is returned.
     */
    public synchronized boolean getAttribute(String key, String trueValue,
					     boolean defaultValue) {
        Object o = this.get(key);
	if(o != null) {
	    if(o instanceof Boolean) {
		return ((Boolean)o).booleanValue();
	    } else if(o instanceof Number) {
		return ( ((Number)o).intValue() > 0 );
	    } else {
		return ((String)o).equalsIgnoreCase(trueValue);
	    }
	} else {
	    return defaultValue;
	}
    }



    public synchronized void setAttribute(String key, String value) {
	this.put(key, value);
    }


    public synchronized void setAttribute(String key, int value) {
	this.put(key, new Integer(value));
    }


    public synchronized void setAttribute(String key, long value) {
	this.put(key, new Long(value));
    }


    public synchronized void setAttribute(String key, double value) {
	this.put(key, new Double(value));
    }


    public synchronized void setAttribute(String key, float value) {
	this.put(key, new Float(value));
    }


    public synchronized void setAttribute(String key, boolean value) {
	this.put(key, new Boolean(value));
    }



    /* --------------------------------------------------------------------
     * All the following had to be overridden because of my optimizations
     * above. The major change is to allow non-String objects to be written
     * out without having the world explode.
     * ----------------------------------------------------------------- */

    private String saveConvert(String theString, boolean escapeSpace) {
        int len = theString.length();
        StringBuffer outBuffer = new StringBuffer(len*2);

        for(int x=0; x<len; x++) {
            char aChar = theString.charAt(x);
            switch(aChar) {
	    case ' ':
		if (x == 0 || escapeSpace) 
		    outBuffer.append('\\');
		
		outBuffer.append(' ');
		break;
	    case '\\':outBuffer.append('\\'); outBuffer.append('\\');
		break;
	    case '\t':outBuffer.append('\\'); outBuffer.append('t');
		break;
	    case '\n':outBuffer.append('\\'); outBuffer.append('n');
		break;
	    case '\r':outBuffer.append('\\'); outBuffer.append('r');
		break;
	    case '\f':outBuffer.append('\\'); outBuffer.append('f');
		break;
	    default:
		/*
		if ((aChar < 0x0020) || (aChar > 0x007e)) {
		    outBuffer.append('\\');
		    outBuffer.append('u');
		    outBuffer.append(toHex((aChar >> 12) & 0xF));
		    outBuffer.append(toHex((aChar >>  8) & 0xF));
		    outBuffer.append(toHex((aChar >>  4) & 0xF));
		    outBuffer.append(toHex( aChar        & 0xF));
		}
		else {
		    if (specialSaveChars.indexOf(aChar) != -1)
			outBuffer.append('\\');
		    outBuffer.append(aChar);
		}
		*/
            }
        }
        return outBuffer.toString();
    }


    public synchronized void store(OutputStream out,
				   String header) throws IOException {
        BufferedWriter awriter = new 
	    BufferedWriter(new OutputStreamWriter(out, "8859_1"));

        for (Enumeration enum = this.keys(); enum.hasMoreElements(); ) {
            String key = (String) enum.nextElement();
            Object val = this.get(key);

	    String strVal;
	    if(val instanceof String) {
		strVal = (String) val;
	    } else if(val instanceof Number) {
		strVal = val.toString();
	    } else {
		strVal = val.toString();
	    }

	    key = saveConvert(key, true);	    
            strVal = saveConvert(strVal, false);

            writeln( awriter, key.concat("=").concat(strVal) );
        }
        awriter.flush();
    }


    private static void writeln(BufferedWriter bw,
				String s) throws IOException {
        bw.write(s);
        bw.newLine();
    }


    public void list(PrintStream out) {
	out.println("-- listing properties --");
	for (Enumeration e = keys() ; e.hasMoreElements() ;) {
	    String key = (String)e.nextElement();
	    Object val = get(key);

	    String strVal;
	    if(val instanceof String) {
		strVal = (String) val;
	    } else if(val instanceof Number) {
		strVal = val.toString();
	    } else {
		strVal = val.toString();	    
	    }

	    if (strVal.length() > 40) {
                strVal = strVal.substring(0, 37) + "...";
	    }

	    out.println( key.concat("=").concat(strVal) );
	}
    }


    public void list(PrintWriter out) {
	out.println("-- listing properties --");
	for (Enumeration e = keys() ; e.hasMoreElements() ;) {
	    String key = (String)e.nextElement();
	    Object val = get(key);

	    String strVal;
	    if(val instanceof String) {
		strVal = (String) val;
	    } else if(val instanceof Number) {
		strVal = val.toString();
	    } else {
		strVal = val.toString();	    
	    }

	    if (strVal.length() > 40) {
                strVal = strVal.substring(0, 37) + "...";
	    }

	    out.println( key.concat("=").concat(strVal) );
	}
    }


    public synchronized IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }


    private static final String
	SET = "SET",
	KEY = "KEY",
	TYPE = "TYPE",
	VAL = "VALUE";

    
    public synchronized void toXMLElement(IXMLElement xml) {
	for(Enumeration enum = keys(); enum.hasMoreElements(); ) {
	    String key = (String) enum.nextElement();
	    Object val = get(key);

	    IXMLElement nxt = new XMLElement(SET);
	    nxt.setAttribute(KEY, key);
	    
	    if(val instanceof String) {

		nxt.setContent( (String)val );

	    } else {

		final String p_type = TYPE;

		if(val instanceof Integer) {
		    nxt.setAttribute(p_type, "int");
		} else if(val instanceof Long) {
		    nxt.setAttribute(p_type, "long");
		} else if(val instanceof Float) {
		    nxt.setAttribute(p_type, "float");
		} else if(val instanceof Double) {
		    nxt.setAttribute(p_type, "double");
		} else if(val instanceof Boolean) {
		    nxt.setAttribute(p_type, "boolean");
		} else {
		    continue;
		}
		
		nxt.setAttribute(VAL, val.toString());
	    }

	    xml.addChild(nxt);
	}
    }


    public void load(IXMLElement xml) {
	for(Enumeration enum = xml.getChildrenNamed(SET).elements();
	    enum.hasMoreElements(); ) {
	    IXMLElement nxt = (IXMLElement) enum.nextElement();

	    String key = nxt.getAttribute(KEY, null);
	    if( key == null ) {
		continue;
	    }

	    String type = nxt.getAttribute(TYPE, null);
	    String val;
	    if( type == null ) {
		val = nxt.getAttribute(VAL, null);
		if(val == null) {
		    val = nxt.getContent();
		}
		put(key, val);

	    } else {

		val = nxt.getAttribute(VAL, "0");

		if( type.equalsIgnoreCase("int") ) {
		    Integer v = new Integer(val);
		    put(key, v);
		} else if( type.equalsIgnoreCase("long") ) {
		    Long v = new Long(val);
		    put(key, v);
		} else if( type.equalsIgnoreCase("float") ) {
		    Float v = new Float(val);
		    put(key, v);
		} else if( type.equalsIgnoreCase("double") ) {
		    Double v = new Double(val);
		    put(key, v);
		} else if( type.equalsIgnoreCase("boolean") ) {
		    Boolean v = new Boolean(val);
		    put(key, v);
		} else {
		    continue;
		}		
	    }
	}
    }


    public static final Attributes loadAttributes(IXMLElement xml) {
	Attributes a = new Attributes();
	a.load(xml);
	return a;
    }

}
