package  logicmoo.obj;


import logicmoo.Jamud;
import logicmoo.util.*;
import java.util.*;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public class ConversationManager {

   public static final String MARKUP = "CONVERSATIONMANAGER";

   private Hashtable boards;
      public Hashtable getConversations() { return boards; }
		public Conversation get(String boardName) { return (Conversation) boards.get(boardName); }
		public void put(Conversation board) { boards.put(board.getName(), board); }


   public ConversationManager(XMLElement xml) throws Exception {
      if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in ConversationManager constructor");

      boards = new Hashtable();

      for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
         Conversation p = Conversation.loadConversation((XMLElement) enum.nextElement());
         boards.put(p.getName(), p);
      }

   }

}