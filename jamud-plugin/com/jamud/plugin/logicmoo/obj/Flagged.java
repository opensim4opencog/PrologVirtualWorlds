package  logicmoo.obj;

import logicmoo.util.FlagList;
import org.opencyc.cycobject.*; import logicmoo.*;

public abstract interface Flagged {

	public static final String
		PROPERTY_FLAGS = "FLAGS";

	public FlagList getFlags();
	public void setFlags(FlagList flags);

       // public boolean isFlagged(String flag);
	public boolean isFlagged(CycFort flag);

}