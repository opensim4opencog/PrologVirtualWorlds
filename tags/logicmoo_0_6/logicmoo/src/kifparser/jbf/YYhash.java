/**************************************************/

package jbf;

/**************************************************/

import java.io.*;

/**************************************************/

/*
Get around the fact that StringBuffer is not usable
as a hash search object.
*/

class YYstring {
    public Object key; // String or StringBuffer

    public YYstring() {key = null;}
    public YYstring(String s) {key = s;}

    // assume that key must be either string or stringbuffer; same for o.
    public boolean equals(Object o)
    {
	if(!(o instanceof YYstring)) return false;
	YYstring yys = (YYstring)o;
	int i;
	String s1 = (key instanceof String)?((String)key):null;
	String s2 = (yys.key instanceof String)?((String)yys.key):null;
	StringBuffer sb1 = (key instanceof StringBuffer)?((StringBuffer)key):null;
	StringBuffer sb2 = (yys.key instanceof StringBuffer)?((StringBuffer)yys.key):null;

	if(s1 != null) {
	    if(s2 != null) return s1.equals(s2); // String(key) & String(o.key)
	    sb1 = sb2; // String(key) & StringBuffer(o.key)
	} else if(sb2 == null) { 
	    s1 = s2;  // StringBuffer(key) & String(o.key)
	}
	int l = sb1.length();
	if(s1 == null) { // 2 stringbuffers
	    if(l != sb2.length()) return false;
	    for(i=0;i<l;i++) {
		if(sb1.charAt(i) != sb2.charAt(i)) return false;
	    }
	    return true;
	} else { // string and stringbuffer
	    if(l != s1.length()) return false;
	    for(i=0;i<l;i++) {
		if(sb1.charAt(i) != s1.charAt(i)) return false;
	    }
	    return true;
	}
    }

    public int hashCode()
    {
	/* do something with the bytes in the string */
	int i = 0;
	int h = 0;
	int l;
	if(key instanceof String) {
	    l = ((String)key).length();
	    while(i<l) {h += (h<<3)+((String)key).charAt(i++);}
	} else {
	    l = ((StringBuffer)key).length();
	    while(i<l) {h += (h<<3)+((StringBuffer)key).charAt(i++);}
	}
	h = 1103515245*(h);
	if(h < 0) h = - h;
	return h;
    }

};

public
class YYhash extends java.util.Hashtable  {

    YYstring yys;

    public YYhash() {super(); yys = new YYstring();}

    public Object find(String s) { yys.key = s; return get(yys); }
    public Object find(StringBuffer sb) { yys.key = sb; return get(yys); }


    public void insert(StringBuffer s, Integer val) {insert(s.toString(),val);}
    public void insert(String s, Integer val)
	{put(new YYstring(s),val);}

    public void erase(String s) { yys.key = s; remove(yys); }
    public void erase(StringBuffer sb) { yys.key = sb; remove(yys); }

};
