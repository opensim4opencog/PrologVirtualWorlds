package cycmoo.lang.fluent;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;
import java.util.Enumeration;
import java.util.Vector;

abstract public class SinkFluent extends Fluent  implements IFluent {

  public SinkFluent(IKernel p) {
    super(p);
  }
  
  abstract public int putElement(ITerm T);
  public ITerm collect() {
      return null;
  }

}
