/**
 *
 */
package jp.ac.kobe_u.cs.prolog.lang.impl;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Vector;

import jp.ac.kobe_u.cs.prolog.lang.HashtableOfTerm;
import jp.ac.kobe_u.cs.prolog.lang.InternalException;
import jp.ac.kobe_u.cs.prolog.lang.Predicate;
import jp.ac.kobe_u.cs.prolog.lang.PredicateEncoder;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.PrologMethods;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.Token;
import jp.ac.kobe_u.cs.prolog.lang.Trail;
import jp.ac.kobe_u.cs.prolog.lang.Undoable;
import jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation;

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
    final Term t = this.makeTerm(tin);
    return StaticProlog.isCompound(t) ? ((StructureTerm) t).arity() : ((SymbolTerm) t).arity();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#bind(java.lang.Object, java.lang.Object)
   */
  final public boolean bind(Object thiz, Object that) {
    final VariableTerm r = ((VariableTerm) thiz);
    if (r == that) {
      return true;
    }
    final Undoable undoable;
    final Prolog machine = r.getMachine();
    final Trail trail = machine.trail;

    if (StaticProlog.isVariable(that) && (((VariableTerm) that).timeStamp() >= r.timeStamp())) {

      if (((VariableTerm) that).timeStamp() < trail.engine.stack_getTimeStamp()) {
        undoable = ((VariableTerm) that).getUndoable();
        trail.push(undoable);
      }
      ((VariableTerm) that).setVal(r);
    } else {

      if (r.timeStamp() < trail.engine.stack_getTimeStamp()) {
        undoable = ((VariableTerm) r).getUndoable();
        trail.push(undoable);
      }
      r.setVal(that);
    }
    return true;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#compareTo(java.lang.Object, java.lang.Object)
   */
  final public int compareTerm(Object left, Object o) {
    if (left == o) return Term.EQUAL;
    // TODO Auto-generated method stub
    if (left instanceof Term) return ((Term) left).compareTo(o);
    if (o instanceof Term) return -(((Term) o).compareTo(left));
    //////             ATOM         //////////
    if (isAtomTerm(left)) {
      ///SymbolTerm left = ((SymbolTermBase)left);
      if (StaticProlog.isVariable(o) || StaticProlog.isNumber(o)) return Term.AFTER;
      if (!StaticProlog.isAtomTerm(o)) return Term.BEFORE;
      //above if (left == o) return Term.EQUAL;
      final int x = left.toString().compareTo(StaticProlog.nameUQ((o)));
      if (x != 0) return x;
      final int y = arity(left) - StaticProlog.arity(o);
      if (y != 0) return y;
      throw new InternalException("SymbolTerm is not unique");
    }
    //////             LIST         //////////
    if (isListTerm(left)) {
      ListTerm r = ((ListTermBase) left);
      if (StaticProlog.isVariable(o) || StaticProlog.isNumber(o) || StaticProlog.isAtomTerm(o)) return Term.AFTER;
      if (StaticProlog.isCompound(o)) {
        final int arity = StaticProlog.arity(o);
        if (2 != arity) return (2 - arity);
        /*SymbolTerm*/final Object functor = StaticProlog.functor(o);
        if (!StaticProlog.prologEquals(ListTermBase.SYM_DOT, functor)) return StaticProlog.compareTerm(ListTermBase.SYM_DOT, functor);
      }
      Object[] args = new Object[2];
      if (StaticProlog.isListTerm(o)) {
        args[0] = StaticProlog.first((o));
        args[1] = StaticProlog.rest(o);//.rest();
      } else if (StaticProlog.isCompound(o)) {
        args = StaticProlog.args(o);
      } else {
        return Term.BEFORE;
      }
      Object tmp = first(r);
      int rc;
      for (int i = 0; i < 2; i++) {
        rc = StaticProlog.compareTerm(tmp, StaticProlog.deref(args[i]));
        if (rc != Term.EQUAL) return rc;
        tmp = rest(r);//.cdr;
      }
      return Term.EQUAL;
    }
    //////           COMPOUND         //////////
    if (isCompound(left)) {
      ///    StructureTerm left = ((StructureTerm)left);
      /*SymbolTerm*/Object functor2;
      Object[] args2;
      int arity2, rc;

      if (StaticProlog.isVariable(o) || StaticProlog.isNumber(o) || StaticProlog.isAtomTerm(o)) return Term.AFTER;
      if (StaticProlog.isListTerm(o)) {
        functor2 = ListTerm.SYM_DOT;
        args2 = new Object[2];
        args2[0] = StaticProlog.first(o);
        args2[1] = StaticProlog.rest(o);
        arity2 = 2;
      } else if (StaticProlog.isCompound(o)) {
        functor2 = StaticProlog.functor(o);
        args2 = StaticProlog.args(o);
        arity2 = StaticProlog.arity(o);
      } else {
        return Term.BEFORE;
      }
      int rarity = arity(left);
      Object rfunctor = functor(left);
      Object[] rargs = args(left);
      if (rarity != arity2) return (rarity - arity2);
      if (!StaticProlog.prologEquals(rfunctor, functor2)) return StaticProlog.compareTerm(rfunctor, functor2);
      for (int i = 0; i < rarity; i++) {
        rc = StaticProlog.compareTerm(rargs[i], StaticProlog.deref(args2[i]));
        if (rc != Term.EQUAL) return rc;
      }
      return Term.EQUAL;
    }

    throw new NoSuchMethodError(left.getClass().getName() + ".compareTerm " + left + " to " + o);

  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#consArgs(java.lang.Object)
   */
  final public Object[] consArgs(Object obj) {
    final ListTerm ll = (ListTerm) obj;
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
    return this.makeTerm(thiz).convertible(to);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#copy(java.lang.Object, jp.ac.kobe_u.cs.prolog.lang.Prolog)
   */
  public Object copy(Object thiz) {
    if (thiz instanceof Term) return ((Term) thiz).copy();
    //  if (thiz instanceof Cloneable) return (Object)((Cloneable) thiz).clone();
    if (thiz instanceof String) return thiz;
    if (StaticProlog.isJavaObject(thiz)) return thiz;
    return this.makeTerm(thiz).copy();
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
    return ((Number) StaticProlog.toJava(t)).doubleValue();
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
    return StaticProlog.prologEquals(s2, StaticProlog.functor(a3));
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
    return StaticProlog.toJava(thiz).getClass();
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
    return StaticProlog.hashCode(thiz, 40);
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
    return (obj instanceof VariableTerm) || (obj instanceof NumberTerm) || (obj instanceof SymbolTerm) || (obj instanceof ListTerm) || (obj instanceof StructureTerm) || (obj instanceof JavaObjectTerm) || (obj instanceof ClosureTerm);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#intValue(java.lang.Object)
   */
  final public long intValue(Object obj) {
    return ((Number) StaticProlog.toJava(obj)).longValue();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isAtomTerm(java.lang.Object)
   */
  final public boolean isAtomTerm(Object thiz) {
    if (thiz instanceof CharSequence) return true;
    if (thiz instanceof Number) return false;
    if (thiz instanceof Class) return true;
    if (thiz instanceof SymbolTerm) return true;
    //    
    //    final Term t = this.makeTerm(thiz);
    //    if (t.isAtomTerm()) return true;
    //    if (t instanceof SymbolTerm) {
    //      if (StaticProlog.isVariable(t)) return false;
    //      return false;
    //    }
    //    if (true) return false;
    return thiz instanceof SymbolTerm;// thisTerm(thiz).isAtomTerm();
  }//{ return this instanceof /*SymbolTerm*/Object; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isClosure(java.lang.Object)
   */
  final public boolean isClosure(Object thiz) {
    if (thiz instanceof String) return false;
    if (thiz instanceof Number) return false;
    return this.makeTerm(thiz).isClosure();
  }// { return this instanceof ClosureTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isCompound(java.lang.Object)
   */
  final public boolean isCompound(Object thiz) {
    if (thiz instanceof String) return false;
    if (thiz instanceof Number) return false;
    return this.makeTerm(thiz).isCompound();
  }//{ return this instanceof StructureTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isCutter(java.lang.Object)
   */
  final public boolean isCutter(Object a3) {
    // TODO Auto-generated method stub
    return StaticProlog.isInteger(a3);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isDouble(java.lang.Object)
   */
  final public boolean isDouble(Object thiz) {
    thiz = StaticProlog.toJava(thiz);
    if (thiz instanceof Number) {
      final Number n = (Number) thiz;
      if (n.hashCode() != n.intValue()) return true;
      return false;
    }
    return false;
  }//{ return this instanceof DoubleTerm; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isGround(java.lang.Object)
   */
  final public boolean isGround(Object thiz) {
    if (thiz instanceof Number) return true;
    if (thiz instanceof String) return true;
    if (StaticProlog.isJavaObject(thiz)) return true;
    return this.makeTerm(thiz).isGround();

  }//{ return true; }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#isInteger(java.lang.Object)
   */
  final public boolean isInteger(Object thiz) {
    thiz = StaticProlog.toJava(thiz);
    if (thiz instanceof Number) {
      final Number n = (Number) thiz;
      if (n.hashCode() == n.intValue()) return true;
      return false;
    }
    return false;
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
    return this.makeTerm(thiz).isNil();
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
    return (toJava(thiz) instanceof Number);//return true;
    //return this.makeTerm(thiz).isNumber();

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
    while (StaticProlog.isListTerm(thiz)) {
      count++;
      thiz = StaticProlog.deref(StaticProlog.rest(thiz));
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
  final public Number makeDouble(Number c) {
    //    NumberTerm it = null;// (NumberTerm) javaLang2TermMap.get(c);
    //    if (it == null) {
    //      it = new NumberTerm(c);
    //      //  javaLang2TermMap.put(c, it);
    //    }
    //    return it;
    return c;
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
  final public Number makeInteger(Number c) {
    //    if (isDouble(c)) {
    //      throw new Error("non integer: " + c);
    //    }
    NumberTerm it = (NumberTerm) this.javaLang2TermMap.get(c);
    if (it == null) {
      it = new NumberTerm(c) {

        /**
         *
         */
        private static final long serialVersionUID = -7130847454857024319L;
      };
      this.javaLang2TermMap.put(c, it);
    }
    return it;
    //return c;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeJavaObject(java.lang.Object)
   */
  final public Object makeJavaObject(Object userInput) {
    return userInput;
    // return new JavaObjectTermBase(userInput);
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
  final public Object makeStructure(Object s, final Object[] args) {
    if (s instanceof Class) {
      final Class c = ((Class) s);
      try {
        if ((args == null) || (args.length == 0)) return c.newInstance();
        if (true) {
          return c.getConstructors()[0].newInstance(args);
        }
        return c.getConstructor(this.toClasses(args)).newInstance(args);
      } catch (final InstantiationException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (final IllegalAccessException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (final IllegalArgumentException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (final SecurityException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (final InvocationTargetException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (final NoSuchMethodException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      }
    }
    return new StructureTermBase(s, args) {
    };
  }

  /**
   * @param args
   * @return
   */
  private Class[] toClasses(Object[] args) {
    if ((args == null) || (args.length == 0)) return new Class[0];
    final Class[] argsc = new Class[args.length]; // TODO Auto-generated method stub
    for (int i = 0; i < args.length; i++) {
      argsc[i] = StaticProlog.getClass(args[i]);//.getClass();
    }
    return argsc;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeSymbol(java.lang.String)
   */
  public Object makeSymbol(String n) {
    return StaticProlog.makeAtom(n);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#makeTerm(java.lang.Object)
   */
  public final Term makeTerm(final Object o) {
    if (true) return (Term) o;
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
      return (Term) o;
      //      Number n = (Number) o;
      //      if (n.intValue() == n.hashCode()) {
      //        return makeInteger(n);
      //      }
      //      return makeDouble(n);
    }

    if (false) if (o instanceof Vector) {
      final Vector v = (Vector) o;
      Object t = Prolog.Nil;
      for (int i = v.size(); i > 0; i--) {
        t = StaticProlog.makeList(this.makeTerm(v.elementAt(i - 1)), t);
      }
      return (Term) t;
    }
    if (false) if (o instanceof List) {
      final List v = (List) o;
      Object t = Prolog.Nil;
      for (int i = v.size(); i > 0; i--) {
        t = StaticProlog.makeList(this.makeTerm(v.get(i - 1)), t);
      }
      return (Term) t;
    }
    return (Term) StaticProlog.makeJavaObject(o);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#nameUQ(java.lang.Object)
   */
  final public String nameUQ(Object thiz) {
    // TODO Auto-generated method stub
    //functor(thiz)
    if (thiz instanceof Class) {
      return PredicateEncoder.decodeName((Class) thiz);
    }
    if (thiz instanceof StructureTerm) {
      return ((StructureTermBase) thiz).nameUQ();
    }
    if (thiz instanceof ListTerm) {
      final ListTermBase r = ((ListTermBase) thiz);
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
    if (thiz instanceof String) return thiz.equals(obj);
    if (thiz.getClass() != obj.getClass()) {
      if (thiz.equals(obj)) {
        throw new Error(thiz + "==" + obj);
      }
      return false;
    }
    return thiz.equals(obj);
  }// throws Throwable;

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
    if (thiz instanceof Term) return this.makeTerm(thiz).toQuotedString(400);
    if (thiz instanceof String) return Token.toQuotedString(thiz.toString());
    return Token.toQuotedString("" + thiz);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#toString(java.lang.Object)
   */
  final public String toString(Object thiz) {
    if (thiz instanceof Term) return this.makeTerm(thiz).toStringImpl(40);
    return thiz.toString();
  }// throws Throwable;

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.IPrologMethods#unify(java.lang.Object, java.lang.Object)
   */
  final public boolean unify(Object thiz, Object o) {
    if (thiz == o) return true;
    if (thiz instanceof Term) return ((Term) thiz).unify(o);
    if (o instanceof Term) return ((Term) o).unify(thiz);
    if (this.isNonTerm(thiz)) {
      return StaticProlog.prologEquals(thiz, o);
    }
    return this.makeTerm(thiz).unify(o);
  }//throws Throwable;

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.StaticProlog.PrologMethods#makeVariable(jp.ac.kobe_u.cs.prolog.lang.Prolog)
   */
  @Override
  public VariableTerm makeVariable(Prolog prolog) {
    // TODO Auto-generated method stub
    return new VariableTermBase(prolog, new VariableTermLocation() {
      Object value = unboundInit;

      /* (non-Javadoc)
       * @see jp.ac.kobe_u.cs.prolog.lang.impl.VariableTermLocation#isBound()
       */
      @Override
      public boolean isBound() {
        // TODO Auto-generated method stub
        return value != unboundInit;
      }

      @Override
      public Object getVal() {
        // TODO Auto-generated method stub
        return value;
      }

      @Override
      public void setVal(Object o) {
        value = o;
      }
    });
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.StaticProlog.PrologMethods#getCode(java.lang.Object)
   */
  @Override
  public Predicate getCode(Object a1) {
    // TODO Auto-generated method stub
    return ((ClosureTerm) a1).getCode();
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.PrologMethods#makeVariableLoc(jp.ac.kobe_u.cs.prolog.lang.impl.VariableTermLocation)
   */
  @Override
  public Object makeVariableLoc(VariableTermLocation a) {
    return new VariableTermBase(VariableTermBase.machine, a);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.PrologMethods#toQuotedString(java.lang.Object, int)
   */
  @Override
  public String toQuotedString(Object arg1, int d0) {
    // TODO Auto-generated method stub
    if (arg1 instanceof Term) return ((Term) arg1).toQuotedString(d0);
    return toQuotedString(arg1);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.PrologMethods#toString(java.lang.Object, int)
   */
  @Override
  public String toString(Object arg1, int d0) {
    // TODO Auto-generated method stub
    if (arg1 instanceof Term) return ((Term) arg1).toStringImpl(d0);
    return toString(arg1);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.PrologMethods#arg0(java.lang.Object, int)
   */
  @Override
  public Object arg0(Object obj, int i) {
    // TODO Auto-generated method stub
    return ((StructureTerm) obj).arg0(i);
  }

}
