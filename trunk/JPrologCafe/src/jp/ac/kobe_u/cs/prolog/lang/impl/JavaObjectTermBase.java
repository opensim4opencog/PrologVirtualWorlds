package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;

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
abstract class JavaObjectTermBase extends TermBase implements JavaObjectTerm {
  /**
   * 
   */
  private static final long serialVersionUID = -7741865217868762397L;
  /** Holds a java object that this <code>JavaObjectTerm</code> wraps. */
  private Object value = null;
  /** Holds a <code>java.lang.Class</code> of object wrapped by this <code>JavaObjectTerm</code>. */
  private Class clazz = null;

  /** Constructs a new Prolog java-term that wraps the argument object. */
  public JavaObjectTermBase(Object _obj) {
    // super(_obj);
    this.value = _obj;
    if (_obj != null) this.setObject(_obj);
  }

  /** Sets the argument object to this <code>JavaObjectTerm</code>. */
  public void setObject(Object _obj) {
    this.value = _obj;
    this.clazz = _obj.getClass();
  }

  /** Returns the object wrapped by this <code>JavaObjectTerm</code>. */
  @Override
  public Object toJava() {
    return this.value;
  }

  /** Returns a <code>java.lang.Class</code> of object wrapped by this <code>JavaObjectTerm</code>. */
  public Class getClazz() {
    return this.clazz;
  }

  @Override
  public String toQuotedString(int depth) {
    return this.toString();
  }

  /* Object */
  @Override
  public boolean unify(Object t) {
    if (StaticProlog.isVariable(t)) return StaticProlog.unify(t, this);
    if (!StaticProlog.isJavaObject(t)) return false;
    return this.value.equals(StaticProlog.toJava(t));
  }

  /**
   * Check whether the wrapped object is convertible with the given Java class type.
   * @return the <code>boolean</code> whose value is
   * <code>convertible(clazz, type)</code>.
   * @see #clazz
   * @see Object#convertible(Class, Class)
   */
  @Override
  public boolean convertible(Class type) {
    return StaticProlog.convertible(this.clazz, type);
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
  @Override
  public boolean equalJProlog(Object o) {
    if (!(o instanceof JavaObjectTerm)) return false;
    return this.value.equals(StaticProlog.toJava(o));
  }

  @Override
  public int hashCode(int d) {
    return this.value.hashCode();
  }

  /** Returns a string representation of this <code>JavaObjectTerm</code>. */
  @Override
  public String toStringImpl(int d) {
    return this.clazz.getName() + "(" + this.hashCode(d) + ")";
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
    if (StaticProlog.isVariable(anotherTerm) || StaticProlog.isNumber(anotherTerm) || StaticProlog.isAtomTerm(anotherTerm) || StaticProlog.isListTerm(anotherTerm) || StaticProlog.isCompound(anotherTerm)) return Term.AFTER;
    if (!StaticProlog.isJavaObject(anotherTerm)) return Term.BEFORE;
    final Object otherObj = StaticProlog.toJava(anotherTerm);
    if (this.value.equals(otherObj)) return Term.EQUAL;
    return this.value.hashCode() - otherObj.hashCode(); //???
  }

}
