package cycmoo.lang.builtin;  //
import cycmoo.lang.fluent.*;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;
import java.util.Enumeration;
import java.util.Vector;
import java.util.Hashtable;
import java.util.*;
import java.io.Reader;
import java.io.*;
import java.lang.reflect.*;

/**
 This class contains a dictionary of all builtins i.e.
 Java based classes callable from Prolog.
 They should provide a constructor and an exec method.
 @author Paul Tarau
*/
public class SystemProceedures {

    /**
       This constructor bb.registers builtins. Please put a header here
       if you add a builtin at the bottom of this file.
    */
    public SystemProceedures(IProceedureMap bb) {
        // add a line here for each new builtin

        // basics
        bb.register(new is_builtin());
        bb.register(Atom.aTrue);
        bb.register(Atom.aFail);
        bb.register(new halt());
        bb.register(new compute());

        // I/O and traceln related
        bb.register(new get_stdin());
        bb.register(new get_stdout());
        bb.register(new set_max_answers());
        bb.register(new set_trace());
        bb.register(new stack_dump());
        bb.register(new consult());
        bb.register(new reconsult());
        bb.register(new reconsult_again());

        // database
        bb.register(new at_key());
        bb.register(new pred_to_string());
        bb.register(new db_to_string());

        bb.register(new new_db());
        bb.register(new get_default_db());
        bb.register(new db_remove());
        bb.register(new db_add());
        bb.register(new db_collect());
        bb.register(new db_source());

        // data structure builders/converters
        bb.register(new arg());
        bb.register(new new_fun());
        bb.register(new get_arity());
        bb.register(new name_to_chars());
        bb.register(new chars_to_name());
        bb.register(new numbervars());

        // fluent constructors 
        bb.register(new unfolder_source());
        bb.register(new answer_source());

        bb.register(new source_list());
        bb.register(new list_source());

        bb.register(new term_source());
        bb.register(new source_term());

        bb.register(new integer_source());
        bb.register(new source_loop());

        // IFluent Modifiers

        bb.register(new set_persistent());
        bb.register(new get_persistent());

        // Input Sources
        bb.register(new file_char_reader());
        bb.register(new char_file_writer());

        bb.register(new file_clause_reader());
        bb.register(new clause_file_writer());

        // writable Sinks
        bb.register(new term_string_collector());
        bb.register(new term_collector());

        bb.register(new string_char_reader());
        bb.register(new string_clause_reader());

        // fluent controllers
        bb.register(new get());
        bb.register(new put());
        bb.register(new stop());
        bb.register(new collect());

        // fluent combinators
        bb.register(new split_source());
        bb.register(new merge_sources()); 
        // see compose_sources,append_sources,merge_sources in lib.pro

        // discharges a IFluent to a IFluent
        bb.register(new discharge());

        // multi-var operations
        bb.register(new def());
        bb.register(new set());
        bb.register(new val());   

        // lazy list operations  
        bb.register(new source_lazy_list());
        bb.register(new lazy_head());
        bb.register(new lazy_tail());

        // OS and process interface
        bb.register(new system());
        bb.register(new ctime());

        // Java interfaces
        bb.register(new jtype());
        bb.register(new jtype_arg());
        bb.register(new jpromote());
        bb.register(new invoke_java3());
        bb.register(new java_info1());
        blackboard = (TermMap/**/)bb;
    }                                     

    private TermMap/**/ blackboard=null;
    public TermMap/**/ getDatabase(IKernel p) {
        if ( p!=null ) return p.getBlackboard();
        return blackboard;
    }

    // Code for actual kernel SystemProceedures:
    // add your own builtins in UserProceedures.java, by cloning the closest example:-)

    class invoke_java3 extends CompoundProceedure {
        invoke_java3() {
            super("invoke_java",3);
        }

