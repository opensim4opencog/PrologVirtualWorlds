package jamud.object;


import net.n3.nanoxml.IXMLElement;


/**
 * A MudObject which does not save.
 * Identical to MudObject except the save() method always returns null, and
 * the save(XMLElement) method doesn't modify the passed XMLElement at all.
 */
public class TemporaryMudObject extends MudObject {



    /** returns null */
    public IXMLElement save() {
	return null;
    }

    /** doesn't do anything at all */
    public void save(IXMLElement xml) {
	return;
    }

}
