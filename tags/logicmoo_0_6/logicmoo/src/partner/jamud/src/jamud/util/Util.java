package jamud.util;


import java.io.ByteArrayOutputStream;
import java.util.*;


/**
 * Contains a slew of general-purpose static methods
 *
 * @author Siege
 */
public abstract class Util {
    
    
    /**
     * Replaces all instances of String original with
     * String replacement found in String source.
     *
     * @param source String source
     * @param original Text you want replaced
     * @param replacement Text to replace original
     * @return source with all instances of original changed to
     * replacement
     */
    public static final String replace(String source, String original,
				 String replacement) {

	if(source == null || original == null) {
	    return null;
	}
	
	if(replacement == null) {
	    replacement = "";
	}

	return new String( replace(source.getBytes(), original.getBytes(),
				   replacement.getBytes()) );
    }


    public static final byte[] replace(byte[] source, byte[] original,
				 byte[] replacement) {

	byte[] ret = new byte[source.length];
	
	//source counter
	int I = 0;

	//ret counter
	int J = 0;

	//change in size per replacement
	int d = replacement.length - original.length;

	for( ; I < source.length; I++ ) {

	    //if the current byte is the same as the first byte of original
	    if(source[I] == original[0]) {
		boolean f = true;
		
		//check if all of original follows
		for(int i = 1, II = I+1 ; i < original.length; ) {
		    if(source[II++] != original[i++]) {
			f = false;
			break;
		    }
		}

		//if so, then use replacement instead of original
		if(f) {

		    //if there's a change in size, then resize the array
		    if(d != 0) {
			byte[] t = new byte[ret.length + d];
			System.arraycopy(ret, 0, t, 0,
					 ((d < 0) ? t.length : ret.length));
			ret = t;
		    }

		    //copy over the replacement bytes
		    for(int i = 0; i < replacement.length; i++) {
			ret[J++] = replacement[i];
		    }

		    //update our placement in the source
		    I += original.length - 1;

		} else {
		    ret[J++] = source[I];

		}

	    } else {
		ret[J++] = source[I];

	    }
	}

	//see, that wasn't so hard.
	return ret;
    }


    public static final String concatStrings(String[] strings) {
	return concatStrings(strings, " ");
    }



    public static final String concatStrings(String[] strings, String spacer) {
	if(spacer == null) spacer = " ";

	int len = 0;
	for(int I=0; I<strings.length; I++) {
	    len += strings[I].length();
	}

	if(strings.length > 1) {
	    len += spacer.length() * strings.length;
	}
	
	byte[] ret = new byte[len];
	byte[] sp = spacer.getBytes();
	spacer = null;

	for(int I=0, J=0; I<strings.length; I++) {
	    byte[] st = strings[I].getBytes();
	    strings[I] = null;
	    System.arraycopy(st, 0, ret, J, st.length);
	    J += st.length;
	    System.arraycopy(sp, 0, ret, J, sp.length);
	    J += sp.length;
	}

	return new String(ret);
    }
    

    
    /**
     * Concatenates text as a new line at end of existing text
     *
     * @param all Text to add a line to
     * @param add_text Line of text to add
     * @return Text, a newLine, and add_Text
     */
    public static final String addLine(String all, String add_text) {
	return all + "\n" + add_text;
    }
   
   

    /**
     * Removes last line in a multi-line String
     *
     * @param all Text to remove last line from
     * @return all with the last line cut off
     */   
    public static final String remLine(String all) {
	return all.substring(0, all.lastIndexOf("\n"));
    }
   


    /**
     * Parses the backspace characters (char 8) and removes that
     * char, and the last non-bs char before this one. In other words
     * it applies the backspaces.
     *
     * @param text Text to apply backspaces to
     * @return Text without backspaces
     */
    public static final String cleanBackSpace(String text) {
	return new String( cleanBackSpace( text.getBytes() ) );
    }


    public static final byte[] cleanBackSpace(byte[] text) {
	int C = 0, I = 0;
	final byte B = (byte)(char) 8;

	for( ; I < text.length; I++) {
	    if( text[I] == B ) {
		if( C > 0 ) {
		    C--;
		}
	    } else {
		text[C++] = text[I];
	    }
	}

	if(C < I) {
	    byte[] b = new byte[C];
	    System.arraycopy(text, 0, b, 0, C);
	    return b;
	} else {
	    return text;
	}
    }


    /** Converts \n to \n\r without duplicating \r's */
    public static final byte[] convertCrLf(byte[] text) {

	ByteArrayOutputStream bo = new ByteArrayOutputStream();

	final byte n = (byte)'\n', r = (byte)'\r';

	for(int I = 0; I < text.length; I++ ) {
	    if( text[I] == n ) {
		// converts \n into \n\r without duplicating extra \r's
		bo.write( text[I] );
		final int y = I+1;
		if( y < text.length && text[y] == r ) {
		    ++I;
		}
		bo.write( r );

	    } else {
		bo.write( text[I] );
	    }
	}

	return bo.toByteArray();
    }
   

