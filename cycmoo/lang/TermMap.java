package jinni.kprolog;  //tarau.jinni;
import java.util.Vector;
import java.util.Enumeration;
import java.util.Map;
import java.util.Hashtable;
import java.util.NoSuchElementException;
import java.util.*;
import java.io.*;
import jinni.kprolog.parser.PrologParser;  //tarau.jinni;
import java.util.*;
import java.io.Reader;


/**
This class implementes generic multiple tuples by key
operations for use by the Blackboard class implementing
Linda operations on Prolog terms. It uses the O1Queue class for
keeping elemetns of type ITerm sharing the same key.

@see Blackboard
@see O1Queue
@see ITerm
*/
public class TermMap extends PersistentFluent implements IProceedureMap, ITermMap, IKernel, Cloneable, Serializable {

    public final static Map atomTable = new Hashtable();
    static public final ITerm useAtomStatic(String s){
        ITerm I=(ITerm)atomTable.get(s);
        if (I!=null) return I;
        I=new Atom(s);
        atomTable.put(I.name(),I);
        return I;
    }

    static public final Atom defAtomStatic(String s){
        Atom I=(Atom)atomTable.get(s);
        if (I!=null) return I;
        I=new Atom(s);
        atomTable.put(I.name(),I);
        return I;
    }
    
    public Map getLocalAtomTable() {
        return tble;
    }

    public final ITerm useAtom(String s){
        ITerm I=(ITerm)getLocalAtomTable().get(s);
        if (I!=null) return I;
        I=new Atom(s);
        getLocalAtomTable().put(I.name(),I);
        return I;
    }

    public final Atom defAtom(String s){
        Atom I=(Atom)getLocalAtomTable().get(s);
        if (I!=null) return I;
        I=new Atom(s);
        getLocalAtomTable().put(I.name(),I);
        return I;
    }
    public final static Atom useStringAtom(String s){
        return new Atom(s);
    }

    /**
       creates a new TermMap 
       @see ITerm
    */
    public Map tble = null;
    
     public TermMap(Map map) {
        super();
        setSym(getName(this));
        tble = map;//new Hashtable(map);
    }
    
    public int size() {
        return tble.size();
    }
    public boolean isEmpty() {
        return tble.isEmpty();
    }
    public java.util.Enumeration keys() {
        return new Vector(tble.keySet()).elements();
    }
    public java.util.Enumeration elements() {
        return new Hashtable(tble).elements();
    }
    public java.lang.Object put(java.lang.Object A, java.lang.Object B) {
        return (ITerm)tble.put(A,B);
    }
    public boolean containsKey(java.lang.Object A) {
        return tble.containsKey(A);
    }
    public boolean containsValue( java.lang.Object  A) {
        return tble.containsValue(A);
    }
    public java.lang.Object get(java.lang.Object A) {
        return (ITerm)tble.get(A);
    }
    public java.util.Set keySet() {
        return tble.keySet();
    }
    public java.lang.Object remove(java.lang.Object A) {
        return tble.remove(A);
    }
    public void putAll(java.util.Map A) {
        tble.putAll(A);
    }
    public void clear() {
        tble.clear();
    }
    public java.util.Collection values() {
        return tble.values();
    }
    public java.util.Set entrySet() {
        return tble.entrySet();
    }

    public Object clone() {
        TermMap c = new TermMap(null);
        c.tble = (Map)new Hashtable(this.tble).clone();
        return c;
    }

    public String stat() {
        return "TermMap: "+size();
    }
/*
    public String name() {
        return "{TermMap:"+size()+"}";
    }
    public int getArity() {
        return Term.CONST;
    }
*/
    /** This gives an enumeration view for the sequence of
         objects kept under key k.
    */
    synchronized public Enumeration toEnumerationFor(String k) {
        Object obj = get(k);
        if ( obj!=null && !(obj instanceof O1Queue) ) {
            IO.mes("% toEnumerationFor: "+k+"="+obj);
            return null;
        }
        O1Queue Q=(O1Queue)obj;
        Vector V=(Q==null)?new Vector():Q.toVector();
        return V.elements();
    }

