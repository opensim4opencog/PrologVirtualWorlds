package jp.ac.kobe_u.cs.prolog.lang.impl;

import java.lang.reflect.Field;

import com.sun.org.apache.bcel.internal.generic.VariableLengthInstruction;

import jp.ac.kobe_u.cs.prolog.lang.InternalException;
import jp.ac.kobe_u.cs.prolog.lang.JavaException;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation;

/**
 * Compound term. <br>
 * The <code>StructureTerm</code> class represents a compound term but list.<br>
 *
 * <pre>
 *  % father(abraham, X)
 *  Object a1 = Prolog.makeSymbol("abraham");
 *  Object a2 = Prolog.makeVariable();
 *  Object[] a3 = {a1, a2};
 *  Object a4 = Prolog.makeSymbol("father", 2);
 *  Object  t = Prolog.makeStructure(a4, a3);
 *
 *  Object functor = ((StructureTerm)t).functor();
 *  Object[]  args = ((StructureTerm)t).args();
 *  int    arity = ((StructureTerm)t).arity();
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
final class StructureTermObject extends TermBase implements StructureTerm, JavaObjectTerm {
  /**
   * 
   */
  private static final long serialVersionUID = 6905572459626027669L;

  /** Holds the functor symbol of this <code>StructureTerm</code>. */
  final private/*SymbolTerm*/Class functor;

  /** Holds the argument terms of this <code>StructureTerm</code>. */
  private Field[] fields;
  private Object[] args;

  /** Holds the arity of this <code>StructureTerm</code>. */
  final private int arity;

  final private Object obj;

  /**
   * Constructs a new Prolog compound term
   * such that <code>_functor</code> is the functor symbol, and
   * <code>_args</code> is the argument terms respectively.
   */
  public StructureTermObject(Object idea) {
    //this.obj = idea;
    obj = idea;
    this.functor = idea.getClass();
    this.fields = functor.getFields();
    if (fields.length == 0) {
      this.fields = functor.getDeclaredFields();
    }
    this.arity = fields.length;//(functor);
    args = new Object[arity];
    if (this.arity != this.fields.length) throw new InternalException("Invalid argument length in StructureTerm");
    for (int i = 0; i < arity; i++) {
      args[i] = arg0(i);
    }
  }

  /** Returns the functor symbol of this <code>StructureTerm</code>.
   * @return the value of <code>functor</code>.
   * @see #functor
   */
  public/*SymbolTerm*/Object functor() {
    return this.functor;
  }

  /** Returns the arity of this <code>StructureTerm</code>.
   * @return the value of <code>arity</code>.
   * @see #arity
   */
  public int arity() {
    return this.arity;
  }

  /** Returns the argument terms of this <code>StructureTerm</code>.
   * @return the value of <code>args</code>.
   * @see #args
   */
  public Object[] args() {
    if (true) return args;
    Object[] args = new Object[arity];
    for (int i = 0; i < arity; i++) {
      try {
        args[i] = arg0(i);
      } catch (Exception e) {
        e.printStackTrace();
        throw new JavaException(e);
      }
    }
    return args;
  }

  /** Returns the string representation of functor symbol of this <code>StructureTerm</code>.
   * @return a <code>String</code> whose value is <code>functor.name()</code>.
   * @see #functor
   * @see SymbolTerm#name
   */
  @Override
  public String nameUQ() {
    return StaticProlog.nameUQ(this.functor);
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.TermBase#toJava()
   */
  @Override
  public Object toJava() {
    return obj;
  }

  /* Object */
  @Override
  public boolean unify(Object t) {
    if (StaticProlog.isVariable(t)) return StaticProlog.unify(t, this);
    if (!StaticProlog.isCompound(t)) return false;
    if (!StaticProlog.prologEquals(this.functor, StaticProlog.functor(t))) return false;
    final Object[] targs = StaticProlog.args(t);
    for (int i = 0; i < this.arity; i++) {
      if (!StaticProlog.unify(this.arg0(i), targs[i])) return false;
    }
    return true;
  }

  @Override
  public Object copy() {
    if (true) return this;
    final Object[] a = new Object[this.arity];
    for (int i = 0; i < this.arity; i++)
      a[i] = StaticProlog.copy(this.arg0(i));
    return StaticProlog.makeStructure(this.functor, a);
  }

  @Override
  public boolean isGround() {
    for (int i = 0; i < this.arity; i++) {
      if (!StaticProlog.isGround(this.arg0(i))) return false;
    }
    return true;
  }

  @Override
  public String toQuotedString(int d) {
    String delim = "";
    String s = StaticProlog.toQuotedString(this.functor, d) + "(";
    for (int i = 0; i < this.arity; i++) {
      s += delim + StaticProlog.toQuotedString(this.arg0(i), d);
      delim = ",";
    }
    s += ")";
    return s;
  }

  /**
   * @param i
   * @return
   */
  public Object arg0(int i) {
    if (args[i] != null) return args[i];
    Field f = fields[i];
    try {
      Object res = f.get(obj);
      if (res == null) {
        return termFieldLocationVar(fields[i], obj);
      }
      return res;
    } catch (IllegalArgumentException e) {
      return termFieldLocationVar(fields[i], obj);
    } catch (IllegalAccessException e) {
      f.setAccessible(true);
      return arg0(i);
    }
  }

  /**
   * @param field
   * @param obj2
   * @return
   */
  private Object termFieldLocationVar(Field field, Object obj2) {
    return StaticProlog.makeVariableLoc(new VariableFieldLocation(field, obj2));
  }

  /* Object */
  /**
   * Checks <em>term equality</em> of two terms.
   * The result is <code>true</code> if and only if the argument is an instance of
   * <code>StructureTerm</code>, has the same functor symbol and arity, and
   * all corresponding pairs of arguments in the two compound terms are <em>term-equal</em>.
   * @param obj the object to compare with. This must be dereferenced.
   * @return <code>true</code> if the given object represents a Prolog compound term
   * equivalent to this <code>StructureTerm</code>, false otherwise.
   * @see #compareTo
   */
  @Override
  public boolean equalJProlog(Object obj) {
    if (!(obj instanceof StructureTerm)) return false;
    if (!StaticProlog.prologEquals(this.functor, StaticProlog.functor(obj))) return false;
    final Object[] targs = StaticProlog.args(obj);
    for (int i = 0; i < this.arity; i++) {
      if (!StaticProlog.prologEquals(this.arg0(i), (StaticProlog.deref(targs[i])))) return false;
    }
    return true;
  }

  @Override
  public int hashCode(int d) {
    int h = 1;
    h = 31 * h + StaticProlog.hashCode(this.functor, d);
    for (int i = 0; i < this.arity; i++)
      h = 31 * h + StaticProlog.hashCode(StaticProlog.deref(this.arg0(i)), d);
    return h;
  }

  /** Returns a string representation of this <code>StructureTerm</code>. */
  @Override
  public String toStringImpl(int d) {
    String delim = "";
    String s = StaticProlog.toString(this.functor()) + "(";
    for (int i = 0; i < this.arity; i++) {
      s += delim + StaticProlog.toString(this.arg0(i));
      delim = ",";
    }
    s += ")";
    return s;
  }

  /* Comparable */
  /**
   * Compares two terms in <em>Prolog standard order of terms</em>.<br>
   * It is noted that <code>t1.compareTo(t2) == 0</code> has the same
   * <code>boolean</code> value as <code>t1.equals(t2)</code>.
   * @param anotherTerm the term to compared with. It must be dereferenced.
   * @return the value <code>0</code> if two terms are identical;
   * a value less than <code>0</code> if this term is <em>before</em> the <code>anotherTerm</code>;
   * and a value greater than <code>0</code> if this term is <em>after</em> the <code>anotherTerm</code>.
   */
  public int compareTo(Object anotherTerm) { // anotherTerm must be dereferenced.
    /*SymbolTerm*/Object functor2;
    Object[] args2;
    int arity2, rc;

    if (StaticProlog.isVariable(anotherTerm) || StaticProlog.isNumber(anotherTerm) || StaticProlog.isAtomTerm(anotherTerm)) return Term.AFTER;
    if (StaticProlog.isListTerm(anotherTerm)) {
      functor2 = ListTerm.SYM_DOT;
      args2 = new Object[2];
      args2[0] = StaticProlog.first(anotherTerm);
      args2[1] = StaticProlog.rest(anotherTerm);
      arity2 = 2;
    } else if (StaticProlog.isCompound(anotherTerm)) {
      functor2 = StaticProlog.functor(anotherTerm);
      args2 = StaticProlog.args(anotherTerm);
      arity2 = StaticProlog.arity(anotherTerm);
    } else {
      return Term.BEFORE;
    }
    if (this.arity != arity2) return (this.arity - arity2);
    if (!StaticProlog.prologEquals(this.functor, functor2)) return StaticProlog.compareTerm(this.functor, functor2);
    for (int i = 0; i < this.arity; i++) {
      rc = StaticProlog.compareTerm(this.arg0(i), StaticProlog.deref(args2[i]));
      if (rc != Term.EQUAL) return rc;
    }
    return Term.EQUAL;
  }

}
