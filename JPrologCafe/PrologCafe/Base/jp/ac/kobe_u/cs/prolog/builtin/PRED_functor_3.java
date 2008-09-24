/* PRED_functor_3 - at Sun Sep 21 05:38:55 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 3 more classes */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_functor_3; */

public class PRED_functor_3 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 565517774219593438L;
  static/*SymbolTerm*/Object SYM_DOT = makeSymbol(".");
  Object arg1;
  Object arg2;
  Object arg3;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 3;
  }

  public PRED_functor_3() {
    /* flow_0_0_EmptyBlock_1741604980
    */
    /* empty */}

  @Override
  public String toString() {
    /* flow_0_0_StringAddOperator
    */
    return "functor(" + this.arg1 + "," + this.arg2 + "," + this.arg3 + ")";
  }

  public PRED_functor_3(Object a1, Object a2, Object a3, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.arg2 = a2;
    this.arg3 = a3;
    this.cont = cont;
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    Object a3 = this.arg3;
    a1 = deref(a1);
    if (isVariable(a1)) {
      a2 = deref(a2);
      if (isVariable(a2)) throw new PInstantiationException(this, 2);
      if (!isAtomTerm(a2) && !isNumber(a2) && !isJavaObject(a2) && !isClosure(a2)) throw new IllegalTypeException(this, 2, "atomic", a2);
      a3 = deref(a3);
      if (isVariable(a3)) throw new PInstantiationException(this, 3);
      if (!isInteger(a3)) throw new IllegalTypeException(this, 3, "integer", a3);
      final int n = (int) intValue(a3);//(IntegerTerm) car).longValue();;
      if (n < 0) throw new IllegalDomainException(this, 3, "not_less_than_zero", a3);
      if (n == 0) {
        if (!unify(a1, a2)) return this.fail(engine);
        return this.cont;
      }
      if (!isAtomTerm(a2)) throw new IllegalTypeException(this, 2, "atom", a2);
      if ((n == 2) && prologEquals(a2, PRED_functor_3.SYM_DOT)) {
        final Object t = makeList(engine.makeVariable(this), engine.makeVariable(this));
        if (!unify(a1, t)) return this.fail(engine);
        return this.cont;
      }
      final Object[] args = new Object[n];
      for (int i = 0; i < n; i++)
        args[i] = engine.makeVariable(this);
      final/*SymbolTerm*/Object sym = makeSymbol(nameUQ(a2), n);
      if (!unify(a1, makeStructure(sym, args))) return this.fail(engine);
      return this.cont;
    }
    Object functor;
    Object arity;
    if (isAtomTerm(a1) || isNumber(a1) || isJavaObject(a1) || isClosure(a1)) {
      functor = a1;
      arity = makeInteger(0);
    } else if (isListTerm(a1)) {
      functor = PRED_functor_3.SYM_DOT;
      arity = makeInteger(2);
    } else if (isCompound(a1)) {
      functor = makeSymbol(nameUQ(a1));
      arity = makeInteger(arity(a1));
    } else
      return this.fail(engine);
    if (!unify(a2, functor)) return this.fail(engine);
    if (!unify(a3, arity)) return this.fail(engine);
    return this.cont;
  }

  @Override
  public void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = args[0];
    this.arg2 = args[1];
    this.arg3 = args[2];
    this.cont = cont;
  }
}
