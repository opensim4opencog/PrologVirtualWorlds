package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$dcg_translate_dcg_atom'/4</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$dcg_translate_dcg_atom_4 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("phrase", 3);
    static /*Symbol*/Object s2 = makeAtom(":", 2);
    static /*IntegerTerm*/Object si3 = makeInteger(1);
    static /*IntegerTerm*/Object si4 = makeInteger(2);
    static Predicate _$dcg_translate_dcg_atom_4_top = new PRED_$dcg_translate_dcg_atom_4_top();
    static Predicate _$dcg_translate_dcg_atom_4_var = new PRED_$dcg_translate_dcg_atom_4_var();
    static Predicate _$dcg_translate_dcg_atom_4_var_1 = new PRED_$dcg_translate_dcg_atom_4_var_1();
    static Predicate _$dcg_translate_dcg_atom_4_var_2 = new PRED_$dcg_translate_dcg_atom_4_var_2();
    static Predicate _$dcg_translate_dcg_atom_4_int = new PRED_$dcg_translate_dcg_atom_4_int();
    static Predicate _$dcg_translate_dcg_atom_4_int_1 = new PRED_$dcg_translate_dcg_atom_4_int_1();
    static Predicate _$dcg_translate_dcg_atom_4_1 = new PRED_$dcg_translate_dcg_atom_4_1();
    static Predicate _$dcg_translate_dcg_atom_4_2 = new PRED_$dcg_translate_dcg_atom_4_2();
    static Predicate _$dcg_translate_dcg_atom_4_3 = new PRED_$dcg_translate_dcg_atom_4_3();

    public Object arg1, arg2, arg3, arg4;

    public PRED_$dcg_translate_dcg_atom_4(Object a1, Object a2, Object a3, Object a4, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        this.cont = cont;
    }

    public PRED_$dcg_translate_dcg_atom_4(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        this.cont = cont;
    }

    public int arity() { return 4; }

    public String nameUQ() { return "$dcg_translate_dcg_atom"; }

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
        return "'$dcg_translate_dcg_atom'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine.cont = cont;
        return exit(engine, _$dcg_translate_dcg_atom_4_top);
    }
}

class PRED_$dcg_translate_dcg_atom_4_top extends PRED_$dcg_translate_dcg_atom_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        engine.setB0();
        return engine.switch_on_term(_$dcg_translate_dcg_atom_4_var, _$dcg_translate_dcg_atom_4_int, _$dcg_translate_dcg_atom_4_int, _$dcg_translate_dcg_atom_4_int, _$dcg_translate_dcg_atom_4_var, _$dcg_translate_dcg_atom_4_int);
    }
}

class PRED_$dcg_translate_dcg_atom_4_var extends PRED_$dcg_translate_dcg_atom_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$dcg_translate_dcg_atom_4_1, _$dcg_translate_dcg_atom_4_var_1);
    }
}

class PRED_$dcg_translate_dcg_atom_4_var_1 extends PRED_$dcg_translate_dcg_atom_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$dcg_translate_dcg_atom_4_2, _$dcg_translate_dcg_atom_4_var_2);
    }
}

class PRED_$dcg_translate_dcg_atom_4_var_2 extends PRED_$dcg_translate_dcg_atom_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$dcg_translate_dcg_atom_4_3);
    }
}

class PRED_$dcg_translate_dcg_atom_4_int extends PRED_$dcg_translate_dcg_atom_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$dcg_translate_dcg_atom_4_1, _$dcg_translate_dcg_atom_4_int_1);
    }
}

class PRED_$dcg_translate_dcg_atom_4_int_1 extends PRED_$dcg_translate_dcg_atom_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$dcg_translate_dcg_atom_4_3);
    }
}

class PRED_$dcg_translate_dcg_atom_4_1 extends PRED_$dcg_translate_dcg_atom_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dcg_translate_dcg_atom'(A, B, C, D):-var(A), !, B=phrase(A, C, D)
        Object a1, a2, a3, a4, a5, a6;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$dcg_translate_dcg_atom'(A, B, C, D):-['$get_level'(E), var(A), '$cut'(E), '$unify'(B, phrase(A, C, D))]
        a5 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(5))
        if (! unify(a5,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of var(a(1))
        a1 = deref( a1);
        if (! isVariable(a1)) {
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
        Object[] y1 = {a1, a3, a4};
        a6 = makeStructure(s1, y1);
        //START inline expansion of $unify(a(2), a(6))
        if (! unify(a2,a6)) {
            return fail(engine);
        }
        //END inline expansion
        return exit(engine,cont);
    }
}

class PRED_$dcg_translate_dcg_atom_4_2 extends PRED_$dcg_translate_dcg_atom_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dcg_translate_dcg_atom'(A:B, A:C, D, E):-!, '$dcg_translate_dcg_atom'(B, C, D, E)
        Object a1, a2, a3, a4, a5, a6, a7;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$dcg_translate_dcg_atom'(A:B, A:C, D, E):-['$neck_cut', '$dcg_translate_dcg_atom'(B, C, D, E)]
        a1 = deref( a1);
        if (isCompound(a1)){
            if (!functorOf( s2,a1))
                return fail(engine);
            Object[] args = args(a1);
            a5 = args[0];
            a6 = args[1];
        } else if (isVariable(a1)){
            a5 = engine.makeVariable(this);
            a6 = engine.makeVariable(this);
            Object[] args = {a5, a6};
            bind(/*VAR*/ a1,makeStructure(s2, args));
        } else {
            return fail(engine);
        }
        a2 = deref( a2);
        if (isCompound(a2)){
            if (!functorOf( s2,a2))
                return fail(engine);
            Object[] args = args(a2);
            if (!unify( a5,args[0]))
                return fail(engine);
            a7 = args[1];
        } else if (isVariable(a2)){
            a7 = engine.makeVariable(this);
            Object[] args = {a5, a7};
            bind(/*VAR*/ a2,makeStructure(s2, args));
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        engine_aregs[1] = a6;
        engine_aregs[2] = a7;
        engine_aregs[3] = a3;
        engine_aregs[4] = a4;
        engine.cont = cont;
        return exit(engine, _$dcg_translate_dcg_atom_4_top);
    }
}

class PRED_$dcg_translate_dcg_atom_4_3 extends PRED_$dcg_translate_dcg_atom_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dcg_translate_dcg_atom'(A, B, C, D):-functor(A, E, F), G is F+1, H is F+2, functor(B, E, H), arg(G, B, C), arg(H, B, D), '$dcg_copy_args'(F, A, B)
        Object a1, a2, a3, a4, a5, a6, a7, a8;
        Predicate p1, p2, p3, p4, p5, p6;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$dcg_translate_dcg_atom'(A, B, C, D):-[functor(A, E, F), '$plus'(F, 1, G), '$plus'(F, 2, H), functor(B, E, H), arg(G, B, C), arg(H, B, D), '$dcg_copy_args'(F, A, B)]
        a5 = engine.makeVariable(this);
        a6 = engine.makeVariable(this);
        a7 = engine.makeVariable(this);
        a8 = engine.makeVariable(this);
        p1 = new PRED_$dcg_copy_args_3(a6, a1, a2, cont);
        p2 = new PRED_arg_3(a8, a2, a4, p1);
        p3 = new PRED_arg_3(a7, a2, a3, p2);
        p4 = new PRED_functor_3(a2, a5, a8, p3);
        p5 = new PRED_$plus_3(a6, si4, a8, p4);
        p6 = new PRED_$plus_3(a6, si3, a7, p5);
        return exit(engine, new PRED_functor_3(a1, a5, a6, p6));
    }
}