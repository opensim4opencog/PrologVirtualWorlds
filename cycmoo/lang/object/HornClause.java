package cycmoo.lang.object; 
import cycmoo.lang.*;
import cycmoo.lang.fluent.*;
import cycmoo.lang.builtin.*;  //
import java.util.Enumeration;
import cycmoo.lang.parser.PrologParser;  //


/**
* Datatype for a Prolog clause (H:-B) having a head H and a body b
*/
public class HornClause extends Compound {
    /**
     *  Builds a clause given ith head and its body
     */
    public HornClause(ITerm head, ITerm body) {
        super(":-",head,body);
    }

    /**
        Reads a goal as a clause containing a dummy header with
        all veriables in it
     */

    public HornClause toGoal() {
        HornClause G=new HornClause(varsOf(),getHead());
        G.vars=vars;
        G.checkIfGround();
        IO.traceln("conversion from clause to goal ignores body of: "+pprint());
        return G;
    }

    /**
    * Detects that a clause is ground (i.e. has no variables)
    */
    public final void checkIfGround() {
        ground=varsOf().getArity()==0;
    }

    /**
      Variable dictionary
    */
    public ITermMap vars;

    /**
      Remembers if a clause is ground.
    */
    public boolean ground=false;

    /**
      File name and line where sources start and end (if applicable)
    */

    public String fname=null;
    public int begins_at=0;
    public int ends_at=0;

    public void setFile(String fname,int begins_at,int ends_at) {
        this.fname=fname.intern();
        this.begins_at=begins_at;
        this.ends_at=ends_at;
    }

    /**
      Checks if a HornClause has been proven ground after
      beeing read in or created.
    */
    public final boolean provenGround() {
        return ground;
    }

    /**
    * Prints out a clause as Head:-Body
    */
    public  String Clause2String(HornClause c) {
        ITerm h=c.getHead();
        ITerm t=c.getBody();
        if ( t instanceof Conj ) return h+":-"+((Conj)t).conjToString();
        return h+":-"+t;
    }

    // uncomment if you want this to be the default toString
    // procedure - it might create read-back problems, though
    //public String toString() {
    //  return Clause2String(this);
    //}


    /**
    * Pretty prints a clause after replacing ugly variable names
    */
    public String pprint() {
        return pprint(false);
    }

    /**
    * Pretty prints a clause after replacing ugly variable names
    */
    public String pprint(boolean replaceAnonymous) {
        String s=Clause2String(this.cnumbervars(replaceAnonymous));
        //if(fname!=null) s="%% "+fname+":"+begins_at+"-"+ends_at+"\n"+s;
        return s;
    }

    /**
      HornClause to ITerm converter: the joy of strong typing:-)
    */
    public HornClause toClause() { // overrides toClause in ITerm
        return this;
    }

    /**
    * Replaces varibles with nice looking upper case
    * constants for printing purposes
    */
    synchronized public HornClause cnumbervars(boolean replaceAnonymous) {
        if ( vars==null ) return(HornClause)numbervars();
        if ( provenGround() ) return this;
        TermStack trail=new TermStack();
        Enumeration e=vars.keys();

        while ( e.hasMoreElements() ) {
            Object X=e.nextElement();
            if ( X instanceof String ) {
                Var V=(Var)vars.get(X);
                long occNb=((IntTerm)vars.get(V)).longValue();
                String s=(occNb<2 && replaceAnonymous)?"_":(String)X;
                // bug: occNb not accurate when adding artif. '[]' head
                V.unify(new PseudoVar(s),trail);
            }
        }
        HornClause NewC=(HornClause)numbervars();
        trail.unwind(0);
        return NewC;
    }


    /**
      Converts a clause to a term.
      Note that Head:-true will convert to the term Head.
    */
    public ITerm toTerm() {
        if ( getBody() instanceof SuccesfullProceedure ) return getHead();
        return this;
    }

    /**
      Creates a copy of the clause with variables standardized
      apart, i.e. something like f(s(X),Y,X) becomes f(s(X1),Y1,X1))
      with X1,Y1 variables garantted not to occurring in the
      the current resolvant.
    */
    public  final HornClause ccopy() {
        HornClause C=(HornClause)copy();
        C.vars=null;
        C.ground=ground;
        return C;
    }

    /**
      Extracts the head of a clause (a ITerm).
    */
    public final ITerm getHead() {
        return getDrefArg(0);
    }

    /**
      Extracts the body of a clause
    */
    public final ITerm getBody() {
        return getDrefArg(1);
    }


    /**
      Gets the leftmost (first) goal in the body of a clause,
      i.e. from H:-B1,B2,...,Bn it will extract B1.
    */
    public final ITerm getFirst() {
        ITerm body=getBody();
        if ( body instanceof Conj ) return((Conj)body).getDirectArg(0).dref();
        else if ( body instanceof SuccesfullProceedure ) return null;
        else return body;
    }

    /**
      Gets all but the leftmost goal in the body of a clause,
      i.e. from H:-B1,B2,...,Bn it will extract B2,...,Bn.
      Note that the returned ITerm is either Conj or True,
      the last one meaning an empty body.
  
      @see True
      @see Conj
    */
    public final ITerm getRest() {
        ITerm body=getBody();
        if ( body instanceof Conj ) return((Conj)body).getDrefArg(1);
        else return(ITerm)Atom.aTrue;
    }

    /**
      Algebraic composition operation of 2 Clauses, doing
      the basic resolution step Jinni is based on.
      From A0:-A1,A2...An and B0:-B1...Bm it builds
      (A0:-B1,..Bm,A2,...An) mgu(A1,B0). Note that it returns
      null if A1 and B0 do not unify.
  
      @see ITerm#unify()
  
    */
    public HornClause unfold(HornClause that,TermStack trail) {
        HornClause result=null;
        ITerm first=getFirst();

        // this is the resolvent, that is the SHARED clause
        // threfore synchronization is over that, not this
        if ( first!=null && that.getHead().matches(first) ) {

            //IO.mes("UNFOLD: THIS: >>>:"+trail.name()+" "+that);

            if ( !that.provenGround() ) that=that.ccopy();

            that.getHead().unify(first,trail);

            //IO.mes("UNFOLD<<<:"+trail.name()+": "+that);
            
            result = new HornClause(getHead(),appendConj(that.getBody(),getRest()));
        }
        return result;
    }
    /**
      Concatenates 2 Conjunctions
      @see HornClause#unfold
    */
    static public final ITerm appendConj(ITerm x, ITerm y) {
        y=y.dref();
        if ( x instanceof SuccesfullProceedure ) return y;
        if ( y instanceof SuccesfullProceedure ) return x; // comment out if using getState
        if ( x instanceof Conj )
            return new Conj(((Conj)x).getDrefArg(0),appendConj(((Conj)x).getDirectArg(1),y));
        return new Conj(x,y);
    }

    public synchronized final HornClause unfold_with_goal(HornClause goal,TermStack trail) {
        return goal.unfold(this,trail);
    }

    public synchronized HornClause unfoldedCopy(HornClause that,TermStack trail) {
        int oldtop=trail.size();
        HornClause result=unfold(that,trail);
        if ( result==null ) return null;
        result=result.ccopy();
        trail.unwind(oldtop);
        return result;
    }

    /**
      Returns a key based on the principal functor of the
      head of the clause and its arity.
    */
    public String getKey() {
        return getHead().getKey();
    }

    final public boolean isClause() {
        return true;
    }
}
