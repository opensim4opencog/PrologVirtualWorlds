package jamud.util;


import jamud.command.*;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;


/** An ArrayList that sorts its additions using compareTo.
 */
public class SortedList extends ArrayList {


    public SortedList() {
	super();
    }


    public SortedList( int initialcapacity ) {
	super( initialcapacity );
    }


    public SortedList( Collection c ) {
	super();
	this.addAll( c );
    }



    public boolean add( Object o ) {
	try {
	    Comparable c = (Comparable) o;

	    for(int I = this.size(); I-- > 0; ) {
		Object p = this.get( I );
		if( c.compareTo(p) > 0 ) {
		    super.add( I+1, c );
		    return true;
		}
	    }

	    super.add( 0, c );
	    return true;

	} catch(Exception e) {
	    return false;
	}
    }


    /** Overridden, because this List sorts the additions
     * as they're added, and allowing an outside source to
     * specify an index is counterproductive to those ends
     */
    public void add( int index, Object o ) {
	// overridden and unavailable
    }


    public boolean addAll( Collection c ) {
	for(Iterator i = c.iterator(); i.hasNext(); ) {
	    if( ! this.add( i.next() ) ) {
		return false;
	    }
	}
	return true;
    }


    /** Overridden, because this List sorts the additions
     * as they're added, and allowing an outside source to
     * specify an index is counterproductive to those ends
     */
    public boolean addAll( int index, Collection c ) {
	return false;
    }

    
    /** Overridden, because this List sorts the additions
     * as they're added, and allowing an outside source to
     * specify an index is counterproductive to those ends
     */
    public Object set( int index, Object element ) {
	return null;
    }

}
