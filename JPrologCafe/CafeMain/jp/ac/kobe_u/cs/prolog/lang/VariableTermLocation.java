/**
 * 
 */
package jp.ac.kobe_u.cs.prolog.lang;

public interface VariableTermLocation {
  final Object unboundInit = new Object() {
    /* (non-Javadoc)
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
      // TODO Auto-generated method stub
      return "<?NULL-unboundInit?>";
    }
  };
  
  Object getVal();
  void setVal(Object o);
  boolean isBound();
}