// File: KifHornSent

/* set tabstop=4 to read this file

   Data Created: Nov 96
   Maintenance history: 
       Feb 97: Horn clause was dropped from the SKIF specification,
		       therefore, this class no longer makes any sences.
*/

package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The class for horn clause. Please note that there is no horn clause
 *  definition in KIF. We add the definition for horn clause to please
 *  many applications that focuse on horn cluse based inference. 
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifHornSent extends KifLogSent {

	public KifHornSent(KifRelSent head, KifSeq condition)
	{
		setType(KifObj.KIF_HORNSENT);
		_sentence = head;
		_sentSeq  = condition;
	}

	public KifHornSent(KifRelSent head)
	{
		setType(KifObj.KIF_HORNSENT);
		_sentence = head;
	}

	public KifHornSent(KifSeq condition)
	{
		setType(KifObj.KIF_HORNSENT);
		_sentSeq  = condition;
	}


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



	public String
	toString()
	{
		if(_sentSeq == null)
			return "(" + _sentence.toString() + " <=)";

		return "(" + _sentence.toString() + " <= " + 
					_sentSeq.toString() + ")";
	}
}

