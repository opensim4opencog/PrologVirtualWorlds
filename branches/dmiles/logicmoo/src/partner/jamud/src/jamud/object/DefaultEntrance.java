package jamud.object;


import jamud.util.Attributes;
import net.n3.nanoxml.*;


abstract class DefaultEntrance extends Entrance {


    private static final String
	PROPERTY_CLASS = "CLASS",
	PROPERTY_TYPE = "TYPE",
	PROPERTY_STATE = "STATE",
	PROPERTY_TARGET = "TARGET";


    private static final String
	PROPERTY_DESCRIPTION = "DESC",
	PROPERTY_SHORT = "SHORT",
	PROPERTY_KEYWORDS = "KEYWORDS";


    public abstract String defaultName();

    public abstract String defaultDestinationMessage();

    public abstract String defaultOriginMessage();



    public DefaultEntrance() {
	super.setName( defaultName() );
	super.setDestinationMessage( defaultDestinationMessage() );
	super.setOriginMessage( defaultOriginMessage() );
    }


    
    public final IXMLElement toXMLElement() {
	XMLElement xml = new XMLElement( super.MARKUP );
	toXMLElement( xml );
	return xml;
    }


    public final void toXMLElement(IXMLElement xml) {
	xml.setAttribute( PROPERTY_CLASS, this.getClass().getName() );
	
	// Random Attribute Stuff
	xml.setAttribute( PROPERTY_TYPE, Integer.toString(getType()) );
	xml.setAttribute( PROPERTY_STATE, Integer.toString(getState()) );
	xml.setAttribute( PROPERTY_TARGET, getDestinationTid() );

	//MudObjectViewable
	IXMLElement nxt;

	// desc, sdesc
	String desc = getDescription();
	String sdesc = getShortDescription();
	if( (desc != null && desc.length() > 0) ||
	    (sdesc != null && sdesc.length() > 0) ) {

	    nxt = new XMLElement(PROPERTY_DESCRIPTION);
	    nxt.setContent(desc);
	    if(sdesc != null) {
		nxt.setAttribute(PROPERTY_SHORT, sdesc);
	    }
	    xml.addChild(nxt);
	}

    }


    public IXMLElement save() {
	MudObjectContainer destination = getDestination();
	if(destination != null &&
	   destination.isTemplate()) {
	    return toXMLElement();
	} else {
	    return null;
	}
    }


    public void save(IXMLElement xml) {
	MudObjectContainer destination = getDestination();
	if(destination != null &&
	   destination.isTemplate()) {
	    toXMLElement(xml);
	}
    }


    public void load(IXMLElement xml) {
	// desc, sdesc
	IXMLElement nxt = xml.getFirstChildNamed(PROPERTY_DESCRIPTION);
	if(nxt != null) {
	    String desc = nxt.getContent();
	    if(desc == null) {
		desc = "";
	    }
	    setDescription( desc );
	    setShortDescription( nxt.getAttribute(PROPERTY_SHORT, "") );
	}

	Attributes at = new Attributes(xml.getAttributes());
	setType( at.getAttribute(PROPERTY_TYPE, TYPE_OPEN) );
	setState( at.getAttribute(PROPERTY_STATE, STATE_OPEN) );
	setDestinationTid( at.getAttribute(PROPERTY_TARGET, null) );

    }

}
