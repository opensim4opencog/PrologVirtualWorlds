/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang;

//import jp.ac.kobe_u.cs.prolog.lang.impl.StaticPrologNumbers;

/**
 * @author root
 *
 */
abstract public class StaticProlog extends StaticPrologNumbers {

  public static PrologMethods //change to interface for builds 
  //StaticPrologMethodImpl 
  //
  //
  methods;

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
  final public static Number makeDouble(Number c) {
    return methods.makeDouble(c);
  }

  /**
   * @return
   */
  final public static HashtableOfTerm makeHashtableOf() {
    return methods.makeHashtableOf();
  }

  final public static Number makeInteger(Number c) {
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

  //  public static final Object makeTerm(final Object o) {
  //    return methods.makeTerm(o);
  //  }

  final public static String nameUQ(Object thiz) {
    return methods.nameUQ(thiz);
  }

  /**
   * @return
   */
  final public static IdentityLinkedList newIdentityLinkedList() {
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

  /**
   * @param a
   * @return
   */
  public static Object makeVariableLoc(VariableTermLocation a) {
    // TODO Auto-generated method stub
    return methods.makeVariableLoc(a);
  }

  /**
   * @param obj
   * @param i
   * @return
   */
  public static Object arg0(Object obj, int i) {
    // TODO Auto-generated method stub
    return methods.arg0(obj, i);
  }

  /**
   * @param arg1
   * @param d0
   * @return
   */
  public static String toString(Object arg1, int d0) {
    // TODO Auto-generated method stub
    return methods.toString(arg1, d0);
  }

  /**
   * @param arg1
   * @param d0
   * @return
   */
  public static String toQuotedString(Object arg1, int d0) {
    // TODO Auto-generated method stub
    return methods.toQuotedString(arg1, d0);
  }

}
