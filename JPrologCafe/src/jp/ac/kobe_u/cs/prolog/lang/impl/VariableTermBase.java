package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.InternalException;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.Trail;
import jp.ac.kobe_u.cs.prolog.lang.Undoable;
import jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation;

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
class VariableTermBase extends MachineTerm implements VariableTerm, VariableTermLocation {
  /**
   * 
   */
  private static final long serialVersionUID = -4728841990842289056L;
  /** Holds a term to which this variable is bound. Initial value is <code>this</code> (self-reference). */
  // private Object value;
  /** A CPF time stamp when this object is newly constructed. */
  private long timeStamp;

  final VariableTermLocation value;

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
  public VariableTermBase(Prolog engine, VariableTermLocation loc) {
    value = loc;
    this.timeStamp = engine.getCPFTimeStamp();
    this.machine = engine;
    this.setVal(this);
  }

  /**
   * Returns the value of <code>timeStamp</code>.
   * @see #timeStamp
   */
  public long timeStamp() {
    return this.timeStamp;
  }

  /** Returns a string representation of this object.*/
  @Override
  public String nameUQ() {
    return "_" + Integer.toHexString(System.identityHashCode(this)).toUpperCase();
  }

  /* (non-Javadoc)
   * @see java.lang.Object#hashCode()
   */
  //  @Override
  @Override
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
  @Override
  public boolean unify(Object t) {
    if (this.isBound()) {
      return StaticProlog.unify(this.getVal(), t);
    }
    t = StaticProlog.deref(t);
    StaticProlog.bind(this, t);
    return true;
  }

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
  public void setVal(Object v) {
    if (v == this) {
      value.setVal(v);
      if (value.isBound()) {
        //    System.out.println("odd...");
      }
    } else {
      value.setVal(v);
    }
  }

  public Object getVal() {
    return value.getVal();
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
  @Override
  public boolean convertible(Class type) {
    if (this.isBound()) return StaticProlog.convertible(this.getVal(), type);
    return StaticProlog.convertible(this.getClass(), type);
  }

  /**
   * Returns a copy of this object if unbound variable.
   * Otherwise, returns the value of <code>val.copy(engine)</code>.
   * @see #value
   */
  @Override
  public Object copy() {
    VariableTermBase co;
    if (this.isBound()) return StaticProlog.copy(this.getVal());
    co = (VariableTermBase) this.machine.copyHash.get(this);
    if (co == null) {
      //	    co = Prolog.makeVariable(engine);
      co = (VariableTermBase) StaticProlog.makeVariable(machine);
      co.timeStamp = Long.MIN_VALUE;
      //machine
      this.machine.copyHash.put(this, co);
    }
    return co;
  }

  @Override
  public Object deref() {
    if (!isBound()) return this;
    return StaticProlog.deref(this.getVal());//.deref();
  }

  @Override
  public boolean isGround() {
    if (this.isBound()) return StaticProlog.isGround(this.getVal());
    return false;
  }

  /**
   * Returns <code>this</code> if this variable is unbound.
   * Otherwise, returns a Java object that corresponds to the dereferenced term:
   * <code>val.toJava()</code>.
   * @return a Java object defined in <em>Prolog Cafe interoperability with Java</em>.
   * @see #value
   */
  @Override
  public Object toJava() {
    if (this.isBound()) return StaticProlog.toJava(this.getVal());//.toJava();
    return this;
  }

  /**
   * Returns a quoted string representation of this term if unbound.
   * Otherwise, returns the value of dereferenced term:
   * <code>val.toQuotedString()</code>
   * @see #value
   */
  @Override
  public String toQuotedString(int depth) {
    if (this.isBound()) return StaticProlog.toQuotedString(this.getVal());//.toQuotedString();
    return this.nameUQ();
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
  @Override
  public boolean equalJProlog(Object obj) {
    if (this.isBound()) return StaticProlog.prologEquals(this.getVal(), obj);
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
  @Override
  public String toStringImpl(int d) {
    if (this.isBound()) return StaticProlog.toString(this.getVal());
    return this.nameUQ();
  }

  /**
   * @return  val!=this;
   */
  @Override
  public boolean isBound() {
    // TODO Auto-generated method stub
    Object v = this.getVal();
    return v != null && v != this;
  }

  /* Undoable */
  public void unTrailSelf() {
    this.setVal(this);
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
    if (this.isBound()) return StaticProlog.compareTerm(this.getVal(), anotherTerm);
    if (!StaticProlog.isVariable(anotherTerm)) return Term.BEFORE;
    if (this == anotherTerm) return Term.EQUAL;
    final int x = this.hashCode(4) - StaticProlog.hashCode(anotherTerm, 4);
    if (x != 0) return x;
    throw new InternalException("VariableTerm is not unique");
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.VariableTerm#getMachine()
   */
  @Override
  public Prolog getMachine() {
    // TODO Auto-generated method stub
    return this.machine;
  }

  //  /* (non-Javadoc)
  //   * @see jp.ac.kobe_u.cs.prolog.lang.VariableTerm#getUndoable()
  //   */
  @Override
  public Undoable getUndoable() {
    final Object undoValue = getVal();
    // TODO Auto-generated method stub
    return new Undoable() {
      /* (non-Javadoc)
       * @see jp.ac.kobe_u.cs.prolog.lang.Undoable#unTrailSelf()
       */
      @Override
      public void unTrailSelf() {
        // TODO Auto-generated method stub
        setVal(undoValue);
      }
    };
  }

}
