package cycmoo.lang;  //tarau.jinni;
import java.io.Writer;
import java.io.FileOutputStream;
import java.io.IOException;

/**
  Builds Jinni Fluents from Java
  Streams
*/
public class StringSink extends SinkFluent {
  public StringBuffer buffer;
  
  public StringSink(IKernel p) {
    super(p);
    this.buffer=new StringBuffer();
  }

  public int putElement(ITerm t) {
    buffer.append(t.toUnquoted());
    return 1;
  }
  
  public void stop() {     buffer=null;
  }
   
  public ITerm collect() {
    return Blackboard.useStringAtom(buffer.toString());
  }
}
