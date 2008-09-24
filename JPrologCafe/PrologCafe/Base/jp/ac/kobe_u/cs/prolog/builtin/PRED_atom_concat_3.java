/* PRED_atom_concat_3 - at Sun Sep 21 05:39:01 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 4 more classes */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$member_in_reverse_2; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_atom_concat_3; */

public class PRED_atom_concat_3 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 6215763422195688291L;
  static /*SymbolTerm*/Object AC_2 = makeAtom("ac", 2);
  public Object arg1;
  public Object arg2;
  public Object arg3;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 3;
  }

  public PRED_atom_concat_3() {
    /* flow_0_0_EmptyBlock_920482395
    */
    /* empty */}

  @Override
  public String toString() {
    /* flow_0_0_StringAddOperator
    */
    return "atom_concat(" + this.arg1 + "," + this.arg2 + "," + this.arg3 + ")";
  }

  public PRED_atom_concat_3(Object a1, Object a2, Object a3, Predicate cont) {
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
    a3 = deref(a3);
    if (isAtomTerm(a3)) {
      final String str3 = nameUQ(a3);
      final int endIndex = str3.length();
      Object t =  Prolog.Nil;
      for (int i = 0; i <= endIndex; i++) {
        final Object[] args = { makeAtom(str3.substring(0, i)), makeAtom(str3.substring(i, endIndex)) };
        t = makeList(makeStructure(PRED_atom_concat_3.AC_2, args), t);
      }
      final Object[] args12 = { a1, a2 };
      return new PRED_$member_in_reverse_2(makeStructure(PRED_atom_concat_3.AC_2, args12), t, this.cont);
    }
    if (!isVariable(a3)) throw new IllegalTypeException(this, 3, "atom", a3);
    a1 = deref(a1);
    a2 = deref(a2);
    if (isVariable(a1)) throw new PInstantiationException(this, 1);
    if (isVariable(a2)) throw new PInstantiationException(this, 2);
    if (!isAtomTerm(a1)) throw new IllegalTypeException(this, 1, "integer", a1);
    if (!isAtomTerm(a2)) throw new IllegalTypeException(this, 2, "integer", a2);
    final String str3 = nameUQ(a1).concat(nameUQ(a2));
    if (!unify(a3, makeAtom(str3))) return this.fail(engine);
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
