package jamud.object;


import java.io.IOException;
import net.n3.nanoxml.*;


public class MudObjectRoot extends MudObjectContainer {


    public static final String
	MARKUP = "ROOT";



    public MudObjectRoot() {
	super(null);
    }



    public IXMLElement save() throws Exception {
	IXMLElement xml = new XMLElement(MARKUP);
	save(xml);
	return xml;
    }


    public void load(IXMLElement xml) throws Exception {
	super.load(xml);
    }
    

    public IXMLElement toXMLElement() {
	IXMLElement xml = new XMLElement(MARKUP);
	toXMLElement(xml);
	return xml;
    }

}
