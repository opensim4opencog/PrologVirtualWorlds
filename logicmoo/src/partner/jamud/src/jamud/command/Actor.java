package jamud.command;

public abstract interface Actor {

    public abstract int getTrust();

    public abstract boolean isGranted(String command);

    public abstract void enact(String command);
    
}
