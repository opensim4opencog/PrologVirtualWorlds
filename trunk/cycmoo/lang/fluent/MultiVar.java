package cycmoo.lang.fluent;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;
import java.util.Stack;

/**
 * Varable-like entity, with a multiple values, in stack order.
 * Set operations are undone on backtraking, when the previous
 * value is restored.
 */
public class MultiVar extends Fluent {
    Stack vals;

    public MultiVar(ITerm T,IKernel p) {
        super(p);
        vals=new Stack();
        vals.push(T.dref());
    }

    final public void set(ITerm T,IKernel p) {
        vals.push(T);
        p.getTrail().push(this);
    }

    public ITerm val() {
        return(ITerm)vals.peek();
    }

    /** 
     * cannot be made presistent    */
    public void undo() {
        vals.pop();
    }

  /*
    public String toString() {
        return "MultiVar["+vals.size()+"]->{"+vals.peek().toString()+"}";
    }
    */
}

