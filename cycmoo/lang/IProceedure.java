package jinni.kprolog;  //tarau.jinni;
import java.util.*;
import java.io.Reader;
/**
  Implements a Executable Proceedure Interface 
  
*/
public interface IProceedure {
//  abstract public KernelAgent getPeer(); 
    public int getArity();
    public String qname();
    public String name();
    public String getKey();
    /**
      Executed when a builtin is called.
      Needs to be overriden. Returns a run-time
      warning if this is forgotten.
    */
    public int exec(IKernel parentF);
    public boolean isProceedure();
}

