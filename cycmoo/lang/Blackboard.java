package cycmoo.lang;  //
import cycmoo.lang.parser.PrologParser;  //
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

