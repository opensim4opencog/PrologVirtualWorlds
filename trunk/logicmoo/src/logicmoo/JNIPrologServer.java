package logicmoo;
import java.util.Hashtable;
import java.lang.String;
import jpl.*;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;
import java.lang.*;


public class JNIPrologServer extends Thread {


    /* Creates a non-started Server */
    public  void JNIPrologServer() {
    }

    /* Creates a non-started Server with a prolog @sourceFile */
    public void JNIPrologServer(String sourceFile) {
        bootFile=sourceFile;
    }

    /* Creates a @started Server with a prolog @sourceFile  */
    public void JNIPrologServer(String sourceFile, boolean started) {
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

        if ( bootFile!=null )
            if ( bootFile.equals("test.pl") ) testJava();
    }

    /* Starts an instance of a PrologServer running as thread*/
    public void run() {
        runStatic();
    }

    /* Starts an instance of a PrologServer running as static interface class*/
    public static void runStatic() {
        if ( !m_isInitialized ) {
            initAll();
            setCwd(cwd);
            consult(bootFile);
        }
    }

    /* Ensures Prolog VM is running and Scripting Engine is running*/
    public synchronized static void initAll() {
        JPL.init();
        JNIPrologServer.createState();
        m_isInitialized = true;
    }


    /* Ensures Scripting Engine is running*/
    public synchronized static void createState() {
        if ( allObjects!=null ) return;
        stringClass = (new String()).getClass();

        try {
            stringClassArrayOfOne = (Class[])(Array.newInstance(Class.forName("java.lang.Class"),1));
        } catch ( Exception e ) {
            System.out.println("invokeConstructor: " + e);
            e.printStackTrace(System.out);
            System.exit(2);
        }

        Array.set(stringClassArrayOfOne,0,stringClass);
        debug=1;
        allObjects = new HashMap();
        allObjects.put("object('JNIPrologServer')",new JNIPrologServer());                
    }

    /* Scripting Engine is running = true*/
    public synchronized boolean isStarted() {
        return m_isInitialized;
    }

    /* Loads a Prolog sourceFile to running server*/
    public synchronized static boolean consult( String sourceFile) {
        if ( sourceFile==null ) return false;
        return invokePrologN1("consult",sourceFile);
    }

    /* This is invoked only on a running PrologServer to change it's local directory */
    public synchronized static boolean setCwd( String pathame) {
        cwd = pathame;
        return invokePrologN1("cd",cwd);
    }

    /* This is an easy to use method(param) inovation to the Prolog kernel */
    public static boolean invokePrologN1(String method, String param) {
        jpl.Term consult_arg[] = { new jpl.Atom(param)};
        jpl.Query consult_query = new jpl.Query(method, consult_arg );
        return consult_query.query();
    }


    /* Runs diagnostics on Prolog kernel (test.pl must be in path) */
    public synchronized static boolean testJava() {

        java.util.Hashtable solution=null;
        jpl.Query query = null;
        jpl.Variable X = null;

        if ( consult("test.pl") ) {

            jpl.Term args1[] = { new jpl.Atom( "joe" ),new jpl.Atom( "ralf" )};
            query = new jpl.Query("child_of", args1 );

            System.out.println( "child_of(joe, ralf) = " + query.query() );

            jpl.Term args2[] = { new jpl.Atom( "steve" ),new jpl.Atom( "ralf" )};
            query = new jpl.Query("descendent_of", args2 );

            System.out.println( "descendent_of(steve, ralf) = " + query.query() );

            X = new jpl.Variable();
            jpl.Term args3[] = { X, new jpl.Atom( "ralf" )};
            query = new jpl.Query( "descendent_of", args3 );

            System.out.println( "querying descendent_of( X, ralf )" );
            solution = query.oneSolution();
            System.out.println( "X = " + solution.get( X ) );

            X = new jpl.Variable();
            jpl.Term args4[] = { X,new jpl.Atom( "ralf" )};
            query = new jpl.Query( "descendent_of", args4 );

            System.out.println( "querying descendent_of( X, ralf )" );

            while ( query.hasMoreSolutions() ) {
                solution = query.nextSolution();
                System.out.println( "X = " + solution.get( X ) );
            }
        }

        System.out.println( "querying intospection" );
        X = new jpl.Variable();
        jpl.Term args5[] = 
        {
            new jpl.Compound("object",new jpl.Atom("JNIPrologServer")),new jpl.Atom( "getClassDef" ),new jpl.List(new jpl.Atom("Class"),new jpl.List.Nil()),X 
        };
        query = new jpl.Query("java_invoke_object", args5 );
        solution = query.oneSolution();
        System.out.println( "X = " + solution.get( X ) );

        return true;
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
        String intKey = "" + innerInstance.hashCode();
        if ( allObjects.get(intKey) !=null ) return;
        allObjects.put(intKey,innerInstance);
        allObjects.put(nameForInstance(innerInstance),innerInstance);
        allObjects.put(innerInstance.toString(),innerInstance);
    }

    public synchronized static void delInstance(Object innerInstance) {
        String intKey = "" + innerInstance.hashCode();
        if ( allObjects.get(intKey) ==null ) return;
        allObjects.remove(innerInstance.toString());
        allObjects.remove(nameForInstance(innerInstance));
        allObjects.remove(intKey);
    }

