package cycmoo.lang.fluent;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;
import java.util.Enumeration;
import java.util.Vector;

/**
  Builds Jinni Iterators from Java
  Sequences and Iterator type classes
*/
public class EnumerationSource extends SourceFluent {
  private Enumeration e;
  
  public EnumerationSource(IKernel p) {
    super(p);
    e=null;
  }

  public EnumerationSource(Enumeration e,IKernel p) {
    super(p);
    this.e=e;
  }

  public EnumerationSource(Vector V,IKernel p) {
    super(p);
    this.e=V.elements();
  }

  public ITerm getElement() {
    if(null==e || ! e.hasMoreElements()) return null;
    else return (ITerm)e.nextElement();
  }

  public void stop() {
    e=null;
  }
}
