
//  File KifObj.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/



package jkp;

import jbf.*;
import java.lang.*;



/**
 * The base class for all KIF objects.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public abstract class KifObj {

	/**
	   A constructor that does nothing.
	*/
	public KifObj() { }

	private int	_typeID;
	private String	_name;


	/**
	   Get the type ID of this KIF object. Type IDs of various of Kif objects
	   are defined in this class. you are encouraged to use InstanceOf 
	   operator instead of this method.
	   @return the type ID of this KIF object.
	*/
	public int 
	objType() 
	{	
		return _typeID;	
	}


	/**
	 * Set the type ID. Generally, this method is called internally 
	 * upon creation ONLY.
	 * @param id The id number for the classs
	 */
	public void 
	setType(int id) 
	{	
		_typeID = id;	
		_name = idToName(id);
	}


	/**
	   Get the name this KIF object. 
	   @return the name of this KIF object.
	   @see KifObj#KifObjName
	*/
	public String 
	objName()
	{
		return _name;
	} 
	
	
	/**
	   Convert this KIF object into String format. Subclasses are
	   supposed to overide this method.
	   @return this KIF object in String format.
	*/
	public String
	toString()
	{
		return "";
	}


	/**
	   Get the object name with given type ID.
	   @param ID the given type ID.
	   @return the type ID.
	*/
	public static String 
	idToName(int ID) throws KifRTException
	{
		for(int i=0; i<_KifObjName.length; i++)
			if(_KifObjName[i]._ID == ID)
				return new String(_KifObjName[i]._name);
		
		throw (new KifRTException("No such Operator ID !"));
	}



	/*	Defination of object type IDs
	                    !!!!   Important  !!!!
		when insert/update/delete objects IDs, be sure to check
		the consistency of _kifObjName of this KifObj class and
		the _kifOpString of KifOperator class.
	*/
	public static final int KIF_SEQ = 1;
	public static final int KIF_SEQVAR = 2;

	public static final int KIF_INDVARTERM = 100;
	public static final int KIF_STRINGTERM = 101;
	public static final int KIF_OBJCONSTTERM = 102;
	public static final int KIF_FUNTERM = 103;
	public static final int KIF_REALTERM = 104;
	public static final int KIF_INTTERM = 105; 

	public static final int KIF_LOGCONST = 200;
	public static final int KIF_RELSENT = 201;

	public static final int KIF_NEGATION = 202;
	public static final int KIF_CONJUNCTION = 203;
	public static final int KIF_DISJUNCTION = 204;
	public static final int KIF_IMPLICATION = 205;
	public static final int KIF_EQUIVALENCE = 206;

	public static final int KIF_HORNSENT = 207;


/*	Kif operator IDs
*/
	public static final int KIF_MIN_OP = 4;

	//	term operator
	public static final int KIFOP_MIN_TERM_OP = 300;
	public static final int KIFOP_LISTOF = 301;
	public static final int KIFOP_QUOTE = 302;
	public static final int KIFOP_IF = 303;
	public static final int KIFOP_COND = 304;
	public static final int KIFOP_MAX_TERM_OP = 305;


	//	sentence operator
	public static final int KIFOP_MIN_SENT_OP = 320;
	public static final int KIFOP_SENTEQ = 321;
	public static final int KIFOP_SENTNOTEQ = 322;
	public static final int KIFOP_NOT = 323;
	public static final int KIFOP_AND = 324;
	public static final int KIFOP_OR = 325;
	public static final int KIFOP_IMPLIES = 326;
	public static final int KIFOP_IMPLIED = 327;
	public static final int KIFOP_EQUIV = 328;
	public static final int KIFOP_FORALL = 329;
	public static final int KIFOP_EXISTS = 330;
	public static final int KIFOP_MAX_SENT_OP = 331;

	//	defination operator
	public static final int KIFOP_MIN_DEFOP = 340;
	public static final int KIFOP_DEFOBJECT = 341;
	public static final int KIFOP_DEFUNCTION = 342;
	public static final int KIFOP_DEFRELATION = 343;
	public static final int KIFOP_MAX_DEFOP = 344;


	//	numerical relational operator
	public static final int KIFOP_MIN_NUM_OP = 360;
	public static final int KIFOP_NUMEQ = 361;
	public static final int KIFOP_NUMNEQ = 362;
	public static final int KIFOP_LESS = 363;
	public static final int KIFOP_GREATER = 364;
	public static final int KIFOP_EQLESS = 365;
	public static final int KIFOP_EQGREATER = 366;

