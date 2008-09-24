/* PRED_$erase_1 - at Sun Sep 21 05:37:46 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 3 more classes */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$erase_1; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */

class PRED_$erase_1 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = 8457207027203112610L;
    Object arg1;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 1;
}
    public  PRED_$erase_1() {
    /* flow_0_0_EmptyBlock_1638726193
 */
/* empty */}
    public  PRED_$erase_1(Object a1, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.cont = cont;
}
    @Override
    public  String toString() {
        /* flow_0_0_StringAddOperator
 */
return "$erase(" + this.arg1 + ")";
}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    a1 = deref(a1);
    if (!isInteger(a1))
 throw new IllegalTypeException(this, 1, "integer", a1);
    final int idx = (int)intValue(a1);
    engine.internalDB.erase(idx);
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.cont = cont;
}}
