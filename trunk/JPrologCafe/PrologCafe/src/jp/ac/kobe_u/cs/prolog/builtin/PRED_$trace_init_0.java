package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$trace_init'/0</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$trace_init_0 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom(":", 2);
    static /*Symbol*/Object s2 = makeAtom("jp.ac.kobe_u.cs.prolog.builtin");
    static /*Symbol*/Object s3 = makeAtom("$leap_flag", 1);
    static /*Symbol*/Object s4 = makeAtom("$current_leash", 1);
    static /*Symbol*/Object s5 = makeAtom("$current_spypoint", 3);
    static /*Symbol*/Object s6 = makeAtom("no");
    static Object[] s7 = {s6};
    static /*Structure*/Object s8 = makeStructure(s3, s7);
    static Object[] s9 = {s2, s8};
    static /*Structure*/Object s10 = makeStructure(s1, s9);
    static /*Symbol*/Object s11 = makeAtom("call");
    static Object[] s12 = {s11};
    static /*Structure*/Object s13 = makeStructure(s4, s12);
    static Object[] s14 = {s2, s13};
    static /*Structure*/Object s15 = makeStructure(s1, s14);
    static /*Symbol*/Object s16 = makeAtom("exit");
    static Object[] s17 = {s16};
    static /*Structure*/Object s18 = makeStructure(s4, s17);
    static Object[] s19 = {s2, s18};
    static /*Structure*/Object s20 = makeStructure(s1, s19);
    static /*Symbol*/Object s21 = makeAtom("redo");
    static Object[] s22 = {s21};
    static /*Structure*/Object s23 = makeStructure(s4, s22);
    static Object[] s24 = {s2, s23};
    static /*Structure*/Object s25 = makeStructure(s1, s24);
    static /*Symbol*/Object s26 = makeAtom("fail");
    static Object[] s27 = {s26};
    static /*Structure*/Object s28 = makeStructure(s4, s27);
    static Object[] s29 = {s2, s28};
    static /*Structure*/Object s30 = makeStructure(s1, s29);

    public PRED_$trace_init_0(Predicate cont) {
        this.cont = cont;
    }

    public PRED_$trace_init_0(){}

    public void setArgument(Object[] args, Predicate cont) {
        this.cont = cont;
    }

    public int arity() { return 0; }

    public String nameUQ() { return "$trace_init"; }

    public void sArg(int i0, Object val) {  switch (i0) {default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$trace_init";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
    // '$trace_init':-retractall('$leap_flag'(A)), retractall('$current_leash'(B)), retractall('$current_spypoint'(C, D, E)), assertz('$leap_flag'(no)), assertz('$current_leash'(call)), assertz('$current_leash'(exit)), assertz('$current_leash'(redo)), assertz('$current_leash'(fail)), !
        engine.setB0();
        Object a1, a2, a3, a4, a5, a6, a7;
        Predicate p1, p2, p3, p4, p5, p6, p7, p8;
    // '$trace_init':-['$get_level'(A), retractall('jp.ac.kobe_u.cs.prolog.builtin':'$leap_flag'(B)), retractall('jp.ac.kobe_u.cs.prolog.builtin':'$current_leash'(C)), retractall('jp.ac.kobe_u.cs.prolog.builtin':'$current_spypoint'(D, E, F)), assertz('jp.ac.kobe_u.cs.prolog.builtin':'$leap_flag'(no)), assertz('jp.ac.kobe_u.cs.prolog.builtin':'$current_leash'(call)), assertz('jp.ac.kobe_u.cs.prolog.builtin':'$current_leash'(exit)), assertz('jp.ac.kobe_u.cs.prolog.builtin':'$current_leash'(redo)), assertz('jp.ac.kobe_u.cs.prolog.builtin':'$current_leash'(fail)), '$cut'(A)]
        a1 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(1))
        if (! unify(a1,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        Object[] y1 = {engine.makeVariable(this)};
        a2 = makeStructure(s3, y1);
        Object[] y2 = {s2, a2};
        a3 = makeStructure(s1, y2);
        Object[] y3 = {engine.makeVariable(this)};
        a4 = makeStructure(s4, y3);
        Object[] y4 = {s2, a4};
        a5 = makeStructure(s1, y4);
        Object[] y5 = {engine.makeVariable(this), engine.makeVariable(this), engine.makeVariable(this)};
        a6 = makeStructure(s5, y5);
        Object[] y6 = {s2, a6};
        a7 = makeStructure(s1, y6);
        p1 = new PRED_$cut_1(a1, cont);
        p2 = new PRED_assertz_1(s30, p1);
        p3 = new PRED_assertz_1(s25, p2);
        p4 = new PRED_assertz_1(s20, p3);
        p5 = new PRED_assertz_1(s15, p4);
        p6 = new PRED_assertz_1(s10, p5);
        p7 = new PRED_retractall_1(a7, p6);
        p8 = new PRED_retractall_1(a5, p7);
        return exit(engine, new PRED_retractall_1(a3, p8));
    }
}
