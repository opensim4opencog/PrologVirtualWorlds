/* PRED_$statistics_2 - at Sun Sep 21 05:38:54 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 4 more classes */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */
/* import java.lang.System; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$statistics_2; */

class PRED_$statistics_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 9091734488065009859L;
  public static /*SymbolTerm*/Object Nil = makeSymbol("[]");
  public static /*SymbolTerm*/Object SYM_RUNTIME = makeSymbol("runtime");
  public static /*SymbolTerm*/Object SYM_TRAIL = makeSymbol("trail");
  public static /*SymbolTerm*/Object SYM_CHOICE = makeSymbol("choice");
  Object arg1;
  Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_$statistics_2() {
    /* flow_0_0_EmptyBlock_788458698
    */
    /* empty */}

  public PRED_$statistics_2(Object a1, Object a2, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
  }

  @Override
  public String toString() {
    /* flow_0_0_StringAddOperator
    */
    return "$statistics(" + this.arg1 + "," + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    final Object a2 = this.arg2;
    Object result = null;
    a1 = deref(a1);
    if (isVariable(a1)) throw new PInstantiationException(this, 1);
    if (!isAtomTerm(a1)) throw new IllegalTypeException(this, 1, "atom", a1);
    if (prologEquals(a1, PRED_$statistics_2.SYM_RUNTIME)) {
      final long val1 = System.currentTimeMillis() - engine.getStartRuntime();
      final long val2 = val1 - engine.getPreviousRuntime();
      engine.setPreviousRuntime(val1);
      final Object start = makeInteger((int) val1);
      final Object previous = makeInteger((int) val2);
      result = makeList(start, makeList(previous, PRED_$statistics_2.Nil));
    } else if (prologEquals(a1, PRED_$statistics_2.SYM_TRAIL)) {
      final int top = engine.trail.top();
      final int max = engine.trail.max();
      final Object used = makeInteger(top);
      final Object free = makeInteger(max - top);
      result = makeList(used, makeList(free, PRED_$statistics_2.Nil));
    } else if (prologEquals(a1, PRED_$statistics_2.SYM_CHOICE)) {
      final int top = engine.stack_top();
      final int max = engine.stack_max();
      final Object used = makeInteger(top);
      final Object free = makeInteger(max - top);
      result = makeList(used, makeList(free, PRED_$statistics_2.Nil));
    } else
      return this.fail(engine);
    if (!unify(a2, result)) return this.fail(engine);
    return this.cont;
  }

  @Override
  public void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = args[0];
    this.arg2 = args[1];
    this.cont = cont;
  }
}
