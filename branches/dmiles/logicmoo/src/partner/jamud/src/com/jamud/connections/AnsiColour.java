package com.jamud.connections;


import jamud.util.Colour;
import java.io.ByteArrayOutputStream;


public abstract class AnsiColour {


    // prefix to various ANSI markups
    private static final byte[] ESC = {
	(byte) 27,
	(byte) '['
    };
    

    /** ANSI clear-screen */
    private static final byte[] CLEAR_SCREEN = {
	ESC[0],
	ESC[1],
	(byte) 72,
	ESC[0],
	ESC[1],
	(byte) 74
    };


    // index of colour characters
    private static final byte[] c_index = {
	(byte) Colour.BLACK,
	(byte) Colour.RED,
	(byte) Colour.GREEN,
	(byte) Colour.YELLOW,
	(byte) Colour.BLUE,
	(byte) Colour.MAGENTA,
	(byte) Colour.CYAN,
	(byte) Colour.WHITE,
	(byte) Colour.GREY,
	(byte) Colour.BRIGHT_RED,
	(byte) Colour.BRIGHT_GREEN,
	(byte) Colour.BRIGHT_YELLOW,
	(byte) Colour.BRIGHT_BLUE,
	(byte) Colour.BRIGHT_MAGENTA,
	(byte) Colour.BRIGHT_CYAN,
	(byte) Colour.BRIGHT_WHITE
    };


    // echo off
    private static final byte[] ECHO_OFF = {
	(byte) 255,
	(byte) 251,
	(byte) 1
    };


    // echo on
    private static final byte[] ECHO_ON = {
	(byte) 255,
	(byte) 252,
	(byte) 1
    };



    public static final byte[] parseColourMarkup(byte[] text) {

	ByteArrayOutputStream bo = new ByteArrayOutputStream();

	final byte n = (byte)'\n',
	    r = (byte)'\r';

	for(int I = 0; I < text.length; I++ ) {
	    if( text[I] == (byte)Colour.COLOUR_MARK ) {

		if( ++I < text.length ) {
		    final byte f = text[I];
		    if( ++I < text.length) {
			final byte[] t = translateColourMarkup(f, text[I]);
			bo.write(t, 0, t.length);
		    }
		}

	    } else if( text[I] == (byte)'\\' ) {
		// escapes the next character
		bo.write( text[++I] );

	    } else if( text[I] == n ) {
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


    public static final byte[] translateColourMarkup(final byte f,
						     final byte b) {

	//System.out.println("translating %" + (char)f + (char)b );

	String o = new String( ESC );

	switch( f ) {
	case Colour.ECHO_OFF:
	    //System.out.println("ECHO_OFF");
	    return AnsiColour.ECHO_OFF;

	case Colour.ECHO_ON:
	    //System.out.println("ECHO_ON");
	    return AnsiColour.ECHO_ON;

	case Colour.CLEAR_SCREEN:
	    //System.out.println("CLEAR_SCREEN");
	    return AnsiColour.CLEAR_SCREEN;

	case Colour.RESET:
	    //System.out.print("ESC 0");
	    o = o.concat( "0" );
	    if( b == f || b == Colour.COLOUR_MARK ) {
		//System.out.println("m");
		return o.concat( "m" ).getBytes();
	    } else {
		//System.out.print(";");
		o = o.concat( ";" );
	    }
	    break;

	case Colour.COLOUR_MARK:
	    if( b == f ) {
		//System.out.println("[ignored]");
		return new byte[0];
	    }
	    break;

	default:
	    for(int I = c_index.length; I-- > 0; ) {
		if( f == c_index[I] ) {
		    if( I > 7) {
			//System.out.print("1;");
			o = o.concat( "1;" );
			I -= 8;
		    }
		    String t = Integer.toString( I + 30 );
		    //System.out.print( t );
		    o = o.concat( t );
		    if( b == Colour.COLOUR_MARK ) {
			//System.out.println("m");
			return o.concat( "m" ).getBytes();
		    } else {
			//System.out.print(";");
			o = o.concat( ";" );
		    }
		    break;
		}
	    }
	}

	switch( b ) {
	case Colour.ECHO_OFF:
	    //System.out.println("ECHO_OFF");
	    return AnsiColour.ECHO_OFF;

	case Colour.ECHO_ON:
	    //System.out.println("ECHO_ON");
	    return AnsiColour.ECHO_ON;

	case Colour.CLEAR_SCREEN:
	    //System.out.println("CLEAR_SCREEN");
	    return AnsiColour.CLEAR_SCREEN;

	case Colour.RESET:
	    //System.out.println("0m");
	    return o.concat("0m").getBytes();

	default:
	    for(int I = c_index.length; I-- > 0; ) {
		if( b == c_index[I] ) {
		    if( I > 7) {
			I -= 8;
		    }
		    String t = Integer.toString( I + 40 );
		    //System.out.print( t );
		    o = o.concat( t );
		    break;
		}
	    }
	    //System.out.println( "m" );
	    return o.concat( "m" ).getBytes();
	}

    }


}
