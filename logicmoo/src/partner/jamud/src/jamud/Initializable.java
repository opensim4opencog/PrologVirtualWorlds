package jamud;


public abstract interface Initializable {


    /** <code>STATE_TERMINATED = 0</code> */
    public static final int STATE_TERMINATED = 0;


    /** <code>STATE_INITIALIZING = 1</code> */
    public static final int STATE_INITIALIZING = 1;


    /** <code>STATE_TERMINATING = 2</code> */
    public static final int STATE_TERMINATING = 2;


    /** <code>STATE_INITIALIZED = 3</code> */
    public static final int STATE_INITIALIZED = 3;


    /** returns current state */
    public abstract int initializableState();


    /** returns true if target initialized correctly */
    public abstract boolean initialize();


    /** returns true if target terminated correctly */
    public abstract boolean terminate();


}
