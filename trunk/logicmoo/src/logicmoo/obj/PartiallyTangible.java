package logicmoo.obj;

import logicmoo.util.FlagList;
import logicmoo.*;
import org.opencyc.api.*;
import org.opencyc.creation.*;
import org.opencyc.cycobject.*;
import org.opencyc.kif.*;
import org.opencyc.util.*;

public abstract class PartiallyTangible extends CycThing {

    
    public static final String
    PROPERTY_DESC = "DESC",
    PROPERTY_SHORTDESC = "SHORT",
    PROPERTY_KEYWORDS = "KEYWORDS";

    public String getDesc() {  return ""+getSlotValueObject(getMt(),getCycFort(),"mudDescription");}
    public void setDesc(String desc) { assertSlotValue(getMt(),getCycFort(),"mudDescription",desc,true);}

    public String getShortDesc() {  return ""+getSlotValueObject(getMt(),getCycFort(),"mudShortDescription");}
    public void setShortDesc(String desc) { assertSlotValue(getMt(),getCycFort(),"mudShortDescription",desc,true);}

    public FlagList getKeywords() { return getSlotValueFlagList(getMt(),getCycFort(),"mudKeyword",null); }
    public void setKeywords(FlagList keywords) { assertSlotValue(getMt(),getCycFort(),"mudKeyword",keywords,true); }

    public boolean isFlagged(String flag) { return getFlags().contains(flag);}
    public FlagList getFlags(String type) { return getSlotValueFlagList(getMt(),getCycFort(),"mudFlag",makeCycFort(type));}
    public void setFlags(FlagList flags,String type) { assertSlotValue(getMt(),getCycFort(),"mudFlag",flags.enumerate(),makeCycFort(type),true);}

    private CycFort thisMt = CycMoo.jamudMt;
    public CycFort getMt() {return thisMt;}
    public void setMt(CycFort mt) {thisMt = mt;}
    public void tickMt() {thisMt = thisMt;}

}