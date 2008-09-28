package jp.ac.kobe_u.cs.prolog.builtin;

import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.*;
import jp.ac.kobe_u.cs.prolog.lang.NonDetPredicate;
import jp.ac.kobe_u.cs.prolog.lang.Predicate;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;

/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>java_instanceof/3</code> defined in callbox.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@daxtron.com)
 @version 1.0-dmiles
 
   java_instanceof(Obj,ClassName).

   Gets the class, superclasses and interfaces of an object.
   if ClassName is unbound it return JavaObjectTerms that wrap classes
   if ClassName is bound then it will try to unify with the argument with what is returned with above.
   However, if ClassName supplied is an atom like 'native' it will succced if one of the above classes
   and interface names end with 'native' like 'foo.bar.wildnative' or 'wild_native'.

*/
public class PRED_java_instanceof_2 extends NonDetPredicate {
  /**
   * 
   */
  private static final long serialVersionUID = -8599798881026405564L;

  static void addClasses(Set set, Class c) {
    if (c != null) if (set.add(c)) {
      addClasses(set, c.getSuperclass());
      for (final Class cs : c.getInterfaces()) {
        addClasses(set, cs);
      }
    }
  }

  Iterator iter;
  Object arg1, arg2;
  String orName = null;
  boolean isComplete = false;

  public PRED_java_instanceof_2() {
  }

  public PRED_java_instanceof_2(Object a1, Object a2, Object a3, Predicate cont) {
    this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
  }

  /** (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.builtin.PRED_java_instanceof_2.CallControl#advance(jp.ac.kobe_u.cs.prolog.lang.Prolog)
   */
  @Override
  public boolean advance(Prolog engine) {
    if (!this.hasNext()) return false;
    return true;
  }

  /** (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Predicate#arity()
   */
  @Override
  public int arity() {
    // TODO Auto-generated method stub
    return 2;
  }

  /** (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.builtin.PRED_java_instanceof_2.CallControl#redo(jp.ac.kobe_u.cs.prolog.lang.Prolog)
   */
  @Override
  public boolean call(Prolog engine) {
    if (isComplete) return true;
    Object a1, a2;
    final Predicate cont = engine.cont;
    Object[] enginearegs = engine.getAreg();
    a1 = enginearegs[1];
    a2 = enginearegs[2];
    a2 = deref(a2);
    boolean hasNext = this.iter.hasNext();
    while (hasNext) {
      final Object next = this.iter.next();
      final Object a3 = (next);
      if (unify(a2, a3)) return true;
      if (orName != null) {
        if (next.toString().endsWith(orName)) {
          isComplete = true;
          return true;
        }
      }
      hasNext = this.iter.hasNext();
    }
    return false;
  }

  /** (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.builtin.PRED_java_instanceof_2.CallControl#hasNext()
   */
  @Override
  public boolean hasNext() {
    if (isComplete) return false;
    return this.iter.hasNext();
  }

  @Override
  public boolean init(Prolog engine) {
    Object[] enginearegs = engine.getAreg();
    enginearegs[1] = this.arg1;
    enginearegs[2] = this.arg2;
    engine.cont = this.cont;
    final Object a1 = deref(this.arg1);
    final Object a2 = deref(this.arg2);
    final HashSet classes = new HashSet();
    addClasses(classes, a1.getClass());
    final Object o = toJava(a1);
    if (o != null) addClasses(classes, o.getClass());
    this.iter = classes.iterator();
    if (isAtomTerm(a2)) {
      this.orName = nameUQ(a2);
    }
    return true;
  }

  /**
  * (non-Javadoc)
  * @see jp.ac.kobe_u.cs.prolog.builtin.ThisPredicate#setArgument(com.cyc.tool.subl.jrtl.nativeCode.type.core.Object[], jp.ac.kobe_u.cs.prolog.lang.Predicate)
  */

  @Override
  public void setArgument(Object[] args, Predicate cont) {
    this.arg1 = args[0];
    this.arg2 = args[1];
    this.cont = cont;
  }

  //@Override
  public String toPrologString(Collection newParam) {
    return "java_instanceof(" + this.arg1 + "," + this.arg2 + ")";
  }
}