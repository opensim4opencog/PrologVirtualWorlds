package cycmoo.lang;  //
import cycmoo.lang.fluent.*;
import cycmoo.lang.builtin.*;  //
import cycmoo.lang.object.*;  //
import java.util.*;
import java.io.Reader;
/**
  Implements a Executable Proceedure Interface 
  
*/
public interface IKernel extends ITerm {
    abstract public TermStack getTrail();
    abstract public IKernel getParent();
    abstract public ITerm getElement();
    abstract public Blackboard getBlackboard();
    abstract public void stop();
}