    /**
     * Removes unwanted characters from strings (ie illegal
     * control characters or non-standard chars)
     *
     * @param text String to clean Escape characters from
     * @return cleaned version of text
     */
    public static final String cleanup(String text) {
	if(text == null || text.length() == 0) return "";
	String K = "", J = cleanBackSpace(text);
	for(int I=0; I<J.length(); I++) {
	    char X = J.charAt(I);
	    //all the legal chars just happen to fall between 31 and 127
	    K = K + ( ((int)X > 31 && (int)X < 127) ? ("" + X) : ("") );
	}     
	return K;
    }



    /**
     * Verifies that a String contains only certain characters
     *
     * @param src String to verify
     * @param key String containing all allowed characters
     * @return boolan true if src contains only characters in key
     */
    public static final boolean verifyString(String src, String key) {
	if(src == null || key == null)
	    return false;
	
	for(int I=0; I<src.length(); I++)
	    if(key.indexOf(src.charAt(I)) == -1)
		return false;
	
	return true;
    }



    /**
     * Applies word-wrapping to text by adding a line-break 
     * after a defined number of characters (but not breaking 
     * words)
     *
     * @param text String to format
     * @param width integer indicating desired width in characters
     * @param cleanWhite boolean indicating whether extra white space
     * should be ignored
     * @return formatted version of text
     */
    public static final String format(String text, int width,
				boolean cleanWhite) {
	if(text==null || text.length()==0)
	    return "";

	String o = "", b = "";

	if(cleanWhite) {
	    text = cleanWhiteSpace(text);
	}

	text = "  ".concat(text);

	while(text.length() > 0) {
	    if(width < text.length()) {
		b = text.substring(0, width);
		text = text.substring(width);
		
		int I = b.indexOf('\n');
		if(I>-1) {
		    o += b.substring(0, I + 1);
		    text = b.substring(I + 1) + text;

		} else {
		    I = b.lastIndexOf(" ", b.length() - 1);
		    o += b.substring(0, I) + "\n";
		    text = b.substring(I + 1) + text;

		}

	    } else {
		b = text;
		text = "";
		o += b;
	    }
	}
	return o;
    }



    /**
     * Splits a String into an array of two substrings at
     * a point defined by a deliminating string (not included
     * in return)
     *
     * @param src String to split
     * @param delim String deliminator
     * @return String[2] containing halves of src
     */
    public static final String[] split(String src, String delim) {
	int I = src.indexOf(delim), J = delim.length();
	String[] ret = new String[2];
	if(I < 0) {
	    ret[0] = src;
	    ret[1] = "";

	} else {
	    ret[0] = src.substring(0, I);
	    ret[1] = src.substring(I + J);

	}
	return ret;
    }
    


    /**
     * Splits a String into an array of two substrings at
     * a point defined by a deliminating char (not included
     * in return)
     *
     * @param src String to split
     * @param delim char deliminator
     * @return String[2] containing halves of src
     */
    public static final String[] split(String src, char delim) {
	int I = src.indexOf(delim);
	String[] ret = new String[2];

	if(I == -1) {
	    ret[0] = src;
	    ret[1] = "";

	} else {
	    ret[0] = src.substring(0, I);
	    ret[1] = src.substring(I + 1);

	}
	return ret;
    }



    /**
     * Fixes the length of String text to be the desired length
     * by adding TRAILING spaces.
     *
     * @param text String to fix the length of
     * @param desiredLen integer length to make text
     * @return lengthened version of text
     */
    public static final String fixLength(String text, int desiredLen) {
	return fixLength(text, desiredLen, ' ');
    }
    
    public static final String fixLength(String text, int desiredLen,
				   char filler) {
	if(text == null) {
	    text = "";
	}
	
	final int i = text.length();
	
	if(i == desiredLen) {
	    return text;

	} else if(i>desiredLen) {
	    return text.substring(0, desiredLen);

	} else {
	    byte[] ret = new byte[desiredLen];
	    System.arraycopy(text.getBytes(), 0, ret, 0, i);
	    text = new String( ret );
	    return text.replace( '\0', filler );
	}
    }


    
    /**
     * Fixes the length of String text to be the desired length
     * by adding spaces.
     *
     * @param text String to fix the length of
     * @param desiredLen integer length to make text
     * @param addLeft if true, adds proceding spaces. false adds
     * trailing spaces
     * @return lengthened version of text
     */
    public static final String fixLength(String text, int desiredLen,
				   boolean addLeft) {
	return fixLength(text, desiredLen, ' ', addLeft);
    }


