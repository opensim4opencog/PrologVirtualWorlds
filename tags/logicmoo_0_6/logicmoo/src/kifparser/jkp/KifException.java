// File: KifException.java


/*  Set tabstop = 4 to read this file.
*  
*   Date Created: Nov 96
*   Maintanence History:
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 * The base class for all KIF exception classes.
 * <p>
 * Copyright (c) 1996, Xiaocheng Luan.
 * All rights reserved.
 * <p>
 * @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */

public class KifException extends Exception {

	private String _msgString;

	/**
	   Create a object of KifExceptoin with default message: "KifException !"
	*/
	public KifException() {
		_msgString = new String("KifException !");
	}

	/**
	   Create a object of KifExceptoin with given message.
	   @param msg the exception message.
	*/
	public KifException(String msg) {
		super(msg);
		_msgString = msg;
	}


	/**
	 *  get the associated exception message.
	 *  @return the associated exception message.
	 */
	public String 
	getMessage()
	{
		return _msgString;

	}
}


