/* PRED_tab_2 - at Sun Sep 21 05:39:02 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.io.PrintWriter;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 4 more classes */
/* import java.lang.Object; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_tab_2; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */

public class PRED_tab_2 extends PredicateBase {
  /**
   * 
   */
  private static final long serialVersionUID = 8778258553766372149L;
  public Object arg1;
  public Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_tab_2() {
    /* flow_0_0_EmptyBlock_1176318351
    */
    /* empty */}

  public PRED_tab_2(Object a1, Object a2, Predicate cont) {
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
    return "tab(" + this.arg1 + "," + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    String s = "";
    Object stream = null;
    a2 = deref(a2);
    if (isVariable(a2)) throw new PInstantiationException(this, 2);
    if (!isInteger(a2)) {
      try {
        a2 = Arithmetic.evaluate(a2);
      } catch (final BuiltinException e) {
        e.goal = this;
        e.argNo = 2;
        throw e;
      }
    }
    final int n = (int) intValue(a2);
    a1 = deref(a1);
    if (isVariable(a1)) throw new PInstantiationException(this, 1);
    if (isAtomTerm(a1)) {
      if (!engine.getStreamManager().containsKey(a1)) throw new ExistenceException(this, 1, "stream", a1, "");
      stream = toJava(engine.getStreamManager().get(a1));
    } else if (isJavaObject(a1))
      stream = toJava(a1);
    else
      throw new IllegalDomainException(this, 1, "stream_or_alias", a1);
    if (!(stream instanceof PrintWriter)) throw new PermissionException(this, "output", "stream", a1, "");
    for (int i = 0; i < n; i++)
      s += " ";
    ((PrintWriter) stream).print(s);
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
