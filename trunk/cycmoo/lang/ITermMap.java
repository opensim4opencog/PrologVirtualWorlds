package cycmoo.lang;  //tarau.jinni;
import java.util.*;
import java.io.Reader;
/**
  Implements a ITerm and HornClause  objects based blackboard (database).
*/
public interface ITermMap extends Map {
//    abstract public void register(IProceedure proto);
//    abstract public IProceedure getProceedure(Nonvar proto);
    abstract public Enumeration keys();
    abstract public ITerm useAtom(String s);
    abstract public Atom defAtom(String s);
   // abstract public Atom useStringAtom(String s);
    //abstract public Enumeration elements();
  
}

