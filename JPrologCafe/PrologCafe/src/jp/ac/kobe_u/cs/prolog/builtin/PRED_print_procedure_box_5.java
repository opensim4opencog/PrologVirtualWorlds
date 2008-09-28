package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>print_procedure_box/5</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_print_procedure_box_5 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("/", 2);
    static /*Symbol*/Object s2 = makeAtom(":", 2);
    static /*Symbol*/Object s3 = makeAtom("jp.ac.kobe_u.cs.prolog.builtin");
    static /*Symbol*/Object s4 = makeAtom("$current_spypoint", 3);
    static /*Symbol*/Object s5 = makeAtom("+");
    static /*Symbol*/Object s6 = makeAtom(":");
    static /*Symbol*/Object s7 = makeAtom("[]");
    static /*Symbol*/Object s8 = makeAtom("print_procedure_box", 5);
    static /*Symbol*/Object s9 = makeAtom("$leap_flag", 1);
    static /*Symbol*/Object s10 = makeAtom("no");
    static Object[] s11 = {s10};
    static /*Structure*/Object s12 = makeStructure(s9, s11);
    static Object[] s13 = {s3, s12};
    static /*Structure*/Object s14 = makeStructure(s2, s13);
    static /*Symbol*/Object s15 = makeAtom(" ");
    static Predicate _print_procedure_box_5_sub_1 = new PRED_print_procedure_box_5_sub_1();
    static Predicate _print_procedure_box_5_sub_2 = new PRED_print_procedure_box_5_sub_2();
    static Predicate _print_procedure_box_5_1 = new PRED_print_procedure_box_5_1();
    static Predicate _print_procedure_box_5_2 = new PRED_print_procedure_box_5_2();
    static Predicate _print_procedure_box_5_3 = new PRED_print_procedure_box_5_3();

    public Object arg1, arg2, arg3, arg4, arg5;

    public PRED_print_procedure_box_5(Object a1, Object a2, Object a3, Object a4, Object a5, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        arg5 = a5;
        this.cont = cont;
    }

    public PRED_print_procedure_box_5(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        arg5 = args[4];
        this.cont = cont;
    }

    public int arity() { return 5; }

    public String nameUQ() { return "print_procedure_box"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
         case 2: arg3 = val;break ;
         case 3: arg4 = val;break ;
         case 4: arg5 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
         case 2: return arg3;
         case 3: return arg4;
         case 4: return arg5;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'print_procedure_box'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + "," + argString(arg5,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine_aregs[5] = arg5;
        engine.cont = cont;
        engine.setB0();
        return engine.jtry(_print_procedure_box_5_1, _print_procedure_box_5_sub_1);
    }
}

class PRED_print_procedure_box_5_sub_1 extends PRED_print_procedure_box_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_print_procedure_box_5_2, _print_procedure_box_5_sub_2);
    }
}

class PRED_print_procedure_box_5_sub_2 extends PRED_print_procedure_box_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_print_procedure_box_5_3);
    }
}

class PRED_print_procedure_box_5_1 extends PRED_print_procedure_box_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // print_procedure_box(A, B, C, D/E, F):-clause('$current_spypoint'(C, D, E), G), !, '$builtin_message'([+, F, A, :, C:B]), '$read_blocked'(print_procedure_box(A, B, C, D/E, F))
        Object a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18;
        Predicate p1, p2, p3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        a5 = engine_aregs[5];
        cont = engine.cont;
    // print_procedure_box(A, B, C, D/E, F):-['$get_level'(G), clause('jp.ac.kobe_u.cs.prolog.builtin':'$current_spypoint'(C, D, E), H), '$cut'(G), '$builtin_message'([+, F, A, :, C:B]), '$read_blocked'(print_procedure_box(A, B, C, D/E, F))]
        a4 = deref( a4);
        if (isCompound(a4)){
            if (!functorOf( s1,a4))
                return fail(engine);
            Object[] args = args(a4);
            a6 = args[0];
            a7 = args[1];
        } else if (isVariable(a4)){
            a6 = engine.makeVariable(this);
            a7 = engine.makeVariable(this);
            Object[] args = {a6, a7};
            bind(/*VAR*/ a4,makeStructure(s1, args));
        } else {
            return fail(engine);
        }
        a8 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(8))
        if (! unify(a8,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        Object[] y1 = {a3, a6, a7};
        a9 = makeStructure(s4, y1);
        Object[] y2 = {s3, a9};
        a10 = makeStructure(s2, y2);
        Object[] y3 = {a3, a2};
        a11 = makeStructure(s2, y3);
        a12 = makeList(a11, s7);
        a13 = makeList(s6, a12);
        a14 = makeList(a1, a13);
        a15 = makeList(a5, a14);
        a16 = makeList(s5, a15);
        Object[] y4 = {a6, a7};
        a17 = makeStructure(s1, y4);
        Object[] y5 = {a1, a2, a3, a17, a5};
        a18 = makeStructure(s8, y5);
        p1 = new PRED_$read_blocked_1(a18, cont);
        p2 = new PRED_$builtin_message_1(a16, p1);
        p3 = new PRED_$cut_1(a8, p2);
        return exit(engine, new PRED_clause_2(a10, engine.makeVariable(this), p3));
    }
}

class PRED_print_procedure_box_5_2 extends PRED_print_procedure_box_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // print_procedure_box(A, B, C, D, E):-clause('$leap_flag'(no), F), !, '$builtin_message'([' ', E, A, :, C:B]), '$dummy_27_builtins.pl'(A, B, C, D, E, G)
        Object a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12;
        Predicate p1, p2, p3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        a5 = engine_aregs[5];
        cont = engine.cont;
    // print_procedure_box(A, B, C, D, E):-['$get_level'(F), clause('jp.ac.kobe_u.cs.prolog.builtin':'$leap_flag'(no), G), '$cut'(F), '$builtin_message'([' ', E, A, :, C:B]), '$dummy_27_builtins.pl'(A, B, C, D, E, H)]
        a6 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(6))
        if (! unify(a6,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        Object[] y1 = {a3, a2};
        a7 = makeStructure(s2, y1);
        a8 = makeList(a7, s7);
        a9 = makeList(s6, a8);
        a10 = makeList(a1, a9);
        a11 = makeList(a5, a10);
        a12 = makeList(s15, a11);
        p1 = new PRED_$dummy_27_builtins$002Epl_6(a1, a2, a3, a4, a5, engine.makeVariable(this), cont);
        p2 = new PRED_$builtin_message_1(a12, p1);
        p3 = new PRED_$cut_1(a6, p2);
        return exit(engine, new PRED_clause_2(s14, engine.makeVariable(this), p3));
    }
}

class PRED_print_procedure_box_5_3 extends PRED_print_procedure_box_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // print_procedure_box(A, B, C, D, E):-true
        Object a1, a2, a3, a4, a5;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        a5 = engine_aregs[5];
        cont = engine.cont;
    // print_procedure_box(A, B, C, D, E):-[]
        return exit(engine,cont);
    }
}