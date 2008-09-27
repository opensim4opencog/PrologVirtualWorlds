package jp.ac.kobe_u.cs.prolog.lang.impl;

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
public interface StructureTerm extends Term {
  //    /** Holds the functor symbol of this <code>StructureTerm</code>. */
  //    final private  /*SymbolTerm*/Object functor;
  //
  //    /** Holds the argument terms of this <code>StructureTerm</code>. */
  //    final private  Object[] args;
  //
  //    /** Holds the arity of this <code>StructureTerm</code>. */
  //    final private  int arity;
  //
  //    /**
  //     * Constructs a new Prolog compound term
  //     * such that <code>_functor</code> is the functor symbol, and
  //     * <code>_args</code> is the argument terms respectively.
  //     */
  //    public StructureTerm(/*SymbolTerm*/Object _functor, Object[] _args){
  //	functor = _functor;
  //	arity = functor.arity();
  //	args = _args;
  //	if (arity != args.length)
  //	    throw new InternalException("Invalid argument length in StructureTerm");
  //    }

  /** Returns the functor symbol of this <code>StructureTerm</code>.
   * @return the value of <code>functor</code>.
   * @see #functor
   */
  public/*SymbolTerm*/Object functor();//{ return functor; }

  //
  //  /** Returns the arity of this <code>StructureTerm</code>.
  //   * @return the value of <code>arity</code>.
  //   * @see #arity
  //   */
  public int arity();//{ return arity; }

  //
  /** Returns the argument terms of this <code>StructureTerm</code>.
   * @return the value of <code>args</code>.
   * @see #args
   */

  public Object[] args();//{ return args; }

  //
  //  /** Returns the string representation of functor symbol of this <code>StructureTerm</code>.
  //   * @return a <code>String</code> whose value is <code>functor.name()</code>.
  //   * @see #functor
  //   * @see /*SymbolTerm*/Object#name
  //   */
  //  public String nameUQ();//{ return functor.name(); }
  //
  //  /* Object */
  //  public boolean unify(Object t);//
  //
  //  public Object copy(Prolog engine);
  //
  //  public boolean isGround();
  //
  //  public String toQuotedString();
  //
  //  /* Object */
  //  /**
  //   * Checks <em>term equality</em> of two terms.
  //   * The result is <code>true</code> if and only if the argument is an instance of
  //   * <code>StructureTerm</code>, has the same functor symbol and arity, and
  //   * all corresponding pairs of arguments in the two compound terms are <em>term-equal</em>.
  //   * @param obj the object to compare with. This must be dereferenced.
  //   * @return <code>true</code> if the given object represents a Prolog compound term
  //   * equivalent to this <code>StructureTerm</code>, false otherwise.
  //   * @see #compareTo
  //   */
  //  public boolean equals(Object obj);
  //
  //  public int hashCode();
  //
  //  /** Returns a string representation of this <code>StructureTerm</code>. */
  //  public String toString();
  //
  //  /* Comparable */
  //  /**
  //   * Compares two terms in <em>Prolog standard order of terms</em>.<br>
  //   * It is noted that <code>t1.compareTo(t2) == 0</code> has the same
  //   * <code>boolean</code> value as <code>t1.equals(t2)</code>.
  //   * @param anotherTerm the term to compared with. It must be dereferenced.
  //   * @return the value <code>0</code> if two terms are identical;
  //   * a value less than <code>0</code> if this term is <em>before</em> the <code>anotherTerm</code>;
  //   * and a value greater than <code>0</code> if this term is <em>after</em> the <code>anotherTerm</code>.
  //   */
  //  public int compareTo(Object anotherTerm);

  /**
   * @param i
   * @return
   */
  public Object arg0(int i);

  /**
   * @return
   */
  public String nameUQ();

}
