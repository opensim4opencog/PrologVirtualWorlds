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
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog.PrologMethods;

import sun.awt.util.IdentityLinkedList;

/**
 * @author root
 *
 */
public class StaticPrologMethodImpl implements PrologMethods {
  /**
   * @param ceil
   * @return
   */
  final HashMap javaLang2TermMap = new HashMap();

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#args(java.lang.Object)
   */
  final public Object[] args(Object obj) {
    return ((StructureTerm) obj).args();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#arity(java.lang.Object)
   */
  final public int arity(Object tin) {
    Term t = thisTerm(tin);
    return isCompound(t) ? ((StructureTerm) t).arity() : ((SymbolTerm) t).arity();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#bind(java.lang.Object, java.lang.Object)
   */
  final public boolean bind(Object a1, Object si1) {
    VariableTermBase r = ((VariableTermBase) a1);
    if (r == si1) {
      return true;
    }
    Prolog machine = r.machine;
    Trail trail = machine.trail;
    if (VariableTermBase.isVariable(si1) && ((VariableTerm) si1).timeStamp() >= r.timeStamp()) {
      ((VariableTerm) si1).setVal(r);
      if (((VariableTerm) si1).timeStamp() < trail.engine.stack_getTimeStamp()) {
        trail.push((VariableTerm) si1);
      }
    } else {
      r.setVal(si1);
      if (r.timeStamp() < trail.engine.stack_getTimeStamp()) {
        trail.push(r);
      }
    }
    return true;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#compareTo(java.lang.Object, java.lang.Object)
   */
  final public int compareTerm(Object arg1, Object arg2) {
    // TODO Auto-generated method stub
    return thisTerm(arg1).compareTo(arg2);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#consArgs(java.lang.Object)
   */
  final public Object[] consArgs(Object obj) {
    ListTerm ll = (ListTerm) obj;
    return new Object[] { ll.first(), ll.rest() };
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#convertible(java.lang.Class, java.lang.Class)
   */
  final public boolean convertible(Class from, Class<?> to) {
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

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#convertible(java.lang.Object, java.lang.Class)
   */

  public boolean convertible(Object thiz, Class to) {
    // TODO Auto-generated method stub
    return thisTerm(thiz).convertible(to);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#copy(java.lang.Object, jp.ac.kobe_u.cs.prolog.lang.Prolog)
   */
  public Object copy(Object thiz, Prolog engine) {
    // TODO Auto-generated method stub
    if (thiz instanceof String) return thiz;
    if (isJavaObject(thiz)) return thiz;
    return thisTerm(thiz).copy(engine);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#deref(java.lang.Object)
   */
  final public Object deref(Object thiz) {
    if (thiz instanceof Term) return ((Term) thiz).deref();
    return thiz;
  }//{ return this; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#doubleValue(java.lang.Object)
   */
  public double doubleValue(Object t) {
    // TODO Auto-generated method stub
    if (isNumber(t)) {
      return ((Number) toJava(t)).doubleValue();
    }
    return t.hashCode();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#first(java.lang.Object)
   */
  final public Object first(Object x) {
    // TODO Auto-generated method stub
    return ((ListTerm) x).first();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#functor(java.lang.Object)
   */
  final public/*SymbolTerm*/Object functor(Object t) {
    // TODO Auto-generated method stub
    return ((StructureTerm) t).functor();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#functorOf(java.lang.Object, java.lang.Object)
   */
  final public boolean functorOf(Object s2, Object a3) {
    // TODO Auto-generated method stub
    return prologEquals(s2, functor(a3));
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#getClass(java.lang.Object)
   */
  final public Class getClass(Object evaluate) {
    // TODO Auto-generated method stub
    return evaluate.getClass();//si2);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#getClazz(java.lang.Object)
   */
  final public Class getClazz(Object a1) {
    // TODO Auto-generated method stub
    return toJava(a1).getClass();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#getName(java.lang.Class)
   */
  final public String getName(Class c) {
    // TODO Auto-generated method stub
    return c.getName();
    //return evaluate.arithCompareTo(si2);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#hashCode(java.lang.Object)
   */
  final public int hashCode(Object a1) {
    // TODO Auto-generated method stub
    return hashCode(a1, 40);
  }

  //
  //  final public  boolean prologEquals(Object thiz, Object o) {
  //    return thisTerm(thiz).equalJProlog(o);
  //  }//throws Throwable;

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#hashCode(java.lang.Object, int)
   */
  final public int hashCode(Object a1, int depth) {
    // TODO Auto-generated method stub
    if (a1 instanceof Term) return ((Term) a1).hashCode(depth);
    return a1.hashCode();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#instanceOfTerm(java.lang.Object)
   */
  final public boolean instanceOfTerm(Object obj) {
    return obj instanceof VariableTerm || obj instanceof IntegerTerm || obj instanceof NumberTerm || obj instanceof SymbolTerm || obj instanceof ListTerm || obj instanceof StructureTerm || obj instanceof JavaObjectTerm || obj instanceof ClosureTerm;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#intValue(java.lang.Object)
   */
  final public long intValue(Object obj) {
    return ((NumberTerm) obj).longValue();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isAtomTerm(java.lang.Object)
   */
  final public boolean isAtomTerm(Object thiz) {
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

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isClosure(java.lang.Object)
   */
  final public boolean isClosure(Object thiz) {
    if (thiz instanceof String) return false;
    return thisTerm(thiz).isClosure();
  }// { return this instanceof ClosureTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isCompound(java.lang.Object)
   */
  final public boolean isCompound(Object thiz) {
    if (thiz instanceof String) return false;
    return thisTerm(thiz).isCompound();
  }//{ return this instanceof StructureTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isCutter(java.lang.Object)
   */
  final public boolean isCutter(Object a3) {
    // TODO Auto-generated method stub
    return isInteger(a3);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isDouble(java.lang.Object)
   */
  final public boolean isDouble(Object thiz) {
    if (thiz instanceof Number) {
      Number n = (Number) thiz;
      if (n.hashCode() != n.intValue()) return true;
      return false;
    }
    if (thiz instanceof String) return false;
    return thisTerm(thiz).isDouble();
  }//{ return this instanceof DoubleTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isGround(java.lang.Object)
   */
  final public boolean isGround(Object thiz) {
    if (isJavaObject(thiz)) return true;
    return thisTerm(thiz).isGround();

  }//{ return true; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isInteger(java.lang.Object)
   */
  final public boolean isInteger(Object thiz) {
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

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isJavaObject(java.lang.Object)
   */
  final public boolean isJavaObject(Object thiz) {
    if (thiz instanceof Term) return ((Term) thiz).isJavaObject();
    return true;
  }// { return this instanceof JavaObjectTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isListTerm(java.lang.Object)
   */
  final public boolean isListTerm(Object thiz) {
    if (thiz instanceof Term) return ((Term) thiz).isListTerm();
    return thiz instanceof Collection;
  }//{ return this instanceof ListTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isNil(java.lang.Object)
   */
  final public boolean isNil(Object thiz) {
    if (thiz == null) return true;
    if (thiz instanceof String) return "[]".equals(thiz);
    return thisTerm(thiz).isNil();
  }//{ return Prolog.Nil.equals(Object thiz,this){} }

  /**
   * @param thiz
   * @return
   */
  private boolean isNonTerm(Object thiz) {
    // TODO Auto-generated method stub
    if (thiz instanceof Term) return false;
    return true;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isNumber(java.lang.Object)
   */
  final public boolean isNumber(Object thiz) {
    if (thiz instanceof Number) return true;
    return thisTerm(thiz).isNumber();

  }// { return (Object thiz,(Object thiz,this instanceof IntegerTerm) || (Object thiz,this instanceof DoubleTerm)){} }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isVariable(java.lang.Object)
   */
  final public boolean isVariable(Object thiz) {
    return (thiz instanceof VariableTerm);// thisTerm(thiz).isVariable();

  }//{ return this instanceof VariableTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#listLength(java.lang.Object)
   */
  final public int listLength(Object x) {
    // TODO Auto-generated method stub
    int count = 0;
    while (ListTermBase.isListTerm(x)) {
      count++;
      x = ListTermBase.deref(ListTermBase.rest(x));
    }
    return count;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeAtom(java.lang.String)
   */
  final public/*SymbolTerm*/Object makeAtom(Object s) {
    return SymbolTermBase.makeSymbol(s);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeAtom(java.lang.String, int)
   */
  final public/*SymbolTerm*/Object makeAtom(Object s, int i) {
    return SymbolTermBase.makeSymbol(s, i);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeClosure(jp.ac.kobe_u.cs.prolog.lang.Predicate)
   */
  final public ClosureTerm makeClosure(Predicate pred) {
    // TODO Auto-generated method stub
    return new ClosureTermBase(pred);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeDouble(java.lang.Number)
   */
  final public NumberTerm makeDouble(Number c) {
    NumberTerm it = (NumberTerm) javaLang2TermMap.get(c);
    if (it == null) {
      it = new NumberTerm(c);
      javaLang2TermMap.put(c, it);
    }
    return it;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeHashtableOf()
   */
  final public HashtableOfTerm makeHashtableOf() {
    // TODO Auto-generated method stub
    return new HashtableOfTerm();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeInteger(java.lang.Number)
   */
  final public NumberTerm makeInteger(Number c) {
    IntegerTerm it = (IntegerTerm) javaLang2TermMap.get(c);
    if (it == null) {
      it = new IntegerTerm(c);
      javaLang2TermMap.put(c, it);
    }
    return it;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeJavaObject(java.lang.Object)
   */

  final public JavaObjectTerm makeJavaObject(Object userInput) {
    // TODO Auto-generated method stub
    return new JavaObjectTermBase(userInput);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeList(java.lang.Object, java.lang.Object)
   */
  final public ListTerm makeList(Object car, Object cdr) {
    return new ListTermBase(car, cdr);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeStructure(java.lang.Object, java.lang.Object[])
   */
  final public StructureTerm makeStructure(Object s, Object[] args) {
    return new StructureTermBase((/*SymbolTerm*/Object) s, args);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeSymbol(java.lang.String)
   */
  public Object makeSymbol(String n) {
    return SymbolTermBase.makeAtom(n);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeTerm(java.lang.Object)
   */
  public final Term makeTerm(final Object o) {
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
        return makeInteger(n);
      }
      return makeDouble(n);
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

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#nameUQ(java.lang.Object)
   */
  final public String nameUQ(Object a1) {
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

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#newIdentityLinkedList()
   */
  final public IdentityLinkedList newIdentityLinkedList() {
    // TODO Auto-generated method stub
    return new IdentityLinkedList();
  }

  /* (Object thiz,non-Javadoc)
   * @see java.lang.Object#equals(Object thiz,java.lang.Object)
   */
  // @Override
  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#prologEquals(java.lang.Object, java.lang.Object)
   */
  final public boolean prologEquals(Object thiz, Object obj) {
    if (thiz == obj) return true;
    if (thiz instanceof Term) return ((Term) thiz).equalJProlog(obj);
    if (obj instanceof Term) return ((Term) obj).equalJProlog(thiz);
    return thiz.equals(obj);

  }// throws Throwable;

  //  final public   /*SymbolTerm*/Object functor(Object obj) {
  //    return obj.functor();//args();
  //  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#prologHashCode(java.lang.Object, java.util.Collection)
   */
  final public int prologHashCode(Object thiz, Collection newParam) {
    return thisTerm(thiz).hashCode(40);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#rest(java.lang.Object)
   */
  final public Object rest(Object x) {
    // TODO Auto-generated method stub
    return ((ListTerm) x).rest();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#setCar(java.lang.Object, java.lang.Object)
   */
  final public void setCar(Object x, Object deref) {
    ((ListTerm) x).setCar(deref);
    // TODO Auto-generated method stub

  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#setCdr(java.lang.Object, java.lang.Object)
   */
  final public void setCdr(Object x, Object deref) {
    ((ListTerm) x).setCdr(deref);
    // TODO Auto-generated method stub

  }

  /**
   * @param thiz
   * @return
   */
  private Term thisTerm(Object thiz) {
    if (thiz instanceof Term)
    // TODO Auto-generated method stub
      return (Term) thiz;
    return makeTerm(thiz);// new JavaObjectTermBase(thiz);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#toJava(java.lang.Object)
   */
  final public Object toJava(Object a1) {
    // TODO Auto-generated method stub
    if (a1 instanceof Term) return (((Term) a1).toJava());
    return a1;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#toQuotedString(java.lang.Object)
   */
  final public String toQuotedString(Object a1) {
    // TODO Auto-generated method stub
    if (a1 instanceof Term) return thisTerm(a1).toQuotedString();
    return Token.toQuotedString("" + a1);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#toString(java.lang.Object)
   */
  final public String toString(Object thiz) {
    if (thiz instanceof Term) return thisTerm(thiz).toStringImpl(40);
    return thiz.toString();
  }// throws Throwable;

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#unify(java.lang.Object, java.lang.Object)
   */
  final public boolean unify(Object thiz, Object o) {
    if (thiz == o) return true;
    if (thiz instanceof Term) return ((Term) thiz).unifyImpl(o);
    if (o instanceof Term) return ((Term) o).unifyImpl(thiz);
    if (isNonTerm(thiz)) return prologEquals(thiz, o);
    return thisTerm(thiz).unifyImpl(o);
  }//throws Throwable;

}
