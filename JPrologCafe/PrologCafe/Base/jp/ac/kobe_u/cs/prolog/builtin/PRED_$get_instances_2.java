/* PRED_$get_instances_2 - at Sun Sep 21 05:39:00 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 3 more classes */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$get_instances_2; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */

class PRED_$get_instances_2 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = -573729492312632850L;
    public Object arg1;
    public Object arg2;
    public static /*SymbolTerm*/Object COMMA = makeAtom(",", 2);

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 2;
}
    public  PRED_$get_instances_2() {
    /* flow_0_0_EmptyBlock_1322735643
 */
/* empty */}
    public  PRED_$get_instances_2(Object a1, Object a2, Predicate cont) {
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
return "$get_instances(" + this.arg1 + "," + this.arg2 + ")";
}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    final Object a2 = this.arg2;
    a1 = deref(a1);
    if (isNil(a1))
 return this.fail(engine);
    if (!isListTerm(a1))
 throw new IllegalTypeException(this, 1, "list", a1);
    Object x = Prolog.Nil;
    for (Object tmp = a1; !isNil(tmp); tmp = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest((Object) tmp))){
 if (!isListTerm(tmp))
     throw new IllegalTypeException(this, 1, "list", a1);
 final Object car = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.first((Object) tmp));
 if (isVariable(car))
     throw new PInstantiationException(this, 1);
 if (!isInteger(car))
     throw new RepresentationException(this, 1, "integer");
 final int i = (int) intValue(car);//(IntegerTerm) car).longValue();
 final Object e = engine.internalDB.get(i);
 if (e != null){
     final Object[] arg = { e, car};
     x = makeList(makeStructure(PRED_$get_instances_2.COMMA, arg), x);
 }
    }
    if (!unify(a2, x))
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
