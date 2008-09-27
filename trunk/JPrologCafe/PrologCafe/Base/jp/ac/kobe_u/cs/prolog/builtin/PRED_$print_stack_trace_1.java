/* PRED_$print_stack_trace_1 - at Sun Sep 21 05:38:53 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 7 more classes */
/* import java.lang.Object; */
/* import java.lang.InterruptedException; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import java.lang.System; */
/* import java.lang.Exception; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$print_stack_trace_1; */

class PRED_$print_stack_trace_1 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = 3341357600311586376L;
    public Object arg1;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 1;
}
    public  PRED_$print_stack_trace_1() {
    /* flow_0_0_EmptyBlock_1703379623
 */
/* empty */}
    public  PRED_$print_stack_trace_1(Object a1, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.cont = cont;
}
//    @Override
//    public  String toString() {
//        /* flow_0_0_StringAddOperator
// */
//return "$print_stack_trace(" + this.arg1 + ")";
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
    if (!isJavaObject(a1))
 throw new IllegalTypeException(this, 1, "java", a1);
    final Object obj = toJava(a1);
    if (obj instanceof InterruptedException)
 System.exit(1);
    if (engine.getPrintStackTrace().equals("on"))
 ((Exception) obj).printStackTrace();
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.cont = cont;
}}
