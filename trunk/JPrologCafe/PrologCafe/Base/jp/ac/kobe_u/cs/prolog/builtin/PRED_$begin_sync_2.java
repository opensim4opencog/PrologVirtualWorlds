package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/**
 * <code>'$begin_sync'/2</code>
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
class PRED_$begin_sync_2 extends BlockPredicate {
  /**
  * 
  */
  private static final long serialVersionUID = -1771007195961174237L;
  Object arg1, arg2;

  public PRED_$begin_sync_2(Object a1, Object a2, Predicate cont) {
    this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
  }

  public PRED_$begin_sync_2() {
  }

  @Override
  public void setArgument(Object[] args, Predicate cont) {
    this.arg1 = args[0];
    this.arg2 = args[1];
    this.cont = cont;
  }

  @Override
  public int arity() {
    return 2;
  }

  @Override
  public String toString() {
    return "$begin_sync(" + this.arg1 + "," + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1, a2;
    a1 = this.arg1;
    a2 = this.arg2;

    Object o = null;
    Predicate code = null;

    // 1st. argument
    a1 = deref(a1);
    if (isVariable(a1)) throw new PInstantiationException(this, 1);
    if (!isJavaObject(a1)) throw new IllegalTypeException(this, 1, "java", a1);
    o = toJava(a1);
    // 2nd. argument
    a2 = deref(a2);
    if (!isVariable(a2)) throw new IllegalTypeException(this, 2, "variable", a1);
    bind(a2, makeJavaObject(this));
    //
    code = this.cont;
    this.outOfScope = false;
    this.outOfLoop = false;
    engine.trail_push(new OutOfLoop(this));
    main_loop: while (true) {
      synchronized (o) {
        while (!this.outOfScope) {
          if (engine.exceptionRaised != 0) {
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
          if (engine.control.thread == null) break main_loop;
          if (code == null) break main_loop;
          if (this.outOfLoop) break main_loop;
          code = code.exec(engine);
        }

      }
      while (this.outOfScope) {
        if (engine.exceptionRaised != 0) {
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
        if (engine.control.thread == null) break main_loop;
        if (code == null) break main_loop;
        if (this.outOfLoop) break main_loop;
        code = code.exec(engine);
      }

    }
    return code;
  }
}
