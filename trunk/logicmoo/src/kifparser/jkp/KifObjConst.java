
//  File: KifString.java


/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*   . Mar 7, 97:
*     - add method objConst().
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The  class for KIF object constant terms
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifObjConst extends KifTerm {
	
	/**
	   Create a instance of KifObjConst from a object cosntant in string
	   format.
	   @param objconst the object cosntant in string format.
	*/
	public KifObjConst(String objconst)
	{
		super(objconst);
		setType(KifObj.KIF_OBJCONSTTERM);
	}

	/**
	 * Convert the object constant into string format.
	 * @return the object constant string, the name of represented object.
	 */
	public String objConst() { return termString(); }
}

