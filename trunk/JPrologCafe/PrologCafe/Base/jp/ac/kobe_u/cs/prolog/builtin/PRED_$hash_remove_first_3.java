/* PRED_$hash_remove_first_3 - at Sun Sep 21 05:38:59 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/* used 4 more classes */
/* import java.lang.Object; */
/* import java.lang.String; */
/* import java.lang.StringBuilder; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_$hash_remove_first_3; */

class PRED_$hash_remove_first_3 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = -8286572780865685524L;
  public static/*SymbolTerm*/Object SYM_NIL = makeAtom("[]");
  public Object arg1;
  public Object arg2;
  public Object arg3;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 3;
  }

  public PRED_$hash_remove_first_3() {
    /* flow_0_0_EmptyBlock_2009270675
    */
    /* empty */}

  @Override
  public String toString() {
    /* flow_0_0_StringAddOperator
    */
    return "$hash_remove_first(" + this.arg1 + "," + this.arg2 + "," + this.arg3 + ")";
  }

  public PRED_$hash_remove_first_3(Object a1, Object a2, Object a3, Predicate cont) {
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
    Object a3 = this.arg3;
    Object hash = null;
    a1 = deref(a1);
    if (isVariable(a1)) throw new PInstantiationException(this, 1);
    if (isAtomTerm(a1)) {
      if (!engine.getHashManager().containsKey(a1)) throw new ExistenceException(this, 1, "hash", a1, "");
      hash = toJava(engine.getHashManager().get(a1));
    } else if (isJavaObject(a1))
      hash = toJava(a1);
    else
      throw new IllegalDomainException(this, 1, "hash_or_alias", a1);
    if (!(hash instanceof HashtableOfTerm)) throw new InternalException(this + ": Hash is not HashtableOfTerm");
    a2 = deref(a2);
    Object elem = (Object) ((HashtableOfTerm) hash).get(a2);
    if ((elem == null) || isNil(elem)) return this.cont;
    a3 = deref(a3);
    Object x = elem;
    final Object Nil = Prolog.Nil;
    Object x0 = Nil;
    Object z;
    for (/**/; !isNil(x); x = z) {
      if (!isListTerm(x)) throw new InternalException(this + ": the valus of " + a2 + " is not list structure");
      final Object y = deref(first(x));
      z = deref(rest(x));
      if (prologEquals(y, a3)) {
        if (isNil(z)) {
          if (isListTerm(x0))
            setCdr(x0, Nil);
          else
            elem = Nil;
        } else {
          setCar(x, deref(first((Object) z)));
          setCdr(x, deref(rest((Object) z)));
        }
        break;
      }
      x0 = x;
    }
    if (isNil(elem) && isInteger(a2))
      ((HashtableOfTerm) hash).remove(a2);
    else
      ((HashtableOfTerm) hash).put(a2, elem);
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
