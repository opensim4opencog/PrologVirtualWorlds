package jamud.object;


/**
 * a Creation object is a sort of script or series of actions which
 * lead to the creation of a Player from a blank template. Normally
 * creation consists of prompting for simple player-choices such as
 * a name, a password, an email address, etc.
 */
public abstract interface Creation {


    /** enact creation upon Player p. If the creation attempt fails
     * for any reason, a CreationException is thrown
     */
    public abstract void enact(Player p) throws CreationException;

}
