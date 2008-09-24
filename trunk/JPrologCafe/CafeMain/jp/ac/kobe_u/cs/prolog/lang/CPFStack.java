package jp.ac.kobe_u.cs.prolog.lang;

import java.io.Serializable;
import java.util.Stack;

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
public class CPFStack extends Stack<CPFEntry> implements Serializable {
  /**
   *
   */
  private static final long serialVersionUID = 7131496080069493750L;

  /** Maximum size of enties. Initial size is <code>20000</code>. */
  protected int maxContents = 20000;

  /** An array of choice point frames. */
  // protected CPFEntry[] elementData;
  /** the top index of this <code>CPFStack</code>. */
  // protected int             top;
  /** Holds the Prolog engine that this <code>CPFStack</code> belongs to. */
  protected Prolog engine;

  /** Constructs a new choice point frame stack. */
  public CPFStack(Prolog _engine) {
    this(_engine, 20000);
    //        this.engine = _engine;
    //        this.elementData = new CPFEntry[this.maxContents];
    //this.top = -1;
  }

  /** Constructs a new choice point frame stack with the given size. */
  public CPFStack(Prolog _engine, int n) {
    //super(n);
    this.engine = _engine;
    this.maxContents = n;
    elementData = new CPFEntry[n];
    //    try {
    //      this.elementData = (CPFEntry[]) getClass().getDeclaredField("elements").get(this);
    //    } catch (IllegalArgumentException e) {
    //      // TODO Auto-generated catch block
    //      e.printStackTrace();
    //    } catch (SecurityException e) {
    //      // TODO Auto-generated catch block
    //      e.printStackTrace();
    //    } catch (IllegalAccessException e) {
    //      // TODO Auto-generated catch block
    //      e.printStackTrace();
    //    } catch (NoSuchFieldException e) {
    //      // TODO Auto-generated catch block
    //      e.printStackTrace();
    //    }// new CPFEntry[this.maxContents];

    //this.top = -1;
  }

  /** Create a new choice point frame.
   * @param args <em>arguments</em>
   * @param p a <em>continuation goal</em>
   */
  public void create(Object[] args, Predicate p) {

    if (true) {
      push(new CPFEntry(args, p));
      return;
    }
    //        try {
    //            this.elementData[++this.top] = new CPFEntry(args, p);
    //        } catch (final ArrayIndexOutOfBoundsException e) {
    //            System.out.println("{expanding choice point stack...}");
    //            final int len = this.elementData.length;
    //            final CPFEntry[] new_buffer = new CPFEntry[len + 10000];
    //            for (int i = 0; i < len; i++) {
    //                new_buffer[i] = (CPFEntry) this.elementData[i];
    //            }
    //            this.elementData = new_buffer;
    //            pushCurrent(new CPFEntry(args, p));
    //            this.maxContents = len + 10000;
    //        }
  }

  /** Discards all choice points after the value of <code>i</code>. */
  public void cut(int i) {
    while (this.top() > i) {
      delete();
    }
  }

  /** Discards the top of choice points. */
  public void delete() {
    pop();
    //this.elementData[this.top--] = null;
  }

  /** Discards all choice points. */
  public void deleteAll() {
    while (!this.isEmpty()) {
      delete();
    }
  }

  //    /** Tests if this stack has no entry. */
  //    public boolean empty() {
  //        return this.top == -1;
  //    }

  /** Returns the <em>arguments</em> of current choice point frame. */
  public Object[] getArgs() {
    return peek().args;
  }

  /** Returns the <em>cut point</em> of current choice point frame. */
  public int getB0() {
    return peek().b0;
  }

  /** Returns the <em>next clause</em> of current choice point frame. */
  public Predicate getBP() {
    return peek().bp;
  }

  /** Returns the <em>continuation goal</em> of current choice point frame. */
  public Predicate getCont() {
    return peek().cont;
  }

  /** Returns the <em>time stamp</em> of current choice point frame. */
  public long getTimeStamp() {
    return peek().timeStamp;
  }

  /** Returns the <em>trail pointer</em> of current choice point frame. */
  public int getTR() {
    return peek().tr;
  }

  /** Discards all choice points. */
  public void init() {
    this.deleteAll();
  }

  /** Returns the value of <code>maxContents</code>.
   * @see #maxContents
   */
  public int max() {
    return this.maxContents;
  }

  /** Sets the <em>cut point</em> of current choice point frame. */
  public void setB0(int i) {
    peek().b0 = i;
  }

  /** Sets the <em>next clause</em> of current choice point frame. */
  public void setBP(Predicate p) {
    peek().bp = p;
  }

  /** Sets the <em>time stamp</em> of current choice point frame. */
  public void setTimeStamp(long t) {
    peek().timeStamp = t;
  }

  /** Sets the <em>trail pointer</em> of current choice point frame. */
  public void setTR(int i) {
    peek().tr = i;
  }

  //
  //    public CPFEntry peek() {
  //        return (CPFEntry) this.elementData[this.top];
  //    }

  //
  //    void pushCurrent(CPFEntry ele) {
  //        this.elementData[this.top] = ele;
  //    }

  /** Shows the contents of this <code>CPFStack</code>. */
  public void show() {
    if (this.empty()) {
      System.out.println("{choice point stack is empty!}");
      return;
    }
    for (int i = 0; i <= this.top(); i++) {
      System.out.print("stack[" + i + "]: ");
      System.out.println(this.elementData[i]);
    }
  }

  /**
   * @return
   */
  public boolean empty() {
    // TODO Auto-generated method stub
    return super.isEmpty();
  }

  /** Returns the value of <code>top</code>.
   * @see #top
   */
  public int top() {
    return this.size() - 1;
  }
}
