package  logicmoo.obj;

import org.opencyc.cycobject.CycFort;
import logicmoo.CycMoo;
import java.util.*;


public abstract class CycThing {

    public static final String
    PROPERTY_NAME = "NAME";

    private String name = null;
    public String getName() {
	if (this.name==null) {
	    if (cycFort==null) return null;
	    return this.cycFort.toString();
	} else return this.name;
    }
    
    public void setName(String name) { 
	this.name = name;
	 if (cycFort==null) {
	     this.cycFort =  PartiallyTangible.makeCycFort(this.name);
	}
    }

    public boolean equals(Object o) {
	if ( o == (Object)this ) return true;
	if ( ! ( o instanceof CycThing) ) return false;
	if ( ((CycThing)o).getCycFort().equals(this.cycFort) )	return true;
	return false;
    }

    public CycFort cycFort = CycMoo.makeCycConstantSafe("load");
    public CycFort getCycFort() { return cycFort;}
    public void setCycFort(CycFort cycFort) { this.cycFort = cycFort;}


    public static Object getSlotValueObject(CycFort obj, String slot) {
	Iterator ans = CycMoo.getArrayListQuerySELStatic("InferencePSC","(#$"+slot + " " + obj.stringApiValue()+" ?SEL)").iterator();
	if ( !ans.hasNext() ) return null;
	return ans.next();
    }

    public static CycFort getSlotValueFort(CycFort obj, String slot) {
	return CycMoo.makeCycFort(getSlotValueObject(obj,slot));
    }

    //abstract public CycFort getType();


}