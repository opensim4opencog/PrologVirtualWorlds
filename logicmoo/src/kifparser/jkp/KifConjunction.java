// File: KifConjunction.java


/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*   . Feb. 97:
*     - move the method conjuncSeq(), from its class KifLogSent to this class.
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 * The class for Conjuction, one type of KIF logical sentence.
 * <p>
 * Copyright (c) 1996, Xiaocheng Luan.
 * All rights reserved.
 * <p>
 * @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifConjunction extends KifLogSent {

	/**
	   Create a KifConjunction object from a sequence of sentences.
	   @param sentSeq the sequence of sentences in the conjunction.
	*/
	public KifConjunction(KifSeq sentSeq)
	{
		setType(KifObj.KIF_CONJUNCTION);
		_sentSeq = sentSeq;
	}


	/**
	   Create a KifConjunction object from a single sentence and a
	   sequence of sentences.
	   @param sent    the single sentence.
	   @param sentSeq the sequence of sentence.
	*/
	public KifConjunction(KifSent sent, KifSeq sentSeq)
	{
		setType(KifObj.KIF_CONJUNCTION);
		_sentSeq = new KifSeq(sent, sentSeq);
	}



	/**
	 * Return the sentence sequence in the this conjunction.
	 * @return the sentence sequence in the this conjunction.
	*/
	public KifSeq
	conjuncSeq()
	{
		return _sentSeq;
	}




	/**
	 * Convert the conjunction sentence into string format.
	 * @return the string format of this KIF sentence (conjunction).
	*/
	public String
	toString()
	{
		return ( "(" + "AND" + " " +  _sentSeq.toString() + ")");
	}
}

