package logicmoo.util;


import java.util.Hashtable;


/**
 * Class containing static values and methods related to translating
 * Jamud colour markup to ANSI colour markup.
 */
public abstract class Colour {

	// prefix to various ANSI markups
	private static final String ESC = (char)27 + "[";

	/** ANSI clear-screen */
	public static final String CLS = (ESC + (char)72) + (ESC + (char)74);

	/** Table of Jamud markups to their ANSI equivalents */
	public static final Hashtable COLOUR_TABLE = new Hashtable();

	//init values into COLOUR_TABLE
	static {
		COLOUR_TABLE.put( "{0", ESC+"0m" );			// reset
		COLOUR_TABLE.put( "{1", ESC+"0;31m" );		// fg red
		COLOUR_TABLE.put( "{2", ESC+"0;32m" );		// fg green
		COLOUR_TABLE.put( "{3", ESC+"0;33m" );		// fg yellow
		COLOUR_TABLE.put( "{4", ESC+"0;34m" );		// fg blue
		COLOUR_TABLE.put( "{5", ESC+"0;35m" );		// fg magenta
		COLOUR_TABLE.put( "{6", ESC+"0;36m" );		// fg cyan
		COLOUR_TABLE.put( "{7", ESC+"0;37m" );		// fg white
		COLOUR_TABLE.put( "{8", ESC+"0;30m" );		// fg black
		COLOUR_TABLE.put( "{!", ESC+"1;31m" );		// fg bright red
		COLOUR_TABLE.put( "{@", ESC+"1;32m" );		// fg bright green
		COLOUR_TABLE.put( "{#", ESC+"1;33m" );		// fg bright yellow
		COLOUR_TABLE.put( "{$", ESC+"1;34m" );		// fg bright blue
		COLOUR_TABLE.put( "{%", ESC+"1;35m" );		// fg bright magenta
		COLOUR_TABLE.put( "{^", ESC+"1;36m" );		// fg bright cyan
		COLOUR_TABLE.put( "{&", ESC+"1;37m" );		// fg bright white
		COLOUR_TABLE.put( "{*", ESC+"1;30m" );		// fg bright grey
		COLOUR_TABLE.put( "}1", ESC+"0;41m" );		// bg red
		COLOUR_TABLE.put( "}2", ESC+"0;42m" );		// bg green
		COLOUR_TABLE.put( "}3", ESC+"0;43m" );		// bg yellow
		COLOUR_TABLE.put( "}4", ESC+"0;44m" );		// bg blue
		COLOUR_TABLE.put( "}5", ESC+"0;45m" );		// bg magenta
		COLOUR_TABLE.put( "}6", ESC+"0;46m" );		// bg cyan
		COLOUR_TABLE.put( "}7", ESC+"0;47m" );		// bg white
		COLOUR_TABLE.put( "}8", ESC+"0;40m" );		// bg black
		COLOUR_TABLE.put( "}!", ESC+"1;41m" );		// bg bright red
		COLOUR_TABLE.put( "}@", ESC+"1;42m" );		// bg bright green
		COLOUR_TABLE.put( "}#", ESC+"1;43m" );		// bg bright yellow
		COLOUR_TABLE.put( "}$", ESC+"1;44m" );		// bg bright blue
		COLOUR_TABLE.put( "}%", ESC+"1;45m" );		// bg bright magenta
		COLOUR_TABLE.put( "}^", ESC+"1;46m" );		// bg bright cyan
		COLOUR_TABLE.put( "}&", ESC+"1;47m" );		// bg bright white
		COLOUR_TABLE.put( "}*", ESC+"1;40m" );		// bg bright grey
		COLOUR_TABLE.put( "{{", "{" );				// escaped {
		COLOUR_TABLE.put( "}}",	"}" );				// escaped }
		COLOUR_TABLE.put( "{-",	""+(char)255+(char)251+(char)1 );	// echo off
		COLOUR_TABLE.put( "{+",	""+(char)255+(char)252+(char)1 );	// echo on
	}


	/**
	 * converts Jamud markups to ANSI markups or removes Jamud markups
	 * @param text byte array to be converted
	 * @param colourize if true then Jamud markups are converted to ANSI<br>
	 * if false then Jamud markups are removed.
	 */
	public static byte[] convert(byte[] text, boolean colourize) {
		String J = new String(text);
		return convert(J, colourize).getBytes();
	}
	
	/**
	 * converts Jamud markups to ANSI markups or removes Jamud markups
	 * @param text String to be converted
	 * @param colourize if true then Jamud markups are converted to ANSI<br>
	 * if false then Jamud markups are removed.
	 */
	public static String convert(String text, boolean colourize) {
		String out = "";
		byte[] t = text.getBytes();
		text = null;

		for(int I=0; I<t.length; I++) {
			char c = (char)t[I];
			if( c == '{'  || c == '}' ) {
				String m = "" + c;
				if(I < t.length -1) {
					m += (char)t[++I];
					String n = (String) COLOUR_TABLE.get(m);
					if(n != null )
						if(colourize || n.length() == 1)
							m = n;
						else
							m = "";
				}
				out += m;
			}
			else
				out += c;
		}

		return out;
	}


}
