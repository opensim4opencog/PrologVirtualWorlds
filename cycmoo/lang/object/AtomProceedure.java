package cycmoo.lang;  //tarau.jinni;
/**
* Template for builtins of arity 0
*/
abstract public class AtomProceedure extends Atom implements IProceedure {
    public KernelAgent getPeer() {
        return KernelAgent.currentInstance();
    }

  public AtomProceedure(String s) {
    super();
    setSym(s);
  }

  abstract public int exec(IKernel p);

  public boolean isProceedure() {
    return true;
  }
}



