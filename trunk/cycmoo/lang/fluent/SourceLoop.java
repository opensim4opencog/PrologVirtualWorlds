package cycmoo.lang;  //tarau.jinni;
import java.util.Enumeration;
import java.util.Vector;

/**
  An Infinite IFluent.
  If based on a finite IFluent, it moves to its the first element after reaching its last element.  A SourceLoop returns 'no' if the original IFluent is empty. 
  In case the original IFluent is infinite, a SourceLoop will 
  return the same elements as the original IFluent. 
  (In particular, this happens if the original IFluent is also a IFluent loop).
*/
public class SourceLoop extends Fluent {
    private Vector v;
    IFluent s;
    private int i;

    public SourceLoop(IFluent s,IKernel p) {
        super(p);
        this.s=s;
        this.v=new Vector();
        this.i=0;
    }

    private final ITerm getMemoized() {
        if ( null==v || v.size()<=0 ) return null;
        ITerm T=(ITerm)v.elementAt(i);
        i= (i+1) % v.size();
        return T;
    }

    public ITerm getElement() {
        ITerm T=null;
        if ( null!=s ) { //s is alive
            T=((Fluent)s).getElement();
            if ( null!=T )
                v.addElement(T);
            else {
                s=null;
            }
        }
        if ( null==s )
            T=getMemoized();
        return T;
    }

    public void stop() {
        v=null;
        s=null;
    }
}