        public int exec(IKernel p) {
            Object o = getDrefArg(0).toObject();
            try {
                Method method = o.getClass().getMethod(getDrefArg(1).toUnquoted(),null);
                return putArg(2,ForeignObject.getRef(method.invoke(o,null)) ,p);
            } catch ( Exception e ) {
                e.printStackTrace();
                return -1;
            }
        }
    }
    class java_info1 extends CompoundProceedure {
        java_info1() {
            super("java_info",1);
        }

        public int exec(IKernel p) {
            Object o = getDrefArg(0);
            pri(o);
            return 1;
        }
         void pri(Object o) {
            if ( o instanceof ForeignObject ) {
                IO.println("% ForeignObject: ");
                pri(((ForeignObject) o).toUnstubbed());
            }
            if ( o instanceof Nonvar ) {
                IO.println("% Nonvar");
            }
            if ( o instanceof Proxy ) {
                IO.println("% Proxy");
            }
            Class c = o.getClass();
            IO.println("% "+c);
            IO.println((new ArrayList(Arrays.asList(c.getClasses()))).iterator());
            IO.println(Arrays.asList(c.getInterfaces()).iterator());
        }
    }
    class jpromote extends CompoundProceedure {
        jpromote() {
            super("jpromote",2);
        }

        public int exec(IKernel p) {
            return putArg(1,((FunctionObject)getDrefArg(0)).registerObject(),p);
        }
    }
    class jtype_arg extends CompoundProceedure {
        jtype_arg() {
            super("jtype_arg",2);
        }

        public int exec(IKernel p) {
            return putArg(1,ForeignObject.getStub(getDirectArg(0).getClass()),p);
        }
    }
    class jtype extends CompoundProceedure {
        jtype() {
            super("jtype",2);
        }

        public int exec(IKernel p) {
            return putArg(1,ForeignObject.getStub(getDrefArg(0).getClass()),p);
        }
    }
    /**
     * checks if something is a builtin
     */
    class is_builtin extends CompoundProceedure {
        is_builtin() {
            super("is_builtin",1);
        }

        public int exec(IKernel p) {
            return getDrefArg(0).isProceedure()?1:0;
        }
    }

    /**
      does its best to halt the program:-)
      to be thoroughly tested with Applets
    */
    class halt extends AtomProceedure {
        halt() {
            super("halt");
        }

        public int exec(IKernel p) {
            if ( IO.applet!=null ) { //applet
                IO.peer.halt();
                p.stop();
            } else // application
                Runtime.getRuntime().exit(0); 
            return 1;
        }
    }

    /**
     * Calls an external program
     */
    class system extends CompoundProceedure {
        system() {
            super("system",1);
        }

        public int exec(IKernel p) {
            String cmd=((Atom)getDrefArg(0)).name();
            return IO.system(cmd);
        }
    }

    /**
      opens a reader returning the content of a file char by char
    */
    class file_char_reader extends CompoundProceedure {
        file_char_reader() {
            super("file_char_reader",2);
        }

        public int exec(IKernel p) {
            ITerm I=getDrefArg(0);
            IFluent f;
            if ( I instanceof CharReader )
                f=new CharReader(((CharReader)I).reader,p);
            else {
                String s=((Atom)I).name();
                f=new CharReader(s,p);
            }
            return putArg(1,f,p);
        }
    }

    /**
      opens a reader returning clauses from a file
    */
    class file_clause_reader extends CompoundProceedure {
        file_clause_reader() {
            super("file_clause_reader",2);
        }

        public int exec(IKernel p) {
            ITerm I=getDrefArg(0);
            IFluent f;
            if ( I instanceof CharReader )
                f=new ClauseReader(((CharReader)I).reader,p);
            else {
                String s=((Atom)getDrefArg(0)).name();
                f=new ClauseReader(s,p);
            }
            return putArg(1,f,p);
        }
    }

    /**
      opens a writer which puts characters to a file one by one
    */
    class char_file_writer extends CompoundProceedure {
        char_file_writer() {
            super("char_file_writer",2);
        }

