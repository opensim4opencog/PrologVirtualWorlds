package logicmoo.obj;


import java.util.*;
import logicmoo.*;
import logicmoo.util.*;
import net.n3.nanoxml.*;


public class AreaManager {

	public static final String MARKUP = "AREAMANAGER";

	public static final String DEFAULT_ROOM_VNUM = "midga_0001";

	private Hashtable areas = new Hashtable();
		public Area get(String areaPrefix) { return (Area) areas.get(areaPrefix.toLowerCase()); }
		public void put(Area area) { areas.put(area.getPrefix().toLowerCase(), area); }
		public void remove(String areaPrefix) { areas.remove(areaPrefix.toLowerCase()); }
		public void remove(Area area) { remove(area.getPrefix()); }
		public Enumeration elements() { return areas.elements(); }



   public AreaManager(XMLElement xml) throws Exception {
      loadAreas(xml);
		applyExits();
   }



	private void loadAreas(XMLElement xml) throws Exception {
      if(! xml.getName().equals(MARKUP))
			throw new Exception("non-" + MARKUP + " element in AreaManager.loadAreas");

		areas.clear();

      for(Enumeration enum = xml.enumerateChildren(); enum.hasMoreElements(); )
         put(Area.loadArea((XMLElement) enum.nextElement()));
	}


   private void applyExits() throws Exception {
      for(Enumeration enum = elements(); enum.hasMoreElements(); ) {
	  
	  Area area = (Area) enum.nextElement();
	  Jamud.defaultArea = area;


         for(Enumeration renum = area.getRooms().elements(); renum.hasMoreElements(); ) {
            Room rm = (Room) renum.nextElement();
	    Jamud.defaultRoom = rm;

            for(Enumeration exnum = rm.enumerateExits(); exnum.hasMoreElements(); )
               applyExit(rm, (Exit) exnum.nextElement());
         }
      }
   }


   public void applyExit(Room src, Exit ex) throws Exception {
      String t = ex.getTargetCycFort();
      Room T = getRoom(null, t);
      if(T==null)
         throw new Exception("Room VNUM does not exist: " + t);
      else
         ex.setTargetRoom(T);
   }


	public Room getDefaultRoom() {
		return getRoom(null, DEFAULT_ROOM_VNUM);
	}


   public Room getRoom(Area origin, String cycFort) {
       if (origin==null) origin = Jamud.defaultArea;
	return (Room) origin.getRooms().get(cycFort);
   }


	/*
   public Item getItem(Area origin, String cycFort) {
   }

   public Mobile getMobile(Area origin, String cycFort) {
   }

   public Reset getReset(Area origin, String cycFort) {
   }
	*/


}