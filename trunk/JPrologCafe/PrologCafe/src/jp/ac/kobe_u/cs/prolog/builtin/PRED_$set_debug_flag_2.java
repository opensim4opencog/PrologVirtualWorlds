package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$set_debug_flag'/2</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$set_debug_flag_2 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("leap");
    static /*Symbol*/Object s2 = makeAtom(":", 2);
    static /*Symbol*/Object s3 = makeAtom("jp.ac.kobe_u.cs.prolog.builtin");
    static /*Symbol*/Object s4 = makeAtom("$leap_flag", 1);
    static Predicate _fail_0 = new PRED_fail_0();
    static Predicate _$set_debug_flag_2_var = new PRED_$set_debug_flag_2_var();
    static Predicate _$set_debug_flag_2_var_1 = new PRED_$set_debug_flag_2_var_1();
    static Predicate _$set_debug_flag_2_1 = new PRED_$set_debug_flag_2_1();
    static Predicate _$set_debug_flag_2_2 = new PRED_$set_debug_flag_2_2();

    public Object arg1, arg2;

    public PRED_$set_debug_flag_2(Object a1, Object a2, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        this.cont = cont;
    }

    public PRED_$set_debug_flag_2(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        this.cont = cont;
    }

    public int arity() { return 2; }

    public String nameUQ() { return "$set_debug_flag"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$set_debug_flag'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine.cont = cont;
        engine.setB0();
        return engine.switch_on_term(_$set_debug_flag_2_var, _fail_0, _fail_0, _$set_debug_flag_2_var, _fail_0, _fail_0);
    }
}

class PRED_$set_debug_flag_2_var extends PRED_$set_debug_flag_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$set_debug_flag_2_1, _$set_debug_flag_2_var_1);
    }
}

class PRED_$set_debug_flag_2_var_1 extends PRED_$set_debug_flag_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$set_debug_flag_2_2);
    }
}

class PRED_$set_debug_flag_2_1 extends PRED_$set_debug_flag_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$set_debug_flag'(leap, A):-clause('$leap_flag'(A), B), !
        Object a1, a2, a3, a4, a5;
        Predicate p1;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        cont = engine.cont;
    // '$set_debug_flag'(leap, A):-['$get_level'(B), clause('jp.ac.kobe_u.cs.prolog.builtin':'$leap_flag'(A), C), '$cut'(B)]
        a1 = deref( a1);
        if (isAtomTerm(a1)){
            if (! prologEquals(a1,s1))
                return fail(engine);
        } else if (isVariable(a1)){
             bind(/*VAR*/ a1,s1);
        } else {
            return fail(engine);
        }
        a3 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(3))
        if (! unify(a3,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        Object[] y1 = {a2};
        a4 = makeStructure(s4, y1);
        Object[] y2 = {s3, a4};
        a5 = makeStructure(s2, y2);
        p1 = new PRED_$cut_1(a3, cont);
        return exit(engine, new PRED_clause_2(a5, engine.makeVariable(this), p1));
    }
}

class PRED_$set_debug_flag_2_2 extends PRED_$set_debug_flag_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$set_debug_flag'(leap, A):-retractall('$leap_flag'(B)), assertz('$leap_flag'(A))
        Object a1, a2, a3, a4, a5, a6;
        Predicate p1;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        cont = engine.cont;
    // '$set_debug_flag'(leap, A):-[retractall('jp.ac.kobe_u.cs.prolog.builtin':'$leap_flag'(B)), assertz('jp.ac.kobe_u.cs.prolog.builtin':'$leap_flag'(A))]
        a1 = deref( a1);
        if (isAtomTerm(a1)){
            if (! prologEquals(a1,s1))
                return fail(engine);
        } else if (isVariable(a1)){
             bind(/*VAR*/ a1,s1);
        } else {
            return fail(engine);
        }
        Object[] y1 = {engine.makeVariable(this)};
        a3 = makeStructure(s4, y1);
        Object[] y2 = {s3, a3};
        a4 = makeStructure(s2, y2);
        Object[] y3 = {a2};
        a5 = makeStructure(s4, y3);
        Object[] y4 = {s3, a5};
        a6 = makeStructure(s2, y4);
        p1 = new PRED_assertz_1(a6, cont);
        return exit(engine, new PRED_retractall_1(a4, p1));
    }
}
