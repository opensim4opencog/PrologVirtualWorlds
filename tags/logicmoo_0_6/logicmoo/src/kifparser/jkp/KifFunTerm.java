
//  File: KifFunTerm.java


/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 9.
*   Maintanence History:
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 * The class for KIF functional term
 * <p>
 * Copyright (c) 1996, Xiaocheng Luan.
 * All rights reserved.
 * <p>
 * @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifFunTerm extends KifTerm {

	private KifSeq _termSeq;

	/**
	   Create a KifFunTerm object with given functional term in string format.
	   @param termString the functional term in string format.
	*/
	public KifFunTerm(String termString)
	{
		setType(KifObj.KIF_FUNTERM);
		_termString = termString;
		_termSeq = null;
	}


	/**
	   Create a KifFunTerm object from given function name and argumentst.
	   @param funconst the function name (funtion constant).
	   @param termSeq the argument sequence of this functional term.
	*/
	public KifFunTerm(String funconst, KifSeq termSeq)
	{
		setType(KifObj.KIF_FUNTERM);
		_termString = funconst;
		_termSeq = termSeq;
	}


	/**
	 * Get the the function name (constant) in string format.
	 * @return the functional constant, that is the function name.
	 */
	public String
	funconst()
	{
		return _termString;
	}


	/**
	 * Get the sequence of arguments of this funcitonal term.
	 * @return the term sequence(arguments) of this functional term.
	 */
	public KifSeq
	termSeq()
	{
		return _termSeq;
	}


	/**
	   Convert this functional term into string format.
	   @return the functional term in string format.  
	*/ 
	public String toString() 
	{ 
		if(_termSeq != null) 
			return("(" +  _termString + " " + _termSeq.toString() + ")"); 

		return "(" + _termString + ")"; 
	}
}



