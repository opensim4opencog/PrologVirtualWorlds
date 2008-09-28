package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$get_indices'/4</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$get_indices_4 extends PredicateBase {

    public Object arg1, arg2, arg3, arg4;

    public PRED_$get_indices_4(Object a1, Object a2, Object a3, Object a4, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        this.cont = cont;
    }

    public PRED_$get_indices_4(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        this.cont = cont;
    }

    public int arity() { return 4; }

    public String nameUQ() { return "$get_indices"; }

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
        return "'$get_indices'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
    // '$get_indices'(A, B, C, D):-'$new_indexing_hash'(A, B, E), '$calc_indexing_key'(C, F), '$dummy_6_builtins.pl'(D, E, F)
        engine.setB0();
        Object a1, a2, a3, a4, a5, a6;
        Predicate p1, p2;
        a1 = arg1;
        a2 = arg2;
        a3 = arg3;
        a4 = arg4;
    // '$get_indices'(A, B, C, D):-['$new_indexing_hash'(A, B, E), '$calc_indexing_key'(C, F), '$dummy_6_builtins.pl'(D, E, F)]
        a5 = engine.makeVariable(this);
        a6 = engine.makeVariable(this);
        p1 = new PRED_$dummy_6_builtins$002Epl_3(a4, a5, a6, cont);
        p2 = new PRED_$calc_indexing_key_2(a3, a6, p1);
        return exit(engine, new PRED_$new_indexing_hash_3(a1, a2, a5, p2));
    }
}