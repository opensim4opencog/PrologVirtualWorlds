/* PRED_$fast_write_1 - at Sun Sep 21 05:38:52 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 3 more classes */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$fast_write_1; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */

public class PRED_$fast_write_1 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = -96789428257710044L;
    Object arg1;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 1;
}
    public  PRED_$fast_write_1() {
    /* flow_0_0_EmptyBlock_1542631196
 */
/* empty */}
    public  PRED_$fast_write_1(Object a1, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.cont = cont;
}
    @Override
    public  String toString() {
        /* flow_0_0_StringAddOperator
 */
return "$fast_write(" + this.arg1 + ")";
}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    final Object a1 = deref(this.arg1);
    engine.getCurrentOutput().print(toString(a1));
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.cont = cont;
}}
