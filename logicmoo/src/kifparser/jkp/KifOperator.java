//  File: KifOperator.java


/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/


package jkp;

import java.lang.*;
import jbf.*;


/**
 *  The class for KIF operator.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifOperator extends KifObj {

	private String _opString;


	/**
	   Create an instance of KifOperator with given operator type(id).
	   @param opID the type ID of the KIF operator.
	*/
	public KifOperator(int opID)
	{
		setType(opID);
		_opString = KifOperator.opToString(opID);
	}


	/**
	   Convert this KIF operator into String format.
	   @return this KIF operator in String format.
	*/
	public String
	toString()
	{
		return _opString;
	}


	/**
	   Given operator type ID, return the operator in String format
	   @param opID type ID of the KIF operator.
	   @return the operator in String format.
	*/
	public static String 
	opToString(int opID) throws KifRTException
	{
		for(int i=0; i<_kifOpString.length; i++)
			if(_kifOpString[i]._opID == opID)
				return new String(_kifOpString[i]._opString);
		
		throw (new KifRTException("No such Operator ID !"));
	}


	private static KifOpString _kifOpString[] = {

		new KifOpString(KifObj.KIFOP_LISTOF,	"LISTOF"),
		new KifOpString(KifObj.KIFOP_QUOTE,		"QUOTE"),
		new KifOpString(KifObj.KIFOP_IF,		"IF"),
		new KifOpString(KifObj.KIFOP_COND,		"COND"),

		new KifOpString(KifObj.KIFOP_SENTEQ,	"="),
		new KifOpString(KifObj.KIFOP_SENTNOTEQ,	"\\="),
		new KifOpString(KifObj.KIFOP_NOT,		"NOT"),
		new KifOpString(KifObj.KIFOP_AND,		"AND"),
		new KifOpString(KifObj.KIFOP_OR,		"OR"),
		new KifOpString(KifObj.KIFOP_IMPLIES,	"=>"),
		new KifOpString(KifObj.KIFOP_IMPLIED,	"<="),
		new KifOpString(KifObj.KIFOP_EQUIV,		"<=>"),
		new KifOpString(KifObj.KIFOP_FORALL,	"FORALL"),
		new KifOpString(KifObj.KIFOP_EXISTS,	"EXISTS"),

		new KifOpString(KifObj.KIFOP_DEFOBJECT,	"DEFOBJECT"),
		new KifOpString(KifObj.KIFOP_DEFUNCTION,"DEFUNCTION"),
		new KifOpString(KifObj.KIFOP_DEFRELATION,"DEFRELATION"),

		new KifOpString(KifObj.KIFOP_NUMEQ,		"="),
		new KifOpString(KifObj.KIFOP_NUMNEQ,	"\\="),
		new KifOpString(KifObj.KIFOP_LESS,		"<"),
		new KifOpString(KifObj.KIFOP_GREATER,	">"),
		new KifOpString(KifObj.KIFOP_EQLESS,	"=<"),
		new KifOpString(KifObj.KIFOP_EQGREATER,	">=")
	};
}



/**
   The typeID-string pairs of the KIF operators.
*/
class KifOpString {
	public int _opID;
	public String _opString;

	/**
	   Create a typeID-string pair of a KIF operator.
	   @param opID type ID of the operator
	   @param opString the string representation of the operator.
	*/
	public KifOpString(int opID, String opString)
	{
		_opID = opID;
		_opString = opString;
	}
}

