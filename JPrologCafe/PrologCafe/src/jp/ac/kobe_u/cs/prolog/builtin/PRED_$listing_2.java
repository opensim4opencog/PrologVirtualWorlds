package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$listing'/2</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$listing_2 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("/", 2);
    static /*Symbol*/Object s2 = makeAtom("type", 1);
    static /*Symbol*/Object s3 = makeAtom("predicate_indicator");
    static Object[] s4 = {s3};
    static /*Structure*/Object s5 = makeStructure(s2, s4);
    static /*Symbol*/Object s6 = makeAtom("listing", 1);
    static /*Symbol*/Object s7 = makeAtom(":", 2);
    static /*IntegerTerm*/Object si8 = makeInteger(1);
    static Predicate _$listing_2_var = new PRED_$listing_2_var();
    static Predicate _$listing_2_var_1 = new PRED_$listing_2_var_1();
    static Predicate _$listing_2_var_2 = new PRED_$listing_2_var_2();
    static Predicate _$listing_2_int = new PRED_$listing_2_int();
    static Predicate _$listing_2_int_1 = new PRED_$listing_2_int_1();
    static Predicate _$listing_2_1 = new PRED_$listing_2_1();
    static Predicate _$listing_2_2 = new PRED_$listing_2_2();
    static Predicate _$listing_2_3 = new PRED_$listing_2_3();

    public Object arg1, arg2;

    public PRED_$listing_2(Object a1, Object a2, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        this.cont = cont;
    }

    public PRED_$listing_2(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        this.cont = cont;
    }

    public int arity() { return 2; }

    public String nameUQ() { return "$listing"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$listing'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine.cont = cont;
        engine.setB0();
        return engine.switch_on_term(_$listing_2_var, _$listing_2_int, _$listing_2_int, _$listing_2_int, _$listing_2_var, _$listing_2_int);
    }
}

class PRED_$listing_2_var extends PRED_$listing_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$listing_2_1, _$listing_2_var_1);
    }
}

class PRED_$listing_2_var_1 extends PRED_$listing_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$listing_2_2, _$listing_2_var_2);
    }
}

class PRED_$listing_2_var_2 extends PRED_$listing_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$listing_2_3);
    }
}

class PRED_$listing_2_int extends PRED_$listing_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$listing_2_1, _$listing_2_int_1);
    }
}

class PRED_$listing_2_int_1 extends PRED_$listing_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$listing_2_3);
    }
}

class PRED_$listing_2_1 extends PRED_$listing_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$listing'(A, B):-var(A), !, '$listing_dynamic_clause'(B, C)
        Object a1, a2, a3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        cont = engine.cont;
    // '$listing'(A, B):-['$get_level'(C), var(A), '$cut'(C), '$listing_dynamic_clause'(B, D)]
        a3 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(3))
        if (! unify(a3,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of var(a(1))
        a1 = deref( a1);
        if (! isVariable(a1)) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of $cut(a(3))
        a3 = deref( a3);
        if (! isCutter/*Integer*/(a3)) {
            throw new IllegalTypeException("integer", a3);
        } else {
            engine.cut(( a3));
        }
        //END inline expansion
        return exit(engine, new PRED_$listing_dynamic_clause_2(a2, engine.makeVariable(this), cont));
    }
}

class PRED_$listing_2_2 extends PRED_$listing_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$listing'(A/B, C):-atom(A), integer(B), !, '$listing_dynamic_clause'(C, A/B)
        Object a1, a2, a3, a4, a5, a6;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        cont = engine.cont;
    // '$listing'(A/B, C):-['$get_level'(D), atom(A), integer(B), '$cut'(D), '$listing_dynamic_clause'(C, A/B)]
        a1 = deref( a1);
        if (isCompound(a1)){
            if (!functorOf( s1,a1))
                return fail(engine);
            Object[] args = args(a1);
            a3 = args[0];
            a4 = args[1];
        } else if (isVariable(a1)){
            a3 = engine.makeVariable(this);
            a4 = engine.makeVariable(this);
            Object[] args = {a3, a4};
            bind(/*VAR*/ a1,makeStructure(s1, args));
        } else {
            return fail(engine);
        }
        a5 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(5))
        if (! unify(a5,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of atom(a(3))
        a3 = deref( a3);
        if (! isAtomTerm(a3)) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of integer(a(4))
        a4 = deref( a4);
        if (! isInteger(a4)) {
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
        Object[] y1 = {a3, a4};
        a6 = makeStructure(s1, y1);
        return exit(engine, new PRED_$listing_dynamic_clause_2(a2, a6, cont));
    }
}

class PRED_$listing_2_3 extends PRED_$listing_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$listing'(A, B):-illarg(type(predicate_indicator), listing(B:A), 1)
        Object a1, a2, a3, a4;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        cont = engine.cont;
    // '$listing'(A, B):-[illarg(type(predicate_indicator), listing(B:A), 1)]
        Object[] y1 = {a2, a1};
        a3 = makeStructure(s7, y1);
        Object[] y2 = {a3};
        a4 = makeStructure(s6, y2);
        return exit(engine, new PRED_illarg_3(s5, a4, si8, cont));
    }
}
