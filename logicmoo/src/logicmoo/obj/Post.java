package logicmoo.obj;


import logicmoo.*;
import logicmoo.util.*;
import java.io.FileWriter;
import java.util.*;
import net.n3.nanoxml.*;


public class Post {

    public static final String MARKUP = "POST";

    private static final String
    PROPERTY_SRC = "SRC",
    PROPERTY_PARENT = "PARENT",
    PROPERTY_FROM = "FROM",
    PROPERTY_RCPT = "RCPT",
    PROPERTY_DATE = "DATE";


    private Board board = null;
    public Board getBoard() { return board;}
    public void setBoard(Board board) { this.board = board;}

    private String src = "";
    public String getSrc() { return src;}
    public void setSrc(String src) { this.src = src;}

    private Post parent = null;
    public Post getParent() { return parent;}
    public void setParent(Post parent) { this.parent = parent;}

    private long parentCycFort = (long) -1;
    public long getParentCycFort() { return parentCycFort;}
    public void setParentCycFort(long cycFort) { this.parentCycFort = cycFort;}

    private Vector replies = new Vector();
    public Vector getReplies() { return replies;}

    private FlagList rcpt = new FlagList();
    public FlagList getRcpt() { return rcpt;}
    public void setRcpt(FlagList rcpt) { this.rcpt = rcpt;}

    private String from = "";
    public String getFrom() { return from;}
    public void setFrom(String from) { this.from = from;}

    private String subject = "";
    public String getSubject() { return subject;}
    public void setSubject(String subject) { this.subject = subject;}

    private String text = "";
    public String getText() { return text;}
    public void setText(String text) { this.text = text;}

    private long cycFort;
    public long getCycFortString() { return cycFort;}
    public void setCycFortString(long cycFort) { this.cycFort = cycFort;}

    private String date;
    public String getDate() { return date;}
    public void setDate(String date) { this.date = date;}

    private boolean posted = false;
    public boolean getPosted() { return posted;}
    public void setPosted(boolean posted) { this.posted = posted;}



    public Post() {
	cycFort = System.currentTimeMillis();
	date = (new java.util.Date(cycFort)).toString();
    }

    public Post(Board board) {
	this();
	this.board = board;
    }



    public void doPosting() {
	board.addPost(this);

	if ( parent != null )
	    parent.getReplies().add(this);
	else
	    board.getTopPosts().add(this);

	setSrc(board.getPath() + cycFort);

	save();
	board.save();

	posted = true;
    }


    public String toString() {
	return toString(null);
    }


    public String toString(String ident) {
	if ( ident == null )
	    ident = "";

	StringBuffer sb = new StringBuffer("{3[{6" + board.getName() + " " + ident);

	if ( replies.size() > 0 )
	    sb.append( ",  " + replies.size() + " Repl" + ((replies.size() == 1) ? "y" : "ies") );
	sb.append( "{3]{0\n" );

	sb.append( "{6From:{0 " + from + ", " + date + "\n" );

	String o = Util.replace(rcpt.toString(), ";", ", ");
	if ( o.length() > 1 )
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
	} catch ( Exception e ) {
	    System.err.println("Exception in Post.save()");
	    e.printStackTrace();
	    return false;
	}
    }


    public static Post loadPost(Board board, XMLElement xml) throws Exception {
	if ( ! xml.getName().equals(MARKUP) )
	    throw new Exception("non-" + MARKUP + " element in Post.loadPost");

	Post post;

	String src = xml.getAttribute(PROPERTY_SRC, null);
	if ( src == null ) {
	    post = new Post();
	    post.posted = true;
	    post.board = board;
	    post.from = xml.getAttribute(PROPERTY_FROM);
	    post.cycFort = Long.parseLong(xml.getAttribute(GuidEnabled.PROPERTY_VNUM));

	    String pr = (xml.getAttribute(PROPERTY_PARENT, null));
	    if ( pr != null && pr.length() > 0 ) {
		post.parentCycFort = Long.parseLong(pr);
		pr = null;
	    }

	    for ( Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
		XMLElement nxt = (XMLElement) enum.nextElement();
		String nom = nxt.getName(); nxt.getContent();

		if ( nom.equals(PROPERTY_RCPT) )
		    post.rcpt = new FlagList(nxt.getContent());
		else if ( nom.equals(PROPERTY_DATE) )
		    post.date = nxt.getContent();
		else if ( nom.equals(PartiallyTangible.PROPERTY_DESC) ) {
		    //	            post.subject = Util.xml_fromMarkup(nxt.getProperty(PartiallyTangible.PROPERTY_SHORTDESC));
		    post.subject = nxt.getAttribute(PartiallyTangible.PROPERTY_SHORTDESC);
		    //	            post.text = Util.xml_fromMarkup(nxt.getContent());
		    post.text = nxt.getContent();
		}
	    }
	} else {
	    IXMLParser xmp = new StdXMLParser();
	    xmp.setBuilder(new StdXMLBuilder());
	    xmp.setValidator(new NonValidator());
	    xmp.setReader(StdXMLReader.fileReader(src));
	    //			System.out.println("trying to parse " + src);
	    xml = (XMLElement) xmp.parse();
	    xmp = null;

	    post = loadPost(board, xml);
	    post.src = src;
	}

	return post;
    }


    public XMLElement toXMLElement() {
	//post
	XMLElement xml = new XMLElement();
	xml.setName(MARKUP);
	xml.setAttribute(PROPERTY_FROM, from);
	xml.setAttribute(GuidEnabled.PROPERTY_VNUM, "" + cycFort);
	if ( parent != null )
	    xml.setAttribute(PROPERTY_PARENT, "" + parent.getCycFortString());

	XMLElement nxt;

	//post.rcpt
	nxt = new XMLElement();
	nxt.setName(PROPERTY_RCPT);
	nxt.setContent(rcpt.toString());
	xml.addChild(nxt);

	//post.date
	nxt = new XMLElement();
	nxt.setName(PROPERTY_DATE);
	nxt.setContent(date);
	xml.addChild(nxt);

	//post.desc
	nxt = new XMLElement();
	nxt.setName(PartiallyTangible.PROPERTY_DESC);
	nxt.setAttribute(PartiallyTangible.PROPERTY_SHORTDESC, subject);
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
