package cycmoo.lang.fluent;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;

/**
 * A Fluent is a Jinni Object which has its own state, subject to changes over time.
 * 
 */
abstract public class Fluent extends FunctionObject {
    public IKernel parent;
    //public Blackboard blackboard;
    public Fluent(IKernel p) {
        parent=p;
        //blackboard = p.getBlackboard();
        trailMe(p);

    }

    private boolean persistent=false;

    /**
     * Dynamically sets the persistence status of this Fluent.
     * A persistent Fluent will not have its stop
     * method outomatically called upon backtracking.
     * A typical example would be a file or socket handle
     * saved to the database to be reused after backtracking.
     */
    public void setPersistent(boolean persistent) {
        this.persistent=persistent;
    }

    /**
     *  returns true if this Fluent is persistent, false otherwise
     */
    public boolean getPersistent() {
        return this.persistent;
    }

    /**
     * Adds this Fluent to the parent Solver's trail, which
     * will eventually call the undo method of the Fluent on
     * backtracking.
     */
    public void trailMe(IKernel p) {
        if ( null!=p ) p.getTrail().push(this);
    }

    public void stop() {
    }

    /**
     * applies a non-persistent Fluent's stop() method on backtracking
     */
    public void undo() {
        if ( !persistent ) stop();
    }

    public int putElement(ITerm e){
        return 0;
    }
    public ITerm getElement(){
        return null;
    }
    public ITerm collect() {
      return null;
    }
}
