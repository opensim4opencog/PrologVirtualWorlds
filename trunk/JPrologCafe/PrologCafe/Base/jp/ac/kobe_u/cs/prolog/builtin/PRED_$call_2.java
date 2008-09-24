/* PRED_$call_2 - at Sun Sep 21 05:38:55 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 11 more classes */
/* import java.lang.IllegalArgumentException; */
/* import java.lang.InstantiationException; */
/* import java.lang.Object; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$call_2; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import java.lang.SecurityException; */
/* import java.lang.Class; */
/* import java.lang.NoSuchMethodException; */
/* import java.lang.IllegalAccessException; */
/* import java.lang.ClassNotFoundException; */

class PRED_$call_2 extends PredicateBase {
  /**
   * 
   */
  private static final long serialVersionUID = 4685706198992180131L;
  Object arg1;
  Object arg2;
  Predicate cont;
  public static /*SymbolTerm*/Object SYM_SLASH_2 = makeAtom("/", 2);

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_$call_2(Object a1, Object a2, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
  }

  public PRED_$call_2() {
    /* flow_0_0_EmptyBlock_509335928
    */
    /* empty */}

  @Override
  public String toString() {
    /* flow_0_0_StringAddOperator
    */
    return "$call(" + this.arg1 + "," + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    final Object a1 = deref(this.arg1);
    final Object a2 = deref(this.arg2);
    try {
      if (!isAtomTerm(a1)) throw new IllegalTypeException(this, 1, "atom", a1);
      String functor;
      Object[] args;
      int arity;
      if (isAtomTerm(a2)) {
        functor = nameUQ(a2);
        args = null;
        arity = 0;
      } else if (isCompound(a2)) {
        functor = nameUQ(jp.ac.kobe_u.cs.prolog.lang.StructureTermBase.functor((Object) a2));
        args = args(a2);
        arity = arity(a2);
      } else
        throw new IllegalTypeException(this, 2, "callable", a2);
      Class clazz;
      try {
        clazz = engine.pcl.loadPredicateClass(nameUQ(a1), functor, arity, true);
      } catch (final ClassNotFoundException e) {
        try {
          clazz = engine.pcl.loadPredicateClass("jp.ac.kobe_u.cs.prolog.builtin", functor, arity, true);
        } catch (final ClassNotFoundException ee) {
          if (engine.getUnknown().equals("fail")) return this.fail(engine);
          final Object[] fa = { makeAtom(functor), makeInteger(arity) };
          throw new ExistenceException(this, 0, "procedure", makeStructure(PRED_$call_2.SYM_SLASH_2, fa), "");
        }
      }
      final Constructor constr = clazz.getConstructor(new Class[0]);
      constr.setAccessible(true);
      final Predicate pred = (Predicate) constr.newInstance(new Object[0]);
      pred.setArgument(args, this.cont);
      return pred;
    } catch (final NoSuchMethodException e) {
      throw new SystemException(e.toString() + " in " + this.toString());
    } catch (final InstantiationException e) {
      throw new SystemException(e.toString() + " in " + this.toString());
    } catch (final IllegalAccessException e) {
      throw new SystemException(e.toString() + " in " + this.toString());
    } catch (final SecurityException e) {
      throw new SystemException(e.toString() + " in " + this.toString());
    } catch (final IllegalArgumentException e) {
      throw new SystemException(e.toString() + " in " + this.toString());
    } catch (final InvocationTargetException e) {
      throw new SystemException(e.toString() + " in " + this.toString());
    }
  }

  @Override
  public void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = args[0];
    this.arg2 = args[1];
    this.cont = cont;
  }
}
