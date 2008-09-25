package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.Predicate;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;

/**
 * Closure.<br>
 * The class <code>ClosureTerm</code> represents a closure structure.<br>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
class ClosureTermBase extends TermBase implements ClosureTerm {
  /**
   * 
   */
  private static final long serialVersionUID = -3130411850977948777L;
  /** Holds a <code>Predicate</code> object that represents a Prolog goal. */
  final private Predicate code;

  /** Constructs a new closure structure for the given Prolog goal. */
  public ClosureTermBase(Predicate _code) {
    this.code = _code;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.Object#nameUQ()
   */
  @Override
  public String nameUQ() {
    // TODO Auto-generated method stub
    this.typeError("NAMED");
    return this.code.toString();
  }

  /** Returns the Prolog goal of this <code>ClosureTerm</code>. */
  public Predicate getCode() {
    return this.code;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.TermBase#isConst()
   */
  @Override
  public boolean isConst() {
    // TODO Auto-generated method stub
    return false;
  }

  /* Object */
  @Override
  public boolean unify(Object t) {
    //	t = t.deref();
    if (StaticProlog.isVariable(t)) return StaticProlog.unify(t, this);
    if (!StaticProlog.isClosure(t)) return false;
    return this.code.equals(((ClosureTerm) t).getCode());
  }

  @Override
  public String toQuotedString() {
    return this.toString();
  }

  /* Object */
  /**
   * Checks <em>term equality</em> of two terms.
   * The result is <code>true</code> if and only if the argument is an instance of
   * <code>ClosureTerm</code>, and
   * the pairs of goals in the two closures are <em>equal</em>.
   * @param obj the object to compare with. This must be dereferenced.
   * @return <code>true</code> if the given object represents a closure
   * equivalent to this <code>ClosureTerm</code>, false otherwise.
   * @see #compareTo
   */
  @Override
  public boolean equalJProlog(Object obj) { // obj must be dereferenced
    if (!(obj instanceof ClosureTerm)) return false;
    return this.code.equals(((ClosureTerm) obj).getCode());
  }

  @Override
  public int hashCode(int d) {
    return this.code.hashCode();
  }

  /** Returns a string representation of this <code>ClosureTerm</code>. */
  @Override
  public String toStringImpl(int d) {
    return "closure[" + this.code.toString() + "]";
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
  public int compareTo(Object anotherTerm) { // anotherTerm must be dereferenced
    if (!StaticProlog.isClosure(anotherTerm)) return Term.AFTER;
    if (this.code.equals(((ClosureTerm) anotherTerm).getCode())) return Term.EQUAL;
    return this.code.hashCode() - ((ClosureTerm) anotherTerm).getCode().hashCode(); //???
  }

  /**
   * @param a1
   * @return
   */
  public static Predicate getCode(ClosureTerm a1) {
    // TODO Auto-generated method stub
    return a1.getCode();
  }
}
