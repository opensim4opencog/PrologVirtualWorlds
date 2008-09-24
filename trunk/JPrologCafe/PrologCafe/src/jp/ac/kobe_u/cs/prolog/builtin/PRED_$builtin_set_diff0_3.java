package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$builtin_set_diff0'/3</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$builtin_set_diff0_3 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("[]");
    static Predicate _$builtin_set_diff0_3_top = new PRED_$builtin_set_diff0_3_top();
    static Predicate _$builtin_set_diff0_3_var = new PRED_$builtin_set_diff0_3_var();
    static Predicate _$builtin_set_diff0_3_var_1 = new PRED_$builtin_set_diff0_3_var_1();
    static Predicate _$builtin_set_diff0_3_var_2 = new PRED_$builtin_set_diff0_3_var_2();
    static Predicate _$builtin_set_diff0_3_var_3 = new PRED_$builtin_set_diff0_3_var_3();
    static Predicate _$builtin_set_diff0_3_var_4 = new PRED_$builtin_set_diff0_3_var_4();
    static Predicate _$builtin_set_diff0_3_con = new PRED_$builtin_set_diff0_3_con();
    static Predicate _$builtin_set_diff0_3_con_1 = new PRED_$builtin_set_diff0_3_con_1();
    static Predicate _$builtin_set_diff0_3_lis = new PRED_$builtin_set_diff0_3_lis();
    static Predicate _$builtin_set_diff0_3_lis_1 = new PRED_$builtin_set_diff0_3_lis_1();
    static Predicate _$builtin_set_diff0_3_lis_2 = new PRED_$builtin_set_diff0_3_lis_2();
    static Predicate _$builtin_set_diff0_3_lis_3 = new PRED_$builtin_set_diff0_3_lis_3();
    static Predicate _$builtin_set_diff0_3_1 = new PRED_$builtin_set_diff0_3_1();
    static Predicate _$builtin_set_diff0_3_2 = new PRED_$builtin_set_diff0_3_2();
    static Predicate _$builtin_set_diff0_3_3 = new PRED_$builtin_set_diff0_3_3();
    static Predicate _$builtin_set_diff0_3_4 = new PRED_$builtin_set_diff0_3_4();
    static Predicate _$builtin_set_diff0_3_5 = new PRED_$builtin_set_diff0_3_5();

    public Object arg1, arg2, arg3;

    public PRED_$builtin_set_diff0_3(Object a1, Object a2, Object a3, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        this.cont = cont;
    }

    public PRED_$builtin_set_diff0_3(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        this.cont = cont;
    }

    public int arity() { return 3; }

    public String nameUQ() { return "$builtin_set_diff0"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
         case 2: arg3 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
         case 2: return arg3;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$builtin_set_diff0'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine.cont = cont;
        return exit(engine, _$builtin_set_diff0_3_top);
    }
}

class PRED_$builtin_set_diff0_3_top extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        engine.setB0();
        return engine.switch_on_term(_$builtin_set_diff0_3_var, _$builtin_set_diff0_3_2, _$builtin_set_diff0_3_2, _$builtin_set_diff0_3_con, _$builtin_set_diff0_3_2, _$builtin_set_diff0_3_lis);
    }
}

class PRED_$builtin_set_diff0_3_var extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$builtin_set_diff0_3_1, _$builtin_set_diff0_3_var_1);
    }
}

class PRED_$builtin_set_diff0_3_var_1 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$builtin_set_diff0_3_2, _$builtin_set_diff0_3_var_2);
    }
}

class PRED_$builtin_set_diff0_3_var_2 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$builtin_set_diff0_3_3, _$builtin_set_diff0_3_var_3);
    }
}

class PRED_$builtin_set_diff0_3_var_3 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$builtin_set_diff0_3_4, _$builtin_set_diff0_3_var_4);
    }
}

class PRED_$builtin_set_diff0_3_var_4 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$builtin_set_diff0_3_5);
    }
}

