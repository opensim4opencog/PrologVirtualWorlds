package  logicmoo.util;

import logicmoo.*; 
import logicmoo.obj.*; 
import java.util.*;
import org.opencyc.cycobject.*; 


public class FlagList {

    private static final char SPLIT = ';';

    private Vector elements;

    public FlagList() {
	elements = new Vector();
    }

    public FlagList(CycFort[] src) {
	this();
	for ( int I=0; I<src.length; I++ ) {
	    elements.add(src[I]);
	}
    }

    public FlagList(Enumeration src) {
	refresh(src);
    }

    public FlagList(FlagList src) {
	refresh(src.enumerate());
    }

    public FlagList(String src) {
	if ( src == null ) {
	    elements = new Vector();
	    return;
	}
	StringTokenizer st = new StringTokenizer(src, "" + SPLIT);
	refresh(st);
    }

    public FlagList(CycFort src) {
	elements = new Vector();
	elements.add(src);
    }

    public boolean contains(Object element) {
	return elements.contains(PartiallyTangible.makeCycFort(element).toString());
    }


    public void add(Object element) {
	if ( element == null ) return;
	if ( ! elements.contains(PartiallyTangible.makeCycFort(element).toString()) )
	    elements.add(PartiallyTangible.makeCycFort(element).toString());
    }

    public void rem(Object element) {
	if ( element == null ) return;
	if ( elements.contains(PartiallyTangible.makeCycFort(element).toString()) )
	    elements.remove(PartiallyTangible.makeCycFort(element).toString());
    }

    public int size() {
	return elements.size();
    }

    public void clear() {
	elements.clear();
    }

    public void refresh(Enumeration enum) {
	elements = new Vector();
	while ( enum.hasMoreElements() )
	    elements.add(enum.nextElement());
    }

    public Enumeration enumerate() {
	return elements.elements();
    }


    public Object clone() {
	return((Object) new FlagList(elements.elements()));
    }


    public String toString() {
	String o = "";
	Enumeration enum = enumerate();
	while ( enum.hasMoreElements() ) {
	    o = o + (CycFort)enum.nextElement() + SPLIT;
	}
	return o;
    }


    public static FlagList match(FlagList a, FlagList b) {
	FlagList c = new FlagList();
	for ( Enumeration enum = a.elements.elements(); enum.hasMoreElements(); ) {
	    Object s = enum.nextElement();
	    if ( b.elements.contains(s) )
		c.elements.add(s);
	}
	return c;
    }

}