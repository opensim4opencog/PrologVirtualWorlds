package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.InternalException;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.Token;

/**
 * Atom.<br>
 * The <code>SymbolTerm</code> class represents a Prolog atom.<br>
 *
 * <pre>
 *   Object t = Prolog.makeSymbol("kobe");
 *   String name = ((SymbolTerm)t).name();
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
class SymbolTermBase extends TermBase implements SymbolTerm {
  /**
   * 
   */
  private static final long serialVersionUID = -6890434653855891020L;

  /* (non-Javadoc)
  * @see jp.ac.kobe_u.cs.prolog.lang.CafeTerm#equalJProlog(java.lang.Object)
  */
  @Override
  public boolean equalJProlog(Object o) {
    // TODO Auto-generated method stub
    boolean t = o == this;
    if (t) return true;
    if (!StaticProlog.isAtomTerm(o)) return false;
    if (!this.nameUQ().equals(StaticProlog.nameUQ(o))) return false;
    t = StaticProlog.arity(o) == this.arity();
    if (!t) return false;
    return true;//throw new Error();
  }

  /** Symbol table. */
  //final private static Hashtable<String,SymbolTerm> SYMBOL_TABLE = new Hashtable<String,SymbolTerm>();
  /** Holds a string representation of this <code>SymbolTerm</code>. */
  public Object value;

  /** Holds the arity of this <code>SymbolTerm</code>. */
  final private int arity;

  /** Returns a Prolog atom for the given name. */
  public static Object makeSymbol(Object _name) {
    // if (true) return _name;
    return SymbolTermBase.makeSymbol(_name, 0);
  }

  /** Returns a Prolog functor for the given name and arity. */
  public static Object makeSymbol(Object _name, int _arity) {
    final String key = _name + "/" + _arity;
    Object sym;
    synchronized (SymbolTerm.SYMBOL_TABLE) {
      sym = SymbolTerm.SYMBOL_TABLE.get(key);
      if (sym == null) {
        if (_arity == 0)
          sym = _name;//new SymbolTermBase(_name, 0);
        else
          sym = new SymbolTermBase(_name, _arity);
        SymbolTerm.SYMBOL_TABLE.put(key, sym);
      }
    }
    return sym;
  }

  /** Check whether this term is an empty list. */
  @Override
  public boolean isNil() {
    return StaticProlog.prologEquals(Prolog.Nil, this);
  }

  /* (non-Javadoc)
   * @see be.kuleuven.jProlog.PrologObject#isConst()
   */
  @Override
  public boolean isConst() {
    return !(this.value instanceof String);
  }

  /** Constructs a new Prolog atom (or functor) with the given symbol name and arity. */
  protected SymbolTermBase(Object _name, int _arity) {
    this.value = _name;
    this.arity = _arity;
  }

  /** Returns the arity of this <code>SymbolTerm</code>.
   * @return the value of <code>arity</code>.
   * @see #arity
   */
  public int arity() {
    return this.arity;
  }

  /** Returns the string representation of this <code>SymbolTerm</code>.
   * @return the value of <code>name</code>.
   * @see #value
   */
  @Override
  public String nameUQ() {
    return this.value.toString();
  }

  /* Object */
  @Override
  public boolean unify(Object t) {
    if (StaticProlog.isVariable(t)) return StaticProlog.unify(t, this);
    //    if (!isAtomTerm(t)) // ???
    //      return false;
    return this.equalJProlog(t);
    //	return name.equals(((SymbolTerm)t).name());
  }

  /**
   * @return the <code>boolean</code> whose value is
   * <code>convertible(String.class, type)</code>.
   * @see Object#convertible(Class, Class)
   */
  @Override
  public boolean convertible(Class type) {
    return StaticProlog.convertible(String.class, type);
  }

  /**
   * Returns a <code>java.lang.String</code> corresponds to this <code>SymbolTerm</code>
   * according to <em>Prolog Cafe interoperability with Java</em>.
   * @return a <code>java.lang.String</code> object equivalent to
   * this <code>SymbolTerm</code>.
   */
  @Override
  public Object toJava() {
    return this.value;
  }

  @Override
  public String toQuotedString() {
    return Token.toQuotedString(this.value.toString());
  }

  /** Returns a string representation of this <code>SymbolTerm</code>. */
  @Override
  public String toStringImpl(int d) {
    if (this.arity == 0) return "" + this.value;
    return "" + this.value;//+"/"+arity;
  }

  @Override
  public String toString() {
    if (this.arity == 0) return "" + this.value;//+"/"+arity;
    return "" + this.value + "/" + this.arity;
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
    if (StaticProlog.isVariable(anotherTerm) || StaticProlog.isNumber(anotherTerm)) return Term.AFTER;
    if (!StaticProlog.isAtomTerm(anotherTerm)) return Term.BEFORE;
    if (this == anotherTerm) return Term.EQUAL;
    final int x = this.value.toString().compareTo(StaticProlog.nameUQ((anotherTerm)));
    if (x != 0) return x;
    final int y = this.arity - StaticProlog.arity(anotherTerm);
    if (y != 0) return y;
    throw new InternalException("SymbolTerm is not unique");
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.TermBase#hashCode(int)
   */
  @Override
  public int hashCode(int depth) {
    // TODO Auto-generated method stub
    return System.identityHashCode(this);
  }
}
