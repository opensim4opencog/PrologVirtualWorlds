package cycmoo.lang;  
import java.util.*;
import java.io.*;
/**
  All blackboard Objects implement Term.
*/
public interface ITerm extends IProceedure {
    
    // Retrieves the Parent as a TermMap
    //public TermMap getParentLex();

    // Retrieves the Parent as a TermMap
    //public TermMap getParentBB();

    /** 
      returns or fakes an arity for all subtypes 
    */
    public String toString();
    public ITerm dref();
    public boolean bind_to(ITerm that,TermStack trail);
    /** Unify dereferenced */
    public boolean unify_to(ITerm that,TermStack trail);
    /** Dereference and unify_to */
    public boolean unify(ITerm that,TermStack trail);
    public void undo();
    public boolean eq(ITerm that);
    public ITerm token();
    public ITerm toTerm();
    public HornClause toClause();
    public boolean isClause();

    /**
      Tests if this term unifies with that.
      Bindings are trailed and undone after the test.
      This should be used with the shared term as this
      and the new term as that. Synchronization makes
      sure that side effects on the shared term are
      not interfering, i.e as in:
         SHARED.matches(NONSHARED,trail).
  
    */
    public boolean matches(ITerm that);

    /**
      Returns a copy of the result if the unification
      of this and that. Side effects on this and that
      are undone using trailing of bindings..
      Synchronization happens over this, not over that.
      Make sure it is used as SHARED.matching_copy(NONSHARED,trail).
    */
    public ITerm matching_copy(ITerm that);

    /**
      Defines the reaction to an agent recursing
      over the structure of a term. <b>This</b> is passed
      to the agent and the result of the action is returned.
      Through overriding, for instance, a Compound term will provide
      the recursion over its arguments, by applying the action
      to each of them.
      
      @see Compound
    */
    public ITerm reaction(ITerm agent);

    /**
       Identity action.
    */
    public ITerm action(ITerm that);

    /**
       Returns a copy of a term with variables
       standardized apart (`fresh variables').
    */
    public ITerm copy(); 

    /**
      Returns '[]'(V1,V2,..Vn) where Vi is a variable
      occuring in this ITerm
    */
    public ITerm varsOf();

    /**
      Replaces variables with uppercase constants named 
      `V1', 'V2', etc. to be read back as variables.
    */
    public ITerm numbervars();

    /**
       Prints out a term to a String with
       variables named in order V1, V2,....
    */
    public String pprint();

    public String pprint(boolean replaceAnonymous);

    /*
      Returns an unquoted version of toString()
    */ 
    public String toUnquoted();

    /**
       Returns a string key used based on the string name
       of the term. Note that the key for a clause AL-B,C. 
       is the key insted of ':-'.
    */
    public String getKey();

    /** 
      Java Object wrapper. In particular, it is
      used to wrap a Thread to hide it inside a Prolog
      data object.
    */
    public Object toObject();

    public Nonvar toChars();

}
