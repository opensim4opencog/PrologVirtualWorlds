//  File: KifRelSent.java


/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/

package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The class for  KIF relational sentence, something like 'predicate'
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifRelSent extends KifSent {
	private String	_relConst;
	private KifSeq	_termSeq;


	/**
	   Create an instance of KifRelSent with given relatioal constant
	   (predicate name) and a sequence of terms(arguments).
	   @param relConst relational constant(predicate name)
	   @param termSeq the sequence of terms(arguments).
	*/
	public KifRelSent(String relConst, KifSeq termSeq)
	{
		setType(KifObj.KIF_RELSENT);

		_relConst = relConst;
		_termSeq = termSeq;
	}


	/**
	   Create an instance of KifRelSent with given relatioal constant
	   (no arguments).
	   @param relConst relational constant(predicate name)
	*/
	public KifRelSent(String relConst)
	{
		setType(KifObj.KIF_RELSENT);

		_relConst = relConst;
		_termSeq = null;
	}


	/**
	 * Get the relational constant (predicate name).
	 * @return the relational constant (predicate name).
	 */
	public String 
	relConst()
	{
		return _relConst;
	}


	/**
	   Get the term(arguments) sequence/list of this predicate.
	   @return the term(arguments) sequence/list of this predicate.
	*/
	public KifSeq 
	termSeq()
	{
		return _termSeq;
	}


	/**
	   Convert this sentence into String format.
	   @return this sentence in String format.
	*/
	public String
	toString()
	{
		if(_termSeq == null)
			return "(" + _relConst + ")";

		return "(" + _relConst + " " + _termSeq.toString() + ")";
	}
}

