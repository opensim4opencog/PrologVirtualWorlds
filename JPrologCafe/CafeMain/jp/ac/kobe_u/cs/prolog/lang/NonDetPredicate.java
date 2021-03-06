package jp.ac.kobe_u.cs.prolog.lang;

import java.util.Collection;


/*
 This file is generated by Prolog Cafe.
 superclass for Non Deterministic java impl predicates
*/

abstract public class NonDetPredicate extends PredicateBase implements Predicate {// CallControl {

  abstract protected class PortPred extends PredicateBase {

    /* (non-Javadoc)
     * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#arity()
     */
    @Override
    public int arity() {
      // TODO Auto-generated method stub
      return NonDetPredicate.this.arity();
    }

    //    /* (non-Javadoc)
    //     * @see tarau.jinni.FunBase#setArg(int, com.cyc.tool.subl.jrtl.nativeCode.type.core.Object)
    //     */
    //    @Override
    //    public void setArg(int i, Object T) {
    //      NonDetPredicate.this.setArg(i, T);
    //
    //    }
    //
    //    /* (non-Javadoc)
    //     * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#setArgument(com.cyc.tool.subl.jrtl.nativeCode.type.core.Object[], jp.ac.kobe_u.cs.prolog.lang.Predicate)
    //     */
    @Override
    public void setArgument(Object[] args, Predicate cont) {
      // TODO Auto-generated method stub
      NonDetPredicate.this.setArgument(args, cont);
    }

    /* (non-Javadoc)
     * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#toPredString()
     */
    @Override
    public String toPrologString(Collection newParam) {
      // TODO Auto-generated method stub
      return NonDetPredicate.this.toPrologString(newParam);
    }

  }
  protected PortPred call_top = new PortPred() {
    @Override
    public Predicate exec(Prolog engine) {
      enter(engine);
      engine.setB0();
      return exit(engine, engine.jtry(NonDetPredicate.this.nd_call, NonDetPredicate.this.redo_sub));
    }
  };
  protected Predicate redo_sub = new PortPred() {
    @Override
    public Predicate exec(Prolog engine) {
      enter(engine);
      return exit(engine, engine.trust(NonDetPredicate.this.nd_next));
    }

  };
  protected Predicate nd_call = new PortPred() {
    @Override
    public Predicate exec(Prolog engine) {
      enter(engine);
      final Predicate savedcont = engine.cont;
      if (!NonDetPredicate.this.call(engine)) return fail(engine);
      return exit(engine, savedcont);
    }
  };

  ///////////////////////////////////////////

  protected Predicate nd_next = new PortPred() {
    @Override
    public Predicate exec(Prolog engine) {
      enter(engine);
      final Predicate savedcont = engine.cont;
      if (!NonDetPredicate.this.advance(engine)) {
        return fail(engine);
      }
      engine.cont = savedcont;
      return exit(engine, NonDetPredicate.this.call_top);
    }
  };

  /**
   *  This advance/P is called before each redo which is expected to be redone on next call/P
   *
   *  @return <code>false</code> when there are no more solutions (otherwise <code>true</code>)
   *  note:  Cleanups should happen the first time it is going to return <code>false</code>.
   */
  abstract public boolean advance(Prolog engine);

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#arity()
   */
  @Override
  abstract public int arity();

  /**
   *  This call/P is called for the first call and subsequent redo's
   *
   *  @return <code>false</code> when the args do not unify (otherwise <code>true</code>)
   *  note: May be again backtracked over even after returning <code>false</code> but not until
   *  this's advance/P returns <code>true</code>.
   */
  abstract public boolean call(Prolog engine);

  @Override
  final public Predicate exec(Prolog engine) {
    enter(engine);
    if (!this.init(engine)) return fail(engine);;
    return exit(engine, this.call_top);
  }

  /**
   * HasNext might be called multiple times
   *
   * (Therefore the result should not change except between advance/Ps)
   *
   * @return <code>false</code> when there are no more solutions (otherwise <code>true</code>)
   */

  abstract public boolean hasNext();

  /**
   *  This init/P is not a call/P as it will be called once as to setup for the first call/P
   *
   *  @return <code>false</code> when the args initially would not unify (otherwise <code>true</code>)
   *
   *  Example for arg/3
   */
  abstract public boolean init(Prolog engine);

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#toPredString()
   */
  //  @Override
  //  abstract public String toPrologString(Collection newParam) throws NullPointerException;
}
