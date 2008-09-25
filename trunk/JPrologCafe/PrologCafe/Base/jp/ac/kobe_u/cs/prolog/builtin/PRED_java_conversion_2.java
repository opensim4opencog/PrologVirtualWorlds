/* PRED_java_conversion_2 - at Sun Sep 21 05:39:03 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.util.Vector;

import jp.ac.kobe_u.cs.prolog.lang.*;

public class PRED_java_conversion_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 3176460443234326340L;
  Object arg1;
  Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_java_conversion_2() {
    /* flow_0_0_EmptyBlock_615949665
    */
    /* empty */}

  public PRED_java_conversion_2(Object a1, Object a2, Predicate cont) {
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
    return "java_conversion(" + this.arg1 + "," + this.arg2 + ")";
  }

  protected Object inverseConversion(Object o) {
    /* flow_0_0_IfThenElseBlock_1394841115
    */
    if (o == null) throw new EvaluationException(this, 2, "undefined");
    if ((o instanceof Byte) || (o instanceof Short) || (o instanceof Integer) || (o instanceof Long)) return makeInteger(((Number) o).intValue());
    if ((o instanceof Float) || (o instanceof Double)) return makeDouble(((Number) o).doubleValue());
    if (o instanceof String) return makeAtom((String) o);
    if (o instanceof Vector) {
      final Vector v = (Vector) o;
      Object t = Prolog.Nil;
      for (int i = v.size(); i > 0; i--)
        t = makeList(this.inverseConversion(v.elementAt(i - 1)), t);
      return t;
    }
    return makeJavaObject(o);
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    a1 = deref(a1);
    a2 = deref(a2);
    if (isVariable(a1)) {
      if (isJavaObject(a2))
        bind(a1, this.inverseConversion(toJava(a2)));
      else
        bind(a1, a2);
    } else {
      if (!isVariable(a2)) throw new IllegalTypeException(this, 2, "variable", a2);
      if (isJavaObject(a1) || isCompound(a1) || isClosure(a1))
        bind(a2, a1);
      else
        bind(a2, makeJavaObject(toJava(a1)));
    }
    return this.cont;
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
