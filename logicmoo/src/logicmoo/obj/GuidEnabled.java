package logicmoo.obj;

import org.opencyc.cycobject.*;

public abstract interface GuidEnabled {

	public static final String
		PROPERTY_VNUM = "VNUM";

	public String getCycFortString();
	public CycFort getCycFort();
	public void setCycFort(CycFort cycFort);	
	public void setCycFortString(String cycFort);

}