package  logicmoo.obj;


import logicmoo.util.*;
import java.util.*;
import java.io.FileWriter;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public class Conversation extends CycThing {

    public static final String MARKUP = "CONVERSATION";

    private static final String
    PROPERTY_SRC = "SRC",
    PROPERTY_PATH = "PATH",
    PROPERTY_NAME = "NAME",
    PROPERTY_ACL_READ = "READ",
    PROPERTY_ACL_WRITE = "WRITE";


    private String name;
    public String getName() { return name;}
    public void setName(String name) { this.name = name;}

    private int aclRead;
    public int getAclRead() { return aclRead;}
    public void setAclRead(int aclRead) { this.aclRead = aclRead;}

    private int aclWrite;
    public int getAclWrite() { return aclWrite;}
    public void setAclWrite(int aclWrite) { this.aclWrite = aclWrite;}

    private String src;
    public String getSrc() { return src;}
    public void setSrc(String src) { this.src = src;}

    private String path;
    public String getPath() { return path;}
    public void setPath(String path) {
	if ( path == null || path.length() == 0 )
	    return;
	String e = path.substring(path.length()-1);
	if ( e.equals("\\") || e.equals("/") )
	    this.path = path;
	else
	    this.path = path + "/";
    }

    private Vector internalInformationTransferEvents = new Vector();
    public InformationTransferEvent getInformationTransferEvent(long cycFort) {
	for ( Enumeration enum = internalInformationTransferEvents.elements(); enum.hasMoreElements(); ) {
	    InformationTransferEvent p = (InformationTransferEvent) enum.nextElement();
	    if ( p.getCycFort() == cycFort ) return p;
	}
	return null;
    }

    public void addInformationTransferEvent(InformationTransferEvent idea) {
	if ( getInformationTransferEvent(idea.getCycFort()) == null )
	    internalInformationTransferEvents.add(idea);

	else {
	    idea.setCycFort(idea.getCycFort() + 1);
	    addInformationTransferEvent(idea);
	}
    }

    public void removeInformationTransferEvent(long cycFort) {
	InformationTransferEvent idea = getInformationTransferEvent(cycFort);
	if ( idea != null )
	    removeInformationTransferEvent(idea);
    }

    public void removeInformationTransferEvent(InformationTransferEvent idea) {
	internalInformationTransferEvents.remove(idea);
	InformationTransferEvent prnt = idea.getParent();
	if ( prnt != null )
	    prnt.getReplies().remove(idea);
	else
	    topInformationTransferEvents.remove(idea);

	if ( idea.getReplies().size() > 0 ) {
	    if ( prnt != null )
		for ( Enumeration enum = idea.getReplies().elements(); enum.hasMoreElements(); ) {
		    InformationTransferEvent p = (InformationTransferEvent) enum.nextElement();
		    p.setParent(prnt);
		    prnt.getReplies().add(p);
		} else
		for ( Enumeration enum = idea.getReplies().elements(); enum.hasMoreElements(); ) {
		    InformationTransferEvent p = (InformationTransferEvent) enum.nextElement();
		    p.setParent(null);
		    topInformationTransferEvents.add(p);
		}

	}

	idea.setParent(null);
    }


    private Vector topInformationTransferEvents = new Vector();
    public Vector getTopInformationTransferEvents() { return topInformationTransferEvents;}



    public Conversation() {
	//empty
    }



    public Vector getInformationTransferEvents(Engineer forWhom) {
	return getInformationTransferEvents(topInformationTransferEvents, forWhom);
    }


    public static Vector getInformationTransferEvents(Vector ideas, Engineer forWhom) {
	Vector ret = new Vector();
	for ( Enumeration enum = ideas.elements(); enum.hasMoreElements(); ) {
	    InformationTransferEvent p = (InformationTransferEvent) enum.nextElement();
	    if ( p.getRcpt().contains("*") ||	 // we check the most likely events
		 p.getRcpt().contains("all") ||	   // first, the least likely (and those
		 p.getFrom().equals(forWhom.getName()) ||    // that take more processing) last
		 p.getRcpt().contains(forWhom.getName()) ) ret.add(p);
	}
	return ret;
    }


    public void constructTree() {
	topInformationTransferEvents.clear();
	for ( Enumeration enum = internalInformationTransferEvents.elements(); enum.hasMoreElements(); ) {
	    InformationTransferEvent idea = (InformationTransferEvent) enum.nextElement();

	    if ( idea.getParentVnum() > 0 ) {
		InformationTransferEvent prnt = getInformationTransferEvent(idea.getParentVnum());
		if ( prnt != null ) {
		    idea.setParent(prnt);
		    prnt.getReplies().add(idea);
		} else
		    topInformationTransferEvents.add(idea);
	    } else
		topInformationTransferEvents.add(idea);
	}
    }


    public void save() {
	try {
	    XMLWriter xmw = new XMLWriter(new FileWriter(src));
	    xmw.write(toXMLElement());
	} catch ( Exception e ) {
	    System.err.println("Exception in Conversation.save()");
	    System.err.println(e);
	}
    }


    public static Conversation loadConversation(XMLElement xml) throws Exception {
	if ( ! xml.getName().equals(MARKUP) )
	    throw new Exception("non-" + MARKUP + " element in Conversation.loadConversation");

	String src = xml.getAttribute(PROPERTY_SRC, null);

	if ( src == null ) {
	    Conversation board = new Conversation();
	    board.path = xml.getAttribute(PROPERTY_PATH);
	    board.setName(xml.getAttribute(CycThing.PROPERTY_NAME));
	    board.aclRead = xml.getAttribute(PROPERTY_ACL_READ, 0);
	    board.aclWrite = xml.getAttribute(PROPERTY_ACL_WRITE, 1);

	    for ( Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); )
		board.addInformationTransferEvent(InformationTransferEvent.loadInformationTransferEvent(board, (XMLElement) enum.nextElement()));

	    board.constructTree();
	    return board;
	} else {
	    IXMLParser xmp = new StdXMLParser();
	    xmp.setBuilder(new StdXMLBuilder());
	    xmp.setValidator(new NonValidator());
	    xmp.setReader(StdXMLReader.fileReader(src));
	    xml = (XMLElement) xmp.parse();
	    xmp = null;

	    Conversation board = loadConversation(xml);
	    board.src = src;

	    return board;
	}
    }


    public XMLElement toXMLElement() {
	//board
	XMLElement xml = new XMLElement();
	xml.setName(MARKUP);
	xml.setAttribute(CycThing.PROPERTY_NAME, getName());
	xml.setAttribute(PROPERTY_PATH, path);
	xml.setAttribute(PROPERTY_ACL_READ, ""+aclRead);
	xml.setAttribute(PROPERTY_ACL_WRITE, ""+aclWrite);

	//board.ideas
	for ( Enumeration enum = internalInformationTransferEvents.elements(); enum.hasMoreElements(); )
	    xml.addChild( ((InformationTransferEvent)enum.nextElement()).toShortXMLElement() );

	return xml;
    }

}