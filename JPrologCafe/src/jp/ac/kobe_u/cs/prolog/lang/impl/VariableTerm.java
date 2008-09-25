package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.Undoable;

/**
 * Variable.<br>
 * The <code>VariableTerm</code> class represents a logical variable.<br>
 * For example,
 * <pre>
 *   Object t = Prolog.makeVariable();
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
interface VariableTerm extends Term, Undoable {

  /** 
   * Returns the value of <code>timeStamp</code>.
   * @see #timeStamp
   */
  public long timeStamp();

  /** Returns a string representation of this object.*/
  public String nameUQ();

  boolean isBound();

  Undoable getUndoable();

  /* Object */
  /** 
   * Checks whether the argument term is unified with this one.
   * If this is an unbound variable, the <code>unify</code> method binds this to 
   * the dereferenced value of argument term: <code>bind(t.deref(), trail)</code>,
   * and returns <code>true</code>.
   * Otherwise, it returns a <code>boolean</code> whose value is <code>val.unify(t, trail)</code>.
   * @param t the term to be unified with.
   * @return <code>true</code> if succeeds, otherwise <code>false</code>.
   * @see #val
   * @see #bind(Object)
   * @see Trail
   */
  //public boolean unify(Object t);
  /** 
   * Binds this variable to a given term. 
   * And pushs this variable to trail stack if necessary. 
   * @param t a term to be bound.
   * @see Trail
   */
  //ublic boolean bind(Object t);
  //  /** 
  //   * Checks whether this object is convertible with the given Java class type 
  //   * if this variable is unbound.
  //   * Otherwise, returns the value of <code>val.convertible(type)</code>.
  //   * @param type the Java class type to compare with.
  //   * @return <code>true</code> if this (or dereferenced term) is 
  //   * convertible with <code>type</code>. Otherwise <code>false</code>.
  //   * @see #val
  //   */
  //  public boolean convertible(Class type);
  //
  //  /** 
  //   * Returns a copy of this object if unbound variable.
  //   * Otherwise, returns the value of <code>val.copy(engine)</code>.
  //   * @see #val
  //   */
  //  public Object copy(Prolog engine);
  //
  //  public Object deref();
  //
  //  // if bound return val.isGround()
  //  public boolean isGround();
  //
  //  /** 
  //   * Returns <code>this</code> if this variable is unbound.
  //   * Otherwise, returns a Java object that corresponds to the dereferenced term:
  //   * <code>val.toJava()</code>.
  //   * @return a Java object defined in <em>Prolog Cafe interoperability with Java</em>.
  //   * @see #val
  //   */
  //  public Object toJava();
  //
  //  /**
  //   * Returns a quoted string representation of this term if unbound.
  //   * Otherwise, returns the value of dereferenced term:
  //   * <code>val.toQuotedString()</code>
  //   * @see #val
  //   */
  //  public String toQuotedString();
  //
  //  /* Object */
  //  /**
  //   * Checks <em>term equality</em> of two terms.
  //   * This method returns a <code>boolean</code> whose value is
  //   * (<code>this == obj</code>) if this variable is unbound.
  //   * Otherwise, it returns the value of <code>val.equals(obj)</code>.
  //   * @param obj the object to compare with. This must be dereferenced.
  //   * @return <code>true</code> if this (or dereferenced term) is the same as the argument;
  //   * <code>false</code> otherwise.
  //   * @see #val
  //   * @see #compareTo
  //  */
  //  public boolean equals(Object obj);
  //
  //  /**
  //   * Returns a string representation of this term if unbound.
  //   * Otherwise, returns the value of dereferenced term:
  //   * <code>val.toString()</code>
  //   * @see #val
  //   */
  //  public String toString();
  //
  //  /* Undoable */
  //  public void unTrailSelf();
  //
  //  /* Comparable */
  //  /** 
  //   * Compares two terms in <em>Prolog standard order of terms</em>.<br>
  //   * It is noted that <code>t1.compareTo(t2) == 0</code> has the same
  //   * <code>boolean</code> value as <code>t1.equals(t2)</code>.
  //   * @param anotherTerm the term to compare with. It must be dereferenced.
  //   * @return the value <code>0</code> if two terms are identical; 
  //   * a value less than <code>0</code> if this term is <em>before</em> the <code>anotherTerm</code>;
  //   * and a value greater than <code>0</code> if this term is <em>after</em> the <code>anotherTerm</code>.
  //   */
  //  public int compareTo(Object anotherTerm);
  /**
   * @param variableTermBase
   */
  public void setVal(Object variableTermBase);

  /**
   * @return
   */
  public Prolog getMachine();
}
