/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang;

import jade.util.leap.HashMap;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import java_cup.terminal;

import sun.awt.util.IdentityLinkedList;

/**
 * @author root
 *
 */
abstract public class StaticProlog extends StaticPrologNumbers {
  /**
   * @param ceil
   * @return
   */
  final static HashMap javaLang2TermMap = new HashMap();

  final public static Object[] args(Object obj) {
    return ((StructureTerm) obj).args();
  }

  final public static int arity(Object tin) {
    Term t = thisTerm(tin);
    return isCompound(t) ? ((StructureTerm) t).arity() : ((SymbolTerm) t).arity();
  }

  /**
   * @param a1
   * @param si1
   */
  final public static void bind(Object a1, Object si1) {
    ((VariableTerm) a1).bind(si1);
  }

  final public static int compareTerm(Object thiz, Object o) {
    return compareTo(thiz, o);
  }//throws Throwable;

  /**
   * @param arg1
   * @param arg2
   * @return
   */
  final public static int compareTo(Object arg1, Object arg2) {
    // TODO Auto-generated method stub
    return thisTerm(arg1).compareTo(arg2);
  }

  final public static Object[] consArgs(Object obj) {
    ListTerm ll = (ListTerm) obj;
    return new Object[] { ll.first(), ll.rest() };
  }

  /** 
   * Check whether there is a widening conversion from <code>from</code> to <code>to</code>.
   */
  final public static boolean convertible(Class from, Class<?> to) {
    if (from == null) return !to.isPrimitive();
    if (to.isAssignableFrom(from)) {
      return true;
    } else if (to.isPrimitive()) {
      if (from.equals(Boolean.class)) {
        return to.equals(Boolean.TYPE);
      } else if (from.equals(Byte.class)) {
        return to.equals(Byte.TYPE) || to.equals(Short.TYPE) || to.equals(Integer.TYPE) || to.equals(Long.TYPE) || to.equals(Float.TYPE) || to.equals(Double.TYPE);
      } else if (from.equals(Short.class)) {
        return to.equals(Short.TYPE) || to.equals(Integer.TYPE) || to.equals(Long.TYPE) || to.equals(Float.TYPE) || to.equals(Double.TYPE);
      } else if (from.equals(Character.class)) {
        return to.equals(Character.TYPE) || to.equals(Integer.TYPE) || to.equals(Long.TYPE) || to.equals(Float.TYPE) || to.equals(Double.TYPE);
      } else if (from.equals(Integer.class)) {
        return to.equals(Integer.TYPE) || to.equals(Long.TYPE) || to.equals(Float.TYPE) || to.equals(Double.TYPE);
      } else if (from.equals(Long.class)) {
        return to.equals(Long.TYPE) || to.equals(Float.TYPE) || to.equals(Double.TYPE);
      } else if (from.equals(Float.class)) {
        return to.equals(Float.TYPE) || to.equals(Double.TYPE);
      } else if (from.equals(Double.class)) {
        return to.equals(Double.TYPE);
      }
    }
    return false;
  }

  /**
   * @param arg1
   * @param to
   * @return
   */

  public static boolean convertible(Object thiz, Class to) {
    // TODO Auto-generated method stub
    return thisTerm(thiz).convertible(to);
  }

  /**
   * @param term
   * @param engine
   * @return
   */
  public static Object copy(Object thiz, Prolog engine) {
    // TODO Auto-generated method stub
    if (thiz instanceof String) return thiz;
    if (isJavaObject(thiz)) return thiz;
    return thisTerm(thiz).copy(engine);
  }

  /** Returns the deref value of this term. */
  final public static Object deref(Object thiz) {
    if (thiz instanceof Term) return ((Term) thiz).deref();
    return thiz;
  }//{ return this; }

  /**
   * @param t
   * @return
   */
  public static double doubleValue(Object t) {
    // TODO Auto-generated method stub
    if (isNumber(t)) {
      return ((Number) toJava(t)).doubleValue();
    }
    return t.hashCode();
  }

  /**
   * @param x
   * @return
   */
  final public static Object first(Object x) {
    // TODO Auto-generated method stub
    return ((ListTerm) x).first();
  }

  /**
   * @param t
   * @return
   */
  final public static/*SymbolTerm*/Object functor(Object t) {
    // TODO Auto-generated method stub
    return ((StructureTerm) t).functor();
  }

  final public static boolean functorOf(Object s2, Object a3) {
    // TODO Auto-generated method stub
    return prologEquals(s2, functor(a3));
  }

  final public static Class getClass(Object evaluate) {
    // TODO Auto-generated method stub
    return evaluate.getClass();//si2);
  }

