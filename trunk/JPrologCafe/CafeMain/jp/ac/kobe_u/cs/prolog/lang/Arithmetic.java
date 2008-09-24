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
  static/*SymbolTerm*/Object SYM_RANDOM = makeSymbol("random");
  static/*SymbolTerm*/Object SYM_PI = makeSymbol("pi");
  static/*SymbolTerm*/Object SYM_E = makeSymbol("e");
  static/*SymbolTerm*/Object SYM_ADD_1 = makeSymbol("+", 1);
  static/*SymbolTerm*/Object SYM_NEGATE_1 = makeSymbol("-", 1);
  static/*SymbolTerm*/Object SYM_ADD_2 = makeSymbol("+", 2);
  static/*SymbolTerm*/Object SYM_SUBTRACT_2 = makeSymbol("-", 2);
  static/*SymbolTerm*/Object SYM_MULTIPLY_2 = makeSymbol("*", 2);
  static/*SymbolTerm*/Object SYM_DIVIDE_2 = makeSymbol("/", 2);
  static/*SymbolTerm*/Object SYM_INTDIVIDE_2 = makeSymbol("//", 2);
  static/*SymbolTerm*/Object SYM_MOD_2 = makeSymbol("mod", 2);
  static/*SymbolTerm*/Object SYM_SHIFTLEFT_2 = makeSymbol("<<", 2);
  static/*SymbolTerm*/Object SYM_SHIFTRIGHT_2 = makeSymbol(">>", 2);
  static/*SymbolTerm*/Object SYM_NOT_1 = makeSymbol("\\", 1);
  static/*SymbolTerm*/Object SYM_AND_2 = makeSymbol("/\\", 2);
  static/*SymbolTerm*/Object SYM_OR_2 = makeSymbol("\\/", 2);
  static/*SymbolTerm*/Object SYM_XOR_2 = makeSymbol("#", 2);
  static/*SymbolTerm*/Object SYM_POW_2 = makeSymbol("**", 2);
  static/*SymbolTerm*/Object SYM_ABS_1 = makeSymbol("abs", 1);
  static/*SymbolTerm*/Object SYM_ACOS_1 = makeSymbol("acos", 1);
  static/*SymbolTerm*/Object SYM_ASIN_1 = makeSymbol("asin", 1);
  static/*SymbolTerm*/Object SYM_ATAN_1 = makeSymbol("atan", 1);
  static/*SymbolTerm*/Object SYM_CEIL_1 = makeSymbol("ceiling", 1);
  static/*SymbolTerm*/Object SYM_COS_1 = makeSymbol("cos", 1);
  static/*SymbolTerm*/Object SYM_DEGREES_1 = makeSymbol("degrees", 1);
  static/*SymbolTerm*/Object SYM_EXP_1 = makeSymbol("exp", 1);
  static/*SymbolTerm*/Object SYM_FLOOR_1 = makeSymbol("floor", 1);
  static/*SymbolTerm*/Object SYM_LOG_1 = makeSymbol("log", 1);
  static/*SymbolTerm*/Object SYM_MAX_2 = makeSymbol("max", 2);
  static/*SymbolTerm*/Object SYM_MIN_2 = makeSymbol("min", 2);
  static/*SymbolTerm*/Object SYM_RADIANS_1 = makeSymbol("radians", 1);
  static/*SymbolTerm*/Object SYM_RINT_1 = makeSymbol("rint", 1);
  static/*SymbolTerm*/Object SYM_ROUND_1 = makeSymbol("round", 1);
  static/*SymbolTerm*/Object SYM_SIN_1 = makeSymbol("sin", 1);
  static/*SymbolTerm*/Object SYM_SQRT_1 = makeSymbol("sqrt", 1);
  static/*SymbolTerm*/Object SYM_TAN_1 = makeSymbol("tan", 1);
  static/*SymbolTerm*/Object SYM_REM_2 = makeSymbol("rem", 2);
  static/*SymbolTerm*/Object SYM_SIGN_1 = makeSymbol("sign", 1);
  static/*SymbolTerm*/Object SYM_FLOAT_1 = makeSymbol("float", 1);
  static/*SymbolTerm*/Object SYM_INTPART_1 = makeSymbol("float_integer_part", 1);
  static/*SymbolTerm*/Object SYM_FRACTPART_1 = makeSymbol("float_fractional_part", 1);
  static/*SymbolTerm*/Object SYM_TRUNCATE_1 = makeSymbol("truncate", 1);

  /**
   * @param negate
   * @return
   */
  private static NumberTerm makeNumber(Number negate) {
    // TODO Auto-generated method stub
    //if (negate instanceof NumberTerm) return (NumberTerm) negate;
    boolean isInt = negate.intValue() == negate.hashCode();
    return isInt ? makeInteger(negate.longValue()) : makeDouble(negate.doubleValue());
  }

  /**
   * Evaluates <code>_t</code> as an arithmetic expression, 
   * and returns the resulting number as <code>NumberTerm</code>.
   *
   * @exception PInstantiationException if <code>_t</code> contains unbound variables.
   * @exception IllegalDomainException if <code>_t</code> is not an arithmetic expression.
   */
  public static NumberTerm evaluate(Object _t) throws PInstantiationException, IllegalDomainException {
    Object t = deref(_t);//.deref();

    if (isVariable(t))
      throw new PInstantiationException();
    else if (isInteger(t))
      return (NumberTerm) t;
    else if (isDouble(t))
      return (NumberTerm) t;
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
    else if (prologEquals(func, SYM_NEGATE_1))
      return makeNumber(evaluate(args[0]).negate());
    else if (prologEquals(func, SYM_ADD_2))
      return evaluate(args[0]).add(evaluate(args[1]));
    else if (prologEquals(func, SYM_SUBTRACT_2))
      return evaluate(args[0]).subtract(evaluate(args[1]));
    else if (prologEquals(func, SYM_MULTIPLY_2))
      return evaluate(args[0]).multiply(evaluate(args[1]));
    else if (prologEquals(func, SYM_INTDIVIDE_2))
      return evaluate(args[0]).intDivide(evaluate(args[1]));
    else if (prologEquals(func, SYM_DIVIDE_2))
      return evaluate(args[0]).divide(evaluate(args[1]));
    else if (prologEquals(func, SYM_MOD_2))
      return evaluate(args[0]).mod(evaluate(args[1]));
    else if (prologEquals(func, SYM_REM_2))
      return evaluate(args[0]).mod(evaluate(args[1]));
    else if (prologEquals(func, SYM_AND_2))
      return evaluate(args[0]).and(evaluate(args[1]));
    else if (prologEquals(func, SYM_OR_2))
      return evaluate(args[0]).or(evaluate(args[1]));
    else if (prologEquals(func, SYM_XOR_2))
      return evaluate(args[0]).xor(evaluate(args[1]));
    else if (prologEquals(func, SYM_NOT_1))
      return evaluate(args[0]).not();
    else if (prologEquals(func, SYM_SHIFTLEFT_2))
      return evaluate(args[0]).shiftLeft(evaluate(args[1]));
    else if (prologEquals(func, SYM_SHIFTRIGHT_2))
      return evaluate(args[0]).shiftRight(evaluate(args[1]));
    else if (prologEquals(func, SYM_ABS_1))
      return evaluate(args[0]).abs();
    else if (prologEquals(func, SYM_MIN_2))
      return evaluate(args[0]).min(evaluate(args[1]));
    else if (prologEquals(func, SYM_MAX_2))
      return evaluate(args[0]).max(evaluate(args[1]));
    else if (prologEquals(func, SYM_RINT_1))
      return evaluate(args[0]).rint();
    else if (prologEquals(func, SYM_ROUND_1))
      return evaluate(args[0]).round();
    else if (prologEquals(func, SYM_FLOOR_1))
      return evaluate(args[0]).floor();
    else if (prologEquals(func, SYM_CEIL_1))
      return evaluate(args[0]).ceil();
    else if (prologEquals(func, SYM_SIN_1))
      return evaluate(args[0]).sin();
    else if (prologEquals(func, SYM_COS_1))
      return evaluate(args[0]).cos();
    else if (prologEquals(func, SYM_TAN_1))
      return evaluate(args[0]).tan();
    else if (prologEquals(func, SYM_ASIN_1))
      return evaluate(args[0]).asin();
    else if (prologEquals(func, SYM_ACOS_1))
      return evaluate(args[0]).acos();
    else if (prologEquals(func, SYM_ATAN_1))
      return evaluate(args[0]).atan();
    else if (prologEquals(func, SYM_SQRT_1))
      return evaluate(args[0]).sqrt();
    else if (prologEquals(func, SYM_LOG_1))
      return evaluate(args[0]).log();
    else if (prologEquals(func, SYM_EXP_1))
      return evaluate(args[0]).exp();
    else if (prologEquals(func, SYM_POW_2))
      return evaluate(args[0]).pow(evaluate(args[1]));
    else if (prologEquals(func, SYM_DEGREES_1))
      return evaluate(args[0]).toDegrees();
    else if (prologEquals(func, SYM_RADIANS_1))
      return evaluate(args[0]).toRadians();
    else if (prologEquals(func, SYM_SIGN_1))
      return evaluate(args[0]).signum();
    else if (prologEquals(func, SYM_FLOAT_1))
      return evaluate(args[0]).toFloat();
    else if (prologEquals(func, SYM_INTPART_1))
      return evaluate(args[0]).floatIntPart();
    else if (prologEquals(func, SYM_FRACTPART_1))
      return evaluate(args[0]).floatFractPart();
    else if (prologEquals(func, SYM_TRUNCATE_1))
      return evaluate(args[0]).truncate();
    else
      throw new IllegalDomainException("arithmetic expression", t);
  }

}