class PRED_$builtin_set_diff0_3_con extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$builtin_set_diff0_3_1, _$builtin_set_diff0_3_con_1);
    }
}

class PRED_$builtin_set_diff0_3_con_1 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$builtin_set_diff0_3_2);
    }
}

class PRED_$builtin_set_diff0_3_lis extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$builtin_set_diff0_3_2, _$builtin_set_diff0_3_lis_1);
    }
}

class PRED_$builtin_set_diff0_3_lis_1 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$builtin_set_diff0_3_3, _$builtin_set_diff0_3_lis_2);
    }
}

class PRED_$builtin_set_diff0_3_lis_2 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$builtin_set_diff0_3_4, _$builtin_set_diff0_3_lis_3);
    }
}

class PRED_$builtin_set_diff0_3_lis_3 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$builtin_set_diff0_3_5);
    }
}

class PRED_$builtin_set_diff0_3_1 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$builtin_set_diff0'([], A, []):-!
        Object a1, a2, a3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$builtin_set_diff0'([], A, []):-['$neck_cut']
        a1 = deref( a1);
        if (isAtomTerm(a1)){
            if (! prologEquals(a1,s1))
                return fail(engine);
        } else if (isVariable(a1)){
             bind(/*VAR*/ a1,s1);
        } else {
            return fail(engine);
        }
        a3 = deref( a3);
        if (isAtomTerm(a3)){
            if (! prologEquals(a3,s1))
                return fail(engine);
        } else if (isVariable(a3)){
             bind(/*VAR*/ a3,s1);
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        return exit(engine,cont);
    }
}

class PRED_$builtin_set_diff0_3_2 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$builtin_set_diff0'(A, [], A):-!
        Object a1, a2, a3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$builtin_set_diff0'(A, [], A):-['$neck_cut']
        a2 = deref( a2);
        if (isAtomTerm(a2)){
            if (! prologEquals(a2,s1))
                return fail(engine);
        } else if (isVariable(a2)){
             bind(/*VAR*/ a2,s1);
        } else {
            return fail(engine);
        }
        if (! unify(a1,a3))
            return fail(engine);
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        return exit(engine,cont);
    }
}

