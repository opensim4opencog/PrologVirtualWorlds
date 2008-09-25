package jp.ac.kobe_u.cs.prolog.lang;

/**
 * The <code>Arithmetic</code> class contains a method
 * for evaluating arithmetic expressions.<br>
 * This class is mainly used by the builtin predicate <code>is/2</code>.
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public class Arithmetic extends StaticProlog {
  static/*SymbolTerm*/Object SYM_RANDOM = makeAtom("random");
  static/*SymbolTerm*/Object SYM_PI = makeAtom("pi");
  static/*SymbolTerm*/Object SYM_E = makeAtom("e");
  static/*SymbolTerm*/Object SYM_ADD_1 = makeAtom("+", 1);
  static/*SymbolTerm*/Object SYM_NEGATE_1 = makeAtom("-", 1);
  static/*SymbolTerm*/Object SYM_ADD_2 = makeAtom("+", 2);
  static/*SymbolTerm*/Object SYM_SUBTRACT_2 = makeAtom("-", 2);
  static/*SymbolTerm*/Object SYM_MULTIPLY_2 = makeAtom("*", 2);
  static/*SymbolTerm*/Object SYM_DIVIDE_2 = makeAtom("/", 2);
  static/*SymbolTerm*/Object SYM_INTDIVIDE_2 = makeAtom("//", 2);
  static/*SymbolTerm*/Object SYM_MOD_2 = makeAtom("mod", 2);
  static/*SymbolTerm*/Object SYM_SHIFTLEFT_2 = makeAtom("<<", 2);
  static/*SymbolTerm*/Object SYM_SHIFTRIGHT_2 = makeAtom(">>", 2);
  static/*SymbolTerm*/Object SYM_NOT_1 = makeAtom("\\", 1);
  static/*SymbolTerm*/Object SYM_AND_2 = makeAtom("/\\", 2);
  static/*SymbolTerm*/Object SYM_OR_2 = makeAtom("\\/", 2);
  static/*SymbolTerm*/Object SYM_XOR_2 = makeAtom("#", 2);
  static/*SymbolTerm*/Object SYM_POW_2 = makeAtom("**", 2);
  static/*SymbolTerm*/Object SYM_ABS_1 = makeAtom("abs", 1);
  static/*SymbolTerm*/Object SYM_ACOS_1 = makeAtom("acos", 1);
  static/*SymbolTerm*/Object SYM_ASIN_1 = makeAtom("asin", 1);
  static/*SymbolTerm*/Object SYM_ATAN_1 = makeAtom("atan", 1);
  static/*SymbolTerm*/Object SYM_CEIL_1 = makeAtom("ceiling", 1);
  static/*SymbolTerm*/Object SYM_COS_1 = makeAtom("cos", 1);
  static/*SymbolTerm*/Object SYM_DEGREES_1 = makeAtom("degrees", 1);
  static/*SymbolTerm*/Object SYM_EXP_1 = makeAtom("exp", 1);
  static/*SymbolTerm*/Object SYM_FLOOR_1 = makeAtom("floor", 1);
  static/*SymbolTerm*/Object SYM_LOG_1 = makeAtom("log", 1);
  static/*SymbolTerm*/Object SYM_MAX_2 = makeAtom("max", 2);
  static/*SymbolTerm*/Object SYM_MIN_2 = makeAtom("min", 2);
  static/*SymbolTerm*/Object SYM_RADIANS_1 = makeAtom("radians", 1);
  static/*SymbolTerm*/Object SYM_RINT_1 = makeAtom("rint", 1);
  static/*SymbolTerm*/Object SYM_ROUND_1 = makeAtom("round", 1);
  static/*SymbolTerm*/Object SYM_SIN_1 = makeAtom("sin", 1);
  static/*SymbolTerm*/Object SYM_SQRT_1 = makeAtom("sqrt", 1);
  static/*SymbolTerm*/Object SYM_TAN_1 = makeAtom("tan", 1);
  static/*SymbolTerm*/Object SYM_REM_2 = makeAtom("rem", 2);
  static/*SymbolTerm*/Object SYM_SIGN_1 = makeAtom("sign", 1);
  static/*SymbolTerm*/Object SYM_FLOAT_1 = makeAtom("float", 1);
  static/*SymbolTerm*/Object SYM_INTPART_1 = makeAtom("float_integer_part", 1);
  static/*SymbolTerm*/Object SYM_FRACTPART_1 = makeAtom("float_fractional_part", 1);
  static/*SymbolTerm*/Object SYM_TRUNCATE_1 = makeAtom("truncate", 1);

  /**
   * @param negate
   * @return
   */
  private static Number makeNumber(Number negate) {
    // TODO Auto-generated method stub
    //if (negate instanceof Number) return (Number) negate;
    boolean isInt = negate.intValue() == negate.hashCode();
    return isInt ? makeInteger(negate.longValue()) : makeDouble(negate.doubleValue());
  }

  /**
   * Evaluates <code>_t</code> as an arithmetic expression, 
   * and returns the resulting number as <code>Number</code>.
   *
   * @exception PInstantiationException if <code>_t</code> contains unbound variables.
   * @exception IllegalDomainException if <code>_t</code> is not an arithmetic expression.
   */
  public static Number evaluate(Object _t) throws PInstantiationException, IllegalDomainException {
    Object t = deref(_t);//.deref();

    if (isVariable(t))
      throw new PInstantiationException();
    else if (isInteger(t))
      return (Number) t;
    else if (isDouble(t))
      return (Number) t;
    else if (prologEquals(t, SYM_RANDOM))
      return makeDouble(Math.random());
    else if (prologEquals(t, SYM_PI))
      return makeDouble(Math.PI);
    else if (prologEquals(t, SYM_E))
      return makeDouble(Math.E);
    else if (isListTerm(t))
      return evaluate(first((t)));
    else if (!isCompound(t)) throw new IllegalDomainException("arithmetic expression", t);

    /*SymbolTerm*/Object func = functor(t);
    Object[] args = args(t);

    if (prologEquals(func, SYM_ADD_1))
      return evaluate(args[0]);
    else if (prologEquals(func, SYM_NEGATE_1)) {
      Number part = evaluate(args[0]);
      if (StaticProlog.isInteger(part)) return StaticProlog.makeInteger(-part.longValue());
      return StaticProlog.makeDouble(-part.doubleValue());
    } else if (prologEquals(func, SYM_ADD_2)) {
      Number t1 = evaluate(args[1]);
      Number r = evaluate(args[0]);
      if (StaticProlog.isDouble(r) || StaticProlog.isDouble(t1)) {
        return StaticProlog.makeDouble(((Number) r).doubleValue() + t1.doubleValue());
      }
      return StaticProlog.makeInteger(((Number) r).longValue() + t1.longValue());
    } else if (prologEquals(func, SYM_SUBTRACT_2)) {
      Number t1 = evaluate(args[1]);
      Number r = evaluate(args[0]);
      if (StaticProlog.isDouble(r) || StaticProlog.isDouble(t1)) return StaticProlog.makeDouble(((Number) r).doubleValue() - t1.doubleValue());
      return StaticProlog.makeInteger((long) (((Number) r).doubleValue() - t1.doubleValue()));
    } else if (prologEquals(func, SYM_MULTIPLY_2)) {
      Number t1 = evaluate(args[1]);
      Number r = evaluate(args[0]);
      if (StaticProlog.isDouble(r) || StaticProlog.isDouble(t1)) return StaticProlog.makeDouble(((Number) r).doubleValue() * t1.doubleValue());
      return StaticProlog.makeInteger(((Number) r).longValue() * t1.longValue());
    } else if (prologEquals(func, SYM_INTDIVIDE_2)) {
      Number t1 = evaluate(args[1]);
      Number r = evaluate(args[0]);
      if (StaticProlog.isDouble(r) || StaticProlog.isDouble(t1)) throw new IllegalTypeException("integer", t1);
      if (t1.longValue() == 0) throw new EvaluationException("zero_divisor");
      return StaticProlog.makeInteger((((Number) r).longValue() / t1.longValue()));
    } else if (prologEquals(func, SYM_DIVIDE_2)) {
      Number t1 = evaluate(args[1]);
      if (t1.doubleValue() == 0) throw new EvaluationException("zero_divisor");
      return StaticProlog.makeDouble(((Number) evaluate(args[0])).doubleValue() / t1.doubleValue());
    } else if (prologEquals(func, SYM_MOD_2)) {
      Number t1 = evaluate(args[1]);
      //if (isDouble(t)) throw new IllegalTypeException("integer", t);
      if (t1.longValue() == 0) throw new EvaluationException("zero_divisor");
      return StaticProlog.makeInteger(((Number) evaluate(args[0])).longValue() % t1.longValue());
    } else if (prologEquals(func, SYM_REM_2)) {
      Number t1 = evaluate(args[1]);
      //if (isDouble(t)) throw new IllegalTypeException("integer", t);
      if (t1.longValue() == 0) throw new EvaluationException("zero_divisor");
      return StaticProlog.makeInteger(evaluate(args[0]).longValue() % t1.longValue());
    } else if (prologEquals(func, SYM_AND_2)) {
      Number t1 = evaluate(args[1]);
      Number r = evaluate(args[0]);
      if (StaticProlog.isDouble(r) || StaticProlog.isDouble(t1)) throw new IllegalTypeException("integer", t1);
      return StaticProlog.makeInteger(r.longValue() & t1.longValue());
    } else if (prologEquals(func, SYM_OR_2)) {
      Number t1 = evaluate(args[1]);
      Number r = evaluate(args[0]);
      if (StaticProlog.isDouble(r) || StaticProlog.isDouble(t1)) throw new IllegalTypeException("integer", t1);
      return StaticProlog.makeInteger(r.longValue() | t1.longValue());
    } else if (prologEquals(func, SYM_XOR_2)) {
      Number t1 = evaluate(args[1]);
      Number r = evaluate(args[0]);
      if (StaticProlog.isDouble(r) || StaticProlog.isDouble(t1)) throw new IllegalTypeException("integer", t1);
      return StaticProlog.makeInteger(r.longValue() ^ t1.longValue());
    } else if (prologEquals(func, SYM_NOT_1))
      return StaticProlog.makeInteger(~evaluate(args[0]).longValue());
    else if (prologEquals(func, SYM_SHIFTLEFT_2)) {
      Number t1 = evaluate(args[1]);
      Number r2 = evaluate(args[0]);
      if (StaticProlog.isDouble(r2) || StaticProlog.isDouble(t1)) throw new IllegalTypeException("integer", t1);
      return StaticProlog.makeInteger(r2.longValue() << t1.longValue());
    } else if (prologEquals(func, SYM_SHIFTRIGHT_2)) {
      Number t3 = evaluate(args[1]);
      Number r5 = evaluate(args[0]);
      if (StaticProlog.isDouble(r5) || StaticProlog.isDouble(t3)) throw new IllegalTypeException("integer", t3);
      return StaticProlog.makeInteger(r5.longValue() >> t3.longValue());
    } else if (prologEquals(func, SYM_ABS_1)) {
      Number r = evaluate(args[0]);
      if (StaticProlog.isInteger(r)) return StaticProlog.makeInteger(Math.abs(r.longValue()));
      return StaticProlog.makeDouble(Math.abs(r.doubleValue()));
    } else if (prologEquals(func, SYM_MIN_2)) {
      Number t2 = evaluate(args[1]);
      Number r4 = evaluate(args[0]);
      if (StaticProlog.isDouble(r4) || StaticProlog.isDouble(t2)) return StaticProlog.makeDouble(Math.min(r4.doubleValue(), t2.doubleValue()));
      return StaticProlog.makeInteger(Math.min(r4.longValue(), t2.longValue()));
    } else if (prologEquals(func, SYM_MAX_2)) {
      Number t1 = evaluate(args[1]);
      Number r3 = evaluate(args[0]);
      if (StaticProlog.isDouble(r3) || StaticProlog.isDouble(t1)) return StaticProlog.makeDouble(Math.max(r3.doubleValue(), t1.doubleValue()));
      return StaticProlog.makeInteger(Math.max(r3.longValue(), t1.longValue()));
    } else if (prologEquals(func, SYM_RINT_1))
      return StaticProlog.makeDouble(Math.rint(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_ROUND_1)) {
      Number r2 = evaluate(args[0]);
      if (StaticProlog.isInteger(r2)) return r2;
      return StaticProlog.makeInteger(Math.round(r2.doubleValue()));
    } else if (prologEquals(func, SYM_FLOOR_1))
      return StaticProlog.makeInteger((int) Math.floor(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_CEIL_1)) {
      Number r = evaluate(args[0]);
      if (StaticProlog.isInteger(r)) return r;
      return StaticProlog.makeInteger((int) Math.ceil(r.doubleValue()));
    } else if (prologEquals(func, SYM_SIN_1))
      return StaticProlog.makeDouble(Math.sin(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_COS_1))
      return StaticProlog.makeDouble(Math.cos(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_TAN_1))
      return StaticProlog.makeDouble(Math.tan(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_ASIN_1))
      return StaticProlog.makeDouble(Math.asin(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_ACOS_1))
      return StaticProlog.makeDouble(Math.acos(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_ATAN_1))
      return StaticProlog.makeDouble(Math.atan(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_SQRT_1)) {
      Number r3 = evaluate(args[0]);
      if (r3.doubleValue() < 0) throw new EvaluationException("undefined");
      return StaticProlog.makeDouble(Math.sqrt(r3.doubleValue()));
    } else if (prologEquals(func, SYM_LOG_1)) {
      Number r2 = evaluate(args[0]);
      if (r2.doubleValue() == 0) throw new EvaluationException("undefined");
      return StaticProlog.makeDouble(Math.log(r2.doubleValue()));
    } else if (prologEquals(func, SYM_EXP_1))
      return StaticProlog.makeDouble(Math.exp(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_POW_2))
      return StaticProlog.makeDouble(Math.pow(evaluate(args[0]).doubleValue(), evaluate(args[1]).doubleValue()));
    else if (prologEquals(func, SYM_DEGREES_1))
      return StaticProlog.makeDouble(Math.toDegrees(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_RADIANS_1))
      return StaticProlog.makeDouble(Math.toRadians(evaluate(args[0]).doubleValue()));
    else if (prologEquals(func, SYM_SIGN_1)) {
      Number r2 = evaluate(args[0]);
      if (StaticProlog.isDouble(r2)) return StaticProlog.makeDouble(Math.signum(r2.doubleValue()));
      return StaticProlog.makeInteger((int) Math.signum(r2.doubleValue()));
    } else if (prologEquals(func, SYM_FLOAT_1)) {
      Number r2 = evaluate(args[0]);
      if (StaticProlog.isDouble(r2)) return r2;//((Number)this);
      return StaticProlog.makeDouble(r2.doubleValue());
    } else if (prologEquals(func, SYM_INTPART_1)) {
      Number r1 = evaluate(args[0]);
      return StaticProlog.makeDouble(Math.signum(r1.doubleValue()) * Math.floor(Math.abs(r1.doubleValue())));
    } else if (prologEquals(func, SYM_FRACTPART_1)) {
      Number r = evaluate(args[0]);
      return StaticProlog.makeDouble(r.doubleValue() - Math.signum(r.doubleValue()) * Math.floor(Math.abs(r.doubleValue())));
    } else if (prologEquals(func, SYM_TRUNCATE_1)) {
      Number r = evaluate(args[0]);
      if (StaticProlog.isInteger(r)) return r;
      if (r.doubleValue() >= 0)
        return StaticProlog.makeInteger((int) Math.floor(r.doubleValue()));
      else
        return StaticProlog.makeInteger((int) (-1 * Math.floor(Math.abs(r.doubleValue()))));
    } else
      throw new IllegalDomainException("arithmetic expression", t);
  }

}
