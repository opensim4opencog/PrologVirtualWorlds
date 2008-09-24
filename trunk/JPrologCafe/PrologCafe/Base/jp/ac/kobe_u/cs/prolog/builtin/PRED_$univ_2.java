/* PRED_$univ_2 - at Sun Sep 21 05:38:58 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 3 more classes */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$univ_2; */

public class PRED_$univ_2 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = 941823099363621809L;
    static /*SymbolTerm*/Object SYM_DOT = makeAtom(".");
    static /*SymbolTerm*/Object SYM_NIL = makeAtom("[]");
    public Object arg1;
    public Object arg2;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 2;
}
    public  PRED_$univ_2() {
    /* flow_0_0_EmptyBlock_822990024
 */
/* empty */}
    public  PRED_$univ_2(Object a1, Object a2, Predicate cont) {
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
return "=..(" + this.arg1 + "," + this.arg2 + ")";
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
    if (isAtomTerm(a1) || isNumber(a1) || isJavaObject(a1) || isClosure(a1)){
 if (!unify(a2, makeList(a1, PRED_$univ_2.SYM_NIL)))
     return this.fail(engine);
    } else if (isListTerm(a1)){
 Object t = makeList(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest((Object) a1), PRED_$univ_2.SYM_NIL);
 t = makeList(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.first((Object) a1), t);
 t = makeList(PRED_$univ_2.SYM_DOT, t);
 if (!unify(a2, t))
     return this.fail(engine);
    } else if (isCompound(a1)){
 final /*SymbolTerm*/Object sym = makeAtom(nameUQ(a1));
 final Object[] args = args(a1);
 Object t = PRED_$univ_2.SYM_NIL;
 for (int i = args.length; i > 0; i--)
     t = makeList(args[i - 1], t);
 if (!unify(a2, makeList(sym, t)))
     return this.fail(engine);
    } else if (isVariable(a1)){
 a2 = deref(a2);
 if (isVariable(a2))
     throw new PInstantiationException(this, 2);
 if (prologEquals(a2, PRED_$univ_2.SYM_NIL))
     throw new IllegalDomainException(this, 2, "non_empty_list", a2);
 if (!isListTerm(a2))
     throw new IllegalTypeException(this, 2, "list", a2);
 final Object head = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.first((Object) a2));
 Object tail = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest((Object) a2));
 if (isVariable(head))
     throw new PInstantiationException(this, 2);
 if (prologEquals(tail, PRED_$univ_2.SYM_NIL)){
     if (isAtomTerm(head) || isNumber(head) || isJavaObject(head) || isClosure(head)){
  if (!unify(a1, head))
      return this.fail(engine);
  return this.cont;
     }
     throw new IllegalTypeException(this, 2, "atomic", head);
 }
 if (!isAtomTerm(head))
     throw new IllegalTypeException(this, 2, "atom", head);
 for (Object x = tail; !prologEquals(x, PRED_$univ_2.SYM_NIL); x = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest( x))){
     if (isVariable(x))
  throw new PInstantiationException(this, 2);
     if (!isListTerm(x))
  throw new IllegalTypeException(this, 2, "list", a2);
 }
 final int n = jp.ac.kobe_u.cs.prolog.lang.ListTermBase.listLength((Object) a2) - 1;
 final /*SymbolTerm*/Object sym = makeAtom(nameUQ(head), n);
 final Object[] args = new Object[n];
 for (int i = 0; i<n; i++){
     args[i] = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.first((Object) tail));
     tail = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest((Object) tail));
 }
 if (!unify(a1, makeStructure(sym, args)))
     return this.fail(engine);
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
