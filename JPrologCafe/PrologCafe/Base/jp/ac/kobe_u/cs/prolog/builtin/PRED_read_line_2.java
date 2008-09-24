/* PRED_read_line_2 - at Sun Sep 21 05:38:56 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.PushbackReader;

import jp.ac.kobe_u.cs.prolog.lang.*;

class PRED_read_line_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = -3149213445255626624L;
  Object arg1;
  Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_read_line_2() {
    /* flow_0_0_EmptyBlock_897350024
    */
    /* empty */}

  public PRED_read_line_2(Object a1, Object a2, Predicate cont) {
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
    return "read_line(" + this.arg1 + "," + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    final Object a2 = this.arg2;
    Object stream = null;
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
      final String line = new BufferedReader((PushbackReader) stream).readLine();
      if (line == null) {
        if (!unify(a2, makeInteger(-1))) return this.fail(engine);
        return this.cont;
      }
      final char[] chars = line.toCharArray();
      Object t = Prolog.Nil;
      for (int i = chars.length; i > 0; i--) {
        if (!Character.isDefined((int) chars[i - 1])) throw new RepresentationException(this, 0, "character");
        t = makeList(makeInteger((byte) chars[i - 1]), t);
      }
      if (!unify(a2, t)) return this.fail(engine);
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
