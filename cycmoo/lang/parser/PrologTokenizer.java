package cycmoo.lang.parser;  //
import cycmoo.lang.*;
import cycmoo.lang.fluent.*;
import cycmoo.lang.builtin.*;  //
import cycmoo.lang.object.*;  //
import java.io.Reader;
import java.io.IOException;
import java.io.StreamTokenizer;
import java.util.Vector;

/**
  Lexicographic analyser reading from a stream
*/
class PrologTokenizer extends StreamTokenizer {
    public Reader input;
    public ITermMap lexicon;
    public ITermMap vars;

    public IProceedureMap getDictionary() {
        return(IProceedureMap)KernelAgent.currentInstance();
    }

    public PrologTokenizer(ITermMap tm, Reader I) throws IOException {
        super(I);
        this.lexicon = tm;
        this.input=I;
        parseNumbers();
        eolIsSignificant(true);
        ordinaryChar('.');
        ordinaryChar('-'); // creates problems with -1 etc.
        ordinaryChar('/');
        quoteChar('\'');
        quoteChar('\"');
        wordChar('$');
        wordChar('_');
        slashStarComments(true);
        commentChar('%');
        vars=KernelAgent.newEmptyTermMap();
    }

    /**
       Path+File name based constructor
       Used in prolog2java
    */

    public PrologTokenizer(ITermMap tm,String path,String s) throws IOException {
        this(tm,IO.url_or_file(path+s)); // stream
    }

    /**
       String based constructor.
       Used in queries ended by \n + prolog2java.
    */

    public PrologTokenizer(ITermMap tm, String s) throws Exception {
        this(tm,IO.string_to_stream(s));
    }

    public PrologTokenizer(ITermMap tm) throws IOException {
        this(tm,IO.input);
    }

    private final static String anonymous="_".intern();

    private final static String char2string(int c) {
        return ""+(char)c;
    }

    private boolean inClause=false;

    public boolean atEOF() {
        boolean yes=(TT_EOF==ttype);
        if ( yes ) try {
                input.close();
            } catch ( IOException e ) {
                IO.traceln("unable to close atEOF");
            }
        return yes;
    }

    public boolean atEOC() {
        return !inClause;
    }

    protected final ITerm make_const(String s) {
        if (s.indexOf('.')>3) return find_make_object(s);
        return new constToken(this.getDictionary(), s);
    }
    protected final ITerm find_make_object(String s) {
        ITerm obj = (ITerm)KernelAgent.objectGet(s);
        if (obj!=null) return obj;
        obj = (ITerm)KernelAgent.objectGet("'"+s+"'");
        if (obj!=null) return obj;
        return new constToken(this.getDictionary(), s);
    }

    private final ITerm make_fun(String s) {
        return new funToken(s);
    }

    private final ITerm make_int(double n) {
        return new intToken((int)n);
    }

    private final ITerm make_real(double n) {
        return new realToken(n);
    }

    private final ITerm make_number(double nval) {
        ITerm T;
        if ( Math.floor(nval)==nval )
            T=make_int(nval);
        else
            T=make_real(nval); 
        return T;
    }

    private final ITerm make_var(String s) {
        s=s.intern();
        Var X;
        long occ;
        if ( s==anonymous ) {
            occ=0;
            X=new PrologVar();
            s=X.toString();
        } else {
            X=(Var)vars.get(s);
            if ( X==null ) {
                occ=1;
                X=new PrologVar();
            } else {
                occ=((IntTerm)vars.get(X)).longValue();
                occ++;
            }  
        }
        IntTerm I=new IntTerm(occ);
        vars.put(X,I);
        vars.put(s,X);
        return new varToken(X,lexicon.defAtom(s),I);
    }

    private final void whitespaceChar(char c) {
        whitespaceChars(c,c);
    }

    private final void wordChar(char c) {
        wordChars(c,c);
    }

    private ITerm getWord(boolean quoted) throws IOException {
        ITerm T;
        if ( quoted && 0==sval.length() ) T=make_const("");
        /* DO NOT DO THIS: quoting is meant to prevent it!!!
        else if("()[]|".indexOf(sval.charAt(0))>=0) {
          switch(sval.charAt(0)) {
            case '(':
               T=new lparToken();
              break;
            case ')':
               T=new rparToken();
              break;
            case '[':
               T=new lbraToken();
              break;
            case ']':
               T=new rbraToken();
              break;
            case '|':
               T=new barToken();
              break;
          }
        }
        */
        else {
            char c=sval.charAt(0);
            if ( !quoted && (Character.isUpperCase(c) || '_'==c) )
                T=make_var(sval);
            else { // nonvar
                String s=sval; int nt=nextToken();
                pushBack();
                T=('('==nt)? make_fun(s):make_const(s); 
            }
        }
        return T;
    }

