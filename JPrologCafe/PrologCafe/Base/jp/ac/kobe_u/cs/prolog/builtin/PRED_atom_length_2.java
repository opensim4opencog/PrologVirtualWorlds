/* PRED_atom_length_2 - at Sun Sep 21 05:39:02 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 3 more classes */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_atom_length_2; */

public class PRED_atom_length_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 2941499554794425145L;
  public Object arg1;
  public Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_atom_length_2() {
    /* flow_0_0_EmptyBlock_1362924847
    */
    /* empty */}

  public PRED_atom_length_2(Object a1, Object a2, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
  }

//  @Override
//  public String toString() {
//    /* flow_0_0_StringAddOperator
//    */
//    return "atom_length(" + this.arg1 + "," + this.arg2 + ")";
//  }

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
    if (isVariable(a1)) throw new PInstantiationException(this, 1);
    if (!isAtomTerm(a1)) throw new IllegalTypeException(this, 1, "atom", a1);
    final int length = nameUQ(a1).length();
    if (isVariable(a2)) {
      if (!unify(a2, makeInteger(length))) return this.fail(engine);
    } else if (isInteger(a2)) {
      final int n = (int) intValue(a2);//(IntegerTerm) car).longValue();;
      if (n < 0) throw new IllegalDomainException(this, 2, "not_less_than_zero", a2);
      if (length != n) return this.fail(engine);
    } else
      throw new IllegalTypeException(this, 1, "integer", a2);
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
