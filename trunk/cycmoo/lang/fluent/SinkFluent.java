package cycmoo.lang;  //tarau.jinni;
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
