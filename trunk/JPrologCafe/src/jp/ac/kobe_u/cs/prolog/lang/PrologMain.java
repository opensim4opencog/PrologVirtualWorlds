package jp.ac.kobe_u.cs.prolog.lang;

import java.util.StringTokenizer;

import jp.ac.kobe_u.cs.prolog.lang.Predicate;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.PrologClassLoader;
import jp.ac.kobe_u.cs.prolog.lang.PrologControl;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.impl.StaticPrologMethodImpl;

/**
 * Prolog Cafe launcher.
 * The <code>PrologMain</code> class launchs the Prolog Cafe system.<br>
 * The usage is as follows, where 
 * <code>package</code> is a package name, and 
 * <code>predicate</code> is a predicate name (only atom).
 * <pre>
 *   % java -cp $PLCAFEDIR/plcafe.jar jp.ac.kobe_u.cs.prolog.lang.PrologMain package:predicate
 *   % java -cp $PLCAFEDIR/plcafe.jar jp.ac.kobe_u.cs.prolog.lang.PrologMain predicate
 * </pre>
 * Let us show a sample session for launching a small Prolog interpreter:
 * <code>jp.ac.kobe_u.cs.prolog.builtin:cafeteria/0</code>.<br>
 * <pre>
 *    % java  -cp $PLCAFEDIR/plcafe.jar:$CLASSPATH jp.ac.kobe_u.cs.prolog.lang.PrologMain jp.ac.kobe_u.cs.prolog.builtin:cafeteria
 *    Prolog Cafe X.X.X (YYY)
 *    Copyright(C) 1997-200X M.Banbara and N.Tamura
 *    | ?- 
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public class PrologMain {
  /** Version information */
  public static String VERSION = "Prolog Cafe 1.1.11 (dragonfly) based on java.lang.Object 2";
  /** Copyright information */
  public static String COPYRIGHT = "Copyright(C) 1997-2008 M.Banbara and N.Tamura";

  public static void main(String argv[]) {
    StaticProlog.methods = new StaticPrologMethodImpl();
    if (argv.length == 0) {
      argv = new String[] { "cafeteria" };
    }
    PrologControl p;
    Predicate code;
    String goal;
    Class clazz;
    try {
      System.err.println("\n" + VERSION);
      System.err.println(COPYRIGHT);
      if (argv.length != 1) {
        usage();
        System.exit(999);
      }
      clazz = (new PrologClassLoader()).loadPredicateClass("jp.ac.kobe_u.cs.prolog.builtin", "initialization", 2, true);
      Object arg1 = Prolog.Nil;
      arg1 = StaticProlog.makeList(StaticProlog.makeAtom("user"), arg1);
      arg1 = StaticProlog.makeList(StaticProlog.makeAtom("jp.ac.kobe_u.cs.prolog.builtin"), arg1);
      //	    arg1 = Prolog.makeList(Prolog.makeSymbol("jp.ac.kobe_u.cs.prolog.compiler.pl2am"), arg1);
      //	    arg1 = Prolog.makeList(Prolog.makeSymbol("jp.ac.kobe_u.cs.prolog.compiler.am2j"), arg1);
      Object arg2 = parseAtomicGoal(argv[0]);
      if (arg2 == null) {
        usage();
        System.exit(1);
      }
      Object[] args = { arg1, arg2 };
      code = (Predicate) (clazz.newInstance());
      p = new PrologControl();
      p.setPredicate(code, args);
      for (boolean r = p.call(); r; r = p.redo()) {
      }
      System.exit(0);
    } catch (Exception e) {
      e.printStackTrace();
      System.exit(1);
    }
  }

  /** Returns a term for given string representation of atom goal, or 
   * <code>null</code> if parsing fails.
   * @param s  a string representation of initial goal (ex. foge:main).
   * @return a term which corresponds to a given string,
   * or <code>null</code> if parsing fails.
   */
  protected static Object parseAtomicGoal(String s) {
    StringTokenizer st = new StringTokenizer(s, ":");
    int i = st.countTokens();
    if (i == 1) {
      Object[] args = { StaticProlog.makeAtom("user"), StaticProlog.makeAtom(st.nextToken()) };
      return StaticProlog.makeStructure(StaticProlog.makeAtom(":", 2), args);
    } else if (i == 2) {
      Object[] args = { StaticProlog.makeAtom(st.nextToken()), StaticProlog.makeAtom(st.nextToken()) };
      return StaticProlog.makeStructure(StaticProlog.makeAtom(":", 2), args);
    } else {
      return null;
    }
  }

  /** Shows usage */
  protected static void usage() {
    String s = "Usage:\n";
    s += "java -cp $PLCAFEDIR/plcafe.jar";
    s += " jp.ac.kobe_u.cs.prolog.lang.PrologMain package:predicate\n";
    s += "java -cp $PLCAFEDIR/plcafe.jar";
    s += " jp.ac.kobe_u.cs.prolog.lang.PrologMain predicate\n";
    s += "    package:        package name\n";
    s += "    predicate:      predicate name (only atom)";
    System.out.println(s);
  }

  /*
  public static Object makeList(int n){
  Object t = Prolog.makeSymbol("[]");
  for (int i=n; i>0; i--){
    t = Prolog.makeList(Prolog.makeInteger(i), t);
  }
  return t;
  }
  */
}
