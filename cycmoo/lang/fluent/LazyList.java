package cycmoo.lang;  //tarau.jinni;
/**
  Lazy List: produces Cons-like sequences, based on a SourceFluent.  Saving a lazy list to the database does not make too much sense  as it will be discarded when backtracking over its creation point.  Note that a Lazy List has its own trail, and is only discarded  when backtracking over its creation point.
*/
public class LazyList extends Cons {
    public LazyList(ITerm head, IFluent source,TermStack trail) {
        super(head,new PrologVar());
        this.source=(SourceFluent)source;
        this.bound=false;
        this.trail=trail;
    }

    private SourceFluent source;
    private boolean bound;
    private TermStack trail;

    /**
     * advances the Lazy List, pulling out elements of the SourceFluent
     * as needed
     */
    private final void advance() {
        if ( bound ) return;
        ITerm nextHead=source.getElement();
        Atom thisTail;
        if ( null==nextHead ) {
            thisTail=getNull();
        } else {
            thisTail=new LazyList(nextHead.copy(),source,trail);
        }
        ((Var)getDrefArg(1)).unify(thisTail,trail);
        bound=true;
    }

    /** 
     * Advances the tail of a lazy list.
     * Note that they inherit getHead() from Cons.
     */

    public ITerm getTail() {
        advance();
        return super.getTail();
    }

    /**
     * this permissive definition for bind_to allows a Lazy
     * List to Unify with any 2 arg constructor chain
     */
    public boolean bind_to(ITerm that,TermStack trail) {
        return that instanceof Compound && 2==that.getArity();
    }

    public Atom getNull() {
        return (Atom)Atom.aNil;
    }

    public void undo() {
        //if(source.getPersistent()) return;
        trail.unwind(0);
        source.stop();
        source=null;
    }
}
