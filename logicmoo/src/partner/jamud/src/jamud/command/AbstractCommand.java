package jamud.command;


public abstract class AbstractCommand implements Command {

    private String name;

    public String getName() {
	return this.name;
    }


    public AbstractCommand(String name) {
	this.name = name;
    }



    public int compareTo(Object o) {
	if(o instanceof Command) {
	    return this.compareTo( (Command)o );
	} else {
	    return -1;
	}
    }


    public int compareTo(Command p) {
	return this.name.compareTo(p.getName());
    }


    public final boolean equals(Object o) {
	return (this == o);
    }

}
