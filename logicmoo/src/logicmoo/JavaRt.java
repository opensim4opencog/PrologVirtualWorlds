package logicmoo;

import java.util.Hashtable;
import java.lang.String;
//import jpl.*;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;
import java.lang.*;


public class JavaRt extends Thread {


	/* Creates a non-started Server */
	public  void JavaRt() {
	}

	/* Creates a non-started Server with a prolog @sourceFile */
	public void JavaRt(String sourceFile) {
		bootFile=sourceFile;
	}

	/* Creates a @started Server with a prolog @sourceFile  */
	public void JavaRt(String sourceFile, boolean started) {
		bootFile=sourceFile;
		if ( started ) runStatic();
	}

	/* Working Directory when started  */
	public static String cwd = ".";
	/* Bootfile when started  */
	public static String bootFile = null;
	/* once started = true  */
	public static boolean m_isInitialized = false;
	/* Introspections version of a String.getClass()  */
	public static Class stringClass;
	/* Introspections version of the String Class in a 1-D array containing a Class[0]=String.getClass()  */
	public static Class[] stringClassArrayOfOne;
	/* Debug=0 (None), Debug=1 (Minor), Debug=2 (Extreme)  */
	public static int debug=2;
	/* Dictionary of All objects available to Scripting engine  */
	public static HashMap allObjects;

	/* Starts an instance of a PrologServer from commandline */
	public static void main( String argv[] ) {

		//TODO Parse argv[] properly
		if ( argv.length>0 ) {
			bootFile=argv[0];
		}

		runStatic();

		//     if ( bootFile!=null )
		//         if ( bootFile.equals("test.pl") ) testJava();
	}

	/* Starts an instance of a PrologServer running as thread*/
	public void run() {
		runStatic();
	}

	/* Starts an instance of a PrologServer running as static interface class*/
	public static void runStatic() {
		if ( !m_isInitialized ) {
			initAll();
			//  setCwd(cwd);
			//   consult(bootFile);
		}
	}

	/* Ensures Prolog VM is running and Scripting Engine is running*/
	public synchronized static void initAll() {
		// JPL.init();
		JavaRt.createState();
		m_isInitialized = true;
	}


	/* Ensures Scripting Engine is running*/
	public synchronized static void createState() {
		if ( allObjects!=null )	return;
		stringClass = (new String()).getClass();

		try {
			stringClassArrayOfOne = (Class[])(Array.newInstance(Class.forName("java.lang.Class"),1));
		} catch ( Exception e ) {
			System.out.println("invokeConstructor: " + e);
			e.printStackTrace(System.out);
			System.exit(2);
		}

		Array.set(stringClassArrayOfOne,0,stringClass);
		debug=0;
		allObjects = new HashMap();
		allObjects.put("oJavaRt",new JavaRt());                
		allObjects.put("oSystem",System.class);                

	}

	/* Scripting Engine is running = true*/
	public synchronized boolean isStarted() {
		return m_isInitialized;
	}

	/* Prolog Will call these */

	public synchronized static String createObject(String className) {
		try {
			return nameForInstance(createGlobal(className));
		} catch ( Exception e ) {
			return makeError(e);
		}
	}

	public synchronized static String forgetObject(String objectName) {
		try {
			delInstance(findInstance(objectName));
		} catch ( Exception e ) {
			return makeError(e);
		}
		return "void";
	}

	public synchronized static void addInstance(Object innerInstance) {
		String intKey = "o" + innerInstance.hashCode();
		if ( allObjects.get(intKey) !=null ) return;
		allObjects.put(intKey,innerInstance);
	}

	public synchronized static void delInstance(Object innerInstance) {
		String intKey = "o" + innerInstance.hashCode();
		if ( allObjects.get(intKey) ==null ) return;
		allObjects.remove(intKey);
	}

