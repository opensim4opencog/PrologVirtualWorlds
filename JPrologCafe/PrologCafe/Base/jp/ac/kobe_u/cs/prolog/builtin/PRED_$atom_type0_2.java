/* PRED_$atom_type0_2 - at Sun Sep 21 05:39:05 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

class PRED_$atom_type0_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = -2749952570335248093L;
  Object arg1;
  Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_$atom_type0_2() {
    /* flow_0_0_EmptyBlock_524700141
    */
    /* empty */}

  public PRED_$atom_type0_2(Object a1, Object a2, Predicate cont) {
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
    return "$atom_type0(" + this.arg1 + ", " + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    final Object a2 = this.arg2;
    a1 = deref(a1);
    if (!isAtomTerm(a1)) return this.fail(engine);
    final int type = Token.getStringType(nameUQ(a1));
    if (!unify(a2, makeInteger(type))) return this.fail(engine);
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
