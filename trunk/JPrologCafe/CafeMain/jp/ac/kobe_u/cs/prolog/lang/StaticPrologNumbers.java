/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang;


/**
 * @author root
 *
 */
public class StaticPrologNumbers {

  final public static Object abs(Number evaluate) {
    // TODO Auto-generated method stub
    if (StaticProlog.isInteger(evaluate)) return StaticProlog.makeInteger(Math.abs(evaluate.longValue()));
    return StaticProlog.makeDouble(Math.abs(evaluate.doubleValue()));//si2);
  }

  final public static Object acos(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.acos(((Number) evaluate).doubleValue()));//si2);
  }

  final public static Object add(Number evaluate, Object si2) {
    // TODO Auto-generated method stub
    Number t = (Number) si2;
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(t)) {
      return StaticProlog.makeDouble(((Number) evaluate).doubleValue() + t.doubleValue());
    }
    return StaticProlog.makeInteger(((Number) evaluate).longValue() + t.longValue());
  }

  final public static Object and(Number evaluate, Number si2) {
    // TODO Auto-generated method stub
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(si2)) throw new IllegalTypeException("integer", si2);
    return StaticProlog.makeInteger(((Number) evaluate).longValue() & si2.longValue());
  }

  final public static int arithCompareTo(Object evaluate, Object si2) {
    Number r = (Number) evaluate;
    final double v = ((Number) si2).doubleValue();
    return Double.compare(r.doubleValue(), v);
  }

  final public static Object asin(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.asin(((Number) evaluate).doubleValue()));//si2);
  }

  final public static Object atan(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.atan(((Number) evaluate).doubleValue()));//si2);
  }

  final public static Object ceil(Number evaluate) {
    // TODO Auto-generated method stub
    if (StaticProlog.isInteger(evaluate)) return evaluate;
    return StaticProlog.makeInteger((int) Math.ceil(((Number) evaluate).doubleValue()));//si2);
  }

  /* (Object thiz,non-Javadoc)
   * @see java.lang.Object#clone(Object thiz)
   */
  //  @Override
  //   final public static  Object clone(Object thiz) throws Throwable;
  //  /**
  //   * @param functor
  //   * @return
  //   */
  //  final public static boolean prologEquals(Object thiz, Object functor) {
  //    return thisTerm(thiz).equalJProlog(functor);
  //
  //  }
  final public static Object cos(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.cos(evaluate.doubleValue()));//si2);
  }

  final public static Object divide(Number evaluate, Number si2) {
    // TODO Auto-generated method stub
    if (si2.doubleValue() == 0) throw new EvaluationException("zero_divisor");
    return StaticProlog.makeDouble(evaluate.doubleValue() / si2.doubleValue());
  }

  final public static Object exp(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.exp(evaluate.doubleValue()));//si2);
  }

  final public static Object floatFractPart(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(evaluate.doubleValue() - Math.signum(evaluate.doubleValue()) * Math.floor(Math.abs(evaluate.doubleValue())));//si2);
  }

  final public static Object floatIntPart(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.signum(evaluate.doubleValue()) * Math.floor(Math.abs(evaluate.doubleValue())));//si2);
  }

  final public static Object floor(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeInteger((int) Math.floor(evaluate.doubleValue()));//si2);
  }

  /**
   * @param evaluate
   * @param si2
   * @return
   */
  final public static Object intDivide(Number evaluate, Number si2) {
    // TODO Auto-generated method stub
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(si2)) throw new IllegalTypeException("integer", si2);
    if (si2.longValue() == 0) throw new EvaluationException("zero_divisor");
    return StaticProlog.makeInteger((evaluate.longValue() / si2.longValue()));
  }

  final public static Object log(Number evaluate) {
    // TODO Auto-generated method stub
    if (evaluate.doubleValue() == 0) throw new EvaluationException("undefined");
    return StaticProlog.makeDouble(Math.log(evaluate.doubleValue()));//si2);
  }

  final public static Object max(Number evaluate, Number si2) {
    // TODO Auto-generated method stub
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(si2)) return StaticProlog.makeDouble(Math.max(evaluate.doubleValue(), si2.doubleValue()));
    return StaticProlog.makeInteger(Math.max(evaluate.longValue(), si2.longValue()));
  }

  final public static Object min(Number evaluate, Number si2) {
    // TODO Auto-generated method stub
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(si2)) return StaticProlog.makeDouble(Math.min(evaluate.doubleValue(), si2.doubleValue()));
    return StaticProlog.makeInteger(Math.min(evaluate.longValue(), si2.longValue()));
  }

  final public static Object mod(Number evaluate, Object si2) {
    // TODO Auto-generated method stub
    Number t = (Number) si2;
    //if (isDouble(t)) throw new IllegalTypeException("integer", t);
    if (t.longValue() == 0) throw new EvaluationException("zero_divisor");
    return StaticProlog.makeInteger(evaluate.longValue() % t.longValue());
  }

  final public static Object multiply(Number evaluate, Object si2) {
    // TODO Auto-generated method stub
    Number t = (Number) si2;
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(t)) return StaticProlog.makeDouble(evaluate.doubleValue() * t.doubleValue());
    return StaticProlog.makeInteger(evaluate.longValue() * t.longValue());
  }

  final public static Object not(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeInteger(~evaluate.longValue());//si2);
  }

  final public static Object or(Number evaluate, Number si2) {
    // TODO Auto-generated method stub
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(si2)) throw new IllegalTypeException("integer", si2);
    return StaticProlog.makeInteger(evaluate.longValue() | si2.longValue());
  }

  final public static Object pow(Number evaluate, Number si2) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.pow(evaluate.doubleValue(), si2.doubleValue()));
  }

  //  /** Sets the specified arguments and continuation goal. */
  //  final public final void setArgument(Object[] args, Predicate cont) {
  //  }

  final public static Object rint(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.rint(evaluate.doubleValue()));//si2);
  }

  final public static Object round(Number evaluate) {
    // TODO Auto-generated method stub
    if (StaticProlog.isInteger(evaluate)) return evaluate;
    return StaticProlog.makeInteger(Math.round(evaluate.doubleValue()));//si2);
  }

  final public static Object shiftLeft(Number evaluate, Number si2) {
    // TODO Auto-generated method stub
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(si2)) throw new IllegalTypeException("integer", si2);
    return StaticProlog.makeInteger(evaluate.longValue() << si2.longValue());
  }

  final public static Object shiftRight(Number evaluate, Number si2) {
    // TODO Auto-generated method stub
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(si2)) throw new IllegalTypeException("integer", si2);
    return StaticProlog.makeInteger(evaluate.longValue() >> si2.longValue());
  }

  final public static Object signum(Number evaluate) {
    // TODO Auto-generated method stub
    if (StaticProlog.isDouble(evaluate)) return StaticProlog.makeDouble(Math.signum(evaluate.doubleValue()));
    return StaticProlog.makeInteger((int) Math.signum(evaluate.doubleValue()));//si2);
  }

  final public static Object sin(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.sin(evaluate.doubleValue()));//si2);
  }

  final public static Object sqrt(Number evaluate) {
    // TODO Auto-generated method stub
    if (evaluate.doubleValue() < 0) throw new EvaluationException("undefined");
    return StaticProlog.makeDouble(Math.sqrt(evaluate.doubleValue()));//si2);
  }

  final public static Object subtract(Number evaluate, Object si2) {
    // TODO Auto-generated method stub
    Number t = (Number) si2;
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(t)) return StaticProlog.makeDouble(evaluate.doubleValue() - t.doubleValue());
    return StaticProlog.makeInteger((long) (evaluate.doubleValue() - t.doubleValue()));
  }

  final public static Object tan(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.tan(evaluate.doubleValue()));//si2);
  }

  final public static Object toDegrees(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.toDegrees(evaluate.doubleValue()));//si2);
  }

  final public static Object toFloat(Number evaluate) {
    // TODO Auto-generated method stub
    if (StaticProlog.isDouble(evaluate)) return evaluate;//((Number)this);
    return StaticProlog.makeDouble(evaluate.doubleValue());//si2);
  }

  final public static Object toRadians(Number evaluate) {
    // TODO Auto-generated method stub
    return StaticProlog.makeDouble(Math.toRadians(evaluate.doubleValue()));//si2);
  }

  final public static Object truncate(Number evaluate) {
    // TODO Auto-generated method stub
    if (StaticProlog.isInteger(evaluate)) return evaluate;
    if (evaluate.doubleValue() >= 0)
      return StaticProlog.makeInteger((int) Math.floor(evaluate.doubleValue()));
    else
      return StaticProlog.makeInteger((int) (-1 * Math.floor(Math.abs(evaluate.doubleValue()))));//si2);
  }

  final public static Object xor(Number evaluate, Number si2) {
    // TODO Auto-generated method stub
    if (StaticProlog.isDouble(evaluate) || StaticProlog.isDouble(si2)) throw new IllegalTypeException("integer", si2);
    return StaticProlog.makeInteger(evaluate.longValue() ^ si2.longValue());
  }

}
