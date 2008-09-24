package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$dummy_23_builtins.pl'/4</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$dummy_23_builtins$002Epl_4 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("domain", 2);
    static /*Symbol*/Object s2 = makeAtom("atom");
    static /*Symbol*/Object s3 = makeAtom("prolog_flag");
    static Object[] s4 = {s2, s3};
    static /*Structure*/Object s5 = makeStructure(s1, s4);
    static /*Symbol*/Object s6 = makeAtom("current_prolog_flag", 2);
    static /*IntegerTerm*/Object si7 = makeInteger(1);
    static Predicate _$dummy_23_builtins$002Epl_4_sub_1 = new PRED_$dummy_23_builtins$002Epl_4_sub_1();
    static Predicate _$dummy_23_builtins$002Epl_4_1 = new PRED_$dummy_23_builtins$002Epl_4_1();
    static Predicate _$dummy_23_builtins$002Epl_4_2 = new PRED_$dummy_23_builtins$002Epl_4_2();

    public Object arg1, arg2, arg3, arg4;

    public PRED_$dummy_23_builtins$002Epl_4(Object a1, Object a2, Object a3, Object a4, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        this.cont = cont;
    }

    public PRED_$dummy_23_builtins$002Epl_4(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        this.cont = cont;
    }

    public int arity() { return 4; }

    public String nameUQ() { return "$dummy_23_builtins.pl"; }

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
        return "'$dummy_23_builtins.pl'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine.cont = cont;
        engine.setB0();
        return engine.jtry(_$dummy_23_builtins$002Epl_4_1, _$dummy_23_builtins$002Epl_4_sub_1);
    }
}

class PRED_$dummy_23_builtins$002Epl_4_sub_1 extends PRED_$dummy_23_builtins$002Epl_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$dummy_23_builtins$002Epl_4_2);
    }
}

class PRED_$dummy_23_builtins$002Epl_4_1 extends PRED_$dummy_23_builtins$002Epl_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dummy_23_builtins.pl'(A, B, C, D):-'$prolog_impl_flag'(A, C, D), !, '$get_prolog_impl_flag'(A, B)
        Object a1, a2, a3, a4, a5;
        Predicate p1, p2;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$dummy_23_builtins.pl'(A, B, C, D):-['$get_level'(E), '$prolog_impl_flag'(A, C, D), '$cut'(E), '$get_prolog_impl_flag'(A, B)]
        a5 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(5))
        if (! unify(a5,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        p1 = new PRED_$get_prolog_impl_flag_2(a1, a2, cont);
        p2 = new PRED_$cut_1(a5, p1);
        return exit(engine, new PRED_$prolog_impl_flag_3(a1, a3, a4, p2));
    }
}

class PRED_$dummy_23_builtins$002Epl_4_2 extends PRED_$dummy_23_builtins$002Epl_4 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dummy_23_builtins.pl'(A, B, C, D):-illarg(domain(atom, prolog_flag), current_prolog_flag(A, B), 1)
        Object a1, a2, a3, a4, a5;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        cont = engine.cont;
    // '$dummy_23_builtins.pl'(A, B, C, D):-[illarg(domain(atom, prolog_flag), current_prolog_flag(A, B), 1)]
        Object[] y1 = {a1, a2};
        a5 = makeStructure(s6, y1);
        return exit(engine, new PRED_illarg_3(s5, a5, si7, cont));
    }
}
