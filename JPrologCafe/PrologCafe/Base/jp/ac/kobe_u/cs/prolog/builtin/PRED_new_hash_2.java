/* PRED_new_hash_2 - at Sun Sep 21 05:39:04 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 3 more classes */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_new_hash_2; */

public class PRED_new_hash_2 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = 6985047342983544235L;
    public static /*SymbolTerm*/Object SYM_NIL = makeAtom("[]");
    public static /*SymbolTerm*/Object SYM_ALIAS_1 = makeAtom("alias", 1);
    public Object arg1;
    public Object arg2;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 2;
}
    public  PRED_new_hash_2() {
    /* flow_0_0_EmptyBlock_2014772420
 */
/* empty */}
    public  PRED_new_hash_2(Object a1, Object a2, Predicate cont) {
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
return "new_hash(" + this.arg1 + "," + this.arg2 + ")";
}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    a1 = deref(a1);
    if (!isVariable(a1))
 throw new IllegalTypeException(this, 1, "variable", a1);
    final Object newHash = makeJavaObject(new HashtableOfTerm());
    a2 = deref(a2);
    if (isNil(a2)){
 if (!unify(a1, newHash))
     return this.fail(engine);
 return this.cont;
    }
    if (!isListTerm(a2))
 throw new IllegalTypeException(this, 2, "list", a2);
    for (Object tmp = a2; !isNil(tmp); tmp = deref(rest((Object) tmp))){
 if (isVariable(tmp))
     throw new PInstantiationException(this, 2);
 if (!isListTerm(tmp))
     throw new IllegalTypeException(this, 2, "list", a2);
 final Object car = deref(first((Object) tmp));
 if (isVariable(car))
     throw new PInstantiationException(this, 2);
 if (isCompound(car)){
     final /*SymbolTerm*/Object functor = functor((Object) car);
     final Object[] args = args(car);
     if (prologEquals(functor, PRED_new_hash_2.SYM_ALIAS_1)){
  final Object alias = deref(args[0]);
  if (!isAtomTerm(alias))
      throw new IllegalDomainException(this, 2, "hash_option", car);
  if (engine.getHashManager().containsKey(alias))
      throw new PermissionException(this, "new", "hash", car, "");
  engine.getHashManager().put(alias, newHash);
  continue ;
     }
     throw new IllegalDomainException(this, 2, "hash_option", car);
 }
 throw new IllegalDomainException(this, 2, "hash_option", car);
    }
    if (!unify(a1, newHash))
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
