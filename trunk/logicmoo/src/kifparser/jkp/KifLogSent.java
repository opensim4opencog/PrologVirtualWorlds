//  File: KifLogSent.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*   . Feb. 97:
*     - Distribute some methods to its subclasses, including KifNegation,
*       KifEquivalence, KifConjunction, KifDisjunction, KifImplication.
*	. Arp 97: method sentence() is no longer encouraged to be used outside
*			  of the parser.
*/

package jkp;

import java.lang.*;
import jbf.*;


/**
 *  The base base class for KIF logical sentences.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifLogSent extends KifSent {

	protected KifSent	_sentence;
	protected KifSeq  	_sentSeq;

	/**
	   Create an instance of empty KifLogSent().
	*/
	public KifLogSent()
	{
		_sentence = null;
		_sentSeq = null;
	}


	/**
	   Get the _sentence field. Any use outside of this parser is strongly
	   discouraged.
	   @return the _sentence field.
	*/
	public KifSent
	sentence()
	{
		return _sentence;
	}

	/**
	   Get the _sentSeq field. Any use outside of this parser is strongly
	   discouraged.
	   @return the _sentSeq field.
	*/
	public KifSeq
	sentenceSeq()
	{
		return _sentSeq;
	}


	/*  Moved to KifEquivalence.java
	public KifSent 
	leftSent()
	{
		return _sentence;
	}


	public KifSent 
	rightSent()
	{
		return (KifSent)_sentSeq.car();
	}
	*/


	/*	Moved to class KifImplication
	public KifSeq
	antecedenceSeq()
	{
		return _sentSeq;
	}


	public KifSent
	consequenceSent()
	{
		return _sentence;
	}
	*/


	/*	Move to class KifConjunction
	public KifSeq
	conjuncSeq()
	{
		return _sentSeq;
	}
	*/


	/*  Move to class KifDisjunction
	public KifSeq
	disjuncSeq()
	{
		return _sentSeq;
	}
	*/


}


