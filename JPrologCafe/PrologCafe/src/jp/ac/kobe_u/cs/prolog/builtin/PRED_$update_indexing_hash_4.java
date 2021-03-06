package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$update_indexing_hash'/4</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$update_indexing_hash_4 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("a");
    static /*Symbol*/Object s2 = makeAtom("z");
    static Predicate _fail_0 = new PRED_fail_0();
    static Predicate _$update_indexing_hash_4_var = new PRED_$update_indexing_hash_4_var();
    static Predicate _$update_indexing_hash_4_var_1 = new PRED_$update_indexing_hash_4_var_1();
    static Predicate _$update_indexing_hash_4_con = new PRED_$update_indexing_hash_4_con();
    static Predicate _$update_indexing_hash_4_1 = new PRED_$update_indexing_hash_4_1();
    static Predicate _$update_indexing_hash_4_2 = new PRED_$update_indexing_hash_4_2();
    static HashtableOfTerm<Predicate> con = new HashtableOfTerm<Predicate>(2);
    static {
        con.put(s1, _$update_indexing_hash_4_1);
        con.put(s2, _$update_indexing_hash_4_2);
    }

    public Object arg1, arg2, arg3, arg4;

    public PRED_$update_indexing_hash_4(Object a1, Object a2, Object a3, Object a4, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        this.cont = cont;
    }

    public PRED_$update_indexing_hash_4(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        this.cont = cont;
    }

    public int arity() { return 4; }

    public String nameUQ() { return "$update_indexing_hash"; }

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
        return "'$update_indexing_hash'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine.cont = cont;
        engine.setB0();
        return engine.switch_on_term(_$update_indexing_hash_4_var, _fail_0, _fail_0, _$update_indexing_hash_4_con, _fail_0, _fail_0);
    }
}

class PRED_$update_indexing_hash_4_var extends PRED_$update_indexing_hash_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$update_indexing_hash_4_1, _$update_indexing_hash_4_var_1);
    }
}

class PRED_$update_indexing_hash_4_var_1 extends PRED_$update_indexing_hash_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$update_indexing_hash_4_2);
    }
}

class PRED_$update_indexing_hash_4_con extends PRED_$update_indexing_hash_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.switch_on_hash(con, _fail_0);
    }
}

class PRED_$update_indexing_hash_4_1 extends PRED_$update_indexing_hash_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$update_indexing_hash'(a, A, B, C):-!, '$hash_addz_all'(A, B, C)
        Object a1, a2, a3, a4;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$update_indexing_hash'(a, A, B, C):-['$neck_cut', '$hash_addz_all'(A, B, C)]
        a1 = deref( a1);
        if (isAtomTerm(a1)){
            if (! prologEquals(a1,s1))
                return fail(engine);
        } else if (isVariable(a1)){
             bind(/*VAR*/ a1,s1);
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        return exit(engine, new PRED_$hash_addz_all_3(a2, a3, a4, cont));
    }
}

class PRED_$update_indexing_hash_4_2 extends PRED_$update_indexing_hash_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$update_indexing_hash'(z, A, B, C):-!, '$hash_adda_all'(A, B, C)
        Object a1, a2, a3, a4;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$update_indexing_hash'(z, A, B, C):-['$neck_cut', '$hash_adda_all'(A, B, C)]
        a1 = deref( a1);
        if (isAtomTerm(a1)){
            if (! prologEquals(a1,s2))
                return fail(engine);
        } else if (isVariable(a1)){
             bind(/*VAR*/ a1,s2);
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        return exit(engine, new PRED_$hash_adda_all_3(a2, a3, a4, cont));
    }
}