        public int exec(IKernel p) {
            String s=((Atom)getDrefArg(0)).name();
            IFluent f=new CharWriter(s,p);
            return putArg(1,f,p);
        }
    }

    /**
      opens a writer which puts characters to a file one by one
    */
    class clause_file_writer extends CompoundProceedure {
        clause_file_writer() {
            super("clause_file_writer",2);
        }

        public int exec(IKernel p) {
            String s=((Atom)getDrefArg(0)).name();
            IFluent f=new ClauseWriter(s,p);
            return putArg(1,f,p);
        }
    }
    /**
      get the standard output (a reader)
    */
    class get_stdin extends CompoundProceedure {
        get_stdin() {
            super("get_stdin",1);
        }

        public int exec(IKernel p) {
            return putArg(0,new ClauseReader(p),p);
        }
    }

    /**
      get standard output (a writer)
    */
    class get_stdout extends CompoundProceedure {
        get_stdout() {
            super("get_stdout",1);
        }

        public int exec(IKernel p) {
            return putArg(0,new ClauseWriter(p),p);
        }
    }


    /**
      gets an arity for any term:
      n>0 for f(A1,...,An)
      0 for a constant like a
      -1 for a variable like X
      -2 for an integer like 13
      -3 for real like 3.14
      -4 for a wrapped ForeignObject;
      @see ITerm#getArity
    */
    class get_arity extends CompoundProceedure {
        get_arity() {
            super("get_arity",2);
        }

        public int exec(IKernel p) {
            IntTerm N=new IntTerm(getDrefArg(0).getArity());
            return putArg(1,N,p);
        }
    }

    /**
     * Dumps the current Java Stack
     */
    class stack_dump extends CompoundProceedure {

        stack_dump() {
            super("stack_dump",1);
        }

        public int exec(IKernel p) {
            String s=getDrefArg(0).toString();
            IO.errmes("User requested dump",(new Exception(s)));
            return 1;
        }
    }

    /**
      returns the real time spent up to now
    */
    class ctime extends CompoundProceedure {

        ctime() {
            super("ctime",1);
        }

        private final long t0=System.currentTimeMillis();

        public int exec(IKernel p) {
            ITerm T=new IntTerm(System.currentTimeMillis()-t0);  
            return putArg(0,T,p);
        }
    }


    /**
      sets max answer counter for toplevel query
      if == 0, it will prompt the user for more answers 
      if > 0 it will not print more than IO.maxAnswers
      if < 0 it will print them out all
    */
    class set_max_answers extends CompoundProceedure {
        set_max_answers() {
            super("set_max_answers",1);
        }

        public int exec(IKernel p) {
            IO.maxAnswers=getIntArg(0);
            return 1;
        }
    }

    /**
      reconsults a file of clauses while overwriting old predicate
      definitions
      @see consult
    
    */

    class reconsult extends CompoundProceedure {
        reconsult() {
            super("reconsult",1);
        }

        public int exec(IKernel p) {
            String f=((Atom)getDrefArg(0)).name();
            return getDatabase(p).fromFile(f)?1:0;
        }
    }

    /**
      consults a file of clauses while adding clauses to
      existing predicate definitions
      @see reconsult
    */
    class consult extends CompoundProceedure {
        consult() {
            super("consult",1);
        }

        public int exec(IKernel p) {
            String f=((Atom)getDrefArg(0)).name();
            IO.traceln("consulting: "+f);
            return getDatabase(p).fromFile(f,false)?1:0;
        }
    }

    /**
      shorthand for reconsulting the last file
    */
    class reconsult_again extends AtomProceedure {
        reconsult_again() {
            super("reconsult_again");
        }

        public int exec(IKernel p) {
            return getDatabase(p).fromFile()?1:0;
        }
    }

    /**
     * gets default database
     */
    class get_default_db extends CompoundProceedure {
        get_default_db() {
            super("get_default_db",1);
        }

