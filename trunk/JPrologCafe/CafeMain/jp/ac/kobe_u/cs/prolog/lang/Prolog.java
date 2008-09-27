package jp.ac.kobe_u.cs.prolog.lang;

import static jp.ac.kobe_u.cs.prolog.lang.StaticProlog.deref;
import static jp.ac.kobe_u.cs.prolog.lang.StaticProlog.intValue;
import static jp.ac.kobe_u.cs.prolog.lang.StaticProlog.isAtomTerm;
import static jp.ac.kobe_u.cs.prolog.lang.StaticProlog.isCompound;
import static jp.ac.kobe_u.cs.prolog.lang.StaticProlog.isDouble;
import static jp.ac.kobe_u.cs.prolog.lang.StaticProlog.isInteger;
import static jp.ac.kobe_u.cs.prolog.lang.StaticProlog.isListTerm;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.PushbackReader;
import java.io.Serializable;
import java.lang.Thread.UncaughtExceptionHandler;
import java.util.Map;

/**
 * Prolog engine.
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 * 
[06:34] <dmiles_afk> i have his downloaded and will test in a few
[06:48] <dmiles_afk> hrrm he has three differnce versoons ;P
[06:49] <dmiles_afk> http://www.rl.pri.ee/rl-prolog/rl-prolog.zip,  http://www.rl.pri.ee/prolog/comp/src.tar.gz,   and a version thats based on w-prolog
[06:52] <dmiles_afk> i am trying to merge the choicepoints and undo-trail into the same stack,  Rla did that, do you use two differnt things subconscious?
[06:53] <subconscious> I have one stack
[06:53] <dmiles_afk> do you save the cuts and the varaible undos both onto that stack?
[06:53] <subconscious> yes
[06:54] <dmiles_afk> do you unwind to the last cut on the !/0 ?
[06:54] <subconscious> I'm not sure what that means
[06:55] <dmiles_afk> on a cut pred, do you iterate backwards until you 'find' a cut-object?
[06:55] <subconscious> no
[06:56] <dmiles_afk> erm neck-object
[06:56] <subconscious> I have the cut index to calculate how may choice points to cut, then I merge down the variables stored in each one while popping them off
[06:57] <dmiles_afk> ah so you use a number as well (cut index) .. ok i think RLa does it that way to
[06:58] <dmiles_afk> i started with PrologCafe and trying to simplify it down
[06:59] <dmiles_afk> probably the one that has the best support for everything: indexed dynamic database preds, mature reader with op/3, Cuts, DCGs etc, java preds
[06:59] <dmiles_afk> took it backwards towards it's origins of jProlog,  removed the need to pass around the trail for Unify/Bind/Copy_term
[07:01] <dmiles_afk> since each unifiable at creation get a reference to their creating engine.. then later reusing that reference,  also stroing it in the java.lang.Thread.setUncounghtExceptionHandler
[07:02] * rabbit- (n=johannes@220.253-203-50.VIC.netspace.net.au) Quit (Read error: 104 (Connection reset by peer))
[07:02] <dmiles_afk> however still gotta move towards your guys' single trail model

*/
public class Prolog implements Serializable, java.lang.Thread.UncaughtExceptionHandler {

  static Prolog lastProlog;

  /* (non-Javadoc)
   * @see java.lang.Thread.UncaughtExceptionHandler#uncaughtException(java.lang.Thread, java.lang.Throwable)
   */
  @Override
  public void uncaughtException(Thread t, Throwable e) {
    // TODO Auto-generated method stub
    e.printStackTrace();
    if (prevUncaughtExceptionHandler != null) {
      prevUncaughtExceptionHandler.uncaughtException(t, e);
    } else {
      UncaughtExceptionHandler use = Thread.getDefaultUncaughtExceptionHandler();
      if (use != null) use.uncaughtException(t, e);

    }
  }

  public static Prolog currentProlog() {
    UncaughtExceptionHandler uc = Thread.currentThread().getUncaughtExceptionHandler();
    if (uc instanceof Prolog) return (Prolog) uc;
    return lastProlog;
  }

  final java.lang.Thread.UncaughtExceptionHandler prevUncaughtExceptionHandler;
  /** Prolog thread */
  final public PrologControl control;
  /** Argument registers */
  final private Object[] aregs;
  /** Continuation goal register */
  public Predicate cont;
  /** Choice point frame stack */
  final private CPFStack orStack;
  /** Trail stack */
  final public Trail trail;
  /** Cut pointer */
  public int B0;
  /** Class loader */
  final public PrologClassLoader pcl;
  /** Internal Database */
  final static public InternalDatabase internalDB;
  static {
    internalDB = new InternalDatabase();
  }

