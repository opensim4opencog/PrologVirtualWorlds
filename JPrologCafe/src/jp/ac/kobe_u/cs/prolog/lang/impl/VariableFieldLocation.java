/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang.impl;

import java.lang.reflect.Field;

import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation;

/**
 * @author root
 *
 */
public class VariableFieldLocation implements VariableTermLocation {

  final Field field;
  final Object obj;

  /**
   * @param field
   * @param obj2
   */
  public VariableFieldLocation(Field field, Object obj2) {
    // TODO Auto-generated constructor stub
    this.field = field;
    this.obj = obj2;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation#getVal()
   */
  @Override
  public Object getVal() {
    try {
      return field.get(obj);
    } catch (Exception e) {
      e.printStackTrace();
      return null;
    }
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation#isBound()
   */
  @Override
  public boolean isBound() {
    return getVal() != null;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation#setVal(java.lang.Object)
   */
  @Override
  public void setVal(Object o) {
    // TODO Auto-generated method stub
    if (((VariableTermBase) o).value == this) {
      return;
    }
    try {
      Object v = StaticProlog.toJava(o);
      field.set(obj, v);
    } catch (IllegalArgumentException e) {
      e.printStackTrace();
    } catch (IllegalAccessException e) {
      field.setAccessible(true);
      setVal(o);
    }
  }

}
