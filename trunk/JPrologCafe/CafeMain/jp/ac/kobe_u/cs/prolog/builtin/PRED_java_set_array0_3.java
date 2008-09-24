/* PRED_java_set_array0_3 - at Sun Sep 21 05:39:00 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.lang.reflect.Array;
import java.lang.reflect.Field;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 11 more classes */
/* import java.lang.IllegalArgumentException; */
/* import java.lang.Object; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_java_set_array0_3; */
/* import java.lang.SecurityException; */
/* import java.lang.NoSuchFieldException; */
/* import java.lang.NullPointerException; */
/* import java.lang.Class; */
/* import java.lang.IllegalAccessException; */
/* import java.lang.ClassNotFoundException; */

public class PRED_java_set_array0_3 extends JavaPredicate {
  /**
  * 
  */
  private static final long serialVersionUID = 1822166623213042486L;
  Object arg1;
  Object arg2;
  Object arg3;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 3;
  }

  public PRED_java_set_array0_3() {
    /* flow_0_0_EmptyBlock_1315792650
    */
    /* empty */}

  @Override
  public String toString() {
    /* flow_0_0_StringAddOperator
    */
    return "java_set_array0(" + this.arg1 + "," + this.arg2 + "," + this.arg3 + ")";
  }

  public PRED_java_set_array0_3(Object a1, Object a2, Object a3, Predicate cont) {
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
    Object instance = null;
    int array = -1;
    Object value = null;
    try {
      a1 = deref(a1);
      if (isVariable(a1)) throw new PInstantiationException(this, 1);
      instance = toJava(a1);
      a2 = deref(a2);
      if (isVariable(a2)) throw new PInstantiationException(this, 2);
      if (!isInteger(a2)) throw new IllegalTypeException(this, 2, "integer", a2);
      array = (int) intValue(a2);
      a3 = deref(a3);
      if (isJavaObject(a3))
        value = toJava(a3);
      else
        value = a3;
      Array.set(instance, array, value);
      return this.cont;
    } catch (final ArrayIndexOutOfBoundsException e) {
      throw new JavaException(this, 2, e);
    } catch (final NullPointerException e) {
      throw new JavaException(this, 2, e);
    } catch (final IllegalArgumentException e) {
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
