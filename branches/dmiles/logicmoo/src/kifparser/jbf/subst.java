
package jbf;

import java.io.*;
import regexp.*;

public class subst {

static void usage(String msg)
{
    System.err.println("subst: "+msg);
    System.err.println("usage: subst {<rexexp> <replacement>}+");
    System.exit(1);
}

public static void main(String argv[]) throws IOException
{
    String line = null;
    StringBuffer linebuf = new StringBuffer();
    Regexp[] re;
    String[] repl;
    int pairs = argv.length / 2;
    int i;
    Result ok;
    int pos;
    DataInputStream in = new DataInputStream(System.in);

    if(argv.length < 2 || 2*pairs != argv.length) usage("too few arguments");
    re = new Regexp[pairs];
    repl = new String[pairs];
    for(i=0;i<pairs;i++) {
	re[i] = Regexp.compile(argv[2*i]);
	String a = argv[2*i+1];
	if(a.equals("''") || a.equals("\"\"")) repl[i] = ""; else repl[i] = a;
    }
    while((line = in.readLine()) != null) {
	for(i=0;i<pairs;i++) {
	    linebuf.setLength(0);
	    pos = 0;
	    while((ok = re[i].searchForward(line,pos)) != null) {
		int startpos = ok.getMatchStart();
		linebuf.append(line.substring(pos,startpos));
		linebuf.append(repl[i]);
		pos = ok.getMatchEnd();
	   }
	   if(pos < line.length()) linebuf.append(line.substring(pos));
	   line = linebuf.toString();
	}
	System.out.println(line);
    }
}

}; /*class Main*/
