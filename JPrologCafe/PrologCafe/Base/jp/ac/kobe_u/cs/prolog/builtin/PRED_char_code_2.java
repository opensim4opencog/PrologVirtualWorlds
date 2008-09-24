/* PRED_char_code_2 - at Sun Sep 21 05:39:00 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 4 more classes */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_char_code_2; */
/* import java.lang.Character; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */

public class PRED_char_code_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 7580463660570869121L;
  Object arg1;
  Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_char_code_2() {
    /* flow_0_0_EmptyBlock_1825486557
    */
    /* empty */}

  public PRED_char_code_2(Object a1, Object a2, Predicate cont) {
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
    return "char_code(" + this.arg1 + "," + this.arg2 + ")";
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
      if (isVariable(a2)) throw new PInstantiationException(this, 2);
      if (!isInteger(a2)) throw new IllegalTypeException(this, 2, "integer", a2);
      final int i = (int) intValue(a2);//(IntegerTerm) car).longValue();;
      if (!Character.isDefined(i)) throw new RepresentationException(this, 2, "character_code");
      if (!unify(a1, makeAtom(String.valueOf((char) i)))) return this.fail(engine);
    } else if (isAtomTerm(a1)) {
      final String s = nameUQ(a1);
      if (s.length() != 1) throw new IllegalTypeException(this, 1, "character", a1);
      if (!unify(a2, makeInteger((byte) s.charAt(0)))) return this.fail(engine);
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
