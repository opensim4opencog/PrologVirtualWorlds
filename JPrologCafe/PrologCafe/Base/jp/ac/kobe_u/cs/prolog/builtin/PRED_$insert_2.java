/* PRED_$insert_2 - at Sun Sep 21 05:38:59 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 3 more classes */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$insert_2; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */

class PRED_$insert_2 extends PredicateBase {
  /**
   * 
   */
  private static final long serialVersionUID = -7679833586875205993L;
  public Object arg1;
  public Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_$insert_2() {
    /* flow_0_0_EmptyBlock_1861447257
    */
    /* empty */}

  public PRED_$insert_2(Object a1, Object a2, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
  }

//  @Override
//  public String toString() {
//    /* flow_0_0_StringAddOperator
//    */
//    return "$insert(" + this.arg1 + "," + this.arg2 + ")";
//  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    a2 = deref(a2);
    if (!isVariable(a2)) throw new IllegalTypeException(this, 2, "variable", a2);
    a1 = deref(a1);
    final int idx = engine.internalDB.insert(a1);
    if (!unify(a2, makeInteger(idx))) return this.fail(engine);
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
