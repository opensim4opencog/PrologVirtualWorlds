package cycmoo.lang;  //tarau.jinni;
import java.util.*;
import java.io.Reader;
/**
  Implements a Executable Proceedure Interface 
  
*/
public interface IFluent extends ITerm {
    /**
     * Dynamically sets the persistence status of this Fluent.
     * A persistent Fluent will not have its stop
     * method outomatically called upon backtracking.
     * A typical example would be a file or socket handle
     * saved to the database to be reused after backtracking.
     */
    public void setPersistent(boolean persistent);
    /**
     *  returns true if this Fluent is persistent, false otherwise
     */
    public boolean getPersistent();

    /**
     * Adds this Fluent to the parent Solver's trail, which
     * will eventually call the undo method of the Fluent on
     * backtracking.
     */
    public void trailMe(IKernel p); 

    public void stop();
        
    /**
     * applies a non-persistent Fluent's stop() method on backtracking
     */
    public void undo();
    public ITerm getElement();
    public int putElement(ITerm e);
    public ITerm collect();

}


