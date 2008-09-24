package jp.ac.kobe_u.cs.prolog.lang;

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
public class StructureTermBase extends TermBase implements StructureTerm {
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
    functor = _functor;
    arity = _args.length;//(functor);
    args = _args;
    if (arity != args.length) throw new InternalException("Invalid argument length in StructureTerm");
  }

  /** Returns the functor symbol of this <code>StructureTerm</code>.
   * @return the value of <code>functor</code>.
   * @see #functor
   */
  public/*SymbolTerm*/Object functor() {
    return functor;
  }

  /** Returns the arity of this <code>StructureTerm</code>.
   * @return the value of <code>arity</code>.
   * @see #arity
   */
  public int arity() {
    return arity;
  }

  /** Returns the argument terms of this <code>StructureTerm</code>.
   * @return the value of <code>args</code>.
   * @see #args
   */
  public Object[] args() {
    return args;
  }

  /** Returns the string representation of functor symbol of this <code>StructureTerm</code>.
   * @return a <code>String</code> whose value is <code>functor.name()</code>.
   * @see #functor
   * @see SymbolTerm#name
   */
  public String nameUQ() {
    return nameUQ(functor);
  }

  /* Object */
  public boolean unify(Object t) {
    if (isVariable(t)) return unify(t, this);
    if (!isCompound(t)) return false;
    if (!prologEquals(functor, functor(t))) return false;
    Object[] targs = args(t);
    for (int i = 0; i < arity; i++) {
      if (!unify(args[i], targs[i])) return false;
    }
    return true;
  }

  public Object copy(Prolog engine) {
    Object[] a = new Object[arity];
    for (int i = 0; i < arity; i++)
      a[i] = StaticProlog.copy(args[i], engine);
    return Prolog.makeStructure(functor, a);
  }

  public boolean isGround() {
    for (int i = 0; i < arity; i++) {
      if (!isGround(args[i])) return false;
    }
    return true;
  }

  public String toQuotedString() {
    String delim = "";
    String s = toQuotedString(functor) + "(";
    for (int i = 0; i < arity; i++) {
      s += delim + toQuotedString(args[i]);
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
  public boolean equalJProlog(Object obj) {
    if (!(obj instanceof StructureTerm)) return false;
    if (!prologEquals(functor, functor(obj))) return false;
    Object[] targs = args(obj);
    for (int i = 0; i < arity; i++) {
      if (!prologEquals(args[i], (deref(targs[i])))) return false;
    }
    return true;
  }

  public int hashCode(int d) {
    int h = 1;
    h = 31 * h + hashCode(functor, d);
    for (int i = 0; i < arity; i++)
      h = 31 * h + hashCode(deref(args[i]), d);
    return h;
  }

  /** Returns a string representation of this <code>StructureTerm</code>. */
  public String toStringImpl(int d) {
    String delim = "";
    String s = toString(functor()) + "(";
    for (int i = 0; i < arity; i++) {
      s += delim + toString(args[i]);
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

    if (isVariable(anotherTerm) || isNumber(anotherTerm) || isAtomTerm(anotherTerm)) return AFTER;
    if (isListTerm(anotherTerm)) {
      functor2 = ListTerm.SYM_DOT;
      args2 = new Object[2];
      args2[0] = first(anotherTerm);
      args2[1] = rest(anotherTerm);
      arity2 = 2;
    } else if (isCompound(anotherTerm)) {
      functor2 = functor(anotherTerm);
      args2 = args(anotherTerm);
      arity2 = arity(anotherTerm);
    } else {
      return BEFORE;
    }
    if (arity != arity2) return (arity - arity2);
    if (!prologEquals(functor, functor2)) return compareTerm(functor, functor2);
    for (int i = 0; i < arity; i++) {
      rc = compareTerm(args[i], deref(args2[i]));
      if (rc != EQUAL) return rc;
    }
    return EQUAL;
  }

}
