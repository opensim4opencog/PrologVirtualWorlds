/* PRED_open_4 - at Sun Sep 21 05:38:51 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.PushbackReader;

import jp.ac.kobe_u.cs.prolog.lang.*;

public class PRED_open_4 extends PredicateBase
{
    /**
   * 
   */
  private static final long serialVersionUID = -2504908944907240194L;
    public static /*SymbolTerm*/Object SYM_NIL = makeAtom("[]");
    public static /*SymbolTerm*/Object SYM_TEXT = makeAtom("text");
    public static /*SymbolTerm*/Object SYM_READ = makeAtom("read");
    public static /*SymbolTerm*/Object SYM_WRITE = makeAtom("write");
    public static /*SymbolTerm*/Object SYM_APPEND = makeAtom("append");
    public static /*SymbolTerm*/Object SYM_INPUT = makeAtom("input");
    public static /*SymbolTerm*/Object SYM_OUTPUT = makeAtom("output");
    public static /*SymbolTerm*/Object SYM_ALIAS_1 = makeAtom("alias", 1);
    public static /*SymbolTerm*/Object SYM_MODE_1 = makeAtom("mode", 1);
    public static /*SymbolTerm*/Object SYM_TYPE_1 = makeAtom("type", 1);
    public static /*SymbolTerm*/Object SYM_FILE_NAME_1 = makeAtom("file_name", 1);
    public Object arg1;
    public Object arg2;
    public Object arg3;
    public Object arg4;

    @Override
    public  int arity() {
        /* flow_0_0_ConstantOperator
 */
return 4;
}
    public  PRED_open_4() {
    /* flow_0_0_EmptyBlock_393171553
 */
/* empty */}
    @Override
    public  String toString() {
        /* flow_0_0_StringAddOperator
 */
return "open(" + this.arg1 + "," + this.arg2 + "," + this.arg3 + "," + this.arg4 + ")";
}
    public  PRED_open_4(Object a1, Object a2, Object a3, Object a4, Predicate cont) {
        /* flow_0_0_PutFieldOperator
 */
this.arg1 = a1;
    this.arg2 = a2;
    this.arg3 = a3;
    this.arg4 = a4;
    this.cont = cont;
}
    @Override
    public  Predicate exec(Prolog engine) {
        /* flow_0_0_LocalStoreOperator
 */
final Object[] engine_aregs = engine.getAreg();
    engine.setB0();
    Object alias = null;
    Object opts = PRED_open_4.SYM_NIL;
    Object a1 = this.arg1;
    Object a2 = this.arg2;
    Object a3 = this.arg3;
    Object a4 = this.arg4;
    a3 = deref(a3);
    if (!isVariable(a3))
 throw new IllegalTypeException(this, 3, "variable", a3);
    a1 = deref(a1);
    if (isVariable(a1))
 throw new PInstantiationException(this, 1);
    if (!isAtomTerm(a1))
 throw new IllegalDomainException(this, 1, "source_sink", a1);
    final File file = new File(nameUQ(a1));
    a2 = deref(a2);
    if (isVariable(a2))
 throw new PInstantiationException(this, 2);
    if (!isAtomTerm(a2))
 throw new IllegalTypeException(this, 2, "atom", a2);
    Object streamObject;
    try {
 if (prologEquals(a2, PRED_open_4.SYM_READ)){
     if (!file.exists())
  throw new ExistenceException(this, 1, "source_sink", a1, "");
     final PushbackReader in = new PushbackReader(new BufferedReader(new FileReader(file)),Prolog. PUSHBACK_SIZE);
     streamObject = makeJavaObject(in);
     opts = makeList(PRED_open_4.SYM_INPUT, opts);
 } else if (prologEquals(a2, PRED_open_4.SYM_WRITE)){
     final PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(file, false)));
     streamObject = makeJavaObject(out);
     opts = makeList(PRED_open_4.SYM_OUTPUT, opts);
 } else if (prologEquals(a2, PRED_open_4.SYM_APPEND)){
     final PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(file, true)));
     streamObject = makeJavaObject(out);
     opts = makeList(PRED_open_4.SYM_OUTPUT, opts);
 } else
     throw new IllegalDomainException(this, 2, "io_mode", a2);
    } catch (final IOException e){
 throw new PermissionException(this, "open", "source_sink", a1, "");
    }
    if (engine.getStreamManager().containsKey(streamObject))
 throw new InternalException("stream object is duplicated");
    a4 = deref(a4);
    for (Object tmp = a4; !isNil(tmp); tmp = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.rest((Object) tmp))){
 if (isVariable(tmp))
     throw new PInstantiationException(this, 4);
 if (!isListTerm(tmp))
     throw new IllegalTypeException(this, 4, "list", a4);
 final Object car = deref(jp.ac.kobe_u.cs.prolog.lang.ListTermBase.first((Object) tmp));
 if (isVariable(car))
     throw new PInstantiationException(this, 4);
 if (isCompound(car)){
     final /*SymbolTerm*/Object functor = jp.ac.kobe_u.cs.prolog.lang.StructureTermBase.functor((Object) car);
     final Object[] args = args(car);
     if (prologEquals(functor, PRED_open_4.SYM_ALIAS_1)){
  alias = deref(args[0]);
  if (!isAtomTerm(alias))
      throw new IllegalDomainException(this, 4, "stream_option", car);
  if (!engine.getStreamManager().containsKey(alias))
      continue ;
  throw new PermissionException(this, "open", "source_sink", car, "");
     }
     throw new IllegalDomainException(this, 4, "stream_option", car);
 }
 throw new IllegalDomainException(this, 4, "stream_option", car);
    }
    final Object[] args1 = { PRED_open_4.SYM_TEXT};
    final Object[] args2 = { a2};
    final Object[] args3 = { makeAtom(file.getAbsolutePath())};
    opts = makeList(makeStructure(PRED_open_4.SYM_TYPE_1, args1), opts);
    opts = makeList(makeStructure(PRED_open_4.SYM_MODE_1, args2), opts);
    opts = makeList(makeStructure(PRED_open_4.SYM_FILE_NAME_1, args3), opts);
    if (alias != null){
 engine.getStreamManager().put(alias, streamObject);
 final Object[] as = { alias};
 opts = makeList(makeStructure(PRED_open_4.SYM_ALIAS_1, as), opts);
    }
    jp.ac.kobe_u.cs.prolog.lang.VariableTermBase.bind((VariableTerm) a3, streamObject);
    engine.getStreamManager().put(streamObject, opts);
    return this.cont;
}
    @Override
    public  void setArgument(Object[] args, Predicate cont) {
    /* flow_0_0_PutFieldOperator
 */
this.arg1 = args[0];
    this.arg2 = args[1];
    this.arg3 = args[2];
    this.arg4 = args[3];
    this.cont = cont;
}}
