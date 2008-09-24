package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>abolish/1</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
public class PRED_abolish_1 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom(":", 2);
    static /*Symbol*/Object s2 = makeAtom("abolish", 1);
    static /*Symbol*/Object s3 = makeAtom("modify");
    static /*Symbol*/Object s4 = makeAtom("static_procedure");
    static /*Symbol*/Object s5 = makeAtom("all");
    static Predicate _abolish_1_sub_1 = new PRED_abolish_1_sub_1();
    static Predicate _abolish_1_1 = new PRED_abolish_1_1();
    static Predicate _abolish_1_2 = new PRED_abolish_1_2();

    public Object arg1;

    public PRED_abolish_1(Object a1, Predicate cont) {
        arg1 = a1;
        this.cont = cont;
    }

    public PRED_abolish_1(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        this.cont = cont;
    }

    public int arity() { return 1; }

    public String nameUQ() { return "abolish"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'abolish'(" + argString(arg1,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine.cont = cont;
        engine.setB0();
        return engine.jtry(_abolish_1_1, _abolish_1_sub_1);
    }
}

class PRED_abolish_1_sub_1 extends PRED_abolish_1 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_abolish_1_2);
    }
}

class PRED_abolish_1_1 extends PRED_abolish_1 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // abolish(A):-'$term_to_predicateindicator'(A, B:C, abolish(A)), '$new_internal_database'(B), '$check_procedure_permission'(B:C, modify, static_procedure, abolish(A)), '$new_indexing_hash'(B, C, D), hash_get(D, all, E), '$erase_all'(E), hash_remove(B, C), fail
        Object a1, a2, a3, a4, a5, a6, a7, a8, a9;
        Predicate p1, p2, p3, p4, p5, p6, p7;
        Predicate cont;
        a1 = engine_aregs[1];
        cont = engine.cont;
    // abolish(A):-['$term_to_predicateindicator'(A, B:C, abolish(A)), '$new_internal_database'(B), '$check_procedure_permission'(B:C, modify, static_procedure, abolish(A)), '$new_indexing_hash'(B, C, D), hash_get(D, all, E), '$erase_all'(E), hash_remove(B, C), fail]
        a2 = engine.makeVariable(this);
        a3 = engine.makeVariable(this);
        Object[] y1 = {a2, a3};
        a4 = makeStructure(s1, y1);
        Object[] y2 = {a1};
        a5 = makeStructure(s2, y2);
        Object[] y3 = {a2, a3};
        a6 = makeStructure(s1, y3);
        Object[] y4 = {a1};
        a7 = makeStructure(s2, y4);
        a8 = engine.makeVariable(this);
        a9 = engine.makeVariable(this);
        p1 = new PRED_fail_0(cont);
        p2 = new PRED_hash_remove_2(a2, a3, p1);
        p3 = new PRED_$erase_all_1(a9, p2);
        p4 = new PRED_hash_get_3(a8, s5, a9, p3);
        p5 = new PRED_$new_indexing_hash_3(a2, a3, a8, p4);
        p6 = new PRED_$check_procedure_permission_4(a6, s3, s4, a7, p5);
        p7 = new PRED_$new_internal_database_1(a2, p6);
        return exit(engine, new PRED_$term_to_predicateindicator_3(a1, a4, a5, p7));
    }
}

class PRED_abolish_1_2 extends PRED_abolish_1 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // abolish(A):-true
        Object a1;
        Predicate cont;
        a1 = engine_aregs[1];
        cont = engine.cont;
    // abolish(A):-[]
        return exit(engine,cont);
    }
}