        public int exec(IKernel p) {
            return putArg(0,getDatabase(p),p);
        }
    }

    // databse operations

    /**
     * creates new database
     */
    class new_db extends CompoundProceedure {
        new_db() {
            super("new_db",1);
        }

        public int exec(IKernel p) {
            return putArg(0,new Blackboard(new Hashtable()),p);
        }
    }
    /**
      Puts a term on the local blackboard
    */
    class db_add extends CompoundProceedure {

        db_add() {
            super("db_add",2);
        }

        public int exec(IKernel p) {
            TermMap/**/ db=(TermMap/**/)getDrefArg(0).toObject();
            ITerm X=getDrefArg(1);
            //IO.mes("X==>"+X);
            String key=X.getKey();
            //IO.mes("key==>"+key);
            if ( null==key ) return 0;
            db.out(key,X);
            //IO.mes("res==>"+R);
            return 1;
        }
    }

    /**
      removes a matching term if available, fails otherwise
    */
    class db_remove extends CompoundProceedure {

        db_remove() {
            super("db_remove",3);
        }

        public int exec(IKernel p) {
            TermMap/**/ db=(TermMap/**/)getDrefArg(0).toObject();
            ITerm X=getDrefArg(1);
            ITerm R=db.cin(X.getKey(),X);
            return putArg(2,R,p);
        }
    }


    /**
      collects all matching terms in a (possibly empty) list
      @see out
      @see in
    */
    class db_collect extends CompoundProceedure {

        db_collect() {
            super("db_collect",3);
        }

        public int exec(IKernel p) {
            TermMap/**/ db=(TermMap/**/)getDrefArg(0).toObject();
            ITerm X=getDrefArg(1);
            ITerm R=db.all(X.getKey(),X);
            return putArg(2,R,p);
        }
    }

    /**
     * Maps a TermMap to a IFluent enumerating its elements
     */
    class db_source extends CompoundProceedure {

        db_source() {
            super("db_source",2);
        }

        public int exec(IKernel p) {
            TermMap/**/ db=(TermMap/**/)getDrefArg(0).toObject();
            IFluent S=new EnumerationSource(db.toEnumeration(),p);
            return putArg(1,S,p);
        }
    }

    /**
      collects all matching terms in a (possibly empty) list
    */
    class at_key extends CompoundProceedure {

        at_key() {
            super("at_key",2);
        }

        public int exec(IKernel p) {
            ITerm R=getDatabase(p).all(getDrefArg(0).getKey(),new PrologVar());
            return putArg(1,R,p);
        }
    }

    /**
     * Returns a representation of predicate as a string constant
     */
    class pred_to_string extends CompoundProceedure {

        pred_to_string() {
            super("pred_to_string",2);
        }

        public int exec(IKernel p) {
            String key=getDrefArg(0).getKey();
            String listing=getDatabase(p).pred_to_string(key);
            if ( null==listing ) return 0;
            return putArg(1,getDatabase(p).useStringAtom(listing),p);
        }
    }


    /**
      lists all the local blackboard to a string (Linda terms + clauses)
    */
    class db_to_string extends CompoundProceedure {
        db_to_string() {
            super("db_to_string",1);
        }

        public int exec(IKernel p) {
            return putArg(0,getDatabase(p).useAtom(getDatabase(p).pprint()),p);
        }
    }


    /**
      arg(I,ITerm,X) unifies X with the I-the argument of functor T
    */
    class arg extends CompoundProceedure {
        arg() {
            super("arg",3);
        }

        public int exec(IKernel p) {
            int i=getIntArg(0);
            Compound F=(Compound)getDrefArg(1);
            ITerm A = 
            (i==0)
            ?getDatabase(p).useAtom(F.name())
            :((i==-1)
              ? new IntTerm(F.getArity())
              : F.getDirectArg(i-1)
             );
            return putArg(2,A,p);
        }
    }

