package cycmoo.lang.fluent;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;
import java.util.Vector;

/**
  Builds Jinni Fluents from Java
  Streams
*/
public class TermCollector extends SinkFluent {
    public Vector buffer;
    private IKernel p;

    public TermCollector(IKernel p) {
        super(p);
        this.p=p;
        this.buffer=new Vector();
    }

    public int putElement(ITerm T) {
        buffer.addElement(T);
        return 1;
    }

    public void stop() {
        buffer=null;
    }

    public ITerm collect() {
        return new EnumerationSource(buffer,p);
    }
}
