

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/


//  File: KifTerm.java

package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The abstract base class for KIF terms
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public abstract class KifTerm extends KifObj {

	/**
	 * For indvar, string and objconst, it is the corresponding string.
	 * for functerm, it is the funconst.
	 */
	protected String _termString;

	/**
	   Create an instance of KifTerm with given KIF term in string format.
	   @param termString KIF term in string format.
	*/
	public KifTerm(String termString)
	{
		_termString = termString;
	}


	/**
	   Create an empty KifTerm object.
	*/
	public KifTerm()
	{
		_termString = null;
	}


	/**
	 * Get the _termString field. Not supposed to be used by developer.
	 * @return
	 * For indvar, string and objconst, it is the corresponding string.
	 * for functerm, it is the funconst.
	 */
	public String
	termString()
	{
		return _termString;
	}


	/**
	 * Get the integer value of this term. valid for KifInt & KifReal.
	 * @return the integer value of term KifInt/ KifReal
	 */
	public int
	intValue()
	{
		return Integer.parseInt(_termString);
	}


	/**
	 * Get the float value of this term. valid for KifInt & KifReal.
	 * @return the real value of term KifInt/ KifReal
	 */
	public float
	floatValue()
	{
		return Float.valueOf(_termString).floatValue();
	}


	/**
	   Convert this KIF term into String format.
	   @return this KIF term in String format.
	*/
	public String
	toString()
	{
		return _termString;
	}
}



