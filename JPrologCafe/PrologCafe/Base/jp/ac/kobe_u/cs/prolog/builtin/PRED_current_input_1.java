/* PRED_current_input_1 - at Sun Sep 21 05:38:57 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

public class PRED_current_input_1 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 8158336387798844305L;
  Object arg1;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 1;
  }

  public PRED_current_input_1() {
    /* flow_0_0_EmptyBlock_531284093
    */
    /* empty */}

  public PRED_current_input_1(Object a1, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.cont = cont;
  }

  //  @Override
  //  public String toString() {
  //    /* flow_0_0_StringAddOperator
  //    */
  //    return "current_input(" + this.arg1 + ")";
  //  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    a1 = deref(a1);
    if (isVariable(a1))
      bind(a1, makeJavaObject(engine.getCurrentInput()));
    else if (isJavaObject(a1)) {
      if (!unify(a1, makeJavaObject(engine.getCurrentInput()))) return this.fail(engine);
    } else
      throw new IllegalDomainException(this, 1, "stream", a1);
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
