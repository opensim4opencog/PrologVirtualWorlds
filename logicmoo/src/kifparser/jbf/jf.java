
package jbf;

import java.io.*;
import java.util.*;
import regexp.*;

class JFpatterns {

static final String Nullstring = "";

static Regexp numdef = Regexp.compile("^#define\\s+(\\w+)\\s+(\\S.*)");

static Regexp tabledef = Regexp.compile("^static\\s+yyconst[^[]+\\[");
static Regexp tabledef1 = Regexp.compile("^static\\s+yyconst\\s+(\\w+)\\s+int\\s+(\\w+)\\[(\\d+)\\]");
static Regexp tabledef2 = Regexp.compile("^static\\s+yyconst\\s+int\\s+(\\w+)\\[(\\d+)\\]");
static Regexp tabledef3 = Regexp.compile("^static\\s+yyconst\\s+short\\s+(\\w+)\\[\\]\\[(\\d+)\\]");
static Regexp tabledef4 = Regexp.compile("^static\\s+yyconst\\s+long\\s+(\\w+)\\[\\]\\[(\\d+)\\]");

static Regexp prefix = Regexp.compile("^#line\\s+(\\d+)\\s+\"([^\"]*)\"");
static Regexp suffix = Regexp.compile("^#if\\s+YY_MAIN");
static Regexp actions = Regexp.compile("\\s*switch\\s*\\(\\s*yy_act\\s*\\)");
static Regexp special = Regexp.compile("\\s+while\\s*\\(\\s*yy_base\\[yy_current_state\\]\\s*!=\\s*(\\d+)\\s*\\)\\s*;");


static Regexp endtab = Regexp.compile("\\s*\\}\\s*;");
static Regexp rulesetend = Regexp.compile("\\s+YY_USER_ACTION");

static Regexp escape = Regexp.compile("\\s*\\\\s*$");
static Regexp arrow =	Regexp.compile("->");
static Regexp atsign = Regexp.compile("@([^@]+)@");

static Regexp linedir = Regexp.compile("#line\\s(\\d+)\\s+\"([^\"]*)\"");

static Regexp action0 = Regexp.compile("^case\\s(\\d+)\\s*:");
static Regexp action1 = Regexp.compile("\\s+case\\s+YY_END_OF_BUFFER:");
static Regexp action2 = Regexp.compile("case YY_STATE_EOF\\(([^)]+)\\):");
static Regexp action3 = Regexp.compile("^\\s+YY_BREAK");
static Regexp action4 = Regexp.compile("^YY_RULE_SETUP");
static Regexp action5 = Regexp.compile("\\s*ECHO;");
static Regexp action6 = Regexp.compile("\\s+yyterminate\\(\\s*\\);");

// Track the define names that are or any interest
static Hashtable definenames = new Hashtable();

static
{
    // fill in definenames
    definenames.put("YY_FLEX_MAJOR_VERSION",Nullstring);
    definenames.put("YY_FLEX_MINOR_VERSION",Nullstring);
    definenames.put("YY_NULL",Nullstring);
    definenames.put("YY_MORE_ADJ",Nullstring);
    definenames.put("YY_END_OF_BUFFER_CHAR",Nullstring);
    definenames.put("YY_BUF_SIZE",Nullstring);
    definenames.put("YY_READ_BUF_SIZE",Nullstring);
    definenames.put("EOB_ACT_CONTINUE_SCAN",Nullstring);
    definenames.put("EOB_ACT_END_OF_FILE",Nullstring);
    definenames.put("EOB_ACT_LAST_MATCH",Nullstring);
    definenames.put("YY_START_STACK_INCR",Nullstring);
    definenames.put("YY_BUFFER_NEW",Nullstring);
    definenames.put("YY_BUFFER_NORMAL",Nullstring);
    definenames.put("YY_BUFFER_EOF_PENDING",Nullstring);
    definenames.put("YY_NUM_RULES",Nullstring);
    definenames.put("YY_END_OF_BUFFER",Nullstring);
}


// not currently used
// static Regexp rulesetup = Regexp.compile("^#define\\s+YY_RULE_SETUP\\s");

}; //class Pattern

class Namedhash extends Hashtable {
    public Namedhash(String s) {super(); nm = s;}
    public String name() {return nm;}
    String nm;
};


