
//  File: KifIndvar.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*   Mar. 7, 97:
*   . add method varName()
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The  class for KIF individual variable terms
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifIndvar extends KifTerm {
	
	/**
	   Create an individual variable from object its string format.
	   @param varName the name of the variable in string
	*/
	public KifIndvar(String varName)
	{
		super(varName);
		setType(KifObj.KIF_INDVARTERM);
	}


	/**
	 * Get the variable name(in the form of: ?name).
	 * @return the variable name in string(of the form: ?name)
	 */
	public String varName() { return termString(); }
}



