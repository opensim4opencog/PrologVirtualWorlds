
package jbf;

import java.io.*;
import java.util.*;
import regexp.*;

class JBpatterns {

static final String Nullstring = "";

static Regexp numdef = Regexp.compile("^#define\\s+(\\w+)\\s+(\\S.*)");

static Regexp tabledef = Regexp.compile("^static\\s+const[^[]+\\[");
static Regexp tabledef1 = Regexp.compile("^static\\s+const\\s+char\\s+(\\w+)\\[\\]");
static Regexp tabledef2 = Regexp.compile("^static\\s+const\\s+short\\s+(\\w+)\\[\\]");
static Regexp tabledef3 = Regexp.compile("^static\\s+const\\s+char\\s+\\*\\s+const\\s+(\\w+)\\[\\]");

static Regexp prefix = Regexp.compile("^#line\\s+(\\d+)\\s+\"([^\"]*)\"");
static Regexp suffix = Regexp.compile("^\\}$");
static Regexp actions = Regexp.compile("\\s*switch\\s*\\(\\s*yyn\\s*\\)");
static Regexp unionspec = Regexp.compile("^\\s*typedef\\s+union\\s+{");
static Regexp transmac = Regexp.compile("#define\\s+YYTRANSLATE\\D+(\\d+)\\D+(\\d+)");

static Regexp starttab = Regexp.compile("static[^{]*{(.*)");
static Regexp endtab = Regexp.compile("};\\s*");

static Regexp endprefix = Regexp.compile("^#");

static Regexp linedir = Regexp.compile("#line\\s(\\d+)\\s+\"([^\"]*)\"");

static Regexp endunion = Regexp.compile("\\}\\s+YYSTYPE\\s*;");
static Regexp unionsplit = Regexp.compile("(\\w+)");

static Regexp ytnameelement = Regexp.compile("\"[^\"]*\"[^,]*,");
static Regexp ytnamenull = Regexp.compile(",\\s+NULL");

//static String ytnamequote = "\"'\\\"'\"";
//static Regexp whitespace = Regexp.compile("\\s*");

static Regexp nontermconst = Regexp.compile("[\"]([^\"]+)[\"]");

static Regexp endaction = Regexp.compile("\\s*/\\* the action file gets copied in in place of this dollarsign \\*/");
static Regexp dollarindex = Regexp.compile("\\[(-?\\d+)\\]");
static Regexp discrim = Regexp.compile("\\.(\\w+)");
static Regexp eqfollows = Regexp.compile("\\s*=");

static Regexp atsign = Regexp.compile("@([^@]+)@");

}; //class Pattern