/*	End of Kif operator IDs
*/

	
	/*	Kif object name table
	*/
	private static KifObjName _KifObjName[] = {

		new KifObjName(KIF_SEQ,			"Kif Sequence"),
		new KifObjName(KIF_SEQVAR,		"Kif Sequential variable"),

		//  terms
		new KifObjName(KIF_INDVARTERM,	"Kif Individual Variable"),
		new KifObjName(KIF_STRINGTERM,	"Kif String"),
		new KifObjName(KIF_OBJCONSTTERM,"Kif Object Constant"),
		new KifObjName(KIF_FUNTERM,		"Kif Functional Term"),
		new KifObjName(KIF_REALTERM,	"Kif Real Number"),
		new KifObjName(KIF_INTTERM,		"Kif Integer"),

		//	sentences
		new KifObjName(KIF_LOGCONST,	"Kif Logical Constant"),
		new KifObjName(KIF_RELSENT,		"Kif Relational Sentence"),
		new KifObjName(KIF_NEGATION,	"Kif Negation"),
		new KifObjName(KIF_CONJUNCTION,	"Kif Conjunction"),
		new KifObjName(KIF_DISJUNCTION,	"Kif Disjunction"),
		new KifObjName(KIF_IMPLICATION,	"Kif Implication"),
		new KifObjName(KIF_EQUIVALENCE,	"Kif Equivalence"),
		new KifObjName(KIF_HORNSENT,	"Kif Horn Sentences"),

		//	term operators
		new KifObjName(KIFOP_LISTOF,	"Kif listof operator"),
		new KifObjName(KIFOP_QUOTE,		"Kif quote operator"),
		new KifObjName(KIFOP_IF,		"Kif IF operator"),
		new KifObjName(KIFOP_COND,		"Kif cond operator"),

		//	sentence operators
		new KifObjName(KIFOP_SENTEQ,	"Kif equal operator"),
		new KifObjName(KIFOP_SENTNOTEQ,	"Kif not equal operator"),
		new KifObjName(KIFOP_NOT,		"Kif not operator"),
		new KifObjName(KIFOP_AND,		"Kif and operator"),
		new KifObjName(KIFOP_OR,		"Kif or operator"),
		new KifObjName(KIFOP_IMPLIES,	"Kif implies operator"),
		new KifObjName(KIFOP_IMPLIED,	"Kif implied operator"),
		new KifObjName(KIFOP_EQUIV,		"Kif equivalence operator"),
		new KifObjName(KIFOP_FORALL,	"Kif forall operator"),
		new KifObjName(KIFOP_EXISTS,	"Kif exists operator"),

		//	defination operators
		new KifObjName(KIFOP_DEFOBJECT,	"Kif defobject operator"),
		new KifObjName(KIFOP_DEFUNCTION,"Kif defunction operator"),
		new KifObjName(KIFOP_DEFRELATION,"Kif defrelation operator"),

		//	numerical relational operator
		new KifObjName(KIFOP_NUMEQ,		"Kif equal operator"),
		new KifObjName(KIFOP_NUMNEQ,	"Kif not equal operator"),
		new KifObjName(KIFOP_LESS,		"Kif less than operator"),
		new KifObjName(KIFOP_GREATER,	"Kif greater than operator"),
		new KifObjName(KIFOP_EQLESS,	"Kif less than or equal operator"),
		new KifObjName(KIFOP_EQGREATER,	"Kif greater than or equal operator"),
	};

}


/**
 *  This class provides an (ID, name) pair struct.
 */
class KifObjName {
	public int		_ID;
	public String	_name;

	/**
	   Create an instance of KifObjName, an ID-name pair.
	   @param ID the type id.
	   @param name the corresponding name.
	*/
	public KifObjName(int ID, String name)
	{
		_ID = ID;
		_name = name;
	}
}



