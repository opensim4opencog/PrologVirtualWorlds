package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$dummy_14_builtins.pl'/1</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$dummy_14_builtins$002Epl_1 extends PredicateBase {
    static Predicate _$dummy_14_builtins$002Epl_1_sub_1 = new PRED_$dummy_14_builtins$002Epl_1_sub_1();
    static Predicate _$dummy_14_builtins$002Epl_1_1 = new PRED_$dummy_14_builtins$002Epl_1_1();
    static Predicate _$dummy_14_builtins$002Epl_1_2 = new PRED_$dummy_14_builtins$002Epl_1_2();

    public Object arg1;

    public PRED_$dummy_14_builtins$002Epl_1(Object a1, Predicate cont) {
        arg1 = a1;
        this.cont = cont;
    }

    public PRED_$dummy_14_builtins$002Epl_1(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        this.cont = cont;
    }

    public int arity() { return 1; }

    public String nameUQ() { return "$dummy_14_builtins.pl"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$dummy_14_builtins.pl'(" + argString(arg1,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine.cont = cont;
        engine.setB0();
        return engine.jtry(_$dummy_14_builtins$002Epl_1_1, _$dummy_14_builtins$002Epl_1_sub_1);
    }
}

class PRED_$dummy_14_builtins$002Epl_1_sub_1 extends PRED_$dummy_14_builtins$002Epl_1 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$dummy_14_builtins$002Epl_1_2);
    }
}

class PRED_$dummy_14_builtins$002Epl_1_1 extends PRED_$dummy_14_builtins$002Epl_1 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dummy_14_builtins.pl'(A):-'$parse_tokens_is_post_in_op'(A), !, fail
        Object a1, a2;
        Predicate p1, p2;
        Predicate cont;
        a1 = engine_aregs[1];
        cont = engine.cont;
    // '$dummy_14_builtins.pl'(A):-['$get_level'(B), '$parse_tokens_is_post_in_op'(A), '$cut'(B), fail]
        a2 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(2))
        if (! unify(a2,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        p1 = new PRED_fail_0(cont);
        p2 = new PRED_$cut_1(a2, p1);
        return exit(engine, new PRED_$parse_tokens_is_post_in_op_1(a1, p2));
    }
}

class PRED_$dummy_14_builtins$002Epl_1_2 extends PRED_$dummy_14_builtins$002Epl_1 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dummy_14_builtins.pl'(A):-true
        Object a1;
        Predicate cont;
        a1 = engine_aregs[1];
        cont = engine.cont;
    // '$dummy_14_builtins.pl'(A):-[]
        return exit(engine,cont);
    }
}
