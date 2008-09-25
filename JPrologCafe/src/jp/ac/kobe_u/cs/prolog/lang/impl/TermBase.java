package jp.ac.kobe_u.cs.prolog.lang.impl;

import java.io.Serializable;

import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.Trail;

/**
 * The superclass of classes for term structures.
 * The subclasses of <code>Object</code> must override
 * the <code>unify</code> method.
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
abstract class TermBase extends StaticProlog

implements Serializable, Comparable<Object>, Term {


  /**
   * 
   */
  private static final long serialVersionUID = -8720103333850253464L;

  /**
   * @param string
   */
  public void typeError(String string) {
    // TODO Auto-generated method stub
    throw new ClassCastException("" + this + " is not " + string);
  }

  /* (non-Javadoc)
   * @see java.lang.Object#toString()
   */
  @Override
  public String toString() {
    // TODO Auto-generated method stub
    return this.toStringImpl(40);
  }

  /* (non-Javadoc)
   * @see java.lang.Object#toString()
   */
  @Override
  final public int hashCode() {
    // TODO Auto-generated method stub
    return this.hashCode(40);
  }

  @Override
  final public boolean equals(Object other) {
    return this.equalJProlog((other));
  }

  abstract public int hashCode(int depth);

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Object#nameUQ()
   */
  //@Override
  public String nameUQ() {
    // TODO Auto-generated method stub
    this.typeError("NAMED");
    return this.toString();
  }

  //abstract boolean prologEquals(Object o) ;

  public abstract boolean equalJProlog(Object s1);

  /**
   * Checks whether the argument term is unified with this one.
   * @param t the term to be unified with.
   * @param trail Trail Stack.
   * @return <code>true</code> if succeeds, otherwise <code>false</code>.
   */
  abstract public boolean unify(Object t);

  abstract public boolean isConst();

  /* (non-Javadoc)
   * @see be.kuleuven.jProlog.APrologObject#toStringImpl(int)
   */
  //@Override
  abstract public String toStringImpl(int depth);

  //    /** Holds an integer value <code>0</code>. */
  //    public static int EQUAL  = 0;
  //    /** Holds an integer value <code>1</code>. */
  //    public static int AFTER  = 1;
  //    /** Holds an integer value <code>-1</code>. */
  //    public static int BEFORE = -1;

  //  /* (non-Javadoc)
  //   * @see jp.ac.kobe_u.cs.prolog.lang.Object#functor()
  //   */
  //  @Override
  //  public /*SymbolTerm*/Object functor() {
  //    // TODO Auto-generated method stub
  //    typeError("HAS-FUNCTOR");
  //    return null;
  //  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Object#arity()
   */
  //  @Override
  //  public int arity() {
  //    typeError("HAS-ARITY");
  //    // TODO Auto-generated method stub
  //    return 0;
  //  }
  /**
   * Checks whether the argument term is unified with this one.
   * @param t the term to be unified with.
   * @return <code>true</code> if succeeds, otherwise <code>false</code>.
   */
  //
  //  public boolean prologEquals(Object s1) {
  //
  //    return equals(s1);
  //  }
  final public boolean unify(Object t, Trail trail) {
    return this.unify(t);
  }

  /**
   * Check whether this term is a logical variable.
   * @return <code>true</code> if <code>this instanceof VariableTerm</code>,
   * otherwise <code>false</code>.
   * @see VariableTerm
   */
  public boolean isVariable() {
    return this instanceof VariableTerm;
  }

  /**
   * Check whether this term is an integer.
   * @return <code>true</code> if <code>this instanceof IntegerTerm</code>,
   * otherwise <code>false</code>.
   * @see IntegerTerm
   */
  public boolean isInteger() {
    return false;// this instanceof IntegerTerm;
  }

  /**
   * Check whether this term is a float.
   * @return <code>true</code> if <code>this instanceof DoubleTerm</code>,
   * otherwise <code>false</code>.
   * @see NumberTerm
   */
  public boolean isDouble() {
    return false;// this instanceof NumberTerm;
  }

  /**
   * Check whether this term is a number.
   * @return <code>true</code> if <code>this instanceof IntegerTerm || this instanceof DoubleTerm</code>,
   * otherwise <code>false</code>.
   * @see IntegerTerm
   * @see NumberTerm
   */
  public boolean isNumber() {
    return false;//((this instanceof IntegerTerm) || (this instanceof NumberTerm));
  }

  /**
   * Check whether this term is an atom.
   * @return <code>true</code> if <code>this instanceof SymbolTerm</code>,
   * otherwise <code>false</code>.
   * @see SymbolTerm
   */
  public boolean isAtomTerm() {
    return this instanceof SymbolTerm;
  }

  /** Check whether this term is an empty list. */
  public boolean isNil() {
    if (true) return false;
    return StaticProlog.prologEquals(Prolog.Nil, this);
  }

  /**
   * Check whether this term is a list structure.
   * @return <code>true</code> if <code>this instanceof ListTerm</code>,
   * otherwise <code>false</code>.
   * @see ListTerm
   */
  public boolean isListTerm() {
    return this instanceof ListTerm;
  }

  /**
   * Check whether this term is a compound term.
   * @return <code>true</code> if <code>this instanceof StructureTerm</code>,
   * otherwise <code>false</code>.
   * @see StructureTerm
   */
  public boolean isCompound() {
    return this instanceof StructureTerm;
  }

  /**
   * Check whether this term is a java term.
   * @return <code>true</code> if <code>this instanceof JavaObjectTerm</code>,
   * otherwise <code>false</code>.
   * @see JavaObjectTerm
   */
  public boolean isJavaObject() {
    return this instanceof JavaObjectTerm;
  }

  /**
   * Check whether this term is a closure term.
   * @return <code>true</code> if <code>this instanceof ClosureTerm</code>,
   * otherwise <code>false</code>.
   * @see ClosureTerm
   */
  public boolean isClosure() {
    return this instanceof ClosureTerm;
  }

  /**
   * Check whether this object is convertible with the given Java class type.
   * @param type the Java class type to compare with.
   * @return <code>true</code> if this is convertible with
   * <code>type</code>. Otherwise <code>false</code>.
   * @see #convertible(Class, Class)
   */
  public boolean convertible(Class type) {
    return StaticProlog.convertible(this.getClass(), type);
  }

  /** Returns a copy of this object. */
  public Object copy() {
    return this;
  }

  /** Returns the deref value of this term. */
  public Object deref() {
    return this;
  }

  /**
   * Check whether this term is a ground term.
   * @return <code>true</code> if ground, otherwise <code>false</code>.
   */
  public boolean isGround() {
    return true;
  }

  /**
   * Returns a Java object that corresponds to this term
   * if defined in <em>Prolog Cafe interoperability with Java</em>.
   * Otherwise, returns <code>this</code>.
   * @return a Java object if defined in <em>Prolog Cafe interoperability with Java</em>,
   * otherwise <code>this</code>.
   */
  public Object toJava() {
    return this;
  }

  /** Returns a quoted string representation of this term. */
  public String toQuotedString() {
    return this.toString();
  }

}
