package logicmoo.obj;


import logicmoo.util.*;
import java.lang.*;
import java.util.*;
import net.n3.nanoxml.*;
import logicmoo.*;
import org.opencyc.api.*;
import org.opencyc.creation.*;
import org.opencyc.cycobject.*;
import org.opencyc.kif.*;
import org.opencyc.util.*;



public class Inventory /* implements Collection, List*/ {
    public static final String MARKUP = "INVENTORY";
    
    protected FlagList flaglist = null ;
    public Inventory(CycFort dataMt,CycFort cycobject, CycFort slot, CycFort type) {
	 flaglist = new FlagList(dataMt,cycobject,slot,type);
    }
    public int size() {
	return flaglist.size();
    }
    public boolean isEmpty() {
	return (flaglist.size()==0);
    }
    public boolean contains(java.lang.Object A) {
	return flaglist.contains(A);
    }
    public java.util.Iterator iterator() {
	return Collections.list(flaglist.enumerate()).iterator();
    }
    public java.lang.Object[] toArray() {
	return Collections.list(flaglist.enumerate()).toArray();
    }
    public boolean addAll(java.util.Collection A) {
	flaglist.refresh(A);
	return true;
    }

    public boolean add(java.lang.Object A) {
	flaglist.add(A);
	return true;
    }

    public boolean remove(java.lang.Object A) {
	if (contains(A)) {
	    flaglist.rem(A);
	    return true;
	}
	return false;
    }

    public boolean containsAll(java.util.Collection A) {
	Iterator B = A.iterator();
	while (B.hasNext()) {
	    if (!flaglist.contains(B.next())) return false;
	}
	return true;
    }

    public boolean equals(java.lang.Object A) {
	if (A==this) return true;
	if (!(A instanceof Collection)) return false;
	return containsAll((Collection)A);
    }

    public boolean removeAll(java.util.Collection A) {
	Iterator B = A.iterator();
	while (B.hasNext()) flaglist.rem(A);
	return true;
    }

    public boolean retainAll(java.util.Collection A) {
	Iterator B = A.iterator();
	while (B.hasNext()) {
	    Object C = B.next();
	    if (!flaglist.contains(C)) flaglist.rem(C);
	}
	return true;
    }    
    public void clear() {
	flaglist.clear();
    }
    public Enumeration elements() {
	return flaglist.elements();
    }
    
    public Item getItem(String key) {
	return(Item) getViewable(key);
    }

    public Item getItem(int offset, String key) {
	return(Item) getViewable(offset, key);
    }


    public Item fetchItem(String key) {
	return(Item) fetchViewable(key);
    }


    public Item fetchItem(int offset, String key) {
	return(Item) fetchViewable(offset, key);
    }

			 
    public static Inventory loadInventory(CycFort dataMt, CycFort cycobject, CycFort slot, CycFort type, XMLElement xml) throws Exception {
	if ( ! xml.getName().equals(MARKUP) )
	    throw new Exception("non-" + MARKUP + " element in Inventory.loadInventory");

	Inventory tmp = new Inventory(dataMt,cycobject,slot,type);

	for ( Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); )
	    tmp.add(Item.loadItem((XMLElement) enum.nextElement()));

