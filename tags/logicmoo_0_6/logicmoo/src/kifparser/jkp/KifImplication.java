// File: KifImplication.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*   . Feb. 97:
*     - move some methods from its class KifLogSent to this class:
*       . antecedenceSeq()
*       . consequenceSent()
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The class for KifImplication, a type of KIF logical sentence.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifImplication extends KifLogSent {

	/**
	   Create a KifImplication object with given antecedent and consequence.
	   @param conSent the consequence sentence.
	   @param anteSeq the sequence of antecedent sentences.
	*/
	public KifImplication(KifSent conSent, KifSeq anteSeq)
	{
		setType(KifObj.KIF_IMPLICATION);
		_sentSeq = anteSeq;
		_sentence = conSent;
	}


	/**
	   Create a KifImplication object with given antecedent and consequence.
	   of the forms (=> sentence) or (<= sentence). The KIF specification
	   for this logical implication is ambigious, Thus this constructor
	   may be changed or even dropped later.
	   @param conSent the consequence sentence.
	*/
	public KifImplication(KifSent conSent)
	{
		setType(KifObj.KIF_IMPLICATION);
		_sentence = conSent;
	}



	/**
	   Get the sequence of antecedent sentences.
	   @return the sequence of antecedent sentences.
	*/
	public KifSeq
	antecedenceSeq()
	{
		return _sentSeq;
	}


	/**
	   Get the consequence sentence.
	   @return the consequence sentence.
	*/
	public KifSent
	consequenceSent()
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
		if(_sentSeq == null)
			return ( "(<= " + _sentence.toString() + ")");
		else
			return ( "(<= " + _sentence.toString() + " " 
							+ _sentSeq.toString() + ")");
	}
}

