package jp.ac.kobe_u.cs.prolog.lang;
/**
 * Builtin exception.<br>
 *
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public class BuiltinException extends PrologException {
    Object message;

    /** Holds the goal in which this exception occurs. */
    public Predicate goal = null;

    /** Holds the arity of goal in which this exception occurs. */
    public int argNo = 0;

    /** Constructs a new <code>BuiltinException</code>. */
    public BuiltinException(){}

    /** Constructs a new <code>BuiltinException</code> with a given message term. */
    public BuiltinException(Object _message){
	message = _message;
    }

    public Object getMessageTerm() {
	return message;
    }
}
