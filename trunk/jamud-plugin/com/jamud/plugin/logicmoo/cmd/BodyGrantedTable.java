package logicmoo.cmd;

import java.util.*;

public class BodyGrantedTable {
	protected Hashtable grantTable = new Hashtable();

	public Enumeration elements() { return grantTable.elements(); }

	public BodyGranted get(String name) { return (BodyGranted) grantTable.get(name.toLowerCase()); }

	public void put(BodyGranted cmd) { grantTable.put(cmd.getName().toLowerCase(), cmd); }
	public void remove(String name) { grantTable.remove(name.toLowerCase()); }
	public void remove(BodyGranted cmd) { remove(cmd.getName()); }
}