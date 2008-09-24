package jp.ac.kobe_u.cs.prolog.lang;

/**
 * Floating point number.
 * The class <code>DoubleTerm</code> wraps a value of 
 * primitive type <code>double</code>.
 *
 * <pre>
 * Object t = makeDouble(3.3333);
 * double d = ((DoubleTerm)t).doubleValue();
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
*/
class NumberTerm extends TermBase {
  /** Holds a <code>double</code> value that this <code>DoubleTerm</code> represents. */
  private final Number val;
  
  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.TermBase#isDouble()
   */
  @Override
  public boolean isDouble() {
    // TODO Auto-generated method stub
    return val.intValue()!=val.hashCode();
  }

  /* (non-Javadoc)
   * @see be.kuleuven.jPrologObject#isConst()
   */
  @Override
  public boolean isConst() {
    // TODO Auto-generated method stub
    return true;
  }

  /**
   * Constructs a new Prolog floating point number 
   * that represents the specified <code>double</code> value.
   */
  public NumberTerm(Number i) {
    val = i;
  }

  /**
   * Returns the value of <code>val</code>.
   * @see #val
   */
  public double value() {
    return doubleValue();
  }

  /* Object */
  public boolean unify(Object t) {
    if (isVariable(t)) return unify(t, this);
    if (!isDouble(t)) return false;
    return this.doubleValue() == doubleValue(t);
  }

  /** 
   * @return the <code>boolean</code> whose value is
   * <code>convertible(Double.class, type)</code>.
   * @see Object#convertible(Class, Class)
   */
  public boolean convertible(Class type) {
    return convertible(Double.class, type);
  }

  //    protected Object copy(Prolog engine) { return makeDouble(val); }

  /** 
   * Returns a <code>java.lang.Double</code> corresponds to this <code>DoubleTerm</code>
   * according to <em>Prolog Cafe interoperability with Java</em>.
   * @return a <code>java.lang.Double</code> object equivalent to
   * this <code>DoubleTerm</code>.
   */
  public Object toJava() {
    return val;
  }

  /* Object */
  /** Returns a string representation of this <code>DoubleTerm</code>. */
  public String toStringImpl(int d) {
    return Double.toString(this.doubleValue());
  }

  /**
   * Checks <em>term equality</em> of two terms.
   * The result is <code>true</code> if and only if the argument is an instance of
   * <code>DoubleTerm</code> and has the same <code>double</code> value as this object.
   * @param obj the object to compare with. This must be dereferenced.
   * @return <code>true</code> if the given object represents a Prolog floating
   * point number equivalent to this <code>DoubleTerm</code>, false otherwise.
   * @see #compareTo
  */
  public boolean equalJProlog(Object obj) {

    if (!(obj instanceof NumberTerm)) return false;
    return Double.doubleToLongBits(this.doubleValue()) == Double.doubleToLongBits(((NumberTerm) obj).doubleValue());
  }

  public int hashCode(int d) {
    long bits = Double.doubleToLongBits(this.doubleValue());
    return (int) (bits ^ (bits >>> 32));
  }

  /* Comparable */
  /** 
   * Compares two terms in <em>Prolog standard order of terms</em>.<br>
   * It is noted that <code>t1.compareTo(t2) == 0</code> has the same
   * <code>boolean</code> value as <code>t1.equals(t2)</code>.
   * @param anotherTerm the term to compared with. It must be dereferenced.
   * @return the value <code>0</code> if two terms are identical; 
   * a value less than <code>0</code> if this term is <em>before</em> the <code>anotherTerm</code>;
   * and a value greater than <code>0</code> if this term is <em>after</em> the <code>anotherTerm</code>.
   */
  public int compareTo(Object anotherTerm) { // anotherTerm must be dereferenced
    if (isVariable(anotherTerm)) return AFTER;
    if (!isDouble(anotherTerm)) return BEFORE;
    return Double.compare(this.doubleValue(), doubleValue(anotherTerm));
  }

  /* NumberTerm */
  public int intValue() {
    return val.intValue();
  }

  public long longValue() {
    return val.longValue();
  }

  public double doubleValue() {
    return val.doubleValue();
  }

  public int arithCompareTo(NumberTerm t) {
    return Double.compare(this.doubleValue(), t.doubleValue());
  }

  public NumberTerm abs() {
    return makeDouble(Math.abs(this.doubleValue()));
  }

  public NumberTerm acos() {
    return makeDouble(Math.acos(this.doubleValue()));
  }

  public NumberTerm add(NumberTerm t) {
    return makeDouble(this.doubleValue() + t.doubleValue());
  }

  /** 
   * Throws a <code>type_error</code>.
   * @exception IllegalTypeException
   */
  public NumberTerm and(NumberTerm t) {
    throw new IllegalTypeException("integer", this);
  }

  //    public NumberTerm and(NumberTerm t) { return makeInteger(this.intValue() & t.intValue()); }

  public NumberTerm asin() {
    return makeDouble(Math.asin(this.doubleValue()));
  }

  public NumberTerm atan() {
    return makeDouble(Math.atan(this.doubleValue()));
  }

  public NumberTerm ceil() {
    return makeInteger((int) Math.ceil(this.doubleValue()));
  }