  /**
   * @param a1
   * @return
   */
  final public static Class getClazz(Object a1) {
    // TODO Auto-generated method stub
    return toJava(a1).getClass();
  }

  final public static String getName(Class c) {
    // TODO Auto-generated method stub
    return c.getName();
    //return evaluate.arithCompareTo(si2);
  }

  /**
   * @param a1
   * @return
   */
  final public static int hashCode(Object a1) {
    // TODO Auto-generated method stub
    return hashCode(a1, 40);
  }

  //
  //  final public static boolean prologEquals(Object thiz, Object o) {
  //    return thisTerm(thiz).equalJProlog(o);
  //  }//throws Throwable;

  /**
   * @param a1
   * @return
   */
  final public static int hashCode(Object a1, int depth) {
    // TODO Auto-generated method stub
    if (a1 instanceof Term) return ((Term) a1).hashCode(depth);
    return a1.hashCode();
  }

  /** Checks whether a given object is an instance of Prolog term. */
  final public static boolean instanceOfTerm(Object obj) {
    return obj instanceof VariableTerm || obj instanceof IntegerTerm || obj instanceof NumberTerm || obj instanceof SymbolTerm || obj instanceof ListTerm || obj instanceof StructureTerm || obj instanceof JavaObjectTerm || obj instanceof ClosureTerm;
  }

  final public static long intValue(Object obj) {
    return ((NumberTerm) obj).longValue();
  }

  /**
   * Check whether this term is an atom.
   * @return <code>true</code> if <code>this instanceof SymbolTerm/code>,
   * otherwise <code>false</code>.
   * @see SymbolTerm
   */
  final public static boolean isAtomTerm(Object thiz) {
    if (thiz instanceof CharSequence) return true;
    Term t = thisTerm(thiz);
    if (t.isAtomTerm()) return true;
    if (t instanceof SymbolTerm) {
      if (isVariable(t)) return false;
      return false;
    }
    if (true) return false;
    return t instanceof SymbolTerm;// thisTerm(thiz).isAtomTerm();
  }//{ return this instanceof /*SymbolTerm*/Object; }

  /**
   * Check whether this term is a closure term.
   * @return <code>true</code> if <code>this instanceof ClosureTerm</code>,
   * otherwise <code>false</code>.
   * @see ClosureTerm
   */
  final public static boolean isClosure(Object thiz) {
    if (thiz instanceof String) return false;
    return thisTerm(thiz).isClosure();
  }// { return this instanceof ClosureTerm; }

  /**
   * Check whether this term is a compound term.
   * @return <code>true</code> if <code>this instanceof StructureTerm</code>,
   * otherwise <code>false</code>.
   * @see StructureTerm
   */
  final public static boolean isCompound(Object thiz) {
    if (thiz instanceof String) return false;
    return thisTerm(thiz).isCompound();
  }//{ return this instanceof StructureTerm; }

  /**
   * @param a3
   * @return
   */
  final public static boolean isCutter(Object a3) {
    // TODO Auto-generated method stub
    return isInteger(a3);
  }

  /**
   * Check whether this term is a float.
   * @return <code>true</code> if <code>this instanceof DoubleTerm</code>,
   * otherwise <code>false</code>.
   * @see NumberTerm
   */
  final public static boolean isDouble(Object thiz) {
    if (thiz instanceof Number) {
      Number n = (Number) thiz;
      if (n.hashCode() != n.intValue()) return true;
      return false;
    }
    if (thiz instanceof String) return false;
    return thisTerm(thiz).isDouble();
  }//{ return this instanceof DoubleTerm; }

  /**
   * Check whether this term is a ground term.
   * @return <code>true</code> if ground, otherwise <code>false</code>.
   */
  final public static boolean isGround(Object thiz) {
    if (isJavaObject(thiz)) return true;
    return thisTerm(thiz).isGround();

  }//{ return true; }

  /**
   * Check whether this term is an integer.
   * @return <code>true</code> if <code>this instanceof IntegerTerm</code>,
   * otherwise <code>false</code>.
   * @see IntegerTerm
   */
  final public static boolean isInteger(Object thiz) {
    thiz = toJava(thiz);
    if (thiz instanceof Number) {
      Number n = (Number) thiz;
      if (n.hashCode() == n.intValue()) return true;
      return false;
    }
    if (thiz instanceof String) return false;
    Term term = thisTerm(thiz);
    if (term.isNumber()) {
      if (((NumberTerm) term).floatFractPart().doubleValue() == 0) {
        return true;
      }
    }
    return term.isInteger();

  }//{ return this instanceof IntegerTerm; }

