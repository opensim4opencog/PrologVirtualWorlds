package cycmoo.lang.fluent;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;
import java.io.Writer;
import java.io.FileOutputStream;
import java.io.IOException;

/**
  Writer
*/
public class ClauseWriter extends CharWriter {
  public ClauseWriter(String f,IKernel p) {
     super(f,p);
  }
  
  public ClauseWriter(IKernel p) {
     super(p);
  }

  public int putElement(ITerm t) {
    if(null==writer) return 0;
    String s=null;
    if((t instanceof Compound) && "$string".equals(((Compound)t).name())) {
      Atom Xs = (Atom)((Compound)t).getDrefArg(0);
      s=Term.charsToString(Xs);
    }
    else
      s=t.pprint();
    IO.print(writer,s);
    return 1;
  }
}
