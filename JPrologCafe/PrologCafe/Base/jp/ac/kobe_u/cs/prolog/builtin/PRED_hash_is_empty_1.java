/* PRED_hash_is_empty_1 - at Sun Sep 21 05:39:01 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 4 more classes */
/* import java.lang.Object; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_hash_is_empty_1; */

public class PRED_hash_is_empty_1 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = 7564258346199101270L;
    public Object arg1;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 1;
}
    public  PRED_hash_is_empty_1() {
    /* flow_0_0_EmptyBlock_642597087
 */
/* empty */}
    public  PRED_hash_is_empty_1(Object a1, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.cont = cont;
}
//    @Override
//    public  String toString() {
//        /* flow_0_0_StringAddOperator
// */
//return "hash_is_empty(" + this.arg1 + ")";
//}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object hash = null;
    a1 = deref(a1);
    if (isVariable(a1))
 throw new PInstantiationException(this, 1);
    if (isAtomTerm(a1)){
 if (!engine.getHashManager().containsKey(a1))
     throw new ExistenceException(this, 1, "hash", a1, "");
 hash = toJava(engine.getHashManager().get(a1));
    } else if (isJavaObject(a1))
 hash = toJava(a1);
    else
 throw new IllegalDomainException(this, 1, "hash_or_alias", a1);
    if (!(hash instanceof HashtableOfTerm))
 throw new InternalException(this + ": Hash is not HashtableOfTerm");
    if (!((HashtableOfTerm) hash).isEmpty())
 return this.fail(engine);
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.cont = cont;
}}
