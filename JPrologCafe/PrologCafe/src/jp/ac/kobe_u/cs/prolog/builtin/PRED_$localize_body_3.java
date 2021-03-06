package jp.ac.kobe_u.cs.prolog.builtin;
import jp.ac.kobe_u.cs.prolog.lang.*;
/*
 This file is generated by Prolog Cafe.
 PLEASE DO NOT EDIT!
*/
/**
 <code>'$localize_body'/3</code> defined in builtins.pl<br>
 @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 @author Douglas R. Miles (dmiles@users.sourceforge.net) for Object(s) 
 @version 1.0-dmiles
*/
class PRED_$localize_body_3 extends PredicateBase {
    static /*Symbol*/Object s1 = makeAtom("call", 1);
    static /*Symbol*/Object s2 = makeAtom(":", 2);
    static /*Symbol*/Object s3 = makeAtom(",", 2);
    static /*Symbol*/Object s4 = makeAtom("->", 2);
    static /*Symbol*/Object s5 = makeAtom(";", 2);
    static /*Symbol*/Object s6 = makeAtom("user");
    static Predicate _$localize_body_3_top = new PRED_$localize_body_3_top();
    static Predicate _$localize_body_3_var = new PRED_$localize_body_3_var();
    static Predicate _$localize_body_3_var_1 = new PRED_$localize_body_3_var_1();
    static Predicate _$localize_body_3_var_2 = new PRED_$localize_body_3_var_2();
    static Predicate _$localize_body_3_var_3 = new PRED_$localize_body_3_var_3();
    static Predicate _$localize_body_3_var_4 = new PRED_$localize_body_3_var_4();
    static Predicate _$localize_body_3_var_5 = new PRED_$localize_body_3_var_5();
    static Predicate _$localize_body_3_var_6 = new PRED_$localize_body_3_var_6();
    static Predicate _$localize_body_3_var_7 = new PRED_$localize_body_3_var_7();
    static Predicate _$localize_body_3_var_8 = new PRED_$localize_body_3_var_8();
    static Predicate _$localize_body_3_var_9 = new PRED_$localize_body_3_var_9();
    static Predicate _$localize_body_3_int = new PRED_$localize_body_3_int();
    static Predicate _$localize_body_3_int_1 = new PRED_$localize_body_3_int_1();
    static Predicate _$localize_body_3_int_2 = new PRED_$localize_body_3_int_2();
    static Predicate _$localize_body_3_int_3 = new PRED_$localize_body_3_int_3();
    static Predicate _$localize_body_3_int_4 = new PRED_$localize_body_3_int_4();
    static Predicate _$localize_body_3_int_5 = new PRED_$localize_body_3_int_5();
    static Predicate _$localize_body_3_str = new PRED_$localize_body_3_str();
    static Predicate _$localize_body_3_str_0 = new PRED_$localize_body_3_str_0();
    static Predicate _$localize_body_3_str_0_1 = new PRED_$localize_body_3_str_0_1();
    static Predicate _$localize_body_3_str_0_2 = new PRED_$localize_body_3_str_0_2();
    static Predicate _$localize_body_3_str_0_3 = new PRED_$localize_body_3_str_0_3();
    static Predicate _$localize_body_3_str_0_4 = new PRED_$localize_body_3_str_0_4();
    static Predicate _$localize_body_3_str_0_5 = new PRED_$localize_body_3_str_0_5();
    static Predicate _$localize_body_3_str_0_6 = new PRED_$localize_body_3_str_0_6();
    static Predicate _$localize_body_3_str_1 = new PRED_$localize_body_3_str_1();
    static Predicate _$localize_body_3_str_1_1 = new PRED_$localize_body_3_str_1_1();
    static Predicate _$localize_body_3_str_1_2 = new PRED_$localize_body_3_str_1_2();
    static Predicate _$localize_body_3_str_1_3 = new PRED_$localize_body_3_str_1_3();
    static Predicate _$localize_body_3_str_1_4 = new PRED_$localize_body_3_str_1_4();
    static Predicate _$localize_body_3_str_1_5 = new PRED_$localize_body_3_str_1_5();
    static Predicate _$localize_body_3_str_1_6 = new PRED_$localize_body_3_str_1_6();
    static Predicate _$localize_body_3_str_2 = new PRED_$localize_body_3_str_2();
    static Predicate _$localize_body_3_str_2_1 = new PRED_$localize_body_3_str_2_1();
    static Predicate _$localize_body_3_str_2_2 = new PRED_$localize_body_3_str_2_2();
    static Predicate _$localize_body_3_str_2_3 = new PRED_$localize_body_3_str_2_3();
    static Predicate _$localize_body_3_str_2_4 = new PRED_$localize_body_3_str_2_4();
    static Predicate _$localize_body_3_str_2_5 = new PRED_$localize_body_3_str_2_5();
    static Predicate _$localize_body_3_str_2_6 = new PRED_$localize_body_3_str_2_6();
    static Predicate _$localize_body_3_str_3 = new PRED_$localize_body_3_str_3();
    static Predicate _$localize_body_3_str_3_1 = new PRED_$localize_body_3_str_3_1();
    static Predicate _$localize_body_3_str_3_2 = new PRED_$localize_body_3_str_3_2();
    static Predicate _$localize_body_3_str_3_3 = new PRED_$localize_body_3_str_3_3();
    static Predicate _$localize_body_3_str_3_4 = new PRED_$localize_body_3_str_3_4();
    static Predicate _$localize_body_3_str_3_5 = new PRED_$localize_body_3_str_3_5();
    static Predicate _$localize_body_3_str_3_6 = new PRED_$localize_body_3_str_3_6();
    static Predicate _$localize_body_3_1 = new PRED_$localize_body_3_1();
    static Predicate _$localize_body_3_2 = new PRED_$localize_body_3_2();
    static Predicate _$localize_body_3_3 = new PRED_$localize_body_3_3();
    static Predicate _$localize_body_3_4 = new PRED_$localize_body_3_4();
    static Predicate _$localize_body_3_5 = new PRED_$localize_body_3_5();
    static Predicate _$localize_body_3_6 = new PRED_$localize_body_3_6();
    static Predicate _$localize_body_3_7 = new PRED_$localize_body_3_7();
    static Predicate _$localize_body_3_8 = new PRED_$localize_body_3_8();
    static Predicate _$localize_body_3_9 = new PRED_$localize_body_3_9();
    static Predicate _$localize_body_3_10 = new PRED_$localize_body_3_10();
    static HashtableOfTerm<Predicate> str = new HashtableOfTerm<Predicate>(4);
    static {
        str.put(s2, _$localize_body_3_str_0);
        str.put(s3, _$localize_body_3_str_1);
        str.put(s4, _$localize_body_3_str_2);
        str.put(s5, _$localize_body_3_str_3);
    }

