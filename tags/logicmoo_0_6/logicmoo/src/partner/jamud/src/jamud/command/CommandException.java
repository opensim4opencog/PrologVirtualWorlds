package jamud.command;

import jamud.JamudException;

public class CommandException extends JamudException {


    private Command c;

    public Command getCommand() {
	return this.c;
    }


    private Actor a;

    public Actor getActor() {
	return this.a;
    }



    public CommandException(Command command, Actor actor, String message) {
	super(message);
	this.c = command;
	this.a = actor;
    }

    public CommandException(Command command, Actor actor, Exception e) {
	super(e);
	this.c = command;
	this.a = actor;
    }


}
