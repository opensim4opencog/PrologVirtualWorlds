
// File: KifRTException.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/


package jkp;

import jbf.*;
import java.lang.*;


/**
 *  The base class for all KIF runtime exception classes.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */

public class KifRTException extends RuntimeException {

	private String _msgString;

	/**
	   Create an instance of KifRTException with default message.
	*/
	public KifRTException() {
		_msgString = new String("KifRTException !");
	}

	/** 
	   Create an instance of KifRTException with given message.
	   @param msg the given message to be associated with the exception.
	*/
	public KifRTException(String msg) {
		super(msg);
		_msgString = msg;
	}

	/**
	 * Get the message associated with the exception.
	 * @return the message associated with the exception.
	 */
	public String 
	getMessage()
	{
		return _msgString;

	}
}