    public Object arg1, arg2, arg3;

    public PRED_$localize_body_3(Object a1, Object a2, Object a3, Predicate cont) {
        arg1 = a1;
        arg2 = a2;
        arg3 = a3;
        this.cont = cont;
    }

    public PRED_$localize_body_3(){}

    public void setArgument(Object[] args, Predicate cont) {
        arg1 = args[0];
        arg2 = args[1];
        arg3 = args[2];
        this.cont = cont;
    }

    public int arity() { return 3; }

    public String nameUQ() { return "$localize_body"; }

    public void sArg(int i0, Object val) {  switch (i0) {         case 0: arg1 = val;break ;
         case 1: arg2 = val;break ;
         case 2: arg3 = val;break ;
default: newIndexOutOfBoundsException("setarg" , i0 , val);  }   }


    public Object gArg(int i0) {  switch (i0) {         case 0: return arg1;
         case 1: return arg2;
         case 2: return arg3;
default: return newIndexOutOfBoundsException("getarg", i0,null);  }   }


    public String toPrologString(java.util.Collection newParam) {
        return "'$localize_body'(" + argString(arg1,newParam) + "," + argString(arg2,newParam) + "," + argString(arg3,newParam) + ")";
    }

    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); 
        engine_aregs[1] = arg1;
        engine_aregs[2] = arg2;
        engine_aregs[3] = arg3;
        engine.cont = cont;
        return exit(engine, _$localize_body_3_top);
    }
}

class PRED_$localize_body_3_top extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        engine.setB0();
        return engine.switch_on_term(_$localize_body_3_var, _$localize_body_3_int, _$localize_body_3_int, _$localize_body_3_int, _$localize_body_3_str, _$localize_body_3_int);
    }
}

