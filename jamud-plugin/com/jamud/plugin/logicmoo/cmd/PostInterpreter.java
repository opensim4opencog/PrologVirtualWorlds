package logicmoo.cmd;


import java.util.*;
import logicmoo.*;
import logicmoo.obj.*;
import logicmoo.util.*;

public abstract class PostInterpreter {

	public static boolean execute(Engineer src, String cmd, String args) {


		if("posts".startsWith(cmd) || "boards".startsWith(cmd)) {
			UNREAD(src);
			return true;
		}

		Conversation I = completeBoard(src, cmd);

		if(I == null) {
			return false;
		}
		else {
			String cmm[] = Util.split(args, " ");
			String c = cmm[0], wrds = cmm[1];

			if(c.equals(""))					NOTE(src, I);
			else if(c.equals("list"))		NOTE_LIST(src, I, wrds);
			else if(c.equals("reply"))		NOTE_REPLY(src, I, wrds);
			else if(c.equals("read"))		NOTE_READ(src, I, wrds);
			else if(c.equals("to"))			NOTE_RCPT(src, I, wrds);
			else if(c.equals("to+"))		NOTE_RCPT_ADD(src, I, wrds);
			else if(c.equals("to-"))		NOTE_RCPT_REM(src, I, wrds);
			else if(c.equals("subject"))	NOTE_SUBJ(src, I, wrds);
			else if(c.equals("+"))			NOTE_ADDLINE(src, I, wrds);
			else if(c.equals("-"))			NOTE_REMLINE(src, I);
			else if(c.equals("write"))		NOTE_WRITE(src, I);
			else if(c.equals("append"))	NOTE_APPEND(src, I);
			else if(c.equals("clear"))		NOTE_CLEAR(src, I);
			else if(c.equals("post"))		NOTE_POST(src, I);
			else if(c.equals("remove"))	NOTE_REMOVE(src, I, wrds);
			else if(c.equals("show"))		NOTE_SHOW(src, I);
			else if(c.equals("format"))	NOTE_FORMAT(src, I, wrds);
			else									NOTE_HUH(src);
			return true;
		}

	}

   private static Conversation completeBoard(Engineer plr, String cmd) {
      for(Enumeration enum = Jamud.getConversationManager().getConversations().elements(); enum.hasMoreElements(); ) {
			Conversation b = (Conversation) enum.nextElement();
         String a = b.getName().toLowerCase();
			int I = b.getAclRead();
			int T = plr.getTrust();
         if(T >= I && a.startsWith(cmd)) return b;
      }
      return null;
   }

	private static InformationTransferEvent verifyBoard(Engineer src, Conversation board) {
		InformationTransferEvent p = (InformationTransferEvent) src.getProperty(Engineer.PROPERTY_CURRENT_NOTE);
		if(p == null) {
			int I = board.getAclWrite();
			int T = src.getTrust();
			if(I <= T) {
				p = new InformationTransferEvent(board);
				p.setFrom(src.getName());
				src.putProperty(Engineer.PROPERTY_CURRENT_NOTE, p);
				return p;
			}
			else {
				src.print("You are not authorized to post on that board.");
				return null;
			}
		}
		else {
			if(p.getConversation() == board) {
				return p;
			}
			else {
				return null;
			}
		}
	}

	public static InformationTransferEvent findInformationTransferEvent(Conversation board, Engineer src, String depth) {
		Vector n = board.getInformationTransferEvents(src);
		// System.out.println(" looking for " + depth + " in " + board.getName());
		try {
			InformationTransferEvent p = null;
			for(String[] sp = Util.split(depth, "."); sp[0].length() + sp[1].length() > 0; sp = Util.split(sp[1], ".")) {
				p = (InformationTransferEvent) n.get(Integer.parseInt(sp[0]));
				n = Conversation.getInformationTransferEvents(p.getReplies(), src);
			}
			return p;
		}
		catch(Exception e) {
			return null;
		}

	}


	/*
	 *	here begin the actual methods associated with
	 * the commands for this interpreter
	 *
	*/


	private static void NOTE_HUH(Engineer src) {
		src.println("Sorry, you can't do that.");
	}

	private static void NOTE_LIST(Engineer src, Conversation board, String depth) {
		Vector n = null;
		if(depth.length() > 0) {
			InformationTransferEvent p = findInformationTransferEvent(board, src, depth);
			if(p != null) n = Conversation.getInformationTransferEvents(p.getReplies(), src);
		}
		else {
			n = board.getInformationTransferEvents(src);
		}

		if(n == null) {
			NOTE_HUH(src);
			return;
		}

		// System.out.println("Starting to build the string");

		String o = " {#+---{6" + board.getName();
		if(depth.length() > 0) o +=  ": " + depth + ".*";
		o += "{#";
		o += Util.fixLength("-",(59 - o.length()) , '-', true) + "-+{0";
		int I=0;
		for(Enumeration enum = n.elements(); enum.hasMoreElements(); I++) {
			InformationTransferEvent p = (InformationTransferEvent) enum.nextElement();
			o += "\n  {#[{^" + Util.fixLength(""+I, 3, true) + "{#]{6 " + p.getFrom() + ": " + p.getSubject();
			int i = Conversation.getInformationTransferEvents(p.getReplies(), src).size();
			if(i > 0) o += " {*(" + i + " repl" + ((i == 1) ? "y" : "ies") + ")";
		}
		o = o + "\n {#+----------------------------------------------------+{0";
		src.println(o);
	}



