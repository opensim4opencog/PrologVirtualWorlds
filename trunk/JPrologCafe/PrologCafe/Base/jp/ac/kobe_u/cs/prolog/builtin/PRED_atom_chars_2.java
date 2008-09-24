/* PRED_atom_chars_2 - at Sun Sep 21 05:38:42 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/* used 5 more classes */
/* import java.lang.StringBuffer; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PRED_atom_chars_2; */
/* import jp.ac.kobe_u.cs.prolog.builtin.PredicateBase; */
/* import java.lang.StringBuilder; */
/* import java.lang.String; */

public class PRED_atom_chars_2 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = -4773050909782667286L;
    static /*SymbolTerm*/Object Nil = makeSymbol("[]");
    Object arg1;
    Object arg2;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 2;
}
    public  PRED_atom_chars_2() {
    /* flow_0_0_EmptyBlock_221571972
 */
/* empty */}
    public  PRED_atom_chars_2(Object a1, Object a2, Predicate cont) {
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
return "atom_chars(" + this.arg1 + "," + this.arg2 + ")";
}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    a1 = deref(a1);
    a2 = deref(a2);
    if (isVariable(a1)){
 if (isNil(a2)){
     if (!unify(a1, makeSymbol("")))
  return this.fail(engine);
     return this.cont;
 }
 if (isVariable(a2))
     throw new PInstantiationException(this, 2);
 if (!isListTerm(a2))
     throw new IllegalTypeException(this, 2, "list", a2);
 final StringBuffer sb = new StringBuffer();
 for (Object x = a2; !isNil(x); x = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest( x))){
     if (isVariable(x))
  throw new PInstantiationException(this, 2);
     if (!isListTerm(x))
  throw new IllegalTypeException(this, 2, "list", a2);
     final Object car = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.first( x));
     if (isVariable(car))
  throw new PInstantiationException(this, 2);
     if (!isAtomTerm(car) || (nameUQ(car).length()!=1))
  throw new IllegalTypeException(this, 2, "character", a2);
     sb.append(nameUQ(car));
 }
 if (!unify(a1, makeSymbol(sb.toString())))
     return this.fail(engine);
 return this.cont;
    }
    if (isNil(a2) || isVariable(a2) || isListTerm(a2)){
 if (!isAtomTerm(a1))
     throw new IllegalTypeException(this, 1, "atom", a1);
 final String s = nameUQ(a1);
 Object x = PRED_atom_chars_2.Nil;
 for (int i = s.length(); i > 0; i--)
     x = makeList(makeSymbol(s.substring(i - 1, i)), x);
 if (!unify(a2, x))
     return this.fail(engine);
 return this.cont;
    }
    return this.fail(engine);
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.arg2 = args[1];
    this.cont = cont;
}}