class PRED_$localize_body_3_var extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$localize_body_3_1, _$localize_body_3_var_1);
    }
}

class PRED_$localize_body_3_var_1 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_2, _$localize_body_3_var_2);
    }
}

class PRED_$localize_body_3_var_2 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_3, _$localize_body_3_var_3);
    }
}

class PRED_$localize_body_3_var_3 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_4, _$localize_body_3_var_4);
    }
}

class PRED_$localize_body_3_var_4 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_5, _$localize_body_3_var_5);
    }
}

class PRED_$localize_body_3_var_5 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_6, _$localize_body_3_var_6);
    }
}

class PRED_$localize_body_3_var_6 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_7, _$localize_body_3_var_7);
    }
}

class PRED_$localize_body_3_var_7 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_8, _$localize_body_3_var_8);
    }
}

class PRED_$localize_body_3_var_8 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_9, _$localize_body_3_var_9);
    }
}

class PRED_$localize_body_3_var_9 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$localize_body_3_10);
    }
}

class PRED_$localize_body_3_int extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$localize_body_3_1, _$localize_body_3_int_1);
    }
}

class PRED_$localize_body_3_int_1 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_6, _$localize_body_3_int_2);
    }
}

class PRED_$localize_body_3_int_2 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_7, _$localize_body_3_int_3);
    }
}

class PRED_$localize_body_3_int_3 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_8, _$localize_body_3_int_4);
    }
}

class PRED_$localize_body_3_int_4 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_9, _$localize_body_3_int_5);
    }
}

class PRED_$localize_body_3_int_5 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$localize_body_3_10);
    }
}

class PRED_$localize_body_3_str extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.switch_on_hash(str, _$localize_body_3_int);
    }
}

class PRED_$localize_body_3_str_0 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$localize_body_3_1, _$localize_body_3_str_0_1);
    }
}

class PRED_$localize_body_3_str_0_1 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_2, _$localize_body_3_str_0_2);
    }
}

class PRED_$localize_body_3_str_0_2 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_6, _$localize_body_3_str_0_3);
    }
}

class PRED_$localize_body_3_str_0_3 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_7, _$localize_body_3_str_0_4);
    }
}

class PRED_$localize_body_3_str_0_4 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_8, _$localize_body_3_str_0_5);
    }
}

class PRED_$localize_body_3_str_0_5 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_9, _$localize_body_3_str_0_6);
    }
}

class PRED_$localize_body_3_str_0_6 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$localize_body_3_10);
    }
}

class PRED_$localize_body_3_str_1 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$localize_body_3_1, _$localize_body_3_str_1_1);
    }
}

class PRED_$localize_body_3_str_1_1 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_3, _$localize_body_3_str_1_2);
    }
}

class PRED_$localize_body_3_str_1_2 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_6, _$localize_body_3_str_1_3);
    }
}

class PRED_$localize_body_3_str_1_3 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_7, _$localize_body_3_str_1_4);
    }
}

class PRED_$localize_body_3_str_1_4 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_8, _$localize_body_3_str_1_5);
    }
}

class PRED_$localize_body_3_str_1_5 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_9, _$localize_body_3_str_1_6);
    }
}

class PRED_$localize_body_3_str_1_6 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$localize_body_3_10);
    }
}

class PRED_$localize_body_3_str_2 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$localize_body_3_1, _$localize_body_3_str_2_1);
    }
}

class PRED_$localize_body_3_str_2_1 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_4, _$localize_body_3_str_2_2);
    }
}

class PRED_$localize_body_3_str_2_2 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_6, _$localize_body_3_str_2_3);
    }
}

class PRED_$localize_body_3_str_2_3 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_7, _$localize_body_3_str_2_4);
    }
}

class PRED_$localize_body_3_str_2_4 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_8, _$localize_body_3_str_2_5);
    }
}

class PRED_$localize_body_3_str_2_5 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_9, _$localize_body_3_str_2_6);
    }
}

class PRED_$localize_body_3_str_2_6 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$localize_body_3_10);
    }
}

class PRED_$localize_body_3_str_3 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.jtry(_$localize_body_3_1, _$localize_body_3_str_3_1);
    }
}

