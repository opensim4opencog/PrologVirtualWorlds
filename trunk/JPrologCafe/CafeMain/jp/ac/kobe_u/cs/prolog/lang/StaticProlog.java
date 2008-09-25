/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang;

import jp.ac.kobe_u.cs.prolog.lang.impl.NumberTerm;
import jp.ac.kobe_u.cs.prolog.lang.impl.StaticPrologMethodImpl;
import jp.ac.kobe_u.cs.prolog.lang.impl.StaticPrologNumbers;
import sun.awt.util.IdentityLinkedList;

/**
 * @author root
 *
 */
abstract public class StaticProlog extends StaticPrologNumbers {

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
    NumberTerm makeDouble(Number c);

    /**
     * @return
     */
    HashtableOfTerm makeHashtableOf();

    NumberTerm makeInteger(Number c);

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
  }

  final static PrologMethods methods = new StaticPrologMethodImpl();

  final public static Object[] args(Object obj) {
    return methods.args(obj);
  }

  final public static int arity(Object tin) {
    return methods.arity(tin);
  }

  final public static boolean bind(Object thiz, Object si1) {
    return methods.bind(thiz, si1);
  }

  final public static int compareTerm(Object thiz, Object o) {
    return methods.compareTerm(thiz, o);
  }//throws Throwable;

  final public static Object[] consArgs(Object obj) {
    return methods.consArgs(obj);
  }

  /** 
   * Check whether there is a widening conversion from <code>from</code> to <code>to</code>.
   */
  final public static boolean convertible(Class from, Class<?> to) {
    return methods.convertible(from, to);
  }

  public static boolean convertible(Object thiz, Class to) {
    return methods.convertible(thiz, to);
  }

  public static Object copy(Object thiz) {
    return methods.copy(thiz);
  }

  /** Returns the deref value of this term. */
  final public static Object deref(Object thiz) {
    return methods.deref(thiz);
  }//{ return this; }

  public static double doubleValue(Object t) {
    return methods.doubleValue(t);
  }

  final public static Object first(Object thiz) {
    return methods.first(thiz);
  }

  final public static/*SymbolTerm*/Object functor(Object t) {
    return methods.functor(t);
  }

  final public static boolean functorOf(Object s2, Object a3) {
    return methods.functorOf(s2, a3);
  }

  final public static Class getClass(Object evaluate) {
    return methods.getClass(evaluate);//si2);
  }

  final public static Class getClazz(Object thiz) {
    return methods.getClazz(thiz);//si2);
  }

  final public static int hashCode(Object thiz) {
    return methods.hashCode(thiz);
  }

  final public static int hashCode(Object thiz, int depth) {
    return methods.hashCode(thiz, depth);
  }

  /** Checks whether a given object is an instance of Prolog term. */
  final public static boolean instanceOfTerm(Object obj) {
    return methods.instanceOfTerm(obj);
  }

  final public static long intValue(Object obj) {
    return methods.intValue(obj);
  }

  /**
   * Check whether this term is an atom.
   * @return <code>true</code> if <code>this instanceof SymbolTerm/code>,
   * otherwise <code>false</code>.
   * @see SymbolTerm
   */
  final public static boolean isAtomTerm(Object thiz) {
    return methods.isAtomTerm(thiz);
  }//{ return this instanceof /*SymbolTerm*/Object; }

  /**
   * Check whether this term is a closure term.
   * @return <code>true</code> if <code>this instanceof ClosureTerm</code>,
   * otherwise <code>false</code>.
   * @see ClosureTerm
   */
  final public static boolean isClosure(Object thiz) {
    return methods.isClosure(thiz);
  }// { return this instanceof ClosureTerm; }

  /**
   * Check whether this term is a compound term.
   * @return <code>true</code> if <code>this instanceof StructureTerm</code>,
   * otherwise <code>false</code>.
   * @see StructureTerm
   */
  final public static boolean isCompound(Object thiz) {
    return methods.isCompound(thiz);
  }//{ return this instanceof StructureTerm; }

  /**
   * @param a3
   * @return
   */
  final public static boolean isCutter(Object a3) {
    return methods.isCutter(a3);
  }

  /**
   * Check whether this term is a float.
   * @return <code>true</code> if <code>this instanceof DoubleTerm</code>,
   * otherwise <code>false</code>.
   * @see NumberTerm
   */
  final public static boolean isDouble(Object thiz) {
    return methods.isDouble(thiz);
  }//{ return this instanceof DoubleTerm; }

  /**
   * Check whether this term is a ground term.
   * @return <code>true</code> if ground, otherwise <code>false</code>.
   */
  final public static boolean isGround(Object thiz) {
    return methods.isGround(thiz);
  }//{ return true; }

  /**
   * Check whether this term is an integer.
   * @return <code>true</code> if <code>this instanceof IntegerTerm</code>,
   * otherwise <code>false</code>.
   * @see IntegerTerm
   */
  final public static boolean isInteger(Object thiz) {
    return methods.isInteger(thiz);

  }//{ return this instanceof IntegerTerm; }

  /**
   * Check whether this term is a java term.
   * @return <code>true</code> if <code>this instanceof JavaObjectTerm</code>,
   * otherwise <code>false</code>.
   * @see JavaObjectTerm
   */
  final public static boolean isJavaObject(Object thiz) {
    return methods.isJavaObject(thiz);
  }// { return this instanceof JavaObjectTerm; }

  /**
   * Check whether this term is a list structure.
   * @return <code>true</code> if <code>this instanceof ListTerm</code>,
   * otherwise <code>false</code>.
   * @see ListTerm
   */
  final public static boolean isListTerm(Object thiz) {
    return methods.isListTerm(thiz);
  }//{ return this instanceof ListTerm; }

  /** Check whether this term is an empty list. */
  final public static boolean isNil(Object thiz) {
    return methods.isNil(thiz);
  }//{ return Prolog.Nil.equals(Object thiz,this){} }

  /**
   * Check whether this term is a number.
   * @return <code>true</code> if <code>this instanceof IntegerTerm || this instanceof DoubleTerm</code>,
   * otherwise <code>false</code>.
   * @see IntegerTerm
   * @see NumberTerm
   */
  final public static boolean isNumber(Object thiz) {
    return methods.isNumber(thiz);
  }// { return (Object thiz,(Object thiz,this instanceof IntegerTerm) || (Object thiz,this instanceof DoubleTerm)){} }

  /**
   * Check whether this term is a logical variable.
   * @return <code>true</code> if <code>this instanceof VariableTerm</code>,
   * otherwise <code>false</code>.
   * @see VariableTerm
   */
  final public static boolean isVariable(Object thiz) {
    return methods.isVariable(thiz);
  }//{ return this instanceof VariableTerm; }

  /**
   * @param thiz
   * @return
   */
  final public static int listLength(Object thiz) {
    return methods.listLength(thiz);
  }

  final public static/*SymbolTerm*/Object makeAtom(Object s) {
    return methods.makeAtom(s);
  }

  final public static/*SymbolTerm*/Object makeAtom(Object s, int i) {
    return methods.makeAtom(s, i);
  }

  /**
   * @param pred_$op_specifier_2
   * @return
   */
  final public static Object makeClosure(Predicate pred) {
    return methods.makeClosure(pred);
  }

  /**
   * @param random
   * @return
   */
  final public static NumberTerm makeDouble(Number c) {
    return methods.makeDouble(c);
  }

  /**
   * @return
   */
  final public static HashtableOfTerm makeHashtableOf() {
    return methods.makeHashtableOf();
  }

  final public static NumberTerm makeInteger(Number c) {
    return methods.makeInteger(c);

  }

  final public static Object makeJavaObject(Object c) {
    return methods.makeJavaObject(c);
  }

  final public static Object makeList(Object car, Object cdr) {
    return methods.makeList(car, cdr);
  }

  final public static Object makeStructure(Object s, Object[] args) {
    return methods.makeStructure(s, args);
  }

  public static final Object makeTerm(final Object o) {
    return methods.makeTerm(o);
  }

  final public static String nameUQ(Object thiz) {
    return methods.nameUQ(thiz);
  }

  /**
   * @return
   */
  final public static IdentityLinkedList newIdentityLinkedList() {
    // TODO Auto-generated method stub
    return new IdentityLinkedList();
  }

  // @Override
  final public static boolean prologEquals(Object thiz, Object obj) {
    return methods.prologEquals(thiz, obj);
  }// throws Throwable;

  /**
   * @param thiz
   * @return
   */
  final public static Object rest(Object thiz) {
    return methods.rest(thiz);
  }

  /**
   * @param thiz
   * @param deref
   */
  final public static void setCar(Object thiz, Object deref) {
    methods.setCar(thiz, deref);

  }

  /**
   * @param thiz
   * @param deref
   */
  final public static void setCdr(Object thiz, Object deref) {
    methods.setCdr(thiz, deref);
  }

  /**
   * @param thiz
   * @return
   */
  final public static Object toJava(Object thiz) {
    return methods.toJava(thiz);
  }

  /**
   * @param thiz
   * @return
   */
  final public static String toQuotedString(Object thiz) {
    return methods.toQuotedString(thiz);
  }

  final public static String toString(Object thiz) {
    return methods.toString(thiz);
  }// throws Throwable;

  final public static boolean unify(Object thiz, Object o) {
    return methods.unify(thiz, o);
  }//throws Throwable;

  /**
   * @param prolog
   * @return
   */
  public static Object makeVariable(Prolog prolog) {
    // TODO Auto-generated method stub
    return methods.makeVariable(prolog);
  }

  /**
   * @param a1
   * @return
   */
  public static Predicate getCode(Object a1) {
    // TODO Auto-generated method stub
    return methods.getCode(a1);
  }

}
