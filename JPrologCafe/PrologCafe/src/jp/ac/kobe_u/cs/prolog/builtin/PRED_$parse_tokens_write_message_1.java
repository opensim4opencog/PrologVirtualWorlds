package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$parse_tokens_write_message'/1</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$parse_tokens_write_message_1 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("[]");
    static /*Symbol*/Object s2 = makeAtom(" ");
    static Predicate _fail_0 = new PRED_fail_0();
    static Predicate _$parse_tokens_write_message_1_var = new PRED_$parse_tokens_write_message_1_var();
    static Predicate _$parse_tokens_write_message_1_var_1 = new PRED_$parse_tokens_write_message_1_var_1();
    static Predicate _$parse_tokens_write_message_1_1 = new PRED_$parse_tokens_write_message_1_1();
    static Predicate _$parse_tokens_write_message_1_2 = new PRED_$parse_tokens_write_message_1_2();

    public Object arg1;

    public PRED_$parse_tokens_write_message_1(Object a1, Predicate cont) {
        arg1 = a1;
        this.cont = cont;
    }

    public PRED_$parse_tokens_write_message_1(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        this.cont = cont;
    }

    public int arity() { return 1; }

    public String nameUQ() { return "$parse_tokens_write_message"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$parse_tokens_write_message'(" + argString(arg1,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine.cont = cont;
        engine.setB0();
        return engine.switch_on_term(_$parse_tokens_write_message_1_var, _fail_0, _fail_0, _$parse_tokens_write_message_1_1, _fail_0, _$parse_tokens_write_message_1_2);
    }
}

class PRED_$parse_tokens_write_message_1_var extends PRED_$parse_tokens_write_message_1 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$parse_tokens_write_message_1_1, _$parse_tokens_write_message_1_var_1);
    }
}

class PRED_$parse_tokens_write_message_1_var_1 extends PRED_$parse_tokens_write_message_1 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$parse_tokens_write_message_1_2);
    }
}

class PRED_$parse_tokens_write_message_1_1 extends PRED_$parse_tokens_write_message_1 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$parse_tokens_write_message'([]):-true
        Object a1;
        Predicate cont;
        a1 = engine_aregs[1];
        cont = engine.cont;
    // '$parse_tokens_write_message'([]):-[]
        a1 = deref( a1);
        if (isAtomTerm(a1)){
            if (! prologEquals(a1,s1))
                return fail(engine);
        } else if (isVariable(a1)){
             bind(/*VAR*/ a1,s1);
        } else {
            return fail(engine);
        }
        return exit(engine,cont);
    }
}

class PRED_$parse_tokens_write_message_1_2 extends PRED_$parse_tokens_write_message_1 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$parse_tokens_write_message'([A|B]):-write(A), write(' '), '$parse_tokens_write_message'(B)
        Object a1, a2, a3;
        Predicate p1, p2;
        Predicate cont;
        a1 = engine_aregs[1];
        cont = engine.cont;
    // '$parse_tokens_write_message'([A|B]):-[write(A), write(' '), '$parse_tokens_write_message'(B)]
        a1 = deref( a1);
        if (isListTerm(a1)){
            Object[] args = consArgs(a1);
            a2 = args[0];
            a3 = args[1];
        } else if (isVariable(a1)){
            a2 = engine.makeVariable(this);
            a3 = engine.makeVariable(this);
             bind(a1,makeList(a2, a3));
        } else {
            return fail(engine);
        }
        p1 = new PRED_$parse_tokens_write_message_1(a3, cont);
        p2 = new PRED_write_1(s2, p1);
        return exit(engine, new PRED_write_1(a2, p2));
    }
}