class PRED_$localize_body_3_str_3_1 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_5, _$localize_body_3_str_3_2);
    }
}

class PRED_$localize_body_3_str_3_2 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_6, _$localize_body_3_str_3_3);
    }
}

class PRED_$localize_body_3_str_3_3 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_7, _$localize_body_3_str_3_4);
    }
}

class PRED_$localize_body_3_str_3_4 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_8, _$localize_body_3_str_3_5);
    }
}

class PRED_$localize_body_3_str_3_5 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.retry(_$localize_body_3_9, _$localize_body_3_str_3_6);
    }
}

class PRED_$localize_body_3_str_3_6 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
        return engine.trust(_$localize_body_3_10);
    }
}

class PRED_$localize_body_3_1 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$localize_body'(A, B, C):-var(A), !, '$localize_body'(call(A), B, C)
        Object a1, a2, a3, a4, a5;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$localize_body'(A, B, C):-['$get_level'(D), var(A), '$cut'(D), '$localize_body'(call(A), B, C)]
        a4 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(4))
        if (! unify(a4,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of var(a(1))
        a1 = deref( a1);
        if (! isVariable(a1)) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of $cut(a(4))
        a4 = deref( a4);
        if (! isCutter/*Integer*/(a4)) {
            throw new IllegalTypeException("integer", a4);
        } else {
            engine.cut(( a4));
        }
        //END inline expansion
        Object[] y1 = {a1};
        a5 = makeStructure(s1, y1);
        engine_aregs[1] = a5;
        engine_aregs[2] = a2;
        engine_aregs[3] = a3;
        engine.cont = cont;
        return exit(engine, _$localize_body_3_top);
    }
}

class PRED_$localize_body_3_2 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$localize_body'(A:B, C, D):-!, '$localize_body'(B, A, D)
        Object a1, a2, a3, a4, a5;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$localize_body'(A:B, C, D):-['$neck_cut', '$localize_body'(B, A, D)]
        a1 = deref( a1);
        if (isCompound(a1)){
            if (!functorOf( s2,a1))
                return fail(engine);
            Object[] args = args(a1);
            a4 = args[0];
            a5 = args[1];
        } else if (isVariable(a1)){
            a4 = engine.makeVariable(this);
            a5 = engine.makeVariable(this);
            Object[] args = {a4, a5};
            bind(/*VAR*/ a1,makeStructure(s2, args));
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        engine_aregs[1] = a5;
        engine_aregs[2] = a4;
        engine_aregs[3] = a3;
        engine.cont = cont;
        return exit(engine, _$localize_body_3_top);
    }
}

class PRED_$localize_body_3_3 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$localize_body'((A, B), C, (D, E)):-!, '$localize_body'(A, C, D), '$localize_body'(B, C, E)
        Object a1, a2, a3, a4, a5, a6, a7;
        Predicate p1;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$localize_body'((A, B), C, (D, E)):-['$neck_cut', '$localize_body'(A, C, D), '$localize_body'(B, C, E)]
        a1 = deref( a1);
        if (isCompound(a1)){
            if (!functorOf( s3,a1))
                return fail(engine);
            Object[] args = args(a1);
            a4 = args[0];
            a5 = args[1];
        } else if (isVariable(a1)){
            a4 = engine.makeVariable(this);
            a5 = engine.makeVariable(this);
            Object[] args = {a4, a5};
            bind(/*VAR*/ a1,makeStructure(s3, args));
        } else {
            return fail(engine);
        }
        a3 = deref( a3);
        if (isCompound(a3)){
            if (!functorOf( s3,a3))
                return fail(engine);
            Object[] args = args(a3);
            a6 = args[0];
            a7 = args[1];
        } else if (isVariable(a3)){
            a6 = engine.makeVariable(this);
            a7 = engine.makeVariable(this);
            Object[] args = {a6, a7};
            bind(/*VAR*/ a3,makeStructure(s3, args));
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        p1 = new PRED_$localize_body_3(a5, a2, a7, cont);
        engine_aregs[1] = a4;
        engine_aregs[2] = a2;
        engine_aregs[3] = a6;
        engine.cont = p1;
        return exit(engine, _$localize_body_3_top);
    }
}

