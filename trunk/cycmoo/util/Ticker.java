package logicmoo.util;

public class Ticker extends Thread {

   private TickListener ticklistener;

   private long Interval = 1000;
      public void setInterval(long Interval) { this.Interval = Interval; }
      public long getInterval() { return Interval; }

   private boolean cont;
      public void halt() { this.cont = false; }
   
   public Ticker(TickListener ticklistener) {
      this.ticklistener = ticklistener;
   }
   public Ticker(TickListener ticklistener, long Interval) {
      this.ticklistener = ticklistener;
      this.Interval = Interval;
   }


   public void run() {
      try {
         cont = true;
         while(cont) {
            this.sleep(Interval);
            if(cont) {
					ticklistener.tick(this);
	            this.yield();
				}
         }
      }
      catch(Exception e) {
         ticklistener.tickError(this, e);
      }
		finally {
			ticklistener = null;
		}
   }

}
