package cycmoo.lang.fluent;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;
import java.util.Enumeration;
import java.util.Vector;

abstract public class SourceFluent extends Fluent  implements IFluent  {

    public SourceFluent(IKernel p) {
      super(p);
    }
  
  abstract public ITerm getElement();
  
  public Atom toList() {
     ITerm head=getElement();
     if(null==head) return (Atom)Atom.aNil;
     Cons l=new Cons(head,Atom.aNil);
     Cons curr=l;
     for(;;) {
       head=getElement();
       if(null==head) break;
       Cons tail=new Cons(head,(ITerm)Atom.aNil);
       curr.setDirectArg(1,tail);
       curr=tail;
     }
     return l;
   }

  public ITerm toCompound() {
     Vector V=new Vector();
     ITerm X;
     while(null!=(X=getElement())) {
       V.addElement(X);
     }
     return TermCopier.VectorToCompound(V);
  }
}
