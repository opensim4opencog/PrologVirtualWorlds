package swijni;

import java.util.Hashtable;
import java.lang.String;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;
import java.lang.*;


public class JavaRt extends Thread {


    /* Creates a non-started Server */
    public  void JavaRt() {
	createState();
    }

    /* Starts an instance of a PrologServer running as thread*/
    public void run() {
	createState();
    }

    /* Working Directory when started  */
    public static String cwd = ".";
    /* Bootfile when started  */
    public static String bootFile = null;
    /* Introspections version of the String Class in a 1-D array containing a Class[0]=String.getClass()  */
    public static Class[] stringClassArrayOfOne;
    /* Debug=0 (None), Debug=1 (Minor), Debug=2 (Extreme)  */
    public static int debug=2;
    /* Dictionary of All objects available to Scripting engine  */
    public static HashMap allObjects;

    /* Starts an instance of a PrologServer from commandline */
    public static void main( String argv[] ) {

	//TODO Parse argv[] properly
	if( argv.length>0 ) {
	    bootFile=argv[0];
	}
	createState();
    }

    /* Scripting Engine is running = true*/
    public static boolean isStarted() {
	return( allObjects!=null );
    }

    /* Ensures Scripting Engine is running*/
    public synchronized static void createState() {
	if( isStarted() ) return;
	try {
	    stringClassArrayOfOne = (Class[])(Array.newInstance(Class.class,1));
	    Array.set(stringClassArrayOfOne,0,String.class);
	} catch( Exception e ) {
	    e.printStackTrace(System.err);
	}
	debug=0;
	allObjects = new HashMap();
	allObjects.put("oJavaRt",JavaRt.class);                
	allObjects.put("oSystem",System.class);                

    }

    /* Prolog Will call these */

    public  static String createObject(String className) {
	try {
	    return nameForInstance(createGlobal(className));
	} catch( Exception e ) {
	    return makeError(e);
	}
    }

    public  static String forgetObject(String objectName) {
	try {
	    delInstance(findInstance(objectName));
	} catch( Exception e ) {
	    return makeError(e);
	}
	return "void";
    }

    public  static void addInstance(Object innerInstance) {
	if(innerInstance==null) return;
	allObjects.put("o" + innerInstance.hashCode(),innerInstance);
    }

    public  static void delInstance(Object innerInstance) {
	if(innerInstance==null) return;
	String intKey = "o" + innerInstance.hashCode();
	if( allObjects.get(intKey) ==null ) return;
	allObjects.remove(intKey);
    }

    /* Creates a new Object for a className */
    public  static Object createGlobal(String className) throws Exception {
	Class innerClass = findClass(className);
	Object innerInstance = innerClass.newInstance();
	addInstance(innerInstance);
	return innerInstance;
    }

    public  static String invokeObject(String objectName,String methodName, Object[] params) {
	if( allObjects==null ) createState();
	if( debug>1 ) System.err.println("invokeObject("+objectName +","+ methodName +","+ params + ")");
	Object innerInstance = argToObject(objectName);
	if( innerInstance==null )      return makeError(new Exception("Object not found in catalog \"" + objectName +"\""));
	if( debug>1 ) System.err.println("Found Instance: \"" + innerInstance +"\"" );
	try {
	    return objectToProlog(invokeObjectReal(innerInstance,(String)argToObject(methodName),argsToObjectVector(params)));
	} catch( Exception e ) {
	    return makeError(e);
	}
    }


    public  static Object invokeObjectReal(Object innerInstance,String methodName, Object[] args) 
    throws Exception {

	// Get/Set Fields
	if( methodName.startsWith("$") ) {
	    Field innerField = getFieldForObject(innerInstance,(String)args[0]);
	    if( methodName.charAt(1)=='s' ) innerField.set(innerInstance,args[1]);
	    return innerField.get(innerInstance);
	}

	// Invokes Methods
	return getMethodForObject(innerInstance,methodName,getClasses(args)).invoke(innerInstance,args);
    }

