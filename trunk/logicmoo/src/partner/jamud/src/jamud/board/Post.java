package jamud.board;


import jamud.util.FlagList;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.util.*;
import net.n3.nanoxml.*;


public class Post {


    // The static PID counter
    private static long pid_counter = 0;

    public synchronized static final long nextPostId() {
	return ++pid_counter;
    }

    private static final HashMap pid_hash = new HashMap();

    public final Post getPostByPostId(long id) {
	synchronized( pid_hash ) {
	    return (Post) pid_hash.get( new Long(id) );
	}
    }



    /** <code>MARKUP = "POST"</code> */
    public static final String MARKUP = "POST";
    

    private static final String
	PROPERTY_TEXT = "DESC",
	PROPERTY_SUBJECT = "SHORT",
	PROPERTY_SRC = "SRC",
	PROPERTY_FROM = "FROM",
	PROPERTY_RCPT = "RCPT",
	PROPERTY_DATE = "DATE";
    
    
    // Board this Post belongs to (not saved, just there to
    // make it easier to compose)
    public Board board;


    // for deletions we can restructure the tree.
    // a node in the tree only guarantees the existence of
    // its children. Removing a parent will eventually remove
    // all the children. Children cannot accidentally keep
    // the parent from being gc'd
    private WeakReference parent;

    public Post getParent() {
	return (Post) this.parent.get();
    }


    public boolean isSaved = false;


    private String src;

    public String getSource() {
	return this.src;
    }


    // My PostID
    private long pid;

    public long getPostId() {
	return pid;
    }


    // Intended Recipients of this Post
    private FlagList rcpt;

    public FlagList getRecipientList() {
	return this.rcpt;
    }


    // Author of this Post
    private String from;

    public String getFrom() {
	return this.from;
    }


    // Date composed (String representation, not used to sort)
    private String date;

    public String getDate() {
	return this.date;
    }

    public void setDate(String date) {
	this.date = date;
    }
    

    private String subject;

    public String getSubject() {
	return this.subject;
    }

    public void setSubject(String subject) {
	this.subject = subject;
    }


    private String text;

    public String getText() {
	return this.text;
    }

    public void setText(String text) {
	this.text = text;
    }


    // Replies
    private HashSet replies;

    public synchronized Iterator replies() {
	return replies.iterator();
    }

    public synchronized void addReply(Post reply) {
	if( replies.add( reply ) ) {
	    reply.parent = new WeakReference(this);
	}
    }

    public synchronized void removeReply(Post reply) {
	if( replies.remove(reply) ) {
	    reply.delete();
	}
    }

    public synchronized void removeReply(long pid) {
	for( Iterator i = replies.iterator(); i.hasNext(); ) {
	    Post reply = (Post) i.next();
	    if( reply.getPostId() == pid ) {
		i.remove();
		reply.delete();
	    }
	}
    }

    public synchronized Post getReply(long pid) {
	for( Iterator i = replies.iterator(); i.hasNext(); ) {
	    Post p = (Post) i.next();
	    if( p.getPostId() == pid ) {
		return p;
	    }
	}
	return null;
    }



    // constructors
    public Post(Board board) {
	this();

	this.board = board;
	this.date = (new java.util.Date()).toString();
    }

    public Post() {
	this.pid = nextPostId();
	synchronized( pid_hash ) {
	    pid_hash.put( new Long(this.pid), this );
	}

	this.rcpt = new FlagList();
	this.replies = new HashSet();
    }
    
    
    
    public synchronized IXMLElement save() throws IOException {
	IXMLElement xml = new XMLElement(MARKUP);
	save(xml);
	return xml;
    }
    

    public synchronized void save(IXMLElement xml) throws IOException {

	if(this.src == null) {
	    this.src = Long.toHexString(this.pid);
	}

	if( !this.isSaved ) {
	    IXMLElement nxt = _save();
	    XMLWriter xmw = new XMLWriter( new FileWriter(this.src) );
	    xmw.write( nxt, false );
	    this.isSaved = true;
	}

	toShortXMLElement( xml );
    }


    private IXMLElement _save() throws IOException {
	IXMLElement xml = new XMLElement(MARKUP);
	_save(xml);
	return xml;
    }


