package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>integer/1</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
public class PRED_integer_1 extends PredicateBase {

    public Object arg1;

    public PRED_integer_1(Object a1, Predicate cont) {
        arg1 = a1;
        this.cont = cont;
    }

    public PRED_integer_1(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        this.cont = cont;
    }

    public int arity() { return 1; }

    public String nameUQ() { return "integer"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'integer'(" + argString(arg1,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
    // integer(A):-integer(A)
        engine.setB0();
        Object a1;
        a1 = arg1;
    // integer(A):-[integer(A)]
        //START inline expansion of integer(a(1))
        a1 = deref( a1);
        if (! isInteger(a1)) {
            return fail(engine);
        }
        //END inline expansion
        return exit(engine,cont);
    }
}
