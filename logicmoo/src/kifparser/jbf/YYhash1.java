/**************************************************/

package jbf;

/**************************************************/

import java.io.*;

/**************************************************/

/*
Get around the fact that StringBuffer is not usable
as a hash search object by using CharVector as key type
*/

public
class YYhash extends java.util.Hashtable  {

    public YYhash() {super(); yys = new YYstring();}

    public Object find(String s) { yys.key = s; return get(yys); }
    public Object find(StringBuffer sb) { yys.key = sb; return get(yys); }


    public void insert(StringBuffer s, Integer val) {insert(s.toString(),val);}
    public void insert(String s, Integer val)
	{put(new CharVector(s),val);}

    public void erase(String s) { yys.key = s; remove(yys); }
    public void erase(StringBuffer sb) { yys.key = sb; remove(yys); }

};
