package  logicmoo.obj;
import org.opencyc.cycobject.*; import logicmoo.*;

public abstract interface GuidTracable {

	public static final String
		PROPERTY_VNUM = "VNUM";

	public CycFort getCycFort();
	public void setCycFort(CycFort cycFort);

}