  /**
   * Check whether this term is a java term.
   * @return <code>true</code> if <code>this instanceof JavaObjectTerm</code>,
   * otherwise <code>false</code>.
   * @see JavaObjectTerm
   */
  final public static boolean isJavaObject(Object thiz) {
    if (thiz instanceof Term) return ((Term) thiz).isJavaObject();
    return true;
  }// { return this instanceof JavaObjectTerm; }

  /**
   * Check whether this term is a list structure.
   * @return <code>true</code> if <code>this instanceof ListTerm</code>,
   * otherwise <code>false</code>.
   * @see ListTerm
   */
  final public static boolean isListTerm(Object thiz) {
    if (thiz instanceof Term) return ((Term) thiz).isListTerm();
    return thiz instanceof Collection;
  }//{ return this instanceof ListTerm; }

  /** Check whether this term is an empty list. */
  final public static boolean isNil(Object thiz) {
    if (thiz == null) return true;
    if (thiz instanceof String) return "[]".equals(thiz);
    return thisTerm(thiz).isNil();
  }//{ return Prolog.Nil.equals(Object thiz,this){} }

  /**
   * @param thiz
   * @return
   */
  private static boolean isNonTerm(Object thiz) {
    // TODO Auto-generated method stub
    if (thiz instanceof Term) return false;
    return true;
  }

  /**
   * Check whether this term is a number.
   * @return <code>true</code> if <code>this instanceof IntegerTerm || this instanceof DoubleTerm</code>,
   * otherwise <code>false</code>.
   * @see IntegerTerm
   * @see NumberTerm
   */
  final public static boolean isNumber(Object thiz) {
    if (thiz instanceof Number) return true;
    return thisTerm(thiz).isNumber();

  }// { return (Object thiz,(Object thiz,this instanceof IntegerTerm) || (Object thiz,this instanceof DoubleTerm)){} }

  /**
   * Check whether this term is a logical variable.
   * @return <code>true</code> if <code>this instanceof VariableTerm</code>,
   * otherwise <code>false</code>.
   * @see VariableTerm
   */
  final public static boolean isVariable(Object thiz) {
    return (thiz instanceof VariableTerm);// thisTerm(thiz).isVariable();

  }//{ return this instanceof VariableTerm; }

  /**
   * @param a1
   * @return
   */
  final public static int listLength(Object x) {
    // TODO Auto-generated method stub
    return ((ListTerm) x).listLength();
  }

  final public static/*SymbolTerm*/Object makeAtom(String s) {
    return SymbolTermBase.makeSymbol(s);
  }

  final public static/*SymbolTerm*/Object makeAtom(String s, int i) {
    return SymbolTermBase.makeSymbol(s, i);
  }

  /**
   * @param pred_$op_specifier_2
   * @return
   */
  final public static ClosureTerm makeClosure(Predicate pred) {
    // TODO Auto-generated method stub
    return new ClosureTermBase(pred);
  }

  /**
   * @param random
   * @return
   */
  final public static NumberTerm makeDouble(Number c) {
    NumberTerm it = (NumberTerm) javaLang2TermMap.get(c);
    if (it == null) {
      it = new NumberTerm(c);
      javaLang2TermMap.put(c, it);
    }
    return it;
  }

  /**
   * @return
   */
  final public static HashtableOfTerm makeHashtableOf() {
    // TODO Auto-generated method stub
    return new HashtableOfTerm();
  }

  final public static NumberTerm makeInteger(Number c) {
    IntegerTerm it = (IntegerTerm) javaLang2TermMap.get(c);
    if (it == null) {
      it = new IntegerTerm(c);
      javaLang2TermMap.put(c, it);
    }
    return it;
  }

  /**
   * @param userInput
   * @return
   */

  final public static JavaObjectTerm makeJavaObject(Object userInput) {
    // TODO Auto-generated method stub
    return new JavaObjectTermBase(userInput);
  }

  final public static ListTerm makeList(Object car, Object cdr) {
    return new ListTermBase(car, cdr);
  }

  final public static StructureTerm makeStructure(Object s, Object[] args) {
    return new StructureTermBase((/*SymbolTerm*/Object) s, args);
  }

  public static Object makeSymbol(String n) {
    return SymbolTermBase.makeSymbol(n);
  }

  /**
   * @param string
   * @param i
   * @return
   */
  public static/*SymbolTerm*/Object makeSymbol(String string, int i) {
    // TODO Auto-generated method stub
    return SymbolTermBase.makeSymbol(string, i);
  }

