package jp.ac.kobe_u.cs.prolog.builtin;

import java.util.Collection;

import jp.ac.kobe_u.cs.prolog.lang.IllegalDomainException;
import jp.ac.kobe_u.cs.prolog.lang.IllegalTypeException;
import jp.ac.kobe_u.cs.prolog.lang.NonDetPredicate;
import jp.ac.kobe_u.cs.prolog.lang.PInstantiationException;
import jp.ac.kobe_u.cs.prolog.lang.Predicate;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.StructureTerm;
import jp.ac.kobe_u.cs.prolog.lang.*;

/**
 <code>arg/3</code> defined in callbox.pl<br>

*/
public class PRED_nd_arg_3 extends NonDetPredicate {
  /////////////////////////////////////////////////////////////////////////////////////

  int arity = -1;
  int currentArg = 1;
  Object[] compound;

  /////////////////////////////////////////////////////////////////////////////////////

  Object arg1, arg2, arg3;

  public PRED_nd_arg_3() {
    PRED_arg_3.class.getName();
  }

  public PRED_nd_arg_3(Object a1, Object a2, Object a3, Predicate cont) {
    this.arg1 = a1;
    this.arg2 = a2;
    this.arg3 = a3;
    this.cont = cont;
  }

  /**
   *  This advance/P is called before each redo which is expected to be redone on next call/P
   *
   *  @return <code>false</code> when there are no more solutions (otherwise <code>true</code>)
   *  note:  Cleanups should happen the first time it is going to return <code>false</code>.
   */
  @Override
  public boolean advance(Prolog engine) {
    if (this.currentArg++ > this.arity) {
      this.compound = null;
      return false;
    }
    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /** (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#arity()
   */
  @Override
  public int arity() {
    // TODO Auto-generated method stub
    return 3;
  }

  /**
   *  This call/P is called for the first call and subsequent redo's
   *
   *  @return <code>false</code> when the args do not unify (otherwise <code>true</code>)
   *  note: May be again backtracked over even after returning <code>false</code> but not until
   *  this's advance/P returns <code>true</code>.
   */
  @Override
  public boolean call(Prolog engine) {
    if (this.currentArg > this.arity) return false;
    Object a1, a3;
    Object[] enginearegs = engine.getAreg();
    a1 = deref(enginearegs[1]);
    a3 = enginearegs[3];

    if (isInteger(a1)) {
      if (intValue(a1) != this.currentArg) return false;
    } else if (isVariable(a1)) {
      bind(a1, makeInteger(this.currentArg));
    } else {
      return false;
    }

    if (!unify(deref(a3), this.compound[this.currentArg - 1])) return false;

    return true;
  }

  /**
   * HasNext might be called multiple times
   *
   * (Therefore the result should not change except between advance/Ps)
   *
   * @return <code>false</code> when there are no more solutions (otherwise <code>true</code>)
   */
  @Override
  public boolean hasNext() {
    return !(this.currentArg > this.arity);
  }

  /**
   *  This init/P is called once as to setup for the first call/P
   *
   *  Important! This is not a call/P!
   *
   *  @return <code>false</code> when the args initially would not unify (otherwise <code>true</code>)
   *
   *  Example for arg/3
   *
   *  @see NonDetPredicate#exec(Prolog)
   */
  @Override
  public boolean init(Prolog engine) {
    Object[] enginearegs = engine.getAreg();
    enginearegs[1] = this.arg1;
    enginearegs[2] = this.arg2;
    enginearegs[3] = this.arg3;
    engine.cont = this.cont;
    final Object a2 = deref(this.arg2);
    if (isVariable(a2)) throw new PInstantiationException(this, 2);
    if (!isCompound(a2) && !isListTerm(a2)) throw new IllegalTypeException(this, 2, "compound", a2);
    this.compound = args(a2);
    this.arity = compound.length;// arity(compound);

    final Object a1 = deref(this.arg1);
    if (isVariable(a1)) {
      this.currentArg = 1;
      return true;
    } else if (isInteger(a1)) {
      this.currentArg = (int) intValue(a1);
      if (this.currentArg < 0) {
        throw new IllegalDomainException(this, 1, "not_less_than_zero", a1);
      } else if (this.currentArg == 0) {
        return false;
      } else if (this.currentArg > this.arity) {
        return false;
      }
      this.arity = this.currentArg;
      return true;
    } else {
      return false;
    }
  }

  /**
  * (non-Javadoc)
  * @see jp.ac.kobe_u.cs.prolog.builtin.ThisPredicate#setArgument(com.cyc.tool.subl.jrtl.nativeCode.type.core.Object[], jp.ac.kobe_u.cs.prolog.lang.Predicate)
  */

  @Override
  public void setArgument(Object[] args, Predicate cont) {
    this.arg1 = args[0];
    this.arg2 = args[1];
    this.arg3 = args[2];
    this.cont = cont;
  }

  //@Override
  public String toPrologString(Collection newParam) {
    return "nd_arg(" + argString(this.arg1, newParam) + "," + argString(this.arg2, newParam) //        this.arg2 
        + "," + argString(this.arg3, newParam) + ")";
  }// deref(this.arg1) + "," + deref(this.arg2) + "," + deref(this.arg3) + ")";
  //}
}