  /** Current time stamp of choice point frame */
  protected long CPFTimeStamp;

  /**
   * Exception level of continuation passing loop:
   * <li><code>0</code> for no exception,
   * <li><code>1</code> for <code>halt/0</code>,
   * <li><code>2</code> for <code>freeze/2</code> (not supported yet)
   * </ul>
   */
  public int exceptionRaised;

  /** <font color="red">Not supported yet</font>. Prolog implementation flag: <code>bounded</code>. */
  protected boolean bounded = false;
  /** Prolog implementation flag: <code>max_integer</code>. */
  protected int maxInteger = Integer.MAX_VALUE;
  /** Prolog implementation flag: <code>min_integer</code>. */
  protected int minInteger = Integer.MIN_VALUE;
  /** Prolog implementation flag: <code>integer_rounding_function</code>. */
  protected String integerRoundingFunction = "down";
  /** <font color="red">Not supported yet</font>. Prolog implementation flag: <code>char_conversion</code>. */
  protected String charConversion;
  /** Prolog implementation flag: <code>debug</code>. */
  protected String debug;
  /** Prolog implementation flag: <code>max_arity</code>. */
  protected int maxArity = 255;
  /** Prolog implementation flag: <code>unknown</code>. */
  protected String unknown;
  /** <font color="red">Not supported yet</font>. Prolog implementation flag: <code>double_quotes</code>. */
  protected String doubleQuotes;
  /** Prolog implementation flag: <code>print_stack_trace</code>. */
  protected String printStackTrace;

  /** Holds a list of frozen goals for <code>freeze/2</code> (not implemented yet). */
  protected Object pendingGoals;
  /** Holds an exception term for <code>catch/3</code> and <code>throw/1</code>. */
  protected Object exception;

  /** Holds the start time as <code>long</code> for <code>statistics/2</code>. */
  protected long startRuntime;
  /** Holds the previous time as <code>long</code> for <code>statistics/2</code>. */
  protected long previousRuntime;

  /** Hashtable for creating a copy of term. */
  public Map copyHash;

  /** The size of the pushback buffer used for creating input streams. */
  public static int PUSHBACK_SIZE = 2;

  /** Standard input stream. */
  protected transient PushbackReader userInput;
  /** Standard output stream. */
  protected transient PrintWriter userOutput;
  /** Standard error stream. */
  protected transient PrintWriter userError;
  /** Current input stream. */
  protected transient PushbackReader currentInput;
  /** Current output stream. */
  protected transient PrintWriter currentOutput;
  /** Hashtable for managing input and output streams. */
  protected HashtableOfTerm streamManager;

  /** Holds an atom <code>[]<code> (empty list). */
  public static/*SymbolTerm*/Object Nil = StaticProlog.makeAtom("[]");

  /* Some symbols for stream options */
  static/*SymbolTerm*/Object SYM_MODE_1 = StaticProlog.makeAtom("mode", 1);
  static/*SymbolTerm*/Object SYM_ALIAS_1 = StaticProlog.makeAtom("alias", 1);
  static/*SymbolTerm*/Object SYM_TYPE_1 = StaticProlog.makeAtom("type", 1);
  static/*SymbolTerm*/Object SYM_READ = StaticProlog.makeAtom("read");
  static/*SymbolTerm*/Object SYM_APPEND = StaticProlog.makeAtom("append");
  static/*SymbolTerm*/Object SYM_INPUT = StaticProlog.makeAtom("input");
  static/*SymbolTerm*/Object SYM_OUTPUT = StaticProlog.makeAtom("output");
  static/*SymbolTerm*/Object SYM_TEXT = StaticProlog.makeAtom("text");
  static/*SymbolTerm*/Object SYM_USERINPUT = StaticProlog.makeAtom("user_input");
  static/*SymbolTerm*/Object SYM_USEROUTPUT = StaticProlog.makeAtom("user_output");
  static/*SymbolTerm*/Object SYM_USERERROR = StaticProlog.makeAtom("user_error");

  /** Constructs new Prolog engine. */
  public Prolog(PrologControl c) {
    final Thread t;
    if (c != null && c.thread != null) {
      t = c.thread;
    } else {
      t = Thread.currentThread();
    }
    prevUncaughtExceptionHandler = t.getUncaughtExceptionHandler();
    t.setUncaughtExceptionHandler(this);
    control = c;
    aregs = new Object[maxArity];
    setCont(null);
    orStack = new CPFStack(this);
    trail = new Trail(this);
    pcl = new PrologClassLoader();
    //    internalDB = new InternalDatabase();
    initOnce();
  }