class PRED_$localize_body_3_4 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$localize_body'((A->B), C, (D->E)):-!, '$localize_body'(A, C, D), '$localize_body'(B, C, E)
        Object a1, a2, a3, a4, a5, a6, a7;
        Predicate p1;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$localize_body'((A->B), C, (D->E)):-['$neck_cut', '$localize_body'(A, C, D), '$localize_body'(B, C, E)]
        a1 = deref( a1);
        if (isCompound(a1)){
            if (!functorOf( s4,a1))
                return fail(engine);
            Object[] args = args(a1);
            a4 = args[0];
            a5 = args[1];
        } else if (isVariable(a1)){
            a4 = engine.makeVariable(this);
            a5 = engine.makeVariable(this);
            Object[] args = {a4, a5};
            bind(/*VAR*/ a1,makeStructure(s4, args));
        } else {
            return fail(engine);
        }
        a3 = deref( a3);
        if (isCompound(a3)){
            if (!functorOf( s4,a3))
                return fail(engine);
            Object[] args = args(a3);
            a6 = args[0];
            a7 = args[1];
        } else if (isVariable(a3)){
            a6 = engine.makeVariable(this);
            a7 = engine.makeVariable(this);
            Object[] args = {a6, a7};
            bind(/*VAR*/ a3,makeStructure(s4, args));
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        p1 = new PRED_$localize_body_3(a5, a2, a7, cont);
        engine_aregs[1] = a4;
        engine_aregs[2] = a2;
        engine_aregs[3] = a6;
        engine.cont = p1;
        return exit(engine, _$localize_body_3_top);
    }
}

class PRED_$localize_body_3_5 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$localize_body'((A;B), C, (D;E)):-!, '$localize_body'(A, C, D), '$localize_body'(B, C, E)
        Object a1, a2, a3, a4, a5, a6, a7;
        Predicate p1;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$localize_body'((A;B), C, (D;E)):-['$neck_cut', '$localize_body'(A, C, D), '$localize_body'(B, C, E)]
        a1 = deref( a1);
        if (isCompound(a1)){
            if (!functorOf( s5,a1))
                return fail(engine);
            Object[] args = args(a1);
            a4 = args[0];
            a5 = args[1];
        } else if (isVariable(a1)){
            a4 = engine.makeVariable(this);
            a5 = engine.makeVariable(this);
            Object[] args = {a4, a5};
            bind(/*VAR*/ a1,makeStructure(s5, args));
        } else {
            return fail(engine);
        }
        a3 = deref( a3);
        if (isCompound(a3)){
            if (!functorOf( s5,a3))
                return fail(engine);
            Object[] args = args(a3);
            a6 = args[0];
            a7 = args[1];
        } else if (isVariable(a3)){
            a6 = engine.makeVariable(this);
            a7 = engine.makeVariable(this);
            Object[] args = {a6, a7};
            bind(/*VAR*/ a3,makeStructure(s5, args));
        } else {
            return fail(engine);
        }
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        p1 = new PRED_$localize_body_3(a5, a2, a7, cont);
        engine_aregs[1] = a4;
        engine_aregs[2] = a2;
        engine_aregs[3] = a6;
        engine.cont = p1;
        return exit(engine, _$localize_body_3_top);
    }
}

class PRED_$localize_body_3_6 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$localize_body'(A, B, C):-functor(A, D, E), '$builtin_meta_predicates'(D, E, F), !, A=..[D|G], '$localize_args'(F, G, B, H), C=..[D|H]
        Object a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11;
        Predicate p1, p2, p3, p4, p5;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$localize_body'(A, B, C):-['$get_level'(D), functor(A, E, F), '$builtin_meta_predicates'(E, F, G), '$cut'(D), '$univ'(A, [E|H]), '$localize_args'(G, H, B, I), '$univ'(C, [E|I])]
        a4 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(4))
        if (! unify(a4,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        a5 = engine.makeVariable(this);
        a6 = engine.makeVariable(this);
        a7 = engine.makeVariable(this);
        a8 = engine.makeVariable(this);
        a9 = makeList(a5, a8);
        a10 = engine.makeVariable(this);
        a11 = makeList(a5, a10);
        p1 = new PRED_$univ_2(a3, a11, cont);
        p2 = new PRED_$localize_args_4(a7, a8, a2, a10, p1);
        p3 = new PRED_$univ_2(a1, a9, p2);
        p4 = new PRED_$cut_1(a4, p3);
        p5 = new PRED_$builtin_meta_predicates_3(a5, a6, a7, p4);
        return exit(engine, new PRED_functor_3(a1, a5, a6, p5));
    }
}

