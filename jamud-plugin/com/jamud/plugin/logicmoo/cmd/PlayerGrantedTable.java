package logicmoo.cmd;

import java.util.*;

public class PlayerGrantedTable {
	protected Hashtable grantTable = new Hashtable();

	public Enumeration elements() { return grantTable.elements(); }

	public PlayerGranted get(String name) { return (PlayerGranted) grantTable.get(name.toLowerCase()); }

	public void put(PlayerGranted cmd) { grantTable.put(cmd.getName().toLowerCase(), cmd); }
	public void remove(String name) { grantTable.remove(name.toLowerCase()); }
	public void remove(PlayerGranted cmd) { remove(cmd.getName()); }
}