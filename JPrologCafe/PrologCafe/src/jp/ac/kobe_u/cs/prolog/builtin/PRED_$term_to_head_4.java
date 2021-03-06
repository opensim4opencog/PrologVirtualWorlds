package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$term_to_head'/4</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$term_to_head_4 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("type", 1);
    static /*Symbol*/Object s2 = makeAtom("callable");
    static Object[] s3 = {s2};
    static /*Structure*/Object s4 = makeStructure(s1, s3);
    static /*IntegerTerm*/Object si5 = makeInteger(1);
    static Predicate _$term_to_head_4_sub_1 = new PRED_$term_to_head_4_sub_1();
    static Predicate _$term_to_head_4_sub_2 = new PRED_$term_to_head_4_sub_2();
    static Predicate _$term_to_head_4_1 = new PRED_$term_to_head_4_1();
    static Predicate _$term_to_head_4_2 = new PRED_$term_to_head_4_2();
    static Predicate _$term_to_head_4_3 = new PRED_$term_to_head_4_3();

    public Object arg1, arg2, arg3, arg4;

    public PRED_$term_to_head_4(Object a1, Object a2, Object a3, Object a4, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        this.cont = cont;
    }

    public PRED_$term_to_head_4(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        this.cont = cont;
    }

    public int arity() { return 4; }

    public String nameUQ() { return "$term_to_head"; }

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
        return "'$term_to_head'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine.cont = cont;
        engine.setB0();
        return engine.jtry(_$term_to_head_4_1, _$term_to_head_4_sub_1);
    }
}

class PRED_$term_to_head_4_sub_1 extends PRED_$term_to_head_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$term_to_head_4_2, _$term_to_head_4_sub_2);
    }
}

class PRED_$term_to_head_4_sub_2 extends PRED_$term_to_head_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$term_to_head_4_3);
    }
}

class PRED_$term_to_head_4_1 extends PRED_$term_to_head_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$term_to_head'(A, A, B, C):-atom(A), !
        Object a1, a2, a3, a4, a5;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$term_to_head'(A, A, B, C):-['$get_level'(D), atom(A), '$cut'(D)]
        if (! unify(a1,a2))
            return fail(engine);
        a5 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(5))
        if (! unify(a5,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of atom(a(1))
        a1 = deref( a1);
        if (! isAtomTerm(a1)) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of $cut(a(5))
        a5 = deref( a5);
        if (! isCutter/*Integer*/(a5)) {
            throw new IllegalTypeException("integer", a5);
        } else {
            engine.cut(( a5));
        }
        //END inline expansion
        return exit(engine,cont);
    }
}

class PRED_$term_to_head_4_2 extends PRED_$term_to_head_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$term_to_head'(A, A, B, C):-compound(A), !
        Object a1, a2, a3, a4, a5;
        Predicate p1;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$term_to_head'(A, A, B, C):-['$get_level'(D), compound(A), '$cut'(D)]
        if (! unify(a1,a2))
            return fail(engine);
        a5 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(5))
        if (! unify(a5,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        p1 = new PRED_$cut_1(a5, cont);
        return exit(engine, new PRED_compound_1(a1, p1));
    }
}

class PRED_$term_to_head_4_3 extends PRED_$term_to_head_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$term_to_head'(A, B, C, D):-illarg(type(callable), D, 1)
        Object a1, a2, a3, a4;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$term_to_head'(A, B, C, D):-[illarg(type(callable), D, 1)]
        return exit(engine, new PRED_illarg_3(s4, a4, si5, cont));
    }
}