public
class jf {

// define the acceptable flex versions
static final int flexvermin = 25;
static final int flexvermax = 25;
static final String tablemodifiers = "public static final";

//////////////////////////////////////////////////

static Namedhash options = new Namedhash("options");
static Namedhash flags = new Namedhash("flags");
static Namedhash parms = new Namedhash("parms");
static Namedhash tablelengths = new Namedhash("tablelengths");
static Namedhash statetypes = new Namedhash("statetypes");
static Vector sortedstatenames = null;

static String lexinname = null;

// indicate if we have the actions section
static boolean noactions = true;

static boolean haveprefix = false;

// 0 => normal; 1 =>Cfe ; 2=>Cfa
static int cfxtemplate = 0;

// 0 => normal; 1 =>Cfe ; 2=>Cfa
static int cfxinput = 0;

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

//////////////////////////////////////////////////

static String nextline() throws IOException
{
    line = null;
    line = src.readLine();
    return line;
}

//////////////////////////////////////////////////

static void Numdef(String line, Regexp re)
{
    if(!match(line,re)) return;

    String varnm = match.getMatch(1);
    String val = match.getMatch(2);

//System.err.println("Numdef: "+varnm+"=/"+val+"/");
    if(JFpatterns.definenames.get(varnm) != null) {
	put(parms,varnm,val);
    }
}

static void Tabledef(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    String typ1 = JFpatterns.Nullstring;
    String typ2 = JFpatterns.Nullstring;
    String nm;
    boolean is2d = false;
    String dim;

    if((ok = JFpatterns.tabledef1.searchForward(line,0)) != null) {
	typ1 = ok.getMatch(1);
	typ2 = "int";
	nm = ok.getMatch(2);
	dim = ok.getMatch(3);
    } else if((ok = JFpatterns.tabledef2.searchForward(line,0)) != null) {
	typ1 = "int";
	nm = ok.getMatch(1);
	dim = ok.getMatch(2);
    } else if((ok = JFpatterns.tabledef3.searchForward(line,0)) != null) {
	typ1 = "short";
	nm = ok.getMatch(1);
	dim = ok.getMatch(2);
	is2d = true;
    } else if((ok = JFpatterns.tabledef4.searchForward(line,0)) != null) {
	typ1 = "long";
	nm = ok.getMatch(1);
	dim = ok.getMatch(2);
	is2d = true;
    } else return; // false alarm


//System.err.println("Tabledef: typ="+typ1+" "+typ2+" ; nm="+nm+" ; is2d="+is2d+" ; dim="+dim);

    if(typ1.equals("long") && nm.equals("yy_accept")) cfxdetected(2);
    if(typ1.equals("short") && nm.equals("yy_nxt") && is2d) cfxdetected(1);
    if(typ1.equals("long") && nm.equals("yy_nxt") && is2d) cfxdetected(2);

    StringBuffer tab = buf;
    
    put(tablelengths,nm,dim);

    tab.setLength(0);
    while((line = nextline()) != null) {
	ok = JFpatterns.endtab.searchForward(line,0);
	if(ok != null) {
	    put(parms,nm,tab.toString());
	    return;
	}
	tab.append(line+"\n");
    }
    error("eof detected while scanning table");
}

// not used
static void Rulesetup(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    StringBuffer body = buf;

    body.setLength(0);
    while((line = nextline()) != null) {
	// figure out when to quit
	ok = JFpatterns.rulesetend.searchForward(line,0);
	if(ok != null) {
	    put(parms,"YY_RULE_SETUP",body.toString());
	    return;
	}
//System.err.println("before: "+line);
	line = substitute(line,JFpatterns.escape,"\n");
	line = substitute(line,JFpatterns.arrow,".");
//System.err.println("after: "+line);
	body.append(line);
    }
    error("yyrulesetup: eof detected while scanning YY_RULE_SETUP macro");
}

static void Prefix(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    if(haveprefix) return;
    // We assume (for these flex versions) that the prefix
    // is after the definition of YY_FLEX_MAJOR_VERSION
//System.err.println("parm names: "+hashnames(parms));
    if(parms.get("YY_FLEX_MAJOR_VERSION") == null) return;
    ok = JFpatterns.linedir.searchForward(line,0);
    if(ok == null) {error("yyprefix: could not parse #line directive");}
    String lineno = ok.getMatch(1);
    String fname = ok.getMatch(2);
//System.err.println("Prefix: (a)lineno="+lineno+"; fname="+fname);
    if(Integer.parseInt(lineno) != 1) {
	error("could not find start of user specified prefix");
    }
    haveprefix = true;
    lexinname = fname;
    StringBuffer body = buf;
    body.setLength(0);
    while((line = nextline()) != null) {
	// figure out when to quit
	ok =  JFpatterns.linedir.searchForward(line,0);
	if(ok != null) {
	    lineno = ok.getMatch(1);
	    fname = ok.getMatch(2);
//System.err.println("Prefix: (b)lineno="+lineno+"; fname="+fname);
	    if(!fname.equals(lexinname)) {
		put(parms,"prefix",body.toString());
		return;
	    }
	    // suppress #line directives
	    continue;
	}
	ok = JFpatterns.numdef.searchForward(line,0);
	if(ok != null) {
	    String varnm = ok.getMatch(1);
	    String val = ok.getMatch(2);
	    // should be one of the state types
	    put(statetypes,varnm,Integer.valueOf(val));
	} else {
	    body.append(line+"\n");
	}
    }
    error("eof detected while scanning user defined prefix code: $body");
}

static void Suffix(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    // skip to #line
    while (true) {
	if((line = nextline()) == null) {
	    error("Suffix: unexpected eof detected");
	}
	ok = JFpatterns.linedir.searchForward(line,0);
	if(ok != null) {
	    String lineno = ok.getMatch(1);
	    String fname = ok.getMatch(2);
//System.err.println("(c)lineno="+lineno+"; fname="+fname);
	    // see if this is the expected file name
	    if(fname.equals(lexinname)) break;
	}
    }
    // absorb everything to the end of the file
    // but suppress line directives
    StringBuffer body = buf;
    body.setLength(0);
    while ((line = nextline()) != null) {
	if(JFpatterns.linedir.searchForward(line,0) != null) continue;
	body.append(line+"\n");
    }
    put(parms,"suffix",body.toString());
}

static void Actions(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    StringBuffer body = buf;
    body.setLength(0);
    // scan for the first real action
    while (true) {
	if((line = nextline()) == null) {
	    error("eof detected while scanning action switch body");
	}
	ok = JFpatterns.action0.searchForward(line,0);
	if(ok != null) {
	    String caseno = ok.getMatch(1);
	    if(Integer.parseInt(caseno) != 1) {
		error("malformed action switch body: "+line);
	    }
	    break;
	}
    }
    // include the case line
    body.append(line+"\n");
    // collect the real thing
    while ((line = nextline()) != null) {
	// test to quit
	if(JFpatterns.action1.searchForward(line,0) != null) {
	    put(parms,"actions",body.toString());
	    noactions = false;
	    return;
	}
	// suppress line directives
	if(JFpatterns.linedir.searchForward(line,0) != null) continue;
	// do some special rewrites
	if((ok = JFpatterns.action2.searchForward(line,0)) != null) {
	    String statename = ok.getMatch(1);
	    line = "case YY_END_OF_BUFFER + 1 + "+statename+" :";
	} else if((ok = JFpatterns.action3.searchForward(line,0)) != null) {
	    line = "break;";
	} else if((ok = JFpatterns.action4.searchForward(line,0)) != null) {
	    continue;
	} else if((ok = JFpatterns.action5.searchForward(line,0)) != null) {
	    continue;
	} else if((ok = JFpatterns.action6.searchForward(line,0)) != null) {
//System.err.println("Action: before: "+line);
	    line = substitute(line,JFpatterns.action6,"yytokentype = YYtokentypes.YYEOF; break;");
//System.err.println("Action: after: "+line);
	}
	body.append(line+"\n");
    }
    error("eof detected while scanning action switch body: $body");
}

static void Special(String line, Regexp re) throws IOException
{
    if(!match(line,re)) return;

    String val = match.getMatch(1);
    put(parms,"YY_SPECIAL",val);
}

//////////////////////////////////////////////////

static
void
scanline(String line) throws IOException
{
    // do some pre-checks to speed up processing
    if(line.length() > 0 && line.charAt(0) == '#') {
	if(line.startsWith("#define")) {
	    Numdef(line,JFpatterns.numdef);
	    return;
	}
	if(line.startsWith("#line")) {
	    Prefix(line,JFpatterns.prefix);
	    return;
	}
	if(line.startsWith("#if")) {
	    Suffix(line,JFpatterns.suffix);
	    return;
	}
    }
    if(line.startsWith("static")) {
	Tabledef(line,JFpatterns.tabledef);
	return;
    }
    if(line.indexOf("switch") >= 0) {
	Actions(line,JFpatterns.actions);
	return;
    }
    if(line.indexOf("while") >= 0) {
	Special(line,JFpatterns.special);
	return;
    }
}

static
void
cfxdetected(int which) throws IOException
{
//System.err.println("cfxdetected: "+which);
    Integer cfx = (Integer)options.get("Cfx");
    if(which == 2) { // -Cfa
	if(cfx.intValue() == 0) {
	    System.err.println("Warning: \"flex -Cfa\" input detected; --Cfa flag assumed");
	    put(options,"Cfx",new Integer(1));
	}
	cfxinput = 2;
    } else { // which == 1 => -Cfe
	if(cfx.intValue() == 0) {
	    System.err.println("Warning: \"flex -Cfe\" input detected; --Cfe flag assumed");
	    put(options,"Cfx",new Integer(1));
	}
	cfxinput = 1;
    }
//System.err.println("cfxdetected: cfxinput="+cfxinput+" ; options(Cfx)="+options.get("Cfx"));
}

static
void
sortstates() throws IOException
{
    int len = statetypes.size();
    sortedstatenames = new Vector(len);
    Vector vals = new Vector(len);
    Enumeration  e = statetypes.keys();
    int i;

    while(e.hasMoreElements()) {
	String key = (String)e.nextElement();
	Integer val = (Integer)statetypes.get(key);
	for(i=0;i<sortedstatenames.size();i++) {
	    if(val.intValue() < ((Integer)vals.elementAt(i)).intValue()) break;
	}
	sortedstatenames.insertElementAt(key,i);
	vals.insertElementAt(val,i);	
    }

//System.err.println("sortedstatenames="+sortedstatenames);
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
genstates() throws IOException
{
    for(int i=0;i<sortedstatenames.size();i++) {
	String key = (String)sortedstatenames.elementAt(i);
	Integer val = (Integer)statetypes.get(key);
//System.err.println("genstates: state "+key+" = "+val);
	buf.append(tablemodifiers+" int "+key+" = "+val+";\n");
    }
//System.err.println("genstates: "+buf.toString());
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
    genconst1("YY_FLEX_MAJOR_VERSION");
    genconst1("YY_FLEX_MINOR_VERSION");
    genconst1("YY_NULL");
    genconst1("YY_MORE_ADJ");
    genconst1("YY_END_OF_BUFFER_CHAR");
    genconst1("YY_BUF_SIZE");
    genconst1("YY_READ_BUF_SIZE");
    genconst1("EOB_ACT_CONTINUE_SCAN");
    genconst1("EOB_ACT_END_OF_FILE");
    genconst1("EOB_ACT_LAST_MATCH");
    genconst1("YY_START_STACK_INCR");
    genconst1("YY_BUFFER_NEW");
    genconst1("YY_BUFFER_NORMAL");
    genconst1("YY_BUFFER_EOF_PENDING");
    genconst1("YY_NUM_RULES");
    genconst1("YY_END_OF_BUFFER");
    if(cfxinput == 0) {genconst1("YY_SPECIAL");}
}

static
void
gentable1(String nm, String typ) throws IOException
{
    buf.append(tablemodifiers+" "+typ+" "
		+nm
		+"[] = \n"
		+parms.get(nm)
		+"};\n\n");
}

static
void
gentablecfx(String nm, String typ) throws IOException
{
    buf.append(tablemodifiers+" "+typ+" "
		+nm
		+"[][] = \n"
		+parms.get(nm)
		+"};\n\n");
}

static
void
gentables() throws IOException
{
    if(cfxinput == 2) {
	gentablecfx("yy_nxt","int");
	gentable1("yy_accept","int");
    } else if(cfxinput == 1) {
	gentablecfx("yy_nxt","short");
	gentable1("yy_accept","short");
	gentable1("yy_ec","int");
    } else {
	gentable1("yy_accept","short");
	gentable1("yy_ec","int");
	gentable1("yy_nxt","short");
	gentable1("yy_meta","int");
	gentable1("yy_base","short");
	gentable1("yy_def","short");
	gentable1("yy_chk","short");
    }
}

static void flagsubst(StringBuffer b, String fl) throws IOException
	{flagsubst(b,fl,false);}

static
void
flagsubst(StringBuffer b, String fl, boolean leave) throws IOException
{
    String s;

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
	s = getflag(fl,JFpatterns.Nullstring);
	if(s != JFpatterns.Nullstring) {
	    b.append("package "+s);
	}
	return;
    }

    if(fl.equals("constructor")) {
	s = getflag(fl,JFpatterns.Nullstring);
	if(s != JFpatterns.Nullstring) {
	    b.append(s+";");
	}
	return;
    }

    if(fl.equals("CONSTANTS")) {
	genconstants();
	return;
    }
    
    if(fl.equals("PROCS")) {
	return;
    }
    
    if(fl.equals("STATES")) {
	genstates();
	return;
    }
    
    if(fl.equals("TABLES")) {
	// put tables here
	gentables();
	return;
    }

    if(fl.equals("FLEXFLAGS=Cfa")) {
	cfxtemplate = 2;
//System.err.println("flexflags=-Cfa ; cfxtemplate=2");
	b.append(fl);
	return;
    }

    if(fl.equals("FLEXFLAGS=Cfe")) {
	cfxtemplate = 1;
//System.err.println("flexflags=-Cfe ; cfxtemplate=1");
	b.append(fl);
	return;
    }

    if(fl.equals("FLEXFLAGS=Cem") || fl.equals("FLEXFLAGS=Cem")) {
	cfxtemplate = 0;
//System.err.println("flexflags=-Cem ; cfxtemplate=0");
	b.append(fl);
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

    while((ok = JFpatterns.atsign.searchForward(src,here)) != null) {
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
cfxverify1() throws IOException
{
    // verify flag matches
    if(cfxinput != ((Integer)options.get("Cfx")).intValue()) {
	switch (cfxinput) {
	case 0:
	    System.err.println("Warning: jf --Cf{a/e} specified, but input not produced using \"flex -Cf{a/e}\"");
	    break;

	case 1:
	    System.err.println("Warning: jf --Cfa specified, but input produced using \"flex -Cfe\"");
	    break;

	case 2:
	    System.err.println("Warning: jf --Cfe specified, but input produced using \"flex -Cfa\"");
	    break;
	}
    }
}

static
void
cfxverify2() throws IOException
{
    // verify flag matches
    if(cfxtemplate != ((Integer)options.get("Cfx")).intValue()) {
	switch (cfxtemplate) {
	case 0:
	    System.err.println("Warning: flex mismatch: template for \"flex -Cf{a/e}\", but input not produced using \"flex -Cf{a/e}\"");
	    break;

	case 1:
	    System.err.println("Warning: flex mismatch: template for \"flex -Cfe\", but input produced using \"flex -Cfa\"");
	    break;

	case 2:
	    System.err.println("Warning: flex mismatch: template for \"flex -Cfa\", but input produced using \"flex -Cfe\"");
	    break;
	}
    }
}

static
void
doit(String flexfilenm, Vector pairs) throws IOException
{
    fsrc = new FileInputStream(flexfilenm);
    src = new DataInputStream(fsrc);
    scanfile();
    fsrc.close();
    cfxverify1();
    sortstates();
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
    System.err.println("jf: "+msg);
    System.err.println("usage: jf [options] [flags] <flexfile> {<template> <target>}+");
    System.exit(1);
}

public
static
void
main(String[] argv) throws IOException
{
    put(options,"Cfx",new Integer(0));
    put(flags,"javapath",JFpatterns.Nullstring);
    put(flags,"flexversion",JFpatterns.Nullstring);
    put(flags,"YYlex","YYlex");
    put(flags,"yylex","yylex");
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
	    } else if(arg.equals("--Cfa")) {
		put(options,"Cfx",new Integer(2));	
	    } else if(arg.equals("--Cfe")) {
		put(options,"Cfx",new Integer(1));
	    } else if(arg.equals("--Cem")) {
		put(options,"Cfx",new Integer(0));
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
    String flexfilenm = (String)av.elementAt(0);
    // use the rest of the args as file pairs
    av.removeElementAt(0);
    if(av.size() % 2 != 0) {usage("too few arguments");}
    doit(flexfilenm,av);
    cfxverify2();
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
