package logicmoo.obj;

import logicmoo.*;
import logicmoo.util.*;
import java.util.*;
import org.opencyc.api.*;
import org.opencyc.creation.*;
import org.opencyc.cycobject.*;
import org.opencyc.kif.*;
import org.opencyc.util.*;

public abstract class CycThing implements GuidEnabled {

    public static final String PROPERTY_NAME = "NAME";
    public static CycFort cycMudThing = makeCycConstant("MudThing");
    public static CycFort cycMudMt = CycMoo.jamudMt;
    

    private CycFort cycFort = makeCycConstant("load");
    public boolean initializedFort = false;

    public CycFort getCycFort() { return cycFort;}
    public void setCycFort(CycFort cycFortIn) { this.cycFort = cycFortIn; initializedFort=true;}
    public String getCycFortString() { return this.cycFort.toString();}
    public void setCycFortString(String cycFortIn) {
	setCycFort(makeCycFort(cycFortIn));
    }


    private CycFort type = makeCycConstant("MudThing");

    public String getType() { return getNearestIsa().toString();}
    public CycFort getNearestIsa() { return type;}
    public void setType(String typestr) { setType(makeCycFort(typestr));}
    public void setType(CycFort typefort) { this.type = typefort;}

    public String getName() {
	String retval = ""+getSlotValueObject(cycMudMt,getCycFort(),"prettyName");
	if (retval == null) retval = getCycFort().toString();
	return retval;
    }

    public void setName(String name) { 
	if ( cycFort==null ) this.cycFort =  PartiallyTangible.makeCycFort(name);
	assertSlotValue(cycMudMt,getCycFort(),"prettyName",name,true);
    }

    public boolean equals(Object o) {
	if ( o == (Object)this ) return true;
	if ( ! ( o instanceof CycThing) ) return false;
	if ( ((CycThing)o).getCycFort().equals(getCycFort()) )	return true;
	return false;
    }

    public static CycFort makeCycFort(Object element) {
	return(CycFort) makeCycObject(element);
    }

    public static Object makeCycObject(Object element) {
	if ( element==null ) return null;
	if ( element instanceof CycFort ) return element;
	if ( element instanceof GuidEnabled ) return((GuidEnabled)element).getCycFort();
	return CycMoo.makeCycFort(element);
    }

    // SETS

    
/*
    public static void assertSlotValue(CycFort dataMt,CycFort cycobject, String slot, Object value, boolean singlevalued) {
	assertSlotValue(dataMt,cycobject,makeCycConstant(slot),value,singlevalued);
    }
*/
    public static void assertSlotValue(CycFort dataMt,CycFort cycobject, Object slot, Object value, boolean singlevalued) {
	CycMoo.assertSlotValue(dataMt,cycobject,slot,value,singlevalued);
    }
    public static void assertSlotValue(CycFort dataMt,CycFort cycobject, Object slot, Object value, Object type,boolean singlevalued) {
	CycMoo.assertSlotValue(dataMt,cycobject,slot,value,type,singlevalued);
    }

    // GETS

    public static Object getSlotValueObject(Object mt, Object obj, Object slot) {
	Iterator ans = CycMoo.getArrayListQuerySELStatic("InferencePSC","( "+ makeCycFort(slot).stringApiValue() + " " + makeCycFort(obj).stringApiValue()+" ?SEL)").iterator();
	if ( !ans.hasNext() ) return null;
	return ans.next();
    }

    public static CycFort getSlotValueFort(Object mt, Object obj, Object slot) {
	return makeCycFort(getSlotValueObject(mt,obj,slot));
    }

    public static Inventory getSlotValueInventory(CycFort dataMt,CycFort cycobject, Object slot, Object type) {
	return new Inventory(dataMt,cycobject,makeCycFort(slot),makeCycFort(type));
    }
    
    public static FlagList getSlotValueFlagList(CycFort dataMt,CycFort cycobject, Object slot,Object type) {
	return new FlagList(dataMt,cycobject,makeCycFort(slot),makeCycFort(type));
    }

    public static CycFort makeCycConstant(String name){
	return CycMoo.makeCycConstantSafe(name);
    }

}