package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>clause/2</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
public class PRED_clause_2 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom(":", 2);
    static /*Symbol*/Object s2 = makeAtom("clause", 2);
    static /*Symbol*/Object s3 = makeAtom("access");
    static /*Symbol*/Object s4 = makeAtom("private_procedure");
    static /*Symbol*/Object s5 = makeAtom(":-", 2);

    public Object arg1, arg2;

    public PRED_clause_2(Object a1, Object a2, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        this.cont = cont;
    }

    public PRED_clause_2(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        this.cont = cont;
    }

    public int arity() { return 2; }

    public String nameUQ() { return "clause"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'clause'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
    // clause(A, B):-'$head_to_term'(A, C, D:E, clause(A, B)), '$new_internal_database'(D), '$check_procedure_permission'(D:E, access, private_procedure, clause(A, B)), '$clause_internal'(D, E, C, F, G), copy_term(F, (C:-B))
        engine.setB0();
        Object a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11;
        Predicate p1, p2, p3, p4;
        a1 = arg1;
        a2 = arg2;
    // clause(A, B):-['$head_to_term'(A, C, D:E, clause(A, B)), '$new_internal_database'(D), '$check_procedure_permission'(D:E, access, private_procedure, clause(A, B)), '$clause_internal'(D, E, C, F, G), copy_term(F, (C:-B))]
        a3 = engine.makeVariable(this);
        a4 = engine.makeVariable(this);
        a5 = engine.makeVariable(this);
        Object[] y1 = {a4, a5};
        a6 = makeStructure(s1, y1);
        Object[] y2 = {a1, a2};
        a7 = makeStructure(s2, y2);
        Object[] y3 = {a4, a5};
        a8 = makeStructure(s1, y3);
        Object[] y4 = {a1, a2};
        a9 = makeStructure(s2, y4);
        a10 = engine.makeVariable(this);
        Object[] y5 = {a3, a2};
        a11 = makeStructure(s5, y5);
        p1 = new PRED_copy_term_2(a10, a11, cont);
        p2 = new PRED_$clause_internal_5(a4, a5, a3, a10, engine.makeVariable(this), p1);
        p3 = new PRED_$check_procedure_permission_4(a8, s3, s4, a9, p2);
        p4 = new PRED_$new_internal_database_1(a4, p3);
        return exit(engine, new PRED_$head_to_term_4(a1, a3, a6, a7, p4));
    }
}
