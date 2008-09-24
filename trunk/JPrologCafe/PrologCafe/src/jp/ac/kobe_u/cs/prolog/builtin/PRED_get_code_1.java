package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>get_code/1</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
public class PRED_get_code_1 extends PredicateBase {

    public Object arg1;

    public PRED_get_code_1(Object a1, Predicate cont) {
        arg1 = a1;
        this.cont = cont;
    }

    public PRED_get_code_1(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        this.cont = cont;
    }

    public int arity() { return 1; }

    public String nameUQ() { return "get_code"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'get_code'(" + argString(arg1,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
    // get_code(A):-current_input(B), get_code(B, A)
        engine.setB0();
        Object a1, a2;
        Predicate p1;
        a1 = arg1;
    // get_code(A):-[current_input(B), get_code(B, A)]
        a2 = engine.makeVariable(this);
        p1 = new PRED_get_code_2(a2, a1, cont);
        return exit(engine, new PRED_current_input_1(a2, p1));
    }
}
