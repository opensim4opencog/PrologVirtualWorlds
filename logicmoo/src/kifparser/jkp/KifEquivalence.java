// File: KifEquivalence.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*   . Feb. 97:
*     - move some methods from its super class, KifLogSent to this class:
*       . leftSent()
*       . rightSent()
*/



package jkp;

import jbf.*;
import java.lang.*;


/**
 * The class for  KIF logical sentence, KifEquivalence.
 * <p>
 * Copyright (c) 1996, Xiaocheng Luan.
 * All rights reserved.
 * <p>
 * @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifEquivalence extends KifLogSent {

	/**
	   Create a KIF equivalence logical sentence from two given sentences.
	   @sent1 one of the two sentences in the created equivalence sentence
	   @sent2 another one of the two sentences in the created equivalence
			  sentence
	*/
	public KifEquivalence(KifSent sent1, KifSent sent2)
	{
		setType(KifObj.KIF_EQUIVALENCE);
		_sentence = sent1;
		_sentSeq = new KifSeq(sent2);
	}


	/**
	   Get one of the two sentences in the equivalence sentence.
	   @return one of the two sentences in the equivalence sentence.
	   @see KifEquivalence#rightSent
	*/
	public KifSent
	leftSent()
	{
		return _sentence;
	}


	/**
	   Get the other one of the two sentences in the equivalence sentence.
	   @return the other one of the two sentences in the equivalence sentence.
	   @see KifEquivalence#leftSent
	*/
	public KifSent
	rightSent()
	{
		return (KifSent)_sentSeq.car();
	}


	/**
	   Convert this sentence into String format.
	   @return this sentence in string format
	*/
	public String
	toString()
	{
		return ( "(<=> " + _sentence.toString() + " " 
						+ _sentSeq.toString() + ")");
	}
}

