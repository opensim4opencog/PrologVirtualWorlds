package jamud.util;


public class Ticker {


    private static final long
	DEFAULT_INTERVAL = 1000;


    private Thread thread;


    private Runnable runner;


    // who we trigger
    private TickListener ticklistener;


    // sleep for how long?
    private long interval;

    /** set the interval to sleep between iterations
     * does not affect current iteration (eg: shortening the interval
     * will not cut short the current sleep state). If an interval is
     * attempted to be set that is not greater than zero, the interval
     * is not modified at all.
     */
    public void setInterval(long interval) {
	if( interval > 0 )
	    this.interval = interval;
    }
    
    /** time in milliseconds between the tick(Ticker) method is called
     * on the associated TickListener
     */
    public long getInterval() {
	return this.interval;
    }


    // keep going?
    private boolean cont;
    
   

    /** create a Ticker which will trigger ticklistener at the default
     * tick interval (1000 ms)
     */
    public Ticker(TickListener ticklistener) {
	this.ticklistener = ticklistener;
	this.interval = Ticker.DEFAULT_INTERVAL;
	init();
    }

    
    /** create a Ticker which will trigger ticklistener every interval
     * milliseconds
     */
    public Ticker(TickListener ticklistener, long interval) {
	this.ticklistener = ticklistener;
	this.interval = interval;
	init();
    }


    private void init() {

	this.runner = new Runnable() {
		public void run() {
		    try {
			cont = true;
			while( cont ) {
			    thread.sleep( interval );
			    if( cont )
				tick();
			}
		    } catch( Exception e ) {
			error( e );
		    }
		}
	    };

	this.thread = new Thread(this.runner);
    }    


    /** Start triggering ticklistener once every interval
     */
    public void start() {
	this.thread.start();
    }


    /** Stop triggering the ticklistener, and stop this thread.
     */
    public void halt() {
	this.cont = false;
    }


    public void tick() {
	this.ticklistener.tick( this );
	this.thread.yield();  // be friendly
    }


    private void error(Exception e) {
	this.ticklistener.tickError( this, e );
    }


    public void finalize() {
	this.ticklistener = null;
    }
    
}
