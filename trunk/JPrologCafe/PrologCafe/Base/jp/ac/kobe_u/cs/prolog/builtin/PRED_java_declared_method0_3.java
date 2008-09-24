/* PRED_java_declared_method0_3 - at Sun Sep 21 05:39:02 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 12 more classes */
/* import java.lang.IllegalArgumentException; */
/* import java.lang.StringBuilder; */
/* import java.lang.SecurityException; */
/* import java.lang.Class; */
/* import java.lang.IllegalAccessException; */
/* import java.lang.Object; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_java_declared_method0_3; */
/* import java.lang.String; */
/* import java.lang.Exception; */
/* import java.lang.NullPointerException; */
/* import java.lang.NoSuchMethodException; */
/* import java.lang.ClassNotFoundException; */

public class PRED_java_declared_method0_3 extends JavaPredicate {
  /**
  * 
  */
  private static final long serialVersionUID = -8404131149128787225L;
  Object arg1;
  Object arg2;
  Object arg3;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 3;
  }

  public PRED_java_declared_method0_3() {
    /* flow_0_0_EmptyBlock_330616165
    */
    /* empty */}

  @Override
  public String toString() {
    /* flow_0_0_StringAddOperator
    */
    return "java_declared_method0(" + this.arg1 + "," + this.arg2 + "," + this.arg3 + ")";
  }

  public PRED_java_declared_method0_3(Object a1, Object a2, Object a3, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.arg2 = a2;
    this.arg3 = a3;
    this.cont = cont;
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    Object a3 = this.arg3;
    Class clazz = null;
    Object instance = null;
    Method[] methods = null;
    Method m = null;
    Object value = null;
    Object[] pArgs = null;
    Object[] jArgs = null;
    String methodName = null;
    a3 = deref(a3);
    if (!isVariable(a3)) throw new IllegalTypeException(this, 3, "variable", a3);
    try {
      a1 = deref(a1);
      if (isVariable(a1)) throw new PInstantiationException(this, 1);
      if (isAtomTerm(a1))
        clazz = Class.forName(nameUQ(a1));
      else if (isJavaObject(a1)) {
        instance = toJava(a1);
        clazz = jp.ac.kobe_u.cs.prolog.lang.JavaObjectTermBase.getClazz(a1);
      } else
        throw new IllegalTypeException(this, 1, "atom_or_java", a1);
      a2 = deref(a2);
      if (isVariable(a2)) throw new PInstantiationException(this, 2);
      do {
        if (isAtomTerm(a2)) {
          m = clazz.getDeclaredMethod(nameUQ(a2), new Class[0]);
          m.setAccessible(true);
          value = m.invoke(instance, new Object[0]);
        } else {
          if (isCompound(a2)) {
            methodName = nameUQ(a2);
            final int arity = arity(a2);
            methods = clazz.getDeclaredMethods();
            if (methods.length == 0) throw new ExistenceException(this, 2, "method", a2, "");
            pArgs = args(a2);
            jArgs = new Object[arity];
            for (int i = 0; i < arity; i++) {
              pArgs[i] = deref(pArgs[i]);
              if (!isJavaObject(pArgs[i])) pArgs[i] = makeJavaObject(pArgs[i]);
              jArgs[i] = toJava(pArgs[i]);
            }
            for (int i = 0; i < methods.length; i++) {
              if (methods[i].getName().equals(methodName) && this.checkParameterTypes(methods[i].getParameterTypes(), pArgs)) {
                try {
                  m = methods[i];
                  m.setAccessible(true);
                  value = m.invoke(instance, jArgs);
                  break;
                } catch (final Exception e) {
                  m = null;
                }
              }
            }
            if (m != null) break;
            throw new ExistenceException(this, 2, "method", a2, "");
          }
          throw new IllegalTypeException(this, 2, "callable", a2);
        }
      } while (false);
      if (value == null) return this.cont;
      if (!unify(a3, this.toPrologTerm(value))) return this.fail(engine);
      return this.cont;
    } catch (final ClassNotFoundException e) {
      throw new JavaException(this, 1, e);
    } catch (final NoSuchMethodException e) {
      throw new JavaException(this, 2, e);
    } catch (final SecurityException e) {
      throw new JavaException(this, 2, e);
    } catch (final IllegalAccessException e) {
      throw new JavaException(this, 2, e);
    } catch (final IllegalArgumentException e) {
      throw new JavaException(this, 2, e);
    } catch (final InvocationTargetException e) {
      throw new JavaException(this, 2, e);
    } catch (final NullPointerException e) {
      throw new JavaException(this, 2, e);
    }
  }

  @Override
  public void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = args[0];
    this.arg2 = args[1];
    this.arg3 = args[2];
    this.cont = cont;
  }
}
