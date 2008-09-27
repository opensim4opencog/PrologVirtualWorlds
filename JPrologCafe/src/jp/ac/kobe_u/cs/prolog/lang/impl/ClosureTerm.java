package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.Predicate;

/**
 * Closure.<br>
 * The class <code>ClosureTerm</code> represents a closure structure.<br>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
interface ClosureTerm extends Term {
    /** Holds a <code>Predicate</code> object that represents a Prolog goal. */
    ;// final private Predicate code;

    /** Constructs a new closure structure for the given Prolog goal. */
    ;// public ClosureTerm(Predicate _code);// { code = _code; }

    /** Returns the Prolog goal of this <code>ClosureTerm</code>. */
    public Predicate getCode();// { return code; }

//    /* Object */
//    public boolean unify(Object t) ;//
//
//    public String toQuotedString(int depth);// { return toString(); }
//
//    /* Object */
//    /**
//     * Checks <em>term equality</em> of two terms.
//     * The result is <code>true</code> if and only if the argument is an instance of
//     * <code>ClosureTerm</code>, and
//     * the pairs of goals in the two closures are <em>equal</em>.
//     * @param obj the object to compare with. This must be dereferenced.
//     * @return <code>true</code> if the given object represents a closure
//     * equivalent to this <code>ClosureTerm</code>, false otherwise.
//     * @see #compareTo
//     */
//    public boolean equals(Object obj) ;//
//    public int hashCode() ;//
//    /** Returns a string representation of this <code>ClosureTerm</code>. */
//    public String toString();//
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
