package cycmoo.lang.object; 
import cycmoo.lang.*;
import cycmoo.lang.fluent.*;
import cycmoo.lang.builtin.*;  //
/**
  Symbolic constant, of arity 0.
*/
import java.util.*;

public class Atom extends Nonvar implements ITerm, IProceedure {

    public final static Atom aNil=new Nil();
    public final static IProceedure aTrue=new SuccesfullProceedure();
    public final static IProceedure aFail=new UnsuccessfullProceedure();
    public final static Atom aYes=TermMap.defAtomStatic("yes");
    public final static Atom aNo=TermMap.defAtomStatic("no");
    public final static Atom anEof=TermMap.defAtomStatic("end_of_file");
    
    public final static Atom the(ITerm X) {
        if (null==X) return Atom.aNo;
        Compound C = Compound.newCompound("the",1);
        C.setDirectArg(0,X);
        return C;
    }


    public Atom() {
    }
    
    public Atom(String s) {
        setSym(s);  
    }

    public String toString() {
        return qname();
    }

    public boolean bind_to(ITerm that,TermStack trail) {
        return super.bind_to(that,trail) && ((Atom)that).sym==sym;
    }

    public boolean eq(ITerm that) {
        return(that instanceof Atom) && ((Atom)that).sym==sym;
    }

    public String getKey() {
        return qname()+"/0";
    }

    /**
      returns an arity normally defined as 0
      @see ITerm#CONST
    */
    public int getArity() {
        return Term.CONST;
    }

    /**
      creates a AtomProceedure from a Atom known to be registered as
      being a builtin while returning its argument unchanged if it
      is just a plain Prolog constant with no builtin code attached to it
    */
    public Atom toProceedure(IProceedureMap bb) {
        if ( name().equals(Atom.aNil.name()) ) return Atom.aNil;
        if ( name().equals(Atom.aNo.name()) ) return Atom.aNo;
        if ( name().equals(Atom.aYes.name()) ) return Atom.aYes;

        Object B=bb.getProceedure(this);
        if ( B instanceof O1Queue  ) return this;
        if ( null==B ) {
            //IO.mes("not a builtin:"+this);
            return this;
        }
        return (Atom)B;
    }

}