    /**
      new_fun(F,N,T) creates a term T based on functor F with arity
      N and new free varables as arguments
    */
    class new_fun extends CompoundProceedure {
        new_fun() {
            super("new_fun",3);
        }

        public int exec(IKernel p) {
            String s=((Atom)getDrefArg(0)).name();
            int i=getIntArg(1);
            ITerm T;
            if ( i==0 ) T=getDatabase(p).defAtom(s).toProceedure(getDatabase(p));
            else {
                Compound F=Compound.newCompound(s,i);
                F.initArgs(i);
                T=F.toProceedure(getDatabase(p));
            }
            return putArg(2,T,p);
        }
    }

    /**
      converts a name to a list of chars
    */
    class name_to_chars extends CompoundProceedure {
        name_to_chars() {
            super("name_to_chars",2);
        }

        public int exec(IKernel p) {
            ITerm Cs=((Nonvar)getDrefArg(0)).toChars();
            return putArg(1,Cs,p);
        }
    }

    /**
      converts a name to a list of chars
    */
    class chars_to_name extends CompoundProceedure {
        chars_to_name() {
            super("chars_to_name",3);
        }

        public int exec(IKernel p) {
            String s=charsToString((Nonvar)getDrefArg(1));
            ITerm T=getDatabase(p).useAtom(s);
            if ( getIntArg(0)>0 ) { // convert?
                try {
                    double r=Double.valueOf(s).doubleValue();
                    if ( Math.floor(r)==r ) T=new IntTerm((long)r);
                    else T=new RealNumTerm(r);
                } catch ( NumberFormatException e ) {
                }
            }
            return putArg(2,T,p);
        }
    }

    /**
     * returns a copy of a ITerm with variables uniformly replaced with 
     * constants 
     */
    class numbervars extends CompoundProceedure {
        numbervars() {
            super("numbervars",2);
        }

        public int exec(IKernel p) {
            ITerm T=getDrefArg(0).numbervars();
            return putArg(1,T,p);
        }
    }

    /**
     * Performs simple arithmetic operations
     * like compute('+',1,2,Result)
     */
    class compute extends CompoundProceedure {
        compute() {
            super("compute",4);
        }

        public int exec(IKernel p) {

            ITerm o=getDrefArg(0); ITerm a=getDrefArg(1); ITerm b=getDrefArg(2);
            if ( !(o instanceof Atom) || !(a instanceof NumericTerm) || !(b instanceof NumericTerm) )
                IO.errmes(
                         "bad arithmetic operation ("+o+"): "+
                         a+","+b+"\nprog: "+p.toString());
            String opname=((Atom)o).name();
            double x=((NumericTerm)a).getValue();
            double y=((NumericTerm)b).getValue();
            double r;
            char op=opname.charAt(0);
            switch ( op ) {
            case '+':r=x+y;break;
            case '-':r=x-y;break;
            case '*':r=x*y;break;
            case '/':r=x/y;break;
            case ':':r=(int)(x/y);break;
            case '%':r=x%y;break;
            case '?':r=(x<y)?(-1):(x==y?0:1);break; // compares!
            case 'p':r=Math.pow(x,y);break;
            case 'l':r=Math.log(y)/Math.log(x);break;
            case 'r':r=x*Math.random()+y;break;
            case '<':r=(long)x<<(long)y;break;
            case '>':r=(long)x>>(long)y;break;

            default: 
                IO.errmes("bad arithmetic operation <"+op+
                          "> on "+x+" and "+y);
                return 0;
            }
            NumericTerm R=((Math.floor(r)==r))?(NumericTerm)(new IntTerm((long)r)):(NumericTerm)(new RealNumTerm(r));
            return putArg(3,R,p);
        }
    }

    /**
     * controls traceln levels for debugging
     */
    class set_trace extends CompoundProceedure {
        set_trace() {
            super("set_trace",1);
        }

