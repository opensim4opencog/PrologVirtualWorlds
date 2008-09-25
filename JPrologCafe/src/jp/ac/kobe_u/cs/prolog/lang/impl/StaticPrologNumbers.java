/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;

/**
 * @author root
 *
 */
public class StaticPrologNumbers {

  final public static Object abs(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.abs();//si2);
  }

  final public static Object acos(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.acos();//si2);
  }

  final public static Object add(NumberTerm evaluate, Object si2) {
    // TODO Auto-generated method stub
    return evaluate.add(makeNumber(si2));
  }

  final public static Object and(NumberTerm evaluate, NumberTerm si2) {
    // TODO Auto-generated method stub
    return evaluate.and(si2);
  }

  final public static int arithCompareTo(Object evaluate, Object si2) {
    // TODO Auto-generated method stub
    return makeNumber(evaluate).arithCompareTo(makeNumber(si2));
  }

  final public static Object asin(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.asin();//si2);
  }

  final public static Object atan(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.atan();//si2);
  }

  final public static Object ceil(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.ceil();//si2);
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
  final public static Object cos(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.cos();//si2);
  }

  final public static Object divide(NumberTerm evaluate, NumberTerm si2) {
    // TODO Auto-generated method stub
    return evaluate.divide(si2);
  }

  final public static Object exp(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.exp();//si2);
  }

  final public static Object floatFractPart(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.floatFractPart();//si2);
  }

  final public static Object floatIntPart(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.floatIntPart();//si2);
  }

  final public static Object floor(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.floor();//si2);
  }

  /**
   * @param evaluate
   * @param si2
   * @return
   */
  final public static Object intDivide(NumberTerm evaluate, NumberTerm si2) {
    // TODO Auto-generated method stub
    return evaluate.intDivide(si2);
  }

  final public static Object log(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.log();//si2);
  }

  /**
   * @param negate
   * @return
   */
  private static NumberTerm makeNumber(Object o) {
    // TODO Auto-generated method stub
    if (o instanceof NumberTerm) return (NumberTerm) o;
    Number negate = (Number) o;
    boolean isInt = negate.intValue() == negate.hashCode();
    return isInt ? StaticProlog.makeInteger(negate.longValue()) : StaticProlog.makeDouble(negate.doubleValue());
  }

  final public static Object max(NumberTerm evaluate, NumberTerm si2) {
    // TODO Auto-generated method stub
    return evaluate.max(si2);
  }

  final public static Object min(NumberTerm evaluate, NumberTerm si2) {
    // TODO Auto-generated method stub
    return evaluate.min(si2);
  }

  final public static Object mod(NumberTerm evaluate, Object si2) {
    // TODO Auto-generated method stub
    return evaluate.mod(makeNumber(si2));
  }

  final public static Object multiply(NumberTerm evaluate, Object si2) {
    // TODO Auto-generated method stub
    return evaluate.multiply(makeNumber(si2));
  }

  final public static Object not(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.not();//si2);
  }

  final public static Object or(NumberTerm evaluate, NumberTerm si2) {
    // TODO Auto-generated method stub
    return evaluate.or(si2);
  }

  final public static Object pow(NumberTerm evaluate, NumberTerm si2) {
    // TODO Auto-generated method stub
    return evaluate.pow(si2);
  }

  //  /** Sets the specified arguments and continuation goal. */
  //  final public final void setArgument(Object[] args, Predicate cont) {
  //  }

  final public static Object rint(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.rint();//si2);
  }

  final public static Object round(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.round();//si2);
  }

  final public static Object shiftLeft(NumberTerm evaluate, NumberTerm si2) {
    // TODO Auto-generated method stub
    return evaluate.shiftLeft(si2);
  }

  final public static Object shiftRight(NumberTerm evaluate, NumberTerm si2) {
    // TODO Auto-generated method stub
    return evaluate.shiftRight(si2);
  }

  final public static Object signum(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.signum();//si2);
  }

  final public static Object sin(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.sin();//si2);
  }

  final public static Object sqrt(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.sqrt();//si2);
  }

  final public static Object subtract(NumberTerm evaluate, Object si2) {
    // TODO Auto-generated method stub
    return evaluate.subtract(makeNumber(si2));
  }

  final public static Object tan(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.tan();//si2);
  }

  final public static Object toDegrees(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.toDegrees();//si2);
  }

  final public static Object toFloat(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.toFloat();//si2);
  }

  final public static Object toRadians(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.toRadians();//si2);
  }

  final public static Object truncate(NumberTerm evaluate) {
    // TODO Auto-generated method stub
    return evaluate.truncate();//si2);
  }

  final public static Object xor(NumberTerm evaluate, NumberTerm si2) {
    // TODO Auto-generated method stub
    return evaluate.xor(si2);
  }

}
