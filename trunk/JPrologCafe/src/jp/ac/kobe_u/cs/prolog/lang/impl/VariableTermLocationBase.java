/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang.impl;

import jp.ac.kobe_u.cs.prolog.lang.VariableTermLocation;

/**
 * @author root
 *
 */
public class VariableTermLocationBase implements VariableTermLocation {

  Object value;

  /**
   * @param unboundInit
   */
  public VariableTermLocationBase(Object unboundInit0) {
    value = unboundInit0;
  }

  @Override
  public Object getVal() {
    // TODO Auto-generated method stub
    return value;
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.impl.VariableTermLocation#isBound()
   */
  @Override
  public boolean isBound() {
    // TODO Auto-generated method stub
    return value != unboundInit;
  }

  @Override
  public void setVal(Object o) {
    value = o;
  }

}
