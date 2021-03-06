package jp.ac.kobe_u.cs.prolog.lang;


/**
 * The abstract class <code>JavaPredicate</code> contains methods
 * for interoperating with Java</em>.<br>
 * For example, the following builtin predicates extends this <code>JavaPredicate</code>.
 * <ul>
 * <li><code>java_constructor</code>
 * <li><code>java_method</code>.
 * </ul>
 * 
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public abstract class JavaPredicate extends PredicateBase {
  /**
   * Checks whether all terms in <code>args</code> are convertible
   * to the corresponding Java types in <code>paraTypes</code>.
   * @return <code>true</code>
   * if <code>args[i].convertible(paraTypes[i])</code> succeeds for all <code>i</code>,
   * otherwise <code>false</code>.
   * @see Object#convertible(Class)
   */
  static public boolean checkParameterTypes(Class[] paraTypes, Object[] args) {
    int arity;
    arity = paraTypes.length;
    if (arity != args.length) return false;
    for (int i = 0; i < arity; i++) {
      if (!convertible(args[i], paraTypes[i])) {
        return false;
      }
    }
    return true;
  }

  static final public Object toPrologTerm(Object obj) {
    /* flow_0_0_IfThenElseBlock_1037573833
    */
    if (instanceOfTerm(obj)) return (Object) obj;
    return makeJavaObject(obj);
  }
}
