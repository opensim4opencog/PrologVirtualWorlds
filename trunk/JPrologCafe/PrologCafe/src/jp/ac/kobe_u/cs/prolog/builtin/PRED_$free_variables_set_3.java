package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$free_variables_set'/3</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$free_variables_set_3 extends PredicateBase {

    public Object arg1, arg2, arg3;

    public PRED_$free_variables_set_3(Object a1, Object a2, Object a3, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        this.cont = cont;
    }

    public PRED_$free_variables_set_3(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        this.cont = cont;
    }

    public int arity() { return 3; }

    public String nameUQ() { return "$free_variables_set"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
         case 2: arg3 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
         case 2: return arg3;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$free_variables_set'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
    // '$free_variables_set'(A, B, C):-'$variables_set'(A, D), '$variables_set'(B, E), '$existential_variables_set'(A, E, F), '$builtin_set_diff'(D, F, C), !
        engine.setB0();
        Object a1, a2, a3, a4, a5, a6, a7;
        Predicate p1, p2, p3, p4;
        a1 = arg1;
        a2 = arg2;
        a3 = arg3;
    // '$free_variables_set'(A, B, C):-['$get_level'(D), '$variables_set'(A, E), '$variables_set'(B, F), '$existential_variables_set'(A, F, G), '$builtin_set_diff'(E, G, C), '$cut'(D)]
        a4 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(4))
        if (! unify(a4,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        a5 = engine.makeVariable(this);
        a6 = engine.makeVariable(this);
        a7 = engine.makeVariable(this);
        p1 = new PRED_$cut_1(a4, cont);
        p2 = new PRED_$builtin_set_diff_3(a5, a7, a3, p1);
        p3 = new PRED_$existential_variables_set_3(a1, a6, a7, p2);
        p4 = new PRED_$variables_set_2(a2, a6, p3);
        return exit(engine, new PRED_$variables_set_2(a1, a5, p4));
    }
}