    public static final String fixLength(String text, int desiredLen,
				   char filler, boolean addLeft) {
	if(addLeft) {
	    final int i = text.length();
	    
	    if(i == desiredLen) {
		return text;

	    } else if(i > desiredLen) {
		return text.substring(i - desiredLen);

	    } else {
		byte[] ret = new byte[desiredLen];
		final int l = desiredLen - i;
		System.arraycopy(text.getBytes(), 0, ret, l, i);
		text = new String( ret );
		return text.replace( '\0', filler );
	    }
	} else {
	    return fixLength(text, desiredLen, filler);
	}
	
    }
    
    
    public static final String cleanWhiteSpace(String text) {
	final byte
	    space = (byte) ' ',
	    tab = (byte) '\t',
	    cr = (byte) '\r',
	    lf = (byte) '\n';
	
	byte[] t = text.getBytes();
	
	int J=0;
	final int L = t.length;
	for( int I = 0; I < L; I++ ) {
	    if( t[I] == tab || t[I] == cr ||
		t[I] == lf || t[I] == space ) {
		if( (J > 0) && (t[J-1] != space) ) {
		    t[J++] = space;
		}
	    } else {
		t[J++] = t[I];
	    }
	}
	
	byte[] r = new byte[J];
	System.arraycopy(t, 0, r, 0, J);
	return new String(r);
    }
    
    
    public static final String convertMillis(long millis) {
	StringBuffer sb = new StringBuffer(" sec");
	synchronized( sb ) {

	    int s = (int) (millis / 1000);
	    if(s > 60) {
		final int sec = s % 60;
		sb.insert( 0, Integer.toString(sec) );
		sb.insert( 0, " min " );
		
		final int m = (s - sec)/60;
		if(m > 60) {
		    final int min = m % 60;
		    sb.insert( 0, Integer.toString(min) );
		    sb.insert( 0, " hour " );
		    
		    final int h = (m - min)/60;
		    if(h > 24) {
			final int hour = m % 24;
			sb.insert( 0, Integer.toString(hour) );
			sb.insert( 0, " days " );
			
			final int d = (h - hour)/24;
			sb.insert( 0, Integer.toString(d) );
			
		    } else {
			sb.insert( 0, Integer.toString(h) );
		    }
		} else {
		    sb.insert( 0, Integer.toString(m) );
		}
	    } else {
		sb.insert( 0, Integer.toString(s) );
	    }
	    return sb.toString();
	}
    }
    

    public static final String escapeParse( char[] map_keys,
					     String[] map_values,
					     String parse ) {
	return escapeParse( map_keys, map_values, '\\', parse );
    }


    public static final String escapeParse( char[] map_keys,
					     String[] map_values,
					     char markup, String parse ) {
	if(parse == null) {
	    return null;
	} else if(parse.length() == 0 || map_keys.length == 0 ) {
	    return parse;
	}

	byte[] p = parse.getBytes();

	StringBuffer sb = new StringBuffer(p.length + 60);
	synchronized(sb) {
	    for(int I = 0; I < p.length; I++) {
		if( p[I] == markup && ++I < p.length ) {
		    for(int J = map_keys.length; J-- > 0; ) {
			if(p[I] == map_keys[J]) {
			    sb.append( map_values[J] );
			}
		    }
		} else {
		    sb.append( (char)p[I] );
		}
	    }
	    return sb.toString();
	}
    }

    

    /**
       $KEY uses next whitespace to delimit key string  <br>
       ${KEY} uses sexy-braces to delimit key string  <br><br>

       \$ escapes $  <br>
       \\ escapes \  <br><br>

       any value not found in mappings is treated as ""  <br>
    */
    public static final String variableParse( Map mappings, String parse ) {
	
	if(parse == null) {
	    return null;
	} else if(parse.length() == 0 ||
		  mappings == null || mappings.size() == 0) {
	    return parse;
	}
	
	final char
	    escape = '\\',
	    money = '$',
	    sexy_open = '{',
	    sexy_close = '}';

	byte[] p = parse.getBytes();
	final int L = p.length;
	
	StringBuffer sb = new StringBuffer(p.length + 60);
	synchronized( sb ) {
	    
	    for(int I = 0; I < L; I++) {
		
		if( (p[I] == escape) && (I+1 < L) ) {
		    sb.append( (char)p[++I] );
		    
		} else if( p[I] == money ) {
		    if( ++I < L ) {
			byte[] tmp = new byte[32];
			int i = 0;
			
			if( p[I] == sexy_open ) {
			    I++;
			}
			
			for(; (i<32 && I<L); ) {
			    final byte b = p[I++];
			    if( b == (byte)sexy_close ) {
				break;
			    } else {
				if( b == (byte)money ||
				    b == (byte)escape ||
				    b == (byte)' ' ||
				    b == (byte)'\n' ||
				    b == (byte)'\t' ) {
				    I--;
				    break;
				}
				tmp[i++] = b;
			    }
			}
			I--;
			
			String t = new String(tmp, 0, i);
			t = (String) mappings.get( t );
			if( t != null && t.length() > 0 ) {
			    sb.append( t );
			}
			
		    } else {
			sb.append( (char)money );
		    }
		    
		} else {
		    sb.append( (char)p[I] );
		}
	    }
	    return sb.toString();
	}
    }   

    
}
