package cycmoo.lang; 
import cycmoo.lang.fluent.*;
import cycmoo.lang.object.*;
import cycmoo.lang.builtin.*;  //
/**
  Used in implementing uniform replacement of
  variables with new constants. useful for printing
  out with nicer variable names.

  @see Var
  @see HornClause
*/
public class VarNumberer extends Nonvar {
   public ITermMap dict;
   public int ctr;

   public VarNumberer() {
     dict = KernelAgent.emptyTermMap();
     ctr = 0;
   }

   public ITerm action(ITerm place) {
     place=place.dref();
     IO.traceln(">>action: "+place);
     if(place instanceof Var) {
        Atom root=(Atom)dict.get(place);
        if(null==root) {
          root=new PseudoVar(ctr++);
          dict.put(place,root);
        }
        place = root;
     }
     IO.traceln("<<action: "+place);
     return place;
   }
}
