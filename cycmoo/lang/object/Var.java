package cycmoo.lang;  //tarau.jinni;

/**
  Part of the ITerm hierarchy implmenting
  logical variables. They are subject to
  reset by application of and undo action keep
  on the trail stack.

  @see TermStack
  @see IKernel
  @see HornClause
  @see Nonvar
*/
public abstract class Var extends Term {
  public ITerm val;

  public Var() {
    val=this;
  }

  public int getArity() {
    return Term.VAR;
  }

  final boolean unbound() {
     return val==this;
  }

  public ITerm dref() {
    return unbound() ? this : val.dref();
  }
  
  public boolean bind_to(ITerm x,TermStack trail) { 
    val=x;
    trail.push(this);
    return true;
  }

  public void undo() {
    val=this;
  }

    public boolean unify_to(ITerm that,TermStack trail) { 
    // expects: this, that are dereferenced
    return val.bind_to(that,trail);
  }

  public boolean eq(ITerm x) { //  not a term compare!
    return dref()==x.dref();
  }

  public String getKey() {
    ITerm t=dref();
    if(t instanceof Var) return null;
    else return t.getKey();
  }


  public ITerm reaction(ITerm agent) {
    
    ITerm R=agent.action(dref());

    if(!(R instanceof Var)) {
      R=R.reaction(agent);
    }

    return R;
  }

  public String name() {
    return "_"+Integer.toHexString(hashCode());
  }
  
  public String toString() {
    return unbound() ? name() : dref().toString();
  }
}