  /** 
   * Initializes some local instances only once.
   * This <code>initOnce</code> method is invoked in the constructor
   * and initializes the following instances:
   * <ul>
   *   <li><code>userInput</code>
   *   <li><code>userOutput</code>
   *   <li><code>userError</code>
   *   <li><code>copyHash</code>
   *   <li><code>hashManager</code>
   *   <li><code>streamManager</code>
   * </ul>
   */
  private void initOnce() {
    lastProlog = this;
    copyHash = StaticProlog.makeHashtableOf();//new HashtableOfTerm<Object>();

    userInput = new PushbackReader(new BufferedReader(new InputStreamReader(System.in)), PUSHBACK_SIZE);
    userOutput = new PrintWriter(new BufferedWriter(new OutputStreamWriter(System.out)), true);
    userError = new PrintWriter(new BufferedWriter(new OutputStreamWriter(System.err)), true);

    streamManager = StaticProlog.makeHashtableOf();// new HashtableOfTerm();//Prolog.makeHashtableOf();

    streamManager.put(SYM_USERINPUT, StaticProlog.makeJavaObject(userInput));
    streamManager.put(StaticProlog.makeJavaObject(userInput), makeStreamProperty(SYM_READ, SYM_INPUT, SYM_USERINPUT, SYM_TEXT));
    streamManager.put(SYM_USEROUTPUT, StaticProlog.makeJavaObject(userOutput));
    streamManager.put(StaticProlog.makeJavaObject(userOutput), makeStreamProperty(SYM_APPEND, SYM_OUTPUT, SYM_USEROUTPUT, SYM_TEXT));
    streamManager.put(SYM_USERERROR, StaticProlog.makeJavaObject(userError));
    streamManager.put(StaticProlog.makeJavaObject(userError), makeStreamProperty(SYM_APPEND, SYM_OUTPUT, SYM_USERERROR, SYM_TEXT));
  }

  /** Initializes this Prolog engine. */
  public void init() {
    orStack.init();
    trail.init();
    B0 = orStack.top();
    CPFTimeStamp = Long.MIN_VALUE;

    // Creates an initial choice point frame.
    Object[] noarg = {};
    orStack.create(noarg, null);
    orStack.setTR(trail.top());
    orStack.setTimeStamp(++CPFTimeStamp);
    orStack.setBP(null);
    orStack.setB0(B0);

    exceptionRaised = 0;

    charConversion = "off";
    debug = "off";
    unknown = "error";
    doubleQuotes = "codes";
    printStackTrace = "off";

    pendingGoals = Nil;
    exception = StaticProlog.makeAtom("$no_ball");
    startRuntime = System.currentTimeMillis();
    previousRuntime = 0;

    userOutput.flush();
    userError.flush();
    currentInput = userInput;
    currentOutput = userOutput;
  }

  /** Sets the top of choice porint stack to <code>B0</code> (cut pointer). */
  public void setB0() {
    B0 = orStack.top();
  }

  //  /** Discards all choice points after the value of <code>i</code>. */
  //  public void cut(int i) {
  //    stack.cut(i);
  //  }

  /** Discards all choice points after the value of <code>i</code>. */
  public void cut(Object i) {
    orStack.cut((int) intValue(i));
  }

  /** Discards all choice points after the value of <code>B0</code>. */
  public void neckCut() {
    orStack.cut(B0);
  }

  /**
   * Returns a copy of term <code>t</code>. 
   * @param t a term to be copied. It must be dereferenced.
   */
  public Object copy(Object t) {
    copyHash.clear();
    return StaticProlog.copy(t);
  }

  /** 
   * Do backtrak.
   * This method restores the value of <code>B0</code>
   * and returns the backtrak point in current choice point.
   */
  public Predicate fail() {
    if (orStack.empty()) return null; // null means fail
    B0 = orStack.getB0(); // restore B0
    return orStack.getBP(); // execute next clause
  }

  /** 
   * Returns the <code>Predicate</code> object refered, respectively, 
   * <code>var</code>, <code>Int</code>, <code>flo</code>, 
   * <code>con</code>, <code>str</code>, or <code>lis</code>, 
   * depending on whether the dereferenced value of argument 
   * register <code>areg[1]</code> is a
   * variable, integer, float,
   * atom, compound term, or non-empty list, respectively.
   */
  public Predicate switch_on_term(Predicate var, Predicate Int, Predicate flo, Predicate con, Predicate str, Predicate lis) {
    Object arg1 = deref(aregs[1]);
    if (StaticProlog.isVariable(arg1)) return var;
    if (isInteger(arg1)) return Int;
    if (isDouble(arg1)) return flo;
    if (isAtomTerm(arg1)) return con;
    if (isListTerm(arg1)) return lis;
    if (isCompound(arg1)) return str;
    return var;
  }

