/* PRED_java_constructor0_2 - at Sun Sep 21 05:38:59 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.lang.reflect.Constructor;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 11 more classes */
/* import java.lang.IllegalArgumentException; */
/* import java.lang.InstantiationException; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_java_constructor0_2; */
/* import java.lang.Object; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import java.lang.SecurityException; */
/* import java.lang.Exception; */
/* import java.lang.Class; */
/* import java.lang.IllegalAccessException; */
/* import java.lang.ClassNotFoundException; */

public class PRED_java_constructor0_2 extends JavaPredicate {
  /**
  * 
  */
  private static final long serialVersionUID = -3259673050132890288L;
  Object arg1;
  Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_java_constructor0_2() {
    /* flow_0_0_EmptyBlock_355807572
    */
    /* empty */}

  public PRED_java_constructor0_2(Object a1, Object a2, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
  }

  @Override
  public String toString() {
    /* flow_0_0_StringAddOperator
    */
    return "java_constructor0(" + this.arg1 + "," + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    Class clazz = null;
    Object instance = null;
    Constructor[] constrs = null;
    Object[] pArgs = null;
    Object[] jArgs = null;
    Constructor c = null;
    try {
      a2 = deref(a2);
      if (!isVariable(a2)) throw new IllegalTypeException(this, 2, "variable", a2);
      a1 = deref(a1);
      if (isVariable(a1)) throw new PInstantiationException(this, 1);
      if (!isAtomTerm(a1) && !isCompound(a1)) throw new IllegalTypeException(this, 1, "callable", a1);
      if (isAtomTerm(a1)) {
        clazz = Class.forName(nameUQ(a1));
        instance = clazz.newInstance();
        if (!unify(a2, this.toPrologTerm(instance))) return this.fail(engine);
        return this.cont;
      }
      clazz = Class.forName(nameUQ(a1));
      final int arity = arity(a1);
      constrs = clazz.getConstructors();
      if (constrs.length == 0) throw new ExistenceException(this, 1, "constructor", a1, "");
      pArgs = args(a1);
      jArgs = new Object[arity];
      for (int i = 0; i < arity; i++) {
        pArgs[i] = deref(pArgs[i]);
        if (!isJavaObject(pArgs[i])) pArgs[i] = makeJavaObject(pArgs[i]);
        jArgs[i] = toJava(pArgs[i]);
      }
      for (int i = 0; i < constrs.length; i++) {
        if (this.checkParameterTypes(constrs[i].getParameterTypes(), pArgs)) {
          try {
            c = constrs[i];
            instance = c.newInstance(jArgs);
            break;
          } catch (final Exception e) {
            c = null;
          }
        }
      }
      if (c == null) throw new ExistenceException(this, 1, "constructor", a1, "");
      if (!unify(a2, this.toPrologTerm(instance))) return this.fail(engine);
      return this.cont;
    } catch (final ClassNotFoundException e) {
      throw new JavaException(this, 1, e);
    } catch (final InstantiationException e) {
      throw new JavaException(this, 1, e);
    } catch (final IllegalAccessException e) {
      throw new JavaException(this, 1, e);
    } catch (final SecurityException e) {
      throw new JavaException(this, 1, e);
    } catch (final IllegalArgumentException e) {
      throw new JavaException(this, 1, e);
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