    public  static Class[] getClasses(Object[] objs) {
	return getClasses(objs, objs.length);
    }

    public  static Class[] getClasses(Object[] objs, int len) {
	if( len==0 ) return null;

	Class[] toReturnClasses=null;
	try {
	    toReturnClasses = (Class[])Array.newInstance(Class.class,len);
	} catch( Exception e ) {
	    fatalEvent(e);
	}
	for( int i = 0 ; i < len; i++ )	toReturnClasses[i]=objs[i].getClass();
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

    public  static Class classFromInstance(Object obj) {
	if( obj instanceof Class )     return(Class)obj;
	return obj.getClass();
    }

    public  static Object[] argsToObjectVector(Object[] args) {
	int len = args.length;
	Object toReturnObjects[]=new Object[len];
	int source = 0;
	for( int i = 0 ; i < len ; i++ ) {
	    toReturnObjects[i ]=argToObject( (Object) args[ i ]);
	    if( debug >1 )
		System.err.println("Arg" + i + "=\"" + args[i] + "\" -> " + toReturnObjects[i].toString());
	}
	return toReturnObjects;
    }                     

    public  static Object argToObject(Object arg) {
	if( arg instanceof String ) return stringToObj((String)arg);
	if( arg.getClass().isArray() ) return argsToObjectVector((Object[])arg);
	return arg;
    }

    public  static Object stringToObj(String arg) {

	char fc = arg.charAt(0);
	switch( fc ) {
	    case 'o':
		return findInstance(arg);
	    case 'b':
		if( arg.charAt(1)=='t' ) return new Boolean(true);
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
		} catch( Exception e ) {
		    warnEvent(e);
		    return new java.lang.Integer(0);
		}
	    case 'l':
		try {
		    return new java.lang.Float(arg.substring(2));
		} catch( Exception e ) {
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

    public  static String objectToProlog(Object obj ) {
	if( obj==null )	return "void";
	if( obj.getClass().isPrimitive() )  return obj.toString();
	if( debug>1 ) System.err.println("objectToProlog(" + obj + ")");
	addInstance(obj);
	/*
		try {
		    if ( obj.getClass().getDeclaredMethod("toString",null)!=null ) 
			return obj.toString(); 
		} catch ( NoSuchMethodException e ) {
		}
	*/
	return nameForInstance(obj);
    }

    public  static Object mktype(String arg) {
	int comma = arg.indexOf(',');
	try {
	    return makeInstanceFromClass(arg.substring(5,comma++),arg.substring(comma,arg.length()-1));
	} catch( Exception e ) {
	    return makeError(e);
	}
    }

    public  static Object mktype(String theType,String theData)
    throws Exception {
	if( theType.equals("Long") ) {
	    try {
		return new java.lang.Long(theData);
	    } catch( Exception e ) {
		warnEvent(e);
		return new java.lang.Long(0);
	    }
	}
	if( theType.equals("Integer") ) {
	    try {
		return new java.lang.Integer(theData);
	    } catch( Exception e ) {
		warnEvent(e);
		return new java.lang.Integer(0);
	    }
	}
	if( theType.equals("Short") ) {
	    try {
		return new Short(theData);
	    } catch( Exception e ) {
		warnEvent(e);
		return new Short((short)0);
	    }
	}
	if( theType.equals("Float") ) {
	    try {
		return new java.lang.Float(theData);
	    } catch( Exception e ) {
		warnEvent(e);
		return new java.lang.Float(0);
	    }
	}
	if( theType.equals("Byte") ) {
	    try {
		return new Byte(theData);
	    } catch( Exception e ) {
		warnEvent(e);
		return new Byte((byte)0);
	    }
	}
	if( theType.equals("Byte") ) {
	    try {
		return new Byte(theData);
	    } catch( Exception e ) {
		warnEvent(e);
		return new Byte((byte)0);
	    }
	}
	if( theType.equals("Boolean") ) {
	    try {
		return new Boolean(theData);
	    } catch( Exception e ) {
		warnEvent(e);
		return new Boolean(false);
	    }
	}
	if( theType.equals("Char") ) {
	    try {
		return new Character(theData.charAt(0));
	    } catch( Exception e ) {
		warnEvent(e);
		return new Character('\0');
	    }
	}
	if( theType.equals("Class") ) {
	    try {
		return findClass(theData);
	    } catch( Exception e ) {
		warnEvent(e);
		return Object.class;
	    }
	}
	if( theType.equals("String") ) return theData;
	// if (theType.equals("Date")) return new Date(theData);
	return makeInstanceFromClass( theType, theData);
    }

    public  static Object makeInstanceFromClass(String theType,String theData)
    throws Exception {
	Class newClass = findClass(theType);
	try {
	    return newClass.getConstructor(stringClassArrayOfOne).newInstance(makeObjectArray(theData));
	} catch( Exception e ) {
	    return newClass.newInstance();
	}
    }

    static Class findClass(String theData) throws Exception {
	try {
	    return Class.forName( theData);
	} catch( Exception e ) {
	    try {
		return Class.forName("java.lang." + theData);
	    } catch( Exception ee ) {
		throw ee;
	    }
	}
    }

    /* Queries the interface for an Instance (all supers)*/
    public  static String getInstanceDef(String instanceName) {
	try {
	    return getInstanceDef(findInstance(instanceName));
	} catch( Exception e ) {
	    return makeError(e );
	}
    }

    public  static String getInstanceDef(Object obj) {
	try {
	    return "'$java_object'("  + classToVector(obj.getClass())  + ":"  + obj.hashCode()+ ")";
	} catch( Exception e ) {
	    return makeError(e );
	}
    }

    /* Queries the interface for an Instance (all className)*/
    public  static String getClassDef(String className) {
	try {
	    return getClassDef(findClass(className));
	} catch( Exception e ) {
	    return makeError(e );
	}
    }

    public  static String getClassDef(Class clsd) {
	try {
	    return classToVector(clsd);
	} catch( Exception e ) {
	    return makeError(e );
	}
    }

    /* Find an instance in allObjects based on HashCode */
    public  static Object findInstance(String instanceName) {
	try {
	    if( debug>1 ) System.out.println("searching for= " + instanceName);
	    return allObjects.get(instanceName);
	} catch( Exception e ) {
	    warnEvent(e);
	    return null;
	}
    }

    /* Return public  instance name (HashCode)*/
    public  static String nameForInstance(Object innerInstance) {
	return "'$java_instance'(" +innerInstance.hashCode() + "," + toScriptingName(innerInstance.getClass().getName()) +"('" + innerInstance.toString() + "'))";
    }


    /* Equivalent Bindings for Class Definition into Prolog List */
    public  static String instanceToVector(Object instance, int depth) {
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
    public  static String membersValuesToVector(Object instance, Member[] pMembs) {
	StringBuffer interfaceList= new StringBuffer();
	for( int nMemb=0 ;nMemb < pMembs.length; nMemb++ ) {
	    if( nMemb>0 ) interfaceList.append(",");
	    interfaceList.append(memberValueToVector(instance, pMembs[nMemb]));
	}
	return interfaceList.toString();
    }

    /* Serializes Instance Member into Prolog List */
    public  static String memberValueToVector(Object instance,Member pMemb) {
	if( pMemb instanceof Method ) return methodValueToVector(instance,(Method)pMemb);
	if( pMemb instanceof Field ) return fieldValueToVector(instance,(Field)pMemb);
	return toScriptingName(pMemb.getName());
    }


    /* Serializes Methods into Prolog List */
    public  static String methodValueToVector(Object instance, Method pMemb) {
	String lcname=  pMemb.getName().toLowerCase();
	if( pMemb.getParameterTypes().length==0 )

	    try {
		if( lcname.startsWith("get") || pMemb.getReturnType().getName().endsWith("String") )
		    return methodToVector(pMemb) + "=" + toPrologString(pMemb.invoke(instance,null));
	    } catch( Exception e ) {
		return methodToVector(pMemb) + "=" + makeError(e);
	    }
	return methodToVector(pMemb) + "=uncalled";

    }


    public  static String fieldValueToVector(Object instance, Field sField) {
	try {
	    return fieldToVector(sField) + "=" + toPrologString(sField.get(instance));
	} catch( Exception e ) {
	    return fieldToVector(sField) + "=" + makeError(e);
	}
    }



    /* Serializes Class Definition into Prolog List */
    public  static String classToVector(Class pClass) {
	StringBuffer interfaceList= new StringBuffer();
	interfaceList.append("class(" + toPrologString(pClass.getName())+ ",fields([");
	interfaceList.append(membersToVector(pClass.getFields()));
	interfaceList.append("]),methods([");
	interfaceList.append(membersToVector(pClass.getMethods()));
	interfaceList.append("]))");
	return interfaceList.toString();
    }


    /* Serializes Members into Prolog List */
    public  static String membersToVector(Member[] pMembs) {
	StringBuffer interfaceList= new StringBuffer();
	for( int nMemb=0 ;nMemb < pMembs.length; nMemb++ ) {
	    if( nMemb>0 ) interfaceList.append(",");
	    interfaceList.append(memberToVector(pMembs[nMemb]));
	}
	return interfaceList.toString();
    }

    /* Serializes Member into Prolog List */
    public  static String memberToVector( Member pMemb) {
	if( pMemb instanceof Method ) return methodToVector((Method)pMemb);
	if( pMemb instanceof Field ) return fieldToVector((Field)pMemb);
	return toScriptingName(pMemb.getName());
    }

    /* Serializes Methods into Prolog List */
    public  static String methodToVector(Method pMemb) {
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

    public  static String parameterTypesToVector(Class[] pMembs) {
	StringBuffer interfaceList= new StringBuffer();
	for( int nMemb=0 ;nMemb < pMembs.length; nMemb++ ) {
	    //if (nMemb>0) 
	    interfaceList.append(",");
	    interfaceList.append(parameterToVector(pMembs[nMemb]));
	}
	return interfaceList.toString();
    }

    public  static String fieldToVector(Field sField) {
	return toScriptingName(sField.getName()) + "(" + toScriptingName(sField.getType().getName()) +")";
    }

    public  static String parameterToVector(Class paramClass) {
	return typeToName(paramClass.getName());
    }

    public  static void warnEvent(Exception e) {
	if( debug>0 )System.err.println("warning: " + e);
    }

    public  static void fatalEvent(Exception e) {
	System.err.println("FATAL ERROR: "+e);
    }

    public  static String makeError(Exception e) {
	return "error('"+e+"')";
    }

    public  static String makeError(String e) {
	return "error('"+e+"')";
    }

    public  static String typeToName(String someName) {
	if( someName.equals("void") ) return "void";
	if( someName.startsWith("java.lang.") )	return typeToName(someName.substring(10));
	return toScriptingName(someName);
    }
    public  static String toScriptingName(String someName) {
	return toPrologString(someName);
    }

    public  static String toPrologString(Object someName) {
	if( someName == null ) return "null";
	return "'" + someName.toString() + "'";
    }

    public  static Object[] makeObjectArray(Object a) {
	Object[] toReturnObject=null; 
	try {
	    toReturnObject = (Object[])Array.newInstance(Object.class,1); 
	} catch( Exception e ) {
	    fatalEvent(e);
	}
	toReturnObject[0] = a;
	return toReturnObject;
    }

    public  static Class[] makeClassArray(Class a) {
	Class[] toReturnClasses=null; 
	try {
	    toReturnClasses = (Class[])Array.newInstance(Class.class,1); 
	} catch( Exception e ) {
	    fatalEvent(e);
	}
	toReturnClasses[0] = a;
	return toReturnClasses;
    }

}


