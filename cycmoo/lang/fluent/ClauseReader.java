package cycmoo.lang.fluent;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;
import cycmoo.lang.parser.PrologParser;  //
import java.io.Reader;
import java.io.IOException;

/**
  Builds Jinni Fluents from Java
  Streams
*/
public class ClauseReader extends CharReader {
    protected PrologParser parser;
    static public HornClause goalFromString(ITermMap tm, String line) {
        IO.traceln("read string: <"+line+">");

        if ( null==line )
            line=Atom.anEof.name();
        else if ( 0==line.length() )
            return null;

        HornClause C=PrologParser.clauseFromString(tm,line);
        if ( null==C ) {
            IO.errmes("warning (null HornClause):"+line);
            return null;
        }

        //IO.traceln("got goal:\n"+C.toGoal()); //OK
        return C.toGoal();
    }

    public ClauseReader(Reader reader,IKernel p) {
        super(reader,p);
        make_parser(p.getBlackboard(),"from shared reader");
    }

    public ClauseReader(String f,IKernel p) {
        super(f,p);
        make_parser(p.getBlackboard(),f);
    }

    public ClauseReader(IKernel p) {
        super(p);
        make_parser(p.getBlackboard(),"standard input");
    }

    /**
     * parses from a string representation of a term
     */
    public ClauseReader(ITerm t,IKernel p) {
        super(t,p);
        make_parser(p.getBlackboard(),"string parser");
    }

    public void make_parser(ITermMap tm, String f) {
        if ( null!=reader )
            try {
                this.parser=new PrologParser(tm,reader);
            } catch ( IOException e ) {
                IO.errmes("unable to build parser for: " +f);
            } else
            this.parser=null;
    }

    public ITerm getElement() {
        HornClause C=null;
        if ( //IO.peer!=null && 
             reader.equals(IO.input) ) {
            String s=IO.promptln(">:");
            if ( null==s||0==s.length() ) C=null;
            else C=PrologParser.clauseFromString(parent.getBlackboard(),s);
        } else if ( null!=parser ) {
            if ( parser.atEOF() ) {
                C=null;
                stop();
            } else
                C=parser.readClause();
            if ( C!=null && C.getHead().eq(Atom.anEof) ) {
                C=null;
                stop();
            }
        }
        return extract_info(C);
    }

    static public Compound extract_info(HornClause C) {
        if ( null==C ) return null;
        ITerm Vs=C.varsOf();
        HornClause SuperC=new HornClause(Vs,C);
        SuperC.vars=C.vars;
        HornClause NamedSuperC=SuperC.cnumbervars(false);
        ITerm Ns=NamedSuperC.getHead();
        ITerm NamedC=NamedSuperC.getBody();
        Compound R = Compound.newCompound("clause",4);
        R.setDirectArg(0,C);
        R.setDirectArg(1,Vs);
        R.setDirectArg(2,NamedC);
        R.setDirectArg(3,Ns);
        return R;
    }

    public void stop() {
        super.stop();
        parser=null;
    }
}
