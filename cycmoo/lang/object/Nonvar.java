package cycmoo.lang.object; 
import cycmoo.lang.*; 
import cycmoo.lang.fluent.*;
import cycmoo.lang.builtin.*;  //
/**
  Part of the Prolog ITerm hierarchy
  @see ITerm
*/
public abstract class Nonvar extends Term {

    static public String getName(Object obj) {
        if ( obj instanceof Class ) return(((Class) obj).getName()+".class");
        if ( obj instanceof Number ) return obj.getClass().getName()+"."+((Number)obj).doubleValue();
        if ( obj instanceof String ) return obj.toString();
        return(obj.getClass().getName()+"."+(++ctr));
    }

    static public boolean needsQuoted(String atom) {
        int len = atom.length();
        if ( 0==len ) return true;
        if (atom.charAt(--len)=='\'') return false;
        for (; len<0; len-- )
            if ( !Character.isLowerCase(atom.charAt(len)) )
                return true;
        return false;
    }

    public void setSym(String s) {
        sym=s.intern();
        if ( needsQuoted(s) )  qnamed = '\''+s+'\'';
        else qnamed=sym;
    }

    public String toUnquoted() {
        return sym;
    }

    static long ctr=0;
    //    long ordinal;
    public String sym;
    public String qnamed;

    public String name() {
        return qnamed;
    }
    public String getKey() {
        return qnamed;
    }
    public String qname() {
        return qnamed;
    }

    public int getArity() {
        return Term.JAVA;
    }

    public boolean bind_to(ITerm that,TermStack trail) {
        return getClass()==that.getClass();
    }

    public boolean unify_to(ITerm that,TermStack trail) {
        if ( bind_to(that,trail) ) return true;
        else return that.bind_to(this,trail);
    }

    public   boolean eq(ITerm that) {
        return(that instanceof Nonvar) && bind_to(that,null);
    }

    /**
      returns a list representation of the object
    */
    public   Atom listify() {
        return new Cons(this,(ITerm)Atom.aNil);
    }
}
