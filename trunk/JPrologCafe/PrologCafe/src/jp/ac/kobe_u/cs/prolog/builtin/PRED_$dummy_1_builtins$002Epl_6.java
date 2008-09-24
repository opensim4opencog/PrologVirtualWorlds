package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$dummy_1_builtins.pl'/6</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$dummy_1_builtins$002Epl_6 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("no");
    static Predicate _$dummy_1_builtins$002Epl_6_sub_1 = new PRED_$dummy_1_builtins$002Epl_6_sub_1();
    static Predicate _$dummy_1_builtins$002Epl_6_1 = new PRED_$dummy_1_builtins$002Epl_6_1();
    static Predicate _$dummy_1_builtins$002Epl_6_2 = new PRED_$dummy_1_builtins$002Epl_6_2();

    public Object arg1, arg2, arg3, arg4, arg5, arg6;

    public PRED_$dummy_1_builtins$002Epl_6(Object a1, Object a2, Object a3, Object a4, Object a5, Object a6, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        arg5 = a5;
        arg6 = a6;
        this.cont = cont;
    }

    public PRED_$dummy_1_builtins$002Epl_6(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        arg5 = args[4];
        arg6 = args[5];
        this.cont = cont;
    }

    public int arity() { return 6; }

    public String nameUQ() { return "$dummy_1_builtins.pl"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
         case 2: arg3 = val;break ;
         case 3: arg4 = val;break ;
         case 4: arg5 = val;break ;
         case 5: arg6 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
         case 2: return arg3;
         case 3: return arg4;
         case 4: return arg5;
         case 5: return arg6;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$dummy_1_builtins.pl'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + "," + argString(arg5,newParam) + "," + argString(arg6,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine_aregs[4] = arg4;
        engine_aregs[5] = arg5;
        engine_aregs[6] = arg6;
        engine.cont = cont;
        engine.setB0();
        return engine.jtry(_$dummy_1_builtins$002Epl_6_1, _$dummy_1_builtins$002Epl_6_sub_1);
    }
}

class PRED_$dummy_1_builtins$002Epl_6_sub_1 extends PRED_$dummy_1_builtins$002Epl_6 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$dummy_1_builtins$002Epl_6_2);
    }
}

class PRED_$dummy_1_builtins$002Epl_6_1 extends PRED_$dummy_1_builtins$002Epl_6 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dummy_1_builtins.pl'(A, B, C, D, E, F):-'$meta_call'(A, C, no, E, F), !, '$meta_call'(B, C, D, E, F)
        Object a1, a2, a3, a4, a5, a6, a7;
        Predicate p1, p2;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        a5 = engine_aregs[5];
        a6 = engine_aregs[6];
        cont = engine.cont;
    // '$dummy_1_builtins.pl'(A, B, C, D, E, F):-['$get_level'(G), '$meta_call'(A, C, no, E, F), '$cut'(G), '$meta_call'(B, C, D, E, F)]
        a7 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(7))
        if (! unify(a7,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        p1 = new PRED_$meta_call_5(a2, a3, a4, a5, a6, cont);
        p2 = new PRED_$cut_1(a7, p1);
        return exit(engine, new PRED_$meta_call_5(a1, a3, s1, a5, a6, p2));
    }
}

class PRED_$dummy_1_builtins$002Epl_6_2 extends PRED_$dummy_1_builtins$002Epl_6 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$dummy_1_builtins.pl'(A, B, C, D, E, F):-fail
        Object a1, a2, a3, a4, a5, a6;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        a4 = engine_aregs[4];
        a5 = engine_aregs[5];
        a6 = engine_aregs[6];
        cont = engine.cont;
    // '$dummy_1_builtins.pl'(A, B, C, D, E, F):-[fail]
        //START inline expansion of fail
        return fail(engine);
        //END inline expansion
    }
}
