package logicmoo.util;

import java.util.*;
import logicmoo.*;
import logicmoo.obj.*;
import org.opencyc.cycobject.*;
import org.opencyc.util.*;

public class FlagList {


    private static final char SPLIT = ';';

    protected CycFort dataMt;
    protected CycFort cycobject;
    protected CycFort slot;
    protected CycFort type;

    public FlagList(CycFort dataMt,CycFort cycobject, CycFort slot, CycFort type){
	this.dataMt=dataMt;
	this.cycobject=cycobject;
	this.slot=slot;
	this.type=type;
    }

    public FlagList(CycFort dataMt,CycFort cycobject, CycFort slot, CycFort type, Object src){
	this.dataMt=dataMt;
	this.cycobject=cycobject;
	this.slot=slot;
	this.type=type;
	addAll(src);
    }

    public FlagList(PartiallyTangible object, CycFort slot, CycFort type) {
	this.dataMt = object.getMt();
	this.cycobject = object.getCycFort();
	this.slot=slot;
	this.type=type;
    }

    public FlagList(PartiallyTangible object, CycFort slot, CycFort type, Object src) {
	this.dataMt = object.getMt();
	this.cycobject = object.getCycFort();
	this.slot=slot;
	this.type=type;
	addAll(src);
    }

    public void addAll(Object src) {
	if ( src==null ) return;
	if ( src instanceof Object[] ) {
	    refresh((Object[]) src);
	} else if ( src instanceof Enumeration ) {
	    refresh((Enumeration) src);
	} else if ( src instanceof Collection ) {
	    refresh((Collection) src);
	} else if ( src instanceof Iterator ) {
	    refresh((Iterator) src);
	} else if ( src instanceof FlagList ) {
	    this.type = ((FlagList)src).type;
	    refresh(((FlagList)src).elements());
	} else if ( src instanceof String ) {
	    refresh(new StringTokenizer((String)src, "" + SPLIT));
	} else {
	    add(src);
	}
    }


    public boolean contains(Object element) {
	try {
	    return 
	    CycMoo.staticAccess.converseBoolean("(cyc-query '" + mkSlot(slot,cycobject,element).stringApiValue() + " " + dataMt.stringApiValue() + ")" );
	} catch ( Exception e ){
	    e.printStackTrace();
	    return false;
	}
    }

    public void add(Object element) {
	try {    
	    CycMoo.staticAccess.converseObject("(cyc-assert '" + mkSlot(slot,cycobject,element).stringApiValue() + " " + dataMt.stringApiValue() + ")" );
	} catch ( Exception e ){
	    e.printStackTrace();
	}
    }

    public void rem(Object element) {
	try {    
	    CycMoo.staticAccess.deleteGaf(mkSlot(slot,cycobject,element),dataMt);
	} catch ( Exception e ){
	    e.printStackTrace();
	}
    }

    public int size() {
	CycList query = new CycList(slot);
	query.add(cycobject);
	CycVariable cv = new CycVariable("Prev");
	query.add(cv);
	try {    
	    ArrayList res = CycMoo.staticAccess.askWithVariable(query,cv,dataMt);
	    return res.size();
	} catch ( Exception e ){
	    e.printStackTrace();
	    return 0;
	}
    }

    public void clear() {
	CycMoo.clearSlot(dataMt,cycobject,slot);
    }

    // Refreshes
    public void refresh(Object[] src) {
	for ( int I=0; I<src.length; I++ ) {
	    add(src[I]);
	}
    }

    public void refresh(Enumeration enum) {
	while ( enum.hasMoreElements() ) add(enum.nextElement());
    }

    public void refresh(Iterator enum) {
	while ( enum.hasNext() ) add(enum.next());
    }
    public void refresh(Collection enum) {
	refresh(enum.iterator());
    }

    public Enumeration enumerate() {
	CycList query = new CycList(slot);
	query.add(cycobject);
	CycVariable cv = new CycVariable("Prev");
	query.add(cv);
	try {    
	    ArrayList res = CycMoo.staticAccess.askWithVariable(query,cv,dataMt);
	    return(((Vector)(new Vector(res))).elements());
	} catch ( Exception e ){
	    e.printStackTrace();
	    return new Vector().elements();
	}
    }

    public Enumeration elements() {
	return enumerate();   
    }

    public Object clone() {
	return this;
    }


    public String toString() {
	String o = "";
	Enumeration enum = enumerate();
	while ( enum.hasMoreElements() ) {
	    o = o + (String)enum.nextElement() + SPLIT;
	}
	return o;
    }

    public CycList mkSlot(CycFort pred, CycFort arg1,Object element) {
	CycList cl = new CycList(slot);
	cl.add(cycobject);
	if ( type!=null ) {
	    CycFort obj = CycThing.makeCycFort(element);
	    try {    
		CycMoo.staticAccess.assertIsa(obj,type,dataMt);
	    } catch ( Exception e ){
		e.printStackTrace();
	    }
	    cl.add(obj);
	} else {
	    cl.add(element);
	}
	return cl;
    }

}


    /*
public static FlagList match(FlagList a, FlagList b) {
FlagList c = new FlagList();
for ( Enumeration enum = a.elements(); enum.hasMoreElements(); ) {
String s = (String) enum.nextElement();
if ( b.contains(s) )
c.add(s);
}
return c;
}     
*/          

