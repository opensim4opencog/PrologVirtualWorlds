package jinni.kprolog.parser;  //tarau.jinni;
import jinni.kprolog.*;
import java.io.Reader;
import java.io.IOException;
import java.io.StreamTokenizer;
import java.util.Vector;

/**
  Simplified Prolog parser:
  Synatax supported:
  a0:-a1,...,an.
  - no operators ( except toplevel :- and ,)
  - use quotes to create special symbol names, i.e.
  compute('+',1,2, Result) and  write(':-'(a,','(b,c)))
*/

public class PrologParser extends PrologTokenizer implements ITermReader {

    public PrologParser(ITermMap tm, Reader I) throws IOException {
        super(tm,I);
    }

    /*
      used in prolog2java
    */
    public PrologParser(ITermMap tm,String p,String s) throws IOException {
        super(tm,p,s);
    }

    public PrologParser(ITermMap tm,String s) throws Exception {
        super(tm,s);
    }

    public Reader getReader() {
        return input;
    }
    public void setReader(Reader r) {
        input = r;
    }

    /**
      Main PrologParser interface: reads a clause together
      with variable name information
    */
    public HornClause readClause() {
        HornClause t=null; boolean verbose=false;
        try {
            t=readClauseOrEOF();
            //IO.mes("GOT HornClause:"+t);
        }
        /**
          catch built exception clauses which are defined
          in lib.pro - allowing to recover or be quiet about
          such errors.
        */
        catch ( ParserException e ) {
            t=errorClause(e,"syntax_error",lineno(),verbose);
            try {
                while ( !atEOC() && !atEOF() )
                    next();
            } catch ( IOException toIgnore ) {
            }
        } catch ( IOException e ) {
            t=errorClause(e,"io_exception",lineno(),verbose);
        } catch ( Exception e ) {
            t=errorClause(e,"unexpected_syntax_exception",lineno(),true);
        }
        return t;
    }

    static final HornClause errorClause(Exception e,String type,
                                        int line,boolean verbose) {

        String mes=e.getMessage();
        if ( null==mes ) mes="unknown_error";
        Compound F = Compound.newCompound("error",3);
        Compound lineNum = Compound.newCompound("line",1);
        F.setDirectArg(0,TermMap.useAtomStatic(type));
        F.setDirectArg(1,TermMap.useStringAtom(mes));
        lineNum.setDirectArg(0,new IntTerm(line));
        F.setDirectArg(2,lineNum);
        HornClause C=new HornClause(F,(ITerm)Atom.aTrue);
        if ( verbose ) {
            IO.errmes(type+" error at line:"+line);
            IO.errmes(C.pprint(),e);
        }
        return C;  
    }

    public final boolean isError(HornClause C) {
        ITerm H=C.getHead();
        if ( H instanceof Compound &&
             "error".equals(((Compound)H).name()) && 
             H.getArity()==3 &&  
             ! (((Compound)H).getDrefArg(0) instanceof Var) 
           )
            return true;
        return false;
    }

    static  public final void showError(HornClause C) {
        IO.errmes("*** "+C);
    }

    static protected final HornClause toClause(ITerm T,ITermMap sdict) {
        HornClause C=T.toClause(); // adds ...:-true if missing
        C.vars=sdict;
        return C;
    }

    private HornClause readClauseOrEOF() throws IOException {

        vars=KernelAgent.newEmptyTermMap();

        ITerm n=next();

        //IO.mes("readClauseOrEOF 0:"+n);

        if ( n instanceof eofToken ) return null; // $$toClause(n.token(),vars);

        if ( n instanceof iffToken ) {
            n=next();
            ITerm t=getTerm(n);
            ITerm bs=getConjCont(t);
            HornClause C=new HornClause(lexicon.useAtom("init"),bs); 
            C.vars=vars;
            return C;
        }

        ITerm h=getTerm(n);

        //IO.mes("readClauseOrEOF 1:"+h);

        n=next();

        //IO.mes("readClauseOrEOF 2:"+n);

        if ( n instanceof eocToken || n instanceof eofToken )
            return toClause(h,vars);

        //IO.mes("readClauseOrEOF 3:"+b);

        HornClause C=null;
        if ( n instanceof iffToken ) {
            ITerm t=getTerm();
            ITerm bs=getConjCont(t);
            C=new HornClause(h,bs); 
            C.vars=vars;
        } else if ( n instanceof commaToken ) {
            ITerm b=getTerm();
            ITerm bs=getConjCont(b);
            C=toClause(new Conj(h,bs),vars);
        } else {
            throw new ParserException("':-' or '.' or ','", "bad body element",n);
        }
        return C;
    } 

