package jamud.command;

import jamud.object.*;
import jamud.plugin.JamudPlugin;

public class MudObjectInterpreter extends AbstractInterpreter {


    public MudObjectInterpreter(String name, int priority) {
	super(name, priority);
    }

    public MudObjectInterpreter(String name, int priority, JamudPlugin owner) {
	super(name, priority, owner);
    }


    public boolean allows(Actor a) {
	if(a == null) {
	    return false;
	} else if(a instanceof MudObject) {
	    return true;
	} else if(a instanceof Player) {
	    return (((Player)a).getBody() != null);
	} else {
	    return false;
	}
    }

}
