package jamud.command;

import jamud.object.Player;
import jamud.plugin.JamudPlugin;

public class PlayerInterpreter extends AbstractInterpreter {


    public PlayerInterpreter(String name, int priority) {
	super(name, priority);
    }

    public PlayerInterpreter(String name, int priority, JamudPlugin owner) {
	super(name, priority, owner);
    }


    public boolean allows(Actor a) {
	return (a != null &&
		a instanceof Player);
    }

}
