
//  File: KifParser.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/


package jkp;

import jbf.*;
import java.lang.*;
import java.util.*;
import java.io.*;


/**
 *  This class is an interface to the the KIF parser (YYparse) generated.
 *  We use synchronized method to avoid more than one
 *  thread to call YYparse at the same time.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifParser {

	public static KifObj _result;

	/**
	 * This is the method you should call when you want to parse
	 * a kif string. This method is safe when called simutaneously
	 * in more than one thread. But in actural fact, these calls
	 * are serialized.
	 * <p>
	 * Note: one sentence at a time.
	 * @param text the KIF language string to parse.
	 * @return the Kif object obtained in the parsing.
	 * @see KifParser#parse(InputStream)
	 */
	public static synchronized KifObj 
	parse(String text)	throws ParseException
	{
		//System.out.println("KifParser.parse: ---" + text + "---");

		StringBufferInputStream inStream = new StringBufferInputStream(text + "\n");

		YYlex	yyl = new YYlex(inStream, System.out);
		YYparse	yyp = new YYparse(yyl);

		_result = null;

		try {
			yyp.yyparse();
		}
		catch (Exception e) {
			System.out.println("exception detected, parse failed");
			e.printStackTrace();
			throw ( new ParseException());
		}

		//System.out.println("Result object type: " + _result.objName());
		return _result;
	}


	/**
	 *  For debug only, not supposed to be used by developers.
	 *  Use this method with caution. when called, if another thread
	 *  is calling "parse", the printed result by this method may not
	 *  be predictable.
	 */
	public static void
	printResult()
	{
		System.out.println("[" + _result.objName() + "] " + _result);
		//System.out.println("parse result is: " + _result);
	}



	/**
	 * Read one KIF sentence from the inStream and parse it.
	 * This method is NOT multi-thread safe.
	 * @param inStream the input stream of KIF sentences.
	 * @return the Kif object obtained in the parsing.
	 * @see KifParser#parse(String)
	 */
	public static synchronized KifObj 
	parse(InputStream inStream)	throws ParseException
	{
		YYlex	yyl = new YYlex(inStream, System.out);
		YYparse	yyp = new YYparse(yyl);

		_result = null;

		try {
			yyp.yyparse();
		}
		catch (Exception e) {
			System.out.println("exception detected, parse failed");
			e.printStackTrace();
			throw ( new ParseException());
		}

		return _result;
	}
}
