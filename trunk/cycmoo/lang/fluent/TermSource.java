package cycmoo.lang;  //tarau.jinni;

/**
 * Maps a ITerm to an SourceFluent for iterating over its arguments
 */
public class TermSource extends SourceFluent {
    public TermSource(Nonvar val,IKernel p) {
        super(p);
        this.val=val;
        pos=0;
    }

    private Nonvar val;
    private int pos;

    public ITerm getElement() {
        ITerm X;
        if ( null==val ) X=null;
        else if ( !(val instanceof Compound) ) {
            X=val;
            val=null;
        } else if ( 0==pos ) X=parent.getBlackboard().useAtom(val.name());
        else if ( pos<=((Compound)val).getArity() ) X=((Compound)val).getDrefArg(pos-1);
        else {
            X=null;
            val=null;
        }
        pos++;
        return X;
    }

    public void stop() {
        val=null;
    }
}
