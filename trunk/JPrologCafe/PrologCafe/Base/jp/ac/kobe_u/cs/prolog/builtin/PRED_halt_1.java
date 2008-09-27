/* PRED_halt_1 - at Sun Sep 21 05:38:52 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 3 more classes */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_halt_1; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */

public class PRED_halt_1 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = -7084809178150270498L;
    public Object arg1;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 1;
}
    public  PRED_halt_1() {
    /* flow_0_0_EmptyBlock_2064366914
 */
/* empty */}
    public  PRED_halt_1(Object a1, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.cont = cont;
}
//    @Override
//    public  String toString() {
//        /* flow_0_0_StringAddOperator
// */
//return "halt(" + this.arg1 + ")";
//}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    a1 = deref(a1);
    if (isVariable(a1))
 throw new PInstantiationException(this, 1);
    if (!isInteger(a1))
 throw new IllegalTypeException(this, 1, "integer", a1);
    engine.exceptionRaised =  (int) intValue(a1);//(IntegerTerm) car).longValue();;
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.cont = cont;
}}
