package cycmoo.lang;  //tarau.jinni;

/**
  Always fails
*/
public class UnsuccessfullProceedure extends AtomProceedure {
  UnsuccessfullProceedure() {super("fail");}

  public int exec(IKernel p) {
    return 0;
  }
}