class PRED_$builtin_set_diff0_3_3 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$builtin_set_diff0'([A|B], [C|D], E):-A==C, !, '$builtin_set_diff0'(B, D, E)
        Object a1, a2, a3, a4, a5, a6, a7, a8;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$builtin_set_diff0'([A|B], [C|D], E):-['$get_level'(F), '$equality_of_term'(A, C), '$cut'(F), '$builtin_set_diff0'(B, D, E)]
        a1 = deref( a1);
        if (isListTerm(a1)){
            Object[] args = consArgs(a1);
            a4 = args[0];
            a5 = args[1];
        } else if (isVariable(a1)){
            a4 = engine.makeVariable(this);
            a5 = engine.makeVariable(this);
             bind(a1,makeList(a4, a5));
        } else {
            return fail(engine);
        }
        a2 = deref( a2);
        if (isListTerm(a2)){
            Object[] args = consArgs(a2);
            a6 = args[0];
            a7 = args[1];
        } else if (isVariable(a2)){
            a6 = engine.makeVariable(this);
            a7 = engine.makeVariable(this);
             bind(a2,makeList(a6, a7));
        } else {
            return fail(engine);
        }
        a8 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(8))
        if (! unify(a8,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of $equality_of_term(a(4), a(6))
        a4 = deref( a4);
        a6 = deref( a6);
        if (! prologEquals(a4,a6)) {
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
        engine_aregs[1] = a5;
        engine_aregs[2] = a7;
        engine_aregs[3] = a3;
        engine.cont = cont;
        return exit(engine, _$builtin_set_diff0_3_top);
    }
}

class PRED_$builtin_set_diff0_3_4 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$builtin_set_diff0'([A|B], [C|D], [A|E]):-A@<C, !, '$builtin_set_diff0'(B, [C|D], E)
        Object a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$builtin_set_diff0'([A|B], [C|D], [A|E]):-['$get_level'(F), '$before'(A, C), '$cut'(F), '$builtin_set_diff0'(B, [C|D], E)]
        a1 = deref( a1);
        if (isListTerm(a1)){
            Object[] args = consArgs(a1);
            a4 = args[0];
            a5 = args[1];
        } else if (isVariable(a1)){
            a4 = engine.makeVariable(this);
            a5 = engine.makeVariable(this);
             bind(a1,makeList(a4, a5));
        } else {
            return fail(engine);
        }
        a2 = deref( a2);
        if (isListTerm(a2)){
            Object[] args = consArgs(a2);
            a6 = args[0];
            a7 = args[1];
        } else if (isVariable(a2)){
            a6 = engine.makeVariable(this);
            a7 = engine.makeVariable(this);
             bind(a2,makeList(a6, a7));
        } else {
            return fail(engine);
        }
        a3 = deref( a3);
        if (isListTerm(a3)){
            Object[] args = consArgs(a3);
            if (!unify( a4,args[0]))
                return fail(engine);
            a8 = args[1];
        } else if (isVariable(a3)){
            a8 = engine.makeVariable(this);
             bind(a3,makeList(a4, a8));
        } else {
            return fail(engine);
        }
        a9 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(9))
        if (! unify(a9,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of $before(a(4), a(6))
        a4 = deref( a4);
        a6 = deref( a6);
        if (compareTerm(a4,a6) >= 0) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of $cut(a(9))
        a9 = deref( a9);
        if (! isCutter/*Integer*/(a9)) {
            throw new IllegalTypeException("integer", a9);
        } else {
            engine.cut(( a9));
        }
        //END inline expansion
        a10 = makeList(a6, a7);
        engine_aregs[1] = a5;
        engine_aregs[2] = a10;
        engine_aregs[3] = a8;
        engine.cont = cont;
        return exit(engine, _$builtin_set_diff0_3_top);
    }
}

class PRED_$builtin_set_diff0_3_5 extends PRED_$builtin_set_diff0_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$builtin_set_diff0'([A|B], [C|D], [C|E]):-'$builtin_set_diff0'([A|B], D, [C|E])
        Object a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$builtin_set_diff0'([A|B], [C|D], [C|E]):-['$builtin_set_diff0'([A|B], D, [C|E])]
        a1 = deref( a1);
        if (isListTerm(a1)){
            Object[] args = consArgs(a1);
            a4 = args[0];
            a5 = args[1];
        } else if (isVariable(a1)){
            a4 = engine.makeVariable(this);
            a5 = engine.makeVariable(this);
             bind(a1,makeList(a4, a5));
        } else {
            return fail(engine);
        }
        a2 = deref( a2);
        if (isListTerm(a2)){
            Object[] args = consArgs(a2);
            a6 = args[0];
            a7 = args[1];
        } else if (isVariable(a2)){
            a6 = engine.makeVariable(this);
            a7 = engine.makeVariable(this);
             bind(a2,makeList(a6, a7));
        } else {
            return fail(engine);
        }
        a3 = deref( a3);
        if (isListTerm(a3)){
            Object[] args = consArgs(a3);
            if (!unify( a6,args[0]))
                return fail(engine);
            a8 = args[1];
        } else if (isVariable(a3)){
            a8 = engine.makeVariable(this);
             bind(a3,makeList(a6, a8));
        } else {
            return fail(engine);
        }
        a9 = makeList(a4, a5);
        a10 = makeList(a6, a8);
        engine_aregs[1] = a9;
        engine_aregs[2] = a7;
        engine_aregs[3] = a10;
        engine.cont = cont;
        return exit(engine, _$builtin_set_diff0_3_top);
    }
}
