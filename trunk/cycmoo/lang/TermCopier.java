package cycmoo.lang;  //tarau.jinni;
import java.util.*;

/**
  ITerm TermCopier agent. Has its own Variable dictionnary.
  Uses a generic action propagator which recurses over Terms.
*/
public class TermCopier extends Nonvar {
   private ITermMap varDict;

   /** creates a new TermCopier together with
     its related TermMap for variables
   */
   public  TermCopier() {
     varDict = KernelAgent.emptyTermMap();
   }

   /**
     This action only defines what happens here (at this
     <b> place </b>).  Ageneric mechanism will be used to recurse
     over Terms in a (truly:-)) OO style (well, looks more
     like some Haskell stuff, but who cares).
   */
   public ITerm action(ITerm place) {

     if(place instanceof Var) {
        Var root=(Var)varDict.get(place);
        if(null==root) {
          root=new PrologVar();
          varDict.put(place,root);
        }
        place = root;
     }

     return place;
   }

   //ITerm copyMe(ITerm that) {
   //   return that.reaction(this);
   //}


   /**
     Reifies an Enumeration as a Vector.
     Vector.elements can give back the enumeration if needed.
     @see TermCopier
   */
   static Vector EnumerationToVector(Enumeration e) {
     Vector V=new Vector();
     while(e.hasMoreElements()) {
       V.addElement(e.nextElement());
     }
     return V;
   }

   static Vector ConsToVector(Atom Xs) {
     Vector V=new Vector();
     ITerm t=Xs;
      for(;;) {
        if(t instanceof Nil) {
           break;
        }
        else if(t instanceof Cons) {
          Cons c=(Cons)t;
          V.addElement(c.getDrefArg(0));
          t=c.getDrefArg(1);
        }
        else if(t instanceof Atom) {
          V.addElement((Atom)t);
          break;
        }
        else {
          V=null;
          IO.errmes("bad Cons in ConsToVector: "+t);
          break;
        }
      }
     //IO.mes("V="+V);
     return V;
   }

   /**
     Converts a reified Enumeration to functor
     based on name of Atom c and args being the elements of
     the Enumeration.
   */

   static ITerm toCompound(Atom c,Enumeration e) {
      Vector V=EnumerationToVector(e);
      int arity=V.size();
      if(arity==0) return c;
      Compound f=Compound.newCompound(c.name(),arity);
      for(int i=0; i<arity; i++) {
         f.setDirectArg(i,(ITerm)V.elementAt(i));
      }       
      return f;
   }


   /**
     Represents a list [f,a1...,an] as f(a1,...,an)
   */
   
   static Compound VectorToCompound(Vector V) {
     Atom f=(Atom)V.firstElement();
     int arity=V.size()-1;
     Compound T=Compound.newCompound(f.name(),arity);
     for(int i=0; i<arity; i++) {
         T.setDirectArg(i,(ITerm)V.elementAt(i+1));
     }
     return T;
   }
  

   /**
     Extracts the free variables of a ITerm, using a
     generic action/reaction mechanism which takes
     care of recursing over its structure.
     It can be speeded up through specialization.
   */
   final static Atom anAnswer=TermMap.defAtomStatic("answer");

   ITerm getMyVars(ITerm that) {
     /*ITerm*/ that.reaction(this);
     return toCompound(anAnswer,varDict.keys());
   }
}