public
class jb {

// define the acceptable bison versions
static final int bisonvermin = 124;
static final int bisonvermax = 125;
static final String tablemodifiers = "public static final";

static String defaulttype = "Integer";

static Vector builtintypes = new Vector();

static
{
    builtintypes.addElement("Object");
    builtintypes.addElement("Integer");
    builtintypes.addElement("Byte");
    builtintypes.addElement("Short");
    builtintypes.addElement("Long");
    builtintypes.addElement("Float");
    builtintypes.addElement("Double");
    builtintypes.addElement("Boolean");
    builtintypes.addElement("String");
}

//////////////////////////////////////////////////

static Namedhash options = new Namedhash("options");
static Namedhash flags = new Namedhash("flags");
static Namedhash parms = new Namedhash("parms");
static Namedhash nonterms = new Namedhash("nonterms");
static Namedhash tokentypes = new Namedhash("tokentypes");
static Vector sortedtokentypes = null;
static Vector sortednonterms = null;

static String lexinname = null;

// indicate if we have the actions section
static boolean noactions = true;

static boolean haveprefix = false;

// indicate state of getting tokens
// 0 => before; 1 =>during ; 2=>after
static int gettingtokens = 0;

static String tabcpushback = null;

//////////////////////////////////////////////////

static FileInputStream fsrc = null;
static DataInputStream src = null;
static FileOutputStream ftarget = null;
static PrintStream target = null;

//////////////////////////////////////////////////

static String line = null;
static Result match = null;
static Result ok = null;
static StringBuffer buf = new StringBuffer();

//////////////////////////////////////////////////

static Vector hashnamevec(Hashtable t)
{
    Vector v = new Vector();
    Enumeration e = t.keys();
    while(e.hasMoreElements()) {
	v.addElement(e.nextElement());
    }
    return v;
}

static String hashnames(Hashtable t)
{
    StringBuffer b = new StringBuffer();
    Enumeration e = t.keys();
    while(e.hasMoreElements()) {
	b.append(" "+(String)e.nextElement());
    }
    return b.toString();
}

static void put(Namedhash t, Object key, Object val)
{
    t.put(key,val);
//System.err.println(t.name()+"("+key+") = /"+val+"/");
}

static boolean lmember(Vector v, String val)
{
    for(int i=0;i<v.size();i++) {
	if(val.equals((String)v.elementAt(i))) return true;
    }
    return false;
}

// unique append
static void lunion(Vector v, String val)
{
    if(lmember(v,val)) return;
    v.addElement(val);
}

static boolean match(String l, Regexp re)
{
    return ((match = re.searchForward(l,0)) != null);
}

static String substitute(String s, Regexp re, String replacement)
{
    StringBuffer b = new StringBuffer();
    Result ok;
    int pos1 = 0;
    int pos2 = 0;
    while ((ok = re.searchForward(s,pos1)) != null) {
	pos2 = ok.getMatchStart();
	if(pos2 > 0) b.append(s.substring(pos1,pos2));
	b.append(replacement);
	pos1 = ok.getMatchEnd();
    }
    if(pos1+1 < s.length()) b.append(s.substring(pos1));
    return b.toString();
}

static int stringmin(int i1, int i2)
{
    if(i1 < 0) return i2;
    if(i2 < 0) return i1;
    if(i1 > i2) return i2;
    return i1;
}

static boolean have(String nm)
{
    return parms.get(nm) != null;
}

static String uniondefault()
{
    return (String)((Vector)parms.get("union")).lastElement();
}

//////////////////////////////////////////////////

static String nextline() throws IOException
{
    if(tabcpushback != null) {
	line = tabcpushback;
	tabcpushback = null;
    } else {
	line = src.readLine();
    }
    return line;
}

static void pushback(String l)
{
    tabcpushback = l;
}

//////////////////////////////////////////////////

static void Numdef(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    String varnm = match.getMatch(1);
    String val = match.getMatch(2);

    if(varnm.equals("YYBISON")) {
	gettingtokens = 1;
	return;
    }
    if(varnm.equals("YYFINAL")
	|| varnm.equals("YYFLAG")
	|| varnm.equals("YYNTBASE")
	|| varnm.equals("YYLAST")) {
	put(parms,varnm,val);
	return;
    }
    if(gettingtokens == 1) {
	// should be one of the token types if the value is an integer
	try {put(tokentypes,varnm,Integer.valueOf(val));}
	catch (Exception e) {}
    }
    // else ignore
}

static void Transmac(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    String c1 = match.getMatch(1);
    String c2 = match.getMatch(2);
    String body = "static final int YYTRANSLATE(int x)\n\t{ return ((x) <= "
			+c1+" ? yytranslate[x] : "+c2+");}\n";
    put(parms,"YYTRANSLATE",body);
}

static void Tabledef(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    if((ok = JBpatterns.tabledef1.searchForward(line,0)) == null) {
	if((ok = JBpatterns.tabledef2.searchForward(line,0)) == null) {
	    ok = JBpatterns.tabledef3.searchForward(line,0);}}
    if(ok == null) return; // false alarm
    String nm = ok.getMatch(1);

    if((ok = JBpatterns.starttab.searchForward(line,0)) == null) {
	error("Tabledef: malformed table line");
    }

    StringBuffer tab = buf;
    tab.setLength(0);
    tab.append(ok.getMatch(1)+"\n");

    while((line = nextline()) != null) {
	ok = JBpatterns.endtab.searchForward(line,0);
	if(ok != null) {
	    tab.append("\n");
	    put(parms,nm,tab.toString());
	    return;
	}
	tab.append(line+"\n");
    }
    error("eof detected while scanning table");
}

static void Prefix(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    if(haveprefix) return;
    // this should also mark the end of the token collection
    gettingtokens = 2;
    haveprefix = true;
    // now, absorb lines;
    // everything from here to next preprocessor line
    // or the union typedef line is assumed prefix code
    StringBuffer body = buf;
    body.setLength(0);
    while((line = nextline()) != null) {
	// figure out when to quit
	if((ok =  JBpatterns.endprefix.searchForward(line,0)) != null) {
	    put(parms,"prefix",body.toString());
	    return;
	}
	if((ok =  JBpatterns.unionspec.searchForward(line,0)) != null) {
	    put(parms,"prefix",body.toString());
	    pushback(line);
	    return;
	}
	body.append(line+"\n");
    }
    error("eof detected while scanning user defined prefix code: $body");
}

static void Suffix(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    if(noactions) return;
    // now, absorb lines to end of file
    StringBuffer body = buf;
    body.setLength(0);
    while((line = nextline()) != null) {
	// suppress line directives
	if((ok =  JBpatterns.linedir.searchForward(line,0)) != null) continue;
	body.append(line+"\n");
    }
    put(parms,"suffix",body.toString());
}

static void Actions(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    StringBuffer body = buf;
    StringBuffer vsp = new StringBuffer();
    body.setLength(0);
    while ((line = nextline()) != null) {
	if(JBpatterns.endaction.searchForward(line,0) != null) {
	    put(parms,"actions",body.toString());
	    noactions = false;
	    return;
	}
	// suppress line directives
	if(JBpatterns.linedir.searchForward(line,0) != null) continue;
	// fix up references to yyvsp and yyval
	int pos = 0;
	while (true) {
	    int i1 =line.indexOf("yyvsp",pos);
	    int i2 =line.indexOf("yyval",pos);
	    if(i1 < 0 && i2 < 0) {
		body.append(line.substring(pos));
		break;
	    }
	    int nxt = stringmin(i1,i2);
	    body.append(line.substring(pos,nxt));
	    boolean iseq = false;
	    pos = nxt + 5;
	    String yy;
	    String typ;
	    if(i1 == nxt) { // yyvsp case
		if((ok= JBpatterns.dollarindex.searchForward(line,pos)) != null){
		    String depth = ok.getMatch(1);
		    pos = ok.getMatchEnd();
		    yy = "dollar("+depth+")";
		} else {
		    // badly formed, so just pass it thru
		    body.append("yyvsp");
		    continue;
		}
		if((ok = JBpatterns.discrim.searchForward(line,pos)) != null
		   && ok.getMatchStart() == pos) {
		    typ = ok.getMatch(1);
		    pos = ok.getMatchEnd();
	    	} else {typ = JBpatterns.Nullstring;}
	    } else { // yyval case
		yy = "yyval";
		if((ok= JBpatterns.discrim.searchForward(line,pos)) != null
		   && ok.getMatchStart() == pos) {
		    typ = ok.getMatch(1);
		    pos = ok.getMatchEnd();
	    	} else {typ = JBpatterns.Nullstring;}
		if((ok= JBpatterns.eqfollows.searchForward(line,pos)) != null
		   && ok.getMatchStart() == pos) {
		    pos = ok.getMatchEnd();
		    iseq = true;
		}
	    }
//System.err.println("yy="+yy+" ; typ="+typ+" ; iseq="+iseq);
	    vsp.setLength(0);
	    if(have("union")) {
		if(typ.equals(JBpatterns.Nullstring)) {typ = uniondefault();}
		if(iseq) {
		    if(lmember((Vector)parms.get("union"),typ)) {
			vsp.append(yy+" =");
		    } else {
			vsp.append(yy+"."+typ+" =");
		    }
		} else {
		    if(lmember((Vector)parms.get("union"),typ)) {
			vsp.append("(("+typ+")"+yy+")");
		    } else {
			vsp.append(yy+"."+typ);
		    }
		}
	    } else { // !have(union)
		if(typ == JBpatterns.Nullstring) {
		    vsp.append(yy);
		} else {
		    vsp.append(yy+"."+typ);
		}
		if(iseq) {vsp.append(" =");}
	    }
//System.err.println("vsp=/"+vsp+"/");
	    body.append(vsp);
	}
	body.append("\n");
    }
    error("eof detected while scanning action switch body: $body");
}

static void Union(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    StringBuffer union = buf;
    buf.setLength(0);
    // concat together all lines thru the next right brace
    while (true) {
	union.append(line);
	if(line.indexOf("}") >= 0) break;
	if((line = nextline()) == null) {
	    error("eof detected while scanning union type");
	}
    }
    String u = union.toString();
// System.err.println("Union: union="+u);
    u = substitute(u,JBpatterns.endunion,JBpatterns.Nullstring);
// System.err.println("Union: union="+u);
    u = substitute(u,JBpatterns.unionspec,JBpatterns.Nullstring);
// System.err.println("Union: union="+u);
    int pos = 0;
    Vector arms = new Vector();
    String dfalt = null;
    int dfaltseen = 0;
    while((ok=JBpatterns.unionsplit.searchForward(u,pos)) != null) {
	String typ = ok.getMatch(1);
//System.err.println("Union: typ="+typ);
	pos = ok.getMatchEnd();
	if(typ.equals("default")) {
	    if(dfaltseen++ > 0) error("union has multiple defaults");
	} else if(dfaltseen == 1) {
	    dfalt = typ;
	    dfaltseen++;
	} else {
	    arms.addElement(typ);
	}
    }
//System.err.println("Union: dfalt="+dfalt+" ; dfaltseen="+dfaltseen);
    if(dfalt == null) {
	if(dfaltseen > 0) error("union default has no argument");
	dfalt = defaulttype;
    }
    for(int i=0;i<builtintypes.size();i++) {
	arms.addElement(builtintypes.elementAt(i));
    }
    arms.addElement(dfalt);
    put(parms,"union",arms);
}

//////////////////////////////////////////////////

static void gettokentypes() throws IOException
{
    while((line = nextline()) != null) {
	if((ok=JBpatterns.numdef.searchForward(line,0)) == null) break;
	// should be one of the token types
	String nm = ok.getMatch(1);
	String val = ok.getMatch(2);
//System.err.println("token: "+nm+" = "+val);
	put(tokentypes,nm,Integer.valueOf(val));
    }
    if(line == null) {
	error("eof detected while looking for token definitions");
    }
}

//////////////////////////////////////////////////

static
void
scanline(String line) throws IOException
{
    // do some pre-checks to speed up processing
    if(line.length() > 0 && line.charAt(0) == '#') {
	if(line.startsWith("#define YYTRANSLATE")) {
	    Transmac(line,JBpatterns.transmac);
	    return;
	}
	if(line.startsWith("#define")) {
	    Numdef(line,JBpatterns.numdef);
	    return;
	}
	if(line.startsWith("#line")) {
	    Prefix(line,JBpatterns.prefix);
	    return;
	}
    }
    if(line.length() > 0 && line.charAt(0) == '}') {
	Suffix(line,JBpatterns.suffix);
	return;
    }
    if(line.startsWith("static const")) {
	Tabledef(line,JBpatterns.tabledef);
	return;
    }
    if(line.indexOf("switch") >= 0) {
	Actions(line,JBpatterns.actions);
	return;
    }
    if(line.indexOf("union") >= 0) {
	Union(line,JBpatterns.unionspec);
	return;
    }
}

static void buildnttable()
{
    // first, we need to extract the list of nonterms from yytnames
    String yyt = (String)parms.get("yytname");
    int yytl = yyt.length();
    int pos = 0;
    Vector names = new Vector();
    while (true) {
	if((ok = JBpatterns.ytnameelement.searchForward(yyt,pos)) == null) {
	    break;
	}
	pos = ok.getMatchEnd();
	String nm = yyt.substring(ok.getMatchStart()+1,pos-2);
	names.addElement(nm);
//System.err.println("buildnt: element:"+nm);
    }
    // now we need to extract the nonterminal names
    Integer I = Integer.valueOf((String)parms.get("YYNTBASE"));
    int tokenmin = ((Integer)tokentypes.get(sortedtokentypes.lastElement())).intValue()+1;
    for(int i=I.intValue();i<names.size();i++,tokenmin++) {
	String nm = (String)names.elementAt(i);
	put(nonterms,nm,new Integer(tokenmin));
    }
    // get the nonterm names in index order as well
    sortnonterms();
}

static
void
sorttokens() throws IOException
{
    int len = tokentypes.size();
    sortedtokentypes = new Vector(len);
    Vector vals = new Vector(len);
    Enumeration  e = tokentypes.keys();
    int i;

    while(e.hasMoreElements()) {
	String key = (String)e.nextElement();
	Integer val = (Integer)tokentypes.get(key);
//System.err.println("sorttokens: "+key+"="+val);
	for(i=0;i<sortedtokentypes.size();i++) {
	    if(val.intValue() < ((Integer)vals.elementAt(i)).intValue()) break;
	}
//System.err.println("sorttokens: insert ("+i+")="+key);
	sortedtokentypes.insertElementAt(key,i);
	vals.insertElementAt(val,i);	
    }
//System.err.println("sortedtokentypes="+sortedtokentypes);
}

static
void
sortnonterms()
{
    int len = nonterms.size();
    sortednonterms = new Vector(len);
    Vector vals = new Vector(len);
    Enumeration  e = nonterms.keys();
    int i;

    while(e.hasMoreElements()) {
	String key = (String)e.nextElement();
	Integer val = (Integer)nonterms.get(key);
//System.err.println("sorttokens: "+key+"="+val);
	for(i=0;i<sortednonterms.size();i++) {
	    if(val.intValue() < ((Integer)vals.elementAt(i)).intValue()) break;
	}
//System.err.println("sorttokens: insert ("+i+")="+key);
	sortednonterms.insertElementAt(key,i);
	vals.insertElementAt(val,i);	
    }
//System.err.println("sortednonterms="+sortednonterms);
}

static
Vector
sortflagnames() throws IOException
{
    // First capture all the flagarg names
    Vector names = hashnamevec(flags);
    int l1 = names.size() - 1;
    int min = 0;
    for(int i=0;i<l1;i++) {
	String SI = (String)names.elementAt(i);
	for(int j=i+1;j<=l1;j++) {
	    String SJ = (String)names.elementAt(j);
	    if(SJ.compareTo(SI) < 0) {min = j;}
	}
	names.setElementAt(names.elementAt(min),i);
	names.setElementAt(SI,min);
    }
//    for(int i=0;i<=l1;i++) {
//	String nm = (String)names.elementAt(i);
//	System.err.println("sortedflagnames["+i+"]="+nm);
//    }
    return names;
}

static
void
scanfile() throws IOException
{
    noactions=true;
    while ((line = nextline()) != null) {
	scanline(line);
    }
}

static
String
getflag(String nm, String def) throws IOException
{
    String s = (String)flags.get(nm);
    if(s == null) {put(flags,nm,def); s = def;}
    return s;
}

static
void
genconst1(String nm) throws IOException
{
    buf.append(tablemodifiers+" int "
		+nm
		+" = "
		+parms.get(nm)
		+";\n");
}

static
void
genconstants() throws IOException
{
    genconst1("YYFINAL");
    genconst1("YYFLAG");
    genconst1("YYNTBASE");
    genconst1("YYLAST");
}

static
void
gentable1(String nm, String typ) throws IOException
{
    buf.append(tablemodifiers+" "+typ+" "
		+nm
		+"[] = {\n"
		+parms.get(nm)
		+"};\n\n");
}

static
void
gentables() throws IOException
{
    gentable1("yytranslate","int");
    gentable1("yyprhs","int");
    gentable1("yyrhs","int");
    gentable1("yyrline","int");
    gentable1("yytname","String");
    gentable1("yyr1","int");
    gentable1("yyr2","int");
    gentable1("yydefact","int");
    gentable1("yydefgoto","int");
    gentable1("yypact","int");
    gentable1("yypgoto","int");
    gentable1("yytable","int");
    gentable1("yycheck","int");
}

static void flagsubst(StringBuffer b, String fl) throws IOException
	{flagsubst(b,fl,false);}

static
void
flagsubst(StringBuffer b, String fl, boolean leave) throws IOException
{
    String s;
    Integer I;
    int i;

    if(fl.equals("PREFIX")) {
	b.append((String)parms.get("prefix"));
	return;
    } 

    if(fl.equals("SUFFIX")) {
	b.append((String)parms.get("suffix"));
	return;
    } 

    if(fl.equals("ACTIONS")) {
	// put switch body here
	b.append((String)parms.get("actions"));
	return;
    }

    if(fl.equals("package")) {
	s = getflag(fl,JBpatterns.Nullstring);
	if(s != JBpatterns.Nullstring) {
	    b.append("package "+s);
	}
	return;
    }

    if(fl.equals("constructor")) {
	s = getflag(fl,JBpatterns.Nullstring);
	if(s != JBpatterns.Nullstring) {
	    b.append(s+";");
	}
	return;
    }

    if(fl.equals("TOKENMAX")) {
	b.append(tokentypes.get(sortedtokentypes.lastElement())+";");
	return;
    }

    if(fl.equals("TOKENMIN")) {
	b.append(tokentypes.get(sortedtokentypes.firstElement())+";");
	return;
    }
    
    if(fl.equals("TOKENTYPES")) {
	for(i=0;i<sortedtokentypes.size();i++) {
	    s = (String)sortedtokentypes.elementAt(i);
	    b.append("public static final Integer "+s);
	    b.append(" = YYtokentypes.Tokentype[");
	    b.append(tokentypes.get(s));
	    b.append("];\n");
	}
	return;
    }

    if(fl.equals("TOKENNAMES")) {
	for(i=0;i<sortedtokentypes.size();i++) {
	    s = (String)sortedtokentypes.elementAt(i);
	    b.append("\""+s+"\", ");
	}
	b.append("null");
	return;
    }
    
    if(fl.equals("NONTERMMAX")) {
	// NONTERMMIN + length(parms(nonterms))
	I = (Integer)tokentypes.get(sortedtokentypes.lastElement());
	b.append((I.intValue()+nonterms.size())+";");
	return;
    }

    if(fl.equals("NONTERMMIN")) {
	// == TOKENMAX+1
	I = (Integer)tokentypes.get(sortedtokentypes.lastElement());
	b.append((I.intValue()+1)+";");
	return;
    }
    
    if(fl.equals("NONTERMTYPES")) {
	for(i=0;i<sortednonterms.size();i++) {
	    b.append("public static final int "+flags.get("ntprefix"));
	    String nt = (String)sortednonterms.elementAt(i);
	    b.append(nt+flags.get("ntsuffix"));
	    b.append(" = "+nonterms.get(nt)+";\n");
	}
	return;
    }

    if(fl.equals("NONTERMNAMES")) {
	for(i=0;i<sortednonterms.size();i++) {
	    b.append("\""+sortednonterms.elementAt(i)+"\", ");
	}
	b.append("null");
	return;
    }
    
    if(fl.equals("CONSTANTS")) {
	genconstants();
	return;
    }
    
    if(fl.equals("PROCS")) {
	// dump a static proc
	b.append(parms.get("YYTRANSLATE"));
	return;
    }
    
    if(fl.equals("TABLES")) {
	// put tables here
	gentables();
	return;
    }

    if(leave) {
	s = "@"+fl+"@";
    } else {
	s = fl;
    }
    b.append(getflag(fl,s));
    return;
}

static
void
atsubst(StringBuffer b, String src, boolean leave) throws IOException
{
    int here = 0;
    int nxt = 0;
    Result ok;
    String fl;

    while((ok = JBpatterns.atsign.searchForward(src,here)) != null) {
	nxt = ok.getMatchStart();
	if(nxt > 0) b.append(src.substring(here,nxt));
	fl = ok.getMatch(1);
//System.err.println("atsubst: @"+fl+"@");
	flagsubst(b,fl,leave);
	here = ok.getMatchEnd();
    }
    if(here+1 < src.length()) b.append(src.substring(here));
}

static
void
construct() throws IOException
{
    while ((line = nextline()) != null) {
	// do a quick check to speed up processing
	if(line.indexOf('@') < 0) {
	    target.println(line);
	} else {
//System.err.println("@ before:"+line);
	    buf.setLength(0);
	    atsubst(buf,line,false);
//System.err.println("@ after:"+buf.toString());
	    target.println(buf.toString());
	}
    }
}

static
String
internsubst(String value, boolean leave) throws IOException
{
    // do a quick check to speed up processing
    if(value.indexOf('@') < 0) return value;
    StringBuffer b = new StringBuffer(value.length()+1000);
    atsubst(b,value,leave);
    return b.toString();    
}

static void fixyytname()
{
    // remove the occurrence of NULL at the end of yytname table
    String yt = (String)parms.get("yytname");
    yt = substitute(yt,JBpatterns.ytnamenull,", null");
    put(parms,"yytname",yt);
}

// Make sure that the prefix and suffix are tested for @...@
static
void
fixprefixsuffix() throws IOException
{
    String s;
    s = (String)parms.get("prefix");
    s = internsubst(s,true);
    put(parms,"prefix",s);
    s = (String)parms.get("suffix");
    s = internsubst(s,true);
    put(parms,"suffix",s);
}

static
void
buildtargetfile(String targetnm, String templatenm) throws IOException
{
//System.err.println("buildtargetfile: target="+targetnm+" ; template="+templatenm);
    ftarget = new FileOutputStream(targetnm);
    fsrc = new FileInputStream(templatenm);
    target = new PrintStream(ftarget);
    src = new DataInputStream(fsrc);
    construct();
    ftarget.close();
    fsrc.close();
}

static
void
doit(String bisonfilenm, Vector pairs) throws IOException
{
    fsrc = new FileInputStream(bisonfilenm);
    src = new DataInputStream(fsrc);
    scanfile();
    fsrc.close();
    sorttokens();
    fixyytname();
    buildnttable();
    fixprefixsuffix();
    for(int i=0;i<pairs.size();i+=2) {
	buildtargetfile((String)pairs.elementAt(i+1),(String)pairs.elementAt(i));
    }
}

static
void
error(String msg) throws IOException
{
    System.err.println("jf: "+msg);
    System.exit(1);
}

static void usage(String msg) throws IOException
{
    System.err.println("jb: "+msg);
    System.err.println("usage: jb [options] [flags] <bisonfile> {<template> <target>}+");
    System.exit(1);
}

public
static
void
main(String[] argv) throws IOException
{
    put(flags,"javapath",JBpatterns.Nullstring);
    put(flags,"YYparse","YYparse");
    put(flags,"YYtokentypes","YYtokentypes");
    put(flags,"YYlex","YYlex");
    put(flags,"yylex","yylex");
    put(flags,"ntprefix","nt_");
    put(flags,"ntsuffix",JBpatterns.Nullstring);
    Vector av = new Vector();    
    Vector flagargs = new Vector();
    int i = 0;
    int argc = argv.length;

    for(i=0;i < argc;i++) {
	String arg = argv[i];
	String cmd;
	if(arg.startsWith("--")) {
	    if(arg.equals("--flags")) {
		put(options,"dumpflags",new Boolean(true));
	    } else {
		usage("unknown option "+arg);
	    }
	} else if(arg.startsWith("-")) {
	    cmd = arg.substring(1);
	    // take this as a flag to set in templates
	    i++;
	    if(i >= argc) {usage("no argument to flag");}
	    arg = argv[i];
	    put(flags,cmd,arg);
	    lunion(flagargs,cmd);
	} else {
	    av.addElement(arg);
	}
    }
    // first file is the input parse file
    if(av.size() == 0) {usage("too few arguments");}
    String bisonfilenm = (String)av.elementAt(0);
    // use the rest of the args as file pairs
    av.removeElementAt(0);
    if(av.size() % 2 != 0) {usage("too few arguments");}
    doit(bisonfilenm,av);
    if(options.get("dumpflags") != null) {
	System.out.println("specified flag values:");
	Vector flagnames = sortflagnames();
	for(i=0;i<flagnames.size();i++) {
	    String fl = (String)flagnames.elementAt(i);
	    if(lmember(flagargs,fl)) {
		String def = (String)flags.get(fl);
		System.out.println("\t"+fl+"=\""+def+"\"");
	    }
	}
	System.out.println("default flags values:");
	for(i=0;i<flagnames.size();i++) {
	    String fl = (String)flagnames.elementAt(i);
	    if(!lmember(flagargs,fl)) {
		String def = (String)flags.get(fl);
		System.out.println("\t"+fl+"=\""+def+"\"");
	    }
	}
    }
}

};
