package jamud.object;


/** Connections normally "feed" a Player's command buffer with commands
 * drawn off of some sort of Connection. The abstract method print(String)
 * is called on a Connection whenever a PrintEvent is generated from the
 * associated Player. Extensions of this class must override that method as
 * necessary to relay the String to a client.
 */
public abstract interface Connection {

    /** Method of identifying what sort of connection this is.
     */
    public abstract String getType();
    

    /** The Player this connection feeds.
     */
    public abstract Player getPlayer();


    /** Set the Player this connection feeds. If changing players, this
     * method unsubscribes the PrintEvent from the old player, and then
     * subscribes to the PrintEvent on the new player.
     */
    public abstract void setPlayer(Player player);


    /** Time in milliseconds this connection was created. Note that
     * this value is from System.currentTimeMillis(), not mud run time
     */
    public abstract long connectedAt();

    
    /** Remote address (IP or hostname, depending on implementation)
     */
    public abstract String getRemoteAddress();


    /** Port on the mud server connection is off of
     */
    public abstract int getLocalPort();


    /** Port on client this connection is off of
     */
    public abstract int getRemotePort();


    /** Close down connection. A connection after the disconnect call
     * should be available for garbage collection
     */
    public abstract void disconnect();


    /** Send text over the connection
     */
    public abstract void print(String text);
}
