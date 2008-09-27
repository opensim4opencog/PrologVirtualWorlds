/* PRED_nl_0 - at Sun Sep 21 05:38:58 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 2 more classes */
/* import java.lang.String; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_nl_0; */

public class PRED_nl_0 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = -670783863646643364L;
    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 0;
}
    public  PRED_nl_0(Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.cont = cont;
}
    public  PRED_nl_0() {
    /* flow_0_0_EmptyBlock_1054706169
 */
/* empty */}
//    @Override
//    public  String toString() {
//        /* flow_0_0_ConstantOperator
// */
//return "nl";
//}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    engine.getCurrentOutput().println();
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.cont = cont;
}}
