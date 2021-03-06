package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$write_term_list_args'/5</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$write_term_list_args_5 extends PredicateBase {
    static /*IntegerTerm*/Object si1 = makeInteger(999);
    static /*Symbol*/Object s2 = makeAtom("punct");
    static /*Symbol*/Object s3 = makeAtom(",");
    static /*Symbol*/Object s4 = makeAtom("[]");
    static /*Symbol*/Object s5 = makeAtom("|");
    static Predicate _fail_0 = new PRED_fail_0();
    static Predicate _$write_term_list_args_5_var = new PRED_$write_term_list_args_5_var();
    static Predicate _$write_term_list_args_5_var_1 = new PRED_$write_term_list_args_5_var_1();
    static Predicate _$write_term_list_args_5_var_2 = new PRED_$write_term_list_args_5_var_2();
    static Predicate _$write_term_list_args_5_1 = new PRED_$write_term_list_args_5_1();
    static Predicate _$write_term_list_args_5_2 = new PRED_$write_term_list_args_5_2();
    static Predicate _$write_term_list_args_5_3 = new PRED_$write_term_list_args_5_3();

    public Object arg1, arg2, arg3, arg4, arg5;

    public PRED_$write_term_list_args_5(Object a1, Object a2, Object a3, Object a4, Object a5, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        arg5 = a5;
        this.cont = cont;
    }

    public PRED_$write_term_list_args_5(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        arg5 = args[4];
        this.cont = cont;
    }

    public int arity() { return 5; }

    public String nameUQ() { return "$write_term_list_args"; }

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
        return "'$write_term_list_args'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + "," + argString(arg5,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine_aregs[5] = arg5;
        engine.cont = cont;
        engine.setB0();
        return engine.switch_on_term(_$write_term_list_args_5_var, _fail_0, _fail_0, _fail_0, _fail_0, _$write_term_list_args_5_var);
    }
}

class PRED_$write_term_list_args_5_var extends PRED_$write_term_list_args_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$write_term_list_args_5_1, _$write_term_list_args_5_var_1);
    }
}

class PRED_$write_term_list_args_5_var_1 extends PRED_$write_term_list_args_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$write_term_list_args_5_2, _$write_term_list_args_5_var_2);
    }
}

class PRED_$write_term_list_args_5_var_2 extends PRED_$write_term_list_args_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$write_term_list_args_5_3);
    }
}

class PRED_$write_term_list_args_5_1 extends PRED_$write_term_list_args_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$write_term_list_args'([A|B], C, D, E, F):-nonvar(B), B=[G|H], !, '$write_term0'(A, 999, C, I, E, F), '$write_space_if_needed'(I, punct, F), put_char(F, (',')), '$write_term_list_args'(B, punct, D, E, F)
        Object a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
        Predicate p1, p2, p3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        a5 = engine_aregs[5];
        cont = engine.cont;
    // '$write_term_list_args'([A|B], C, D, E, F):-['$get_level'(G), nonvar(B), '$unify'(B, [H|I]), '$cut'(G), '$write_term0'(A, 999, C, J, E, F), '$write_space_if_needed'(J, punct, F), put_char(F, (',')), '$write_term_list_args'(B, punct, D, E, F)]
        a1 = deref( a1);
        if (isListTerm(a1)){
            Object[] args = consArgs(a1);
            a6 = args[0];
            a7 = args[1];
        } else if (isVariable(a1)){
            a6 = engine.makeVariable(this);
            a7 = engine.makeVariable(this);
             bind(a1,makeList(a6, a7));
        } else {
            return fail(engine);
        }
        a8 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(8))
        if (! unify(a8,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of nonvar(a(7))
        a7 = deref( a7);
        if (isVariable(a7)) {
            return fail(engine);
        }
        //END inline expansion
        a9 = makeList(engine.makeVariable(this), engine.makeVariable(this));
        //START inline expansion of $unify(a(7), a(9))
        if (! unify(a7,a9)) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of $cut(a(8))
        a8 = deref( a8);
        if (! isCutter/*Integer*/(a8)) {
            throw new IllegalTypeException("integer", a8);
        } else {
            engine.cut(( a8));
        }
        //END inline expansion
        a10 = engine.makeVariable(this);
        p1 = new PRED_$write_term_list_args_5(a7, s2, a3, a4, a5, cont);
        p2 = new PRED_put_char_2(a5, s3, p1);
        p3 = new PRED_$write_space_if_needed_3(a10, s2, a5, p2);
        return exit(engine, new PRED_$write_term0_6(a6, si1, a2, a10, a4, a5, p3));
    }
}

class PRED_$write_term_list_args_5_2 extends PRED_$write_term_list_args_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$write_term_list_args'([A|B], C, D, E, F):-nonvar(B), B=[], !, '$write_term0'(A, 999, C, D, E, F)
        Object a1, a2, a3, a4, a5, a6, a7, a8;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        a5 = engine_aregs[5];
        cont = engine.cont;
    // '$write_term_list_args'([A|B], C, D, E, F):-['$get_level'(G), nonvar(B), '$unify'(B, []), '$cut'(G), '$write_term0'(A, 999, C, D, E, F)]
        a1 = deref( a1);
        if (isListTerm(a1)){
            Object[] args = consArgs(a1);
            a6 = args[0];
            a7 = args[1];
        } else if (isVariable(a1)){
            a6 = engine.makeVariable(this);
            a7 = engine.makeVariable(this);
             bind(a1,makeList(a6, a7));
        } else {
            return fail(engine);
        }
        a8 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(8))
        if (! unify(a8,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of nonvar(a(7))
        a7 = deref( a7);
        if (isVariable(a7)) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of $unify(a(7), s(4))
        if (! unify(a7,s4)) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of $cut(a(8))
        a8 = deref( a8);
        if (! isCutter/*Integer*/(a8)) {
            throw new IllegalTypeException("integer", a8);
        } else {
            engine.cut(( a8));
        }
        //END inline expansion
        return exit(engine, new PRED_$write_term0_6(a6, si1, a2, a3, a4, a5, cont));
    }
}

class PRED_$write_term_list_args_5_3 extends PRED_$write_term_list_args_5 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$write_term_list_args'([A|B], C, D, E, F):-'$write_term0'(A, 999, C, G, E, F), '$write_space_if_needed'(G, punct, F), put_char(F, ('|')), '$write_term0'(B, 999, punct, D, E, F)
        Object a1, a2, a3, a4, a5, a6, a7, a8;
        Predicate p1, p2, p3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        a5 = engine_aregs[5];
        cont = engine.cont;
    // '$write_term_list_args'([A|B], C, D, E, F):-['$write_term0'(A, 999, C, G, E, F), '$write_space_if_needed'(G, punct, F), put_char(F, ('|')), '$write_term0'(B, 999, punct, D, E, F)]
        a1 = deref( a1);
        if (isListTerm(a1)){
            Object[] args = consArgs(a1);
            a6 = args[0];
            a7 = args[1];
        } else if (isVariable(a1)){
            a6 = engine.makeVariable(this);
            a7 = engine.makeVariable(this);
             bind(a1,makeList(a6, a7));
        } else {
            return fail(engine);
        }
        a8 = engine.makeVariable(this);
        p1 = new PRED_$write_term0_6(a7, si1, s2, a3, a4, a5, cont);
        p2 = new PRED_put_char_2(a5, s5, p1);
        p3 = new PRED_$write_space_if_needed_3(a8, s2, a5, p2);
        return exit(engine, new PRED_$write_term0_6(a6, si1, a2, a8, a4, a5, p3));
    }
}
