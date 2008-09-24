package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$dcg_connect'/4</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$dcg_connect_4 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("->", 2);
    static /*Symbol*/Object s2 = makeAtom(",", 2);
    static Predicate _$dcg_connect_4_var = new PRED_$dcg_connect_4_var();
    static Predicate _$dcg_connect_4_var_1 = new PRED_$dcg_connect_4_var_1();
    static Predicate _$dcg_connect_4_var_2 = new PRED_$dcg_connect_4_var_2();
    static Predicate _$dcg_connect_4_int = new PRED_$dcg_connect_4_int();
    static Predicate _$dcg_connect_4_int_1 = new PRED_$dcg_connect_4_int_1();
    static Predicate _$dcg_connect_4_1 = new PRED_$dcg_connect_4_1();
    static Predicate _$dcg_connect_4_2 = new PRED_$dcg_connect_4_2();
    static Predicate _$dcg_connect_4_3 = new PRED_$dcg_connect_4_3();

    public Object arg1, arg2, arg3, arg4;

    public PRED_$dcg_connect_4(Object a1, Object a2, Object a3, Object a4, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        this.cont = cont;
    }

    public PRED_$dcg_connect_4(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        this.cont = cont;
    }

    public int arity() { return 4; }

    public String nameUQ() { return "$dcg_connect"; }

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
        return "'$dcg_connect'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine.cont = cont;
        engine.setB0();
        return engine.switch_on_term(_$dcg_connect_4_var, _$dcg_connect_4_int, _$dcg_connect_4_int, _$dcg_connect_4_int, _$dcg_connect_4_var, _$dcg_connect_4_int);
    }
}

class PRED_$dcg_connect_4_var extends PRED_$dcg_connect_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$dcg_connect_4_1, _$dcg_connect_4_var_1);
    }
}

class PRED_$dcg_connect_4_var_1 extends PRED_$dcg_connect_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$dcg_connect_4_2, _$dcg_connect_4_var_2);
    }
}

class PRED_$dcg_connect_4_var_2 extends PRED_$dcg_connect_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$dcg_connect_4_3);
    }
}

class PRED_$dcg_connect_4_int extends PRED_$dcg_connect_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$dcg_connect_4_2, _$dcg_connect_4_int_1);
    }
}

class PRED_$dcg_connect_4_int_1 extends PRED_$dcg_connect_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$dcg_connect_4_3);
    }
}

class PRED_$dcg_connect_4_1 extends PRED_$dcg_connect_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dcg_connect'((A->B), (C->D), E, (C->F)):-!, '$dcg_and'(D, E, F)
        Object a1, a2, a3, a4, a5, a6, a7;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$dcg_connect'((A->B), (C->D), E, (C->F)):-['$neck_cut', '$dcg_and'(D, E, F)]
        a1 = deref( a1);
        if (isCompound(a1)){
            if (!functorOf( s1,a1))
                return fail(engine);
            Object[] args = args(a1);
        } else if (isVariable(a1)){
            Object[] args = {engine.makeVariable(this), engine.makeVariable(this)};
            bind(/*VAR*/ a1,makeStructure(s1, args));
        } else {
            return fail(engine);
        }
        a2 = deref( a2);
        if (isCompound(a2)){
            if (!functorOf( s1,a2))
                return fail(engine);
            Object[] args = args(a2);
            a5 = args[0];
            a6 = args[1];
        } else if (isVariable(a2)){
            a5 = engine.makeVariable(this);
            a6 = engine.makeVariable(this);
            Object[] args = {a5, a6};
            bind(/*VAR*/ a2,makeStructure(s1, args));
        } else {
            return fail(engine);
        }
        a4 = deref( a4);
        if (isCompound(a4)){
            if (!functorOf( s1,a4))
                return fail(engine);
            Object[] args = args(a4);
            if (!unify( a5,args[0]))
                return fail(engine);
            a7 = args[1];
        } else if (isVariable(a4)){
            a7 = engine.makeVariable(this);
            Object[] args = {a5, a7};
            bind(/*VAR*/ a4,makeStructure(s1, args));
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        return exit(engine, new PRED_$dcg_and_3(a6, a3, a7, cont));
    }
}

class PRED_$dcg_connect_4_2 extends PRED_$dcg_connect_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dcg_connect'(A, (B->C), D, ((B->C), D)):-!
        Object a1, a2, a3, a4, a5, a6, a7;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$dcg_connect'(A, (B->C), D, ((B->C), D)):-['$neck_cut']
        a2 = deref( a2);
        if (isCompound(a2)){
            if (!functorOf( s1,a2))
                return fail(engine);
            Object[] args = args(a2);
            a5 = args[0];
            a6 = args[1];
        } else if (isVariable(a2)){
            a5 = engine.makeVariable(this);
            a6 = engine.makeVariable(this);
            Object[] args = {a5, a6};
            bind(/*VAR*/ a2,makeStructure(s1, args));
        } else {
            return fail(engine);
        }
        a4 = deref( a4);
        if (isCompound(a4)){
            if (!functorOf( s2,a4))
                return fail(engine);
            Object[] args = args(a4);
            a7 = args[0];
            if (!unify( a3,args[1]))
                return fail(engine);
        } else if (isVariable(a4)){
            a7 = engine.makeVariable(this);
            Object[] args = {a7, a3};
            bind(/*VAR*/ a4,makeStructure(s2, args));
        } else {
            return fail(engine);
        }
        a7 = deref( a7);
        if (isCompound(a7)){
            if (!functorOf( s1,a7))
                return fail(engine);
            Object[] args = args(a7);
            if (!unify( a5,args[0]))
                return fail(engine);
            if (!unify( a6,args[1]))
                return fail(engine);
        } else if (isVariable(a7)){
            Object[] args = {a5, a6};
            bind(/*VAR*/ a7,makeStructure(s1, args));
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        return exit(engine,cont);
    }
}

class PRED_$dcg_connect_4_3 extends PRED_$dcg_connect_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dcg_connect'(A, B, C, D):-'$dcg_and'(B, C, D)
        Object a1, a2, a3, a4;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$dcg_connect'(A, B, C, D):-['$dcg_and'(B, C, D)]
        return exit(engine, new PRED_$dcg_and_3(a2, a3, a4, cont));
    }
}