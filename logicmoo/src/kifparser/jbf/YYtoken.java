package jbf;

import java.lang.*;

public
class YYtoken {

  public Integer tokentype;
  public YYlocation loc;
  public StringBuffer text; // of the token

  public YYtoken() {this((Integer)null);}
  public YYtoken(Integer ttype, String s) {this(ttype); settext(s);}
  public YYtoken(Integer ttype, StringBuffer s) {this(ttype); settext(s);}
  public YYtoken(Integer ttype, CharVector s) {this(ttype); settext(s);}

  public YYtoken(Integer ttype, int cn0, int ln, int cn)
	{this(ttype); setlocation(cn0,ln,cn);}

  public YYtoken(YYtoken t)
	{this(t.tokentype(), t.text()); setlocation(t.location());}

  public YYtoken(Integer ttype)
  {
	settokentype(ttype);
	text = new StringBuffer();
	loc = new YYlocation();
  }

  public String toString() {return text.toString();}

  public void clear() {settokentype((Integer)null); text.setLength(0);}
  public int length() {return text.length();}
  public StringBuffer text() {return text;}

  public void chars(Char_Vector v)
  {
    int l = text.length();
    v.setSize(l);
    char[] vc = v.contents();
    text.getChars(0,l,vc,0);
  }

  public Integer tokentype() {return tokentype;}
  public YYlocation location() {return loc;}
  public void settokentype(Integer k) {tokentype = k;}

  public void settext(YYlexbuffer x) {x.getlexeme(text);}

  public void settext(String x) {text.setLength(0); text.append(x);}
  public void settext(StringBuffer x)
	{text.setLength(0); text.append(x.toString());}
  public void settext(CharVector x)
	{text.setLength(0); text.append(x.toString());}
  public void settext(char[] x) {text.setLength(0); text.append(x);}

  public void setlocation(int l, int c0, int c) {loc.setlocation(l,c0,c);}
  public void setlocation(YYlocation l) {loc.setlocation(l);}

  // Pass throughs
  public int lineno() {return loc.lineno;} // 0 based line#
  public int charno0() {return loc.charno0;} // 0 based absolute char#
  public int charno() {return loc.charno;} // 0 based char# in line

  // Following is mostly for use by parsers

};