  public static final Term makeTerm(final Object o) {
    if (TermBase.instanceOfTerm(o)) {
      return (Term) o;
    }
    if (o instanceof Term) {
      return (Term) o;
    }
    if (o instanceof String) {
      return (SymbolTerm) makeAtom(o.toString());
    }
    if (o instanceof Number) {
      Number n = (Number) o;
      if (n.intValue() == n.hashCode()) {
        return makeInteger(n.longValue());
      }
      return makeDouble(n.doubleValue());
    }

    if (false) if (o instanceof Vector) {
      final Vector v = (Vector) o;
      Object t = Prolog.Nil;
      for (int i = v.size(); i > 0; i--) {
        t = makeList(makeTerm(v.elementAt(i - 1)), t);
      }
      return (Term) t;
    }
    if (false) if (o instanceof List) {
      final List v = (List) o;
      Object t = Prolog.Nil;
      for (int i = v.size(); i > 0; i--) {
        t = makeList(makeTerm(v.get(i - 1)), t);
      }
      return (Term) t;
    }
    return makeJavaObject(o);
  }

  /**
   * @param a1
   * @return
   */
  final public static String nameUQ(Object a1) {
    // TODO Auto-generated method stub
    //functor(a1)
    if (a1 instanceof StructureTerm) {
      return ((StructureTermBase) a1).nameUQ();
    }
    if (a1 instanceof ListTerm) {
      ListTermBase r = ((ListTermBase) a1);
      // TODO Auto-generated method stub
      return r.nameUQ();
    }
    if (a1 instanceof SymbolTerm) {
      return ((SymbolTermBase) a1).nameUQ();
    }
    if (a1 instanceof VariableTermBase) {
      return ((VariableTermBase) a1).nameUQ();
    }
    return a1.toString();
  }

  /**
   * @return
   */
  final public static IdentityLinkedList newIdentityLinkedList() {
    // TODO Auto-generated method stub
    return new IdentityLinkedList();
  }

  final public static Object object(Object evaluate) {
    // TODO Auto-generated method stub
    return toJava(evaluate);//si2);
  }

  /* (Object thiz,non-Javadoc)
   * @see java.lang.Object#equals(Object thiz,java.lang.Object)
   */
  // @Override
  final public static boolean prologEquals(Object thiz, Object obj) {
    if (thiz == obj) return true;
    if (thiz instanceof Term) return ((Term) thiz).equalJProlog(obj);
    if (obj instanceof Term) return ((Term) obj).equalJProlog(thiz);
    return thiz.equals(obj);

  }// throws Throwable;

  //  final public static  /*SymbolTerm*/Object functor(Object obj) {
  //    return obj.functor();//args();
  //  }

  /**
   * @param newParam TODO
   * @return
   */
  final public static int prologHashCode(Object thiz, Collection newParam) {
    return thisTerm(thiz).hashCode(40);
  }

  /**
   * @param x
   * @return
   */
  final public static Object rest(Object x) {
    // TODO Auto-generated method stub
    return ((ListTerm) x).rest();
  }

  /**
   * @param x
   * @param deref
   */
  final public static void setCar(Object x, Object deref) {
    ((ListTerm) x).setCar(deref);
    // TODO Auto-generated method stub

  }

  /**
   * @param x
   * @param deref
   */
  final public static void setCdr(Object x, Object deref) {
    ((ListTerm) x).setCdr(deref);
    // TODO Auto-generated method stub

  }

  /**
   * @param thiz
   * @return
   */
  private static Term thisTerm(Object thiz) {
    if (thiz instanceof Term)
    // TODO Auto-generated method stub
      return (Term) thiz;
    return makeTerm(thiz);// new JavaObjectTermBase(thiz);
  }

  /**
   * @param a1
   * @return
   */
  final public static Object toJava(Object a1) {
    // TODO Auto-generated method stub
    if (a1 instanceof Term) return (((Term) a1).toJava());
    return a1;
  }

  /**
   * @param a1
   * @return
   */
  final public static String toQuotedString(Object a1) {
    // TODO Auto-generated method stub
    if (a1 instanceof Term) return thisTerm(a1).toQuotedString();
    return Token.toQuotedString("" + a1);
  }

  final public static String toString(Object thiz) {
    if (thiz instanceof Term) return thisTerm(thiz).toStringImpl(40);
    return thiz.toString();
  }// throws Throwable;

  final public static boolean unify(Object thiz, Object o) {
    if (thiz == o) return true;
    if (thiz instanceof Term) return ((Term) thiz).unifyImpl(o);
    if (o instanceof Term) return ((Term) o).unifyImpl(thiz);
    if (isNonTerm(thiz)) return prologEquals(thiz, o);
    return thisTerm(thiz).unifyImpl(o);
  }//throws Throwable;

}