	/* Creates a new Object for a className */
	public synchronized static Object createGlobal(String className) throws Exception {
		Class innerClass = findClass(className);
		Object innerInstance = innerClass.newInstance();
		addInstance(innerInstance);
		return innerInstance;
	}

	public synchronized static String invokeObject(String objectName,String methodName, Object[] params) {
		if ( allObjects==null )	createState();
		if ( debug>1 ) System.err.println("invokeObject("+objectName +","+ methodName +","+ params + ")");
		Object innerInstance = argToObject(objectName);
		if ( innerInstance==null )		return makeError(new Exception("Object not found in catalog \"" + objectName +"\""));
		if ( debug>1 ) System.err.println("Found Instance: \"" + innerInstance +"\"" );
		try {
			return objectToProlog(invokeObject(innerInstance,(String) argToObject(methodName),argsToObjectVector(params)));
		} catch ( Exception e ) {
			return makeError(e);
		}
	}


	public synchronized static Object invokeObject(Object innerInstance,String methodName, Object[] args) 
	throws Exception {

		// Get/Set Fields
		if ( methodName.startsWith("field") ) {
			Field innerField;
			if ( methodName.charAt(7)=='s' ) {
				innerField = getFieldForObject(innerInstance,(String)args[0]);
				innerField.set(innerInstance,args[1]);
				return innerField.get(innerInstance);
			}
			if ( methodName.charAt(7)=='g' ) {
				innerField = getFieldForObject(innerInstance,(String)args[0]);
				return innerField.get(innerInstance);
			}
		}

		// Invokes Methods
		return getMethodForObject(innerInstance,methodName,getClasses(args)).invoke(innerInstance,args);
	}

	public synchronized static Class[] getClasses(Object[] objs) {
		return getClasses(objs, objs.length);
	}

	public synchronized static Class[] getClasses(Object[] objs, int len) {
		if ( len==0 ) return null;

		Class[] toReturnClasses=null;
		try {
			toReturnClasses = (Class[])Array.newInstance(findClass("Class"),len);
		} catch ( Exception e ) {
			fatalEvent(e);
		}
		for ( int i = 0 ; i < len; i++ ) toReturnClasses[i]=objs[i].getClass();
		return toReturnClasses;
	}


	private static Method getMethodForObject(Object obj,String methodName,Class[] argClasses) 
	throws Exception {
		return classFromInstance(obj).getMethod(methodName,argClasses);
	}

	private static Field getFieldForObject(Object obj,String methodName) 
	throws Exception {
		return classFromInstance(obj).getField(methodName);
	}

	public synchronized static Class classFromInstance(Object obj) {
		if ( obj instanceof Class )		return(Class)obj;
		return obj.getClass();
	}

	public synchronized static Object[] argsToObjectVector(Object[] args) {
		int len = Integer.parseInt((String) args[0]);
		if ( debug >1 )	System.err.println("argsToObjectVector Len=" + (len));

		Object toReturnObjects[]=new Object[len];
		int source = 0;
		for ( int target = 0 ; target < len ; target++ ) {
			source ++;
			toReturnObjects[target ]=argToObject( (Object) args[ source ]);
			if ( debug >1 )
				System.err.println("Arg" + target + "=\"" + args[source] + "\" -> " + toReturnObjects[target].toString());
		}
		return toReturnObjects;
	}                     

	public synchronized static Object argToObject(Object arg) {
		if ( arg instanceof String ) return stringToObj((String)arg);
		if ( arg.getClass().isArray() )	return argsToObjectVector((Object[])arg);
		return arg;
	}

