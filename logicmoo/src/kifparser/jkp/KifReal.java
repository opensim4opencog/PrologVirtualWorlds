//  File: KifReal.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The  class for KIF term of real(float) number.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifReal extends KifTerm {
	
	/**
	   Create an instance of KifReal(real number) from a float number
	   in String format.
	   @param floatString the float number in string format.
	*/
	public KifReal(String floatString)
	{
		super(floatString);
		setType(KifObj.KIF_REALTERM);
	}

	
	/**
	   Create an instance of KifReal(real number) from a float number.
	   @param floatValue the float number.
	*/
	public KifReal(float floatValue)
	{
		setType(KifObj.KIF_REALTERM);
		_termString = String.valueOf(floatValue);
	}
}



