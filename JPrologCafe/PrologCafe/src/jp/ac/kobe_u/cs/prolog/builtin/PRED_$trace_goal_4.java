package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$trace_goal'/4</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$trace_goal_4 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("call");
    static /*Symbol*/Object s2 = makeAtom("trace");
    static /*Symbol*/Object s3 = makeAtom("exit");
    static /*Symbol*/Object s4 = makeAtom("fail");
    static Predicate _$trace_goal_4_sub_1 = new PRED_$trace_goal_4_sub_1();
    static Predicate _$trace_goal_4_1 = new PRED_$trace_goal_4_1();
    static Predicate _$trace_goal_4_2 = new PRED_$trace_goal_4_2();

    public Object arg1, arg2, arg3, arg4;

    public PRED_$trace_goal_4(Object a1, Object a2, Object a3, Object a4, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        this.cont = cont;
    }

    public PRED_$trace_goal_4(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        this.cont = cont;
    }

    public int arity() { return 4; }

    public String nameUQ() { return "$trace_goal"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
         case 2: arg3 = val;break ;
         case 3: arg4 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
         case 2: return arg3;
         case 3: return arg4;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$trace_goal'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine.cont = cont;
        engine.setB0();
        return engine.jtry(_$trace_goal_4_1, _$trace_goal_4_sub_1);
    }
}

class PRED_$trace_goal_4_sub_1 extends PRED_$trace_goal_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$trace_goal_4_2);
    }
}

class PRED_$trace_goal_4_1 extends PRED_$trace_goal_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$trace_goal'(A, B, C, D):-print_procedure_box(call, A, B, C, D), '$call_internal'(A, B, C, D, trace), print_procedure_box(exit, A, B, C, D), redo_procedure_box(A, B, C, D)
        Object a1, a2, a3, a4;
        Predicate p1, p2, p3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$trace_goal'(A, B, C, D):-[print_procedure_box(call, A, B, C, D), '$call_internal'(A, B, C, D, trace), print_procedure_box(exit, A, B, C, D), redo_procedure_box(A, B, C, D)]
        p1 = new PRED_redo_procedure_box_4(a1, a2, a3, a4, cont);
        p2 = new PRED_print_procedure_box_5(s3, a1, a2, a3, a4, p1);
        p3 = new PRED_$call_internal_5(a1, a2, a3, a4, s2, p2);
        return exit(engine, new PRED_print_procedure_box_5(s1, a1, a2, a3, a4, p3));
    }
}

class PRED_$trace_goal_4_2 extends PRED_$trace_goal_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$trace_goal'(A, B, C, D):-print_procedure_box(fail, A, B, C, D), fail
        Object a1, a2, a3, a4;
        Predicate p1;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$trace_goal'(A, B, C, D):-[print_procedure_box(fail, A, B, C, D), fail]
        p1 = new PRED_fail_0(cont);
        return exit(engine, new PRED_print_procedure_box_5(s4, a1, a2, a3, a4, p1));
    }
}
