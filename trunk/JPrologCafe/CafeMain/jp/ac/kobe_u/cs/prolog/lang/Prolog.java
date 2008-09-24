package jp.ac.kobe_u.cs.prolog.lang;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.PushbackReader;
import java.io.Serializable;
import java.util.Dictionary;
import java.util.Hashtable;
import java.util.Map;
import static jp.ac.kobe_u.cs.prolog.lang.StaticProlog.*;

/**
 * Prolog engine.
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
*/
public class Prolog implements Serializable {

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
  public PrologClassLoader pcl;
  /** Internal Database */
  public InternalDatabase internalDB;

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
  protected Map copyHash;

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

  /** Hashtable for managing internal databases. */
  protected HashtableOfTerm hashManager;

  /** Holds an atom <code>[]<code> (empty list). */
  public static/*SymbolTerm*/Object Nil = makeSymbol("[]");

  /* Some symbols for stream options */
  static/*SymbolTerm*/Object SYM_MODE_1 = makeSymbol("mode", 1);
  static/*SymbolTerm*/Object SYM_ALIAS_1 = makeSymbol("alias", 1);
  static/*SymbolTerm*/Object SYM_TYPE_1 = makeSymbol("type", 1);
  static/*SymbolTerm*/Object SYM_READ = makeSymbol("read");
  static/*SymbolTerm*/Object SYM_APPEND = makeSymbol("append");
  static/*SymbolTerm*/Object SYM_INPUT = makeSymbol("input");
  static/*SymbolTerm*/Object SYM_OUTPUT = makeSymbol("output");
  static/*SymbolTerm*/Object SYM_TEXT = makeSymbol("text");
  static/*SymbolTerm*/Object SYM_USERINPUT = makeSymbol("user_input");
  static/*SymbolTerm*/Object SYM_USEROUTPUT = makeSymbol("user_output");
  static/*SymbolTerm*/Object SYM_USERERROR = makeSymbol("user_error");

  /** Constructs new Prolog engine. */
  public Prolog(PrologControl c) {
    control = c;
    aregs = new Object[maxArity];
    setCont(null);
    orStack = new CPFStack(this);
    trail = new Trail(this);
    pcl = new PrologClassLoader();
    internalDB = new InternalDatabase();
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
  protected void initOnce() {
    userInput = new PushbackReader(new BufferedReader(new InputStreamReader(System.in)), PUSHBACK_SIZE);
    userOutput = new PrintWriter(new BufferedWriter(new OutputStreamWriter(System.out)), true);
    userError = new PrintWriter(new BufferedWriter(new OutputStreamWriter(System.err)), true);

    copyHash = new HashtableOfTerm<VariableTerm>();
    hashManager = new HashtableOfTerm();
    streamManager = new HashtableOfTerm();//Prolog.makeHashtableOf();

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
    exception = makeSymbol("$no_ball");
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
    orStack.cut(((NumberTerm) i).intValue());
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
    return StaticProlog.copy(t, this);
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
    return hashManager;
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
  public VariableTerm makeVariable(Predicate pred) {
    // TODO Auto-generated method stub
    return new VariableTermBase(this);
  }

  /**
   * @param s2
   * @param terms
   * @return
   */
  public static StructureTerm makeStructure(/*SymbolTerm*/Object s2, Object[] terms) {
    // TODO Auto-generated method stub
    return new StructureTermBase(s2, terms);
  }

  //  /**
  //   * @param makeInteger
  //   * @param token
  //   * @return
  //   */
  public static ListTerm makeList(Object a, Object d) {
    // TODO Auto-generated method stub
    return new ListTermBase(a, d);
  }

  //
  //  static public IntegerTerm makeInteger(int i) {
  //    return new IntegerTerm(i);
  //  }
  //
  //  static public NumberTerm makeDouble(double i) {
  //    return new NumberTerm(i);
  //  }
  //
  static public JavaObjectTerm makeJavaObject(Object i) {
    return new JavaObjectTermBase(i);
  }

  static public/*SymbolTerm*/Object makeSymbol(String n, int a) {
    return SymbolTermBase.makeSymbol(n, a);
  }

  static public/*SymbolTerm*/Object makeSymbol(String n) {
    return SymbolTermBase.makeSymbol(n);
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
