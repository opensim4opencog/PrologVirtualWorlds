package jinni.kprolog;  //tarau.jinni;
/**
  Implements compound terms
  @see ITerm
*/
public class Compound extends Atom {
    private ITerm[] args;

    public final int getArity() {
        return args.length;
    }

    static public Compound newCompound(String s,int arety){
        return new Compound(s,arety);

    }
    static public Compound newCompoundStub(String s){
        return new Compound(s);

    }
   /*
    public Compound(String s) {
        super(s); 
        args=null;
    }
   */
    /*
    public Compound(int arity) {
       //setDefaultName();
       args=new ITerm[arity];
    }
    */

    public Compound(String s,int arity) {
        //super();
        setSym(s);
        args=new ITerm[arity]; 
    }

    private Compound(String s) {
        //super();
        setSym(s);
        args=null; 
    }

    public void initArgs(int arity) {
        args=new ITerm[arity];
        for ( int i=0; i<arity; i++ ) {
            args[i]=new PrologVar();
        }
    }

    public final ITerm getDrefArg(int i) {
        return args[i].dref();
    }

    public final ITerm[] getAllArgs() {
        return args;
    }

    public final ITerm getDirectArg(int i) {
        return args[i];
    }

    public final int getIntArg(int i) {
        return(int)((IntTerm)getDrefArg(i)).getValue();
    }

    public final void setDrefArg(int i,ITerm T) {
        args[i]=T.dref();
    }

    public final void setDirectArg(int i,ITerm T) {
        args[i]=T;
    }
    public final void setAllArgs(ITerm[] T) {
        args=T;
    }

    public final int putArg(int i,ITerm T,IKernel p) {
        if (T instanceof FunctionObject) ((FunctionObject)T).registerObject();
        return getDrefArg(i).unify(T,p.getTrail())?1:0;
    }
    public Compound(String s,ITerm x0,ITerm x1) {
        this(s,2);
        args[0]=x0;
        args[1]=x1;
    }
    /*
    public Compound(String s,ITerm x0) {
        this(s,1);
        args[0]=x0;
    }


    public Compound(String s,ITerm x0,ITerm x1,ITerm x2) {
        this(s,3);
        args[0]=x0;
        args[1]=x1;
        args[2]=x2;
    }

    public Compound(String s,ITerm x0,ITerm x1,ITerm x2,ITerm x3) {
        this(s,4);
        args[0]=x0;
        args[1]=x1;
        args[2]=x2;
        args[3]=x3;
    }
*/
    public String getKey() {
        if (null==args) return qname()+",_";
        return qname()+"/"+args.length;
    }
      /*
    public String name() {
        if (null==args) return qname()+",_";
        return qname()+","+args.length;
    } 
        */
    public final String funToString() {
        if ( args==null ) return qname()+"()";
        int l=args.length;
        return qname()+(l<=0?"":"("+show_args()+")");
    }

    public String toString() {
        return funToString();
    }

    public static String watchNull(ITerm x) {
        return((null==x)?"null":x.toString());
    }

    private String show_args() {
        StringBuffer s=
        new StringBuffer(
                        watchNull(args[0])
                        );
        for ( int i=1; i<args.length; i++ ) {
            s.append(","+
                     watchNull(args[i])
                    );
        }
        return s.toString();
    }

    public boolean bind_to(ITerm that,TermStack trail) {
        return super.bind_to(that,trail) &&
        args.length==((Compound)that).args.length;
    }

    public boolean unify_to(ITerm that,TermStack trail) {
        if ( bind_to(that,trail) ) {
            for ( int i=0; i<args.length; i++ ) {
                if ( !args[i].unify(((Compound)that).getDirectArg(i),trail) ) return false;
            }
            return true;
        } else return that.bind_to(this,trail);
    }

    public ITerm token() {
        return args[0];
    }

    // stuff allowing polymorphic cloning of Compound subclasses
    // without using reflection - should be probaly faster than 
    // reflection classes - to check

    final public Compound funClone() {
        Compound f=null;

        try {
            // use of clone is needed for "polymorphic" copy
            f=(Compound)clone(); 
        } catch ( CloneNotSupportedException e ) {
            IO.errmes("clone: "+e);
        }

        return f;
    }

    public Compound unInitializedClone() {
        Compound f=funClone();
        f.setAllArgs(new ITerm[args.length]);
        return f;
    }

    public Compound initializedClone() {
        Compound f=funClone();
        f.initArgs(args.length);
        return f;
    }


    public ITerm reaction(ITerm that) {
        //IO.mes("TRACE>> "+toString());
        Compound f=funClone();
        f.setAllArgs(new ITerm[args.length]);
        for ( int i=0; i<args.length; i++ ) {
            f.setDirectArg(i,args[i].reaction(that));
        }
        return f;
    }


    public Atom listify() {
        Cons l=new Cons(TermMap.useAtomStatic(name()),Atom.aNil);
        Cons curr=l;
        for ( int i=0;i<args.length; i++ ) {
            Cons tail=new Cons(args[i],Atom.aNil);
            curr.setDirectArg(1,tail);
            curr=tail;
        }
        return l;
    }

    public Atom toProceedure(IProceedureMap bb) {
        if ( name().equals(":-")&&getArity()==2 ) {
            return new HornClause(args[0],args[1]);
        }
        if ( name().equals(",")&&getArity()==2 ) {
            return new Conj(args[0],args[1]);
        }
        Object obj = bb.getProceedure(this);
        if ( null==obj ) return this;
        if (obj instanceof O1Queue) return this;
        if (obj instanceof Compound) {
            Compound B=((Compound)obj).funClone();
            B.setAllArgs(args);
            return B;
        }
        return null;
    }

    public boolean isClause() {
        return getArity()==2 && name().equals(":-");
    }
}

