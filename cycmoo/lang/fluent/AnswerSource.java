package jinni.kprolog;  //tarau.jinni;
import java.util.Vector;
import java.util.Enumeration;
import java.util.Stack;

/**
  Basic toplevel Prolog Engine. Loads and executes Prolog
  programs and spawns threads executing on new Prolog Engine
  objects as well as networking threads and 
  synchronized local and remote Linda transactions
*/
public class AnswerSource extends SourceFluent implements IKernel, Runnable {
    // CONSTRUCTORS

    /**
      Creates a IKernel starting execution with argument "goal" 
    */
    public AnswerSource(HornClause goal,IKernel parent) {
        super(parent);
        this.parent=parent;
        this.blackboard=parent.getBlackboard();
        goal=goal.ccopy();
        this.trail=new TermStack();
        this.orStack=new Stack();
        if ( null!=goal ) orStack.push(new UnfoldingSourceFluent(goal,this));
    }    

    // INSTANCE FIELDS

    private TermStack trail;
    private Stack orStack;
    private IKernel parent;
    private Blackboard blackboard;

    public final TermStack getTrail() {
        return trail;
    }

    public final IKernel getParent() {
        return parent;
    }

    public final Blackboard getBlackboard() {
        return blackboard;
    }

    // INSTANCE METHODS

    public ITerm getElement() {
        if ( null==orStack ) return null;
        HornClause answer=null;
        while ( !orStack.isEmpty() ) {
            UnfoldingSourceFluent I=(UnfoldingSourceFluent)orStack.pop();
            answer=I.getAnswer();
            if ( null!=answer ) break;
            HornClause goal=(HornClause)I.getElement();
            if ( null!=goal ) {
                if ( I.notLastClause() )
                    orStack.push(I);
                else
                    I.stop();
                if ( null==answer )orStack.push(new UnfoldingSourceFluent(goal,this));
            }
        }
        ITerm head;
        if ( null==answer ) {
            head=null;
            stop();
        } else
            head=answer.getHead();
        return head;
    }

    public void stop() {
        if ( null!=trail ) {
            trail.unwind(0);
            trail=null;
        }
        orStack=null;
    }

    public void run() {
        for ( ;; ) {
            ITerm Answer=getElement();
            if ( null==Answer ) break;
        }
    }
}