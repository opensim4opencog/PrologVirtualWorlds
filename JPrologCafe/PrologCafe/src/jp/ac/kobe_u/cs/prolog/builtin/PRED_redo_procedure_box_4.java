package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>redo_procedure_box/4</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_redo_procedure_box_4 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("redo");
    static Predicate _redo_procedure_box_4_sub_1 = new PRED_redo_procedure_box_4_sub_1();
    static Predicate _redo_procedure_box_4_1 = new PRED_redo_procedure_box_4_1();
    static Predicate _redo_procedure_box_4_2 = new PRED_redo_procedure_box_4_2();

    public Object arg1, arg2, arg3, arg4;

    public PRED_redo_procedure_box_4(Object a1, Object a2, Object a3, Object a4, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        this.cont = cont;
    }

    public PRED_redo_procedure_box_4(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        this.cont = cont;
    }

    public int arity() { return 4; }

    public String nameUQ() { return "redo_procedure_box"; }

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
        return "'redo_procedure_box'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine.cont = cont;
        engine.setB0();
        return engine.jtry(_redo_procedure_box_4_1, _redo_procedure_box_4_sub_1);
    }
}

class PRED_redo_procedure_box_4_sub_1 extends PRED_redo_procedure_box_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_redo_procedure_box_4_2);
    }
}

class PRED_redo_procedure_box_4_1 extends PRED_redo_procedure_box_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // redo_procedure_box(A, B, C, D):-true
        Object a1, a2, a3, a4;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // redo_procedure_box(A, B, C, D):-[]
        return exit(engine,cont);
    }
}

class PRED_redo_procedure_box_4_2 extends PRED_redo_procedure_box_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // redo_procedure_box(A, B, C, D):-print_procedure_box(redo, A, B, C, D), fail
        Object a1, a2, a3, a4;
        Predicate p1;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // redo_procedure_box(A, B, C, D):-[print_procedure_box(redo, A, B, C, D), fail]
        p1 = new PRED_fail_0(cont);
        return exit(engine, new PRED_print_procedure_box_5(s1, a1, a2, a3, a4, p1));
    }
}
