package cycmoo.lang;  //tarau.jinni; 
import java.util.*;

/**
 * For a given clause g= A0:-<Guard>,A1,A2...,An, used as resolvent
 * iterates over its possible unfoldings (LD-resolution steps)
 * with clauses of the form B0:-B1,...,Bm in the default database.
 * For each such step, a new clause (A0:-B1,...,Bm,A2...,An)mgu(A1,B0)
 * is built and returned by the UnfoldingSourceFluent's getElement method.
 * Before the actual unfolding operations, builtins in Guard are executed,
 * possibly providing bindings for some variables or failing.
 * In case of failure of Guard or of unification, getElement() returns null.
 */
public class UnfoldingSourceFluent extends SourceFluent {
    private int oldtop;
    private Enumeration e;
    private HornClause goal;
    private IKernel prog;

    /**
     * Creates an UnfoldingSourceFluent based on goal clause g for resolution
     * step in program p
     */
    public UnfoldingSourceFluent(HornClause g,IKernel p) {
        super(p);
        this.goal=g;
        this.prog=p;
        this.e=null;
        trace_goal(g);
        reduceProceedures();
        if ( null!=goal ) {
            ITerm first=goal.getFirst();
            if ( null!=first ) {
                oldtop=prog.getTrail().size();
                this.e=p.getBlackboard().toEnumerationFor(first.getKey());
                if ( !e.hasMoreElements() )
                    trace_nomatch(first);
            }
        } else
            trace_failing(g);
    }

    /**
     * Overrides default trailing by empty action
     */
    public void trailMe(IKernel p) {
        //IO.mes("not trailing"+this);
    };

    /**
     * Extracts an answer at the end of an AND-derivation
     */
    HornClause getAnswer() {
        if ( null!=goal && goal.getBody() instanceof SuccesfullProceedure ) return goal.ccopy();
        else return null;
    }

    /**
     * Checks if this clause is the last one, allowing LCO
     */
    final boolean notLastClause() {
        return(null!=e) && e.hasMoreElements();
    }

    /**
     * Reduces builtin functions after the neck of a clause, before a "real" atom is unfolded
     */
    final void reduceProceedures() {
        for ( ;; ) {
            ITerm first=goal.getFirst();
            if ( null==first ) break; // cannot reduce further
            if ( first instanceof Conj ) {  // advances to next (possibly) inline builtin
                goal=new HornClause(goal.getHead(),HornClause.appendConj(first,goal.getRest()));
                first=goal.getFirst();
            }

            int ok=first.exec(prog); // (possibly) executes builtin

            switch ( ok ) {
            
            case -1: // nothing to do, this is not a builtin
                break; 

            case 1: // builtin suceeds
                //IO.mes("advancing: "+goal);
                goal=new HornClause(goal.getHead(),goal.getRest());
                continue; // advance

            case 0: // builtin fails
                goal=null;
                break; // get out

            default: // unexpected code: programming error
                IO.errmes("Bad return code:"+ok+") in builtin: "+first);
                goal=null;
                break;
            } 
            //IO.mes("leaving reduceProceedures: "+goal);
            break; // leaves loop
        }
    }

    /**
     * Returns a new clause by unfolding the goal with a matching
     * clause in the database, or null if no such clause exists.
     */
    public ITerm getElement() {
        if ( null==e ) return null;
        HornClause unfolded_goal=null;
        while ( e.hasMoreElements() ) {
            ITerm T=(ITerm)e.nextElement();
            if ( !(T instanceof HornClause) ) continue;
            // resolution step, over goal/resolvent of the form: 
            // Answer:-G1,G2,...,Gn.
            prog.getTrail().unwind(oldtop);
            unfolded_goal=T.toClause().unfold_with_goal(goal,prog.getTrail());
            if ( null!=unfolded_goal ) break;
        }
        return unfolded_goal;
    }

    /**
     * Stops production of more alternatives by setting the clause enumerator to null
     */
    public void stop() {
        e=null;
    }

    /**
     * Tracer on entering g
     */
    final void trace_goal(HornClause g) {
        switch ( KernelAgent.tracing ) {
        case 2:
            IO.println(">>>: "+g.getFirst());
            break;
        case 3:
            IO.println(">>>: "+g.pprint());
            break;
        }
    }

    /**
     * Tracer on exiting g
     */
    final void trace_failing(HornClause g) {
        switch ( KernelAgent.tracing ) {
        case 2:
            IO.println("FAILING CALL IN<<<: "+g.getFirst());
            break;
        case 3:
            IO.println("FAILING CALL IN<<<: "+g.pprint());
            break;
        }
    }

    /**
     * Tracer for undefined predicates
     */
    final void trace_nomatch(ITerm first) {
        if ( KernelAgent.tracing>0 ) {
            IO.println("*** UNDEFINED CALL: "+first.pprint());
        }
    }

    /**
     * Returns a string representation of this unfolder, based
     * on the original clause it is based on.
     */
    /*
    public String toString() {
        return(null==goal)?"{UnfoldingSourceFluent}":"{UnfoldingSourceFluent=> "+goal.pprint()+"}";
    }
    */
}
