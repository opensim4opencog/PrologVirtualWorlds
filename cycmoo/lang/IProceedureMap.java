package cycmoo.lang;  //tarau.jinni;
import java.util.*;
import java.io.Reader;
/**
  Implements a ITerm and HornClause  objects based blackboard (database).
*/
public interface IProceedureMap extends ITermMap {
    abstract public void register(IProceedure proto);
    abstract public IProceedure getProceedure(Nonvar proto);
    //abstract public AtomProceedure newAtomProceedure(Atom A);
    //abstract public CompoundProceedure newCompoundProceedure(Atom A);
}
