package cycmoo.lang;  //tarau.jinni;
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