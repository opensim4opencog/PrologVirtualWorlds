package jamud.channel;


import jamud.object.InsufficientTrustException;
import jamud.object.PlayerMask;
import jamud.object.event.*;
import jamud.util.Attributes;
import jamud.util.Util;
import net.n3.nanoxml.*;


public class Channel implements JamudEventTrigger {


    public static final String
	MARKUP = "CHANNEL";

    private static final String
	PROPERTY_NAME = "NAME",
	PROPERTY_ACLREAD = "READ",
	PROPERTY_ACLWRITE = "WRITE";

    private static final char
	ESCAPE_NAME = 'n',
	ESCAPE_TEXT = 't';



    // The Event Register
    private EventRegister events = new EventRegister();

    public boolean addListener(JamudEventListener listener,
				     Class eventClass) {
	return events.addListener(listener, eventClass);
    }

    public boolean removeListener(JamudEventListener listener) {
	return events.removeListener(listener);
    }

    public boolean removeListener(JamudEventListener listener,
					Class eventClass) {
	return events.removeListener(listener, eventClass);
    }

    public boolean removeListeners() {
	return events.removeListeners();
    }

    public boolean removeListeners(Class eventClass) {
	return events.removeListeners(eventClass);
    }

    public void triggerListeners(JamudEvent event) {
	events.triggerListeners(event);
    }



    private String name;

    public String getName() {
	return this.name;
    }


    private int read;

    public int getAclRead() {
	return this.read;
    }

    public void setAclRead(int read) {
	this.read = read;
    }


    private int write;

    public int getAclWrite() {
	return this.write;
    }

    public void setAclWrite(int write) {
	this.write = write;
    }


    private String display;

    public String getDisplay() {
	return this.display;
    }

    public void setDisplay(String display) {
	this.display = display;
    }



    private Channel() {
	// empty
    }

    public Channel(String name) {
	this.name = name;
	this.display = name.concat(" $n : $t");
    }

    public Channel(String name, String display) {
	this.name = name;
	this.display = display;
    }

    public Channel(String name, int read, int write, String display) {
	this.name = name;
	this.read = read;
	this.write = write;
	this.display = display;
    }



    public void println(PlayerMask source, String text)
	throws InsufficientTrustException {

	if(source.getTrust() < this.write) {
	    throw new InsufficientTrustException(this.write);
	} else {
	    ChannelEvent ce = new ChannelEvent(this, source, text);
	    events.triggerListeners(ce);
	}

    }


    public void subscribe(PlayerMask whom) throws InsufficientTrustException {
	if(whom.getTrust() < this.write) {
	    throw new InsufficientTrustException(this.write);
	} else {
	    this.addListener(whom, ChannelEvent.class);
	    whom.subscribedChannels().add( this.name );
	}
    }


    public void unsubscribe(PlayerMask whom) {
	this.removeListener(whom, ChannelEvent.class);
	whom.subscribedChannels().remove( this.name );
    }


    public static Channel loadChannel(IXMLElement xml) {
	Channel c = new Channel();
	c.load( xml );
	return c;
    }


    public void load(IXMLElement xml) {
	Attributes attrib = new Attributes(xml.getAttributes());

	//name
	this.name = attrib.getAttribute( PROPERTY_NAME, "chat" );

	//acl
	this.read = attrib.getAttribute( PROPERTY_ACLREAD, 0 );
	this.write = attrib.getAttribute( PROPERTY_ACLWRITE, 0 );

	//display
	String c = xml.getContent();
	if(c != null) {
	    this.display = c;
	} else {
	    this.display = name.concat(" $n : $t");
	}
    }


    public IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }


    public void toXMLElement(IXMLElement xml) {
	//name
	xml.setAttribute( PROPERTY_NAME, this.name );
	
	//acl
	xml.setAttribute( PROPERTY_ACLREAD, Integer.toString(this.read) );
	xml.setAttribute( PROPERTY_ACLWRITE, Integer.toString(this.write) );

	//display string
	if( this.display != null) {
	    xml.setContent(this.display);
	}
    }


    public String parseDisplay(PlayerMask source, String text) {
	return Channel.parseDisplay( this.display, source, text );
    }


    public static final String parseDisplay(String display, PlayerMask source,
					    String text) {

	char[] a = { ESCAPE_NAME, ESCAPE_TEXT };
	String[] b = { source.getName(), text };

	return Util.escapeParse( a, b, '$', display );
    }


    public final boolean equals(Object o) {
	return (this == o);
    }

}
