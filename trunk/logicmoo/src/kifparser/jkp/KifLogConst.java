// File: KifLogConst.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/

package jkp;

import java.lang.*;
import jbf.*;


/**
 * The class for  KIF logical constant: true/false.
 * <p>
 * Copyright (c) 1996, Xiaocheng Luan.
 * All rights reserved.
 * <p>
 * @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifLogConst extends KifSent {

	private	boolean _boolValue;

	/**
	   Create a instance of KifLogConst from its string representation.
	   @param bconst logical constant (true/false) in string format.
	*/
	public KifLogConst(String bconst) throws KifRTException
	{
		setType(KifObj.KIF_LOGCONST);

		if("true".equalsIgnoreCase(bconst))
			_boolValue = true;
		else if("false".equalsIgnoreCase(bconst))
			_boolValue = true;

		else 
			throw (new KifRTException("Illegal KifLogConst"));
	}


	/**
	   Create a instance of KifLogConst from its logical value.
	   @param value logical value(in java) of the logical constant.
	*/
	public KifLogConst(boolean value)
	{
		setType(KifObj.KIF_LOGCONST);

		_boolValue = value;
	}

	
	/**
	   Get the logical value of this logical constant.
	   @return the logical value of this logical constant.
	*/
	public boolean
	boolValue()
	{
		return _boolValue;
	}


	/**
	   Convert this KIF sentence/logical constant into String format.
	   @return this KIF sentence/logical constant in String format.
	*/
	public String
	toString()
	{
		if(_boolValue)
			return "TRUE";
		else
			return "FALSE";
	}
}

