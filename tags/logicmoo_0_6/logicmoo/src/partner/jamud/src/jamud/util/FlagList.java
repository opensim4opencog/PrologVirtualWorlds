package jamud.util;


import java.util.*;


public class FlagList implements Cloneable {


    private HashSet flags;



    public FlagList() {
	this.flags = new HashSet();
    }


    private FlagList(HashSet flags) {
	this.flags = flags;
    }


    public FlagList(FlagList src) {
	this.flags = (HashSet) src.flags.clone();
    }


    public FlagList(String src) {
	this.flags = new HashSet();
	if( src != null && (src=src.trim()).length() > 0 ) {
	    this.refresh(src);
	}
    }



    public synchronized boolean contains(String element) {
	return this.flags.contains(element);
    }


    public synchronized boolean add(String element) {
	return this.flags.add(element);
    }


    public synchronized boolean remove(String element) {
	return this.flags.remove(element);
    }


    public int size() {
	return this.flags.size();
    }


    public synchronized void clear() {
	this.flags.clear();
    }


    public synchronized void refresh(String src) {
	StringTokenizer st = new StringTokenizer(src, ";:,");
	refresh(st);
    }


    private void refresh(Enumeration enum) {
	flags.clear();
	while(enum.hasMoreElements()) {
	    String s = (String) enum.nextElement();
	    if(s != null) {
		s = s.trim();
		if(s.length() > 0) {
		    flags.add( s );
		}
	    }
	}
    }


    public synchronized void refresh(FlagList src) {
	for(Iterator i = src.flags.iterator(); i.hasNext(); ) {
	    this.flags.add( (String)i.next() );
	}
    }


    public synchronized Iterator iterator() {
	return this.flags.iterator();
    }
    

    public Object clone() {
	HashSet m = (HashSet) this.flags.clone();
	return new FlagList(m);
    }


    public synchronized String toString() {
	StringBuffer sb = new StringBuffer();
	synchronized(sb) {
	    for(Iterator i = this.flags.iterator(); i.hasNext(); ) {
		sb.append( (String)i.next() );
		sb.append( ',' );
	    }
	    return sb.toString();
	}
    }


    public synchronized FlagList match(FlagList src) {
	HashSet m = (HashSet) src.flags.clone();
	for(Iterator i = m.iterator(); i.hasNext(); ) {
	    if(! this.flags.contains(i.next()) ) {
		i.remove();
	    }
	}
	return new FlagList(m);
    }


    public synchronized boolean containBeginsWith(String flag) {
	if(flag != null && (flag=flag.trim()).length() > 0) {
	    for(Iterator i = this.flags.iterator(); i.hasNext(); ) {
		if( ((String)i.next()).startsWith(flag) ) {
		    return true;
		}
	    }
	}
	return false;
    }

}
