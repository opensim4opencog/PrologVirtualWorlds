package cycmoo.lang;  

import java.util.Stack;
import java.util.Enumeration;
import java.util.Vector;

/**
   Implements a stack of undo actions
   for backtracking, and in particular,
   resetting a Var's val fiels to unbound (i.e. this).

   @see IKernel
   @see HornClause
   @see ITerm
   @see Var
*/
public class TermStack extends PersistentFluent {
    private Stack stk;
    public TermStack() {
        super(null);
        stk=new Stack();
    } 
    public TermStack(IKernel p) {
        super(p);
        stk=new Stack();
    }
    public Enumeration elements(){
        return stk.elements();
    }

    public int putElement(ITerm T){
        stk.push(T);
        return 1;
    }

    public ITerm getElement(){
        return (ITerm)stk.pop();
    }

    public java.lang.Object push(java.lang.Object A) {
        return stk.push(A);
    }
    // pre-synchronized 
    public int search(java.lang.Object A) {
        return stk.search(A);
    }
    // pre-synchronized 
    public java.lang.Object pop() {
        return stk.pop();
    }
    // pre-synchronized 
    public java.lang.Object peek() {
        return stk.peek();
    }
    // pre-synchronized 
    public boolean empty() {
        return stk.empty();
    }
    public int size() {
        return stk.size();
    }
    public boolean isEmpty() {
        return stk.isEmpty();
    }
    /*
        public String pprint() {
            return name()+"\n"+super.toString()+"\n";
        }
    */
    /**
      Used to undo bindings after unification,
      if we intend to leave no side effects.
    */

    synchronized final public void unwind(int to) {
        //KernelAgent.masterio.mes("unwind TRAIL: "+name()+": "+size()+"=>"+to);
        //if(to>size()) KernelAgent.masterio.assertion("unwind attempted from smaller to larger top");
        for ( int i=size()-to; i>0; i-- ) {
            ITerm V=(ITerm)(pop());
            V.undo();
        }
    }
    public String stat() {
        return "TermStack="+size();
    }
}
