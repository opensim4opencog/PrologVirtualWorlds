package cycmoo.lang;  //tarau.jinni;
/**
  Special constants, used to Name variables
  @see ITerm
  @see Var
*/
public class PseudoVar extends Atom {
  public PseudoVar(int i) {
    this("V_"+i);
  }

  public PseudoVar(String s) {
    super();
    setSym(s);
  }

  public String toString() {
      return name();
  }
}
