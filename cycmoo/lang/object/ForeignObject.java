package cycmoo.lang.object;  //
import cycmoo.lang.*;
import cycmoo.lang.fluent.*;
import cycmoo.lang.builtin.*;  //
import java.lang.reflect.*;
import java.util.*;
import java.lang.*;
/**
 * A ForeignObject is a Jinni FunctionObject with a stubbed slot 
 * which containing a wrapped Java object
 */

public class ForeignObject extends FunctionObject implements InvocationHandler {

    final static public Class  baseinterface = ITerm.class;
    static public ITerm getRef(Object obj) {
        if ( obj==null ) return Atom.aNo;
        if (obj instanceof Integer) return new IntTerm(((Integer)obj).longValue());
        if (obj instanceof Number) return new RealNumTerm(((Number)obj).doubleValue());
        return getStub(obj);
    }
    static public ITerm getStub(Object obj) {
        ITerm theStub = null;
        if ( obj==null ) return Atom.aNo;
        if ( obj instanceof String ) {
            theStub=KernelAgent.objectGet((String)obj);
            if ( theStub!=null ) return theStub;
            return TermMap.useStringAtom((String)obj);
        }
        if ( obj instanceof Nonvar ||  obj instanceof ITerm ) return(ITerm)obj;
        String loc = KernelAgent.objectKey(obj);
        if ( loc!=null ) return KernelAgent.objectGet(loc);
        Class[] pclasses = obj.getClass().getClasses();
        Class[] classes = new Class[pclasses.length+1];
        for ( int i=0;i<pclasses.length;i++ ) classes[i]=pclasses[i];
        classes[pclasses.length]=baseinterface;
        try {
            Class proxyClass = Proxy.getProxyClass(baseinterface.getClassLoader(), classes);
            Constructor constructure = proxyClass.getConstructor(new Class[] {InvocationHandler.class});
            theStub = (ITerm)constructure.newInstance(new Object[] {new ForeignObject(obj,new ArrayList(Arrays.asList(pclasses)))});
            KernelAgent.objectPut(theStub.name(),theStub);
            return theStub;
        } catch ( Exception e ) {
            e.printStackTrace();
            return Atom.aNo;
        }
    }       // get_default_db(X),jtype(X,Y),jtype(Y,Z),jtype(Z,A).


    // The object thats wrapped
    Object stubbed;
    // The object class before wrapped
    Class classstubbed;
    // The object classes before wrapped
    private List prevclasses=null;
    // The object is not in critical code
    private boolean available=true;

    private ForeignObject(Object obj, List classesImplemented) {
        super();
        this.prevclasses=classesImplemented;
        this.stubbed=obj;
        this.classstubbed = stubbed.getClass();
        setSym(getName(stubbed));
    }

    public Object toObject() {
        return this;
    }
    public Object toUnstubbed() {
        return stubbed;
    }
    public ITerm token() {
        return this;
    }
    //  public Class getClass() {
    //      return classstubbed;
    //  }

    public java.lang.Object invoke(java.lang.Object A, java.lang.reflect.Method B, java.lang.Object[] C) throws Throwable {
        try {
            if ( prevclasses.contains(B.getDeclaringClass()) ) {
                IO.mes("invoke directly: \"" +B.getName()+ "\" on "+ name() +" as " + B.getDeclaringClass().getName() +" with " +C);
                return B.invoke(stubbed,C);
            }
            IO.mes("invoked wrapper: \"" +B.getName()+ "\" on "+ name() +" as " + B.getDeclaringClass().getName()+" with " +C);
            return B.invoke(this,C);
        } catch ( Exception e ) {
            e.printStackTrace();
        }
        return null;
    }


    synchronized public void suspend() {
        available=false;
        while ( !available ) {
            try {
                wait();
            } catch ( InterruptedException e ) {
            }
        }
    }

    synchronized public void resume() {
        available=true;
        notifyAll();
    }
}