class PRED_$localize_body_3_7 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$localize_body'(A, B, call(B:A)):-var(B), !
        Object a1, a2, a3, a4, a5;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$localize_body'(A, B, call(B:A)):-['$get_level'(C), var(B), '$cut'(C)]
        a3 = deref( a3);
        if (isCompound(a3)){
            if (!functorOf( s1,a3))
                return fail(engine);
            Object[] args = args(a3);
            a4 = args[0];
        } else if (isVariable(a3)){
            a4 = engine.makeVariable(this);
            Object[] args = {a4};
            bind(/*VAR*/ a3,makeStructure(s1, args));
        } else {
            return fail(engine);
        }
        a4 = deref( a4);
        if (isCompound(a4)){
            if (!functorOf( s2,a4))
                return fail(engine);
            Object[] args = args(a4);
            if (!unify( a2,args[0]))
                return fail(engine);
            if (!unify( a1,args[1]))
                return fail(engine);
        } else if (isVariable(a4)){
            Object[] args = {a2, a1};
            bind(/*VAR*/ a4,makeStructure(s2, args));
        } else {
            return fail(engine);
        }
        a5 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(5))
        if (! unify(a5,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of var(a(2))
        a2 = deref( a2);
        if (! isVariable(a2)) {
            return fail(engine);
        }
        //END inline expansion
        //START inline expansion of $cut(a(5))
        a5 = deref( a5);
        if (! isCutter/*Integer*/(a5)) {
            throw new IllegalTypeException("integer", a5);
        } else {
            engine.cut(( a5));
        }
        //END inline expansion
        return exit(engine,cont);
    }
}

class PRED_$localize_body_3_8 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$localize_body'(A, user, A):-!
        Object a1, a2, a3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$localize_body'(A, user, A):-['$neck_cut']
        a2 = deref( a2);
        if (isAtomTerm(a2)){
            if (! prologEquals(a2,s6))
                return fail(engine);
        } else if (isVariable(a2)){
             bind(/*VAR*/ a2,s6);
        } else {
            return fail(engine);
        }
        if (! unify(a1,a3))
            return fail(engine);
        //START inline expansion of $neck_cut
        engine.neckCut();
        //END inline expansion
        return exit(engine,cont);
    }
}

class PRED_$localize_body_3_9 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$localize_body'(A, B, A):-system_predicate(A), !
        Object a1, a2, a3, a4;
        Predicate p1;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$localize_body'(A, B, A):-['$get_level'(C), system_predicate(A), '$cut'(C)]
        if (! unify(a1,a3))
            return fail(engine);
        a4 = engine.makeVariable(this);
        //START inline expansion of $get_level(a(4))
        if (! unify(a4,makeInteger(engine.B0))) {
            return fail(engine);
        }
        //END inline expansion
        p1 = new PRED_$cut_1(a4, cont);
        return exit(engine, new PRED_system_predicate_1(a1, p1));
    }
}

class PRED_$localize_body_3_10 extends PRED_$localize_body_3 {
    public Predicate exec(Prolog engine) { enter(engine); Object[] engine_aregs = engine.getAreg(); //anony 
    // '$localize_body'(A, B, B:A):-true
        Object a1, a2, a3;
        Predicate cont;
        a1 = engine_aregs[1];
        a2 = engine_aregs[2];
        a3 = engine_aregs[3];
        cont = engine.cont;
    // '$localize_body'(A, B, B:A):-[]
        a3 = deref( a3);
        if (isCompound(a3)){
            if (!functorOf( s2,a3))
                return fail(engine);
            Object[] args = args(a3);
            if (!unify( a2,args[0]))
                return fail(engine);
            if (!unify( a1,args[1]))
                return fail(engine);
        } else if (isVariable(a3)){
            Object[] args = {a2, a1};
            bind(/*VAR*/ a3,makeStructure(s2, args));
        } else {
            return fail(engine);
        }
        return exit(engine,cont);
    }
}
