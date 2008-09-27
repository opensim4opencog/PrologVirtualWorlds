/* PRED_put_code_2 - at Sun Sep 21 05:39:04 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.io.PrintWriter;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 5 more classes */
/* import java.lang.Object; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_put_code_2; */
/* import java.lang.Character; */

public class PRED_put_code_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = -7652239915268089470L;
  public Object arg1;
  public Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_put_code_2() {
    /* flow_0_0_EmptyBlock_314160763
    */
    /* empty */}

  public PRED_put_code_2(Object a1, Object a2, Predicate cont) {
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
//    return "put_code(" + this.arg1 + "," + this.arg2 + ")";
//  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    Object stream = null;
    a2 = deref(a2);
    if (isVariable(a2)) throw new PInstantiationException(this, 2);
    if (!isInteger(a2)) throw new IllegalTypeException(this, 2, "integer", a2);
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
    final int c = (int) intValue(a2);//(IntegerTerm) car).longValue();;
    if (!Character.isDefined(c)) throw new RepresentationException(this, 2, "character");
    ((PrintWriter) stream).print((char) c);
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
