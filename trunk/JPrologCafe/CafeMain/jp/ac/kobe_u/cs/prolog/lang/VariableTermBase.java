package jp.ac.kobe_u.cs.prolog.lang;

/**
 * Variable.<br>
 * The <code>VariableTerm</code> class represents a logical variable.<br>
 * For example,
 * <pre>
 *   Object t = Prolog.makeVariable();
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public class VariableTermBase extends MachineTerm implements VariableTerm, Undoable {
  /** Holds a term to which this variable is bound. Initial value is <code>this</code> (self-reference). */
  // private Object value;
  /** A CPF time stamp when this object is newly constructed. */
  private long timeStamp;

  public interface VariableTermLocation {

  }

  /* (non-Javadoc)
   * @see be.kuleuven.jProlog.PrologObject#isConst()
   */
  @Override
  public boolean isConst() {
    // TODO Auto-generated method stub
    return false;
  }

  // final Prolog machine;

  //  /** Constructs a new logical variable so that
  //   * the <code>timeStamp</code> field is set to <code>Long.MIN_VALUE</code>.
  //   */
  //  public VariableTermBase() {
  //    val = this;
  //    timeStamp = Long.MIN_VALUE;
  //  }

  /** Constructs a new logical variable so that
   * the <code>timeStamp</code> field is set to the current value of
   * <code>CPFTimeStamp</code> of the specified Prolog engine.
   * @param engine Current Prolog engine.
   * @see Prolog#getCPFTimeStamp
   */
  public VariableTermBase(Prolog engine) {
    timeStamp = engine.getCPFTimeStamp();
    machine = engine;
    setVal(this);
  }

  /** 
   * Returns the value of <code>timeStamp</code>.
   * @see #timeStamp
   */
  public long timeStamp() {
    return timeStamp;
  }

  /** Returns a string representation of this object.*/
  public String nameUQ() {
    return "_" + Integer.toHexString(System.identityHashCode(this)).toUpperCase();
  }

  /* (non-Javadoc)
   * @see java.lang.Object#hashCode()
   */
  //  @Override
  public int hashCode(int d) {
    // TODO Auto-generated method stub
    return System.identityHashCode(this);
  }

  /* Object */
  /** 
   * Checks whether the argument term is unified with this one.
   * If this is an unbound variable, the <code>unify</code> method binds this to 
   * the dereferenced value of argument term: <code>bind(t.deref(), trail)</code>,
   * and returns <code>true</code>.
   * Otherwise, it returns a <code>boolean</code> whose value is <code>val.unify(t, trail)</code>.
   * @param t the term to be unified with.
   * @return <code>true</code> if succeeds, otherwise <code>false</code>.
   * @see #value
   * @see #bind(Object)
   * @see Trail
   */
  public boolean unify(Object t) {
    if (isBound()) {
      return unify(getVal(), t);
    }
    t = deref(t);
    bind(this, t);
    return true;
  }

  public Object value;

  //  /* (non-Javadoc)
  //   * @see jp.ac.kobe_u.cs.prolog.lang.CafeTerm#prologEquals(java.lang.Object)
  //   */
  //  @Override
  //  public boolean prologEquals(Object s1) ;
  //
  //  /* (non-Javadoc)
  //   * @see jp.ac.kobe_u.cs.prolog.lang.CafeTerm#toStringImpl(int)
  //   */
  //  @Override
  //  public String toStringImpl(int depth) ;
  //
  //  /* (non-Javadoc)
  //   * @see jp.ac.kobe_u.cs.prolog.lang.CafeTerm#unify(jp.ac.kobe_u.cs.prolog.lang.Object)
  //   */
  //  @Override
  //  public boolean unify(Object t);
  //
  //  /* (non-Javadoc)
  //   * @see java.lang.Comparable#compareTo(java.lang.Object)
  //   */
  //  @Override
  //  abstract public int compareTo(Object o) ;
  public void setVal(Object value) {
    this.value = value;
  }

  public Object getVal() {
    return value;
  }

  //  public boolean bind(Object that) {
  //    if (this == that) {
  //      return true;
  //    }
  //    Prolog machine = this.machine;
  //    Trail trail = machine.trail;
  //    if (isVariable(that) && ((VariableTerm) that).timeStamp() >= this.timeStamp) {
  //      ((VariableTerm) that).setVal(this);
  //      if (((VariableTerm) that).timeStamp() < trail.engine.stack_getTimeStamp()) {
  //        trail.push((VariableTerm) that);
  //      }
  //    } else {
  //      this.setVal(that);
  //      if (this.timeStamp() < trail.engine.stack_getTimeStamp()) {
  //        trail.push(this);
  //      }
  //    }
  //    return true;
  //  }

  /** 
   * Checks whether this object is convertible with the given Java class type 
   * if this variable is unbound.
   * Otherwise, returns the value of <code>val.convertible(type)</code>.
   * @param type the Java class type to compare with.
   * @return <code>true</code> if this (or dereferenced term) is 
   * convertible with <code>type</code>. Otherwise <code>false</code>.
   * @see #value
   */
  public boolean convertible(Class type) {
    if (isBound()) return convertible(getVal(), type);
    return convertible(this.getClass(), type);
  }

  /** 
   * Returns a copy of this object if unbound variable.
   * Otherwise, returns the value of <code>val.copy(engine)</code>.
   * @see #value
   */
  public Object copy(Prolog engine) {
    VariableTermBase co;
    if (engine != machine) {
      Thread.dumpStack();
    }
    if (isBound()) return copy(getVal(), engine);
    co = (VariableTermBase) engine.copyHash.get(this);
    if (co == null) {
      //	    co = Prolog.makeVariable(engine);
      co = new VariableTermBase(machine);
      co.timeStamp = Long.MIN_VALUE;
      //machine
      engine.copyHash.put(this, co);
    }
    return co;
  }

  public Object deref() {
    if (getVal() == this) return this;
    return deref(getVal());//.deref();
  }

  public boolean isGround() {
    if (isBound()) return isGround(getVal());
    return false;
  }

  /** 
   * Returns <code>this</code> if this variable is unbound.
   * Otherwise, returns a Java object that corresponds to the dereferenced term:
   * <code>val.toJava()</code>.
   * @return a Java object defined in <em>Prolog Cafe interoperability with Java</em>.
   * @see #value
   */
  public Object toJava() {
    if (isBound()) return toJava(getVal());//.toJava();
    return this;
  }

  /**
   * Returns a quoted string representation of this term if unbound.
   * Otherwise, returns the value of dereferenced term:
   * <code>val.toQuotedString()</code>
   * @see #value
   */
  public String toQuotedString() {
    if (isBound()) return toQuotedString(getVal());//.toQuotedString();
    return nameUQ();
  }

  /* Object */
  /**
   * Checks <em>term equality</em> of two terms.
   * This method returns a <code>boolean</code> whose value is
   * (<code>this == obj</code>) if this variable is unbound.
   * Otherwise, it returns the value of <code>val.equals(obj)</code>.
   * @param obj the object to compare with. This must be dereferenced.
   * @return <code>true</code> if this (or dereferenced term) is the same as the argument;
   * <code>false</code> otherwise.
   * @see #value
   * @see #compareTo
  */
  public boolean equalJProlog(Object obj) {
    if (isBound()) return prologEquals(getVal(), obj);
    if (!(obj instanceof VariableTerm)) // ???
      return false; //???
    return this == obj;
  }

  /**
   * Returns a string representation of this term if unbound.
   * Otherwise, returns the value of dereferenced term:
   * <code>val.toString()</code>
   * @see #value
   */
  public String toStringImpl(int d) {
    if (isBound()) return toString(getVal());
    return nameUQ();
  }

  /**
   * @return  val!=this;
   */
  @Override
  public boolean isBound() {
    // TODO Auto-generated method stub
    return getVal() != this;
  }

  /* Undoable */
  public void unTrailSelf() {
    setVal(this);
  }

  /* Comparable */
  /** 
   * Compares two terms in <em>Prolog standard order of terms</em>.<br>
   * It is noted that <code>t1.compareTo(t2) == 0</code> has the same
   * <code>boolean</code> value as <code>t1.equals(t2)</code>.
   * @param anotherTerm the term to compare with. It must be dereferenced.
   * @return the value <code>0</code> if two terms are identical; 
   * a value less than <code>0</code> if this term is <em>before</em> the <code>anotherTerm</code>;
   * and a value greater than <code>0</code> if this term is <em>after</em> the <code>anotherTerm</code>.
   */
  public int compareTo(Object anotherTerm) { // anotherTerm must be dereferenced.
    if (isBound()) return compareTerm(getVal(), anotherTerm);
    if (!isVariable(anotherTerm)) return BEFORE;
    if (this == anotherTerm) return EQUAL;
    int x = this.hashCode(4) - hashCode(anotherTerm, 4);
    if (x != 0) return x;
    throw new InternalException("VariableTerm is not unique");
  }


}
