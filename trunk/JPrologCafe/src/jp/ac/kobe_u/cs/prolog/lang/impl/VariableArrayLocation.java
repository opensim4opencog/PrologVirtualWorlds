/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang.impl;

import java.lang.reflect.Field;

import jp.ac.kobe_u.cs.prolog.lang.JavaException;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation;

/**
 * @author root
 *
 */
public class VariableArrayLocation implements VariableTermLocation {

  final Object[] obj;
  final int index;

  /**
   * @param field
   * @param obj2
   */
  public VariableArrayLocation(int field, Object[] obj2) {
    // TODO Auto-generated constructor stub
    this.index = field;
    this.obj = obj2;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation#getVal()
   */
  @Override
  public Object getVal() {
    try {
      return obj[index];
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
    return obj != null && getVal() != null;
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
      obj[index] = v;
    } catch (Exception e) {
      e.printStackTrace();
      throw new JavaException(e);
    }
  }

}
