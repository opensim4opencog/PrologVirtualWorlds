package jbf;

import java.lang.*;

public
class YYterminal extends YYnode {

  public Integer tokentype;
  public String text;

  public YYterminal(Integer ttype, Object o)
  {
    settokentype(ttype);
    if(o == null || o instanceof String) {settext((String)o);}
    else if(o instanceof StringBuffer) {settext((StringBuffer)o);}
    else if(o instanceof CharVector) {settext((CharVector)o);}
  }
  public YYterminal(Integer ttype) {this(ttype,null);}

  public YYterminal(int ttype, Object o) {this(new Integer(ttype),o);}
  public YYterminal(int ttype) {this(new Integer(ttype));}

  public YYterminal(YYtoken t) {this(t.tokentype(),t.text());}

  public String toString() {return text;}

  public Integer tokentype() {return tokentype;}
  public void settokentype(Integer t) {tokentype = t;}
  public void settokentype(int t) {settokentype(new Integer(t));}

  public String text() {return text;}
  // Fill in the text part; may modify the text object
  public void settext(String x) {text = x;}
  public void settext(StringBuffer x) {settext(x.toString());}
  public void settext(CharVector x) {settext(x.toString());}
  public int length() {return text.length();}

  /* Add out converters for all the various types */
  public boolean boolean_value()
	{Boolean l = Boolean.valueOf(text); return l.booleanValue();}
  public char char_value() {return text.charAt(0);}
  public int integer_value()
	{Integer l = Integer.valueOf(text); return l.intValue();}
  public long long_value()
	{Long l = Long.valueOf(text); return l.longValue();}
  public double double_value()
	{Double l = Double.valueOf(text); return l.doubleValue();}
  public float float_value()
	{Float l = Float.valueOf(text); return l.floatValue();}
  public String string_value() {return toString();} // also covers Identifier

  /* Add in converters for all the various types */
  public void setvalue(boolean v) {text = v?"true":"false";}
  public void setvalue(char v) {text = String.valueOf(v);}
  public void setvalue(int v) {text = Integer.toString(v);}
  public void setvalue(long v) {text = Long.toString(v);}
  public void setvalue(float v) {text = Float.toString(v);}
  public void setvalue(double v) {text = Double.toString(v);}
  public void setvalue(String v) {text = v;}

  // Add some quoted string processing
  public void stripquotes()
  {
	int l1 = text.length()-1;
	StringBuffer b = new StringBuffer(text);
	
	if(l1 > 0) {for(int i=1;i<l1;i++) {b.setCharAt(i-1,b.charAt(i));}}
	b.setLength(l1-1);
	text = b.toString();
  }

  public void deescape() {
	int l = text.length();
	int j = 0;
	StringBuffer b = new StringBuffer(text);

	for(int i = 0;i<l;) {
	    char c = b.charAt(i++);
	    if(c != '\\' || i == l) {
		b.setCharAt(j++,c);
		continue;
	    }
	    c = b.charAt(i++);
	    if(c != 'u' && !isoctaldigit(c)) {
		b.setCharAt(j++,c);
		continue;
	    }
	    char digits[] = new char[4];
	    boolean isunicode = (c == 'u');
	    int digitcount = isunicode?4:3;
	    int k;
	    if(c != 'u') i--;
	    for(k=0;i<l && k<digitcount;k++) {
		c = b.charAt(i++);
		if(isoctaldigit(c) || (isunicode && ishexdigit(c))) {
		   digits[k] = c;
		} else {i--; break;}
	    }
	    // we have digitcount digits or we hit end of string
	    // or we hit a non-digit
	    if(k == 0) {
		b.setCharAt(j++,'u');
		continue;
	    }
	    int n = 0;
	    if(isunicode) {
		for(int d=0;d<k;d++) {n = n * 16 + hex2int(digits[d]);}
	    } else {
		for(int d=0;d<k;d++) {n = n * 8 + octal2int(digits[d]);}
	    }
	    b.setCharAt(j++,(char)n);
	}
	b.setLength(j);
	text = b.toString();
  }

  public boolean ishexdigit(char c)
	{return ((c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'f')
		|| (c >= 'A' && c <= 'F'));}

  public boolean isoctaldigit(char c) {return (c >= '0' && c <= '7');}

  public int hex2int(char c)
	{return (c >= '0' && c <= '9')?c - '0':
		((c >= 'a' && c <= 'f')?c - 'a':c-'A');}

  public int octal2int(char c) {return c - '0';}

};

