package cycmoo.lang.object; 
import cycmoo.lang.*; 
import cycmoo.lang.fluent.*;
import cycmoo.lang.builtin.*;  //
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
