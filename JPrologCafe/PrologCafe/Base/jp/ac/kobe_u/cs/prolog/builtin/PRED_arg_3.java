/* PRED_arg_3 - at Sun Sep 21 05:39:05 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.util.Collection;
import java.util.Collections;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 3 more classes */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_arg_3; */

public class PRED_arg_3 extends PredicateBase {
  /**
  *
  */
  private static final long serialVersionUID = -8154087947534762503L;
  Object arg1;
  Object arg2;
  Object arg3;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 3;
  }

  public PRED_arg_3() {
    /* flow_0_0_EmptyBlock_1284852143
    */
    /* empty */}

//  @Override
//  public String toString() {
//    /* flow_0_0_StringAddOperator
//    */
//    Collection param = newIdentityLinkedList();
//    return "arg(" + argString(arg1, param) + "," + argString(arg2, param) + "," + argString(arg3, param) + ")";
//  }

  public PRED_arg_3(Object a1, Object a2, Object a3, Predicate cont) {
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
    final Object a3 = this.arg3;
    a1 = StaticProlog.deref(a1);
    if (StaticProlog.isVariable(a1)) throw new PInstantiationException(this, 1);
    if (!StaticProlog.isInteger(a1)) throw new IllegalTypeException(this, 1, "integer", a1);
    a2 = StaticProlog.deref(a2);
    Object[] args;
    int arity;
    if (StaticProlog.isListTerm(a2)) {
      args = new Object[2];
      args[0] = StaticProlog.first(a2);
      args[1] = StaticProlog.rest(a2);
      arity = 2;
    } else if (StaticProlog.isCompound(a2)) {
      args = StaticProlog.args(a2);
      arity = StaticProlog.arity(a2);
    } else {
      if (StaticProlog.isVariable(a2)) throw new PInstantiationException(this, 2);
      throw new IllegalTypeException(this, 2, "compound", a2);
    }
    final int argNo =  (int) intValue(a1);//(IntegerTerm) car).longValue();;
    if (argNo < 0) throw new IllegalDomainException(this, 1, "not_less_than_zero", a1);
    if ((argNo > arity) || (argNo < 1)) return this.fail(engine);
    if (!StaticProlog.unify(a3, args[argNo - 1])) return this.fail(engine);
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
