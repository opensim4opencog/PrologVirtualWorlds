package jamud.channel;


import java.util.*;


public class ChannelManager {


    private Set channels;



    public ChannelManager() {
	this.channels = Collections.synchronizedSet( new HashSet() );
    }



    public Iterator channels() {
	return this.channels.iterator();
    }


    public boolean addChannel(Channel channel) {
	return this.channels.add( channel );
    }


    public boolean removeChannel(Channel channel) {
	return this.channels.remove( channel );
    }


    public boolean removeChannel(String name) {
	for(Iterator i = this.channels.iterator(); i.hasNext(); ) {
	    if( ((Channel)i.next()).getName().equals(name) ) {
		i.remove();
		return true;
	    }
	}
	return false;
    }


    public Channel getChannel(String name) {
	for(Iterator i = this.channels.iterator(); i.hasNext(); ) {
	    Channel c = (Channel) i.next();
	    if( c.getName().equals(name) ) {
		return c;
	    }
	}
	return null;
    }

}