	private static void NOTE_REPLY(Engineer src, Conversation board, String which) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			InformationTransferEvent s = findInformationTransferEvent(board, src, which);
			if(s != null) {
				n.setParent(s);
				n.setRcpt(new FlagList(s.getRcpt()));
				n.getRcpt().add(s.getFrom());
				if(n.getSubject().length() == 0) n.setSubject("re:" + s.getSubject());
				src.println("Ok.");
			}
			else {
				src.println("Can't find post " + which);
			}
		}
	}


	private static void NOTE_FORMAT(Engineer src, Conversation board, String which) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			int width = 78;
			if(which.length() > 0) {
				try {
					width = Integer.parseInt(which);
				}
				catch(Exception e) {
					//empty
				}
			}

			n.setText(Util.format(n.getText().trim(), width, true));
			src.println("Wrapped to " + width + " columns.");
		}
	}

	private static void NOTE_READ(Engineer src, Conversation board, String which) {
		if(which.length() == 0) {
			src.println("Read which one?");
		}
		else {
			try {
				InformationTransferEvent p = findInformationTransferEvent(board, src, which);
				if(p == null) {
					src.println(board.getName() + "[" + which + "] not found.");
				}
				else {
					src.println(p.toString(which));
				}
			}
			catch(Exception e) {
				NOTE_HUH(src);
			}
		}
	}

	private static void NOTE_REMOVE(Engineer src, Conversation board, String which) {
		if(which.length() == 0) {
			src.println("Remove which one?");
		}
		else {
			try {
				InformationTransferEvent p = findInformationTransferEvent(board, src, which);
				if(p == null) {
					src.println(board.getName() + "[" + which + "] not found.");
				}
				else {
					int t = src.getTrust();
					if(p.getFrom().equals(src.getName()) || t > Engineer.TRUST_IMMORTAL) {
						//only authors and immortals may remove notes
						if(p.getReplies().size() == 0) {
							board.removeInformationTransferEvent(p);
							(new java.io.File(p.getSrc())).delete();
							board.save();
							src.println(board.getName() + "[" + which + "] removed.");
						}
						else {
							src.println(board.getName() + "[" + which + "] has " + p.getReplies().size() + " replies; you must remove them first.");
						}
					}
					else {
						src.println("You can't do that.");
					}
				}
			}
			catch(Exception e) {
				NOTE_HUH(src);
			}
		}
	}


	private static void NOTE_RCPT(Engineer src, Conversation board, String who) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			n.setRcpt(new FlagList(Util.split(who, " ")[0]));
			src.println("Ok.");
		}
	}

	private static void NOTE_RCPT_ADD(Engineer src, Conversation board, String who) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			if(who.length() == 0) {
				src.println("Add who to rcpt?");
			}
			else {
				n.getRcpt().add(Util.split(who, " ")[0]);
				src.println("Ok.");
			}
		}
	}

	private static void NOTE_RCPT_REM(Engineer src, Conversation board, String who) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			if(who.length() == 0) {
				src.println("Remove who from rcpt?");
			}
			else {
				n.getRcpt().rem(Util.split(who, " ")[0]);
				src.println("Ok.");
			}
		}
	}

	private static void NOTE_ADDLINE(Engineer src, Conversation board, String args) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			n.setText(Util.addLine(n.getText(), args));
			src.println("Ok.");
		}
	}

	private static void NOTE_REMLINE(Engineer src, Conversation board) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			n.setText(Util.remLine(n.getText()));
			src.println("Ok.");
		}
	}

	private static void NOTE_SUBJ(Engineer src, Conversation board, String args) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			n.setSubject(args);
			src.println("Ok.");
		}
	}

	private static void NOTE_WRITE(Engineer src, Conversation board) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			n.setText(src.getText("Enter Text Below. Type a period(.) on a line by itself to Stop."));
			src.println("Ok.");
		}
	}

	private static void NOTE_APPEND(Engineer src, Conversation board) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			n.setText(Util.addLine(n.getText(), src.getText("Enter Text Below. Type a period(.) on a line by itself to Stop.")));
			src.println("Ok.");
		}
	}

	private static void NOTE_CLEAR(Engineer src, Conversation board) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			src.getProperties().remove(Engineer.PROPERTY_CURRENT_NOTE);
			src.println("Ok.");
		}
	}

	private static void NOTE_SHOW(Engineer src, Conversation board) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			src.println(n.toString("new"));
		}
	}

	private static void NOTE_POST(Engineer src, Conversation board) {
		InformationTransferEvent n = verifyBoard(src, board);
		if(n==null) {
			src.println("You are not currently writing a " + board.getName().toLowerCase());
		}
		else {
			//verify it's all filled out.
			if(n.getRcpt().size() < 1) {
				src.println("You must specify a recipient.");
			}
			else if(n.getSubject().length() == 0) {
				src.println("You must specify a subject.");
			}
			else if(n.getText().length() == 0) {
				src.println("You must specify a message.");
			}
			else {
				n.doInformationTransferEventing();
				src.getProperties().remove(Engineer.PROPERTY_CURRENT_NOTE);
				src.println(board.getName() + " posted ok!");
			}
		}
	}

	private static void UNREAD(Engineer src) {
		String o = " {#+---{6Boards{#-------------------------------------------+{0";

      for(Enumeration enum = Jamud.getConversationManager().getConversations().elements(); enum.hasMoreElements(); ) {
			Conversation b = (Conversation) enum.nextElement();
			int I = b.getAclRead();
			int T = src.getTrust();
         if(T >= I) {
				o = o + "\n    " + Util.fixLength(b.getName(), 8, false) + b.getInformationTransferEvents(src).size();
			}
      }

		o = o + "\n {#+----------------------------------------------------+{0";
		src.println(o);
	}


	private static void NOTE(Engineer src, Conversation board) {
		//find first unread post in board and show it.
		src.println("");
	}

}