package cycmoo.lang;  //tarau.jinni;
public class Conj extends Cons  implements IProceedure {
    public Conj(ITerm x0, ITerm x1) {
        super(",",x0,x1);
    }

    public String conjToString() {
        ITerm h=getDrefArg(0);
        ITerm t=getDrefArg(1);
        StringBuffer s=new StringBuffer(watchNull(h));
        for ( ;; ) {
            if ( !(t instanceof Conj) ) {
                s.append(","+t); 
                break;
            } else {
                h=((Conj)t).getDrefArg(0);
                t=((Conj)t).getDrefArg(1);
                s.append(","+watchNull(h));
            }
        }
        return s.toString();
    }

    public String toString() {
        return funToString();
    }

    static public final ITerm getHead(ITerm T) {
        T=T.dref();
        return(T instanceof Conj)?((Conj)T).getDrefArg(0):T;
    }

    static public final ITerm getTail(ITerm T) {
        T=T.dref();
        return(T instanceof Conj)?((Conj)T).getDrefArg(1):(ITerm)Atom.aTrue;
    }
}
