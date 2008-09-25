/* PRED_sort_2 - at Sun Sep 21 05:38:57 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import java.util.Arrays;

import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 4 more classes */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_sort_2; */
/* import java.lang.ClassCastException; */

public class PRED_sort_2 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = -1598610344045447227L;
    static /*SymbolTerm*/Object Nil = makeAtom("[]");
    Object arg1;
    Object arg2;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 2;
}
    public  PRED_sort_2() {
    /* flow_0_0_EmptyBlock_1105339954
 */
/* empty */}
    public  PRED_sort_2(Object a1, Object a2, Predicate cont) {
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
return "sort(" + this.arg1 + "," + this.arg2 + ")";
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
    if (isVariable(a1))
 throw new PInstantiationException(this, 1);
    if (prologEquals(a1, PRED_sort_2.Nil)){
 if (!unify(a2, PRED_sort_2.Nil))
     return this.fail(engine);
 return this.cont;
    }
    if (!isListTerm(a1))
 throw new IllegalTypeException(this, 1, "list", a1);
    final int len = listLength((Object) a1);
    final Object[] list = new Object[len];
    Object tmp = a1;
    for (int i = 0; i<len; i++){
 if (!isListTerm(tmp))
     throw new IllegalTypeException(this, 1, "list", a1);
 list[i] = deref(first((Object) tmp));
 tmp = deref(rest((Object) tmp));
    }
    if (!prologEquals(tmp, PRED_sort_2.Nil))
 throw new PInstantiationException(this, 1);
    try {
 Arrays.sort(list);
    } catch (final ClassCastException e){
 throw new JavaException(this, 1, e);
    }
    tmp = PRED_sort_2.Nil;
    Object tmp2 = null;
    for (int i = list.length - 1; i >= 0; i--){
 if (!prologEquals(list[i], tmp2))
     tmp = makeList(list[i], tmp);
 tmp2 = list[i];
    }
    if (!unify(a2, tmp))
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