  public NumberTerm cos() {
    return makeDouble(Math.cos(this.doubleValue()));
  }

  /** 
   * @exception EvaluationException if the given argument
   * <code>NumberTerm</code> represents <coe>0</code>.
   */
  public NumberTerm divide(NumberTerm t) {
    if (t.doubleValue() == 0) throw new EvaluationException("zero_divisor");
    return makeDouble(this.doubleValue() / t.doubleValue());
  }

  public NumberTerm exp() {
    return makeDouble(Math.exp(this.doubleValue()));
  }

  public NumberTerm floatIntPart() {
    return makeDouble(Math.signum(this.doubleValue()) * Math.floor(Math.abs(this.doubleValue())));
  }

  public NumberTerm floatFractPart() {
    return makeDouble(this.doubleValue() - Math.signum(this.doubleValue()) * Math.floor(Math.abs(this.doubleValue())));
  }

  public NumberTerm floor() {
    return makeInteger((int) Math.floor(this.doubleValue()));
  }

  /** 
   * Throws a <code>type_error</code>.
   * @exception IllegalTypeException
   */
  public NumberTerm intDivide(NumberTerm t) {
    throw new IllegalTypeException("integer", this);
  }

  //    public NumberTerm intDivide(NumberTerm t) {	return makeInteger((int)(this.intValue() / t.intValue())); }

  /** 
   * @exception EvaluationException if this object represents <coe>0</code>.
   */
  public NumberTerm log() {
    if (this.doubleValue() == 0) throw new EvaluationException("undefined");
    return makeDouble(Math.log(this.doubleValue()));
  }

  public NumberTerm max(NumberTerm t) {
    return makeDouble(Math.max(this.doubleValue(), t.doubleValue()));
  }

  public NumberTerm min(NumberTerm t) {
    return makeDouble(Math.min(this.doubleValue(), t.doubleValue()));
  }

  /** 
   * Throws a <code>type_error</code>.
   * @exception IllegalTypeException
   */
  public NumberTerm mod(NumberTerm t) {
    throw new IllegalTypeException("integer", this);
  }

  //    public NumberTerm mod(NumberTerm t) { return makeInteger(this.intValue() % t.intValue()); }

  public NumberTerm multiply(NumberTerm t) {
    return makeDouble(this.doubleValue() * t.doubleValue());
  }

  public Number negate() {
    return -this.doubleValue();
  }

  /** 
   * Throws a <code>type_error</code>.
   * @exception IllegalTypeException
   */
  public NumberTerm not() {
    throw new IllegalTypeException("integer", this);
  }

  //    public NumberTerm not() { return makeInteger(~ this.intValue()); }

  /** 
   * Throws a <code>type_error</code>.
   * @exception IllegalTypeException
   */
  public NumberTerm or(NumberTerm t) {
    throw new IllegalTypeException("integer", this);
  }

  //    public NumberTerm or(NumberTerm t) { return makeInteger(this.intValue() | t.intValue()); }

  public NumberTerm pow(NumberTerm t) {
    return makeDouble(Math.pow(this.doubleValue(), t.doubleValue()));
  }

  public NumberTerm rint() {
    return makeDouble(Math.rint(this.doubleValue()));
  }

  public NumberTerm round() {
    return makeInteger((int) Math.round(this.doubleValue()));
  }

  /** 
   * Throws a <code>type_error</code>.
   * @exception IllegalTypeException
   */
  public NumberTerm shiftLeft(NumberTerm t) {
    throw new IllegalTypeException("integer", this);
  }

  /** 
   * Throws a <code>type_error</code>.
   * @exception IllegalTypeException
   */
  public NumberTerm shiftRight(NumberTerm t) {
    throw new IllegalTypeException("integer", this);
  }

  public NumberTerm signum() {
    return makeDouble(Math.signum(this.doubleValue()));
  }

  public NumberTerm sin() {
    return makeDouble(Math.sin(this.doubleValue()));
  }

  /** 
   * @exception EvaluationException if this object represents
   * a floating point number less than <coe>0</code>.
   */
  public NumberTerm sqrt() {
    if (this.doubleValue() < 0) throw new EvaluationException("undefined");
    return makeDouble(Math.sqrt(this.doubleValue()));
  }

  public NumberTerm subtract(NumberTerm t) {
    return makeDouble(this.doubleValue() - t.doubleValue());
  }

  public NumberTerm tan() {
    return makeDouble(Math.tan(this.doubleValue()));
  }

  public NumberTerm toDegrees() {
    return makeDouble(Math.toDegrees(this.doubleValue()));
  }

  public NumberTerm toFloat() {
    return this;
  }

  public NumberTerm toRadians() {
    return makeDouble(Math.toRadians(this.doubleValue()));
  }

  public NumberTerm truncate() {
    if (this.doubleValue() >= 0)
      return makeInteger((int) Math.floor(this.doubleValue()));
    else
      return makeInteger((int) (-1 * Math.floor(Math.abs(this.doubleValue()))));
  }

  /** 
   * Throws a <code>type_error</code>.
   * @exception IllegalTypeException
   */
  public NumberTerm xor(NumberTerm t) {
    throw new IllegalTypeException("integer", this);
  }
}
