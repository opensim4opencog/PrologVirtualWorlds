
// File: KifResult.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/


package jkp;

import java.lang.*;
import jbf.*;

/**
 *  This class is NO LONGER a part of this parser, and will no longer
 *  be maintained.
 */

public class KifResult {

	public static KifObj _kifObj;


	public KifObj
	parseResult()
	{
		return KifResult._kifObj;
	}


	public static void printKifObj()
	{
		System.out.println("KifObj type ID is: " + KifResult._kifObj.objType());
		System.out.println(KifResult._kifObj.toString());
	}
}
