package logicmoo.util;

public abstract interface TickListener {
   public void tick(Ticker source);
   public void tickError(Ticker source, Exception e);
}