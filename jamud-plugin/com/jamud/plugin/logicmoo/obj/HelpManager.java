package  logicmoo.obj;


import java.util.*;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public class HelpManager {

	public static final String MARKUP = "HELPMANAGER";

	private Vector contents = new Vector();
		public Vector getHelp() { return contents; }



	public HelpManager(XMLElement xml) throws Exception {
		if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in HelpManager constructor");

		for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); )
			contents.add(Help.loadHelp((XMLElement) enum.nextElement()));
	}



	public Help getTopic(String topic, int trust) {
		topic = topic.toLowerCase();
		for(Enumeration enum = contents.elements(); enum.hasMoreElements(); ) {
			Help h = (Help) enum.nextElement();
			if(h.getTrust() <= trust)
				for(Enumeration kw = h.getKeywords().enumerate(); kw.hasMoreElements(); )
					if(((String) kw.nextElement()).toLowerCase().startsWith(topic))
						return h;
		}
		return null;
	}

}