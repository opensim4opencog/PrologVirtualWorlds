package  logicmoo.obj;

import java.util.*;
import logicmoo.util.*;
import org.opencyc.cycobject.*; import logicmoo.*;

public class ReifiableGroup extends Vector {


    public PartiallyTangible getReifiable(CycFort key)  {
	return  getReifiable(key.toString());

    }

    public PartiallyTangible getReifiable(String key) {
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

	return getReifiable(I, key);
    }

    public PartiallyTangible getReifiable(int offset, CycFort key)  {
	return  getReifiable(offset,key.toString());
    }

    public PartiallyTangible getReifiable(int offset, String key) {
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


    public PartiallyTangible fetchReifiable(CycFort key)  {
	return  fetchReifiable(key.toString());
    }

    public PartiallyTangible fetchReifiable(String key) {
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

	return fetchReifiable(I, key);
    }


    public PartiallyTangible fetchReifiable(int offset,CycFort key)  {
	return  fetchReifiable(offset,key.toString());
    }

    public PartiallyTangible fetchReifiable(int offset, String key) {
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
	    r += "\n " + ((PartiallyTangible) enum.nextElement()).getPrettyName();

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


    public String getNameView(AgentGeneric viewer, boolean combine) {
	return getNameView(combine);
    }


    public String getPrettyNameView() {
	String r = "";

	for ( Enumeration enum = this.elements(); enum.hasMoreElements(); )
	    r += "\n " + ((PartiallyTangible) enum.nextElement()).getPrettyName();

	if ( r.length() > 1 )
	    r = r.substring(1);

	return r;       
    }


    public String getPrettyNameView(boolean combine) {
	if ( ! combine )
	    return toString();

	Vector a = new Vector(), count = new Vector();
	for ( Enumeration enum = this.elements(); enum.hasMoreElements(); ) {
	    String s = ((PartiallyTangible)enum.nextElement()).getPrettyName();
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


    public String getPrettyNameView(AgentGeneric viewer, boolean combine) {
	return getPrettyNameView(combine);
    }


}