package jamud.object;


import net.n3.nanoxml.IXMLElement;


public class TemporaryEntrance extends Entrance {


    public TemporaryEntrance() {
	//empty
    }

    public TemporaryEntrance(String tid) {
	super.setDestinationTid(tid);
    }

    public TemporaryEntrance(MudObjectContainer dest) {
	super.setDestination(dest);
    }



    public IXMLElement save() {
	return null;
    }


    public void save(IXMLElement xml) {
	return;
    }


    public Object clone() {
	TemporaryEntrance mo = new TemporaryEntrance();
	mo.copycat( this );
	return (Object) mo;	
    }

}