	public synchronized static Object stringToObj(String arg) {

		char fc = arg.charAt(0);
		switch ( fc ) {
			case 'o':
				return findInstance(arg);
			case 'b':
				if ( arg.charAt(1)=='t' ) return new Boolean(true);
				else return new	Boolean(false);
			case 's':
				return arg.substring(1);
			case 'n':
				return null;
			case '$':
				return null;
			case 'i':
				try {
					return new java.lang.Integer(arg.substring(1));
				} catch ( Exception e ) {
					warnEvent(e);
					return new java.lang.Integer(0);
				}
			case 'l':
				try {
					return new java.lang.Float(arg.substring(2));
				} catch ( Exception e ) {
					warnEvent(e);
					return new java.lang.Float(0);
				}
			case 'u':
				return arg.substring(1);
			case 't':
				return arg.substring(1);
		}
		return arg;
	}

	public synchronized static String objectToProlog(Object obj ) {
		if ( obj==null ) return "void";
		if ( obj.getClass().isPrimitive() )		return obj.toString();
		try {
			if ( obj.getClass().getDeclaredMethod("toString",null)!=null ) return obj.toString();
		} catch ( NoSuchMethodException e ) {
		}
		addInstance(obj);
		return nameForInstance(obj);
	}

	public synchronized static Object mktype(String arg) {
		int comma = arg.indexOf(',') ;
		try {
			return makeInstanceFromClass(arg.substring(5,comma++),arg.substring(comma,arg.length()-1));
		} catch ( Exception e ) {
			return makeError(e);
		}
	}

	public synchronized static Object mktype(String theType,String theData)
	throws Exception {
		if ( theType.equals("Long") ) {
			try {
				return new java.lang.Long(theData);
			} catch ( Exception e ) {
				warnEvent(e);
				return new java.lang.Long(0);
			}
		}
		if ( theType.equals("Integer") ) {
			try {
				return new java.lang.Integer(theData);
			} catch ( Exception e ) {
				warnEvent(e);
				return new java.lang.Integer(0);
			}
		}
		if ( theType.equals("Short") ) {
			try {
				return new Short(theData);
			} catch ( Exception e ) {
				warnEvent(e);
				return new Short((short)0);
			}
		}
		if ( theType.equals("Float") ) {
			try {
				return new java.lang.Float(theData);
			} catch ( Exception e ) {
				warnEvent(e);
				return new java.lang.Float(0);
			}
		}
		if ( theType.equals("Byte") ) {
			try {
				return new Byte(theData);
			} catch ( Exception e ) {
				warnEvent(e);
				return new Byte((byte)0);
			}
		}
		if ( theType.equals("Byte") ) {
			try {
				return new Byte(theData);
			} catch ( Exception e ) {
				warnEvent(e);
				return new Byte((byte)0);
			}
		}
		if ( theType.equals("Boolean") ) {
			try {
				return new Boolean(theData);
			} catch ( Exception e ) {
				warnEvent(e);
				return new Boolean(false);
			}
		}
		if ( theType.equals("Char") ) {
			try {
				return new Character(theData.charAt(0));
			} catch ( Exception e ) {
				warnEvent(e);
				return new Character('\0');
			}
		}
		if ( theType.equals("Class") ) {
			try {
				return findClass(theData);
			} catch ( Exception e ) {
				warnEvent(e);
				return findClass("Object");
			}
		}
		if ( theType.equals("String") )	return theData;
		// if (theType.equals("Date")) return new Date(theData);
		return makeInstanceFromClass( theType, theData);
	}

	public synchronized static Object makeInstanceFromClass(String theType,String theData)
	throws Exception {
		Class newClass = findClass(theType);
		try {
			return newClass.getConstructor(stringClassArrayOfOne).newInstance(makeObjectArray(theData));
		} catch ( Exception e ) {
			return newClass.newInstance();
		}
	}

	static Class findClass(String theData) throws Exception {
		try {
			return Class.forName("java.lang." + theData);
		} catch ( Exception e ) {
			try {
				return Class.forName( theData);
			} catch ( Exception ee ) {
				throw ee;
			}
		}
	}


	/* Queries the interface for an Instance (all supers)*/
	public synchronized static String getInstanceDef(String instanceName) {
		try {
			return getInstanceDef(findInstance(instanceName));
		} catch ( Exception e ) {
			return makeError(e );
		}
	}

