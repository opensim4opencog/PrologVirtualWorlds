package logicmoo.obj;


import logicmoo.Jamud;
import logicmoo.util.*;
import java.util.*;
import net.n3.nanoxml.*;


public class PostManager {

   public static final String MARKUP = "POSTMANAGER";

   private Hashtable boards;
      public Hashtable getBoards() { return boards; }
		public Board get(String boardName) { return (Board) boards.get(boardName); }
		public void put(Board board) { boards.put(board.getName(), board); }


   public PostManager(XMLElement xml) throws Exception {
      if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in PostManager constructor");

      boards = new Hashtable();

      for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
         Board p = Board.loadBoard((XMLElement) enum.nextElement());
         boards.put(p.getName(), p);
      }

   }

}