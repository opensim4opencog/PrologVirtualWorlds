/* PRED_set_input_1 - at Sun Sep 21 05:38:50 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import java.io.PushbackReader;

import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 4 more classes */
/* import java.lang.Object; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_set_input_1; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */

public class PRED_set_input_1 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = -6939266735884012881L;
    public Object arg1;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 1;
}
    public  PRED_set_input_1() {
    /* flow_0_0_EmptyBlock_606971302
 */
/* empty */}
    public  PRED_set_input_1(Object a1, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.cont = cont;
}
//    @Override
//    public  String toString() {
//        /* flow_0_0_StringAddOperator
// */
//return "set_input(" + this.arg1 + ")";
//}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object stream = null;
    a1 = deref(a1);
    if (isVariable(a1))
 throw new PInstantiationException(this, 1);
    if (isAtomTerm(a1)){
 if (!engine.getStreamManager().containsKey(a1))
     throw new ExistenceException(this, 1, "stream", a1, "");
 stream = toJava(engine.getStreamManager().get(a1));
    } else if (isJavaObject(a1))
 stream = toJava(a1);
    else
 throw new IllegalDomainException(this, 1, "stream_or_alias", a1);
    if (!(stream instanceof PushbackReader))
 throw new PermissionException(this, "input", "stream", a1, "");
    engine.setCurrentInput((PushbackReader) stream);
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.cont = cont;
}}
