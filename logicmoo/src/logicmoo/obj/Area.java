package logicmoo.obj;


import logicmoo.*;
import logicmoo.obj.event.*;
import logicmoo.util.*;
import java.io.FileWriter;
import java.util.*;
import net.n3.nanoxml.*;


public class Area extends CycThing implements JamudEventTrigger {

	private static final String MARKUP = "AREA";

	private static final String
		PROPERTY_SRC = "SRC",
		PROPERTY_PREFIX = "PREFIX",
		PROPERTY_AUTHOR = "AUTHOR",
		PROPERTY_ITEMS = "ITEMS",
		PROPERTY_BODIES = "BODIES",
		PROPERTY_RESETS = "RESETS",
		PROPERTY_ROOMS = "ROOMS";

	private String src;
		public String getSrc() { return src; }
		public void setSrc(String src) { this.src = src; }

   private String prefix="midga";
      public String getPrefix() { return prefix; }
      public void setPrefix(String prefix) { this.prefix = prefix; }

   private String author;
      public String getAuthor() { return author; }
      public void setAuthor(String author) { this.author = author; }

   protected Hashtable items;
      public Hashtable getItems() { return items; }

   protected Hashtable bodies;
      public Hashtable getBodies() { return bodies; }

   protected Hashtable resets;
      public Hashtable getResets() { return resets; }

   protected Hashtable rooms;
      public Hashtable getRooms() { return rooms; }



	protected Area() {
		items = new Hashtable();
		bodies = new Hashtable();
		resets = new Hashtable();
		rooms = new Hashtable();
	}



   public void println(String text) {
      for(Enumeration rms = rooms.elements(); rms.hasMoreElements(); )
         ((Room) rms.nextElement()).println(text);
   }


   public void println(Body from, String f_text, String text) {
      for(Enumeration enum = rooms.elements(); enum.hasMoreElements(); )
			((Room) enum.nextElement()).println(from, f_text, text);
   }


	public void tick() {
		//apply resets
   	for(Enumeration enum = resets.elements(); enum.hasMoreElements(); )
         ((Reset) enum.nextElement()).apply();

		//tick each room
		for(Enumeration enum = rooms.elements(); enum.hasMoreElements(); )
			((Room) enum.nextElement()).tick();
	}


	public synchronized boolean save() {
      try {
			XMLWriter xmw = new XMLWriter(new FileWriter(src));
			xmw.write(toXMLElement());
         return true;
      }
      catch(Exception e) {
         System.err.println("Exception while saving Area " + prefix);
			e.printStackTrace();
         return false;
      }
	}


	public static Area loadArea(XMLElement xml) throws Exception {
		if(! xml.getName().equals(Area.MARKUP))
			throw new Exception("non-" + MARKUP + " element in Area.loadArea");

		String src = xml.getAttribute(PROPERTY_SRC, null);
		if(src == null) {
	      Area area = new Area();

	      area.setName(xml.getAttribute(CycThing.PROPERTY_NAME));
	      area.prefix = xml.getAttribute(Area.PROPERTY_PREFIX);
	      area.author = xml.getAttribute(Area.PROPERTY_AUTHOR);

	      for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); ) {
	         XMLElement nxt = (XMLElement) enum.nextElement();
	         String nom = nxt.getName(); nxt.getContent();
	
   	      if(nom.equals(PROPERTY_ITEMS)) {
			      for( Enumeration nxtEnum = nxt.enumerateChildren(); nxtEnum.hasMoreElements(); ) {
						Item item = Item.loadItem((XMLElement) nxtEnum.nextElement());
						//item.setArea(area.prefix);
	         		area.items.put(item.getCycFortString(), item);
	      		}
	         }
	         else if(nom.equals(PROPERTY_BODIES)) {
			      for( Enumeration nxtEnum = nxt.enumerateChildren(); nxtEnum.hasMoreElements(); ) {
						Body body = Body.loadBody((XMLElement) nxtEnum.nextElement());
						//body.setArea(area.prefix);
	         		area.bodies.put(body.getCycFortString(), body);
	      		}
	         }
	         else if(nom.equals(PROPERTY_RESETS)) {
			      for( Enumeration nxtEnum = nxt.enumerateChildren(); nxtEnum.hasMoreElements(); ) {
						Reset reset = Reset.loadReset(area, (XMLElement) nxtEnum.nextElement());
			         area.resets.put(reset.getCycFortString(), reset);
			      }
	         }
	         else if(nom.equals(PROPERTY_ROOMS)) {
			      for( Enumeration nxtEnum = nxt.enumerateChildren(); nxtEnum.hasMoreElements(); ) {
						Room room = Room.loadRoom(area, (XMLElement) nxtEnum.nextElement());
			         area.rooms.put(room.getCycFortString(), room);
			      }
	         }
	      }
			return area;
		}
		else {
			IXMLParser xmp = new StdXMLParser();
			xmp.setBuilder(new StdXMLBuilder());
			xmp.setValidator(new NonValidator());
			xmp.setReader(StdXMLReader.fileReader(src));
			xml = (XMLElement) xmp.parse();
			xmp = null;

			Area area = Area.loadArea(xml);
			area.src = src;

	      return area;
		}
	}


	public XMLElement toXMLElement() {
		XMLElement xml = new XMLElement();

		//area
		xml.setName(MARKUP);
		xml.setAttribute(CycThing.PROPERTY_NAME, getName());
		xml.setAttribute(PROPERTY_PREFIX, prefix);
		xml.setAttribute(PROPERTY_AUTHOR, author);
		if(src != null)
			xml.setAttribute(PROPERTY_SRC, src);

		XMLElement nxt;

		//area.items
		nxt = new XMLElement();
		nxt.setName(PROPERTY_ITEMS);
		for(Enumeration enum = items.elements(); enum.hasMoreElements(); )
			nxt.addChild( ((Item)enum.nextElement()).toXMLElement() );
		xml.addChild(nxt);

		//area.bodies
		nxt = new XMLElement();
		nxt.setName(PROPERTY_BODIES);
		for(Enumeration enum = bodies.elements(); enum.hasMoreElements(); )
			nxt.addChild( ((Body)enum.nextElement()).toXMLElement() );
		xml.addChild(nxt);

		//area.resets
		nxt = new XMLElement();
		nxt.setName(PROPERTY_RESETS);
		for(Enumeration enum = resets.elements(); enum.hasMoreElements(); )
			nxt.addChild( ((Body)enum.nextElement()).toXMLElement() );
		xml.addChild(nxt);

		//area.rooms
		nxt = new XMLElement();
		nxt.setName(PROPERTY_ROOMS);
		for(Enumeration enum = rooms.elements(); enum.hasMoreElements(); )
			nxt.addChild( ((Body)enum.nextElement()).toXMLElement() );
		xml.addChild(nxt);

		return xml;
	}

}