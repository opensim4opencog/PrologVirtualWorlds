package logicmoo;

import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;
import java.awt.*;

import jamud.*;
import CiaoJava.*;

import jamud.command.*;
import jamud.object.*;
import jamud.object.event.*;
import jamud.util.*;
import com.jamud.commands.*;
import com.jamud.connections.*;
import com.jamud.communications.*;
import com.jamud.creation.*;
import net.n3.nanoxml.*;

public class LogicMoo extends JNIPrologServer {

	/*
	Constructor called by prolog code:
	
	createJamud:-
			 java_create_object('logicmoo.LogicMoo',JAMUD),
			 format('\njamud.Jamud=~q\n',[JAMUD]),!,
			 assert(jamud_object(JAMUD)),!.
	
	*/

	public void LogicMoo() {
		try {
			LogfileWriter = new FileWriter(new File("/logicmoo.debug.txt"));
		} catch ( Exception e ) {

		}

		logWrite("Start jamud");
		//startJamud();


	}


	public FileWriter LogfileWriter;
	public Jamud jamudInstance;
	public MudObjectRoot jamudMudObjectRoot; 
	public MudObject jamudMudObjectBelowRoot; 


	/* 
	Temporary debugging for now
	*/
	public void logWrite(String msg) {
		try {
			System.out.println(msg);
			LogfileWriter.write(msg);
		} catch ( Exception e ) {

		}
	}

	/*
	
	For loading references to Jamud basic objects as used in:

	loadJamudReferences:-
			  jamud_object(JAMUD),
			  java_invoke_method(JAMUD,getJamudInstance(Instance)),
			  java_invoke_method(JAMUD,getJamudMudObjectRoot(MudRoot)),!,
			assert(jamud_instance(Instance)),
			assert(jamud_root(MudRoot)).
	
	
	*/              

	public Jamud getJamudInstance() {
		return  jamudInstance;
	}

	/*
	
	
	For loading references to Jamud basic objects as used in:

	loadJamudReferences:-
			  jamud_object(JAMUD),
			  java_invoke_method(JAMUD,getJamudInstance(Instance)),
			  java_invoke_method(JAMUD,getJamudMudObjectRoot(MudRoot)),!,
			assert(jamud_instance(Instance)),
			assert(jamud_root(MudRoot)).
	
	*/              

	public MudObjectRoot getJamudMudObjectRoot() {
		return  jamudMudObjectRoot;
	}

	/*
	Boots the Jamud for now from prolog as:
	
	startJamud:-
					 jamud_object(JAMUD),!,
					 java_invoke_method(JAMUD,startJamud(X)).
	*/              
	public void startJamud() {
		logWrite("Start jamud");
		if ( jamudInstance==null ) {
			try {
				String[] args = { "jamud.xml"} ; 
				Jamud.main( args );
				jamudInstance = Jamud.currentInstance();
				jamudMudObjectRoot = jamudInstance.mudObjectRoot();
				//Iterator cobjs = jamudMudObjectRoot.childObjects();
				//jamudMudObjectBelowRoot = (MudObject)cobjs.next();
			} catch ( Exception e ) {
				logWrite("" +e );
				e.printStackTrace(System.out);
			}
		}
	}


	/* Equivalent Bindings for Class Definition into Prolog List */
	public synchronized static PLStructure classToPLStructure(Class pClass) {
		return consStructure(pClass.getName(),
						  consStructure("fields",membersToPLStructure(pClass.getFields() )),
						  consStructure("methods",membersToPLStructure(pClass.getMethods()))
						 );
	}

	/* Equivalent Bindings for Class Definition into Prolog List */
	public synchronized static PLStructure classToPLStructure(Object obj) {

		return classToPLStructure(obj.getClass());
	}

	public synchronized static PLStructure consStructure(String functor, PLTerm arg1) {
		PLTerm args[] = { arg1};
		return new PLStructure(functor,args);
	}
	public synchronized static PLStructure consStructure(String functor, PLTerm arg1,PLTerm arg2) {  
		PLTerm args[] = { arg1 , arg2};
		 return new PLStructure(functor,args);
	}
	public synchronized static PLStructure consStructure(String functor, PLTerm arg1,PLTerm arg2,PLTerm arg3) {
		PLTerm args[] = { arg1 , arg2, arg3}; 
		return new PLStructure(functor,args);
	}

	/* Serializes Members into Prolog List */
	public synchronized static PLTerm membersToPLStructure(Member[] pMembs) {
//		PLList interfaceList= new PLList(PLList.nil);
		int plen = pMembs.length;
		if (plen==0) return PLList.nil;

		PLList interfaceList = null;
		try {
			 interfaceList= new PLList(memberToPLStructure(pMembs[0]), (PLList.nil));
		}
		catch (Exception e){
		}
		for ( int nMemb=1 ;nMemb < pMembs.length; nMemb++ ) {
			interfaceList.add(memberToPLStructure(pMembs[nMemb]));
		}
		return interfaceList;
	}
  

	/* Serializes Member into Prolog List */
	public synchronized static PLTerm memberToPLStructure( Member pMemb) {
		if ( pMemb instanceof Method ) return methodToPLStructure((Method)pMemb);
		if ( pMemb instanceof Field ) return fieldToPLStructure((Field)pMemb);
		return new PLAtom(pMemb.getName());
	}

	/* Serializes Methods into Prolog List */
	public synchronized static PLStructure methodToPLStructure(Method pMemb) {
		return new PLStructure(pMemb.getName(),
							   parameterTypesToPLStructure(pMemb.getParameterTypes(),
														   pMemb.getReturnType()));
	}

	public synchronized static PLTerm[] parameterTypesToPLStructure(Class[] pMembs,Class returnType) {
		
		PLTerm args[]=new PLTerm[pMembs.length+1];

		for ( int nMemb=0 ;nMemb < pMembs.length; nMemb++ ) {
			args[nMemb] = parameterToPLTerm(pMembs[nMemb]);
		}
		args[pMembs.length] = parameterToPLTerm(returnType);
		return args;
	}

	public synchronized static PLStructure fieldToPLStructure(Field sField) {
		return consStructure(sField.getName(),parameterToPLTerm(sField.getType()));
	}

	public synchronized static PLTerm parameterToPLTerm(Class paramClass) {
		return new PLAtom(paramClass.getName());
	}







}


