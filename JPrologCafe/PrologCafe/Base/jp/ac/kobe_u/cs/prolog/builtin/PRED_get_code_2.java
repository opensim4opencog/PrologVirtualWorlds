/* PRED_get_code_2 - at Sun Sep 21 05:38:58 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.io.IOException;
import java.io.PushbackReader;

import jp.ac.kobe_u.cs.prolog.lang.*;

public class PRED_get_code_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = -4157699040909593077L;
  public static /*IntegerTerm*/Object INT_EOF = makeInteger(-1);
  public Object arg1;
  public Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_get_code_2() {
    /* flow_0_0_EmptyBlock_908255607
    */
    /* empty */}

  public PRED_get_code_2(Object a1, Object a2, Predicate cont) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
  }
//
//  @Override
//  public String toString() {
//    /* flow_0_0_StringAddOperator
//    */
//    return "get_code(" + this.arg1 + "," + this.arg2 + ")";
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
    if (!isVariable(a2)) {
      if (!isInteger(a2)) throw new IllegalTypeException(this, 2, "integer", a2);
      final int n = (int) intValue(a2);//(IntegerTerm) car).longValue();;
      if ((n != -1) && !Character.isDefined(n)) throw new RepresentationException(this, 2, "in_character_code");
    }
    a1 = deref(a1);
    if (isVariable(a1)) throw new PInstantiationException(this, 1);
    if (isAtomTerm(a1)) {
      if (!engine.getStreamManager().containsKey(a1)) throw new ExistenceException(this, 1, "stream", a1, "");
      stream = toJava(engine.getStreamManager().get(a1));
    } else if (isJavaObject(a1))
      stream = toJava(a1);
    else
      throw new IllegalDomainException(this, 1, "stream_or_alias", a1);
    if (!(stream instanceof PushbackReader)) throw new PermissionException(this, "input", "stream", a1, "");
    try {
      final int c = ((PushbackReader) stream).read();
      if (c < 0) {
        if (!unify(a2, PRED_get_code_2.INT_EOF)) return this.fail(engine);
        return this.cont;
      }
      if (!Character.isDefined(c)) throw new RepresentationException(this, 0, "character");
      if (!unify(a2, makeInteger(c))) return this.fail(engine);
      return this.cont;
    } catch (final IOException e) {
      throw new TermException(makeJavaObject(e));
    }
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
