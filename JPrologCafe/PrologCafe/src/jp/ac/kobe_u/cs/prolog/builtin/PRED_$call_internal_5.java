package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$call_internal'/5</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$call_internal_5 extends PredicateBase {
    static /*IntegerTerm*/Object si1 = makeInteger(1);
    static /*Symbol*/Object s2 = makeAtom(":", 2);
    static Predicate _$call_internal_5_sub_1 = new PRED_$call_internal_5_sub_1();
    static Predicate _$call_internal_5_1 = new PRED_$call_internal_5_1();
    static Predicate _$call_internal_5_2 = new PRED_$call_internal_5_2();

    public Object arg1, arg2, arg3, arg4, arg5;

    public PRED_$call_internal_5(Object a1, Object a2, Object a3, Object a4, Object a5, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        arg5 = a5;
        this.cont = cont;
    }

    public PRED_$call_internal_5(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        arg5 = args[4];
        this.cont = cont;
    }

    public int arity() { return 5; }

    public String nameUQ() { return "$call_internal"; }

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
        return "'$call_internal'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + "," + argString(arg5,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine_aregs[5] = arg5;
        engine.cont = cont;
        engine.setB0();
        return engine.jtry(_$call_internal_5_1, _$call_internal_5_sub_1);
    }
}

class PRED_$call_internal_5_sub_1 extends PRED_$call_internal_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$call_internal_5_2);
    }
}

class PRED_$call_internal_5_1 extends PRED_$call_internal_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$call_internal'(A, B, C, D, E):-'$new_internal_database'(B), hash_contains_key(B, C), !, '$get_level'(F), G is D+1, clause(B:A, H), '$meta_call'(H, B, F, G, E)
        Object a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
        Predicate p1, p2, p3, p4, p5, p6;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        a5 = engine_aregs[5];
        cont = engine.cont;
    // '$call_internal'(A, B, C, D, E):-['$get_level'(F), '$new_internal_database'(B), hash_contains_key(B, C), '$cut'(F), '$get_level'(G), '$plus'(D, 1, H), clause(B:A, I), '$meta_call'(I, B, G, H, E)]
        a6 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(6))
        if (! unify(a6,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        a7 = engine.makeVariable(this);
        a8 = engine.makeVariable(this);
        Object[] y1 = {a2, a1};
        a9 = makeStructure(s2, y1);
        a10 = engine.makeVariable(this);
        p1 = new PRED_$meta_call_5(a10, a2, a7, a8, a5, cont);
        p2 = new PRED_clause_2(a9, a10, p1);
        p3 = new PRED_$plus_3(a4, si1, a8, p2);
        p4 = new PRED_$get_level_1(a7, p3);
        p5 = new PRED_$cut_1(a6, p4);
        p6 = new PRED_hash_contains_key_2(a2, a3, p5);
        return exit(engine, new PRED_$new_internal_database_1(a2, p6));
    }
}

class PRED_$call_internal_5_2 extends PRED_$call_internal_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$call_internal'(A, B, C, D, E):-'$call'(B, A)
        Object a1, a2, a3, a4, a5;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        a5 = engine_aregs[5];
        cont = engine.cont;
    // '$call_internal'(A, B, C, D, E):-['$call'(B, A)]
        return exit(engine, new PRED_$call_2(a2, a1, cont));
    }
}