    synchronized public Enumeration toEnumeration() {
        return new BBoardEnumerator(elements());
    }



    public Blackboard getBlackboard() {
        return new Blackboard(tble);
    }

















    /**
      registers a symbol as name of a builtin
    */
    public void register(IProceedure proto) {
        //IO.mes("registering builtin: "+key);
        put(proto.qname()+"/"+proto.getArity(),proto);
        //put(proto.getKey(),proto);
    }
    /**
      Creates a new builtin
    */
    public IProceedure getProceedure(Nonvar proto) {
        return (IProceedure)get(proto.qname()+"/"+proto.getArity());
        //return (IProceedure)get(proto.getKey());
    }

    /**
      Removes a matching ITerm from the blackboards and
      signals failure if no such term is found.
    */
    public ITerm cin(String k,ITerm pattern) {
        ITerm found=take(k,pattern);
        //if(found!=null) {
        //  found=found.matching_copy(pattern);
        //}
        if ( found==null ) return Atom.aNo;
        return Atom.the(found.copy());
    }

    private TermStack trail = new TermStack();

    public TermStack getTrail() {
        return trail;
    }
    public IKernel getParent() {
        return this;
    }
    public ITerm getElement() {
        return null;
    }
    public void stop() {
    }

    /**
      Adds a ITerm to the blackboard
    */
    public ITerm out(String k,ITerm pattern,boolean copying) {
        add(k,copying?pattern.copy():pattern);
        return Atom.aYes;
    }

    /**
      Adds a copy of a ITerm to the blackboard
    */

    synchronized public ITerm out(String key,ITerm pattern) {
        return out(key,pattern,true); //copies pattern
    }

    private void all0(int max,Vector To,String k,ITerm FXs) {
        if ( 0==max ) max=-1;
        Object obj=get(k);
        if ( null==obj || !(obj instanceof O1Queue)) return;
        O1Queue Q=(O1Queue)obj;
        // todo: use always the same "server's" trail
        for ( Enumeration e=Q.toEnumeration();e.hasMoreElements(); ) {
            ITerm t=(ITerm)e.nextElement(); if ( null==t ) break;
            t=t.matching_copy(FXs);
            if ( t!=null && 0!=max-- ) To.addElement(t);
        }
    }

    private ITerm all1(int max,ITerm FXs) {
        Vector To=new Vector();
        for ( Enumeration e=keys();e.hasMoreElements(); ) {
            all0(max,To,(String)e.nextElement(),FXs);   
        }
        Compound R=Compound.newCompound("$",To.size());
        //IO.mes("RR"+R);
        To.copyInto(R.getAllArgs());
        return((Cons)R.listify()).getDirectArg(1);
    }

    private ITerm all2(int max,String k,ITerm FXs) {
        if ( k==null ) {
            //IO.mes("expensive operation: all/2 with unknown key");
            return all1(max,FXs);
        }
        Vector To=new Vector();
        all0(max,To,k,FXs);
        if ( To.size()==0 ) return (ITerm)Atom.aNil;
        Compound R=Compound.newCompound("$",To.size());
        To.copyInto(R.getAllArgs());
        ITerm T=((Cons)R.listify()).getDirectArg(1);
        return T;
    }

    /**
       Returns a (possibly empty) list of matching ITerm objects
    */
    public ITerm all(String k,ITerm FX) {
        FX=all2(0,k,FX);
        return FX;
    }


    /**
       Gives an Enumeration view to the O1Queue
       of ITerm or HornClause objects stored at key k
       @see O1Queue
       @see ITerm
       @see HornClause
    */        /*
    public Enumeration toEnumerationFor(String k) {
        Enumeration E=super.toEnumerationFor(k);
        return E;
    }
                      */

