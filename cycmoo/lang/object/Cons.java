package cycmoo.lang;  //tarau.jinni;
/**
  List Atomructor. Cooperates with terminator Nil.

  @see Nil
*/
public class Cons extends Compound {
    public Cons(String cons,ITerm x0, ITerm x1) {
        super(cons,x0,x1);
    }

    public Cons(ITerm x0, ITerm x1) {
        this(".",x0,x1);
    }

    public ITerm getHead() {
        return getDrefArg(0);
    }

    public ITerm getTail() {
        return getDrefArg(1);
    }

    /**
      List printer.
    */
    public String toString() {
        ITerm h=getDrefArg(0);
        ITerm t=getDrefArg(1);
        StringBuffer s=new StringBuffer("["+watchNull(h));
        for ( ;; ) {
            if ( t instanceof Nil ) {
                s.append("]"); break;
            } else if ( t instanceof Cons ) {
                h=((Cons)t).getDrefArg(0);
                t=((Cons)t).getDrefArg(1);
                s.append(","+watchNull(h));
            } else {
                s.append("|"+watchNull(t)+"]"); break;
            }
        }
        return s.toString();
    }
}
