package jinni.kprolog;  //tarau.jinni;
import jinni.kprolog.parser.PrologParser;  //tarau.jinni;
import java.util.*;
import java.io.Reader;
/**
  Implements a ITerm and HornClause  objects based blackboard (database).
*/
public class Blackboard extends TermMap implements IProceedureMap,Map,IKernel {

/*
    public Blackboard() {
        super(new Hashtable());
    }
*/
    public Blackboard(Map map ) {
        super(map);  
    }
    public Blackboard getBlackboard() {
        return this;
    }
}