        public int exec(IKernel p) {
            KernelAgent.tracing=getIntArg(0);
            return 1;
        }
    }

    /**
      Explores a finite iterator and return its
      successive values as a list.
    */

    class source_list extends CompoundProceedure {
        source_list() {
            super("source_list",2);
        }

        public int exec(IKernel p) {
            SourceFluent S=(SourceFluent)getDrefArg(0);
            ITerm Xs=S.toList();
            return putArg(1,Xs,p);
        }
    }

    /* maps a IFluent to a Java Enumeration
    class JinniEnumeration extends FunctionObject implements Enumeration {
      JinniEnumeration(IFluent I) {
        this.I=I;
        this.current=this.I.getElement();
      }
    
      private IFluent I;
      private ITerm current;
    
      public boolean hasMoreElements() {
        if(null==current) {
          I=null;
          return false;
        }
        return true;
      }
    
      public Object nextElement() {
        ITerm next=current;
        current=I.getElement();
        return next;
      }
    }    */

    /**
     * maps a List to a IFluent
     */
    class list_source extends CompoundProceedure {

        list_source() {
            super("list_source",2);
        }

        public int exec(IKernel p) {
            IFluent E=new ListSource((Atom)getDrefArg(0),p);
            return putArg(1,E,p);
        }
    }

    /**
     * maps a ITerm to a IFluent
     */
    class term_source extends CompoundProceedure {

        term_source() {
            super("term_source",2);
        }

        public int exec(IKernel p) {
            TermSource E=new TermSource((Nonvar)getDrefArg(0),p);
            return putArg(1,E,p);
        }
    }


    /**
     * Creates an Integer IFluent which advances at most Fuel (infinite if Fule==0) 
     * Steps computing each time x:= a*x+b. Called as: integer_source(Fuel,A,X,B,NewSource)
     */
    class integer_source extends CompoundProceedure {

        integer_source() {
            super("integer_source",5);
        }

        public int exec(IKernel p) {
            IntegerSource E=new IntegerSource(
                                             ((IntTerm)getDrefArg(0)).longValue(),
                                             ((IntTerm)getDrefArg(1)).longValue(),
                                             ((IntTerm)getDrefArg(2)).longValue(),
                                             ((IntTerm)getDrefArg(3)).longValue(),
                                             p);
            return putArg(4,E,p);
        }
    }

    /**
      Builds a Looping IFluent from a IFluent.
    */
    class source_loop extends CompoundProceedure {
        source_loop() {
            super("source_loop",2);
        }

        public int exec(IKernel p) {
            IFluent s=(IFluent)getDrefArg(0);
            return putArg(1,new SourceLoop(s,p),p);
        }
    }

    /**
     * Builds a IFluent from a ITerm
     */
    class source_term extends CompoundProceedure {

        source_term() {
            super("source_term",2);
        }

        public int exec(IKernel p) {
            SourceFluent S=(SourceFluent)getDrefArg(0);
            ITerm Xs=((Atom)S.toCompound()).toProceedure(getDatabase(p));
            return putArg(1,Xs,p);
        }
    }


    // Solvers and iterators over clauses

    /**     * When called as answer_source(X,G,R), it
       builds a new clause and maps it to an IKernel        LD-resolution interpreter which will return one answer        at a time of the form "the(X)" using G as initial resolvent       and "no" when no more answers are available.
    */
    class answer_source extends CompoundProceedure {
        answer_source() {
            super("answer_source",3);
        }

        public int exec(IKernel p) {
            HornClause goal=new HornClause(getDrefArg(0),getDrefArg(1));
            IKernel U=new AnswerSource(goal,p);
            return putArg(2,U,p);
        }
    }


    /**
      Builds a new clause H:-B and maps it to an iterator
    */
    class unfolder_source extends CompoundProceedure {
        unfolder_source() {
            super("unfolder_source",2);
        }

