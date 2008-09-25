package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.InternalException;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;

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
class StructureTermBase extends TermBase implements StructureTerm {
  /**
   * 
   */
  private static final long serialVersionUID = 6905572459626027669L;

  /** Holds the functor symbol of this <code>StructureTerm</code>. */
  final private/*SymbolTerm*/Object functor;

  /** Holds the argument terms of this <code>StructureTerm</code>. */
  final private Object[] args;

  /** Holds the arity of this <code>StructureTerm</code>. */
  final private int arity;

  /* (non-Javadoc)
   * @see be.kuleuven.jProlog.PrologObject#isConst()
   */
  @Override
  public boolean isConst() {
    // TODO Auto-generated method stub
    return false;
  }

  /**
   * Constructs a new Prolog compound term
   * such that <code>_functor</code> is the functor symbol, and
   * <code>_args</code> is the argument terms respectively.
   */
  public StructureTermBase(Object _functor, Object[] _args) {
    this.functor = _functor;
    this.arity = _args.length;//(functor);
    this.args = _args;
    if (this.arity != this.args.length) throw new InternalException("Invalid argument length in StructureTerm");
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
    return this.args;
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

  /* Object */
  @Override
  public boolean unify(Object t) {
    if (StaticProlog.isVariable(t)) return StaticProlog.unify(t, this);
    if (!StaticProlog.isCompound(t)) return false;
    if (!StaticProlog.prologEquals(this.functor, StaticProlog.functor(t))) return false;
    final Object[] targs = StaticProlog.args(t);
    for (int i = 0; i < this.arity; i++) {
      if (!StaticProlog.unify(this.args[i], targs[i])) return false;
    }
    return true;
  }

  @Override
  public Object copy() {
    final Object[] a = new Object[this.arity];
    for (int i = 0; i < this.arity; i++)
      a[i] = StaticProlog.copy(this.args[i]);
    return StaticProlog.makeStructure(this.functor, a);
  }

  @Override
  public boolean isGround() {
    for (int i = 0; i < this.arity; i++) {
      if (!StaticProlog.isGround(this.args[i])) return false;
    }
    return true;
  }

  @Override
  public String toQuotedString() {
    String delim = "";
    String s = StaticProlog.toQuotedString(this.functor) + "(";
    for (int i = 0; i < this.arity; i++) {
      s += delim + StaticProlog.toQuotedString(this.args[i]);
      delim = ",";
    }
    s += ")";
    return s;
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
      if (!StaticProlog.prologEquals(this.args[i], (StaticProlog.deref(targs[i])))) return false;
    }
    return true;
  }

  @Override
  public int hashCode(int d) {
    int h = 1;
    h = 31 * h + StaticProlog.hashCode(this.functor, d);
    for (int i = 0; i < this.arity; i++)
      h = 31 * h + StaticProlog.hashCode(StaticProlog.deref(this.args[i]), d);
    return h;
  }

  /** Returns a string representation of this <code>StructureTerm</code>. */
  @Override
  public String toStringImpl(int d) {
    String delim = "";
    String s = StaticProlog.toString(this.functor()) + "(";
    for (int i = 0; i < this.arity; i++) {
      s += delim + StaticProlog.toString(this.args[i]);
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
      rc = StaticProlog.compareTerm(this.args[i], StaticProlog.deref(args2[i]));
      if (rc != Term.EQUAL) return rc;
    }
    return Term.EQUAL;
  }

}
