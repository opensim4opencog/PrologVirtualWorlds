
//  File: KifString.java


/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The  class for KIF string terms
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifString extends KifTerm {
	
	/**
	   Create a KifString object from given string.
	   @param string given kif string in string format.
	*/
	public KifString(String string)
	{
		super(string);
		setType(KifObj.KIF_STRINGTERM);
	}

	/*
	public String 
	toString()
	{
		return "\"" + super.toString() + "\"";
	}
	*/
}



