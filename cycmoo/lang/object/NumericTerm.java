package cycmoo.lang.object; 
import cycmoo.lang.*;
import cycmoo.lang.fluent.*;
/**
  Abstract numeric class, part of the ITerm hierarchy
  @see IntTerm
  @see RealNumTerm
  @see ITerm
*/
public abstract class NumericTerm extends Nonvar {
 
  public String toString() {
    return name();
  }

  abstract public double getValue();

}