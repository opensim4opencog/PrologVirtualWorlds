//<?
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

    public static void rememberObject(Object innerInstance) {
	if( innerInstance==null ) return;
	allObjects.put("o" + innerInstance.hashCode(),innerInstance);
    }

    public static String debugList(Object[] clasparams) {
	if( clasparams == null ) return "null";
	StringBuffer sb= new StringBuffer(" {");
	for( int i=0;i<clasparams.length;i++ ) sb.append(" ").append(""+clasparams[i]);
	return sb.append("} ").toString();  
    }

    public  static String invokeObject(Object innerInstance,String methodName, Object[] args) {
	ensureState();
	try {
	    return objectToProlog(invokeSomething(classFromInstance(innerInstance),innerInstance,methodName,args));
	} catch( Exception e ) {
	    return makeError(e);
	}
    }

    public static String invokeStatic(Class innerClass,String methodName, Object[] args) {
	ensureState();
	try {
	    return objectToProlog(invokeSomething(innerClass,null,methodName,args));
	} catch( Exception e ) {
	    return makeError(e);
	}
    }                                

    public static Object invokeSomething(Class innerClass,Object innerInstance,String methodName, Object[] args) throws Exception {
	if( args==null || args.length==0 ) {
	    return innerClass.getMethod(methodName,null).invoke(innerInstance,args);
	} else {
	    Class[] clasparams = getClasses(args);
	    Method[] methods = innerClass.getMethods();
	    Object calling = Array.newInstance(Object.class,args.length);
	    for( int i=0 ;i<methods.length;i++ ) {
		Class[] methodParms = methods[i].getParameterTypes();
		if( methodParms.length==clasparams.length && methodName.equals(methods[i].getName()) ) {
		    try {
			if( convertTypeArray( args,clasparams,calling,methodParms) ) return methods[i].invoke(innerInstance,(Object[])calling);
		    } catch( IllegalArgumentException iae ) {
		    }
		}
	    }
	}
	throw new NoSuchMethodException("invokeObject \"" +innerInstance+ "\" ("+innerClass+")" + methodName + " " + debugList(getClasses(args))+ " " + debugList(args));
    }

    public static String createObject(String className, Object[] params) {
	try {
	    Object innerInstance = newObject(findClass(className),params);
	    rememberObject(innerInstance);
	    return objectToProlog(innerInstance);
	} catch( Exception e ) {
	    return makeError(e);
	}
    }

    public static Object newObject(Class innerClass, Object[] args) throws Exception {
	ensureState();
	try {
	    if( args==null || args.length==0 ) return innerClass.newInstance();
	    Class[] clasparams = getClasses(args);
	    Constructor[] methods = innerClass.getConstructors();
	    Object calling = Array.newInstance(Object.class,args.length);
	    for( int i=0 ;i<methods.length;i++ ) {
		Class[] methodParms = methods[i].getParameterTypes();
		if( clasparams.length==methodParms.length ) {
		    try {
			if( convertTypeArray( args,clasparams,calling,methodParms) )
			    return methods[i].newInstance((Object[])calling);
		    } catch( IllegalArgumentException iae ) {
		    }
		}
	    }
	} catch( Exception e ) {
	    throw new Exception( ""+innerClass+ " " + debugList(getClasses(args))+ " " + debugList(args) +" "+ e );
	}
	throw new InvalidClassException(""+innerClass+ " " + debugList(getClasses(args))+ " " + debugList(args));
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
	if( objs == null ) return null;
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

    public static  Integer newInt(int value) {
	return new Integer(value);
    }

    public static  Float newFloat(float value) {
	return new Float(value);
    }

    public static  Boolean newBoolean(boolean tf) {
	return new Boolean(tf);
    }
    
    public static  String newString(String value) {
	return new String(value);
    }

    public static  Object newNull() {
	return null;
    }


    public static Object findObject(int hashcode) {
	return allObjects.get("o" + hashcode);
    }

    public static boolean superClass(Class sub,Class sup) {
	Class[] sups = sub.getClasses();
	for( int i=0;i<sups.length;i++ )
	    if( sups[i]==sup ) return true;
	return false;
    }

    public static boolean convertTypeArray(Object[] objs,Class[] from, Object callArray, Class[] to) {
	for( int i=0;i<to.length;i++ )
	    if( !convertType(objs[i],from[i],callArray,i,to[i]) ) return false;
	return true;
    }

    public static boolean fillArrayType(Class to,int len,Object[] objs,Class[] from,Object callArray) {
	for( int i=0; i<len ;i++ )
	    if( !convertType(objs[i],from[i],callArray,i,to) ) return false;
	return true;
    }


    public static boolean convertType(Object obj,Class from,Object callArray, int arg, Class to) {
	if( to.isAssignableFrom(from) ) {
	    Array.set(callArray,arg,obj);
	    return true;
	}

	if( to.isArray() ) {
	    if( to==char[].class ) {
		if( obj instanceof String ) {
		    Array.set(callArray,arg,((String)obj).toCharArray());
		    return true;
		}
	    }
	    if( !from.isArray() ) return false;

	    Object[] objA = (Object[])obj;
	    Object subCallArray = Array.newInstance(to,objA.length);
	    if( !fillArrayType(to.getComponentType(),objA.length,objA,getClasses(objA),subCallArray) ) return false;
	    Array.set(callArray,arg,subCallArray);
	    return true;
	}
	if( !to.isPrimitive() ) {
	    try {
		Class[] fromA = {from};
		//fromA[0]=from;
		Constructor cons = to.getConstructor(fromA);
		Object[] objA = {obj};
		Array.set(callArray,arg,cons.newInstance(objA));
		return true;
	    } catch( Exception e ) {
	    }
	    // Object to 
	    if( from.isArray() ) {
		Object[] fromA = (Object[])obj;
		if( to == String.class ) {
		    Object puthere = Array.newInstance(char.class,fromA.length);
		    if( !fillArrayType(char.class,fromA.length,fromA,getClasses(fromA),puthere) ) return false;
		    Array.set(callArray,arg,puthere);
		    return true;
		}
		if( superClass(to,Collection.class) ) {
		    try {
			Collection make = (Collection) to.newInstance();
			for( int i=0;i<fromA.length;i++ ) {
			    make.add(fromA[i]);
			}
			Array.set(callArray,arg,make);
		    } catch( Exception ie ) {
			return false;
		    }
		    return true;
		}
		return false;
	    }
	    if( from==String.class ) {
		return false;
	    } else {
		try {
		    Class[] sa1 = {String.class};
		    Object[] objA = {obj.toString()};
		    Array.set(callArray,arg,to.getConstructor(sa1).newInstance(objA));
		    return true;
		} catch( Exception e ) {
		}
		return false;
	    }
	    //return false;    // Object to;
	} else {
	    // Primitive to 
	    if( from.isArray() ) return false;
	    if( to==boolean.class ) {
		if( obj==Boolean.TRUE ) {
		    Array.setBoolean(callArray,arg,true);
		    return true;
		}
		if( obj==Boolean.FALSE ) {
		    Array.setBoolean(callArray,arg,false);
		    return true;
		}
		return false;
	    }
	    if( obj instanceof Number ) {
		if( to==float.class ) {
		    Array.setFloat(callArray,arg,((Number)obj).floatValue());
		    return true;
		}
		if( to==int.class ) {
		    Array.setInt(callArray,arg,((Number)obj).intValue());
		    return true;
		}
		if( to==long.class ) {
		    Array.setLong(callArray,arg,((Number)obj).longValue());
		    return true;
		}
		if( to==double.class ) {
		    Array.setDouble(callArray,arg,((Number)obj).doubleValue());
		    return true;
		}
		if( to==short.class ) {
		    Array.setShort(callArray,arg,((Number)obj).shortValue());
		    return true;
		}
		if( to==byte.class ) {
		    Array.setByte(callArray,arg,((Number)obj).byteValue());
		    return true;
		}
		if( to==char.class ) {
		    Array.setChar(callArray,arg,new Character((char)((Number)obj).intValue()).charValue());
		    return true;
		}
		return false;
	    }
	    if( obj instanceof String ) {
		if( to==char.class ) {
		    Array.setChar(callArray,arg,((String)obj).charAt(0));
		    return true;
		}
		Float cvfloat = null;
		try {
		    cvfloat = new Float((String)obj);
		} catch( Exception e ) {
		    return false;
		}
		if( to==float.class ) {
		    Array.setFloat(callArray,arg,cvfloat.floatValue());
		    return true;
		}
		if( to==int.class ) {
		    Array.setInt(callArray,arg,cvfloat.intValue());
		    return true;
		}
		if( to==long.class ) {
		    Array.setLong(callArray,arg,cvfloat.longValue());
		    return true;
		}
		if( to==double.class ) {
		    Array.setDouble(callArray,arg,cvfloat.doubleValue());
		    return true;
		}
		if( to==short.class ) {
		    Array.setShort(callArray,arg,cvfloat.shortValue());
		    return true;
		}
		if( to==byte.class ) {
		    Array.setByte(callArray,arg,cvfloat.byteValue());
		    return true;
		}
		return false;
	    }
	}
	return false;
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