    /**
       Returns a formatted String representation of this
       PrologBlackboard object
    */
    public String pprint() {
        StringBuffer s=new StringBuffer(name());
        Enumeration e=keys();
        while ( e.hasMoreElements() ) {
            s.append(pred_to_string((String)e.nextElement()));
            s.append("\n");
        }
        return s.toString();
    }


    public String pred_to_string(String key) {
        Object obj=get(key);
        if ( null==obj ) return null;
        if (!(obj instanceof O1Queue)) return "\n% key: "+key+"\n% obj: "+obj+"\n% class: "+obj.getClass().getName()+"\n\n";
        O1Queue Q=(O1Queue)obj;
        Enumeration e=Q.toEnumeration();
        StringBuffer s=new StringBuffer("\n% key: "+key+"\n\n");
        while ( e.hasMoreElements() ) {
            s.append(((ITerm)e.nextElement()).pprint(true));
            s.append(".\n");
        }
        s.append("\n");
        return s.toString();
    }

    /**
      consults or reconsults a Prolog file by adding or
      overriding existing predicates
      to be extended to load from URLs transparently
    */
    /* this.static */ public boolean fromFile(String f,boolean overwrite) {
        IO.traceln("last consulted file was: "+lastFile);
        boolean ok=fileToProg(f,overwrite);
        if ( ok ) {
            IO.traceln("last consulted file set to: "+f);
            lastFile=f;
        } else
            IO.errmes("error in consulting file: "+f);
        return ok;
    }

    /**
      reconsults a file by overwritting similar predicates in memory
    */
    /* this.static */ public boolean fromFile(String f) {
        return fromFile(f,true); 
    }

    public /* this.static */ String lastFile="jinni/kprolog/lib.pro";

    /**
      reconsults the last reconsulted file
    */
    /* this.static */ public boolean fromFile() {
        IO.println("begin('"+lastFile+"')");
        boolean ok=fromFile(lastFile);
        if ( ok )
            IO.println("end('"+lastFile+"')");
        return ok;
    }

    /* this.static */ private boolean fileToProg(String fname,boolean overwrite) {
        Reader sname=IO.toFileReader(fname);
        if ( null==sname ) return false;
        return streamToProg(fname,sname,overwrite); 
    }

    /**
      Reads a set of clauses from a stream and adds them to the
      blackboard. Overwrites old predicates if asked to.
      Returns true if all went well.
    */
    /* this.static */ public boolean streamToProg(Reader sname,boolean overwrite) {
        return streamToProg(sname.toString(),sname,overwrite);
    }

    /* this.static */ private boolean streamToProg(String fname,Reader sname,boolean overwrite) {
        ITermMap ktable=overwrite?(ITermMap)this.clone():null;
        //HornClause Err=new HornClause(p.useAtom("error"),new PrologVar());
        try {
            PrologParser p=new PrologParser(ktable,sname);
            apply_parser(p,fname,ktable);
        } catch ( Exception e ) { // already catched by readClause
            IO.errmes("unexpected error in streamToProg",e);
            return false;
        }
        return true;
    }

    /* this.static */ private void apply_parser(PrologParser p,String fname,ITermMap ktable) {
        for ( ;; ) {
            if ( p.atEOF() ) return;
            int begins_at=p.lineno();
            HornClause C=p.readClause();
            if ( null==C ) return;
            if ( p.isError(C) ) p.showError(C);
            else {
                //IO.mes("ADDING= "+C.pprint());
                processClause(C,ktable);
                C.setFile(fname,begins_at,p.lineno());
            }
        }
    }

    /**
      adds a HornClause to the joint Linda and Predicate table
    */
    /* this.static */ public void addClause(HornClause C,ITermMap ktable) {
        String k=C.getKey();
        // overwrites previous definitions
        if ( null!=ktable && null!=ktable.get(k) ) {
            ktable.remove(k);
            this.remove(k);
        }
        this.out(k,C,false);
    }

