/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang;


public interface PrologMethods {

  Object[] args(Object obj);

  int arity(Object tin);

  /** 
   * Binds this variable to a given term. 
   * And pushs this variable to trail stack if necessary. 
   * @param that a term to be bound.
   * @see Trail
   */
  boolean bind(Object thiz, Object si1);

  int compareTerm(Object thiz, Object o);

  Object[] consArgs(Object obj);

  /** 
   * Check whether there is a widening conversion from <code>from</code> to <code>to</code>.
   */
  boolean convertible(Class from, Class to);

  boolean convertible(Object thiz, Class to);

  Object copy(Object thiz);

  /** Returns the deref value of this term. */
  Object deref(Object thiz);

  double doubleValue(Object t);

  Object first(Object thiz);

  /*SymbolTerm*/Object functor(Object t);

  boolean functorOf(Object s2, Object a3);

  Class getClass(Object evaluate);

  Class getClazz(Object thiz);

  int hashCode(Object thiz);

  int hashCode(Object thiz, int depth);

  /** Checks whether a given object is an instance of Prolog term. */
  boolean instanceOfTerm(Object obj);

  long intValue(Object obj);

  /**
   * Check whether this term is an atom.
   * @return <code>true</code> if <code>this instanceof SymbolTerm/code>,
   * otherwise <code>false</code>.
   * @see SymbolTerm
   */
  boolean isAtomTerm(Object thiz);

  /**
   * Check whether this term is a closure term.
   * @return <code>true</code> if <code>this instanceof ClosureTerm</code>,
   * otherwise <code>false</code>.
   * @see ClosureTerm
   */
  boolean isClosure(Object thiz);

  /**
   * Check whether this term is a compound term.
   * @return <code>true</code> if <code>this instanceof StructureTerm</code>,
   * otherwise <code>false</code>.
   * @see StructureTerm
   */
  boolean isCompound(Object thiz);

  /**
   * @param a3
   * @return
   */
  boolean isCutter(Object a3);

  /**
   * Check whether this term is a float.
   * @return <code>true</code> if <code>this instanceof DoubleTerm</code>,
   * otherwise <code>false</code>.
   * @see NumberTerm
   */
  boolean isDouble(Object thiz);

  /**
   * Check whether this term is a ground term.
   * @return <code>true</code> if ground, otherwise <code>false</code>.
   */
  boolean isGround(Object thiz);

  /**
   * Check whether this term is an integer.
   * @return <code>true</code> if <code>this instanceof IntegerTerm</code>,
   * otherwise <code>false</code>.
   * @see IntegerTerm
   */
  boolean isInteger(Object thiz);

  //{ return this instanceof IntegerTerm; }

  /**
   * Check whether this term is a java term.
   * @return <code>true</code> if <code>this instanceof JavaObjectTerm</code>,
   * otherwise <code>false</code>.
   * @see JavaObjectTerm
   */
  boolean isJavaObject(Object thiz);

  /**
   * Check whether this term is a list structure.
   * @return <code>true</code> if <code>this instanceof ListTerm</code>,
   * otherwise <code>false</code>.
   * @see ListTerm
   */
  boolean isListTerm(Object thiz);

  /** Check whether this term is an empty list. */
  boolean isNil(Object thiz);

  /**
   * Check whether this term is a number.
   * @return <code>true</code> if <code>this instanceof IntegerTerm || this instanceof DoubleTerm</code>,
   * otherwise <code>false</code>.
   * @see IntegerTerm
   * @see NumberTerm
   */
  boolean isNumber(Object thiz);

  /**
   * Check whether this term is a logical variable.
   * @return <code>true</code> if <code>this instanceof VariableTerm</code>,
   * otherwise <code>false</code>.
   * @see VariableTerm
   */
  boolean isVariable(Object thiz);

  /**
   * @param thiz
   * @return
   */
  int listLength(Object thiz);

  /*SymbolTerm*/Object makeAtom(Object s);

  /*SymbolTerm*/Object makeAtom(Object s, int i);

  /**
   * @param pred_$op_specifier_2
   * @return
   */
  Object makeClosure(Predicate pred);

  /**
   * @param random
   * @return
   */
  Number makeDouble(Number c);

  /**
   * @return
   */
  HashtableOfTerm makeHashtableOf();

  Number makeInteger(Number c);

  Object makeJavaObject(Object c);

  Object makeList(Object car, Object cdr);

  Object makeStructure(Object s, Object[] args);

  Object makeTerm(final Object o);

  String nameUQ(Object thiz);

  // @Override
  boolean prologEquals(Object thiz, Object obj);

  /**
   * @param thiz
   * @return
   */
  Object rest(Object thiz);

  /**
   * @param thiz
   * @param deref
   */
  void setCar(Object thiz, Object deref);

  /**
   * @param thiz
   * @param deref
   */
  void setCdr(Object thiz, Object deref);

  /**
   * @param thiz
   * @return
   */
  Object toJava(Object thiz);

  /**
   * @param thiz
   * @return
   */
  String toQuotedString(Object thiz);

  String toString(Object thiz);

  boolean unify(Object thiz, Object o);

  /**
   * @param prolog
   * @return
   */
  Object makeVariable(Prolog prolog);

  /**
   * @param a1
   * @return
   */
  Predicate getCode(Object a1);

  /**
   * @param a
   * @return
   */
  Object makeVariableLoc(VariableTermLocation a);

  /**
   * @param arg1
   * @param d0
   * @return
   */
  String toString(Object arg1, int d0);

  /**
   * @param arg1
   * @param d0
   * @return
   */
  String toQuotedString(Object arg1, int d0);

  /**
   * @param obj
   * @param i
   * @return
   */
  Object arg0(Object obj, int i);
}
