package jamud.util;


/**
 * Class containing static values and methods related to colour markup.
 * Actual markup to utility conversion (eg: markup to ansi colours) is
 * done by the Connection class. Implementations of conversions may vary
 * from Connection implementations due to subclassing. <br><br>
 *
 * Colour markup is intended to be in the following format: <br>
 * <code>%fb</code>  where <code>f</code> is a char indicating the
 * foreground colour, and <code>b</code> is a char indicating the
 * background colour. <br><br>
 *
 * <code>%^^</code> to reset to normal <br>
 * <code>%--</code> to turn client-echo off (eg: for password entry) <br>
 * <code>%++</code> to turn client-echo on <br>
 * <code>%!!</code> to clear the client screen <br><br>
 *
 * To change only one property of the two, set the other property to
 * <code>%</code> (eg: <code>%A%</code> to change the foreground to bright
 * green, but not modify the background colour at all, or <code>%%A</code>
 * to change only the background colour to green.<br><br>
 *
 */
public abstract class Colour {
    

    /** General use Player flag to indicate colour should be used */
    public static final String PLAYER_COLOUR_FLAG = "COLOUR";


    /** <code>COLOUR_MARK = '%'</code> */
    public static final char COLOUR_MARK = '%';

    public static final char
	BLACK = '0',
	RED = '1',
	GREEN = '2',
	YELLOW = '3',
	BLUE = '4',
	MAGENTA = '5',
	CYAN = '6',
	WHITE = '7',
	GREY = '8',
	BRIGHT_RED = '9',
	BRIGHT_GREEN = 'A',
	BRIGHT_YELLOW = 'B',
	BRIGHT_BLUE = 'C',
	BRIGHT_MAGENTA = 'D',
	BRIGHT_CYAN = 'E',
	BRIGHT_WHITE = 'F',
	RESET = '^',
	ECHO_OFF = '-',
	ECHO_ON = '+',
	CLEAR_SCREEN = '!';


    /** removes colour markups */
    public static final byte[] wipeColour(byte[] text) {
	int a = 0, b = 0;

	for(; a < text.length; a++ ) {
	    if( text[a] == (byte)COLOUR_MARK ) {
		if( ++a < text.length &&
		    ++a < text.length) {
		    ; //empty line
		}

	    } else if( text[a] == (byte)'\\' ) {
		text[b++] = text[++a];

	    } else if( a != b ) {
		text[b++] = text[a];

	    } else {
		b++;
	    }
	}

	if( a != b ) {
	    byte[] ret = new byte[b];
	    System.arraycopy(text, 0, ret, 0, b);
	    return ret;
	} else {
	    return text;
	}
    }


    /** removes colour markups */
    public static final String wipeColour(String text) {
	return new String( wipeColour(text.getBytes()) );
    }

}
