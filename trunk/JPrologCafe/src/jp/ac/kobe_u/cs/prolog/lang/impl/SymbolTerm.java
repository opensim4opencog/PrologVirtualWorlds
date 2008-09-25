package jp.ac.kobe_u.cs.prolog.lang.impl;

import java.util.Hashtable;


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
interface SymbolTerm extends Term {

  /** Symbol table. */
  final static Hashtable<String, Object> SYMBOL_TABLE = new Hashtable<String, Object>();

  //
  /** Returns the arity of this <code>SymbolTerm</code>.
   * @return the value of <code>arity</code>.
   * @see #arity
   */
  public int arity();//{ return arity; }

  //
  /** Returns the string representation of this <code>SymbolTerm</code>.
   * @return the value of <code>name</code>.
   * @see #name
   */
  public String nameUQ();//{ return name; }
  //
  //    /* Object */
  //    public boolean unify(Object t);//
  //
  //    /** 
  //     * @return the <code>boolean</code> whose value is
  //     * <code>convertible(String.class, type)</code>.
  //     * @see Object#convertible(Class, Class)
  //     */
  //    public boolean convertible(Class type);//{ return convertible(String.class, type); }
  //
  //    /** 
  //     * Returns a <code>java.lang.String</code> corresponds to this <code>SymbolTerm</code>
  //     * according to <em>Prolog Cafe interoperability with Java</em>.
  //     * @return a <code>java.lang.String</code> object equivalent to
  //     * this <code>SymbolTerm</code>.
  //     */
  //    public Object toJava();//{ return name; }
  //
  //    public String toQuotedString();// { return Token.toQuotedString(name); }
  //
  //    /** Returns a string representation of this <code>SymbolTerm</code>. */
  //    public String toString();// { return name; }
  //
  //    /* Comparable */
  //    /** 
  //     * Compares two terms in <em>Prolog standard order of terms</em>.<br>
  //     * It is noted that <code>t1.compareTo(t2) == 0</code> has the same
  //     * <code>boolean</code> value as <code>t1.equals(t2)</code>.
  //     * @param anotherTerm the term to compared with. It must be dereferenced.
  //     * @return the value <code>0</code> if two terms are identical; 
  //     * a value less than <code>0</code> if this term is <em>before</em> the <code>anotherTerm</code>;
  //     * and a value greater than <code>0</code> if this term is <em>after</em> the <code>anotherTerm</code>.
  //     */
  //    public int compareTo(Object anotherTerm);//
}
