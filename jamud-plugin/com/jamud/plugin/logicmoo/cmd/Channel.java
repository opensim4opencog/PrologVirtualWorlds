package logicmoo.cmd;


import logicmoo.obj.*;
import java.util.*;
import net.n3.nanoxml.*;


public class Channel extends CycThing {

   public static final String MARKUP = "CHANNEL";

   private static final String
		PROPERTY_NAME = "NAME",
      PROPERTY_TRUST = "TRUST",
      PROPERTY_ALT = "ALT",
      PROPERTY_DISPLAY = "DISPLAY",
      PROPERTY_LOCAL = "LOCAL";

   private int trust;
      public int getTrust() { return trust; }
      public void setTrust(int trust) { this.trust = trust; }

   private String alt;
      public String getAlt() { return alt; }
      public void setAlt(String alt) { this.alt = alt; }

   private String display;
      public String getDisplay() { return display; }
      public void setDisplay(String display ) { this.display = display; }

   private String local;
      public String getLocal() { return local; }
      public void setLocal(String local) { this.local = local; }

	private Vector subscribers = new Vector();



	public void subscribe(ChannelListener ch) {
		if(! subscribers.contains(ch))
			subscribers.add(ch);
	}


	public void unsubscribe(ChannelListener ch) {
		subscribers.remove(ch);
	}


	public void channelOut(PlayerMask src, String text) {
      for(Enumeration enum = subscribers.elements(); enum.hasMoreElements(); )
			((ChannelListener) enum.nextElement()).channelOut(this, src, text);
	}


   public static Channel loadChannel(XMLElement xml) throws Exception {
      if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in Channel.loadChannel");

      Channel c = new Channel();

      return c;
   }


	public XMLElement toXMLElement() {
		XMLElement xml = new XMLElement();

		return xml;
	}

}