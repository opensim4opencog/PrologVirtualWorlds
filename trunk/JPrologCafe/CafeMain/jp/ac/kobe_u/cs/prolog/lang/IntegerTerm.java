package jp.ac.kobe_u.cs.prolog.lang;

/**
 * Integer.<br>
 * The class <code>IntegerTerm</code> wraps a value of primitive type 
 * <code>int</code>. 
 * <pre>
 *   Object t = makeInteger(100);
 *   int i = ((IntegerTerm)t).intValue();
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
class IntegerTerm extends NumberTerm implements Term {
  /** Holds an <code>int</code> value that this <code>IntegerTerm</code> represents. */
  /// final private long val;
  /** Constructs a new Prolog integer that represents the specified <code>int</code> value. */
  public IntegerTerm(Number i) {
    super(i);
    //    if (i != (long) i) {
    //      throw new IllegalArgumentException("integer " + i);
    //    }
  }

  /* (non-Javadoc)
   * @see be.kuleuven.jProlog.PrologObject#isConst()
   */
  @Override
  public boolean isConst() {
    // TODO Auto-generated method stub
    return true;
  }

  //  /**
  //   * Constructs a new Prolog integer that represents integer value 
  //   * of specified <code>String</code> parameter.
  //   * @exception NumberFormatException
  //   * if the <code>String</code> does not contain a parsable integer.
  //   */
  //  public IntegerTerm(String i) {
  //    try {
  //      val = Integer.parseInt(i);
  //    } catch (NumberFormatException e) {
  //      throw e;
  //    }
  //  }

  //
  //  /**
  //   * Returns the value of <code>val</code>.
  //   * @see #val
  //   */
  //  public int value() {
  //    return val;
  //  }

  /* Object */
  public boolean unify(Object t) {
    if (isVariable(t)) return unify(t, this);
    if (!isInteger(t))
      return false;
    else
      return this.doubleValue() == intValue(t);
  }

  /** 
   * @return the <code>boolean</code> whose value is
   * <code>convertible(Integer.class, type)</code>.
   * @see Object#convertible(Class, Class)
   */
  public boolean convertible(Class type) {
    return convertible(Integer.class, type);
  }

  //    protected Object copy(Prolog engine) { return makeInteger(val); }

  //  /** 
  //   * Returns a <code>java.lang.Integer</code> corresponds to this <code>IntegerTerm</code>
  //   * according to <em>Prolog Cafe interoperability with Java</em>.
  //   * @return a <code>java.lang.Integer</code> object equivalent to
  //   * this <code>IntegerTerm</code>.
  //   */
  //  public Object toJava() {
  //    return ;//Integer.valueOf(this.val);
  //  }

  /* Object */
  /** Returns a string representation of this <code>IntegerTerm</code>. */
  public String toStringImpl(int d) {
    return "" + toJava();
  }

  /**
   * Checks <em>term equality</em> of two terms.
   * The result is <code>true</code> if and only if the argument is an instance of
   * <code>IntegerTerm</code> and has the same <code>int</code> value as this object.
   * @param obj the object to compare with. This must be dereferenced.
   * @return <code>true</code> if the given object represents a Prolog integer 
   * equivalent to this <code>IntegerTerm</code>, false otherwise.
   * @see #compareTo
  */
  public boolean equalJProlog(Object obj) {
    if (!(obj instanceof IntegerTerm)) return false;
    return this.doubleValue() == ((IntegerTerm) obj).longValue();
  }

  public int hashCode(int d) {
    return (int) this.doubleValue();
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
  public int compareTo(Object anotherTerm) { // anotherTerm must be dereferenced.
    if (isVariable(anotherTerm) || isDouble(anotherTerm)) return AFTER;
    if (!isInteger(anotherTerm)) return BEFORE;
    long v = ((IntegerTerm) anotherTerm).longValue();
    if (this.doubleValue() == v) return EQUAL;
    if (this.doubleValue() > v) return AFTER;
    return BEFORE;
  }

  //  /* NumberTerm */
  //  public int intValue() {
  //    return (int) this.doubleValue();
  //  }
  //
  //  public long longValue() {
  //    return (long) (this.doubleValue());
  //  }

  public float floatValue() {
    return (float) (this.doubleValue());
  }

  public int arithCompareTo(NumberTerm t) {
    //    if (isDouble(t)) return -(t.arithCompareTo(this));
    double v = t.doubleValue();
    if (this.doubleValue() == v) return EQUAL;
    if (this.doubleValue() > v) return AFTER;
    return BEFORE;
  }

  boolean isInt() {
    return true;
  }

  public NumberTerm abs() {
    if (isInt()) return makeInteger((long) Math.abs(this.doubleValue()));
    return super.abs();
  }

  public NumberTerm acos() {
    return makeDouble(Math.acos(this.doubleValue()));
  }

  public NumberTerm add(NumberTerm t) {
    if (isDouble(t)) return super.add(t);
    return makeInteger(this.mustInt() + t.longValue());
  }

  /**
   * @return
   */
  private long mustInt() {
    // TODO Auto-generated method stub
    return (long) doubleValue();
  }

  /** 
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   */
  public NumberTerm and(NumberTerm t) {
    if (isDouble(t)) throw new IllegalTypeException("integer", t);
    return makeInteger(this.mustInt() & t.longValue());
  }

  public NumberTerm asin() {
    return makeDouble(Math.asin(this.doubleValue()));
  }

  public NumberTerm atan() {
    return makeDouble(Math.atan(this.doubleValue()));
  }

  public NumberTerm ceil() {
    return this;
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

  //  public NumberTerm floatIntPart() {
  //    throw new IllegalTypeException("float", this);
  //  }
  //
  //  public NumberTerm floatFractPart() {
  //    throw new IllegalTypeException("float", this);
  //  }

  public NumberTerm floor() {
    return this;
  }

  /** 
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   * @exception EvaluationException if the given argument
   * <code>NumberTerm</code> represents <coe>0</code>.
   */
  public NumberTerm intDivide(NumberTerm t) {
    if (isDouble(t)) throw new IllegalTypeException("integer", t);
    if (t.longValue() == 0) throw new EvaluationException("zero_divisor");
    return makeInteger((this.mustInt() / t.longValue()));
  }

  /** 
   * @exception EvaluationException if this object represents <coe>0</code>.
   */
  public NumberTerm log() {
    if (this.doubleValue() == 0) throw new EvaluationException("undefined");
    return makeDouble(Math.log(this.doubleValue()));
  }

  //
  //  public NumberTerm max(NumberTerm t) {
  //    if (isDouble(t)) return t.max(this);
  //    return makeInteger(Math.max(this.val, t.longValue()));
  //  }
  //
  //  public NumberTerm min(NumberTerm t) {
  //    if (isDouble(t)) return t.min(this);
  //    return makeInteger(Math.min(this.val, t.longValue()));
  //  }

  /** 
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   * @exception EvaluationException if the given argument
   * <code>NumberTerm</code> represents <coe>0</code>.
   */
  public NumberTerm mod(NumberTerm t) {
    //if (isDouble(t)) throw new IllegalTypeException("integer", t);
    if (t.longValue() == 0) throw new EvaluationException("zero_divisor");
    return makeInteger(this.mustInt() % t.longValue());
  }

  public NumberTerm multiply(NumberTerm t) {
    if (isDouble(t)) return t.multiply(this);
    return makeInteger(this.mustInt() * t.longValue());
  }

  public Number negate() {
    if (isInt()) return -longValue();
    return -this.doubleValue();
  }

  public NumberTerm not() {
    return makeInteger(~this.mustInt());
  }

  /** 
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   */
  public NumberTerm or(NumberTerm t) {
    if (isDouble(t)) throw new IllegalTypeException("integer", t);
    return makeInteger(this.mustInt() | t.longValue());
  }

  public NumberTerm pow(NumberTerm t) {
    return makeDouble(Math.pow(this.doubleValue(), t.doubleValue()));
  }

  public NumberTerm rint() {
    return makeDouble(this.doubleValue());
  }

  public NumberTerm round() {
    return this;
  }

  /** 
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   */
  public NumberTerm shiftLeft(NumberTerm t) {
    if (isDouble(t)) throw new IllegalTypeException("integer", t);
    return makeInteger(this.mustInt() << t.longValue());
  }

  /** 
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   */
  public NumberTerm shiftRight(NumberTerm t) {
    if (isDouble(t)) throw new IllegalTypeException("integer", t);
    return makeInteger(this.mustInt() >> t.longValue());
  }

  public NumberTerm signum() {
    return makeInteger((int) Math.signum((double) this.doubleValue()));
  }

  public NumberTerm sin() {
    return makeDouble(Math.sin(this.doubleValue()));
  }

  /** 
   * @exception EvaluationException if this object represents
   * an integer less than <coe>0</code>.
   */
  public NumberTerm sqrt() {
    if (this.doubleValue() < 0) throw new EvaluationException("undefined");
    return makeDouble(Math.sqrt(this.doubleValue()));
  }

  public NumberTerm subtract(NumberTerm t) {
    if (isDouble(t)) return makeDouble(this.doubleValue() - t.doubleValue());
    return makeInteger((long) (this.doubleValue() - t.doubleValue()));
  }

  public NumberTerm tan() {
    return makeDouble(Math.tan(this.doubleValue()));
  }

  public NumberTerm toDegrees() {
    return makeDouble(Math.toDegrees(this.doubleValue()));
  }

  public NumberTerm toFloat() {
    return makeDouble((double) this.doubleValue());
  }

  public NumberTerm toRadians() {
    return makeDouble(Math.toRadians(this.doubleValue()));
  }

  public NumberTerm truncate() {
    return this;
  }

  /** 
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   */
  public NumberTerm xor(NumberTerm t) {
    if (isDouble(t)) throw new IllegalTypeException("integer", t);
    return makeInteger(this.mustInt() ^ t.longValue());
  }
}
