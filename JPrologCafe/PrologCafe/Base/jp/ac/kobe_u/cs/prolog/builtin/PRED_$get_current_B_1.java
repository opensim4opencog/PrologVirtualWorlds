/* PRED_$get_current_B_1 - at Sun Sep 21 05:39:02 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 3 more classes */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$get_current_B_1; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */

class PRED_$get_current_B_1 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 3327873704575629722L;
  Object arg1;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 1;
  }

  public PRED_$get_current_B_1() {
    /* flow_0_0_EmptyBlock_1191679188
    */
    /* empty */}

  public PRED_$get_current_B_1(Object a1, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.cont = cont;
  }

  //    @Override
  //    public  String toString() {
  //        /* flow_0_0_StringAddOperator
  // */
  //return "$get_current_B(" + this.arg1 + ")";
  //}
  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    final Object a1 = this.arg1;
    if (!unify(a1, makeInteger(engine.stack_top()))) return this.fail(engine);
    return this.cont;
  }

  @Override
  public void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = args[0];
    this.cont = cont;
  }
}
