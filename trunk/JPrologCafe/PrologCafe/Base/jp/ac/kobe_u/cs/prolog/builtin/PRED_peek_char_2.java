/* PRED_peek_char_2 - at Sun Sep 21 05:38:50 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import java.io.IOException;
import java.io.PushbackReader;

import jp.ac.kobe_u.cs.prolog.lang.*;

public class PRED_peek_char_2 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = 3643102696425524225L;
    public static /*SymbolTerm*/Object SYM_EOF = makeAtom("end_of_file");
    public Object arg1;
    public Object arg2;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 2;
}
      boolean inCharacter(Object t) {
        /* flow_0_0_IfThenElseBlock_1963878523
 */
if (!isAtomTerm(t))
 return false;
    if (prologEquals(t, PRED_peek_char_2.SYM_EOF))
 return true;
    if (nameUQ(t).length()==1)
 return true;
    return false;
}
    public  PRED_peek_char_2() {
    /* flow_0_0_EmptyBlock_82681671
 */
/* empty */}
    public  PRED_peek_char_2(Object a1, Object a2, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
}
    @Override
    public  String toString() {
        /* flow_0_0_StringAddOperator
 */
return "peek_char(" + this.arg1 + "," + this.arg2 + ")";
}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    Object stream = null;
    a2 = deref(a2);
    if (!isVariable(a2) && !this.inCharacter(a2))
 throw new IllegalTypeException(this, 2, "in_character", a2);
    a1 = deref(a1);
    if (isVariable(a1))
 throw new PInstantiationException(this, 1);
    if (isAtomTerm(a1)){
 if (!engine.getStreamManager().containsKey(a1))
     throw new ExistenceException(this, 1, "stream", a1, "");
 stream = toJava(engine.getStreamManager().get(a1));
    } else if (isJavaObject(a1))
 stream = toJava(a1);
    else
 throw new IllegalDomainException(this, 1, "stream_or_alias", a1);
    if (!(stream instanceof PushbackReader))
 throw new PermissionException(this, "input", "stream", a1, "");
    try {
 final int c = ((PushbackReader) stream).read();
 if (c < 0){
     if (!unify(a2, PRED_peek_char_2.SYM_EOF))
  return this.fail(engine);
     return this.cont;
 }
 if (!Character.isDefined(c))
     throw new RepresentationException(this, 0, "character");
 ((PushbackReader) stream).unread(c);
 if (!unify(a2, makeAtom(String.valueOf((char) c))))
     return this.fail(engine);
 return this.cont;
    } catch (final IOException e){
 throw new TermException(makeJavaObject(e));
    }
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.arg2 = args[1];
    this.cont = cont;
}}
