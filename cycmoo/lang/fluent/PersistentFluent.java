package cycmoo.lang;  
import java.util.Enumeration;
import java.util.Vector;

/**
 * A Fluent is a Agent Object which has its own state, subject to changes over time.
 * 
 */
abstract public class PersistentFluent extends Fluent implements IFluent, Cloneable {

    public boolean persistent=true;   
    public PersistentFluent() {
        super(null);
    }
    public PersistentFluent(IKernel as) {
        super(as);
    }


}

