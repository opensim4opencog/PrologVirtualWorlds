package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$rm_redundant_vars'/2</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$rm_redundant_vars_2 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("[]");
    static /*Symbol*/Object s2 = makeAtom("=", 2);
    static /*Symbol*/Object s3 = makeAtom("_");
    static Predicate _$rm_redundant_vars_2_top = new PRED_$rm_redundant_vars_2_top();
    static Predicate _fail_0 = new PRED_fail_0();
    static Predicate _$rm_redundant_vars_2_var = new PRED_$rm_redundant_vars_2_var();
    static Predicate _$rm_redundant_vars_2_var_1 = new PRED_$rm_redundant_vars_2_var_1();
    static Predicate _$rm_redundant_vars_2_var_2 = new PRED_$rm_redundant_vars_2_var_2();
    static Predicate _$rm_redundant_vars_2_lis = new PRED_$rm_redundant_vars_2_lis();
    static Predicate _$rm_redundant_vars_2_lis_1 = new PRED_$rm_redundant_vars_2_lis_1();
    static Predicate _$rm_redundant_vars_2_1 = new PRED_$rm_redundant_vars_2_1();
    static Predicate _$rm_redundant_vars_2_2 = new PRED_$rm_redundant_vars_2_2();
    static Predicate _$rm_redundant_vars_2_3 = new PRED_$rm_redundant_vars_2_3();

    public Object arg1, arg2;

    public PRED_$rm_redundant_vars_2(Object a1, Object a2, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        this.cont = cont;
    }

    public PRED_$rm_redundant_vars_2(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        this.cont = cont;
    }

    public int arity() { return 2; }

    public String nameUQ() { return "$rm_redundant_vars"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$rm_redundant_vars'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine.cont = cont;
        return exit(engine, _$rm_redundant_vars_2_top);
    }
}

class PRED_$rm_redundant_vars_2_top extends PRED_$rm_redundant_vars_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        engine.setB0();
        return engine.switch_on_term(_$rm_redundant_vars_2_var, _fail_0, _fail_0, _$rm_redundant_vars_2_1, _fail_0, _$rm_redundant_vars_2_lis);
    }
}

class PRED_$rm_redundant_vars_2_var extends PRED_$rm_redundant_vars_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$rm_redundant_vars_2_1, _$rm_redundant_vars_2_var_1);
    }
}

class PRED_$rm_redundant_vars_2_var_1 extends PRED_$rm_redundant_vars_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$rm_redundant_vars_2_2, _$rm_redundant_vars_2_var_2);
    }
}

class PRED_$rm_redundant_vars_2_var_2 extends PRED_$rm_redundant_vars_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$rm_redundant_vars_2_3);
    }
}

class PRED_$rm_redundant_vars_2_lis extends PRED_$rm_redundant_vars_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$rm_redundant_vars_2_2, _$rm_redundant_vars_2_lis_1);
    }
}

class PRED_$rm_redundant_vars_2_lis_1 extends PRED_$rm_redundant_vars_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$rm_redundant_vars_2_3);
    }
}

class PRED_$rm_redundant_vars_2_1 extends PRED_$rm_redundant_vars_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$rm_redundant_vars'([], []):-!
        Object a1, a2;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        cont = engine.cont;
    // '$rm_redundant_vars'([], []):-['$neck_cut']
        a1 = deref( a1);
        if (isAtomTerm(a1)){
            if (! prologEquals(a1,s1))
                return fail(engine);
        } else if (isVariable(a1)){
             bind(/*VAR*/ a1,s1);
        } else {
            return fail(engine);
        }
        a2 = deref( a2);
        if (isAtomTerm(a2)){
            if (! prologEquals(a2,s1))
                return fail(engine);
        } else if (isVariable(a2)){
             bind(/*VAR*/ a2,s1);
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        return exit(engine,cont);
    }
}

class PRED_$rm_redundant_vars_2_2 extends PRED_$rm_redundant_vars_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$rm_redundant_vars'(['_'=A|B], C):-!, '$rm_redundant_vars'(B, C)
        Object a1, a2, a3, a4;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        cont = engine.cont;
    // '$rm_redundant_vars'(['_'=A|B], C):-['$neck_cut', '$rm_redundant_vars'(B, C)]
        a1 = deref( a1);
        if (isListTerm(a1)){
            Object[] args = consArgs(a1);
            a3 = args[0];
            a4 = args[1];
        } else if (isVariable(a1)){
            a3 = engine.makeVariable(this);
            a4 = engine.makeVariable(this);
             bind(a1,makeList(a3, a4));
        } else {
            return fail(engine);
        }
        a3 = deref( a3);
        if (isCompound(a3)){
            if (!functorOf( s2,a3))
                return fail(engine);
            Object[] args = args(a3);
            if (!unify( s3,args[0]))
                return fail(engine);
        } else if (isVariable(a3)){
            Object[] args = {s3, engine.makeVariable(this)};
            bind(/*VAR*/ a3,makeStructure(s2, args));
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        engine_aregs[1] = a4;
        engine_aregs[2] = a2;
        engine.cont = cont;
        return exit(engine, _$rm_redundant_vars_2_top);
    }
}

class PRED_$rm_redundant_vars_2_3 extends PRED_$rm_redundant_vars_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$rm_redundant_vars'([A|B], [A|C]):-'$rm_redundant_vars'(B, C)
        Object a1, a2, a3, a4, a5;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        cont = engine.cont;
    // '$rm_redundant_vars'([A|B], [A|C]):-['$rm_redundant_vars'(B, C)]
        a1 = deref( a1);
        if (isListTerm(a1)){
            Object[] args = consArgs(a1);
            a3 = args[0];
            a4 = args[1];
        } else if (isVariable(a1)){
            a3 = engine.makeVariable(this);
            a4 = engine.makeVariable(this);
             bind(a1,makeList(a3, a4));
        } else {
            return fail(engine);
        }
        a2 = deref( a2);
        if (isListTerm(a2)){
            Object[] args = consArgs(a2);
            if (!unify( a3,args[0]))
                return fail(engine);
            a5 = args[1];
        } else if (isVariable(a2)){
            a5 = engine.makeVariable(this);
             bind(a2,makeList(a3, a5));
        } else {
            return fail(engine);
        }
        engine_aregs[1] = a4;
        engine_aregs[2] = a5;
        engine.cont = cont;
        return exit(engine, _$rm_redundant_vars_2_top);
    }
}