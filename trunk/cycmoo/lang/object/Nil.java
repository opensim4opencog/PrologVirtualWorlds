package cycmoo.lang;  //tarau.jinni;
/**
  Special constant terminating a list
*/
public class Nil extends Atom {
   private Nil(String s) {
     super();
     setSym(s);
   }

   public Nil() {
     this("[]");
   }
}
 
 
