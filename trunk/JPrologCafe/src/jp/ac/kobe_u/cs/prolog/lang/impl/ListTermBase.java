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

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.TermBase#isNil()
   */
  @Override
  public boolean isNil() {
    // TODO Auto-generated method stub
    return false;
  }

  /* (non-Javadoc)
   * @see be.kuleuven.jProlog.PrologObject#isConst()
   */
  @Override
  public boolean isConst() {
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
    car = _car;
    cdr = _cdr;
  }

  /** Returns the value of <code>car</code>.
   * @see #car
   */
  public Object first() {
    return car;
  }

  /** Returns the value of <code>cdr</code>.
   * @see #cdr
   */
  public Object rest() {
    return cdr;
  }

  /** Sets the value to <code>car</code>.
   * @see #car
   */
  public void setCar(Object t) {
    car = t;
  }

  /** Sets the value to <code>cdr</code>.
   * @see #cdr
   */
  public void setCdr(Object t) {
    cdr = t;
  }

  /* Object */
  public boolean unify(Object t) {
    if (isVariable(t)) return unify(t, this);
    if (!isListTerm(t)) return false;
    return unify(car, first(t)) && unify(cdr, rest(t));
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
  public boolean convertible(Class type) {
    return convertible(Vector.class, type);
  }

  public Object copy() {
    return makeList(copy(car), copy(cdr));
  }

  public boolean isGround() {
    if (!isGround(car)) return false;
    if (!isGround(cdr)) return false;
    return true;
  }

  /** Returns the length of this <code>ListTerm</code>. */
  public int listLength() {
    int count = 0;
    Object t = this;
    while (isListTerm(t)) {
      count++;
      t = deref(rest(t));
    }
    return count;
  }

  /** 
   * Returns a <code>java.util.Vector</code> corresponds to this <code>ListTerm</code>
   * according to <em>Prolog Cafe interoperability with Java</em>.
   * @return a <code>java.util.Vector</code> object equivalent to
   * this <code>IntegerTerm</code>.
   */
  public Object toJava() {
    Vector<Object> vec = new Vector<Object>();
    Object t = this;
    while (isListTerm(t)) {
      vec.addElement(toJava(deref(first(t))));
      t = deref(rest(t));
    }
    return vec;
  }

  public String toQuotedString() {
    Object x = this;
    String s = "[";
    for (;;) {
      s += toQuotedString(deref(first(x)));
      x = deref(rest(x));
      if (!isListTerm(x)) break;
      s += ",";
    }
    if (!isNil(x)) s += "|" + toQuotedString(x);
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
  public boolean equalJProlog(Object obj) {
    if (!(obj instanceof ListTerm)) return false;
    return prologEquals(car, deref(first(obj))) && prologEquals(cdr, deref(rest(obj)));
  }

  public int hashCode(int d) {
    int h = 1;
    h = 31 * h + SYM_DOT.hashCode();
    h = 31 * h + hashCode(deref(first()), d);
    h = 31 * h + hashCode(deref(rest()), d);

    return h;
  }

  /** Returns a string representation of this <code>ListTerm</code>. */
  public String toStringImpl(int d) {
    Object x = this;
    String s = "[";
    for (;;) {
      s += toString(deref(first(x)));
      x = deref(rest(x));
      if (!isListTerm(x)) break;
      s += ",";
    }
    if (!isNil(x)) s += "|" + toString(x);
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
    if (isVariable(anotherTerm) || isNumber(anotherTerm) || isAtomTerm(anotherTerm)) return AFTER;
    if (isCompound(anotherTerm)) {
      int arity = arity(anotherTerm);
      if (2 != arity) return (2 - arity);
      /*SymbolTerm*/Object functor = functor(anotherTerm);
      if (!prologEquals(SYM_DOT, functor)) return compareTerm(SYM_DOT, functor);
    }
    Object[] args = new Object[2];
    if (isListTerm(anotherTerm)) {
      args[0] = first(((ListTerm) anotherTerm));
      args[1] = rest((ListTerm) anotherTerm);//.rest();
    } else if (isCompound(anotherTerm)) {
      args = args(anotherTerm);
    } else {
      return BEFORE;
    }
    Object tmp = car;
    int rc;
    for (int i = 0; i < 2; i++) {
      rc = compareTerm(tmp, deref(args[i]));
      if (rc != EQUAL) return rc;
      tmp = cdr;
    }
    return EQUAL;
  }

}
