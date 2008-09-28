package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$dummy_19_builtins.pl'/2</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$dummy_19_builtins$002Epl_2 extends PredicateBase {
    static Predicate _$dummy_19_builtins$002Epl_2_sub_1 = new PRED_$dummy_19_builtins$002Epl_2_sub_1();
    static Predicate _$dummy_19_builtins$002Epl_2_1 = new PRED_$dummy_19_builtins$002Epl_2_1();
    static Predicate _$dummy_19_builtins$002Epl_2_2 = new PRED_$dummy_19_builtins$002Epl_2_2();

    public Object arg1, arg2;

    public PRED_$dummy_19_builtins$002Epl_2(Object a1, Object a2, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        this.cont = cont;
    }

    public PRED_$dummy_19_builtins$002Epl_2(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        this.cont = cont;
    }

    public int arity() { return 2; }

    public String nameUQ() { return "$dummy_19_builtins.pl"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$dummy_19_builtins.pl'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine.cont = cont;
        engine.setB0();
        return engine.jtry(_$dummy_19_builtins$002Epl_2_1, _$dummy_19_builtins$002Epl_2_sub_1);
    }
}

class PRED_$dummy_19_builtins$002Epl_2_sub_1 extends PRED_$dummy_19_builtins$002Epl_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$dummy_19_builtins$002Epl_2_2);
    }
}

class PRED_$dummy_19_builtins$002Epl_2_1 extends PRED_$dummy_19_builtins$002Epl_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dummy_19_builtins.pl'(A, B):-number(A), !, number_codes(A, B)
        Object a1, a2, a3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        cont = engine.cont;
    // '$dummy_19_builtins.pl'(A, B):-['$get_level'(C), number(A), '$cut'(C), number_codes(A, B)]
        a3 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(3))
        if (! unify(a3,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of number(a(1))
        a1 = deref( a1);
        if (! isNumber(a1)) {
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
        return exit(engine, new PRED_number_codes_2(a1, a2, cont));
    }
}

class PRED_$dummy_19_builtins$002Epl_2_2 extends PRED_$dummy_19_builtins$002Epl_2 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dummy_19_builtins.pl'(A, B):-'$dummy_20_builtins.pl'(A, B)
        Object a1, a2;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        cont = engine.cont;
    // '$dummy_19_builtins.pl'(A, B):-['$dummy_20_builtins.pl'(A, B)]
        return exit(engine, new PRED_$dummy_20_builtins$002Epl_2(a1, a2, cont));
    }
}