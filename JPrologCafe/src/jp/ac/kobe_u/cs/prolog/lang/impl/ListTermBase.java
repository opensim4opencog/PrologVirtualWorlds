package jp.ac.kobe_u.cs.prolog.lang.impl;

import java.util.Vector;

import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;

/**
 * List.<br>
 * The class <code>ListTerm</code> represents a list structure.<br>
 *
 * <pre>
 *  % [1,2]
 *  Object Nil = Prolog.makeSymbol("[]");
 *  Object  n1 = IntegerTerm(1);
 *  Object  n2 = IntegerTerm(2);
 *  Object   t = Prolog.makeList(n1, Prolog.makeList(n2, Nil));
 *
 *  Object car = ((ListTerm)t).car();
 *  Object cdr = ((ListTerm)t).cdr();
 * </pre>
 *
 * Here is sample program for creating a list from <code>1</code> to <code>n</code>.
 * <pre>
 * public static Object makeList(int n) {
 *   Object t = Prolog.makeSymbol("[]");
 *   for (int i=n; i>0; i--) {
 *     t = Prolog.makeList(Prolog.makeInteger(i), t);
 *   }
 *   return t;
 * }
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
class ListTermBase extends TermBase implements ListTerm {

  /**
   * 
   */
  private static final long serialVersionUID = -6251840816820280692L;

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.TermBase#isNil()
   */
  @Override
  public boolean isNil() {
    // TODO Auto-generated method stub
    return false;
  }

  /** A functor <code>'.' /2</code>. */
  public static/*SymbolTerm*/Object SYM_DOT = StaticProlog.makeAtom(".", 2);

  /** Holds the first element of this <code>ListTerm</code>. */
  private Object car;

  /**
   * Holds the list consisting of all the rest of the elements of
   * this <code>ListTerm</code> but the first one.
   */
  private Object cdr;

  /**
   * Constructs a new Prolog list structure
   * such that <code>_car</code> is the first element of this list, and
   * <code>_cdr</code> is the list consisting of all the rest of the
   * elements of this list but the first one.
   */
  public ListTermBase(Object _car, Object _cdr) {
    this.car = _car;
    this.cdr = _cdr;
  }

  /** Returns the value of <code>car</code>.
   * @see #car
   */
  public Object first() {
    return this.car;
  }

  /** Returns the value of <code>cdr</code>.
   * @see #cdr
   */
  public Object rest() {
    return this.cdr;
  }

  /** Sets the value to <code>car</code>.
   * @see #car
   */
  public void setCar(Object t) {
    this.car = t;
  }

  /** Sets the value to <code>cdr</code>.
   * @see #cdr
   */
  public void setCdr(Object t) {
    this.cdr = t;
  }

  /* Object */
  @Override
  public boolean unify(Object t) {
    if (StaticProlog.isVariable(t)) return StaticProlog.unify(t, this);
    if (!StaticProlog.isListTerm(t)) return false;
    return StaticProlog.unify(this.car, StaticProlog.first(t)) && StaticProlog.unify(this.cdr, StaticProlog.rest(t));
    //	return car.unify(((ListTerm)t).car().deref(), trail)
    //	    && cdr.unify(((ListTerm)t).cdr().deref(), trail);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.TermBase#nameUQ()
   */
  @Override
  public String nameUQ() {
    // TODO Auto-generated method stub
    return ".";// SYM_DOT.nameUQ();
  }

  /**
   * @return the <code>boolean</code> whose value is
   * <code>convertible(Vector.class, type)</code>.
   * @see Object#convertible(Class, Class)
   */
  @Override
  public boolean convertible(Class type) {
    return StaticProlog.convertible(Vector.class, type);
  }

  @Override
  public Object copy() {
    return StaticProlog.makeList(StaticProlog.copy(this.car), StaticProlog.copy(this.cdr));
  }

  @Override
  public boolean isGround() {
    if (!StaticProlog.isGround(this.car)) return false;
    if (!StaticProlog.isGround(this.cdr)) return false;
    return true;
  }

  /** Returns the length of this <code>ListTerm</code>. */
  public int listLength() {
    int count = 0;
    Object t = this;
    while (StaticProlog.isListTerm(t)) {
      count++;
      t = StaticProlog.deref(StaticProlog.rest(t));
    }
    return count;
  }

  /**
   * Returns a <code>java.util.Vector</code> corresponds to this <code>ListTerm</code>
   * according to <em>Prolog Cafe interoperability with Java</em>.
   * @return a <code>java.util.Vector</code> object equivalent to
   * this <code>IntegerTerm</code>.
   */
  @Override
  public Object toJava() {
    final Vector<Object> vec = new Vector<Object>();
    Object t = this;
    while (StaticProlog.isListTerm(t)) {
      vec.addElement(StaticProlog.toJava(StaticProlog.deref(StaticProlog.first(t))));
      t = StaticProlog.deref(StaticProlog.rest(t));
    }
    return vec;
  }

  @Override
  public String toQuotedString(int depth) {
    Object x = this;
    String s = "[";
    for (;;) {
      s += StaticProlog.toQuotedString(StaticProlog.deref(StaticProlog.first(x)));
      x = StaticProlog.deref(StaticProlog.rest(x));
      if (!StaticProlog.isListTerm(x)) break;
      s += ",";
    }
    if (!StaticProlog.isNil(x)) s += "|" + StaticProlog.toQuotedString(x);
    s += "]";
    return s;
  }

  /* Object */
  /**
   * Checks <em>term equality</em> of two terms.
   * The result is <code>true</code> if and only if the argument is an instance of
   * <code>ListTerm</code>, and
   * all corresponding pairs of elements in the two lists are <em>term-equal</em>.
   * @param obj the object to compare with. This must be dereferenced.
   * @return <code>true</code> if the given object represents a Prolog list
   * equivalent to this <code>ListTerm</code>, false otherwise.
   * @see #compareTo
   */
  @Override
  public boolean equalJProlog(Object obj) {
    if (!(obj instanceof ListTerm)) return false;
    return StaticProlog.prologEquals(this.car, StaticProlog.deref(StaticProlog.first(obj))) && StaticProlog.prologEquals(this.cdr, StaticProlog.deref(StaticProlog.rest(obj)));
  }

  @Override
  public int hashCode(int d) {
    int h = 1;
    h = 31 * h + ListTermBase.SYM_DOT.hashCode();
    h = 31 * h + StaticProlog.hashCode(StaticProlog.deref(this.first()), d);
    h = 31 * h + StaticProlog.hashCode(StaticProlog.deref(this.rest()), d);

    return h;
  }

  /** Returns a string representation of this <code>ListTerm</code>. */
  @Override
  public String toStringImpl(int d) {
    Object x = this;
    String s = "[";
    for (;;) {
      s += StaticProlog.toString(StaticProlog.deref(StaticProlog.first(x)));
      x = StaticProlog.deref(StaticProlog.rest(x));
      if (!StaticProlog.isListTerm(x)) break;
      s += ",";
    }
    if (!StaticProlog.isNil(x)) s += "|" + StaticProlog.toString(x);
    s += "]";
    return s;
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
    if (StaticProlog.isVariable(anotherTerm) || StaticProlog.isNumber(anotherTerm) || StaticProlog.isAtomTerm(anotherTerm)) return Term.AFTER;
    if (StaticProlog.isCompound(anotherTerm)) {
      final int arity = StaticProlog.arity(anotherTerm);
      if (2 != arity) return (2 - arity);
      /*SymbolTerm*/final Object functor = StaticProlog.functor(anotherTerm);
      if (!StaticProlog.prologEquals(ListTermBase.SYM_DOT, functor)) return StaticProlog.compareTerm(ListTermBase.SYM_DOT, functor);
    }
    Object[] args = new Object[2];
    if (StaticProlog.isListTerm(anotherTerm)) {
      args[0] = StaticProlog.first((anotherTerm));
      args[1] = StaticProlog.rest(anotherTerm);//.rest();
    } else if (StaticProlog.isCompound(anotherTerm)) {
      args = StaticProlog.args(anotherTerm);
    } else {
      return Term.BEFORE;
    }
    Object tmp = this.car;
    int rc;
    for (int i = 0; i < 2; i++) {
      rc = StaticProlog.compareTerm(tmp, StaticProlog.deref(args[i]));
      if (rc != Term.EQUAL) return rc;
      tmp = this.cdr;
    }
    return Term.EQUAL;
  }

}
