/* PRED_close_2 - at Sun Sep 21 05:39:02 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.PushbackReader;

import jp.ac.kobe_u.cs.prolog.lang.*;

public class PRED_close_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 3076288140617792857L;
  public static /*SymbolTerm*/Object SYM_ALIAS_1 = makeAtom("alias", 1);
  public static /*SymbolTerm*/Object SYM_FORCE_1 = makeAtom("force", 1);
  public static /*SymbolTerm*/Object SYM_TRUE = makeAtom("true");
  public static /*SymbolTerm*/Object SYM_FALSE = makeAtom("false");
  public Object arg1;
  public Object arg2;

  @Override
  public int arity() {
    /* flow_0_0_ConstantOperator
    */
    return 2;
  }

  public PRED_close_2() {
    /* flow_0_0_EmptyBlock_761170597
    */
    /* empty */}

  public PRED_close_2(Object a1, Object a2, Predicate cont) {
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
    return "close(" + this.arg1 + "," + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    boolean forceFlag = false;
    Object stream = null;
    a2 = deref(a2);
    for (Object tmp = a2; !isNil(tmp); tmp = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest((Object) tmp))) {
      if (isVariable(tmp)) throw new PInstantiationException(this, 2);
      if (!isListTerm(tmp)) throw new IllegalTypeException(this, 2, "list", a2);
      final Object car = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.first((Object) tmp));
      if (isVariable(car)) throw new PInstantiationException(this, 2);
      if (isCompound(car)) {
        final /*SymbolTerm*/Object functor = jp.ac.kobe_u.cs.prolog.lang.StructureTermBase.functor((Object) car);
        final Object[] args = args(car);
        if (prologEquals(functor, PRED_close_2.SYM_FORCE_1)) {
          final Object bool = deref(args[0]);
          if (prologEquals(bool, PRED_close_2.SYM_TRUE)) {
            forceFlag = true;
            continue;
          }
          if (prologEquals(bool, PRED_close_2.SYM_FALSE)) {
            forceFlag = false;
            continue;
          }
          throw new IllegalDomainException(this, 2, "close_option", car);
        }
        throw new IllegalDomainException(this, 2, "close_option", car);
      }
      throw new IllegalDomainException(this, 2, "close_option", car);
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
    do {
      if (stream instanceof PushbackReader) {
        final PushbackReader in = (PushbackReader) stream;
        if (in.equals(engine.getUserInput())) return this.cont;
        if (in.equals(engine.getCurrentInput())) engine.setCurrentInput(engine.getUserInput());
        try {
          in.close();
          break;
        } catch (final IOException e) {
          throw new TermException(makeJavaObject(e));
        }
      }
      if (stream instanceof PrintWriter) {
        final PrintWriter out = (PrintWriter) stream;
        if (out.checkError() && !forceFlag) throw new SystemException("output stream error");
        out.flush();
        if (out.equals(engine.getUserOutput()) || out.equals(engine.getUserError())) return this.cont;
        if (out.equals(engine.getCurrentOutput())) engine.setCurrentOutput(engine.getUserOutput());
        out.close();
      } else
        throw new IllegalDomainException(this, 1, "stream_or_alias", a1);
    } while (false);
    final HashtableOfTerm streamManager = engine.getStreamManager();
    if (isAtomTerm(a1)) {
      streamManager.remove(engine.getStreamManager().get(a1));
      streamManager.remove(a1);
    } else if (isJavaObject(a1)) {
      for (Object tmp2 =(Object)  streamManager.get(a1); !isNil(tmp2); tmp2 = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest((Object) tmp2))) {
        final Object car = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.first((Object) tmp2));
        if (isCompound(car)) {
          final /*SymbolTerm*/Object functor = jp.ac.kobe_u.cs.prolog.lang.StructureTermBase.functor((Object) car);
          final Object[] args = args(car);
          if (prologEquals(functor, PRED_close_2.SYM_ALIAS_1)) {
            final Object alias = deref(args[0]);
            streamManager.remove(alias);
          }
        }
      }
      streamManager.remove(a1);
    } else
      throw new IllegalDomainException(this, 1, "stream_or_alias", a1);
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
