/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang.impl;

import java.lang.reflect.InvocationTargetException;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Vector;

import jp.ac.kobe_u.cs.prolog.lang.HashtableOfTerm;
import jp.ac.kobe_u.cs.prolog.lang.Predicate;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.Token;
import jp.ac.kobe_u.cs.prolog.lang.Trail;
import jp.ac.kobe_u.cs.prolog.lang.Undoable;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog.PrologMethods;

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
    Term t = makeTerm(tin);
    return isCompound(t) ? ((StructureTerm) t).arity() : ((SymbolTerm) t).arity();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#bind(java.lang.Object, java.lang.Object)
   */
  final public boolean bind(Object thiz, Object that) {
    VariableTerm r = ((VariableTerm) thiz);
    if (r == that) {
      return true;
    }
    Prolog machine = r.getMachine();
    Trail trail = machine.trail;
    if (isVariable(that) && ((VariableTerm) that).timeStamp() >= r.timeStamp()) {
      ((VariableTerm) that).setVal(r);
      if (((VariableTerm) that).timeStamp() < trail.engine.stack_getTimeStamp()) {
        trail.push((Undoable) that);
      }
    } else {
      r.setVal(that);
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
    return makeTerm(arg1).compareTo(arg2);
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
  final public boolean convertible(Class from, Class to) {
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
    return makeTerm(thiz).convertible(to);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#copy(java.lang.Object, jp.ac.kobe_u.cs.prolog.lang.Prolog)
   */
  public Object copy(Object thiz) {
    if (thiz instanceof Term) return ((Term) thiz).copy();
    //  if (thiz instanceof Cloneable) return (Object)((Cloneable) thiz).clone();
    if (thiz instanceof String) return thiz;
    if (isJavaObject(thiz)) return thiz;
    return makeTerm(thiz).copy();
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
    return ((Number) toJava(t)).doubleValue();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#first(java.lang.Object)
   */
  final public Object first(Object thiz) {
    // TODO Auto-generated method stub
    return ((ListTerm) thiz).first();
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
  final public Class getClazz(Object thiz) {
    // TODO Auto-generated method stub
    return toJava(thiz).getClass();
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
  final public int hashCode(Object thiz) {
    // TODO Auto-generated method stub
    return hashCode(thiz, 40);
  }

  //
  //  final public  boolean prologEquals(Object thiz, Object o) {
  //    return thisTerm(thiz).equalJProlog(o);
  //  }//throws Throwable;

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#hashCode(java.lang.Object, int)
   */
  final public int hashCode(Object thiz, int depth) {
    // TODO Auto-generated method stub
    if (thiz instanceof Term) return ((Term) thiz).hashCode(depth);
    return thiz.hashCode();
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
    return ((Number) toJava(obj)).longValue();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isAtomTerm(java.lang.Object)
   */
  final public boolean isAtomTerm(Object thiz) {
    if (thiz instanceof CharSequence) return true;
    Term t = makeTerm(thiz);
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
    return makeTerm(thiz).isClosure();
  }// { return this instanceof ClosureTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isCompound(java.lang.Object)
   */
  final public boolean isCompound(Object thiz) {
    if (thiz instanceof String) return false;
    return makeTerm(thiz).isCompound();
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
    return makeTerm(thiz).isDouble();
  }//{ return this instanceof DoubleTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isGround(java.lang.Object)
   */
  final public boolean isGround(Object thiz) {
    if (isJavaObject(thiz)) return true;
    return makeTerm(thiz).isGround();

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
    Term term = makeTerm(thiz);
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
    return makeTerm(thiz).isNil();
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
    return makeTerm(thiz).isNumber();

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
  final public int listLength(Object thiz) {
    // TODO Auto-generated method stub
    int count = 0;
    while (ListTermBase.isListTerm(thiz)) {
      count++;
      thiz = ListTermBase.deref(ListTermBase.rest(thiz));
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
  final public Term makeJavaObject(Object userInput) {
    return new JavaObjectTermBase(userInput);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeList(java.lang.Object, java.lang.Object)
   */
  final public Object makeList(Object car, Object cdr) {
    return new ListTermBase(car, cdr);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeStructure(java.lang.Object, java.lang.Object[])
   */
  final public Object makeStructure(Object s, Object[] args) {
    if (s instanceof Class) {
      Class c = ((Class) s);
      try {
        if (args == null || args.length == 0) return c.newInstance();
        return c.getConstructor(toClasses(args)).newInstance(args);
      } catch (InstantiationException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (IllegalAccessException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (IllegalArgumentException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (SecurityException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (InvocationTargetException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (NoSuchMethodException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      }
    }
    return new StructureTermBase((/*SymbolTerm*/Object) s, args);
  }

  /**
   * @param args
   * @return
   */
  private Class[] toClasses(Object[] args) {
    if (args == null || args.length == 0) return new Class[0];
    Class[] argsc = new Class[args.length]; // TODO Auto-generated method stub
    for (int i = 0; i < args.length; i++) {
      argsc[i] = getClass(args);//.getClass();
    }
    return argsc;
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
    if (o instanceof Term) {
      return (Term) o;
    }
    //    if (TermBase.instanceOfTerm(o)) {
    //      return (Term) o;
    //    }
    //    if (o instanceof String) {
    //      return (SymbolTerm) makeAtom(o.toString());
    //    }
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
  final public String nameUQ(Object thiz) {
    // TODO Auto-generated method stub
    //functor(thiz)
    if (thiz instanceof StructureTerm) {
      return ((StructureTermBase) thiz).nameUQ();
    }
    if (thiz instanceof ListTerm) {
      ListTermBase r = ((ListTermBase) thiz);
      // TODO Auto-generated method stub
      return r.nameUQ();
    }
    if (thiz instanceof SymbolTerm) {
      return ((SymbolTermBase) thiz).nameUQ();
    }
    if (thiz instanceof VariableTermBase) {
      return ((VariableTermBase) thiz).nameUQ();
    }
    return thiz.toString();
  }

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

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#prologHashCode(java.lang.Object, java.util.Collection)
   */
  final public int prologHashCode(Object thiz, Collection newParam) {
    return makeTerm(thiz).hashCode(40);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#rest(java.lang.Object)
   */
  final public Object rest(Object thiz) {
    return ((ListTerm) thiz).rest();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#setCar(java.lang.Object, java.lang.Object)
   */
  final public void setCar(Object thiz, Object deref) {
    ((ListTerm) thiz).setCar(deref);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#setCdr(java.lang.Object, java.lang.Object)
   */
  final public void setCdr(Object thiz, Object deref) {
    ((ListTerm) thiz).setCdr(deref);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#toJava(java.lang.Object)
   */
  final public Object toJava(Object thiz) {
    if (thiz instanceof Term) return ((Term) thiz).toJava();
    return thiz;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#toQuotedString(java.lang.Object)
   */
  final public String toQuotedString(Object thiz) {
    // TODO Auto-generated method stub
    if (thiz instanceof Term) return makeTerm(thiz).toQuotedString();
    if (thiz instanceof String) return Token.toQuotedString(thiz.toString());
    return Token.toQuotedString("" + thiz);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#toString(java.lang.Object)
   */
  final public String toString(Object thiz) {
    if (thiz instanceof Term) return makeTerm(thiz).toStringImpl(40);
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
    return makeTerm(thiz).unifyImpl(o);
  }//throws Throwable;

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.StaticProlog.PrologMethods#makeVariable(jp.ac.kobe_u.cs.prolog.lang.Prolog)
   */
  @Override
  public VariableTerm makeVariable(Prolog prolog) {
    // TODO Auto-generated method stub
    return new VariableTermBase(prolog);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.StaticProlog.PrologMethods#getCode(java.lang.Object)
   */
  @Override
  public Predicate getCode(Object a1) {
    // TODO Auto-generated method stub
    return ((ClosureTerm) a1).getCode();
  }

}
