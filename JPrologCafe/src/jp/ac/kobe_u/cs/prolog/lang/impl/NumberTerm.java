package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;

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
abstract class NumberTerm extends Number implements Term {

  /* (non-Javadoc)
   * @see java.lang.Object#hashCode()
   */
  @Override
  public int hashCode() {
    // TODO Auto-generated method stub
    return this.val.hashCode();
  }

  /* (non-Javadoc)
   * @see java.lang.Object#equals(java.lang.Object)
   */
  @Override
  public boolean equals(Object obj) {
    // TODO Auto-generated method stub
    if (obj instanceof Number) {
      if (!(obj instanceof NumberTerm)) {
        return this.val.equals(obj);
      }
    }
    return this.equalJProlog(obj);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#copy()
   */
  @Override
  public Object copy() {
    // TODO Auto-generated method stub
    return this;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#deref()
   */
  @Override
  public Object deref() {
    // TODO Auto-generated method stub
    return this;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#isAtomTerm()
   */
  @Override
  public boolean isAtomTerm() {
    // TODO Auto-generated method stub
    return false;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#isClosure()
   */
  @Override
  public boolean isClosure() {
    // TODO Auto-generated method stub
    return false;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#isCompound()
   */
  @Override
  public boolean isCompound() {
    // TODO Auto-generated method stub
    return false;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#isGround()
   */
  @Override
  public boolean isGround() {
    // TODO Auto-generated method stub
    return true;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#isJavaObject()
   */
  @Override
  public boolean isJavaObject() {
    // TODO Auto-generated method stub
    return false;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#isListTerm()
   */
  @Override
  public boolean isListTerm() {
    // TODO Auto-generated method stub
    return false;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#isNil()
   */
  @Override
  public boolean isNil() {
    // TODO Auto-generated method stub
    return false;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#isNumber()
   */
  @Override
  public boolean isNumber() {
    // TODO Auto-generated method stub
    return true;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.Term#isVariable()
   */
  @Override
  public boolean isVariable() {
    // TODO Auto-generated method stub
    return false;
  }

  /**
   * Checks <em>term equality</em> of two terms.
   * The result is <code>true</code> if and only if the argument is an instance of
   * <code>IntegerTerm</code> and has the same <code>int</code> value as ((Number)this) object.
   * @param obj the object to compare with. This must be dereferenced.
   * @return <code>true</code> if the given object represents a Prolog integer
   * equivalent to ((Number)this) <code>IntegerTerm</code>, false otherwise.
   * @see #compareTo
  */
  @Override
  final public boolean equalJProlog(Object obj) {
    if (StaticProlog.isDouble(this)) {
      if (!StaticProlog.isDouble(obj)) return false;
    } else {
      if (!StaticProlog.isInteger(obj)) return false;
    }
    return this.doubleValue() == ((NumberTerm) obj).doubleValue();
  }

  /* Comparable */
  /**
   * Compares two terms in <em>Prolog standard order of terms</em>.<br>
   * It is noted that <code>t1.compareTo(t2) == 0</code> has the same
   * <code>boolean</code> value as <code>t1.equals(t2)</code>.
   * @param anotherTerm the term to compared with. It must be dereferenced.
   * @return the value <code>0</code> if two terms are identical;
   * a value less than <code>0</code> if ((Number)this) term is <em>before</em> the <code>anotherTerm</code>;
   * and a value greater than <code>0</code> if ((Number)this) term is <em>after</em> the <code>anotherTerm</code>.
   */
  @Override
  final public int compareTo(Object anotherTerm) { // anotherTerm must be dereferenced.
    if (StaticProlog.isVariable(anotherTerm) || (StaticProlog.isDouble(this) || StaticProlog.isDouble(anotherTerm))) return Term.AFTER;
    if (StaticProlog.isInteger(this) && !StaticProlog.isInteger(anotherTerm)) return Term.BEFORE;
    if (StaticProlog.isDouble(this) && !StaticProlog.isDouble(anotherTerm)) return Term.BEFORE;
    final double v = ((NumberTerm) anotherTerm).doubleValue();
    if (this.doubleValue() == v) return Term.EQUAL;
    if (this.doubleValue() > v) return Term.AFTER;
    return Term.BEFORE;
  }

  /* Object */
  @Override
  final public boolean unify(Object t) {
    if (StaticProlog.isVariable(t)) return StaticProlog.unify(t, this);
    if (StaticProlog.isDouble(this)) {
      if (!StaticProlog.isDouble(t)) return false;
    } else {
      if (!StaticProlog.isInteger(t)) return false;
    }
    return this.doubleValue() == StaticProlog.intValue(t);
  }

  //
  //  /* (non-Javadoc)
  //   * @see be.kuleuven.jPrologObject#isConst()
  //   */
  //  @Override
  //  final public boolean isConst() {
  //    // TODO Auto-generated method stub
  //    return true;
  //  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.TermBase#isDouble()
   */
  @Override
  final public boolean isDouble() {
    return this.val.intValue() != this.val.hashCode();
  }

  @Override
  final public boolean isInteger() {
    return this.val.intValue() == this.val.hashCode();
  }

  /**
   * Returns a <code>java.lang.Double</code> corresponds to ((Number)this) <code>DoubleTerm</code>
   * according to <em>Prolog Cafe interoperability with Java</em>.
   * @return a <code>java.lang.Double</code> object equivalent to
   * ((Number)this) <code>DoubleTerm</code>.
   */
  @Override
  final public Object toJava() {
    return this.val;
  }

  /* Object */
  /** Returns a string representation of ((Number)this) <code>IntegerTerm</code>. */
  @Override
  final public String toStringImpl(int d) {
    return "" + this.toJava();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.TermBase#toQuotedString()
   */
  @Override
  final public String toQuotedString() {
    // TODO Auto-generated method stub
    return "" + this.toJava();
  }

  /**
   * @return the <code>boolean</code> whose value is
   * <code>convertible(Integer.class, type)</code>.
   * @see Object#convertible(Class, Class)
   */
  @Override
  final public boolean convertible(Class type) {
    return StaticProlog.convertible(this.val.getClass(), type);
  }

  @Override
  final public int hashCode(int d) {
    return this.toJava().hashCode();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.TermBase#toQuotedString()
   */
  @Override
  final public String toString() {
    // TODO Auto-generated method stub
    return "" + this.toJava();
  }

  /**
   *
   */
  private static final long serialVersionUID = 3922819481957309645L;

  /** Holds a <code>double</code> value that ((Number)this) <code>DoubleTerm</code> represents. */
  private final Number val;

  /**
   * Constructs a new Prolog floating point number
   * that represents the specified <code>double</code> value.
   */
  public NumberTerm(Number i) {
    this.val = i;
  }

  //    protected Object copy(Prolog engine) { return makeInteger(val); }

  //  /**
  //   * Returns a <code>java.lang.Integer</code> corresponds to ((Number)this) <code>IntegerTerm</code>
  //   * according to <em>Prolog Cafe interoperability with Java</em>.
  //   * @return a <code>java.lang.Integer</code> object equivalent to
  //   * ((Number)this) <code>IntegerTerm</code>.
  //   */
  //  final public  Object toJava() {
  //    return ;//Integer.valueOf(this.val);
  //  }

  @Override
  final public double doubleValue() {
    return this.val.doubleValue();
  }

  @Override
  final public float floatValue() {
    return (float) (this.doubleValue());
  }

  /* NumberTerm */
  @Override
  final public int intValue() {
    return this.val.intValue();
  }

  @Override
  final public long longValue() {
    return this.val.longValue();
  }
  //
  //  final public NumberTerm negate() {
  //    if (StaticProlog.isInteger(this)) return StaticProlog.makeInteger(-this.longValue());
  //    return StaticProlog.makeDouble(-this.doubleValue());
  //  }

}
