package logicmoo.util;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.StringTokenizer;

public class StatTable extends Hashtable implements Cloneable {


	public StatTable() {
		super();
	}

	public StatTable(String list) {
		super();
		if(list != null && list.trim().length() > 0) {
			list = Util.replace(Util.replace(Util.replace(list, "\r", ""), "\n", ""), " ", "");
			for(StringTokenizer st = new StringTokenizer(list, ";"); st.hasMoreTokens(); ) {
				String[] n = Util.split(st.nextToken(), ":");
				n[1] = n[1].trim();
				if(n[1].length() > 0)
					put(n[0].trim(), n[1]);
			}
		}
	}



	public synchronized String stat(String name, String defaultVal) {
		Object o = get(name);
		if(o != null)
			return o.toString();
		else
			return defaultVal;
	}


   public synchronized int stat(String name, int defaultVal) {
		Object o = get(name);
		if(o != null) {
			if(o instanceof Integer)
				return ((Integer)o).intValue();
			else if(o instanceof String) {
				try {
					Integer i = Integer.valueOf((String)o);
					put(name, i);
					return i.intValue();
				}
				catch(Exception e) {
					return defaultVal;
				}
			}
		}
		return defaultVal;
   }


	public synchronized void set(String name, int value) {
		put(name, new Integer(value));
	}


	public synchronized void set(String name, String value) {
		put(name, value);
	}


	public synchronized String toString() {
		String s = "";
		for(Enumeration enum = keys(); enum.hasMoreElements(); ) {
			String k = (String) enum.nextElement();
			Object i = get(k);
			if(i != null)
				s += k + ":" + i.toString() + ";\n";
		}
		return s;
	}


}