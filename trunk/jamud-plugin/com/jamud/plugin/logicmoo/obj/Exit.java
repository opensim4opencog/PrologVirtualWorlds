package  logicmoo.obj;


import logicmoo.obj.event.*;
import logicmoo.util.Util;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public class Exit extends PartiallyTangible implements JamudEventTrigger {

	public static final String MARKUP = "EXIT";

	private static final String
		PROPERTY_TARGET = "TARGET",
		PROPERTY_KEY = "KEY",
		PROPERTY_TYPE = "TYPE",
		PROPERTY_STATE = "STATE",
		PROPERTY_IN_MESSAGE = "IN_MSG",
		PROPERTY_OUT_MESSAGE = "OUT_MSG";

   public static final String
		DIR_NORTH = "NORTH",
		DIR_SOUTH = "SOUTH",
		DIR_EAST = "EAST",
		DIR_WEST = "WEST",
		DIR_UP = "UP",
		DIR_DOWN = "DOWN";

	public static final int
		BASE_DIR_NORTH = 0,
		BASE_DIR_SOUTH = 1,
		BASE_DIR_EAST = 2,
		BASE_DIR_WEST = 3,
		BASE_DIR_UP = 4,
		BASE_DIR_DOWN = 5;

	public static final String[] BASE_DIR = {
		DIR_NORTH,
		DIR_SOUTH,
		DIR_EAST,
		DIR_WEST,
		DIR_UP,
		DIR_DOWN };

   public static final String
		TYPE_OPEN = "OPEN",
      TYPE_DOOR = "DOOR",
      TYPE_LOCK = "LOCK";

   public static final int
		STATE_OPEN = 0,
      STATE_CLOSED = 1,
      STATE_LOCKED = 2,
		STATE_BLOCKED = 3;

   private CycFort targetCycFort;
      public CycFort getTargetCycFort() { return targetCycFort; }
      public void setTargetVnum(CycFort targetCycFort) { this.targetCycFort = targetCycFort; }

   private GeographicalRegion targetGeographicalRegion;
      public GeographicalRegion getTargetGeographicalRegion() { return targetGeographicalRegion; }
      public void setTargetGeographicalRegion(GeographicalRegion targetGeographicalRegion) { this.targetGeographicalRegion = targetGeographicalRegion; }

   private String type;
      public String getType() { return type; }
      public void setType(String type) { this.type = type; }
		public boolean isType(String type) { return this.type.equals(type); }

   private int state;
      public int getState() { return state; }
      public void setState(int state) { this.state = state; }
		public boolean isState(int which) { return ( (state & which) == which ); }

   private String key;
      public String getKey() { return key; }
      public void setKey(String key) { this.key = key; }

	private String in_msg;
		public String getInMessage() { return in_msg; }
		public String getInMessage(AgentGeneric user) {
			if(in_msg != null && user != null)
				return Util.replace(in_msg, "%n", user.getName());
			else
				return in_msg;
		}
		public void setInMessage(String msg) { this.in_msg = msg; }

	private String out_msg;
		public String getOutMessage() { return out_msg; }
		public String getOutMessage(AgentGeneric user) {
			if(out_msg != null && user != null)
				return Util.replace(out_msg, "%n", user.getName());
			else
				return out_msg;
		}
		public void setOutMessage(String msg) { this.out_msg = msg; }



	public Exit() {
		//empty
	}



	public static Exit loadExit(XMLElement xml) {
		Exit e = new Exit();

      e.setName(xml.getAttribute(CycThing.PROPERTY_NAME));
      e.targetCycFort = CycMoo.makeCycFort(xml.getAttribute(PROPERTY_TARGET));
      e.type = xml.getAttribute(PROPERTY_TYPE, TYPE_OPEN);
      e.state = xml.getAttribute(PROPERTY_STATE, STATE_OPEN);
      e.key = xml.getAttribute(PROPERTY_KEY, null);
      e.setComment(Util.format(xml.getContent(), 70, true));
		e.in_msg = xml.getAttribute(PROPERTY_IN_MESSAGE, null);
		e.out_msg = xml.getAttribute(PROPERTY_OUT_MESSAGE, null);

      return e;
	}


	public XMLElement toXMLElement() {
		XMLElement xml = new XMLElement();
		xml.setName(MARKUP);
		xml.setAttribute(CycThing.PROPERTY_NAME, getName());
		xml.setAttribute(PROPERTY_TARGET, targetCycFort.toString());

		if(! isType(TYPE_OPEN))
			xml.setAttribute(PROPERTY_TYPE, type);

		if(state > 0)
			xml.setAttribute(PROPERTY_STATE, ""+state);

		if(key != null)
			xml.setAttribute(PROPERTY_KEY, key);

		if(in_msg != null)
			xml.setAttribute(PROPERTY_IN_MESSAGE, in_msg);

		if(out_msg != null)
			xml.setAttribute(PROPERTY_OUT_MESSAGE, out_msg);

		if(getComment() != null && getComment().length() > 0)
			xml.setContent(getComment());

		return xml;
	}

}