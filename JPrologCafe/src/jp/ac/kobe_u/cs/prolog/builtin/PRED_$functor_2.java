/* PRED_$functor_2 - at Sun Jul 13 19:55:44 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.util.Collection;

import jp.ac.kobe_u.cs.prolog.lang.JavaPredicate;
import jp.ac.kobe_u.cs.prolog.lang.Predicate;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;

public class PRED_$functor_2 extends JavaPredicate {
  /**
   *
   */
  private static final long serialVersionUID = 4109608981683844863L;
  public Object arg1;
  public Object arg2;

  public PRED_$functor_2() {
    /* flow_0_0_EmptyBlock_1852833748
    */
    /* empty */}

  public PRED_$functor_2(Object term, Object term_0_, Predicate predicate) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = term;
    this.arg2 = term_0_;
    this.cont = predicate;
  }

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  @Override
  public Predicate exec(Prolog prolog) {
    /* flow_0_0_INVOKE_VIRTUAL
    */
    prolog.setB0();
    Object term = this.arg1;
    final Object term_1_ = this.arg2;
    term = deref(term);
    final Object functor = StaticProlog.functor(term);
    if (!unify(term_1_, functor)) return prolog.fail();
    return this.cont;
  }

  @Override
  public void setArgument(Object[] terms, Predicate predicate) {
    /* flow_0_0_PutFieldOperator
    */
    this.arg1 = terms[0];
    this.arg2 = terms[1];
    this.cont = predicate;
  }

  //  @Override
  public String toPrologString(Collection newParam) {
    /* flow_0_0_StringAddOperator
    */
    return "$functor(" + this.arg1 + "," + this.arg2 + ")";
  }
}
