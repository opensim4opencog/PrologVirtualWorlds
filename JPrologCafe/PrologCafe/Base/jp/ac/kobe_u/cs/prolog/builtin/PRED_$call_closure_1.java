/* PRED_$call_closure_1 - at Sun Sep 21 05:38:59 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.Predicate;
import jp.ac.kobe_u.cs.prolog.lang.PredicateBase;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;

/* used 3 more classes */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$call_closure_1; */

class PRED_$call_closure_1 extends PredicateBase {
  /**
   * 
   */
  private static final long serialVersionUID = -5975543340803486238L;
  Object arg1;
  Predicate cont;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 1;
  }

  public PRED_$call_closure_1() {
    /* flow_0_0_EmptyBlock_1069969095
    */
    /* empty */}

  public PRED_$call_closure_1(Object a1, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.cont = cont;
  }

//  @Override
//  public String toString() {
//    /* flow_0_0_StringAddOperator
//    */
//    return "$call_closure(" + this.arg1 + ")";
//  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    final Object a1 = deref(this.arg1);
    if (!isClosure(a1)) return this.fail(engine);
    final Predicate code =StaticProlog.getCode( a1);
    ((PredicateBase)code).cont = this.cont;
    return code;
  }


  @Override
  public void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = args[0];
    this.cont = cont;
  }
}