	return tmp;
    }				      


    public XMLElement toXMLElement() {
	XMLElement xml = new XMLElement();
	xml.setName(MARKUP);

	for ( Enumeration enum = this.elements(); enum.hasMoreElements(); )
	    xml.addChild( ((Item)enum.nextElement()).toXMLElement() );

	return xml;
    }
   /*
    public static void saveInventory(CycFort mt,Inventory inv, String slot,CycFort where) {
	Iterator inside =  inv.iterator();
	while ( inside.hasNext() ) {
	    Object it = inside.next();
	    if ( it instanceof Item ) {
		Item itm = (Item)it;
		itm.saveCyc();
		CycMoo.assertSlotValue(mt,itm.getCycFort(),slot,where,true);
	    } else if ( it instanceof PartiallyTangible ) {
		((PartiallyTangible)it).saveCyc();
		CycMoo.assertSlotValue(mt,((PartiallyTangible)it).getCycFort(),slot,where,true);
	    } else if ( it instanceof CycThing ) {
		((CycThing)it).saveCyc();
		CycMoo.assertSlotValue(mt,((CycThing) it).getCycFort(),slot,where,true);
	    }
	}
    }
		  */

    public PartiallyTangible getViewable(String key) {
	if ( key == null || key.length() == 0 )
	    return null;

	key = key.toLowerCase();
	String[] t = Util.split(key, '.');

	int I = 0;
	if ( t[1].length() > 0 ) {
	    try {
		I = Integer.parseInt(t[0]);
		if ( I < 0 )
		    I = 0;
	    } catch ( Exception e ) {
		return null;
	    }
	    key = t[1];
	}

	return getViewable(I, key);
    }


    public PartiallyTangible getViewable(int offset, String key) {
	for ( Enumeration enum = this.elements(); enum.hasMoreElements(); ) {
	    PartiallyTangible i = (PartiallyTangible) enum.nextElement();
	    for ( Enumeration kw = i.getKeywords().enumerate(); kw.hasMoreElements(); )
		if ( ( (String)kw.nextElement() ).toLowerCase().startsWith(key) ) {
		    if ( offset == 0 )
			return i;
		    else if ( offset < 0 )
			return null;
		    else {
			offset--;
			break;
		    }
		}
	}
	return null;
    }


    public PartiallyTangible fetchViewable(String key) {
	if ( key == null || key.length() == 0 )
	    return null;

	key = key.toLowerCase();
	String[] t = Util.split(key, '.');

	int I = 0;
	if ( t[1].length() > 0 ) {
	    try {
		I = Integer.parseInt(t[0]);
		if ( I < 0 )
		    I = 0;
	    } catch ( Exception e ) {
		return null;
	    }
	    key = t[1];
	}

	return fetchViewable(I, key);
    }


    public PartiallyTangible fetchViewable(int offset, String key) {
	for ( Enumeration enum = this.elements(); enum.hasMoreElements(); ) {
	    PartiallyTangible i = (PartiallyTangible) enum.nextElement();
	    for ( Enumeration kw = i.getKeywords().enumerate(); kw.hasMoreElements(); )
		if ( ( (String) kw.nextElement() ).toLowerCase().startsWith(key) ) {
		    if ( offset == 0 ) {
			this.remove(i);
			return i;
		    } else if ( offset < 0 )
			return null;
		    else {
			offset--;
			break;
		    }
		}
	}
	return null;
    }


    public String getNameView() {
	String r = "";

	for ( Enumeration enum = this.elements(); enum.hasMoreElements(); )
	    r += "\n " + ((PartiallyTangible) enum.nextElement()).getShortDesc();

	if ( r.length() > 1 )
	    r = r.substring(1);

	return r;       
    }


    public String getNameView(boolean combine) {
	if ( ! combine )
	    return toString();

	Vector a = new Vector(), count = new Vector();
	for ( Enumeration enum = this.elements(); enum.hasMoreElements(); ) {
	    String s = ((PartiallyTangible)enum.nextElement()).getName();
	    int i = a.indexOf(s);
	    if ( i != -1 )
		count.set(i, new Integer(((Integer)count.get(i)).intValue() + 1));
	    else {
		a.add(s);
		count.add(new Integer(1));
	    }
	}

	String r = "";
	for ( int i = 0; i < a.size(); i++ ) {
	    int j = ((Integer)count.get(i)).intValue();
	    r += ((j>1) ? ("\n (" + Util.fixLength("" + j, 2) + ") ") : "\n      " ) + (String)a.get(i);
	}

	a.clear();
	count.clear();

	if ( r.length() > 1 )
	    r = r.substring(1);

	return r;
    }


    public String getNameView(Body viewer, boolean combine) {
	return getNameView(combine);
    }


    public String getShortDescView() {
	String r = "";

	for ( Enumeration enum = this.elements(); enum.hasMoreElements(); )
	    r += "\n " + ((PartiallyTangible) enum.nextElement()).getShortDesc();

	if ( r.length() > 1 )
	    r = r.substring(1);

	return r;       
    }


    public String getShortDescView(boolean combine) {
	if ( ! combine )
	    return toString();

	Vector a = new Vector(), count = new Vector();
	for ( Enumeration enum = this.elements(); enum.hasMoreElements(); ) {
	    String s = ((PartiallyTangible)enum.nextElement()).getShortDesc();
	    int i = a.indexOf(s);
	    if ( i != -1 )
		count.set(i, new Integer(((Integer)count.get(i)).intValue() + 1));
	    else {
		a.add(s);
		count.add(new Integer(1));
	    }
	}

	String r = "";
	for ( int i = 0; i < a.size(); i++ ) {
	    int j = ((Integer)count.get(i)).intValue();
	    r += ((j>1) ? ("\n (" + Util.fixLength("" + j, 2) + ") ") : "\n      " ) + (String)a.get(i);
	}

	a.clear();
	count.clear();

	if ( r.length() > 1 )
	    r = r.substring(1);

	return r;
    }


    public String getShortDescView(Body viewer, boolean combine) {
	return getShortDescView(combine);
    }



}