    private final ITerm getConjCont(ITerm curr) 
    throws IOException {

        ITerm n=next();ITerm t=null;
        if ( n instanceof eocToken ) t=curr;
        else if ( n instanceof commaToken ) {
            ITerm other=getTerm();
            t=new Conj(curr,getConjCont(other));
        }
        if ( null==t ) {
            throw new ParserException("'.'", "bad body element",n);
        }
        return t;
    } 

    protected final ITerm getTerm(ITerm n) throws IOException {
        ITerm t=n.token();
        if ( n instanceof varToken || 
             n instanceof intToken ||
             n instanceof realToken ||
             n instanceof constToken ) {
            // is just OK as it is
        } else if ( n instanceof stringToken ) {
            t=((Nonvar)((stringToken)n).getDirectArg(0)).toChars();
            //IO.mes("getTerm:stringToken-->"+t);

        } else if ( n instanceof lbraToken ) {
            t=getList();
        } else if ( n instanceof funToken ) {
            Compound f=(Compound)t;
            f.setAllArgs(getArgs());
            t=f.toProceedure(getDictionary());
        } //else throw new ParserException("var,int,real,constant,'[' or functor","bad term",n);
        return t;
    } 

    protected ITerm getTerm() throws IOException {
        ITerm n=next();
        return getTerm(n);
    } 

    private final ITerm[] getArgs() throws IOException {
        ITerm n=next(); 
        if ( !(n instanceof lparToken) )
            throw new ParserException("'('","in getArgs",n);
        Vector v=new Vector();
        ITerm t=getTerm();
        v.addElement(t);
        for ( ;; ) {
            n=next();
            if ( n instanceof rparToken ) {
                ITerm args[]=new ITerm[v.size()];
                v.copyInto(args);
                return args;
            } else if ( n instanceof commaToken ) {
                t=getTerm();
                v.addElement(t);
            } else {
                throw new ParserException("',' or ')'","bad arg",n);
            }
        }
    } 

    private final ITerm getList() throws IOException {
        ITerm n=next();
        if ( n instanceof rbraToken ) return Atom.aNil;
        ITerm t=getTerm(n);
        return getListCont(t);
    } 

    private final ITerm getListCont(ITerm curr) 
    throws IOException {
        //IO.traceln("curr: "+curr);
        ITerm n=next();ITerm t=null;
        if ( n instanceof rbraToken ) t=new Cons(curr,Atom.aNil);
        else if ( n instanceof barToken ) {
            t=new Cons(curr,getTerm());
            n=next(); 
            if ( !(n instanceof rbraToken) ) {
                throw new ParserException("']'","bad list end after '|'",n);
            }
        } else if ( n instanceof commaToken ) {
            ITerm other=getTerm();
            t=new Cons(curr,getListCont(other));
        }
        if ( t==null )
            throw new ParserException("| or ]","bad list continuation",n);
        return t;
    }


    public static final String normalizeParseString(String s) {
        if ( !(s.lastIndexOf(".")>=s.length()-2) ) s=s+".";
        return s;
    }

    public static HornClause clauseFromString(ITermMap tm,String s) {
        if ( null==s ) return null;
        s=normalizeParseString(s);
        HornClause t=null;
        try {
            PrologParser p;
            p=new PrologParser(tm,s);
            t=p.readClause();
        } catch ( Exception e ) { // nothing expected to catch
            IO.errmes("unexpected parsing error",e);
        }
        if ( t.vars==null ) t.ground=false;
        else t.ground=t.vars.isEmpty();
        return t;
    }

}


class ParserException extends IOException {
    public ParserException(String e,String f,ITerm n) {
        super(
             "expected: "+e+", found: "+f+
             "'"+n+"'");
    }
}

