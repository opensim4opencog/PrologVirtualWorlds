package logicmoo.obj;


import logicmoo.util.*;
import java.util.Enumeration;
import net.n3.nanoxml.*;


public class Help {

   public static final String MARKUP = "HELP";

   private static final String
      PROPERTY_KEYWORDS = "KEYWORDS",
      PROPERTY_AUTHOR = "AUTHOR",
      PROPERTY_SHORT = "SHORT",
      PROPERTY_DESC = "DESC",
      PROPERTY_TRUST = "TRUST",
      PROPERTY_SRC = "SRC";

   private int trust;
      public int getTrust() { return trust; }
      public void setTrust(int trust) { this.trust = trust; }

   private FlagList keywords;
      public FlagList getKeywords() { return keywords; }
      public void setKeywords(FlagList keywords) { this.keywords = keywords; }

   private String author;
      public String getAuthor() { return author; }
      public void setAuthor(String author) { this.author = author; }

   private String title;
      public String getTitle() { return title; }
      public void setTitle(String title) { this.title = title; }

   private String body;
      public String getBody() { return body; }
      public void setBody(String body) { this.body = body; }


   public String toString() {
   // String l = "--------------------------------------------------------------------------------";
   // return l + title + "\n[by " + author + "]\n" + body + "\n" + l;
      return title + "\n[by " + author + "]\n" + body;
   }


   public static Help loadHelp(XMLElement xml) throws Exception {
      if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in Help.loadHelp");

      Help h;

      String src = xml.getAttribute(PROPERTY_SRC, null);
      if(src == null) {
         h = new Help();
         h.trust = xml.getAttribute(PROPERTY_TRUST, 0);
         h.author = xml.getAttribute(PROPERTY_AUTHOR, "daemonnix");

         for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
            XMLElement nxt = (XMLElement) enum.nextElement();
            String nom = nxt.getName(); nxt.getContent();
            if(nom.equals(PROPERTY_KEYWORDS))
               h.keywords = new FlagList(nxt.getContent());
            else if(nom.equals(PROPERTY_DESC)) {
               h.title = nxt.getAttribute(PROPERTY_SHORT);
               h.body = nxt.getContent();
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

         h = Help.loadHelp(xml);
      }

      return h;
   }


	public XMLElement toXMLElement() {
		XMLElement xml = new XMLElement();
		xml.setName(MARKUP);

		return xml;
	}

}