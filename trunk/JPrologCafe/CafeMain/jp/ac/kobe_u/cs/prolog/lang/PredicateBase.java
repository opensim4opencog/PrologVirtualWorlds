/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang;

import java.util.Collection;


/**
 * @author root
 *
 */
abstract public class PredicateBase extends StaticProlog implements Predicate {
  public Predicate cont = null;

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#arity()
   */
  @Override
  abstract public int arity();

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#exec(jp.ac.kobe_u.cs.prolog.lang.Prolog)
   */
  //@Override
  abstract public Predicate exec(Prolog engine) throws PrologException;

  public void enter(Prolog engine) {
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#execCode(jp.ac.kobe_u.cs.prolog.lang.Prolog)
   */
  @Override
  public Predicate execCode(Prolog engine) throws PrologException {
    // TODO Auto-generated method stub
    return exec(engine);
  }
  

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#setArgument(jp.ac.kobe_u.cs.prolog.lang.Object[], jp.ac.kobe_u.cs.prolog.lang.Predicate)
   */
  @Override
  public void setArgument(Object[] args, Predicate cont) {
  }

  /**
   * @param string
   * @param i0
   * @param val
   */
  public Object newIndexOutOfBoundsException(String string, int i0, Object val) {
    // TODO Auto-generated method stub
    throw new IndexOutOfBoundsException(string + " " + i0 + " with " + val);
  }

  public String toPrologString(java.util.Collection newParam) {
    return super.toString();
  }

  /**
   * @param string
   * @param i0
   * @param val
   */
  @Override
  public String toString() {
    return toPrologString(newIdentityLinkedList());
  }

  /**
   * @param arg32
   * @param newParam TODO
   * @return
   */
  public final String argString(Object arg32, Collection newParam) {
    // TODO Auto-generated method stub

    if (arg32 == null) return "__/*NULL*/";
    if (!newParam.add(this)) {
      return ".." + getClass() + "..";
    }
    try {
      //            if (isVariable(arg32)) {
      //                Object arg2 = deref(arg32);
      //                //   if (arg2 != arg32) return arg32.toPrologString(newParam) + "/*=" + arg2.toPrologString(newParam) + "*/";
      //            }
      return "" + arg32.toString();//(40);
    } finally {
      newParam.remove(this);
    }
  }

  /**
   * @param engine
   * @param cont
   * @return
   */
  public Predicate exit(Prolog engine, Predicate cont0) {
    // TODO Auto-generated method stub
    return cont0;
  }

  /** @param engine*/
  public Predicate fail(Prolog engine) {
    // TODO Auto-generated method stub
    return engine.fail();
  }

}
