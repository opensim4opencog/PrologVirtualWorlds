package jamud.util;


public abstract interface TickListener {


   public abstract void tick(Ticker source);


   public abstract void tickError(Ticker source, Exception e);

}
