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
    if (false) {
      String s = "TRACE: " + this;
      System.out.flush();
      if (s.contains("PRED_nl_")) {
        //  System.err.print(false);
      }
      System.out.println(s);
      System.out.flush();
    }
    Predicate p = exec(engine);
    System.out.flush();
    return p;
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

  public String toPrologString(Collection newParam) {
    String s;
    final Class c = this.getClass();
    s = c.getSimpleName();
    boolean added = newParam.add(this);
    s += "(";
    final int a = this.arity();
    for (int i = 1; i <= a; i++) {
      java.lang.reflect.Field f;
      Object v;
      try {
        f = c.getDeclaredField("arg" + i);
        f.setAccessible(true);
        try {
          v = f.get(this);
          s += argString(v, newParam);
        } catch (final IllegalArgumentException e) {
          // TODO Auto-generated catch block
          e.printStackTrace();
        } catch (final IllegalAccessException e) {
          // TODO Auto-generated catch block
          e.printStackTrace();
        }
      } catch (final SecurityException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      } catch (final NoSuchFieldException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
      }

    }
    s += ")";
    if (added) {
      newParam.remove(this);
    }
    return s;
  }

  /**
   * @param string
   * @param i0
   * @param val
   */
  @Override
  final public String toString() {
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
      return ".." + arg32.getClass().getSimpleName() + "..";
    }
    try {
      if (isVariable(arg32)) {
        Object arg2 = deref(arg32);
        //   if (arg2 != arg32) return arg32.toPrologString(newParam) + "/*=" + arg2.toPrologString(newParam) + "*/";
      }
      try {
        return "" + StaticProlog.toString(arg32, 23);//(40);
      } catch (Throwable t) {
        return "..<" + arg32.getClass().getSimpleName() + ">..";
      }
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
