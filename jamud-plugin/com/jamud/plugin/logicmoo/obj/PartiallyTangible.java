package  logicmoo.obj;

import logicmoo.util.*;
import logicmoo.obj.*;
import logicmoo.cmd.*;
import logicmoo.CycMoo;
import org.opencyc.cycobject.CycFort;

public abstract class PartiallyTangible extends CycThing {

    public static final String
    PROPERTY_COMMENT = "COMMENT",
    PROPERTY_PRETTYNAME = "PRETTYNAME",
    PROPERTY_KEYWORDS = "KEYWORDS";


    private String desc = null;
    public String getComment() { return this.desc;}
    public void setComment(String desc) { this.desc = desc;}

    private String sdesc = null;
    public String getPrettyName() { return this.sdesc;}
    public void setPrettyName(String sdesc) { this.sdesc = sdesc;}

    private FlagList keywords = new FlagList();
    public FlagList getKeywords() { return keywords;}
    public void setKeywords(FlagList keywords) { this.keywords = keywords;}

    public Microtheory microtheory;// = CycMoo.makeCycConstantSafe("JamudMt");
    public Microtheory getMicrotheory() { return microtheory;}
//    public CycFort getMicrotheory() { return microtheory;}
//    public void setMicrotheory(CycFort microtheory) { this.microtheory = microtheory;}
    public void setMicrotheory(Microtheory a) { this.microtheory = a;}


    public FlagList flags = new FlagList();
    public FlagList getFlags() { return flags;}
    public void setFlags(FlagList flags) { this.flags = flags;}
    public boolean isFlagged(String flag) { return flags.contains(flag);}
    public boolean isFlagged(CycFort flag) { return flags.contains(flag);}
    

    public CycFort type;
    public CycFort getType() { return type;}
    public void setType(CycFort type) { this.type = type;}

    
    public static CycFort makeCycFort(Object element) {
	return(CycFort) makeCycObject(element);
    }

    public static Object makeCycObject(Object element) {
	if ( element==null ) return null;
	if ( element instanceof GuidTracable ) return((GuidTracable)element).getCycFort();
	return CycMoo.makeCycFort(element);
    }

}