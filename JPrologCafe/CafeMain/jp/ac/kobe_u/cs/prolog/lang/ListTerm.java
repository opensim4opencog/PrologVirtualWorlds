package jp.ac.kobe_u.cs.prolog.lang;

/**
 * List.<br>
 * The class <code>ListTerm</code> represents a list structure.<br>
 * 
 * <pre>
 *  % [1,2]
 *  Object Nil = Prolog.makeSymbol("[]");
 *  Object  n1 = IntegerTerm(1);
 *  Object  n2 = IntegerTerm(2);
 *  Object   t = Prolog.makeList(n1, Prolog.makeList(n2, Nil));
 *  
 *  Object car = ((ListTerm)t).car();
 *  Object cdr = ((ListTerm)t).cdr();
 * </pre>
 *
 * Here is sample program for creating a list from <code>1</code> to <code>n</code>.
 * <pre>
 * public static Object makeList(int n) {
 *   Object t = Prolog.makeSymbol("[]");
 *   for (int i=n; i>0; i--) {
 *     t = Prolog.makeList(Prolog.makeInteger(i), t);
 *   }
 *   return t;
 * }
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public interface ListTerm extends Term {
  //
  //    /** A functor <code>'.' /2</code>. */
  public static /*SymbolTerm*/Object SYM_DOT = Prolog.makeSymbol(".", 2);

  //
  //    /** Holds the first element of this <code>ListTerm</code>. */
  // //   private Object           car;
  //
  //    /**
  //     * Holds the list consisting of all the rest of the elements of 
  //     * this <code>ListTerm</code> but the first one.
  //     */
  // //   private Object           cdr;
  //
  ////    /**
  ////     * Constructs a new Prolog list structure
  ////     * such that <code>_car</code> is the first element of this list, and 
  ////     * <code>_cdr</code> is the list consisting of all the rest of the 
  ////     * elements of this list but the first one.
  ////     */
  ////    public ListTerm(Object _car, Object _cdr) {
  ////        car = _car;
  ////        cdr = _cdr;
  ////    }

  /** Returns the value of <code>car</code>.
   * @see #car
   */
  public Object first();

  /** Returns the value of <code>cdr</code>.
   * @see #cdr
   */
  public Object rest();

  /** Sets the value to <code>car</code>.
   * @see #car
   */
  public void setCar(Object t);

  /** Sets the value to <code>cdr</code>.
   * @see #cdr
   */
  public void setCdr(Object t);

  //  //    /* Object */
  //  //    public boolean unify(Object t);
  //  //
  //  //    /** 
  //  //     * @return the <code>boolean</code> whose value is
  //  //     * <code>convertible(Vector.class, type)</code>.
  //  //     * @see Object#convertible(Class, Class)
  //  //     */
  //  //    public boolean convertible(Class type);
  //  //
  //  //    public Object copy(Prolog engine);
  //  //
  //  //    public boolean isGround();
  //  //
  /** Returns the length of this <code>ListTerm</code>. */
  public int listLength();
  //
  //    /** 
  //     * Returns a <code>java.util.Vector</code> corresponds to this <code>ListTerm</code>
  //     * according to <em>Prolog Cafe interoperability with Java</em>.
  //     * @return a <code>java.util.Vector</code> object equivalent to
  //     * this <code>IntegerTerm</code>.
  //     */
  //    public Object toJava() ;
  //    public String toQuotedString();
  //    /* Object */
  //    /**
  //     * Checks <em>term equality</em> of two terms.
  //     * The result is <code>true</code> if and only if the argument is an instance of
  //     * <code>ListTerm</code>, and 
  //     * all corresponding pairs of elements in the two lists are <em>term-equal</em>.
  //     * @param obj the object to compare with. This must be dereferenced.
  //     * @return <code>true</code> if the given object represents a Prolog list
  //     * equivalent to this <code>ListTerm</code>, false otherwise.
  //     * @see #compareTo
  //     */
  //    public boolean equals(Object obj);
  //
  //    public int hashCode();
  //
  //    /** Returns a string representation of this <code>ListTerm</code>. */
  //    public String toString() ;
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
  //    public int compareTo(Object anotherTerm) ;
}
