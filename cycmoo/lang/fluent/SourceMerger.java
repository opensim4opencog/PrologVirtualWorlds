package cycmoo.lang;  //tarau.jinni;
import java.util.Vector;
import java.util.Enumeration;
/**
  Merges a List of Sources into a new SourceFluent which  (fairly) iterates over them breadth first.
*/
public class SourceMerger extends EnumerationSource {
    public SourceMerger(Atom Xs,IKernel p) {
        super(p);
        this.Q=new O1Queue(TermCopier.ConsToVector(Xs));
    }

    private O1Queue Q;

    public ITerm getElement() {
        if ( null==Q ) return null;
        while ( !Q.isEmpty() ) {
            SourceFluent current=(SourceFluent)Q.deq();
            if ( null==current ) continue;
            ITerm T=current.getElement();
            if ( null==T ) continue;
            Q.enq(current); 
            return T;
        }
        return null;
    }
}

