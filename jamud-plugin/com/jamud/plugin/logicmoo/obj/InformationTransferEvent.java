package  logicmoo.obj;


import logicmoo.*; 
import logicmoo.util.*;
import java.io.FileWriter;
import java.util.*;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public class InformationTransferEvent {

   public static final String MARKUP = "CONVERSATION";

   private static final String
		PROPERTY_SRC = "SRC",
		PROPERTY_PARENT = "PARENT",
      PROPERTY_FROM = "FROM",
      PROPERTY_RCPT = "RCPT",
      PROPERTY_DATE = "DATE";


   private Conversation board = null;
      public Conversation getConversation() { return board; }
      public void setConversation(Conversation board) { this.board = board; }

   private String src = "";
      public String getSrc() { return src; }
      public void setSrc(String src) { this.src = src; }

   private InformationTransferEvent parent = null;
      public InformationTransferEvent getParent() { return parent; }
      public void setParent(InformationTransferEvent parent) { this.parent = parent; }

	private long parentVnum = (long) -1;
      public long getParentVnum() { return parentVnum; }
		public void setParentVnum(long cycFort) { this.parentVnum = cycFort; }

   private Vector replies = new Vector();
      public Vector getReplies() { return replies; }

   private FlagList rcpt = new FlagList();
      public FlagList getRcpt() { return rcpt; }
      public void setRcpt(FlagList rcpt) { this.rcpt = rcpt; }

   private String from = "";
      public String getFrom() { return from; }
      public void setFrom(String from) { this.from = from; }

   private String subject = "";
      public String getSubject() { return subject; }
      public void setSubject(String subject) { this.subject = subject; }

   private String text = "";
      public String getText() { return text; }
      public void setText(String text) { this.text = text; }

	private long cycFort;
		public long getCycFort() { return cycFort; }
		public void setCycFort(long cycFort) { this.cycFort = cycFort; }

   private String date;
      public String getDate() { return date; }
      public void setDate(String date) { this.date = date; }

	private boolean ideaed = false;
		public boolean getInformationTransferEvented() { return ideaed; }
		public void setInformationTransferEvented(boolean ideaed) { this.ideaed = ideaed; }



   public InformationTransferEvent() {
		cycFort = System.currentTimeMillis();
      date = (new java.util.Date(cycFort)).toString();
   }

	public InformationTransferEvent(Conversation board) {
		this();
		this.board = board;
	}



   public void doInformationTransferEventing() {
		board.addInformationTransferEvent(this);

		if(parent != null)
			parent.getReplies().add(this);
		else
			board.getTopInformationTransferEvents().add(this);

	   setSrc(board.getPath() + cycFort);

      save();
      board.save();

		ideaed = true;
   }


	public String toString() {
		return toString(null);
	}


   public String toString(String ident) {
		if(ident == null)
			ident = "";

		StringBuffer sb = new StringBuffer("{3[{6" + board.getName() + " " + ident);

		if(replies.size() > 0)
			sb.append( ",  " + replies.size() + " Repl" + ((replies.size() == 1) ? "y" : "ies") );
		sb.append( "{3]{0\n" );

      sb.append( "{6From:{0 " + from + ", " + date + "\n" );

		String o = Util.replace(rcpt.toString(), ";", ", ");
		if(o.length() > 1)
			o = o.substring(0, o.length() - 2);

      sb.append( "{6  To:{0 " + o + "\n" );
      sb.append( "{6Subj:{0 " + subject + "\n" );
      sb.append( text );

      return sb.toString();
   }


   public boolean save() {
      try {
			XMLWriter xmw = new XMLWriter(new FileWriter(src));
			xmw.write(toXMLElement());
         return true;
      }
      catch(Exception e) {
         System.err.println("Exception in InformationTransferEvent.save()");
			e.printStackTrace();
         return false;
      }
   }


   public static InformationTransferEvent loadInformationTransferEvent(Conversation board, XMLElement xml) throws Exception {
      if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in InformationTransferEvent.loadInformationTransferEvent");

		InformationTransferEvent idea;

		String src = xml.getAttribute(PROPERTY_SRC, null);
		if(src == null) {
	      idea = new InformationTransferEvent();
			idea.ideaed = true;
			idea.board = board;
			idea.from = xml.getAttribute(PROPERTY_FROM);
	      idea.cycFort = Long.parseLong(xml.getAttribute(GuidTracable.PROPERTY_VNUM));

			String pr = (xml.getAttribute(PROPERTY_PARENT, null));
			if(pr != null && pr.length() > 0) {
				idea.parentVnum = Long.parseLong(pr);
				pr = null;
			}

	      for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
	         XMLElement nxt = (XMLElement) enum.nextElement();
	         String nom = nxt.getName();

				if(nom.equals(PROPERTY_RCPT))
	            idea.rcpt = new FlagList(nxt.getContent());
	         else if(nom.equals(PROPERTY_DATE))
	            idea.date = nxt.getContent();
	         else if(nom.equals(PartiallyTangible.PROPERTY_COMMENT)) {
//	            idea.subject = Util.xml_fromMarkup(nxt.getProperty(PartiallyTangible.PROPERTY_PRETTYNAME));
					idea.subject = nxt.getAttribute(PartiallyTangible.PROPERTY_PRETTYNAME);
//	            idea.text = Util.xml_fromMarkup(nxt.getContent());
					idea.text = nxt.getContent();
	         }
	      }
		}
		else {
			IXMLParser xmp = new StdXMLParser();
			xmp.setBuilder(new StdXMLBuilder());
			xmp.setValidator(new NonValidator());
			xmp.setReader(StdXMLReader.fileReader(src));
//			System.out.println("trying to parse " + src);
			xml = (XMLElement) xmp.parse();
			xmp = null;

			idea = loadInformationTransferEvent(board, xml);
			idea.src = src;
		}

		return idea;
   }

	
	public XMLElement toXMLElement() {
		//idea
		XMLElement xml = new XMLElement();
		xml.setName(MARKUP);
		xml.setAttribute(PROPERTY_FROM, from);
		xml.setAttribute(GuidTracable.PROPERTY_VNUM, "" + cycFort);
		if(parent != null)
			xml.setAttribute(PROPERTY_PARENT, "" + parent.getCycFort());

		XMLElement nxt;

		//idea.rcpt
		nxt = new XMLElement();
		nxt.setName(PROPERTY_RCPT);
		nxt.setContent(rcpt.toString());
		xml.addChild(nxt);

		//idea.date
		nxt = new XMLElement();
		nxt.setName(PROPERTY_DATE);
		nxt.setContent(date);
		xml.addChild(nxt);

		//idea.desc
		nxt = new XMLElement();
		nxt.setName(PartiallyTangible.PROPERTY_COMMENT);
		nxt.setAttribute(PartiallyTangible.PROPERTY_PRETTYNAME, subject);
		nxt.setContent(text);
		xml.addChild(nxt);

		return xml;
	}


	public XMLElement toShortXMLElement() {
		XMLElement xml = new XMLElement();
		xml.setName(MARKUP);
		xml.setAttribute(PROPERTY_SRC, src);
		return xml;
	}

}
