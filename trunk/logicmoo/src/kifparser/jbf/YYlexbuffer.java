package jbf;

import java.lang.*;
import java.io.*;

public
class YYlexbuffer {

protected static final int EOF = -1;
protected static final int EOFCHAR = '\uffff';

// Keep a vector of characters around the current char;
Char_Vector lbuf;
// Keep a corresponding vector of position in line counts
Int_Vector cno;

protected int current; // index into yybuffer of current char
protected int thisch; // char at current

int mark; // index into yybuffer of start of lexeme

/* line and char info */
protected int charno0; // 0-base, char in file
protected int lineno; // 0-base line in file
protected int linenobase;

protected int linecount; // total # lines in buffer
protected int maxlines; // max total # lines kept in buffer

protected java.io.InputStream yyin;
protected java.io.PrintStream yyout;

public YYlexbuffer(InputStream fin, PrintStream fout)
    {this(fin,fout,2);}

public YYlexbuffer(InputStream fin, PrintStream fout, int max)
{
    yyin = fin;
    yyout = fout;
    maxlines = max;
    linecount = 0;
    lbuf = new Char_Vector();
    cno = new Int_Vector();
    current = -1;
    mark = -1;
    charno0 = -1;    
    lineno = -1;
    linenobase = -1;
    thisch = '\n'; // pretend
    yydebug = 0;
}

public int nextchar() throws IOException
{
    int lastch = thisch;
    current++;
    if(current >= lbuf.size()) {
        int ch = yyin.read();
//System.out.println("---------------------next char is: (" + ch + ")");
        current = lbuf.size();
        lbuf.addElement((char)ch);
        cno.addElement(linenobase);
        if(ch == '\n') linecount++;
    }
    // take char of EOF by proper casting
    thisch = (int)(short)lbuf.elementAt(current);
    charno0++;
    if(lastch == '\n') {
        lineno++;
        linenobase = charno0;
    }
    if(yydebug >= 100) {yyout.println("next/"+sch(thisch)
+"/"
+charno0()
+":"
+lineno()
+"+"
+charno()
+"%"
+linenobase
+"<"
+sch(lastch)
+">");}
    return thisch;
}

public void backup()
{
    int nextch = thisch;
    current--;
    thisch = lbuf.elementAt(current);
    // remember that newline is considered part
    // of the line it ends not the next line
    if(thisch == '\n') {
        lineno--;
        linenobase = cno.elementAt(current);
    }
    charno0--;
    if(yydebug >= 100) {yyout.println("push/"+sch(nextch)
+"/"
+charno0()
+":"
+lineno()
+"+"
+charno()
+"%"
+linenobase
+"<"
+sch(thisch)
+">");}
}

public int peek() throws IOException
{
    int ch = nextchar();
    backup();
    return ch;
}

public int lastchar()
{
    int ch = 0;
    if(current > 0) {
        ch = lbuf.elementAt(current-1);
    }
    return ch;
}

public int currentchar()
{
    int ch = 0;
    if(current >= 0 && current < lexemesize()) {
	ch = lbuf.elementAt(current);
    }
    return ch;
}

public void setmaxlines(int n)
{
    maxlines = n;
}

public void purge()
{
    int rem = linecount - maxlines;
    int len = lexemesize();
    int i;

    if(rem <= 0) return;
///**/yyout.println("purge:linecount="+linecount+" ; maxlines="+maxlines+" ; rem="+rem);
///**/yyout.println("purge: contents before=/"+contents()+"/");
    for(i=0;i<len && rem > 0;i++) {if(lbuf.elementAt(i) == '\n') rem--;}
    if(i > 0) {
        lbuf.removeElementRange(0,i); // purge thru the final newline
        cno.removeElementRange(0,i);
    }
    linecount = maxlines+rem;
    mark -= i;
    current -= i;
///**/yyout.println("purge:count="+i);
///**/yyout.println("purge: contents after=/"+contents()+"/");
}

// return 0 based line number
public int lineno() {return lineno;}

// return 0 based absolute char number
public int charno0() {return charno0;}

// return 0 based char number within line
public int charno() {return charno0 - linenobase;}

// Note, We assume that setmark is called JUST BEFORE
// the first character to be in the lexeme()
// => the lexeme begins at mark + 1
public void setmark()
{
    mark = current+1;
///**/yyout.print("mark=/");
///**/for(int k=mark;k<lexemesize();k++) yyout.print((char)lbuf.elementAt(k));
///**/yyout.println("/");
}

public void getlexemelocation(YYlocation loc)
{
    int l = lexemesize();
    int lno = lineno();
    if(lno < 0) lno = 0;
    loc.setlocation(charno0() - l + 1, lno, charno() - l + 1);

}

public void getlexeme(StringBuffer buf)
  {
    int len = lexemesize();
    buf.setLength(0);
    buf.append(lbuf.contents(),mark,len);
///**/System.out.println("YYlexbuffer.lexeme: len="+len+" mark="+mark+" lexeme=/"+buf+"/");
}

public void getlexeme(CharVector buf)
  {
    int len = lexemesize();
    buf.setLength(0);
    buf.append(lbuf.contents(),mark,len);
///**/System.out.println("YYlexbuffer.lexeme: len="+len+" mark="+mark+" lexeme=/"+buf+"/");
}

public int yydebug;

public void setdebug(boolean d) {yydebug = d?1:0;}
public void setdebug(int i) {yydebug = i>0?i:0;}

private String sch(int ch)
    {return (ch != EOF?String.valueOf((char)ch):"EOF");}

public int lexemesize() {return (current - mark) + 1;}

public Char_Vector contents() {return lbuf;}

public String toString()
{
    return new String(lbuf.contents(),mark,lexemesize());
}

public int currentpos()
{
    return current;
}

};
