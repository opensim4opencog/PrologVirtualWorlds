package jamud.command;

public abstract interface Command extends Comparable {

    public abstract String getName();

    public abstract boolean allows(Actor a);

    public abstract void execute(Actor a, String args)
	throws CommandException;

}
