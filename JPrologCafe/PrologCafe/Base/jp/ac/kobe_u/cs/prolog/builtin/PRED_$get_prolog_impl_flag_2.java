/* PRED_$get_prolog_impl_flag_2 - at Sun Sep 21 05:38:54 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 3 more classes */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$get_prolog_impl_flag_2; */

class PRED_$get_prolog_impl_flag_2 extends PredicateBase {
  /**
   * 
   */
  private static final long serialVersionUID = 6253715433630034714L;
  static /*SymbolTerm*/Object TRUE = makeAtom("true");
  static /*SymbolTerm*/Object FALSE = makeAtom("false");
  static /*SymbolTerm*/Object BOUNDED = makeAtom("bounded");
  static /*SymbolTerm*/Object MAX_INTEGER = makeAtom("max_integer");
  static /*SymbolTerm*/Object MIN_INTEGER = makeAtom("min_integer");
  static /*SymbolTerm*/Object INTEGER_ROUNDING_FUNCTION = makeAtom("integer_rounding_function");
  static /*SymbolTerm*/Object CHAR_CONVERSION = makeAtom("char_conversion");
  static /*SymbolTerm*/Object DEBUG = makeAtom("debug");
  static /*SymbolTerm*/Object MAX_ARITY = makeAtom("max_arity");
  static /*SymbolTerm*/Object UNKNOWN = makeAtom("unknown");
  static /*SymbolTerm*/Object DOUBLE_QUOTES = makeAtom("double_quotes");
  static /*SymbolTerm*/Object PRINT_STACK_TRACE = makeAtom("print_stack_trace");
  public Object arg1;
  public Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_$get_prolog_impl_flag_2() {
    /* flow_0_0_EmptyBlock_744614985
    */
    /* empty */}

  public PRED_$get_prolog_impl_flag_2(Object a1, Object a2, Predicate cont) {
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
    return "$get_prolog_impl_flag(" + this.arg1 + "," + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    a1 = deref(a1);
    a2 = deref(a2);
    if (prologEquals(a1, PRED_$get_prolog_impl_flag_2.BOUNDED)) {
      if (engine.isBounded()) {
        if (!unify(a2, PRED_$get_prolog_impl_flag_2.TRUE)) return this.fail(engine);
      } else if (!unify(a2, PRED_$get_prolog_impl_flag_2.FALSE)) return this.fail(engine);
    } else if (prologEquals(a1, PRED_$get_prolog_impl_flag_2.MAX_INTEGER)) {
      if (!unify(a2, makeInteger(engine.getMaxInteger()))) return this.fail(engine);
    } else if (prologEquals(a1, PRED_$get_prolog_impl_flag_2.MIN_INTEGER)) {
      if (!unify(a2, makeInteger(engine.getMinInteger()))) return this.fail(engine);
    } else if (prologEquals(a1, PRED_$get_prolog_impl_flag_2.INTEGER_ROUNDING_FUNCTION)) {
      if (!unify(a2, makeAtom(engine.getIntegerRoundingFunction()))) return this.fail(engine);
    } else if (prologEquals(a1, PRED_$get_prolog_impl_flag_2.CHAR_CONVERSION)) {
      if (!unify(a2, makeAtom(engine.getCharConversion()))) return this.fail(engine);
    } else if (prologEquals(a1, PRED_$get_prolog_impl_flag_2.DEBUG)) {
      if (!unify(a2, makeAtom(engine.getDebug()))) return this.fail(engine);
    } else if (prologEquals(a1, PRED_$get_prolog_impl_flag_2.MAX_ARITY)) {
      if (!unify(a2, makeInteger(engine.getMaxArity()))) return this.fail(engine);
    } else if (prologEquals(a1, PRED_$get_prolog_impl_flag_2.UNKNOWN)) {
      if (!unify(a2, makeAtom(engine.getUnknown()))) return this.fail(engine);
    } else if (prologEquals(a1, PRED_$get_prolog_impl_flag_2.DOUBLE_QUOTES)) {
      if (!unify(a2, makeAtom(engine.getDoubleQuotes()))) return this.fail(engine);
    } else if (prologEquals(a1, PRED_$get_prolog_impl_flag_2.PRINT_STACK_TRACE)) {
      if (!unify(a2, makeAtom(engine.getPrintStackTrace()))) return this.fail(engine);
    } else
      return this.fail(engine);
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
