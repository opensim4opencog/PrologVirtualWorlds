package jamud.command;


public abstract class TrustedCommand extends AbstractCommand {

    private int trust;

    public int getTrust() {
	return this.trust;
    }


    public TrustedCommand(String name, int trust) {
	super(name);
	this.trust = trust;
    }


    public boolean allows(Actor a) {
	return (a.getTrust() >= this.trust);
    }

}
