package swijni;

import java.util.Hashtable;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;
import java.lang.*;
//import java.awt.*;
//import jpl.*;
//import jpl.fli.*;


public class JavaRt extends Thread {


    /* Working Directory when started  */
    public static String cwd = ".";
    /* Bootfile when started  */
    public static String bootFile = null;
    /* Introspections version of the String Class in a 1-D array containing a Class[0]=String.getClass()  */
    public static Class[] stringClassArrayOfOne;
    /* Debug=0 (None), Debug=1 (Minor), Debug=2 (Extreme)  */
    public static int debug=0;
    /* Dictionary of All objects available to Scripting engine  */
    public static HashMap allObjects;

    /* Creates a non-started Server */
    public  void JavaRt() {
	ensureState();
    }

    /* Starts an instance of a PrologServer running as thread*/
    public void run() {
	ensureState();
    }

    /* Scripting Engine is running = true*/
    public static boolean isStarted() {
	return( allObjects!=null );
    }

    /* Ensures Scripting Engine is running*/
    public synchronized static void ensureState() {
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

    /* Starts an instance of a PrologServer from commandline */
    public static void main( String argv[] ) {

	//TODO Parse argv[] properly
	if( argv.length>0 ) {
	    bootFile=argv[0];
	}
	ensureState();
    }

    /* Prolog Will call these */

    public  static boolean forgetObject(int hashcode) {
	ensureState();
	try {
	    String intKey = "o" + hashcode;
	    if( allObjects.get(intKey) ==null )	return false;
	    allObjects.remove(intKey);
	    return true;
	} catch( Exception e ) {
	    return false;
	}
    }

    public static Object createObject(String className, Object[] params) {
	try {
	    Object innerInstance = newObject(findClass(className),params);
	    rememberObject(innerInstance);
	    return innerInstance;
	} catch( Exception e ) {
	    return makeError(e);
	}
    }

    public static void rememberObject(Object innerInstance) {
	if( innerInstance==null ) return;
	allObjects.put("o" + innerInstance.hashCode(),innerInstance);
    }

    public  static String invokeObject(Object innerInstance,String methodName, Object[] args) {
	ensureState();
	try {
	    try {
		Method method = classFromInstance(innerInstance).getMethod(methodName,getClasses(args));
	    return objectToProlog(method.invoke(innerInstance,args));
	    } catch( NoSuchMethodException me ) {
		throw new Exception("" + me + " " + getClasses(args)[0] + " " + getClasses(args)); 
	    }
	} catch( Exception e ) {
	    return makeError(e);
	}
    }

    public static String setObjectField(Object innerInstance,String fieldName, Object prolog_value) {
	ensureState();
	try {
	    classFromInstance(innerInstance).getField(fieldName).set(innerInstance,prolog_value);
	    return "true";
	} catch( Exception e ) {
	    return makeError(e);
	}
    }

    public static String getObjectField(Object innerInstance,String fieldName) {
	ensureState();
	try {
	    return objectToProlog(classFromInstance(innerInstance).getField(fieldName).get(innerInstance));
	} catch( Exception e ) {
	    return makeError(e);
	}
    }

    public  static Class[] getClasses(Object[] objs) {
	return getClasses(objs, objs.length);
    }

    public  static Class[] getClasses(Object[] objs, int len) {
	Class[] toReturnClasses = new Class[len];
	for( int i = 0 ; i < len; i++ ) {
	    if( objs[i]==null )
		toReturnClasses[i]=null;
	    else toReturnClasses[i]=objs[i].getClass();
	}
	return toReturnClasses;
    }

    public  static Class classFromInstance(Object innerInstance) throws NullPointerException {
	if( innerInstance instanceof Class ) return(Class)innerInstance;
	return innerInstance.getClass();
    }


    public  static String makeError(Exception e) {
	return "error('"+e+"')";
    }

    public static Class findClass(String className) throws Exception {
	try {
	    return Class.forName(className);
	} catch( Exception e ) {
	    try {
		return Class.forName("java.lang." + className);
	    } catch( Exception ee ) {
		throw e;
	    }
	}
    }

    public static int newInt(int value) {
	return value;
    }

    public static  Float newFloat(float value) {
	return new Float(value);
    }

    public static  String newString(String value) {
	return new String(value);
    }

    public static  Object newNull() {
	return null;
    }

    public static  Object newBoolean(boolean tf) {
	return new Boolean(tf);
    }

    public static Object findObject(int hashcode) {
	return allObjects.get("o" + hashcode);
    }

    public static Object newObject(Class innerClass, Object[] args) {
	ensureState();
	try {
	    Object innerInstance =  innerClass.getConstructor(getClasses(args)).newInstance(args);
	    return objectToProlog(innerInstance);
	} catch( Exception e ) {
	    return makeError(e);
	}
    }

    public  static String StringToProlog(Object someName) {
	return "'" + someName + "'";
    }

    public  static String objectToProlog(Object innerInstance ) {
	if( innerInstance==null )   return "$null";
	if( innerInstance instanceof Void ) return "'Void'";
	if( innerInstance instanceof Number ) return innerInstance.toString();
	//if( object instanceof Byte ) return(Integer)object;
	//if( object instanceof Integer )	   return object;
	//if( object instanceof Long ) return object;
	//if( object instanceof Double ) return object;
	if( innerInstance instanceof Character ) return "`"+innerInstance;
	if( innerInstance instanceof Iterator )	return "`"+innerInstance;
	if( innerInstance instanceof Boolean ) {
	    if( innerInstance.equals(Boolean.TRUE) ) return "$true";
	    return "$false";
	}
	if( innerInstance instanceof String ) return StringToProlog(innerInstance);
	if( innerInstance instanceof Class ) return "$java_class("+StringToProlog(((Class)innerInstance).getName())+")";
	Class jclass = innerInstance.getClass(); 
	if( innerInstance instanceof Iterator )	return iteratorToList(jclass,(Iterator)innerInstance);
	if( innerInstance instanceof Collection ) return iteratorToList(jclass,((Collection)innerInstance).iterator());
	if( jclass.isArray() ) return arrayToPrologList(jclass,(Object[])innerInstance);
	if( jclass.isPrimitive() ) {
	    if( jclass == boolean.class ) {
		if( Boolean.TRUE.equals(innerInstance) ) return "$true";
		return "$false";
	    }
	    if( jclass == char.class ) return "`"+innerInstance;
	    return innerInstance.toString();

	}
	rememberObject(innerInstance);
	return "'$java_object'(" +innerInstance.hashCode() + ")";
    } 

    /* Serializes Array into OpenCyc List */ 
    public static String arrayToPrologList(Class parent,Object[] pMembs) {
	int len = pMembs.length; 
	if( len<1 ) return "$java_array(" + StringToProlog(parent.getName()) + ",[])";
	StringBuffer sb = new StringBuffer("$java_array(" + StringToProlog(parent.getName()) + ",[" + objectToProlog(pMembs[0]));
	for( int nMemb=1 ; nMemb < len; nMemb++ ) sb.append(",").append(objectToProlog(pMembs[nMemb]));
	sb.append("])");
	return sb.toString(); 
    } 


    /* Serializes Iterator into OpenCyc List */ 
    public static synchronized String iteratorToList(Class parent,Iterator pMembs) {
	StringBuffer sb = new StringBuffer("$iterator(" + StringToProlog(parent.getName()) + ",[");
	if( pMembs.hasNext() ) sb.append(objectToProlog(pMembs.next()));
	while( pMembs.hasNext() ) sb.append(",").append(objectToProlog(pMembs.next()));
	sb.append("])");
	return sb.toString(); 
    } 

    public static Object createCollection(Class innerClass, Object[] objs) {
	try {
	    Collection col = (Collection)(innerClass.newInstance());
	    for( int i=0 ; i<objs.length ; i++ ) {
		col.add(objs[i]);
	    }
	    return col;
	} catch( Exception e ) {
	    return makeError(e);
	}
    }
}


