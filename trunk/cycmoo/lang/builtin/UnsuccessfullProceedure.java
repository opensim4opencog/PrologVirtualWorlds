package cycmoo.lang.builtin;  //
import cycmoo.lang.*;
import cycmoo.lang.fluent.*;
import cycmoo.lang.object.*;


/**
  Always fails
*/
public class UnsuccessfullProceedure extends AtomProceedure {
  public UnsuccessfullProceedure() {super("fail");}

  public int exec(IKernel p) {
    return 0;
  }
}