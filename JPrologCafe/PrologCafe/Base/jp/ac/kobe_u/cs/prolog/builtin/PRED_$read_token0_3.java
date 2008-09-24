/* PRED_$read_token0_3 - at Sun Sep 21 05:38:55 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.io.PushbackReader;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 8 more classes */
/* import java.lang.StringBuffer; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$read_token0_3; */
/* import java.lang.Object; */
/* import java.lang.Integer; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import java.lang.Double; */
/* import java.lang.Exception; */

class PRED_$read_token0_3 extends PredicateBase {
  /**
   * 
   */
  private static final long serialVersionUID = 7505822672105098025L;
  Object arg1;
  Object arg2;
  Object arg3;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 3;
  }

  public PRED_$read_token0_3() {
    /* flow_0_0_EmptyBlock_1657057220
    */
    /* empty */}

  @Override
  public String toString() {
    /* flow_0_0_StringAddOperator
    */
    return "$read_token0(" + this.arg1 + ", " + this.arg2 + "," + this.arg3 + ")";
  }

  public PRED_$read_token0_3(Object a1, Object a2, Object a3, Predicate cont) {
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
    final Object a2 = this.arg2;
    final Object a3 = this.arg3;
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
    final StringBuffer s = new StringBuffer();
    int type;
    Object token;
    try {
      type = Token.read_token(s, (PushbackReader) stream);
      switch (type) {
        case 73:
          token = makeInteger(Integer.parseInt(s.toString()));
          break;
        case 68:
          token = makeDouble(Double.parseDouble(s.toString()));
          break;
        case 83: {
          final char[] chars = s.toString().toCharArray();
          token = Prolog.Nil;
          for (int i = chars.length; i > 0; i--)
            token = makeList(makeInteger((byte)chars[i - 1]), token);
          break;
        }
        default:
          token = makeAtom(s.toString());
      }
    } catch (final Exception e) {
      throw new TermException(makeJavaObject(e));
    }
    if (!unify(a2, makeInteger(type))) return this.fail(engine);
    if (!unify(a3, token)) return this.fail(engine);
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
