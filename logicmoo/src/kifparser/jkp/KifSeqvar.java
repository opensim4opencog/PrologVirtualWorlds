//  File: KifSeqvar.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*   . Mar. 7, 97:
*     - add method: varName()
*
*   Note: Further work on this class might be necessary.
*/

package jkp;

import jbf.*;
import java.lang.*;


/**
 * The class for KIF sequencial variable.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifSeqvar extends KifObj {

	private String _varName;

	/**
	   Create an instance of KifSeqvar with given variable name.
	*/
	public KifSeqvar(String varName)
	{
		setType(KifObj.KIF_SEQVAR);
		_varName = varName;
	}

	/**
	 * Get the name of this varialbe.
	 * @return the variable name.
	 */
	public String varName() { return _varName; }
	public String
	toString()
	{
		return _varName;
	}
}

