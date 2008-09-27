package jp.ac.kobe_u.cs.prolog.lang;

import java.io.Serializable;
import java.util.Stack;

/**
 * Trail stack.<br>
 * The class <code>Trail</code> represents a trail stack.<br>
 * Entries pushed to this trail stack must implement
 * the <code>Undoable</code> interface.
 * @see Undoable
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public class Trail extends Stack<Undoable> implements Serializable {
  /**
   *
   */
  private static final long serialVersionUID = 2109560047917187716L;

  /** Maximum size of enties. Initial size is <code>20000</code>. */
  protected int maxContents = 20000;

  /** An array of <code>Undoable</code> entries. */
  //  protected Undoable[]      elementData;
  /** the top index of this <code>Trail</code>. */
  // protected int             top;
  /** Holds the Prolog engine that this <code>Trail</code> belongs to. */
  final public Prolog engine;

  /** Constructs a new trail stack. */
  public Trail(Prolog _engine) {
    super();
    this.engine = _engine;
    this.elementData = new Undoable[this.maxContents];
    //        this.top = -1;
  }

  /* (non-Javadoc)
   * @see java.util.Stack#push(java.lang.Object)
   */
  @Override
  public Undoable push(Undoable item) {
    // TODO Auto-generated method stub
    return super.push(item);
  }

  /** Constructs a new trail stack with the given size. */
  public Trail(Prolog _engine, int n) {
    this.engine = _engine;
    this.maxContents = n;
    this.elementData = new Undoable[this.maxContents];
    //   this.top = -1;
  }

  /** Discards all entries. */
  protected void deleteAll() {
    while (!this.empty()) {
      pop();
      //   this.elementData[this.top--] = null;
    }
  }

  /** Tests if this stack has no entry. */
  //    @Override
  //    public boolean empty() {
  //        return this.top == -1;
  //    }
  /** Discards all entries. */
  public void init() {
    this.deleteAll();
  }

  /** Returns the value of <code>maxContents</code>.
   * @see #maxContents
   */
  public int max() {
    return this.maxContents;
  }

  //    /** Pops an entry from this <code>Trail</code>. */
  //    @Override
  //    public Undoable pop() {
  //        final Undoable t = this.elementData[this.top];
  //        this.elementData[this.top--] = null;
  //        return t;
  //    }

  //    /** Pushs an entry to this <code>Trail</code>. */
  //    @Override
  //    public Undoable push(Undoable t) {
  //        try {
  //            this.elementData[++this.top] = t;
  //        } catch (final ArrayIndexOutOfBoundsException e) {
  //            System.out.println("{expanding trail...}");
  //            final int len = this.elementData.length;
  //            final Undoable[] new_buffer = new Undoable[len + 20000];
  //            for (int i = 0; i < len; i++) {
  //                new_buffer[i] = this.elementData[i];
  //            }
  //            this.elementData = new_buffer;
  //            this.elementData[this.top] = t;
  //            this.maxContents = len + 20000;
  //        }
  //        return t;
  //    }

  /** Shows the contents of this <code>Trail</code>. */
  public void show() {
    if (this.empty()) {
      System.out.println("{trail stack is empty!}");
      return;
    }
    for (int i = 0; i <= this.top(); i++) {
      System.out.print("trail[" + i + "]: ");
      System.out.println(this.elementData[i]);
    }
  }

  /** Returns the value of <code>top</code>.
   * @see #top
   */
  public int top() {
    return this.size() - 1;
  }

  /** Unwinds all entries after the value of <code>i</code>. */
  public void unwind(int i) {
    Undoable t;
    while (this.top() > i) {
      t = pop();
      t.unTrailSelf();
    }
  }
}
