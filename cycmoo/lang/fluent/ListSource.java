package cycmoo.lang.fluent;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;
import java.util.Vector;

/**
  Builds an iterator from a list
*/
public class ListSource extends EnumerationSource {
  public ListSource(Atom Xs,IKernel p) {
    super(TermCopier.ConsToVector(Xs),p);
  }
}

