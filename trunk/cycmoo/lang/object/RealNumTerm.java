package cycmoo.lang.object;  //
import cycmoo.lang.*;
import cycmoo.lang.fluent.*;

/**
  Part of the ITerm hierarchy, implementing double float
  point numbers.
  @see ITerm
  @see Nonvar
*/
public class RealNumTerm extends NumericTerm {
  public RealNumTerm(double i) {
    val=i;
  }

  double val;

  public String name() {
    return ""+val;
  }

  public boolean bind_to(ITerm that,TermStack trail) {
     return super.bind_to(that,trail) 
		 && val==((RealNumTerm)that).val;
  }

  public final int getArity() {
    return Term.REAL;
  }

  public final double getValue() {
	  return val;
  }
}

