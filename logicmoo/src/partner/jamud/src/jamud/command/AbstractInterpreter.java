package jamud.command;


import jamud.plugin.*;
import jamud.util.SortedList;
import java.util.*;


public abstract class AbstractInterpreter
    implements Interpreter, PluginOwned {


    // From PluginOwned
    private JamudPlugin owner;

    public JamudPlugin getOwner() {
	return this.owner;
    }


    // From Interpreter
    private String name;

    public String getName() {
	return this.name;
    }


    private int priority;

    public int getPriority() {
	return this.priority;
    }


    private List commands;

    public boolean addCommand(Command c) {
	return this.commands.add( c );
    }

    public boolean removeCommand(Command c) {
	return this.commands.remove( c );
    }

    public Iterator commands() {
	return this.commands.iterator();
    }



    public AbstractInterpreter(String name, int priority) {
	this.name = name;
	this.priority = priority;

	this.commands = Collections.synchronizedList(new SortedList());
    }

    public AbstractInterpreter(String name, int priority, JamudPlugin owner) {
	this(name, priority);

	this.owner = owner;
    }



    // From Comprable (which is from Interpreter)
    public int compareTo(Object o) {
	if(o instanceof Interpreter) {
	    return this.compareTo( (Interpreter)o );
	} else {
	    return -1;
	}
    }

    public int compareTo(Interpreter i) {
	final int r = this.priority - i.getPriority();
	return r;
    }


    public final boolean equals(Object o) {
	return (this == o);
    }


    public abstract boolean allows(Actor whom);

}
