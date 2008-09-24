/* PRED_current_output_1 - at Sun Sep 21 05:38:59 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;

public class PRED_current_output_1 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = 9001740246732161364L;
    Object arg1;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 1;
}
    public  PRED_current_output_1() {
    /* flow_0_0_EmptyBlock_1461723831
 */
/* empty */}
    public  PRED_current_output_1(Object a1, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.cont = cont;
}
    @Override
    public  String toString() {
        /* flow_0_0_StringAddOperator
 */
return "current_output(" + this.arg1 + ")";
}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    a1 = deref(a1);
    if (isVariable(a1))
 jp.ac.kobe_u.cs.prolog.lang.VariableTermBase.bind((VariableTerm) a1, makeJavaObject(engine.getCurrentOutput()));
    else if (isJavaObject(a1)){
 if (!unify(a1, makeJavaObject(engine.getCurrentOutput())))
     return this.fail(engine);
    } else
 throw new IllegalDomainException(this, 1, "stream", a1);
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.cont = cont;
}}
