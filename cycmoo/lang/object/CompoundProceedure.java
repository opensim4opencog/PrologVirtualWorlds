package cycmoo.lang;  //tarau.jinni;
/**
* Template for builtins of arity >0
*/

abstract public class CompoundProceedure extends Compound implements IProceedure {
    public KernelAgent getPeer() {
        return KernelAgent.currentInstance();
    }
  public CompoundProceedure(String f,int i) {
    super(f,i);
  }

  abstract public int exec(IKernel p);

  public boolean isProceedure() {
    return true;
  }
}