    /* Creates a new Object for a className */
    public synchronized static Object createGlobal(String className) throws Exception {
        Class innerClass = findClass(className);
        Object innerInstance = innerClass.newInstance();
        addInstance(innerInstance);
        return innerInstance;
    }

    public synchronized static String invokeObject(String objectName,String methodName,int params, String[] args) {
        if ( allObjects==null ) createState();
        if ( debug>1 ) System.err.println("invokeObject("+objectName +","+ methodName +","+ params + ")");
        Object innerInstance = findInstance(objectName);
        if ( innerInstance==null )      return makeError(new Exception("Object not found in catalog \"" + objectName +"\""));
        if ( debug>1 ) System.err.println("Found Instance: \"" + innerInstance +"\"" );
        try {
            return objectToProlog(invokeObject(innerInstance,methodName,argsToObjectVector(args,params)));
        } catch ( Exception e ) {
            return makeError(e);
        }
    }

    public synchronized static Object invokeObject(Object innerInstance,String methodName, Object[] args) 
    throws Exception {

        // Get/Set Fields
        if ( methodName.startsWith("$T") ) {
            Field innerField;
            if ( methodName.charAt(2)=='s' ) {
                innerField = getFieldForObject(innerInstance,methodName.substring(6));
                innerField.set(innerInstance,args[0]);
                return innerField.get(innerInstance);
            }
            if ( methodName.charAt(2)=='g' ) {
                innerField = getFieldForObject(innerInstance,methodName.substring(6));
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
        if ( obj instanceof Class )     return(Class)obj;
        return obj.getClass();
    }

    public synchronized static Object[] argsToObjectVector(String[] args, int len) {
        Object[] toReturnObjects=null;
        try {
            toReturnObjects = (Object[])Array.newInstance(findClass("Object"),len);
        } catch ( Exception e ) {
            fatalEvent(e);
        }

        for ( int i = 0 ; i < len; i++ ) {
            try {
                Object obj = argToObject(args[i]);
                if ( debug >1 ) System.err.println("Arg" + i + "=\"" + args[i] + "\" -> " + obj.getClass() + ":" + obj.toString());
                Array.set(toReturnObjects,i,obj);
            } catch ( Exception e ) {
                System.err.println("Arg" + i + "=\"" + args[i] + "\" -> " +e);
                e.printStackTrace(System.err);
            }
        }
        return toReturnObjects;
    }

    public synchronized static String objectToProlog(Object obj ) {
        if ( obj==null ) return "void";
        if ( obj.getClass().isPrimitive() )     return obj.toString();
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
        if ( theType.equals("String") ) return theData;
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
            return "object("  + classToVector(obj.getClass())  + ":"  + obj.hashCode()+ ")";
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

        if ( debug>1 ) System.out.println("searching for= " + instanceName);
        //$Tobject,JNIPrologServer
        try {
            if ( instanceName.equals("System") ) return java.lang.System.class;
        } catch ( Exception e ) {
            warnEvent(e);
            return null;
        }
        if ( instanceName.charAt(0)=='$' ) {
            int comma=instanceName.indexOf(',');
            switch ( instanceName.charAt(1) ) {
                case 'T':
                    return findInstance("object('"+instanceName.substring(comma+1)+"')");
                case 'i':
                    return allObjects.get(instanceName.substring(2));
                case 'l':
                    return allObjects.get(instanceName.substring(2));
            }
        }
        try {
            Object objRef = allObjects.get(instanceName);
            if ( debug>1 ) {
                System.err.println("findInstance("+instanceName+")==" + objRef + ".");
            }
            return objRef; 
        } catch ( Exception e ) {
            fatalEvent(e );
        }
        return null;
    }

    /* Return public synchronized instance name (HashCode)*/
    public synchronized static String nameForInstance(Object innerInstance) {
        return "object(" + toScriptingName(innerInstance.getClass().getName()) +":" + innerInstance.hashCode() + ")";
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

    public synchronized static Object argToObject(String arg) {
        if ( arg.startsWith("object(") ) return findInstance(arg);
        if ( arg.startsWith("type(") ) return mktype(arg);
        if ( arg.length()==0 ) return "";
        if ( arg.charAt(0)!='$' ) return arg;
        char fc = arg.charAt(1);
        switch ( fc ) {
            case 'x':
                if ( arg.charAt(2)=='t' ) return new Boolean(true);
                else return new Boolean(false); 
            case '$':
                return "";
            case 'i':
                try {
                    return new java.lang.Integer(arg.substring(2));
                } catch ( Exception e ) {
                    warnEvent(e);
                    return new java.lang.Integer(0);
                }
            case 'f':
                try {
                    return new java.lang.Float(arg.substring(2));
                } catch ( Exception e ) {
                    warnEvent(e);
                    return new java.lang.Float(0);
                }
            case 'u':
                return arg.substring(2);
            case 'l':
                return arg.substring(2);
            case 'T':
                if ( arg.startsWith("$Tobject") ) return findInstance(arg);
                int comma = arg.indexOf(',');
                try {
                    return mktype(arg.substring(2,comma),arg.substring(comma+1));
                } catch ( Exception e ) {
                    warnEvent(e);
                    return ""+e;
                }
        }
        return arg;
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
        if ( someName == null ) return "null";
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




