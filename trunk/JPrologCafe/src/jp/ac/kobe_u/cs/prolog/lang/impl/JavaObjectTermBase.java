package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.*;


/**
 * Java-term.<br>
 * The <code>JavaObjectTerm</code> class wraps a java object.<br>
 * 
 * <pre>
 *  import java.util.Hashtable;
 *  Object t = Prolog.makeJavaObject(new Hashtable());
 *  Hashtable hash = (Hashtable)(((JavaObjectTerm)t).object());
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
class JavaObjectTermBase extends TermBase implements JavaObjectTerm {
  /** Holds a java object that this <code>JavaObjectTerm</code> wraps. */
  private Object value = null;
  /** Holds a <code>java.lang.Class</code> of object wrapped by this <code>JavaObjectTerm</code>. */
  private Class clazz = null;

  /** Constructs a new Prolog java-term that wraps the argument object. */
  public JavaObjectTermBase(Object _obj) {
    // super(_obj);
    value = _obj;
    if (_obj != null) setObject(_obj);
  }

  /** Sets the argument object to this <code>JavaObjectTerm</code>. */
  public void setObject(Object _obj) {
    value = _obj;
    clazz = _obj.getClass();
  }

  /* (non-Javadoc)
   * @see be.kuleuven.jProlog.PrologObject#isConst()
   */
  @Override
  public boolean isConst() {
    // TODO Auto-generated method stub
    return true;
  }

  /** Returns the object wrapped by this <code>JavaObjectTerm</code>. */
  public Object toJava() {
    return value;
  }

  /** Returns a <code>java.lang.Class</code> of object wrapped by this <code>JavaObjectTerm</code>. */
  public Class getClazz() {
    return clazz;
  }

  public String toQuotedString() {
    return toString();
  }

  /* Object */
  public boolean unify(Object t) {
    if (isVariable(t)) return unify(t, this);
    if (!isJavaObject(t)) return false;
    return value.equals(toJava(t));
  }

  /** 
   * Check whether the wrapped object is convertible with the given Java class type.
   * @return the <code>boolean</code> whose value is
   * <code>convertible(clazz, type)</code>.
   * @see #clazz
   * @see Object#convertible(Class, Class)
   */
  public boolean convertible(Class type) {
    return convertible(clazz, type);
  }

  /** 
   * Returns the object wrapped by this <code>JavaObjectTerm</code>.
   * @return the value of <code>obj</code>.
   * @see #value
   */
  //    public Object toJava() { return value; }
  /* Object */
  /**
   * Checks <em>term equality</em> of two terms.
   * The result is <code>true</code> if and only if the argument is an instance of
   * <code>JavaObjectTerm</code>, and 
   * the pairs of wrapped objects in the two java-term are <em>equal</em>
   * by <code>obj.equals(((JavaObjectTerm)o).obj)</code>.
   * @param o the object to compare with. This must be dereferenced.
   * @return <code>true</code> if the given object represents a java-term
   * equivalent to this <code>JavaObjectTerm</code>, false otherwise.
   * @see #compareTo
   */
  public boolean equalJProlog(Object o) {
    if (!(o instanceof JavaObjectTerm)) return false;
    return value.equals(toJava(o));
  }

  public int hashCode(int d) {
    return value.hashCode();
  }

  /** Returns a string representation of this <code>JavaObjectTerm</code>. */
  public String toStringImpl(int d) {
    return clazz.getName() + "(" + hashCode(d) + ")";
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
    if (isVariable(anotherTerm) || isNumber(anotherTerm) || isAtomTerm(anotherTerm) || isListTerm(anotherTerm) || isCompound(anotherTerm)) return AFTER;
    if (!isJavaObject(anotherTerm)) return BEFORE;
    Object otherObj = toJava(anotherTerm);
    if (value.equals(otherObj)) return EQUAL;
    return value.hashCode() - otherObj.hashCode(); //???
  }

}
