package cycmoo.lang;  //tarau.jinni;

import java.util.Hashtable;
/**
 * A FunctionObject is a Jinni Nonvar with system assigned name
 * 
 */
public abstract class FunctionObject extends Nonvar {

    public FunctionObject() {
       // registerObject();
    }
    
    public ITerm registerObject() {
        if (sym==null) setSym(getName(this));
        KernelAgent.objectPut(name(),this);
        return this;
    }

    final public void finalize() {
     //  IO.mes("% collect " + name());
        KernelAgent.objectRemove(name());
    }

    public final String name() {
        return qnamed;
    }

    public int getArity() {
        return Term.VAR;
    }

    public boolean bind_to(ITerm that,TermStack trail) {
        return super.bind_to(that,trail);// && ordinal==((FunctionObject)that).ordinal;
    }

    public final String toString() {
        return name();
    }

    public ITerm token() {
        return this;
    }

}
