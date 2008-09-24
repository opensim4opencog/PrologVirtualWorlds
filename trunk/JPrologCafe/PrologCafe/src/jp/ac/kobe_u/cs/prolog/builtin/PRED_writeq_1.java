package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>writeq/1</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
public class PRED_writeq_1 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("quoted", 1);
    static /*Symbol*/Object s2 = makeAtom("true");
    static Object[] s3 = {s2};
    static /*Structure*/Object s4 = makeStructure(s1, s3);
    static /*Symbol*/Object s5 = makeAtom("numbervars", 1);
    static /*Structure*/Object s6 = makeStructure(s5, s3);
    static /*Symbol*/Object s7 = makeAtom("[]");
    static /*List*/Object s8 = makeList(s6, s7);
    static /*List*/Object s9 = makeList(s4, s8);

    public Object arg1;

    public PRED_writeq_1(Object a1, Predicate cont) {
        arg1 = a1;
        this.cont = cont;
    }

    public PRED_writeq_1(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        this.cont = cont;
    }

    public int arity() { return 1; }

    public String nameUQ() { return "writeq"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'writeq'(" + argString(arg1,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
    // writeq(A):-current_output(B), write_term(B, A, [quoted(true), numbervars(true)])
        engine.setB0();
        Object a1, a2;
        Predicate p1;
        a1 = arg1;
    // writeq(A):-[current_output(B), write_term(B, A, [quoted(true), numbervars(true)])]
        a2 = engine.makeVariable(this);
        p1 = new PRED_write_term_3(a2, a1, s9, cont);
        return exit(engine, new PRED_current_output_1(a2, p1));
    }
}
