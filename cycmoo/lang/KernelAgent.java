package cycmoo.lang;
import cycmoo.lang.fluent.*;
import cycmoo.lang.object.*;
import cycmoo.lang.builtin.*;  //
import java.util.Date;
import java.util.Map;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Enumeration;

/**
  Initializes Jinni. Sets up shared data areas.
  Ensures that lib.class, obtained from lib.pro->lib.java is loaded.
*/
public class KernelAgent extends Blackboard implements IProceedureMap {
    
    // CLASS FIELDS
    static public int tracing=1;
    static public final int version=104;
    static public KernelAgent firstInstance=null;
    static public KernelAgent currentInstance() {
        return firstInstance;
    }
    static public ITermMap newEmptyTermMap() {
        return new TermMap(new Hashtable());
        //return firstInstance;
    }
    static public ITermMap emptyTermMap() {
        return new TermMap(new Hashtable());
    }
    static public void main(String args[]) {
        IO.println(
                  "Kernel Prolog "+version/100.0+"\n"+
              //    "Copyright (c) Paul Tarau && BinNet Corp. 1999\n"+
                  "Open SourceFluent Edition, under GNU General Public License.\n"+
                  "Download latest version from: http://www.binnetcorp.com/kprolog/Main.html\n"+
                  "For commercial licensing, related service or support contracts\n"+
                  "and commercial extensions in binary form, contact BinNet Corporation at:\n"+
                  "binnetcorp@binnetcorp.com, http://www.binnetcorp.com\n");
        firstInstance = new KernelAgent(new HashMap());
        firstInstance.askJinni("reconsult('"+firstInstance.lastFile+"')");
        if ( !firstInstance.run(args) ) return;
        firstInstance.standardTop("?-"); // interactive
    } 

    /**
       Initialises key data areas. Runs a first query, which,
       if suceeeds a true, otherwise false is returned
    */
    public KernelAgent(Map map) {
        super(map);
        setBlackboard(this);
        new SystemProceedures(this);
    }

    //static public Hashtable systemObjectMap = new Hashtable();
    
    // Object tracking
    static public void objectPut(String named, ITerm any){
        if (firstInstance==null) return;
        currentInstance().put(named,any);
    }
    static public ITerm objectGet(String named){
       return (ITerm)currentInstance().get(named);
    }
    static public String objectKey(Object any){
       Enumeration e = currentInstance().keys();
       while (e.hasMoreElements()) {
           Object key=e.nextElement();
           if (currentInstance().get(key)==any) return key.toString();
       }
       return null;
    }
    static public void objectRemove(String named){
       currentInstance().remove(named);
    }



    public  IOPeer asPeer() {
        return IO.peer;
    }

    /**
    *  (almost) standard Prolog-like toplevel in Java
    *  (will) print out variables and values
    */
    public  void standardTop(String prompt) {
        for ( ;; ) {
            HornClause G=getGoal(IO.promptln(prompt));
            if ( null==G ) {
                continue;
            }
            IO.peer=null;
            timeGoal(G);
        }
    }
   
    public synchronized void setBlackboard(KernelAgent bb) {
        firstInstance = bb;
    }

    public HornClause getGoal(String line) {
        HornClause G=ClauseReader.goalFromString(this,line);
        //IO.mes("getGoal: "+G+" DICT: "+G.dict); //OK
        return G;
    }

    /**
    * evalutes a query
    */
    public  void evalGoal(HornClause Goal) {
        HornClause NamedGoal=Goal.cnumbervars(false);
        ITerm Names=NamedGoal.getHead();
        if ( !(Names instanceof Compound) ) { // no vars in Goal
            ITerm Result=firstSolution(Goal.getHead(),Goal.getBody());
            if ( !Atom.aNo.eq(Result) )
                Result=Atom.aYes;
            IO.println(Result.toString());
            return;
        }

        IKernel E=new AnswerSource(Goal,this);

        for ( int i=0;;i++ ) {
            ITerm R=E.getElement();
            //IO.mes("GOAL:"+Goal+"\nANSWER: "+R);
            if ( R==null ) {
                IO.println("no"); 
                break;
            }
            if ( Names instanceof Compound ) {
                Compound NamedR=(Compound)R.numbervars();
                for ( int j=0; j<Names.getArity(); j++ ) {
                    IO.println(((Compound)Names).getDrefArg(j)+
                               "="+NamedR.getDrefArg(j));
                }
                //IO.println(";");
                if ( !moreAnswers(i) ) {
                    E.stop();
                    break;
                }
            }
        }
    }

   boolean moreAnswers(int i) {
        if ( IO.maxAnswers==0 ) { // under user control
            String more=IO.promptln("; for more, <enter> to stop: ");
            return more.equals(";");
        } else if ( i<IO.maxAnswers || IO.maxAnswers<0 ) {
            IO.println(";"); // print all remaining
            return true;
        } else { // i >= ...}
            IO.println(";");
            IO.println("No more answers computed, max reached! ("+
                       IO.maxAnswers+")");
            return false;
        }
    }

    /**
    *  evaluates and times a Goal querying program P
    */

    public  void timeGoal(HornClause Goal) {
        long t1 = System.currentTimeMillis();
        try {
            evalGoal(Goal);
        } catch ( Throwable e ) {
            IO.errmes("Execution error in goal:\n  "+Goal.pprint()+".\n",e);
        }
        long t2 = System.currentTimeMillis();
        IO.println("Time: "+(t2-t1)/1000.0+" sec, threads="+
                   Thread.activeCount());
    }


    /**
     Asks Jinni a query Answer, Goal and returns the
     first solution of the form "the(Answer)" or the constant
     "no" if no solution exists
   */
    public  ITerm askJinni(ITerm Answer,ITerm Body) {
        return firstSolution(Answer,Body);
    }

    /**
      Asks Jinni a query Goal and returns the
      first solution of the form "the(Answer)" , where
      Answer is an instance of Goal or the constant
      "no" if no solution exists
    */
    public  ITerm askJinni(ITerm Goal) {
        return askJinni(Goal,Goal);
    }

    /**
      Asks Jinni a String query and gets back a string Answer
      of the form "the('[]'(VarsOfQuery))" containing a binding
      of the variables or the first solution to the query or "no"
      if no such solution exists
    */
    public  String askJinni(String query) {
        HornClause Goal=getGoal(query);
        ITerm Body=Goal.getBody();
        return askJinni(Body).pprint();
    }

    public  boolean run(String[] args) {
        if ( null!=args ) {
            for ( int i=0; i<args.length; i++ ) {
                String result=askJinni(args[i]);
                IO.traceln(result);
                if ( "no".equals(result.intern()) ) {
                    IO.errmes("failing cmd line argument: "+args[i]);
                    return false;
                }
            }
        }
        return true;
    }

}