    /**
      adds a HornClause to the joint Linda and Predicate table
      @see HornClause
    */
    /* this.static */ public void processClause(HornClause C,ITermMap ktable) {
        if ( C.getHead().matches(useAtom("init")) ) {
            //IO.mes("init: "+C.getBody());
            firstSolution(C.getHead(),C.getBody());
        } else {
            //IO.mes("ADDING= "+C.pprint());
            addClause(C,ktable);
        }
    }   

    /** 
      Computes a copy of the first solution X of Goal G.
    */  

    public ITerm firstSolution(ITerm X, ITerm G) {
        IKernel p=new AnswerSource(new HornClause(X,G),this);
        ITerm A=p.getElement();
        if ( A!=null ) {
            A=Atom.the(A);
            p.stop();
        } else A=Atom.aNo;
        return A;
    }


    /**
       Removes the first ITerm having key k
       or the first enumerated key if k is null
    */
    synchronized private final ITerm pick(String k) {
        if ( k==null ) {
            Enumeration e=keys();
            if ( !e.hasMoreElements() ) return null;
            k=(String)e.nextElement();
            // IO.traceln("$$Got key:"+k+this);
        }
        Object obj = get(k);
        if ( obj!=null & !(obj instanceof O1Queue) ) {
            IO.mes("% TermMap.pick("+k+")="+obj);
            return null;
        }
        O1Queue Q=(O1Queue)obj;
        if ( Q==null ) return null;
        ITerm T=(ITerm)Q.deq();
        if ( Q.isEmpty() ) {
            remove(k);
            // IO.traceln("$$Removed key:"+k+this);
        }
        return T;
    }


    private final void addBack(String k,Vector V) {
        for ( Enumeration e=V.elements(); e.hasMoreElements(); ) {
            // cannot be here if k==null
            add(k,(ITerm)e.nextElement());
        }
    }

    /**
       Removes the first matching ITerm or HornClause from the
       blackboard, to be used by Linda in/1 operation in
       Blackboard
    
       @see Blackboard#in()
    */

    // synchronized 
    public final ITerm take(String k,ITerm pattern) {
        Vector V=new Vector();
        ITerm t;
        while ( true ) {
            t=pick(k);
            if ( null==t ) break;
            // IO.traceln("$$After pick: t="+t+this);
            if ( t.matches(pattern) ) break;
            else V.addElement(t);
        }
        addBack(k,V);
        return t;
    }

    /**
       Adds a ITerm or HornClause
       to the the blackboard, to be used by Linda out/1 operation
    
       @see Blackboard
    
    */
    synchronized public final void add(String k,ITerm value) {
        Object obj = get(k);
        if ( obj!=null & !(obj instanceof O1Queue) ) {
            IO.mes("% TermMap.add("+k+","+value+")="+obj);
            return;
        }
        O1Queue Q=(O1Queue)obj;
        if ( Q==null ) {
            Q=new O1Queue();
            put(k,Q);
        }
        if ( !Q.enq(value) ) IO.errmes("O1Queue full, key:"+k);
        // IO.traceln("$$Added key/val:"+k+"/"+value+"=>"+this);
    }




}

/**
   Generates an Enumeration view of the blackboard
   @see Enumeration
*/

class BBoardEnumerator extends Object implements Enumeration {
    public  BBoardEnumerator(Enumeration EH) {
        EQ=null;
        this.EH=EH; //elements();
    }

    private Enumeration EQ;
    private Enumeration EH;

    synchronized public boolean hasMoreElements() {
        if ( (EQ==null || !EQ.hasMoreElements()) && EH.hasMoreElements() ) {
            EQ=((O1Queue)EH.nextElement()).toEnumeration();
        }
        return(EQ!=null && EQ.hasMoreElements());
    }

    synchronized public Object nextElement() {
        if ( hasMoreElements() )
            return EQ.nextElement();
        throw new NoSuchElementException(this.getClass().getName());
    }

}

