package logicmoo.util;


import java.util.Enumeration;
import java.util.Hashtable;
import java.util.StringTokenizer;

/**
 * Contains a slew of general-purpose static methods
 *
 * @author Siege
 */
public abstract class Util {

	public static final String ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

   /**
    * Replaces all instances of String original with
    * String replacement found in String source.
    *
    * @param source String source
    * @param original Text you want replaced
    * @param replacement Text to replace original
    * @return source with all instances of original changed to replacement
    */
   public static String replace(String source, String original, String replacement) {
		if(source == null || original == null)
			return null;

		if(replacement == null)
			replacement = "";

      return new String( replace(source.getBytes(), original.getBytes(), replacement.getBytes()) );
   }


	public static byte[] replace(byte[] source, byte[] original, byte[] replacement) {
		byte[] ret = new byte[source.length];
		int I = 0,	//source counter
			J = 0,	//ret counter
			d = replacement.length - original.length;	//change in size per replacement

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

					//if there's a change in size, then go and resize the array
					if(d != 0) {
						byte[] t = new byte[ret.length + d];
						System.arraycopy(ret, 0, t, 0, ((d < 0) ? t.length : ret.length));
						ret = t;
					}

					//copy over the replacement bytes
					for(int i = 0; i < replacement.length; i++)
						ret[J++] = replacement[i];

					//update our placement in the source
					I += original.length - 1;
				}
				else
					ret[J++] = source[I];
			}
			else
				ret[J++] = source[I];
		}

		//see, that wasn't so hard.
		return ret;
	}


   public static String concatStrings(String[] strings) {
      return concatStrings(strings, " ");
   }



   public static String concatStrings(String[] strings, String spacer) {
		if(spacer == null) spacer = " ";

		int len = 0;
		for(int I=0; I<strings.length; I++)
			len += strings[I].length();
		if(strings.length > 1)
			len += spacer.length() * strings.length;

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
   public static String addLine(String all, String add_text) {
      return all + "\n" + add_text;
   }
   
   

   /**
    * Removes last line in a multi-line String
    *
    * @param all Text to remove last line from
    * @return all with the last line cut off
    */   
   public static String remLine(String all) {
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
   public static String cleanBackSpace(String text) {
      String OOO = "";
      if(text != null) {
         byte B = (byte)(char) 8;
         byte[] A = text.getBytes();
         for(int I = 0; I<A.length; I++) {
            if(A[I] == B) {
               A[I] = 0;
               for(int J = I-1; J > -1; J--) {
                  if(A[J]!= 0) {
                     A[J] = 0;
                     J = -1;
                  }
               }
            }
         }
         for(int I=0; I<A.length; I++) {
            if(A[I]!= 0) OOO = OOO + (char)A[I];
         }
      }
      return OOO;
   }

   

   /**
    * Removes unwanted characters from strings (ie illegal
    * control characters or non-standard chars)
    *
    * @param text String to clean Escape characters from
    * @return cleaned version of text
    */
   public static String cleanup(String text) {
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
	public static boolean verifyString(String src, String key) {
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
    * @param cleanWhite boolean indicating whether extra white space should be ignored
    * @return formatted version of text
    */
   public static String format(String text, int width, boolean cleanWhite) {
      if(text==null || text.length()==0)
			return "";

      String o = "",
			b = "";

		if(cleanWhite)
			text = cleanWhiteSpace(text);
		text = "  " + text;

      while(text.length() > 0) {
         if(width < text.length()) {
            b = text.substring(0, width);
            text = text.substring(width);
         
            int I = b.indexOf('\n');
            if(I>-1) {
               o += b.substring(0, I + 1);
               text = b.substring(I + 1) + text;
            }
            else {
               I = b.lastIndexOf(" ", b.length() - 1);
               o += b.substring(0, I) + "\n";
               text = b.substring(I + 1) + text;
            }
         }
         else {
            b = text;
            text = "";
            o += b;
         }
      }
      return o;
   }



	/**
	 * Splits a String into an array of arguments. Length of array passed
	 * in determines the number of commands parsed. Last element of array
	 * contains all remaining parts of the passed String. Parses by spaces,
	 * double-quotes, and parenthasis. The parts determined to be sectional
	 * markup are not counted as part of the argument.<br>
	 * eg:
	 * <pre>String[] st = Util.commandSplit("(big stick) little dude");
	 *
	 * st[0] would be "big stick"
	 * st[1] would be "little dude"</pre>
	 */
	public static String[] commandSplit(String command) {
		String[] ret = new String[2];
		int L = 0;

		if(command.charAt(0) == '\"')
			L = command.indexOf("\"", 1);
		else if(command.charAt(0) == '(')
			L = command.indexOf(")", 1);
		else
			return split(command, " ");

		if(L == -1 || L == command.length()) {
			ret[0] = command.substring(1).trim();
			ret[1] = "";
		}
		else {
			ret[0] = command.substring(1, L).trim();
			ret[1] = command.substring(L+1, command.length() - L).trim();
		}

		return ret;
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
	public static String[] split(String src, String delim) {
		int I = src.indexOf(delim), J = delim.length();
		String[] ret = new String[2];
		if(I < 0) {
			ret[0] = src;
			ret[1] = "";
		}
		else {
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
	public static String[] split(String src, char delim) {
		int I = src.indexOf(delim);
		String[] ret = new String[2];
		if(I < 0) {
			ret[0] = src;
			ret[1] = "";
		}
		else {
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
	public static String fixLength(String text, int desiredLen) {
		return fixLength(text, desiredLen, ' ');
	}

	public static String fixLength(String text, int desiredLen, char filler) {
		if(text==null)
			text = "";

		int i = text.length();

		if(i == desiredLen)
			return text;

		else if(i>desiredLen)
			return text.substring(0, desiredLen);

		else {
			byte[] ret = new byte[desiredLen];
			System.arraycopy(text.getBytes(), 0, ret, 0, i);
			text = null;

			for(int I = i; I < desiredLen; I++)
				ret[I] = (byte)filler;

			return new String(ret);
		}
	}



	/**
	 * Fixes the length of String text to be the desired length
	 * by adding spaces.
	 *
	 * @param text String to fix the length of
	 * @param desiredLen integer length to make text
	 * @param addLeft if true, adds proceding spaces. false adds trailing spaces
	 * @return lengthened version of text
	 */
	public static String fixLength(String text, int desiredLen, boolean addLeft) {
		return fixLength(text, desiredLen, ' ', addLeft);
	}

	public static String fixLength(String text, int desiredLen, char filler, boolean addLeft) {
		if(addLeft) {
			int i = text.length();

			if(i == desiredLen)
				return text;
			else if(i > desiredLen)
				return text.substring(i - desiredLen);
			else {
				byte[] ret = new byte[desiredLen];
				int l = desiredLen - i;
				System.arraycopy(text.getBytes(), 0, ret, l, i);
				text = null;

				for(int I = 0; I < l; I++)
					ret[I] = (byte)filler;

				return new String(ret);
			}
		}
		else
			return fixLength(text, desiredLen, filler);

	}


	public static String cleanWhiteSpace(String text) {
		byte space = (byte) ' ', tab = (byte) '\t',
			cr = (byte) '\r', lf = (byte) '\n';

		byte[] t = text.getBytes();

		int J=0;
		for(int I=0; I<t.length; I++) {
			if( t[I] == tab || t[I] == cr || t[I] == lf || t[I] == space ) {
				if( (J > 0) && (t[J-1] != space) )
					t[J++] = space;
			}
			else if(J < I)
				t[J++] = t[I];
		}

		byte[] r = new byte[J];
		System.arraycopy(t, 0, r, 0, J);

		return new String(r);
	}


	public static String convertMillis(long millis) {
		String tm = " sec";

		int s = (int) (millis / 1000);
		if(s > 60) {
			int sec = s % 60;
			tm = " min " + sec + tm;

			int m = (s - sec)/60;
			if(m > 60) {
				int min = m % 60;
				tm = " hour " + min + tm;

				int h = (m - min)/60;
				if(h > 24) {
					int hour = m % 24;
					tm = " days " + hour + tm;

					int d = (h - hour)/24;
					tm = "" + d + tm;
				}
				else
					tm = "" + h + tm;
			}
			else
				tm = "" + m + tm;
		}
		else
			tm = "" + s + tm;

		return tm;
	}


}