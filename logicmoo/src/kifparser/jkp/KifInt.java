
//  File: KifInt.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The class for KIF term of integer number.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifInt extends KifTerm {
	
	/**
	   Create a KifInt from a integer in its string format.
	   @param intString the integer in string format.
	*/
	public KifInt(String intString)
	{
		super(intString);
		setType(KifObj.KIF_INTTERM);
	}

	
	/**
	   Create a KifInt from a integer number.
	   @param intValue the integer in value.
	*/
	public KifInt(int intValue)
	{
		setType(KifObj.KIF_INTTERM);
		_termString = String.valueOf(intValue);
	}
}