        public int exec(IKernel p) {
            HornClause goal=getDrefArg(0).toClause();
            IKernel newp=new AnswerSource(goal,p);
            UnfoldingSourceFluent S=new UnfoldingSourceFluent(goal,newp);
            return putArg(1,S,p);
        }
    }

    /**
     generic IFluent advancement step,     similar to an iterator's nextElement operation,     gets one element from the IFluent
    */

    class get extends CompoundProceedure {
        get() {
            super("get",2);
        }

        public int exec(IKernel p) {
            return putArg(1,the(((SourceFluent)getDrefArg(0)).getElement()),p);
        }
    }

    /**
     generic IFluent advancement step,     sends one element to the IFluent 
    */

    class put extends CompoundProceedure {
        put() {
            super("put",2);
        }

        public int exec(IKernel p) {
            IFluent S=(IFluent)getDrefArg(0);
            ITerm X=getDrefArg(1);
            if ( 0==S.putElement(X) ) {
                IO.errmes("error in putElement: "+X);
            }
            return 1;
        }
    }


    /**
      frees a IFluent's resources and ensures
      it cannot produce/consume any new values
    */
    class stop extends CompoundProceedure {
        stop() {
            super("stop",1);
        }

        public int exec(IKernel p) {
            IFluent S=(IFluent)getDrefArg(0);
            S.stop();
            return 1;
        }
    }


    /**
      Splits a (finite) IFluent in two new ones
      which inherit the current state of the parent.
    */
    class split_source extends CompoundProceedure {
        split_source() {
            super("split_source",3);
        }

        public int exec(IKernel p) {
            SourceFluent original=(SourceFluent)getDrefArg(0);
            Atom Xs=original.toList();
            return 
            ( putArg(1,new ListSource(Xs,p),p)>0 &&
              putArg(2,new ListSource(Xs,p),p)>0
            )
            ?1
            :0;
        }
    }


    /**
      Merges all Sources contained in a List into one IFluent.
    */
    class merge_sources extends CompoundProceedure {
        merge_sources() {
            super("merge_sources",2);
        }

        public int exec(IKernel p) {
            Atom list=(Atom)getDrefArg(0);
            return putArg(1,new SourceMerger(list,p),p);
        }
    }

    /**
      Flushes to a IFluent the content of a IFluent IFluent
    */
    class discharge extends CompoundProceedure {
        discharge() {
            super("discharge",2);
        }

        public int exec(IKernel p) {
            IFluent from=(IFluent)getDrefArg(0);
            IFluent to=(IFluent)getDrefArg(1);
            for ( ;; ) {
                ITerm X=from.getElement();
                if ( null==X ) {
                    to.stop();
                    break;
                } else
                    to.putElement(X);
            }
            return 1;
        }
    }

    /**
      Collects a reference to or the content of a IFluent
    */

    class collect extends CompoundProceedure {
        collect() {
            super("collect",2);
        }
        public int exec(IKernel p) {
            IFluent s=(IFluent)getDrefArg(0);
            return putArg(1,the(s.collect()),p);
        }
    }

    /**
     * Builds a StringSink which concatenates String representations
     * of Terms with put/1 and the return their concatenation with collect/1
     */
    class term_string_collector extends CompoundProceedure {
        term_string_collector() {
            super("term_string_collector",1);
        }

        public int exec(IKernel p) {
            return putArg(0,new StringSink(p),p);
        }
    }

    /**
     * Builds a TermCollector IFluent which accumulates
     * Terms with put/1 and the return them with collect/1
     */
    class term_collector extends CompoundProceedure {
        term_collector() {
            super("term_collector",1);
        }

        public int exec(IKernel p) {
            return putArg(0,new TermCollector(p),p);
        }
    }

    /**
     * Creates a char reader from a String.
     */
    class string_char_reader extends CompoundProceedure {
        string_char_reader() {
            super("string_char_reader",2);
        }

        public int exec(IKernel p) {
            return putArg(1,new CharReader(getDrefArg(0),p),p);
        }
    }

