package logicmoo.obj;


import logicmoo.util.*;
import java.util.*;
import java.io.FileWriter;
import net.n3.nanoxml.*;


public class Board extends CycThing {

	public static final String MARKUP = "BOARD";

	private static final String
		PROPERTY_SRC = "SRC",
		PROPERTY_PATH = "PATH",
		PROPERTY_NAME = "NAME",
		PROPERTY_ACL_READ = "READ",
		PROPERTY_ACL_WRITE = "WRITE";


	private String name;
		public String getName() { return name; }
		public void setName(String name) { this.name = name; }

	private int aclRead;
		public int getAclRead() { return aclRead; }
		public void setAclRead(int aclRead) { this.aclRead = aclRead; }

	private int aclWrite;
		public int getAclWrite() { return aclWrite; }
		public void setAclWrite(int aclWrite) { this.aclWrite = aclWrite; }

	private String src;
		public String getSrc() { return src; }
		public void setSrc(String src) { this.src = src; }

   private String path;
		public String getPath() { return path; }
		public void setPath(String path) {
			if(path == null || path.length() == 0)
				return;
			String e = path.substring(path.length()-1);
			if(e.equals("\\") || e.equals("/"))
				this.path = path;
			else
				this.path = path + "/";
		}

	private Vector internalPosts = new Vector();
		public Post getPost(long cycFort) {
			for(Enumeration enum = internalPosts.elements(); enum.hasMoreElements(); ) {
				Post p = (Post) enum.nextElement();
				if(p.getCycFortString() == cycFort) return p;
			}
			return null;
		}

		public void addPost(Post post) {
			if(getPost(post.getCycFortString()) == null)
				internalPosts.add(post);

			else {
				post.setCycFortString(post.getCycFortString() + 1);
				addPost(post);
			}
		}

		public void removePost(long cycFort) {
			Post post = getPost(cycFort);
			if(post != null)
				removePost(post);
		}

		public void removePost(Post post) {
			internalPosts.remove(post);
			Post prnt = post.getParent();
			if(prnt != null)
				prnt.getReplies().remove(post);
			else
				topPosts.remove(post);

			if(post.getReplies().size() > 0) {
				if(prnt != null)
					for(Enumeration enum = post.getReplies().elements(); enum.hasMoreElements(); ) {
						Post p = (Post) enum.nextElement();
						p.setParent(prnt);
						prnt.getReplies().add(p);
					}
				else
					for(Enumeration enum = post.getReplies().elements(); enum.hasMoreElements(); ) {
						Post p = (Post) enum.nextElement();
						p.setParent(null);
						topPosts.add(p);
					}

			}

			post.setParent(null);
		}


	private Vector topPosts = new Vector();
		public Vector getTopPosts() { return topPosts; }



	public Board() {
		//empty
	}



	public Vector getPosts(Player forWhom) {
		return getPosts(topPosts, forWhom);
	}


	public static Vector getPosts(Vector posts, Player forWhom) {
		Vector ret = new Vector();
		for(Enumeration enum = posts.elements(); enum.hasMoreElements(); ) {
			Post p = (Post) enum.nextElement();
         if(p.getRcpt().contains("*") ||						// we check the most likely events
            p.getRcpt().contains("all") ||					// first, the least likely (and those
				p.getFrom().equals(forWhom.getName()) ||		// that take more processing) last
            p.getRcpt().contains(forWhom.getName())) ret.add(p);
		}
		return ret;
	}


	public void constructTree() {
		topPosts.clear();
		for(Enumeration enum = internalPosts.elements(); enum.hasMoreElements(); ) {
			Post post = (Post) enum.nextElement();

			if(post.getParentCycFort() > 0) {
				Post prnt = getPost(post.getParentCycFort());
				if(prnt != null) {
					post.setParent(prnt);
					prnt.getReplies().add(post);
				}
				else
					topPosts.add(post);
			}
			else
				topPosts.add(post);
		}
	}


	public void save() {
		try {
			XMLWriter xmw = new XMLWriter(new FileWriter(src));
			xmw.write(toXMLElement());
		}
		catch(Exception e) {
			System.err.println("Exception in Board.save()");
			System.err.println(e);
		}
	}


	public static Board loadBoard(XMLElement xml) throws Exception {
		if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in Board.loadBoard");

		String src = xml.getAttribute(PROPERTY_SRC, null);

		if(src == null) {
			Board board = new Board();
			board.path = xml.getAttribute(PROPERTY_PATH);
			board.setName(xml.getAttribute(CycThing.PROPERTY_NAME));
			board.aclRead = xml.getAttribute(PROPERTY_ACL_READ, 0);
			board.aclWrite = xml.getAttribute(PROPERTY_ACL_WRITE, 1);

	      for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); )
				board.addPost(Post.loadPost(board, (XMLElement) enum.nextElement()));

			board.constructTree();
			return board;
		}
		else {
			IXMLParser xmp = new StdXMLParser();
			xmp.setBuilder(new StdXMLBuilder());
			xmp.setValidator(new NonValidator());
			xmp.setReader(StdXMLReader.fileReader(src));
			xml = (XMLElement) xmp.parse();
			xmp = null;

			Board board = loadBoard(xml);
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

		//board.posts
		for(Enumeration enum = internalPosts.elements(); enum.hasMoreElements(); )
			xml.addChild( ((Post)enum.nextElement()).toShortXMLElement() );

		return xml;
	}

}