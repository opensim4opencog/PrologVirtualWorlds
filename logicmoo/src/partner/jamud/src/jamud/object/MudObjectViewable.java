package jamud.object;


import jamud.util.FlagList;
import jamud.util.Util;
import java.io.IOException;
import java.util.Enumeration;
import net.n3.nanoxml.*;


public abstract class MudObjectViewable implements Viewable {


    private static final String
	PROPERTY_NAME = "NAME",
	PROPERTY_DESCRIPTION = "DESC",
	PROPERTY_SHORT = "SHORT",
	PROPERTY_KEYWORDS = "KEYWORDS";

    private String name = "";

    public void setName(String name) {
	this.name = name;
    }

    public String getName() {
	return this.name;
    }


    private String desc = "";

    public void setDescription(String desc) {
	this.desc = desc;
    }

    public String getDescription() {
	return this.desc;
    }


    private String sdesc = "";

    public void setShortDescription(String sdesc) {
	this.sdesc = sdesc;
    }

    public String getShortDescription() {
	return this.sdesc;
    }


    private FlagList keywords = new FlagList();

    public void setKeywords(FlagList keywords) {
	this.keywords = keywords;
    }

    public FlagList getKeywords() {
	return this.keywords;
    }

    public boolean matchKeyword(String keyword) {
	return (this.keywords.containBeginsWith(keyword) ||
		this.name.startsWith(keyword));
    }


    public void load(IXMLElement xml) throws Exception {

	// load Named
	this.name = xml.getAttribute(PROPERTY_NAME, null);

	IXMLElement nxt;

	// desc, sdesc
	nxt = xml.getFirstChildNamed(PROPERTY_DESCRIPTION);
	if(nxt != null) {
	    this.desc = nxt.getContent();
	    if(this.desc == null) {
		this.desc = ""; // TODO: default no-desc string?
	    } else {
		this.desc = Util.format(this.desc, 78, true);
	    }
	    this.sdesc = nxt.getAttribute(PROPERTY_SHORT, "");
	}

	// keywords
	nxt = xml.getFirstChildNamed(PROPERTY_KEYWORDS);
	if(nxt != null) {
	    String k = nxt.getContent();
	    if(k != null && k.length() != 0) {
		keywords.refresh( k );
	    }
	}
    }


    public void toXMLElement(IXMLElement xml) {
	// save Named
	if( this.name != null ) {
	    xml.setAttribute(PROPERTY_NAME, this.name);
	}

	IXMLElement nxt;

	// desc, sdesc
	if( (desc != null && desc.length() > 0) ||
	    (sdesc != null && sdesc.length() > 0) ) {

	    nxt = new XMLElement(PROPERTY_DESCRIPTION);
	    nxt.setContent(desc);
	    if(sdesc != null) {
		nxt.setAttribute(PROPERTY_SHORT, sdesc);
	    }
	    xml.addChild(nxt);
	}

	// keywords
	if( keywords.size() > 0 ) {
	    nxt = new XMLElement(PROPERTY_KEYWORDS);
	    nxt.setContent(keywords.toString());
	    xml.addChild(nxt);
	}
    }


    public void copycat(Viewable mirror) {
	// copy Named
	this.name = mirror.getName();

	// desc, sdesc
	desc = mirror.getDescription();
	sdesc = mirror.getShortDescription();

	// keywords
	keywords = new FlagList(mirror.getKeywords());
    }

}
