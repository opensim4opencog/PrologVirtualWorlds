/* PRED_$end_sync_1 - at Sun Sep 21 05:39:03 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 5 more classes */
/* import java.lang.Object; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$begin_sync_2; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$end_sync_1; */

class PRED_$end_sync_1 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = -6679237091735530872L;
    Object arg1;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 1;
}
    public  PRED_$end_sync_1() {
    /* flow_0_0_EmptyBlock_1424873950
 */
/* empty */}
    public  PRED_$end_sync_1(Object a1, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.cont = cont;
}
    @Override
    public  String toString() {
        /* flow_0_0_StringAddOperator
 */
return "$end_sync(" + this.arg1 + ")";
}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    a1 = deref(a1);
    if (!isJavaObject(a1))
 throw new IllegalTypeException(this, 1, "java", a1);
    final Object obj = toJava(a1);
    if (!(obj instanceof PRED_$begin_sync_2))
 throw new SystemException("a1 must be an object of PRED_$begin_sync_2: " + this);
    final PRED_$begin_sync_2 p = (PRED_$begin_sync_2) obj;
    p.outOfScope = true;
    engine.trail_push(new OutOfScope(p));
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.cont = cont;
}}