	public synchronized static String getInstanceDef(Object obj) {
		try {
			return "'$java_object'("  + classToVector(obj.getClass())  + ":"  + obj.hashCode()+ ")";
		} catch ( Exception e ) {
			return makeError(e );
		}
	}

	/* Queries the interface for an Instance (all className)*/
	public synchronized static String getClassDef(String className) {
		try {
			return getClassDef(findClass(className));
		} catch ( Exception e ) {
			return makeError(e );
		}
	}

	public synchronized static String getClassDef(Class clsd) {
		try {
			return classToVector(clsd);
		} catch ( Exception e ) {
			return makeError(e );
		}
	}

	/* Find an instance in allObjects based on HashCode */
	public synchronized static Object findInstance(String instanceName) {


		try {

			if ( debug>1 ) System.out.println("searching for= " + instanceName);
			return allObjects.get(instanceName);

		} catch ( Exception e ) {
			warnEvent(e);
			return null;
		}
	}

	/* Return public synchronized instance name (HashCode)*/
	public synchronized static String nameForInstance(Object innerInstance) {
		return "'$java_instance'(o" +innerInstance.hashCode() + "," + toScriptingName(innerInstance.getClass().getName()) +"('" + innerInstance.toString() + "'))";
	}


	/* Equivalent Bindings for Class Definition into Prolog List */
	public synchronized static String instanceToVector(Object instance, int depth) {
		Class pClass = instance.getClass();
		StringBuffer interfaceList= new StringBuffer();
		interfaceList.append("class(" + toPrologString(pClass.getName())+ ",fields([");
		interfaceList.append(membersValuesToVector(instance, pClass.getFields()));
		interfaceList.append("]),methods([");
		interfaceList.append(membersValuesToVector(instance, pClass.getMethods()));
		interfaceList.append("]))");
		return interfaceList.toString();
	}





	/* Serializes Instance Members into Prolog List */
	public synchronized static String membersValuesToVector(Object instance, Member[] pMembs) {
		StringBuffer interfaceList= new StringBuffer();
		for ( int nMemb=0 ;nMemb < pMembs.length; nMemb++ ) {
			if ( nMemb>0 ) interfaceList.append(",");
			interfaceList.append(memberValueToVector(instance, pMembs[nMemb]));
		}
		return interfaceList.toString();
	}

	/* Serializes Instance Member into Prolog List */
	public synchronized static String memberValueToVector(Object instance,Member pMemb) {
		if ( pMemb instanceof Method ) return methodValueToVector(instance,(Method)pMemb);
		if ( pMemb instanceof Field ) return fieldValueToVector(instance,(Field)pMemb);
		return toScriptingName(pMemb.getName());
	}


	/* Serializes Methods into Prolog List */
	public synchronized static String methodValueToVector(Object instance, Method pMemb) {
		String lcname=  pMemb.getName().toLowerCase();
		if ( pMemb.getParameterTypes().length==0 )

			try {
				if ( lcname.startsWith("get") || pMemb.getReturnType().getName().endsWith("String") )
					return methodToVector(pMemb) + "=" + toPrologString(pMemb.invoke(instance,null));
			} catch ( Exception e ) {
				return methodToVector(pMemb) + "=" + makeError(e);
			}
		return methodToVector(pMemb) + "=uncalled";

	}


	public synchronized static String fieldValueToVector(Object instance, Field sField) {
		try {
			return fieldToVector(sField) + "=" + toPrologString(sField.get(instance));
		} catch ( Exception e ) {
			return fieldToVector(sField) + "=" + makeError(e);
		}
	}