  /**
   * If the dereferenced value of arugment register <code>areg[1]</code>
   * is an integer, float, atom, or compound term (except for non-empty list),
   * this returns the <code>Predicate</code> object to which its key is mapped
   * in hashtable <code>hash</code>.
   *
   * The key is calculated as follows:
   * <ul>
   *   <li>integer - itself
   *   <li>float - itself
   *   <li>atom - itself
   *   <li>compound term - functor/arity
   * </ul>
   *
   * If there is no mapping for the key of <code>areg[1]</code>, 
   * this returns <code>otherwise</code>.
   */
  public Predicate switch_on_hash(Map<Object, Predicate> hash, Predicate otherwise) {
    Object arg1 = deref(aregs[1]);
    Object key;
    if (isInteger(arg1) || isDouble(arg1) || isAtomTerm(arg1)) {
      key = arg1;
    } else if (isCompound(arg1)) {
      key = StaticProlog.functor(arg1);
    } else {
      throw new SystemException("Invalid argument in switch_on_hash");
    }
    Predicate p = hash.get(key);
    if (p != null)
      return p;
    else
      return otherwise;
  }

  /** Restores the argument registers and continuation goal register from the current choice point frame. */
  public void restore() {
    Object[] args = orStack.getArgs();
    int i = args.length;
    System.arraycopy(args, 0, aregs, 1, i);
    setCont(orStack.getCont());
  }

  /** Creates a new choice point frame. */
  public Predicate jtry(Predicate p, Predicate next) {
    int i = p.arity();
    Object[] args = new Object[i];
    System.arraycopy(aregs, 1, args, 0, i);
    orStack.create(args, getCont());
    orStack.setTR(trail.top());
    orStack.setTimeStamp(++CPFTimeStamp);
    orStack.setBP(next);
    orStack.setB0(B0);
    return p;
  }

  /** 
   * Resets all necessary information from the current choice point frame,
   * updates its next clause field to <code>next</code>,
   * and then returns <code>p</code>.
   */
  public Predicate retry(Predicate p, Predicate next) {
    restore();
    trail.unwind(orStack.getTR());
    orStack.setBP(next);
    return p;
  }

  /** 
   * Resets all necessary information from the current choice point frame,
   * discard it, and then returns <code>p</code>.
   */
  public Predicate trust(Predicate p) {
    restore();
    trail.unwind(orStack.getTR());
    orStack.delete();
    return p;
  }

  Object makeStreamProperty(/*SymbolTerm*/Object _mode, /*SymbolTerm*/Object io, /*SymbolTerm*/Object _alias, /*SymbolTerm*/Object _type) {
    Object[] mode = { _mode };
    Object[] alias = { _alias };
    Object[] type = { _type };

    Object t = Nil;
    t = StaticProlog.makeList(StaticProlog.makeStructure(SYM_MODE_1, mode), t);
    t = StaticProlog.makeList(io, t);
    t = StaticProlog.makeList(StaticProlog.makeStructure(SYM_ALIAS_1, alias), t);
    t = StaticProlog.makeList(StaticProlog.makeStructure(SYM_TYPE_1, type), t);
    return t;
  }

  /** Returns the current time stamp of choice point frame. */
  public long getCPFTimeStamp() {
    return CPFTimeStamp;
  }

  /** Returns the value of Prolog implementation flag: <code>bounded</code>. */
  public boolean isBounded() {
    return bounded;
  }

  /** Returns the value of Prolog implementation flag: <code>max_integer</code>. */
  public int getMaxInteger() {
    return maxInteger;
  }

  /** Returns the value of Prolog implementation flag: <code>min_integer</code>. */
  public int getMinInteger() {
    return minInteger;
  }

  /** Returns the value of Prolog implementation flag: <code>integer_rounding_function</code>. */
  public String getIntegerRoundingFunction() {
    return integerRoundingFunction;
  }

  /** Returns the value of Prolog implementation flag: <code>char_conversion</code>. */
  public String getCharConversion() {
    return charConversion;
  }

  /** Sets the value of Prolog implementation flag: <code>char_conversion</code>. */
  public void setCharConversion(String mode) {
    charConversion = mode;
  }

