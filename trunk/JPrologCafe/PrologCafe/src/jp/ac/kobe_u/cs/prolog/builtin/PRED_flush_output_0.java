package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>flush_output/0</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
public class PRED_flush_output_0 extends PredicateBase {

    public PRED_flush_output_0(Predicate cont) {
        this.cont = cont;
    }

    public PRED_flush_output_0(){}

    public void setArgument(Object[] args, Predicate cont) {
        this.cont = cont;
    }

    public int arity() { return 0; }

    public String nameUQ() { return "flush_output"; }

    public void sArg(int i0, Object val) {  switch (i0) {default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'flush_output";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
    // flush_output:-current_output(A), flush_output(A)
        engine.setB0();
        Object a1;
        Predicate p1;
    // flush_output:-[current_output(A), flush_output(A)]
        a1 = engine.makeVariable(this);
        p1 = new PRED_flush_output_1(a1, cont);
        return exit(engine, new PRED_current_output_1(a1, p1));
    }
}
