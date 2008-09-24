/* PRED_keysort_2 - at Sun Sep 21 05:38:58 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.util.Arrays;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 5 more classes */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import jp.ac.kobe_u.cs.prolog.builtin.KeySortComparator; */
/* import java.lang.ClassCastException; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_keysort_2; */

public class PRED_keysort_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 1937513875144863756L;
  static /*SymbolTerm*/Object SYM_HYPHEN_2 = makeAtom("-", 2);
  static /*SymbolTerm*/Object Nil = makeAtom("[]");
  Object arg1;
  Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_keysort_2() {
    /* flow_0_0_EmptyBlock_1007300861
    */
    /* empty */}

  public PRED_keysort_2(Object a1, Object a2, Predicate cont) {
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
    return "keysort(" + this.arg1 + "," + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    final Object a2 = this.arg2;
    a1 = deref(a1);
    if (isVariable(a1)) throw new PInstantiationException(this, 1);
    if (prologEquals(a1, PRED_keysort_2.Nil)) {
      if (!unify(a2, PRED_keysort_2.Nil)) return this.fail(engine);
      return this.cont;
    }
    if (!isListTerm(a1)) throw new IllegalTypeException(this, 1, "list", a1);
    final int len = jp.ac.kobe_u.cs.prolog.lang.ListTermBase.listLength((Object) a1);
    final Object[] list = new Object[len];
    Object tmp = a1;
    for (int i = 0; i < len; i++) {
      if (!isListTerm(tmp)) throw new IllegalTypeException(this, 1, "list", a1);
      list[i] = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.first((Object) tmp));
      if (isVariable(list[i])) throw new PInstantiationException(this, 1);
      if (!isCompound(list[i])) throw new IllegalTypeException(this, 1, "key_value_pair", a1);
      if (!prologEquals(jp.ac.kobe_u.cs.prolog.lang.StructureTermBase.functor((Object) list[i]), PRED_keysort_2.SYM_HYPHEN_2)) throw new IllegalTypeException(this, 1, "key_value_pair", a1);
      tmp = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest((Object) tmp));
    }
    if (!prologEquals(tmp, PRED_keysort_2.Nil)) throw new PInstantiationException(this, 1);
    try {
      Arrays.sort(list, new KeySortComparator());
    } catch (final BuiltinException e) {
      e.goal = this;
      e.argNo = 1;
      throw e;
    } catch (final ClassCastException e1) {
      throw new JavaException(this, 1, e1);
    }
    tmp = PRED_keysort_2.Nil;
    for (int i = list.length - 1; i >= 0; i--)
      tmp = makeList(list[i], tmp);
    if (!unify(a2, tmp)) return this.fail(engine);
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