  /** Returns the value of Prolog implementation flag: <code>debug</code>. */
  public String getDebug() {
    return debug;
  }

  /** Sets the value of Prolog implementation flag: <code>debug</code>. */
  public void setDebug(String mode) {
    debug = mode;
  }

  /** Returns the value of Prolog implementation flag: <code>max_arity</code>. */
  public int getMaxArity() {
    return maxArity;
  }

  /** Returns the value of Prolog implementation flag: <code>unknown</code>. */
  public String getUnknown() {
    return unknown;
  }

  /** Sets the value of Prolog implementation flag: <code>unknown</code>. */
  public void setUnknown(String mode) {
    unknown = mode;
  }

  /** Returns the value of Prolog implementation flag: <code>double_quotes</code>. */
  public String getDoubleQuotes() {
    return doubleQuotes;
  }

  /** Sets the value of Prolog implementation flag: <code>double_quotes</code>. */
  public void setDoubleQuotes(String mode) {
    doubleQuotes = mode;
  }

  /** Returns the value of Prolog implementation flag: <code>print_stack_trace</code>. */
  public String getPrintStackTrace() {
    return printStackTrace;
  }

  /** Sets the value of Prolog implementation flag: <code>print_stack_trace</code>. */
  public void setPrintStackTrace(String mode) {
    printStackTrace = mode;
  }

  /** Returns the value of <code>exception</code>. This is used in <code>catch/3</code>. */
  public Object getException() {
    return exception;
  }

  /** Sets the value of <code>exception</code>. This is used in <code>throw/1</code>. */
  public void setException(Object t) {
    exception = t;
  }

  /** Returns the value of <code>startRuntime</code>. This is used in <code>statistics/2</code>. */
  public long getStartRuntime() {
    return startRuntime;
  }

  /** Returns the value of <code>previousRuntime</code>. This is used in <code>statistics/2</code>. */
  public long getPreviousRuntime() {
    return previousRuntime;
  }

  /** Sets the value of <code>previousRuntime</code>. This is used in <code>statistics/2</code>. */
  public void setPreviousRuntime(long t) {
    previousRuntime = t;
  }

  /** Returns the standard input stream. */
  public PushbackReader getUserInput() {
    return userInput;
  }

  /** Returns the standard output stream. */
  public PrintWriter getUserOutput() {
    return userOutput;
  }

  /** Returns the standard error stream. */
  public PrintWriter getUserError() {
    return userError;
  }

  /** Returns the current input stream. */
  public PushbackReader getCurrentInput() {
    return currentInput;
  }

  /** Sets the current input stream to <code>in</code>. */
  public void setCurrentInput(PushbackReader in) {
    currentInput = in;
  }

  /** Returns the current output stream. */
  public PrintWriter getCurrentOutput() {
    return currentOutput;
  }

  /** Sets the current output stream to <code>out</code>. */
  public void setCurrentOutput(PrintWriter out) {
    currentOutput = out;
  }

  /** Returns the stream manager. */
  public HashtableOfTerm getStreamManager() {
    return streamManager;
  }

  /** Returns the hash manager. */
  public HashtableOfTerm getHashManager() {
    return internalDB.hashManager;
  }

  //    /**
  //     * @param prolog
  //     * @return
  //     */
  //    public static VariableTerm makeVariable(Prolog prolog) {
  //        // TODO Auto-generated method stub
  //        return new VariableTermBase(prolog);
  //    }

  /**
   * @param prolog
   * @return
   */
  public Object makeVariable(Predicate pred) {
    // TODO Auto-generated method stub
    return StaticProlog.makeVariable(this);
  }

  /**
   * @return
   */
  public Object[] getAreg() {
    // TODO Auto-generated method stub
    return aregs;
  }

  //  /**
  //   * @param intValue
  //   */
  //  public void cut(long intValue) {
  //    cut((int) intValue);
  //
  //  }

  public void setCont(Predicate cont) {
    this.cont = cont;
  }

  public Predicate getCont() {
    return cont;
  }

  /**
   * @return
   */
  public int stack_top() {
    // TODO Auto-generated method stub
    return orStack.top();
  }

  /**
   * @return
   */
  public int stack_max() {
    // TODO Auto-generated method stub
    return orStack.max();
  }

  /**
   * @return
   */
  public long stack_getTimeStamp() {
    // TODO Auto-generated method stub
    return orStack.getTimeStamp();
  }

  /**
   * @param outOfLoop
   */
  public void trail_push(Undoable outOfLoop) {
    trail.push(outOfLoop);
    // TODO Auto-generated method stub

  }
}
