/* PRED_$fast_writeq_2 - at Sun Sep 21 05:38:56 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import java.io.PrintWriter;

import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 4 more classes */
/* import java.lang.Object; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$fast_writeq_2; */

public class PRED_$fast_writeq_2 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = 588708786483571569L;
    Object arg1;
    Object arg2;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 2;
}
    public  PRED_$fast_writeq_2() {
    /* flow_0_0_EmptyBlock_1449799553
 */
/* empty */}
    public  PRED_$fast_writeq_2(Object a1, Object a2, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
}

  //    @Override
  //    public  String toString() {
  //        /* flow_0_0_StringAddOperator
  // */
  //return "$fast_writeq(" + this.arg1 + ", " + this.arg2 + ")";
  //}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    final Object a2 = this.arg2;
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
    if (!(stream instanceof PrintWriter))
 throw new PermissionException(this, "output", "stream", a1, "");
    ((PrintWriter) stream).print(toQuotedString(deref(a2)));
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.arg2 = args[1];
    this.cont = cont;
}}
