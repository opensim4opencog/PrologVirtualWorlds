package cycmoo;

import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;
import java.awt.*;


public class SwiMoo extends LogicMoo {

	/*
	Constructor called by prolog code:
	
	createJamud:-
			 java_create_object('cycmoo.SwiMoo',JAMUD),
			 format('\njamud.Jamud=~q\n',[JAMUD]),!,
			 assert(jamud_object(JAMUD)),!.
	
	*/
	public SwiMoo() throws Exception  {
	}


	/* Equivalent Bindings for Class Definition into Prolog List */
	public synchronized static String classToPLStructure(Class pClass) {
		return consStructure(pClass.getName(),
							 consStructure("fields",membersToPLStructure(pClass.getFields() )),
							 consStructure("methods",membersToPLStructure(pClass.getMethods()))
							);
	}

	/* Equivalent Bindings for Class Definition into Prolog List */
	public synchronized static String classToPLStructure(Object obj) {

		return classToPLStructure(obj.getClass());
	}

	public synchronized static String consStructure(String functor, String arg1) {
		return "'" + functor +"'(" + arg1 + ")"; 
	}
	public synchronized static String consStructure(String functor, String arg1,String arg2) {
		return "'" + functor +"'(" + arg1 + "," + arg2 + ")"; 
	}
	public synchronized static String consStructure(String functor, String arg1,String arg2,String arg3) {
		return "'" + functor +"'(" + arg1 + "," + arg2 + "," + arg3 + ")"; 
	}

	/* Serializes Members into Prolog List */
	public synchronized static String membersToPLStructure(Member[] pMembs) {
		int len = pMembs.length;

		switch ( len ) {
			case 0:
				return "[]";
			case 1:
				return "[" + memberToPLStructure(pMembs[0]) + "]";
		}
		StringBuffer args = new StringBuffer("[" + memberToPLStructure(pMembs[0]));

		for ( int nMemb=1 ; nMemb < len; nMemb++ )
			args.append(",").append(memberToPLStructure(pMembs[nMemb]));

		return args.append("]").toString();
	}


	/* Serializes Member into Prolog List */
	public synchronized static String memberToPLStructure( Member pMemb) {
		if ( pMemb instanceof Method ) return methodToPLStructure((Method)pMemb);
		if ( pMemb instanceof Field ) return fieldToPLStructure((Field)pMemb);
		return "'" + pMemb.getName() + "'";
	}

	public synchronized static String fieldToPLStructure(Field sField) {
		return consStructure(sField.getName(),parameterToPLTerm(sField.getType()));
	}

	/* Serializes Methods into Prolog List */
	public synchronized static String methodToPLStructure(Method pMemb) {
		return  parameterTypesToPLStructure(
										   pMemb.getName(),
										   pMemb.getParameterTypes(),
										   pMemb.getReturnType());
	}

	public synchronized static String parameterTypesToPLStructure(String functor, Class[] pMembs,Class returnType) {

		int len =  pMembs.length;

		switch ( len ) {
			case 0:
				return "'" + functor + "'(" + parameterToPLTerm(returnType) + ")";
			case 1:
				return "'" + functor + "'(" + parameterToPLTerm(pMembs[0]) + "," + parameterToPLTerm(returnType) + ")";
		}
		StringBuffer args = new StringBuffer("'" + functor + "'(" + parameterToPLTerm(pMembs[0]));

		for ( int nMemb=1 ; nMemb < len; nMemb++ )
			args.append(",").append(parameterToPLTerm(pMembs[nMemb]));

		args.append(",").append(parameterToPLTerm(returnType)).append(")");
		return args.toString();
	}


	public synchronized static String parameterToPLTerm(Class paramClass) {
		return "'" + paramClass.getName() + "'";
	}


  }


