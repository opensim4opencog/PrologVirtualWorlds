package jbf;

import java.lang.*;
import java.util.*;

public
class YYnonterminal extends YYnode {
    
    public Vector nodes;

    public YYnonterminal() { nodes = new Vector(); }
    public YYnonterminal(int kind) { this(); append(kind); }
    public YYnonterminal(int kind, Object o) { this(kind); append(o); }
    public YYnonterminal(int kind, Object o1, Object o2)
	{ this(kind); append(o1); append(o2);}
    public YYnonterminal(int kind, Object o1, Object o2, Object o3)
	{ this(kind); append(o1); append(o2); append(o3);}
    public YYnonterminal(int kind, Object o1, Object o2, Object o3, Object o4)
	{ this(kind); append(o1); append(o2); append(o3); append(o4); }

    public int length() { return nodes.size(); }
    public Vector nodes() {return nodes;}

    public void append(Object o)
    {
	if(o != null) {
	    if(o instanceof YYtoken) {
		nodes.addElement(new YYterminal((YYtoken)o));
	    } else {
		nodes.addElement(o);
	    }
	}
    }
    public void append(int i) { append(new Integer(i)); }

    public Object ith(int i) { return nodes.elementAt(i); }

    public String toString() 
    {
	StringBuffer b;
	int i;
	b = new StringBuffer("(");
	for(i=0;i<nodes.size();i++) {
	    if(i > 0) b.append(" ");
	    b.append(ith(i).toString());
	}
	b.append(")");
	return b.toString();
    }

};

