/* PRED_$set_prolog_impl_flag_2 - at Sun Sep 21 05:39:04 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 3 more classes */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$set_prolog_impl_flag_2; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */

class PRED_$set_prolog_impl_flag_2 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = 1044821584364287346L;
    static /*SymbolTerm*/Object CHAR_CONVERSION = makeAtom("char_conversion");
    static /*SymbolTerm*/Object DEBUG = makeAtom("debug");
    static /*SymbolTerm*/Object UNKNOWN = makeAtom("unknown");
    static /*SymbolTerm*/Object DOUBLE_QUOTES = makeAtom("double_quotes");
    static /*SymbolTerm*/Object PRINT_STACK_TRACE = makeAtom("print_stack_trace");
    public Object arg1;
    public Object arg2;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 2;
}
    public  PRED_$set_prolog_impl_flag_2() {
    /* flow_0_0_EmptyBlock_1583668601
 */
/* empty */}
    public  PRED_$set_prolog_impl_flag_2(Object a1, Object a2, Predicate cont) {
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
//return "$set_prolog_impl_flag(" + this.arg1 + "," + this.arg2 + ")";
//}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    a1 = deref(a1);
    a2 = deref(a2);
    if (prologEquals(a1, PRED_$set_prolog_impl_flag_2.CHAR_CONVERSION)){
 if (!isAtomTerm(a2))
     return this.fail(engine);
 engine.setCharConversion(nameUQ(a2));
    } else if (prologEquals(a1, PRED_$set_prolog_impl_flag_2.DEBUG)){
 if (!isAtomTerm(a2))
     return this.fail(engine);
 engine.setDebug(nameUQ(a2));
    } else if (prologEquals(a1, PRED_$set_prolog_impl_flag_2.UNKNOWN)){
 if (!isAtomTerm(a2))
     return this.fail(engine);
 engine.setUnknown(nameUQ(a2));
    } else if (prologEquals(a1, PRED_$set_prolog_impl_flag_2.DOUBLE_QUOTES)){
 if (!isAtomTerm(a2))
     return this.fail(engine);
 engine.setDoubleQuotes(nameUQ(a2));
    } else if (prologEquals(a1, PRED_$set_prolog_impl_flag_2.PRINT_STACK_TRACE)){
 if (!isAtomTerm(a2))
     return this.fail(engine);
 engine.setPrintStackTrace(nameUQ(a2));
    } else
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
