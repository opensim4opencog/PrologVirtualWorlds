/* PRED_hash_keys_2 - at Sun Sep 21 05:39:03 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import java.util.Enumeration;

import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 4 more classes */
/* import java.lang.Object; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_hash_keys_2; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */

public class PRED_hash_keys_2 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = -7871656436022781084L;
    public static /*SymbolTerm*/Object SYM_NIL = makeSymbol("[]");
    public Object arg1;
    public Object arg2;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 2;
}
    public  PRED_hash_keys_2() {
    /* flow_0_0_EmptyBlock_1550590728
 */
/* empty */}
    public  PRED_hash_keys_2(Object a1, Object a2, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
}
    @Override
    public  String toString() {
        /* flow_0_0_StringAddOperator
 */
return "hash_keys(" + this.arg1 + "," + this.arg2 + ")";
}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    final Object a2 = this.arg2;
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
    Object keys = PRED_hash_keys_2.SYM_NIL;
    final Enumeration e = ((HashtableOfTerm) hash).keys();
    while (e.hasMoreElements())
 keys = makeList((Object) e.nextElement(), keys);
    if (!unify(a2, keys))
 return this.fail(engine);
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