	/* Serializes Class Definition into Prolog List */
	public synchronized static String classToVector(Class pClass) {
		StringBuffer interfaceList= new StringBuffer();
		interfaceList.append("class(" + toPrologString(pClass.getName())+ ",fields([");
		interfaceList.append(membersToVector(pClass.getFields()));
		interfaceList.append("]),methods([");
		interfaceList.append(membersToVector(pClass.getMethods()));
		interfaceList.append("]))");
		return interfaceList.toString();
	}


	/* Serializes Members into Prolog List */
	public synchronized static String membersToVector(Member[] pMembs) {
		StringBuffer interfaceList= new StringBuffer();
		for ( int nMemb=0 ;nMemb < pMembs.length; nMemb++ ) {
			if ( nMemb>0 ) interfaceList.append(",");
			interfaceList.append(memberToVector(pMembs[nMemb]));
		}
		return interfaceList.toString();
	}

	/* Serializes Member into Prolog List */
	public synchronized static String memberToVector( Member pMemb) {
		if ( pMemb instanceof Method ) return methodToVector((Method)pMemb);
		if ( pMemb instanceof Field ) return fieldToVector((Field)pMemb);
		return toScriptingName(pMemb.getName());
	}

	/* Serializes Methods into Prolog List */
	public synchronized static String methodToVector(Method pMemb) {
		StringBuffer interfaceList= new StringBuffer();
		//                interfaceList.append(toScriptingName(Modifier.toString(pMemb.getModifiers())));
		//                interfaceList.append("(");
		interfaceList.append(toScriptingName(pMemb.getName()));
		interfaceList.append("(");
		interfaceList.append(typeToName(pMemb.getReturnType().getName()));
		//                interfaceList.append(",[");
		interfaceList.append(parameterTypesToVector(pMemb.getParameterTypes()));
		interfaceList.append(")");
		return interfaceList.toString();
	}

	public synchronized static String parameterTypesToVector(Class[] pMembs) {
		StringBuffer interfaceList= new StringBuffer();
		for ( int nMemb=0 ;nMemb < pMembs.length; nMemb++ ) {
			//if (nMemb>0) 
			interfaceList.append(",");
			interfaceList.append(parameterToVector(pMembs[nMemb]));
		}
		return interfaceList.toString();
	}

	public synchronized static String fieldToVector(Field sField) {
		return toScriptingName(sField.getName()) + "(" + toScriptingName(sField.getType().getName()) +")";
	}

	public synchronized static String parameterToVector(Class paramClass) {
		return typeToName(paramClass.getName());
	}

	public synchronized static void warnEvent(Exception e) {
		if ( debug>0 )System.err.println("warning: " + e);
	}

	public synchronized static void fatalEvent(Exception e) {
		System.err.println("FATAL ERROR: "+e);
	}

	public synchronized static String makeError(Exception e) {
		return "error('"+e+"')";
	}

	public synchronized static String makeError(String e) {
		return "error('"+e+"')";
	}

	public synchronized static String typeToName(String someName) {
		if ( someName.equals("void") ) return "void";
		if ( someName.startsWith("java.lang.") ) return typeToName(someName.substring(10));
		return toScriptingName(someName);
	}
	public synchronized static String toScriptingName(String someName) {
		return toPrologString(someName);
	}

	public synchronized static String toPrologString(Object someName) {
		if ( someName == null )	return "null";
		return "'" + someName.toString() + "'";
	}

	public synchronized static Object[] makeObjectArray(Object a) {
		Object[] toReturnObject=null; 
		try {
			toReturnObject = (Object[])Array.newInstance(findClass("Object"),1); 
		} catch ( Exception e ) {
			fatalEvent(e);
		}
		toReturnObject[0] = a;
		return toReturnObject;
	}

	public synchronized static Class[] makeClassArray(Class a) {
		Class[] toReturnClasses=null; 
		try {
			toReturnClasses = (Class[])Array.newInstance(findClass("Class"),1); 
		} catch ( Exception e ) {
			fatalEvent(e);
		}
		toReturnClasses[0] = a;
		return toReturnClasses;
	}

}




