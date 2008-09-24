/* PRED_atom_codes_2 - at Sun Sep 21 05:39:01 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 5 more classes */
/* import java.lang.StringBuffer; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import java.lang.Character; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_atom_codes_2; */

public class PRED_atom_codes_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = -913101710459999986L;
  static /*SymbolTerm*/Object Nil = makeAtom("[]");
  Object arg1;
  Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_atom_codes_2() {
    /* flow_0_0_EmptyBlock_709342045
    */
    /* empty */}

  public PRED_atom_codes_2(Object a1, Object a2, Predicate cont) {
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
    return "atom_codes(" + this.arg1 + "," + this.arg2 + ")";
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
    if (isVariable(a1)) {
      final StringBuffer sb = new StringBuffer();
      for (Object x = a2; !isNil(x); x = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest(x))) {
        if (isVariable(x)) throw new PInstantiationException(this, 2);
        if (!isListTerm(x)) throw new IllegalTypeException(this, 2, "list", a2);
        final Object car = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.first(x));
        if (isVariable(car)) throw new PInstantiationException(this, 2);
        if (!isInteger(car)) throw new RepresentationException(this, 2, "character_code");
        final int i =  (int) intValue(car);//(IntegerTerm) car).longValue();;
        if (!Character.isDefined((char) i)) throw new RepresentationException(this, 2, "character_code");
        sb.append((char) i);
      }
      if (!unify(a1, makeAtom(sb.toString()))) return this.fail(engine);
      return this.cont;
    }
    if (!isAtomTerm(a1)) throw new IllegalTypeException(this, 1, "atom", a1);
    final char[] chars = nameUQ(a1).toCharArray();
    Object x = PRED_atom_codes_2.Nil;
    for (int i = chars.length; i > 0; i--)
      x = makeList(makeInteger((byte)chars[i - 1]), x);
    if (!unify(a2, x)) return this.fail(engine);
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
