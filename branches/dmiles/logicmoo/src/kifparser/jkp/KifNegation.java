// File: KifNegation.java


/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*   . Apr 97: add method body()
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The class for  KIF logical sentence, negation.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifNegation extends KifLogSent {

	/**
	   Create an instance of KifNegation given the sentence to be negated.
	   @param sentence the sentence to be negated.
	*/
	public KifNegation(KifSent sentence)
	{
		setType(KifObj.KIF_NEGATION);
		_sentence = sentence;
	}

	/**
	   Get the body sentence of this negation.
	   @return the body sentence of this negation.
	*/
	public KifSent
	body()
	{
		return _sentence;
	}

	/**
	   Convert this KIF sentence into String format.
	   @return this KIF sentence in String format.
	*/
	public String
	toString()
	{
		return ( "(" + "not" + " " +  _sentence.toString() + ")");
	}
}

