package jp.ac.kobe_u.cs.prolog.lang;

import java.io.Serializable;

/**
 * Choice point frame stack.<br>
 * The <code>CPFStack</code> class represents a stack
 * of choice point frames.<br>
 * Each choice point frame has the following fields:
 * <ul>
 * <li><em>arguments</em>
 * <li><em>continuation goal</em>
 * <li><em>next clause</em>
 * <li><em>trail pointer</em>
 * <li><em>cut point</em>
 * <li><em>time stamp</em>
 * </ul>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
/**
 * Choice point frame.<br>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
class CPFEntry implements Serializable {
  /**
   *
   */
  private static final long serialVersionUID = -3761557812958612952L;
  public long timeStamp;
  public Object[] args; // argument register
  public Predicate cont; // continuation goal
  public Predicate bp; // next cluase
  public int tr; // trail pointer
  public int b0; // cut point

  protected CPFEntry(Object[] args, Predicate cont) {
    this.args = args;
    this.cont = cont;
  }

  @Override
  public String toString() {
    String t = " time:" + this.timeStamp + "\n";
    t = t + "args:";
    for (int i = 0; i < this.args.length; i++) {
      t = t + this.args[i] + " ";
    }
    t = t + "\n";
    t = t + " cont:" + this.cont + "\n";
    t = t + " bp:" + this.bp + "\n";
    t = t + " tr:" + this.tr + "\n";
    t = t + " b0:" + this.b0 + "\n";
    return t;
  }
}
