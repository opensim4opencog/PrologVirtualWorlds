/* PRED_java_declared_constructor0_2 - at Sun Sep 21 05:38:57 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 12 more classes */
/* import java.lang.IllegalArgumentException; */
/* import java.lang.InstantiationException; */
/* import java.lang.StringBuilder; */
/* import java.lang.SecurityException; */
/* import java.lang.Class; */
/* import java.lang.IllegalAccessException; */
/* import java.lang.Object; */
/* import java.lang.String; */
/* import java.lang.Exception; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_java_declared_constructor0_2; */
/* import java.lang.NoSuchMethodException; */
/* import java.lang.ClassNotFoundException; */

public class PRED_java_declared_constructor0_2 extends JavaPredicate {
  /**
  * 
  */
  private static final long serialVersionUID = 7999222246289511276L;
  Object arg1;
  Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_java_declared_constructor0_2() {
    /* flow_0_0_EmptyBlock_1030169021
    */
    /* empty */}

  public PRED_java_declared_constructor0_2(Object a1, Object a2, Predicate cont) {
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
    return "java_declared_constructor0(" + this.arg1 + "," + this.arg2 + ")";
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
    a2 = deref(a2);
    if (!isVariable(a2)) throw new IllegalTypeException(this, 2, "variable", a2);
    try {
      a1 = deref(a1);
      if (isVariable(a1)) throw new PInstantiationException(this, 1);
      if (!isAtomTerm(a1) && !isCompound(a1)) throw new IllegalTypeException(this, 1, "callable", a1);
      if (isAtomTerm(a1)) {
        clazz = Class.forName(nameUQ(a1));
        c = clazz.getDeclaredConstructor(new Class[0]);
        if (c == null) throw new ExistenceException(this, 1, "constructor", a1, "");
        c.setAccessible(true);
        instance = c.newInstance(new Object[0]);
        if (!unify(a2, this.toPrologTerm(instance))) return this.fail(engine);
        return this.cont;
      }
      clazz = Class.forName(nameUQ(a1));
      final int arity = arity(a1);
      constrs = clazz.getDeclaredConstructors();
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
            c.setAccessible(true);
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
    } catch (final NoSuchMethodException e) {
      throw new JavaException(this, 1, e);
    } catch (final SecurityException e) {
      throw new JavaException(this, 1, e);
    } catch (final IllegalArgumentException e) {
      throw new JavaException(this, 1, e);
    } catch (final InvocationTargetException e) {
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