    protected ITerm next() throws IOException {
        int n=nextToken();
        inClause=true;
        ITerm T; 
        switch ( n ) {
        case TT_WORD:
            T=getWord(false);
            break;

        case '\'':
            T=getWord(true);
            break;

        case TT_NUMBER: 
            T=make_number(nval);
            break;

        case TT_EOF: 
            T=new eofToken(lexicon);
            inClause=false; 
            break;

        case TT_EOL: 
            T=next();
            break;

        case '-':
            if ( TT_NUMBER==nextToken() ) {
                T=make_number(-nval);
            } else {
                pushBack();
                T=make_const(char2string(n));
            }

            break;

        case ':': if ( '-'==nextToken() ) {
                T=new iffToken(lexicon,":-");
            } else {
                pushBack();
                T=make_const(char2string(n));
            }
            break;

        case '.': int c=nextToken();
            if ( TT_EOL==c || TT_EOF==c ) {
                inClause=false;
                //vars.clear();  ///!!!: this looses Var names
                T=new eocToken(lexicon);
            } else {
                pushBack();
                T=make_const(char2string(n));  // !!!: sval is gone
            }
            break;

        case '\"': T=new stringToken((constToken)getWord(true));
            break;

        case '(': T=new lparToken(); break;
        case ')': T=new rparToken(); break;
        case '[': T=new lbraToken(); break;
        case ']': T=new rbraToken(); break;
        case '|': T=new barToken(); break;

        case ',': T=new commaToken(); break;
        default: 
            T=make_const(char2string(n));
        }
        //IO.mes("TOKEN:"+T);
        return T;
    }

    class CompoundToken extends Compound {
        CompoundToken(String s,int arety) {
            super(s,arety);
        }
        CompoundToken(String s,ITerm T) {
            super(s,1);
            setDirectArg(0,T);
        }

    }

    class varToken extends CompoundToken {
        public varToken(Var X,Atom C,IntTerm I) {
            super("varToken",3); 
            setDirectArg(0,X);
            setDirectArg(1,C);
            setDirectArg(2,I);
        }
    }

    class intToken extends CompoundToken {
        public intToken(int i) {
            super("intToken",new IntTerm(i));
        }
    }

    class realToken extends CompoundToken {
        public realToken(double i) {
            super("realToken",new RealNumTerm(i));
        }
    }

    class constToken extends CompoundToken {
        public constToken(IProceedureMap map, Atom c) {
            super("constToken",c);
            setDirectArg(0,c.toProceedure(map));
        }

        public constToken(IProceedureMap map, String s) {
            this(map, map.defAtom(s));
        }
    }

    class stringToken extends CompoundToken {
        public stringToken(constToken c) {
            super("stringToken",(Atom)(c.getDirectArg(0)));
        }
    }

    class funToken extends CompoundToken {
        public funToken(String s) {
            super("funToken",Compound.newCompoundStub(s));
        }
    }

    class eocToken extends CompoundToken {
        public eocToken(ITermMap lex) {
            super("eocToken",lex.useAtom("end_of_clause"));
        }
    }

    class eofToken extends CompoundToken {
        public eofToken(ITermMap lex) {
            super("eofToken",Atom.anEof);
        }
    }

    class iffToken extends CompoundToken {
        public iffToken(ITermMap lex, String s) {
            super("iffToken",lex.useAtom(s));
        }
    }

    class Token extends Atom {
        Token(String s) {
            super();
            setSym(s);
        }
    }

    class lparToken extends Token {
        public lparToken() {
            super("(");
        }
    }

    class rparToken extends Token {
        public rparToken() {
            super(")");
        }
    }

    class lbraToken extends Token {
        public lbraToken() {
            super("[");
        }
    }

    class rbraToken extends Token {
        public rbraToken() {
            super("]");
        }
    }

    class barToken extends Token {
        public barToken() {
            super("|");
        }
    }

    class commaToken extends Token {
        public commaToken() {
            super(",");
        }
    }
}




