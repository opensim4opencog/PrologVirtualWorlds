package jamud.util;


import java.util.*;
import net.n3.nanoxml.*;


public class MudDateScheme {


    public static final String
	MARKUP = "DATESCHEME";


    private HashMap units;


    private MudDateScheme() {
	units = new HashMap();
    }


}



class Unit {


    public static final String
	MARKUP = "UNIT";


    // subunits
    private int current_subunit;
    private ArrayList subunits;

    /** how many of these units have occured */
    protected int count;

    /** separate unit that this unit's count feeds */
    protected String feeds;

    // home many times this unit must be fed in order to increment count
    protected int length;
    protected int remainder;

    private boolean titled; // numbered if false


    public Unit() {

    }

}



class SubUnit {

    public static final String
	MARKUP = "SUBUNIT";


    protected int length;

    protected String title;

}
