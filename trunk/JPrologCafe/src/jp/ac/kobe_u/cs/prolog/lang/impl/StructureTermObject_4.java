package jp.ac.kobe_u.cs.prolog.lang.impl;

import java.lang.reflect.Field;

import jp.ac.kobe_u.cs.prolog.lang.InternalException;
import jp.ac.kobe_u.cs.prolog.lang.JavaException;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation;

/**
 * Example arity 4 Compound term. <br>
 * The <code>StructureTerm</code> class represents a compound term but list.<br>
 * 
 * <pre>
 *  % father(abraham, X)
 *  Object a1 = Prolog.makeSymbol(&quot;abraham&quot;);
 *  Object a2 = Prolog.makeVariable();
 *  Object[] a3 = {a1, a2};
 *  Object a4 = Prolog.makeSymbol(&quot;father&quot;, 2);
 *  Object  t = Prolog.makeStructure(a4, a3);
 *  Object functor = ((StructureTerm)t).functor();
 *  Object[]  args = ((StructureTerm)t).args();
 *  int    arity = ((StructureTerm)t).arity();
 * </pre>
 * 
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public class StructureTermObject_4 extends TermBase implements StructureTerm {

  /**
   * 
   */
  private static final long serialVersionUID = 6905572459626027669L;

  /** Holds the functor symbol of this <code>StructureTerm</code>. */
  //	final private/* SymbolTerm */Class functor;
  /** Holds the argument terms of this <code>StructureTerm</code>. */
  // final private Field[] fields;
  /** Holds the arity of this <code>StructureTerm</code>. */
  //final private int arity;
  /**
   * @param arg1
   * @param arg2
   * @param arg3
   * @param arg4
   */
  public StructureTermObject_4(Object arg1, Object arg2, Object arg3, Object arg4) {
    this();
    this.arg1 = arg1;
    this.arg2 = arg2;
    this.arg3 = arg3;
    this.arg4 = arg4;
  }

  public Object arg1;
  public Object arg2;
  public Object arg3;
  public Object arg4;

  // final private Object obj;

  /**
   * Constructs a new Prolog compound term such that <code>_functor</code>
   * is the functor symbol, and <code>_args</code> is the argument terms
   * respectively.
   */
  public StructureTermObject_4() {
    super();
    // this.obj = idea;
    //    this.functor = this.getClass();
    //  this.fields = getClass().getFields();
    //    //this.arity = fields.length;// (functor);
    //    if (4 != this.fields.length) throw new InternalException("Invalid argument length in StructureTerm");
  }

  /**
   * Returns the functor symbol of this <code>StructureTerm</code>.
   * 
   * @return the value of <code>functor</code>.
   * @see #functor
   */
  public/* SymbolTerm */Object functor() {
    return this.getClass();
  }

  /**
   * Returns the arity of this <code>StructureTerm</code>.
   * 
   * @return the value of <code>arity</code>.
   * @see #arity
   */
  public int arity() {
    return 4;
  }

  /**
   * Returns the argument terms of this <code>StructureTerm</code>.
   * 
   * @return the value of <code>args</code>.
   * @see #args
   */
  public Object[] args() {
    return new Object[] { arg1, arg2, arg3, arg4 };
  }

  /**
   * Returns the string representation of functor symbol of this
   * <code>StructureTerm</code>.
   * 
   * @return a <code>String</code> whose value is
   *         <code>functor.name()</code>.
   * @see #functor
   * @see SymbolTerm#name
   */
  @Override
  public String nameUQ() {
    return "NANnnnnnnnnnnnnnnStaticProlog.nameUQ(this.getClass());";
  }

  /* Object */
  @Override
  public boolean unify(Object t) {
    if (t instanceof StructureTermObject_4) {
      return unify((StructureTermObject_4) t);
    }
    if (StaticProlog.isVariable(t)) return StaticProlog.unify(t, this);
    if (!StaticProlog.isCompound(t)) return false;
    if (!StaticProlog.unify(this.functor(), StaticProlog.functor(t))) return false;
    if (!StaticProlog.unify(arg1, StaticProlog.arg0(t, 0))) return false;
    if (!StaticProlog.unify(arg2, StaticProlog.arg0(t, 1))) return false;
    if (!StaticProlog.unify(arg3, StaticProlog.arg0(t, 2))) return false;
    if (!StaticProlog.unify(arg4, StaticProlog.arg0(t, 3))) return false;
    return true;
  }

  /* Object */
  public boolean unify(StructureTermObject_4 t) {
    if (!StaticProlog.unify(arg1, t.arg0(0))) return false;
    if (!StaticProlog.unify(arg2, t.arg0(1))) return false;
    if (!StaticProlog.unify(arg3, t.arg0(2))) return false;
    if (!StaticProlog.unify(arg4, t.arg0(3))) return false;
    return true;
  }

  @Override
  public Object copy() {
    return new StructureTermObject_4(copy(arg1), copy(arg2), copy(arg3), copy(arg4));
  }

  @Override
  public boolean isGround() {
    if (!StaticProlog.isGround(arg1)) return false;
    if (!StaticProlog.isGround(arg2)) return false;
    if (!StaticProlog.isGround(arg3)) return false;
    if (!StaticProlog.isGround(arg4)) return false;
    return true;
  }

  @Override
  public String toQuotedString(int depth) {
    int d0 = depth - 1;
    String delim = "";
    String s = StaticProlog.toQuotedString(this.getClass()) + "(";
    if (d0 > 0) {
      s += delim + StaticProlog.toQuotedString(this.arg1, d0);
      delim = ",";
      s += delim + StaticProlog.toQuotedString(this.arg2, d0);
      s += delim + StaticProlog.toQuotedString(this.arg3, d0);
      s += delim + StaticProlog.toQuotedString(this.arg3, d0);
    } else {
      s += "......";
    }
    s += ")";
    return s;
  }

  /**
   * @param i
   * @return
   */
  public Object arg0(int i) {
    switch (i) {
    }
    throw new NoSuchFieldError(toString() + " doesnt have a arg" + i);
  }

  /**
   * @param i
   * @return
   */
  public Object arg1(int i) {
    switch (i) {
      case 0: {
        return arg1 != null ? arg1 : makeVariableLoc(varg0(i));
      }
    }
    throw new NoSuchFieldError(toString() + " doesnt have a arg" + i);
  }

  /**
   * @param i
   * @return
   */
  public Object arg2(int i) {
    switch (i) {
      case 0: {
        return arg1 != null ? arg1 : makeVariableLoc(varg0(i));
      }
      case 1: {
        return arg2 != null ? arg2 : makeVariableLoc(varg0(i));
      }
    }
    throw new NoSuchFieldError(toString() + " doesnt have a arg" + i);
  }

  /**
   * @param i
   * @return
   */
  public Object arg3(int i) {
    switch (i) {
      case 0: {
        return arg1 != null ? arg1 : makeVariableLoc(varg0(i));
      }
      case 1: {
        return arg2 != null ? arg2 : makeVariableLoc(varg0(i));
      }
      case 2: {
        return arg3 != null ? arg3 : makeVariableLoc(varg0(i));
      }
    }
    throw new NoSuchFieldError(toString() + " doesnt have a arg" + i);
  }

  /**
   * @param i
   * @return
   */
  public Object arg4(int i) {
    switch (i) {
      case 0: {
        return arg1 != null ? arg1 : makeVariableLoc(varg0(i));
      }
      case 1: {
        return arg2 != null ? arg2 : makeVariableLoc(varg0(i));
      }
      case 2: {
        return arg3 != null ? arg3 : makeVariableLoc(varg0(i));
      }
      case 3: {
        return arg4 != null ? arg4 : makeVariableLoc(varg0(i));
      }
    }
    throw new NoSuchFieldError(toString() + " doesnt have a arg" + i);
  }


  public VariableTermLocation varg0(final int i) {
    switch (i) {
      case 0: {
        return (new VariableTermLocation() {
          @Override
          public Object getVal() {
            return arg1;
          }

          @Override
          public boolean isBound() {
            return arg1 != null;
          }

          @Override
          public void setVal(Object o) {
            arg1 = o;
          }

          @Override
          public String toString() {
            return "arg1 of " + StructureTermObject_4.this + "%" + super.toString();
          }
        });
      }
      case 1: {
        return (new VariableTermLocation() {
          @Override
          public Object getVal() {
            return arg2;
          }

          @Override
          public boolean isBound() {
            return arg2 != null;
          }

          @Override
          public void setVal(Object o) {
            arg2 = o;
          }

          @Override
          public String toString() {
            return "arg2 of " + StructureTermObject_4.this + "%" + super.toString();
          }
        });
      }
      case 2: {
        return (new VariableTermLocation() {
          @Override
          public Object getVal() {
            return arg3;
          }

          @Override
          public boolean isBound() {
            return arg3 != null;
          }

          @Override
          public void setVal(Object o) {
            arg3 = o;
          }

          @Override
          public String toString() {
            return "arg3 of " + StructureTermObject_4.this + "%" + super.toString();
          }
        });
      }
      case 3: {
        return (new VariableTermLocation() {
          @Override
          public Object getVal() {
            return arg4;
          }

          @Override
          public boolean isBound() {
            return arg4 != null;
          }

          @Override
          public void setVal(Object o) {
            arg4 = o;
          }

          @Override
          public String toString() {
            return "arg4 of " + StructureTermObject_4.this + "%" + super.toString();
          }
        });
      }
    }
    throw new NoSuchFieldError(toString() + " doesnt have a arg" + i);
  }

  /* Object */
  /**
   * Checks <em>term equality</em> of two terms. The result is
   * <code>true</code> if and only if the argument is an instance of
   * <code>StructureTerm</code>, has the same functor symbol and arity, and
   * all corresponding pairs of arguments in the two compound terms are
   * <em>term-equal</em>.
   * 
   * @param obj
   *            the object to compare with. This must be dereferenced.
   * @return <code>true</code> if the given object represents a Prolog
   *         compound term equivalent to this <code>StructureTerm</code>,
   *         false otherwise.
   * @see #compareTo
   */
  @Override
  public boolean equalJProlog(Object obj) {
    if (!(obj instanceof StructureTerm)) return false;
    if (!StaticProlog.prologEquals(this.getClass(), StaticProlog.functor(obj))) return false;
    if (!StaticProlog.prologEquals(arg1, StaticProlog.arg0(obj, 0))) return false;
    if (!StaticProlog.prologEquals(arg2, StaticProlog.arg0(obj, 1))) return false;
    if (!StaticProlog.prologEquals(arg3, StaticProlog.arg0(obj, 2))) return false;
    if (!StaticProlog.prologEquals(arg4, StaticProlog.arg0(obj, 3))) return false;
    return true;
  }

  @Override
  public int hashCode(int d) {
    int h = 1;
    h = 31 * h + StaticProlog.hashCode(this.getClass(), d);
    h = 31 * h + StaticProlog.hashCode(StaticProlog.deref(arg1), d);
    h = 31 * h + StaticProlog.hashCode(StaticProlog.deref(arg2), d);
    h = 31 * h + StaticProlog.hashCode(StaticProlog.deref(arg3), d);
    h = 31 * h + StaticProlog.hashCode(StaticProlog.deref(arg4), d);
    return h;
  }

  /** Returns a string representation of this <code>StructureTerm</code>. */
  @Override
  public String toStringImpl(int d) {
    int d0 = d - 1;
    String delim = "";
    String s = StaticProlog.toString(nameUQ()) + "(";
    if (d0 > 0) {
      s += delim + StaticProlog.toString(this.arg1, d0);
      delim = ",";
      s += delim + StaticProlog.toString(this.arg2, d0);
      s += delim + StaticProlog.toString(this.arg3, d0);
      s += delim + StaticProlog.toString(this.arg4, d0);
    } else {
      s += "......";
    }
    s += ")";
    return s;
  }

  /* Comparable */
  /**
   * Compares two terms in <em>Prolog standard order of terms</em>.<br>
   * It is noted that <code>t1.compareTo(t2) == 0</code> has the same
   * <code>boolean</code> value as <code>t1.equals(t2)</code>.
   * 
   * @param anotherTerm
   *            the term to compared with. It must be dereferenced.
   * @return the value <code>0</code> if two terms are identical; a value
   *         less than <code>0</code> if this term is <em>before</em> the
   *         <code>anotherTerm</code>; and a value greater than
   *         <code>0</code> if this term is <em>after</em> the
   *         <code>anotherTerm</code>.
   */

  public int compareTo1(Object anotherTerm) { // anotherTerm must be
    // dereferenced.
    /* SymbolTerm */Object functor2;
    int arity2, rc;
    if (StaticProlog.isVariable(anotherTerm) || StaticProlog.isNumber(anotherTerm) || StaticProlog.isAtomTerm(anotherTerm)) return Term.AFTER;
    if (StaticProlog.isListTerm(anotherTerm)) {
      return (-1);
    } else if (StaticProlog.isCompound(anotherTerm)) {
      arity2 = StaticProlog.arity(anotherTerm);
      if (1 != arity2) return (1 - arity2);
      functor2 = StaticProlog.functor(anotherTerm);
      rc = StaticProlog.compareTerm(this.functor(), functor2);
      if (rc != 0) return rc;
      rc = StaticProlog.compareTerm(arg1, StaticProlog.deref(arg0(anotherTerm, 0)));
      return rc;
    } else {
      return Term.BEFORE;
    }
  }

  public int compareTo2(Object anotherTerm) { // anotherTerm must be
    // dereferenced.
    /* SymbolTerm */Object functor2;
    int arity2, rc;

    if (StaticProlog.isVariable(anotherTerm) || StaticProlog.isNumber(anotherTerm) || StaticProlog.isAtomTerm(anotherTerm)) return Term.AFTER;
    if (StaticProlog.isListTerm(anotherTerm)) {
      rc = StaticProlog.compareTerm(this.functor(), ListTerm.SYM_DOT);
      if (rc != 0) return rc;
      rc = StaticProlog.compareTerm(arg1, StaticProlog.first(anotherTerm));
      if (rc != 0) return rc;
      rc = StaticProlog.compareTerm(arg2, StaticProlog.rest(anotherTerm));
      return rc;
    } else if (StaticProlog.isCompound(anotherTerm)) {
      arity2 = StaticProlog.arity(anotherTerm);
      if (4 != arity2) return (4 - arity2);
      functor2 = StaticProlog.functor(anotherTerm);
      rc = StaticProlog.compareTerm(this.getClass(), functor2);
      if (rc != 0) return rc;
      rc = StaticProlog.compareTerm(arg1, StaticProlog.deref(arg0(anotherTerm, 0)));
      if (rc != 0) return rc;
      rc = StaticProlog.compareTerm(arg2, StaticProlog.deref(arg0(anotherTerm, 1)));
      return rc;
    } else {
      return Term.BEFORE;
    }
  }

  public int compareTo(Object anotherTerm) { // anotherTerm must be
    // dereferenced.
    /* SymbolTerm */Object functor2;
    if (StaticProlog.isVariable(anotherTerm) || StaticProlog.isNumber(anotherTerm) || StaticProlog.isAtomTerm(anotherTerm)) return Term.AFTER;
    if (StaticProlog.isListTerm(anotherTerm)) {
      return (4 - 2);
    } else if (StaticProlog.isCompound(anotherTerm)) {
      int arity2, rc;
      arity2 = StaticProlog.arity(anotherTerm);
      if (4 != arity2) return (4 - arity2);
      functor2 = StaticProlog.functor(anotherTerm);
      rc = StaticProlog.compareTerm(this.getClass(), functor2);
      if (rc != 0) return rc;
      rc = StaticProlog.compareTerm(arg1, StaticProlog.deref(arg0(anotherTerm, 0)));
      if (rc != 0) return rc;
      rc = StaticProlog.compareTerm(arg2, StaticProlog.deref(arg0(anotherTerm, 1)));
      if (rc != 0) return rc;
      rc = StaticProlog.compareTerm(arg3, StaticProlog.deref(arg0(anotherTerm, 2)));
      if (rc != 0) return rc;
      rc = StaticProlog.compareTerm(arg4, StaticProlog.deref(arg0(anotherTerm, 3)));
      return rc;
    } else {
      return Term.BEFORE;
    }
  }
}