    private void _save(IXMLElement xml) throws IOException {
	//post.from
	xml.setAttribute(PROPERTY_FROM, from);
	
	IXMLElement nxt;
	
	//post.rcpt
	nxt = new XMLElement(PROPERTY_RCPT);
	nxt.setContent(rcpt.toString());
	xml.addChild(nxt);
	
	//post.date
	nxt = new XMLElement(PROPERTY_DATE);
	nxt.setContent(date);
	xml.addChild(nxt);
	
	//post.desc
	nxt = new XMLElement(PROPERTY_TEXT);
	nxt.setAttribute(PROPERTY_SUBJECT, subject);
	nxt.setContent(text);
	xml.addChild(nxt);

	//replies
	for(Iterator i = replies.iterator(); i.hasNext(); ) {
	    Post p = (Post) i.next();
	    xml.addChild( p.save() );
	}
    }

    
    public static Post loadPost(IXMLElement xml) throws Exception {
	Post p = new Post();
	p.load(xml);
	return p;
    }

    
    public synchronized void load(IXMLElement xml) throws Exception {
	
	String src = xml.getAttribute(PROPERTY_SRC, null);

	if(src != null) {
	    IXMLParser xmp = new StdXMLParser();
	    xmp.setBuilder(new StdXMLBuilder());
	    xmp.setValidator(new NonValidator());
	    xmp.setReader(StdXMLReader.fileReader(src));

	    xml = (XMLElement) xmp.parse();
	    xmp = null;
	    
	    this.src = src;	    
	}

	// Random stuff
	this.isSaved = false;
	this.from = xml.getAttribute(PROPERTY_FROM, "");
	
	IXMLElement nxt;

	// subject and text
	nxt = xml.getFirstChildNamed(PROPERTY_TEXT);
	if(nxt != null) {
	    this.subject = nxt.getAttribute(PROPERTY_SUBJECT, null);
	    this.text = nxt.getContent();
	}

	// Recipient List
	nxt = xml.getFirstChildNamed(PROPERTY_RCPT);
	if(nxt != null)
	    this.rcpt.refresh( nxt.getContent() );
	
	// Date String
	nxt = xml.getFirstChildNamed(PROPERTY_DATE);
	if(nxt != null)
	    this.date = nxt.getContent();

	// Replies
	for(Enumeration enum = xml.getChildrenNamed(MARKUP).elements();
	    enum.hasMoreElements(); ) {

	    nxt = (XMLElement) enum.nextElement();
	    replies.add( loadPost(nxt) );	    
	}

    }
    
    
    public synchronized IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }
    

    public synchronized void toXMLElement(IXMLElement xml) {

	//post.from
	xml.setAttribute(PROPERTY_FROM, from);
	
	IXMLElement nxt;
	
	//post.rcpt
	nxt = new XMLElement(PROPERTY_RCPT);
	nxt.setContent(rcpt.toString());
	xml.addChild(nxt);
	
	//post.date
	nxt = new XMLElement(PROPERTY_DATE);
	nxt.setContent(date);
	xml.addChild(nxt);
	
	//post.desc
	nxt = new XMLElement(PROPERTY_TEXT);
	nxt.setAttribute(PROPERTY_SUBJECT, subject);
	nxt.setContent(text);
	xml.addChild(nxt);

	//replies
	for(Iterator i = replies.iterator(); i.hasNext(); ) {
	    Post p = (Post) i.next();
	    xml.addChild( p.toXMLElement() );
	}
    }

   
    public IXMLElement toShortXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toShortXMLElement(xml);
	return xml;
    }


    public void toShortXMLElement(IXMLElement xml) {
	xml.setAttribute( PROPERTY_SRC, getSource() );
    }
    

    public void delete() {
	delete(false);
    }


    public synchronized void delete(boolean saveReplies) {
	Post p = (Post) this.parent.get();
	synchronized( p ) {

	    if( saveReplies ) {
		for(Iterator i = this.replies.iterator(); i.hasNext(); ) {
		    p.addReply( (Post)i.next() );
		}
	    }

	    p.removeReply(this);
	}

	this.finalize();
    }


    public final boolean equals(Object o) {
	return (this == o);
    }


    public void finalize() {
	this.parent.clear();
	this.parent = null;
	this.replies.clear();
	this.replies = null;
	this.rcpt.clear();
	this.rcpt = null;
    }

}
