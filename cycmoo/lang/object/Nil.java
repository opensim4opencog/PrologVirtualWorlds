package cycmoo.lang.object; 
import cycmoo.lang.*; 
import cycmoo.lang.fluent.*;
import cycmoo.lang.builtin.*;  //
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
 
 
