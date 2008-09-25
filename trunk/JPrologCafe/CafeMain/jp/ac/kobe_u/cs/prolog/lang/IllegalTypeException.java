package jp.ac.kobe_u.cs.prolog.lang;

/**
 * Type error.<br>
 * There will be a type error when the type of an argument or 
 * one of its components is incorrect, but not a variable.
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public class IllegalTypeException extends BuiltinException {
  /** A functor symbol of <code>type_error/4</code>. */
  public static/*SymbolTerm*/Object TYPE_ERROR = StaticProlog.makeAtom("type_error", 4);

  /*
    type ::= atom | atomic | byte | callable | character | compound | evaluable |
             in_byte | in_character | integer | list | number |
       predicate_indicator | variable |
       flaot | java
  */
  /** Holds a string representation of valid type. */
  public String type;

  /** Holds the argument or one of its components which caused the error. */
  public Object culprit;

  /** Constructs a new <code>IllegalTypeException</code> 
   * with a valid type and its culprit. */
  public IllegalTypeException(String _type, Object _culprit) {
    type = _type;
    culprit = _culprit;
  }

  /** Constructs a new <code>IllegalTypeException</code> 
   * with the given arguments. */
  public IllegalTypeException(Predicate _goal, int _argNo, String _type, Object _culprit) {
    this.goal = _goal;
    this.argNo = _argNo;
    type = _type;
    culprit = _culprit;
  }

  /** Returns a term representation of this <code>IllegalTypeException</code>:
   * <code>type_error(goal,argNo,type,culprit)</code>.
   */
  public Object getMessageTerm() {
    Object[] args = { StaticProlog.makeJavaObject(goal), StaticProlog.makeInteger(argNo), StaticProlog.makeAtom(type), culprit };
    return StaticProlog.makeStructure(TYPE_ERROR, args);
  }

  /** Returns a string representation of this <code>IllegalTypeException</code>. */
  public String toString() {
    String s = "{TYPE ERROR: " + goal.toString();
    if (argNo > 0) s += " - arg " + argNo;
    s += ": expected " + type;
    s += ", found " + culprit.toString();
    s += "}";
    return s;
  }
}
