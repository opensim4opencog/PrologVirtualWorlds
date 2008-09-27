/* PRED_current_engine_1 - at Sun Sep 21 05:39:05 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 3 more classes */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_current_engine_1; */

public class PRED_current_engine_1 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = -3755764858213935357L;
  Object arg1;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 1;
  }

  public PRED_current_engine_1() {
    /* flow_0_0_EmptyBlock_2084371115
    */
    /* empty */}

  public PRED_current_engine_1(Object a1, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.cont = cont;
  }

//  @Override
//  public String toString() {
//    /* flow_0_0_StringAddOperator
//    */
//    return "current_engine(" + this.arg1 + ")";
//  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    a1 = deref(a1);
    if (!unify(a1, makeJavaObject(engine))) return this.fail(engine);
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
