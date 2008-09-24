package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/**
 * <code>'$begin_exception'/1</code><br>
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 0.8, Jan 2004
 */
class PRED_$begin_exception_1 extends BlockPredicate {
  /**
   * 
   */
  private static final long serialVersionUID = -4863792055687817940L;
  Object arg1;

  public PRED_$begin_exception_1(Object a1, Predicate cont) {
    this.arg1 = a1;
    this.cont = cont;
  }

  public PRED_$begin_exception_1() {
  }

  @Override
  public void setArgument(Object[] args, Predicate cont) {
    this.arg1 = args[0];
    this.cont = cont;
  }

  @Override
  public int arity() {
    return 1;
  }

  @Override
  public String toString() {
    return "$begin_exception(" + this.arg1 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1;
    a1 = this.arg1;

    if (!unify(a1, makeJavaObject(this))) return this.fail(engine);
    Predicate code = this.cont;
    final Object B =  makeInteger(engine.stack_top());
    this.outOfScope = false;
    this.outOfLoop = false;
    engine.trail_push(new OutOfLoop(this));

    try {
      main_loop: while (true) {
        while (engine.exceptionRaised == 0) {
          if (engine.control.thread == null) break main_loop;
          if (code == null) break main_loop;
          if (this.outOfLoop) break main_loop;
         /// System.out.println(";;TRACE "+code);
          code = code.exec(engine);
        }
        switch (engine.exceptionRaised) {
          case 1: // halt/0
            break main_loop;
          case 2: // freeze/2
            throw new SystemException("freeze/2 is not supported yet");
            // Do something here
            // engine.exceptionRaised = 0 ;
            // break
          default:
            break main_loop;
        }
      }
    } catch (final PrologException e) {
      if (this.outOfScope) throw e;
      engine.setException(engine.copy(e.getMessageTerm()));
      engine.cut(B);
      return this.fail(engine);
    } catch (final Exception e) {
      if (this.outOfScope) throw new JavaException(e);
      engine.setException(makeJavaObject(e));
      engine.cut(B);
      return this.fail(engine);
    }
    return code;
  }
}
