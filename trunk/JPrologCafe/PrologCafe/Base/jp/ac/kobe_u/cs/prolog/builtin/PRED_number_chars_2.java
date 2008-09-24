package jp.ac.kobe_u.cs.prolog.builtin;

import jp.ac.kobe_u.cs.prolog.lang.*;

/**
 * <code>number_chars/2</code><br>
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public class PRED_number_chars_2 extends PredicateBase {
  /**
  * 
  */
  private static final long serialVersionUID = 5932745211000415718L;
  static /*SymbolTerm*/Object Nil = makeAtom("[]");
  Object arg1, arg2;

  public PRED_number_chars_2(Object a1, Object a2, Predicate cont) {
    this.arg1 = a1;
    this.arg2 = a2;
    this.cont = cont;
  }

  public PRED_number_chars_2() {
  }

  @Override
  public void setArgument(Object[] args, Predicate cont) {
    this.arg1 = args[0];
    this.arg2 = args[1];
    this.cont = cont;
  }

  @Override
  public int arity() {
    return 2;
  }

  @Override
  public String toString() {
    return "number_chars(" + this.arg1 + "," + this.arg2 + ")";
  }

  @Override
  public Predicate exec(Prolog engine) {
    final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object a1, a2;
    a1 = this.arg1;
    a2 = this.arg2;

    a1 = deref(a1);
    a2 = deref(a2);//.deref();
    if (isNil(a2)) throw new SyntaxException(this, 2, "character_code_list", a2, "");
    if (isVariable(a1)) { // number_chars(-Number, +CharList)
      if (isVariable(a2)) {
        throw new PInstantiationException(this, 2);
      } else if (!isListTerm(a2)) {
        throw new IllegalTypeException(this, 2, "list", a2);
      }
      final StringBuffer sb = new StringBuffer();
      Object x = a2;
      while (!isNil(x)) {
        if (isVariable(x)) throw new PInstantiationException(this, 2);
        if (!isListTerm(x)) throw new IllegalTypeException(this, 2, "list", a2);
        final Object car = deref(first(x));//.first().deref();
        if (isVariable(car)) throw new PInstantiationException(this, 2);
        if (!isAtomTerm(car) || nameUQ(car).length() != 1) throw new IllegalTypeException(this, 2, "character", a2);
        sb.append((nameUQ(car)));
        x = deref(rest(x));
      }
      try {
        if (!unify(a1, makeInteger(Integer.parseInt(sb.toString())))) return this.fail(engine);
        return this.cont;
      } catch (final NumberFormatException e) {
      }
      try {
        if (!unify(a1, makeDouble(Double.parseDouble(sb.toString())))) return this.fail(engine);
        return this.cont;
      } catch (final NumberFormatException e) {
        throw new SyntaxException(this, 2, "character_code_list", a2, "");
      }
    } else if (isNumber(a1)) { // number_chars(+Number, ?CharList)
      final String s = a1.toString();
      Object y = PRED_number_chars_2.Nil;
      for (int i = s.length(); i > 0; i--) {
        y = makeList(makeAtom(s.substring(i - 1, i)), y);
      }
      if (!unify(a2, y)) return this.fail(engine);
      return this.cont;
    } else {
      throw new IllegalTypeException(this, 1, "number", a1);
    }
  }
}
