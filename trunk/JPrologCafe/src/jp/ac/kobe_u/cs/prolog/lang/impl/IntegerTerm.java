package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.EvaluationException;
import jp.ac.kobe_u.cs.prolog.lang.IllegalTypeException;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;

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
  /**
   * 
   */
  private static final long serialVersionUID = -6791859586170148195L;

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
  @Override
  public boolean unify(Object t) {
    if (StaticProlog.isVariable(t)) return StaticProlog.unify(t, this);
    if (!StaticProlog.isInteger(t))
      return false;
    else
      return this.doubleValue() == StaticProlog.intValue(t);
  }

  /**
   * @return the <code>boolean</code> whose value is
   * <code>convertible(Integer.class, type)</code>.
   * @see Object#convertible(Class, Class)
   */
  @Override
  public boolean convertible(Class type) {
    return StaticProlog.convertible(Integer.class, type);
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
  @Override
  public String toStringImpl(int d) {
    return "" + this.toJava();
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
  @Override
  public boolean equalJProlog(Object obj) {
    if (!(obj instanceof IntegerTerm)) return false;
    return this.doubleValue() == ((IntegerTerm) obj).longValue();
  }

  @Override
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
  @Override
  public int compareTo(Object anotherTerm) { // anotherTerm must be dereferenced.
    if (StaticProlog.isVariable(anotherTerm) || StaticProlog.isDouble(anotherTerm)) return Term.AFTER;
    if (!StaticProlog.isInteger(anotherTerm)) return Term.BEFORE;
    final long v = ((IntegerTerm) anotherTerm).longValue();
    if (this.doubleValue() == v) return Term.EQUAL;
    if (this.doubleValue() > v) return Term.AFTER;
    return Term.BEFORE;
  }

  //  /* NumberTerm */
  //  public int intValue() {
  //    return (int) this.doubleValue();
  //  }
  //
  //  public long longValue() {
  //    return (long) (this.doubleValue());
  //  }

  @Override
  public int arithCompareTo(NumberTerm t) {
    //    if (isDouble(t)) return -(t.arithCompareTo(this));
    final double v = t.doubleValue();
    if (this.doubleValue() == v) return Term.EQUAL;
    if (this.doubleValue() > v) return Term.AFTER;
    return Term.BEFORE;
  }

  //  boolean isInt() {
  //    return true;
  //  }

  @Override
  public NumberTerm abs() {
    return super.abs();
  }

  @Override
  public NumberTerm acos() {
    return StaticProlog.makeDouble(Math.acos(this.doubleValue()));
  }

  @Override
  public NumberTerm add(NumberTerm t) {
    if (StaticProlog.isDouble(t)) return super.add(t);
    return StaticProlog.makeInteger(this.mustInt() + t.longValue());
  }

  /**
   * @return
   */
  private long mustInt() {
    // TODO Auto-generated method stub
    return (long) this.doubleValue();
  }

  /**
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   */
  @Override
  public NumberTerm and(NumberTerm t) {
    if (StaticProlog.isDouble(t)) throw new IllegalTypeException("integer", t);
    return StaticProlog.makeInteger(this.mustInt() & t.longValue());
  }

  @Override
  public NumberTerm asin() {
    return StaticProlog.makeDouble(Math.asin(this.doubleValue()));
  }

  @Override
  public NumberTerm atan() {
    return StaticProlog.makeDouble(Math.atan(this.doubleValue()));
  }

  @Override
  public NumberTerm ceil() {
    return this;
  }

  @Override
  public NumberTerm cos() {
    return StaticProlog.makeDouble(Math.cos(this.doubleValue()));
  }

  /**
   * @exception EvaluationException if the given argument
   * <code>NumberTerm</code> represents <coe>0</code>.
   */
  @Override
  public NumberTerm divide(NumberTerm t) {
    if (t.doubleValue() == 0) throw new EvaluationException("zero_divisor");
    return StaticProlog.makeDouble(this.doubleValue() / t.doubleValue());
  }

  @Override
  public NumberTerm exp() {
    return StaticProlog.makeDouble(Math.exp(this.doubleValue()));
  }

  //  public NumberTerm floatIntPart() {
  //    throw new IllegalTypeException("float", this);
  //  }
  //
  //  public NumberTerm floatFractPart() {
  //    throw new IllegalTypeException("float", this);
  //  }

  @Override
  public NumberTerm floor() {
    return this;
  }

  /**
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   * @exception EvaluationException if the given argument
   * <code>NumberTerm</code> represents <coe>0</code>.
   */
  @Override
  public NumberTerm intDivide(NumberTerm t) {
    if (StaticProlog.isDouble(t)) throw new IllegalTypeException("integer", t);
    if (t.longValue() == 0) throw new EvaluationException("zero_divisor");
    return StaticProlog.makeInteger((this.mustInt() / t.longValue()));
  }

  /**
   * @exception EvaluationException if this object represents <coe>0</code>.
   */
  @Override
  public NumberTerm log() {
    if (this.doubleValue() == 0) throw new EvaluationException("undefined");
    return StaticProlog.makeDouble(Math.log(this.doubleValue()));
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
  @Override
  public NumberTerm mod(NumberTerm t) {
    //if (isDouble(t)) throw new IllegalTypeException("integer", t);
    if (t.longValue() == 0) throw new EvaluationException("zero_divisor");
    return StaticProlog.makeInteger(this.mustInt() % t.longValue());
  }

  @Override
  public NumberTerm multiply(NumberTerm t) {
    if (StaticProlog.isDouble(t)) return t.multiply(this);
    return StaticProlog.makeInteger(this.mustInt() * t.longValue());
  }

  @Override
  public NumberTerm not() {
    return StaticProlog.makeInteger(~this.mustInt());
  }

  /**
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   */
  @Override
  public NumberTerm or(NumberTerm t) {
    if (StaticProlog.isDouble(t)) throw new IllegalTypeException("integer", t);
    return StaticProlog.makeInteger(this.mustInt() | t.longValue());
  }

  @Override
  public NumberTerm pow(NumberTerm t) {
    return StaticProlog.makeDouble(Math.pow(this.doubleValue(), t.doubleValue()));
  }

  @Override
  public NumberTerm rint() {
    return StaticProlog.makeDouble(this.doubleValue());
  }

  @Override
  public NumberTerm round() {
    return this;
  }

  /**
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   */
  @Override
  public NumberTerm shiftLeft(NumberTerm t) {
    if (StaticProlog.isDouble(t)) throw new IllegalTypeException("integer", t);
    return StaticProlog.makeInteger(this.mustInt() << t.longValue());
  }

  /**
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   */
  @Override
  public NumberTerm shiftRight(NumberTerm t) {
    if (StaticProlog.isDouble(t)) throw new IllegalTypeException("integer", t);
    return StaticProlog.makeInteger(this.mustInt() >> t.longValue());
  }

  @Override
  public NumberTerm signum() {
    return StaticProlog.makeInteger((int) Math.signum(this.doubleValue()));
  }

  @Override
  public NumberTerm sin() {
    return StaticProlog.makeDouble(Math.sin(this.doubleValue()));
  }

  /**
   * @exception EvaluationException if this object represents
   * an integer less than <coe>0</code>.
   */
  @Override
  public NumberTerm sqrt() {
    if (this.doubleValue() < 0) throw new EvaluationException("undefined");
    return StaticProlog.makeDouble(Math.sqrt(this.doubleValue()));
  }

  @Override
  public NumberTerm subtract(NumberTerm t) {
    if (StaticProlog.isDouble(t)) return StaticProlog.makeDouble(this.doubleValue() - t.doubleValue());
    return StaticProlog.makeInteger((long) (this.doubleValue() - t.doubleValue()));
  }

  @Override
  public NumberTerm tan() {
    return StaticProlog.makeDouble(Math.tan(this.doubleValue()));
  }

  @Override
  public NumberTerm toDegrees() {
    return StaticProlog.makeDouble(Math.toDegrees(this.doubleValue()));
  }

  @Override
  public NumberTerm toFloat() {
    return StaticProlog.makeDouble(this.doubleValue());
  }

  @Override
  public NumberTerm toRadians() {
    return StaticProlog.makeDouble(Math.toRadians(this.doubleValue()));
  }

  @Override
  public NumberTerm truncate() {
    return this;
  }

  /**
   * @exception IllegalTypeException if the given argument
   * <code>NumberTerm</code> is a floating point number.
   */
  @Override
  public NumberTerm xor(NumberTerm t) {
    if (StaticProlog.isDouble(t)) throw new IllegalTypeException("integer", t);
    return StaticProlog.makeInteger(this.mustInt() ^ t.longValue());
  }
}
