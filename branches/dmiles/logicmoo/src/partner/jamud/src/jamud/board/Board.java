package jamud.board;


import jamud.util.Attributes;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;
import net.n3.nanoxml.*;


public class Board extends Post {


    /** <code>MARKUP = "BOARD"</code> */
    public static final String
	MARKUP = "BOARD";

    
    private static final String
	PROPERTY_NAME = "NAME",
	PROPERTY_SRC = "SRC",
	PROPERTY_PATH = "PATH",
	PROPERTY_ACL_READ = "READ",
	PROPERTY_ACL_WRITE = "WRITE";

    

    private String name;

    public String getName() {
	return this.name;
    }

    public void setName(String name) {
	this.name = name;
    }


    // minimum trust required to read this board
    private int aclRead;

    public int getAclRead() {
	return this.aclRead;
    }

    public void setAclRead(int aclRead) {
	this.aclRead = aclRead;
    }
    

    // minimum trust required to write to this board
    private int aclWrite;

    public int getAclWrite() {
	return this.aclWrite;
    }

    public void setAclWrite(int aclWrite) {
	this.aclWrite = aclWrite;
    }


    // file this Board is saved to
    private String src;

    public String getSource() {
	return this.src;
    }

    public void setSource(String src) {
	this.src = src;
    }


    // path to save new posts to
    private String path;

    public String getPath() {
	return this.path;
    }

    public void setPath(String path) {
	if(path == null || path.length() == 0) {
	    this.path = "";
	    return;
	} else {
	    if(path.endsWith("\\") ||
	       path.endsWith("/") ) {
		this.path = path;
	    } else {
		this.path = path.concat("/");
	    }
	}
    }

    

    // Default Constructor
    public Board() {
	super();
    }


    
    public synchronized IXMLElement save() throws IOException {
	XMLWriter xmw = new XMLWriter( new FileWriter(src) );
	xmw.write( _save(), false );
	return this.toShortXMLElement();
    }
    

    private IXMLElement _save() throws IOException {
	IXMLElement xml = new XMLElement(MARKUP);
	_save(xml);
	return xml;
    }


    private void _save(IXMLElement xml) throws IOException {
	// board
	xml.setAttribute( PROPERTY_NAME, name );
	xml.setAttribute( PROPERTY_PATH, path );
	xml.setAttribute( PROPERTY_ACL_READ, Integer.toString(aclRead) );
	xml.setAttribute( PROPERTY_ACL_WRITE, Integer.toString(aclWrite) );
	
	// child posts
	for(Iterator i = replies(); i.hasNext(); ) {
	    xml.addChild( ((Post)i.next()).save() );
	}
    }


    
    public static Board loadBoard(IXMLElement xml) throws Exception {
	Board b = new Board();
	b.load(xml);
	return b;
    }


    public synchronized void load(IXMLElement xml) throws Exception {

	String src = xml.getAttribute(PROPERTY_SRC, null);
	if(src == null) {
	    // grab the XML from src file
	    IXMLParser xmp = new StdXMLParser();
	    xmp.setBuilder(new StdXMLBuilder());
	    xmp.setValidator(new NonValidator());
	    xmp.setReader(StdXMLReader.fileReader(src));
	    xml = (IXMLElement) xmp.parse();
	    xmp = null;
	    
	    // remember that we used a file
	    this.src = src;
	}

	// load random attribs
	Attributes attrib = new Attributes(xml.getAttributes());
	this.name = attrib.getAttribute(PROPERTY_NAME, null);
	this.path = attrib.getAttribute(PROPERTY_PATH, null);
	this.aclRead = attrib.getAttribute(PROPERTY_ACL_READ, 0);
	this.aclWrite = attrib.getAttribute(PROPERTY_ACL_WRITE, 1);
	attrib = null;
	
	// load all the children posts
	for(Enumeration enum = xml.getChildrenNamed(Post.MARKUP).elements();
	    enum.hasMoreElements(); ) {

	    addReply( loadPost((IXMLElement)enum.nextElement()) );
	}
    }
    
    
    public synchronized IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }
    

    public synchronized void toXMLElement(IXMLElement xml) {

	// board
	xml.setAttribute( PROPERTY_NAME, name );
	xml.setAttribute( PROPERTY_PATH, path );
	xml.setAttribute( PROPERTY_ACL_READ, Integer.toString(aclRead) );
	xml.setAttribute( PROPERTY_ACL_WRITE, Integer.toString(aclWrite) );
	
	// child posts
	for(Iterator i = replies(); i.hasNext(); ) {
	    Post p = (Post) i.next();
	    xml.addChild( p.toXMLElement() );
	}
    }


    public synchronized IXMLElement toShortXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toShortXMLElement(xml);
	return xml;
    }


    public synchronized void toShortXMLElement(IXMLElement xml) {
	xml.setAttribute(PROPERTY_SRC, this.src);
    }

}
