package cycmoo.lang;  //tarau.jinni;
/**
  Top element of the Prolog term hierarchy.
  Describes a simple or compound ter like:
  X,a,13,f(X,s(X)),[a,s(X),b,c], a:-b,c(X,X),d, etc.
*/
public abstract class Term extends Object implements ITerm, Cloneable {

    public final static int FUNCTION=-1;
    public final static int FLUENT=-5;
    public final static int JAVA=-4;
    public final static int REAL=-3;
    public final static int INT=-2;
    public final static int VAR=-1;
    public final static int CONST=0;

/*
    public Blackboard getBlackboard() {
        return (Blackboard)KernelAgent.currentInstance();
    }
*/
    /** 
      returns or fakes an arity for all subtypes 
    */
    abstract public int getArity();

    /** Dereferences if necessary.
        This should be synchronized
        otherwise vicious non-reentrancy problems
        may occur in the presence of GC and heavy
        multi-threading!!!
    */
    public ITerm dref() {  // synchronized !!!
        return this;
    }

    abstract public boolean bind_to(ITerm that,TermStack trail);

    /** Unify dereferenced */
    abstract public boolean unify_to(ITerm that,TermStack trail);

    /** Dereference and unify_to */
    public final boolean unify(ITerm that,TermStack trail) {
        return dref().unify_to(that.dref(),trail);
    }

    public void undo() { // does nothing
    }

    abstract public boolean eq(ITerm that);

    public ITerm token() {
        return this;
    }

    public ITerm toTerm() {
        return this;
    }

    public HornClause toClause() {
        return new HornClause((ITerm)this,(ITerm)Atom.aTrue);
    }

    public boolean isClause() {
        return false;
    }
/*
    public static ITerm fromString(String s) {
        return HornClause.clauseFromString(s).toTerm();
    }
  */
    /**
      Tests if this term unifies with that.
      Bindings are trailed and undone after the test.
      This should be used with the shared term as this
      and the new term as that. Synchronization makes
      sure that side effects on the shared term are
      not interfering, i.e as in:
         SHARED.matches(NONSHARED,trail).
  
    */
    synchronized public boolean matches(ITerm that) {
        TermStack trail=new TermStack();
        IO.traceln("Curr:>>"+trail.pprint());
        boolean ok=unify(that,trail);
        trail.unwind(0);
        IO.traceln("CurrT<<:"+trail.pprint());
        return ok;
    }

    /**
      Returns a copy of the result if the unification
      of this and that. Side effects on this and that
      are undone using trailing of bindings..
      Synchronization happens over this, not over that.
      Make sure it is used as SHARED.matching_copy(NONSHARED,trail).
    */
    synchronized public ITerm matching_copy(ITerm that) {
        TermStack trail=new TermStack();
        boolean ok=unify(that,trail);
        //if(ok) that=that.copy();
        if ( ok ) that=copy();
        trail.unwind(0);
        return(ok)?that:null;
    }

    /**
      Defines the reaction to an agent recursing
      over the structure of a term. <b>This</b> is passed
      to the agent and the result of the action is returned.
      Through overriding, for instance, a Compound term will provide
      the recursion over its arguments, by applying the action
      to each of them.
      
      @see Compound
    */
    public ITerm reaction(ITerm agent) {
        ITerm T =agent.action(this);
        return T;
    }

    /**
       Identity action.
    */
    public ITerm action(ITerm that) {
        return that;
    }

    /**
       Returns a copy of a term with variables
       standardized apart (`fresh variables').
    */
    synchronized public ITerm copy() {
        return reaction(new TermCopier());
    }

    /**
      Returns '[]'(V1,V2,..Vn) where Vi is a variable
      occuring in this ITerm
    */
    public ITerm varsOf() {
        return(new TermCopier()).getMyVars(this);
    }

    /**
      Replaces variables with uppercase constants named 
      `V1', 'V2', etc. to be read back as variables.
    */
    public ITerm numbervars() {
        return copy().reaction(new VarNumberer());
    }

    /**
       Prints out a term to a String with
       variables named in order V1, V2,....
    */
    public String pprint() {
        return numbervars().toString();
    }

    public String pprint(boolean replaceAnonymous) { // not used
        return pprint();
    }

    /*
      Returns an unquoted version of toString()
    */ 
    public String toUnquoted() {
        return pprint();
    }

    /**
       Returns a string key used based on the string name
       of the term. Note that the key for a clause AL-B,C. 
       is the key insted of ':-'.
    */
    public String getKey() {
        return toString();
    }

    public String qname() {
        return name();
    }

    /** 
      Java Object wrapper. In particular, it is
      used to wrap a Thread to hide it inside a Prolog
      data object.
    */
    public Object toObject() {
        return dref();
    }

    /*
      Just to catch the frequent error when the arg is forgotten while
      definig a builtin. Being final, it will generate a compile time 
      error if this happens
    */
    final public  int exec() {
        return -1;
    }

    /**
      Executed when a builtin is called.
      Needs to be overriden. Returns a run-time
      warning if this is forgotten.
    */
    public int exec(IKernel p) {
        return -1;
    }

    static public final Nonvar stringToChars(String s) {
        if ( 0==s.length() ) return (Nonvar)Atom.aNil;
        Cons l=new Cons(new IntTerm((int)(s.charAt(0))),(ITerm)Atom.aNil);
        Cons curr=l;
        for ( int i=1;i<s.length(); i++ ) {
            Cons tail=new Cons(new IntTerm((int)(s.charAt(i))),(ITerm)Atom.aNil);
            curr.setDirectArg(1,tail);
            curr=tail;
        }
        return l;
    }

    public Nonvar toChars() {
        return stringToChars(toUnquoted());
    }

    /**
      Converts a list of character codes to a String.
    */
    static public String charsToString(Nonvar Cs) {
        StringBuffer s=new StringBuffer("");

        while ( !(Cs instanceof Nil) ) {
            if ( !(Cs instanceof Cons) ) return null;
            Nonvar head=(Nonvar)((Cons)Cs).getDrefArg(0);
            if ( !(head instanceof IntTerm) ) return null;
            char c=(char)((IntTerm)head).val;
            s.append(c);
            Cs=(Nonvar)((Cons)Cs).getDrefArg(1);
        }

        return s.toString();
    }

    public boolean isProceedure() {
        return false;
    }
}
