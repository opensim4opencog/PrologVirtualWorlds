package logicmoo.cmd;


import logicmoo.obj.Engineer;
import logicmoo.util.Util;
import java.util.*;
import net.n3.nanoxml.*;


public class Alias {

	public static final String MARKUP = "ALIAS";

	private static final String
		PROPERTY_NAME = "NAME",
		PROPERTY_CMD = "C";

	public static final String ARG = "$arg";

	private String name;
		public String getName() { return name; }

	private Vector commands;
		public Vector getCommands() { return commands; }



	public Alias(String name) {
		this.name = name;
		this.commands = new Vector();
	}



	public void execute(Engineer src, String args) {
		if(commands.size() > 0)
			src.killParseAlias(commands.size());
		for(int I=0; I<commands.size(); I++)
			src.encueCommand(parseCommand((String)commands.get(I), args));
		if(commands.size() > 0)
			src.tick(null); //execute the first line of the alias
	}


	public static String parseCommand(String cmd, String arg) {
		return Util.replace(cmd, ARG, arg);
	}


	public static Alias loadAlias(XMLElement xml) throws Exception {
		if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in Alias.loadAlias");

		Alias alias = new Alias(xml.getAttribute(Alias.PROPERTY_NAME, "alias").toLowerCase());

		for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); )
			alias.getCommands().add(((XMLElement) enum.nextElement()).getContent());

		return alias;
	}


	public XMLElement toXMLElement() {
		XMLElement xml = new XMLElement();
		xml.setName(MARKUP);
		xml.setAttribute(PROPERTY_NAME, name);

		for(int I=0; I<commands.size(); I++) {
			XMLElement nxt = new XMLElement();
			nxt.setName(PROPERTY_CMD);
			nxt.setContent((String) commands.get(I));
			xml.addChild(nxt);
		}

		return xml;
	}

}