    /**
     *  Creates a clause reader from a String.
     */
    class string_clause_reader extends CompoundProceedure {
        string_clause_reader() {
            super("string_clause_reader",2);
        }

        public int exec(IKernel p) {
            return putArg(1,new ClauseReader(getDrefArg(0),p),p);
        }
    }

    /**
     * def(Var,Val) Initializes a Multi_Variable Var to a value Val.
     */
    class def extends CompoundProceedure {
        def() {
            super("def",2);
        }

        public int exec(IKernel p) {
            Var X=(Var)getDrefArg(0);
            MultiVar V=new MultiVar(getDrefArg(1),p);
            X.bind_to(V,p.getTrail());
            return 1;
        }
    }


    /**
     * set(Var,Val) Sets a Multi_Variable Var to a value Val.
     */
    class set extends CompoundProceedure {
        set() {
            super("set",2);
        }

        public int exec(IKernel p) {
            MultiVar V=(MultiVar)getDrefArg(0);
            V.set(getDrefArg(1),p);
            return 1;
        }
    }


    /**
     * val(Var,Val) gets the value Val of Multi_Variable Var.
     */
    class val extends CompoundProceedure {
        val() {
            super("val",2);
        }

        public int exec(IKernel p) {
            MultiVar V=(MultiVar)getDrefArg(0);
            return putArg(1,V.val(),p);
        }
    }

    /**
     * set_persistent(IFluent,yes)
     * makes a IFluent persistent - i.e. likely to keep
     * its state on backtracking. This assumes that the
     * IFluent remains accessible by being saved in a Database
     * or as element of a IFluent with longer life span.
     * 
     * set_persistent(IFluent,no) makes the IFluent perish
     * on backtracking (default behavior)
     */
    class set_persistent extends CompoundProceedure {
        set_persistent() {
            super("set_persistent",2);
        }

        public int exec(IKernel p) {
            IFluent F=(IFluent)getDrefArg(0);
            Atom R=(Atom)getDrefArg(1);
            boolean yesno=!R.eq(Atom.aNo);
            F.setPersistent(yesno);
            return 1;
        }
    }

    /**
     * Gets the yes/no persistentcy value of a IFluent.
     */
    class get_persistent extends CompoundProceedure {
        get_persistent() {
            super("get_persistent",2);
        }

        public int exec(IKernel p) {
            IFluent F=(IFluent)getDrefArg(0);
            ITerm R=F.getPersistent()?Atom.aYes:Atom.aNo;
            return putArg(1,R,p);
        }
    }

    /**
     * Converts IFluent into a Lazy List which will memorize
     * its elements as it grows.
     */
    class source_lazy_list extends CompoundProceedure {
        source_lazy_list() {
            super("source_lazy_list",2);
        }

        public int exec(IKernel p) {
            IFluent S=(IFluent)getDrefArg(0);
            //S.setPersistent(true);
            ITerm X=((SourceFluent)S).getElement();
            ITerm Xs=(ITerm)Atom.aNil;
            if ( null!=X ) {
                Xs=new LazyList(X,S,new TermStack());
                p.getTrail().push(Xs);
            }
            return putArg(1,Xs,p);
        }
    }

    /**
     * returns the first element of a lazy list
     */
    class lazy_head extends CompoundProceedure {
        lazy_head() {
            super("lazy_head",2);
        }

        public int exec(IKernel p) {
            Cons L=(Cons)getDrefArg(0);
            return putArg(1,L.getHead(),p);
        }
    }

    /**
     * returns the tail if a lazy list after making it
     * grow, if possible
     */
    class lazy_tail extends CompoundProceedure {
        lazy_tail() {
            super("lazy_tail",2);
        }

        public int exec(IKernel p) {
            Cons L=(Cons)getDrefArg(0);
            return putArg(1,L.getTail(),p);
        }
    }

} // end SystemProceedures
