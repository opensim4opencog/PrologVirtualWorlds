package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$parse_tokens'/4</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$parse_tokens_4 extends PredicateBase {

    public Object arg1, arg2, arg3, arg4;

    public PRED_$parse_tokens_4(Object a1, Object a2, Object a3, Object a4, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        arg4 = a4;
        this.cont = cont;
    }

    public PRED_$parse_tokens_4(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        arg4 = args[3];
        this.cont = cont;
    }

    public int arity() { return 4; }

    public String nameUQ() { return "$parse_tokens"; }

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
        return "'$parse_tokens'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + "," + argString(arg4,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
    // '$parse_tokens'(A, B, C, D):-'$parse_tokens_skip_spaces'(C, E), '$parse_tokens1'(B, F, G, E, H), !, '$parse_tokens_skip_spaces'(H, I), '$parse_tokens2'(B, F, G, A, J, I, D), !
        engine.setB0();
        Object a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
        Predicate p1, p2, p3, p4, p5;
        a1 = arg1;
        a2 = arg2;
        a3 = arg3;
        a4 = arg4;
    // '$parse_tokens'(A, B, C, D):-['$get_level'(E), '$parse_tokens_skip_spaces'(C, F), '$parse_tokens1'(B, G, H, F, I), '$cut'(E), '$parse_tokens_skip_spaces'(I, J), '$parse_tokens2'(B, G, H, A, K, J, D), '$cut'(E)]
        a5 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(5))
        if (! unify(a5,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        a6 = engine.makeVariable(this);
        a7 = engine.makeVariable(this);
        a8 = engine.makeVariable(this);
        a9 = engine.makeVariable(this);
        a10 = engine.makeVariable(this);
        p1 = new PRED_$cut_1(a5, cont);
        p2 = new PRED_$parse_tokens2_7(a2, a7, a8, a1, engine.makeVariable(this), a10, a4, p1);
        p3 = new PRED_$parse_tokens_skip_spaces_2(a9, a10, p2);
        p4 = new PRED_$cut_1(a5, p3);
        p5 = new PRED_$parse_tokens1_5(a2, a7, a8, a6, a9, p4);
        return exit(engine, new PRED_$parse_tokens_skip_spaces_2(a3, a6, p5));
    }
}