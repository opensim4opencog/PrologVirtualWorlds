package jamud.command;


public abstract class GrantedCommand extends AbstractCommand {


    public GrantedCommand(String name) {
	super(name);
    }


    public boolean allows(Actor a) {
	return a.isGranted( super.getName() );
    }

}
