// File: KifDisjunction.java


/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*   . Feb. 97:
*     - move the method disjuncSeq(), from its class KifLogSent to this class.
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 * The class for KIF logical sentence, disjunction.
 * <p>
 * Copyright (c) 1996, Xiaocheng Luan.
 * All rights reserved.
 * <p>
 * @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifDisjunction extends KifLogSent {

	/**
	   Create a KIF disjunction from a kif sentence sequence.
	   @param sentSeq the sequence of sentences in the created disjunction
	*/
	public KifDisjunction(KifSeq sentSeq)
	{
		setType(KifObj.KIF_DISJUNCTION);
		_sentSeq = sentSeq;
	}


	/**
	   Create a KIF disjunction from a single kif sentence and a
	   sequence of kif sequences.
	   @param sentSeq the sequence of sentences that would be part of
			  the created disjunction
	   @param sent the sentence that would be part of the created disjunction
	*/
	public KifDisjunction(KifSent sent, KifSeq sentSeq)
	{
		setType(KifObj.KIF_DISJUNCTION);
		_sentSeq = new KifSeq(sent, sentSeq);
	}


	/**
	   get the sequence of sentences in this disjunction.
	   @return the sequence of sentences in this disjunction.
	*/
    public KifSeq
	disjuncSeq()
	{
		return _sentSeq;
	}


	/**
	   Convert this KIF sentence(disjunction) in to string format.
	   @return the KIF sentence(disjunction) in string format.
	*/
	public String
	toString()
	{
		return ( "(" + "OR" + " " +  _sentSeq.toString() + ")");
	}
}

