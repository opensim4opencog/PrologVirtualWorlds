package cycmoo.lang.object; 
import cycmoo.lang.*;
import cycmoo.lang.fluent.*;
import cycmoo.lang.builtin.*;  //
public class IntTerm extends NumericTerm {
  public IntTerm(long i) {
    val=i;
  }

  long val;

  public String name() {
    return ""+val;
  }

  public boolean bind_to(ITerm that,TermStack trail) {
     return super.bind_to(that,trail) &&
		 ((double)val==(double)((IntTerm)that).val);
	 // unbelievable but true: converting
	 // to double is the only way to convince
	 // Microsoft's jview that 1==1
	 // $$ casting to double to be removed
	 // once they get it right
  }

  public final int getArity() {
    return Term.INT;
  }

  public final long longValue() {
    return val;
  }

  public final int intValue() {
    return (int)val;
  }

  public final double getValue() {
	  return (double)val;
  }
}

