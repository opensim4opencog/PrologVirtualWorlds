package jamud.command;


import jamud.plugin.JamudPlugin;
import jamud.util.*;
import java.util.*;
import net.n3.nanoxml.IXMLElement;


public class InterpreterManager {


    public static final String MARKUP = "INTERPRETERMANAGER";


    private boolean ignoreCase = false;

    public boolean ignoresCase() {
	return this.ignoreCase;
    }

    public void setIgnoresCase(boolean ignore) {
	this.ignoreCase = ignore;
    }


    private boolean exactMatch = false;

    public boolean exactlyMatches() {
	return this.exactMatch;
    }

    public void setExactlyMatches(boolean exact) {
	this.exactMatch = exact;
    }


    private List interpreters;

    public boolean addInterpreter(Interpreter i) {
	return this.interpreters.add( i );
    }

    public boolean removeInterpreter(Interpreter i) {
	return this.interpreters.remove( i );
    }

    public Iterator interpreters() {
	return this.interpreters.iterator();
    }



    public InterpreterManager() {
	this.interpreters = Collections.synchronizedList(new SortedList());
    }



    /** Wraps to the methods findCommand and findCommandIgnoreCase, dependant
     * on interpreter manager's settings
     */
    public Command completeCommand(String cmd, Actor whom) {
	if(this.ignoreCase) {
	    return findCommandIgnoreCase(cmd, whom);
	} else {
	    return findCommand(cmd, whom);
	}
    }


    /** Wraps to the methods nearestCommand and exactCommand, dependant
     * on interpreter manager's settings
     */
    public Command findCommand(String cmd, Actor whom) {
	if(this.exactMatch) {
	    return exactCommand(cmd, whom);
	} else {
	    return nearestCommand(cmd, whom);
	}
    }


    /** Wraps to the methods nearestCommandIgnoreCase and
     * exactCommandIgnoreCase, dependant on interpreter manager's settings.
     */
    public Command findCommandIgnoreCase(String cmd, Actor whom) {
	if(this.exactMatch) {
	    return exactCommandIgnoreCase(cmd, whom);
	} else {
	    return nearestCommandIgnoreCase(cmd, whom);
	}
    }


    /**
     * Finds the nearest command match. This is done by iteratring through
     * the registered interpreters in priority order, in turn iterating through
     * each interpreter's commands in alphabetical order. If a command is
     * found which is an exact match, it is returned. If a command is found
     * which is a partial match (in that the String cmd is the starting portion
     * of the found command) it is cached to be returned if no exact matches
     * are later found.
     */
    public Command nearestCommand(String cmd, Actor whom) {

	Command cached = null;

	// this assumes that the iterator is in order of interpreter
	// precedence... which is will be, because so it shall be written
	for(Iterator i = interpreters.iterator(); i.hasNext(); ) {

	    // for each interpreter which doesn't hate you...
	    Interpreter in = (Interpreter) i.next();
	    if( in.allows(whom) ) {
		
		// go through the commands
		for(Iterator t = in.commands(); t.hasNext(); ) {
		    Command c = (Command) t.next();

		    // if the command likes you
		    if(c.allows(whom)) {

			final int compare = c.getName().compareTo(cmd);

			if(cached == null) {
			    if(compare == 0) {
				return c;
			    } else if(c.getName().startsWith(cmd)) {
				cached = c;
			    }

			} else {
			    if(compare == 0) {
				return c;
			    }
			}
			
			if(compare > 0) {
			    // we've gone past it alphabetically
			    break;
			}
			
		    }
		}
	    }
	}

	// we didn't find an exact match, so here's the first near
	// match we found
	return cached;
    }



    /**
     * Finds the nearest command match, ignoring case. This is done by
     * iteratring through the registered interpreters in priority order, in
     * turn iterating through each interpreter's commands in alphabetical
     * order. If a command is found which is an exact match, it is returned.
     * If a command is found which is a partial match (in that the String cmd
     * is the starting portion of the found command) it is cached to be
     * returned if no exact matches are later found.
     */
    public Command nearestCommandIgnoreCase(String cmd, Actor whom) {

	// tricky!
	cmd = cmd.toLowerCase();

	Command cached = null;

	// this assumes that the iterator is in order of interpreter
	// precedence... which is will be, because so it shall be written
	for(Iterator i = interpreters.iterator(); i.hasNext(); ) {

	    // for each interpreter which doesn't hate you...
	    Interpreter in = (Interpreter) i.next();
	    if( in.allows(whom) ) {
		
		// go through the commands
		for(Iterator t = in.commands(); t.hasNext(); ) {
		    Command c = (Command) t.next();

		    // if the command likes you
		    if(c.allows(whom)) {

			String a = c.getName().toLowerCase();

			final int compare = a.compareTo(cmd);

			if(cached == null) {
			    if(compare == 0) {
				return c;
			    } else if(a.startsWith(cmd)) {
				cached = c;
			    }

			} else {
			    if(compare == 0) {
				return c;
			    }
			}
			
			if(compare > 0) {
			    // we've gone past it alphabetically
			    break;
			}
			
		    }
		}
	    }
	}

	// we didn't find an exact match, so here's the first near
	// match we found
	return cached;
    }


    /** Find the first command by its exact name */
    public Command exactCommand(String cmd, Actor whom) {

	// this assumes that the iterator is in order of interpreter
	// precedence... which is will be, because so it shall be written
	for(Iterator i = interpreters.iterator(); i.hasNext(); ) {

	    // for each interpreter which doesn't hate you...
	    Interpreter in = (Interpreter) i.next();
	    if( in.allows(whom) ) {
		
		// go through the commands
		for(Iterator t = in.commands(); t.hasNext(); ) {
		    Command c = (Command) t.next();

		    // if the command likes you
		    if(c.allows(whom)) {

			final int compare = c.getName().compareTo(cmd);
			if(compare == 0) {
			    return c;
			}
			
			if(compare > 0) {
			    // we've gone past it alphabetically
			    break;
			}
			
		    }
		}
	    }
	}

	return null;
    }


    /** Find the first command by its exact name, ignoring case */
    public Command exactCommandIgnoreCase(String cmd, Actor whom) {

	// this assumes that the iterator is in order of interpreter
	// precedence... which is will be, because so it shall be written
	for(Iterator i = interpreters.iterator(); i.hasNext(); ) {

	    // for each interpreter which doesn't hate you...
	    Interpreter in = (Interpreter) i.next();
	    if( in.allows(whom) ) {
		
		// go through the commands
		for(Iterator t = in.commands(); t.hasNext(); ) {
		    Command c = (Command) t.next();

		    // if the command likes you
		    if(c.allows(whom)) {

			final int compare = c.getName()
			    .compareToIgnoreCase(cmd);

			if(compare == 0) {
			    return c;
			}
			
			if(compare > 0) {
			    // we've gone past it alphabetically
			    break;
			}
			
		    }
		}
	    }
	}

	return null;
    }


    /**
     * Splits a String into a command and arguments. Last element of array
     * contains all remaining parts of the passed String. Parsed by spaces,
     * double-quotes, and parenthases. The parts determined to be sectional
     * markup are not counted as part of the argument.<br><br>
     * eg:<br>
     * <code>String[] st = InterpreterManager.commandSplit("(big stick)
     * little dude");</code>
     * <br><br>
     * st[0] would be "big stick"<br>
     * st[1] would be "little dude"
     */
    public static final String[] commandSplit(String command) {
	String[] ret = new String[2];

	if( command == null ||
	    (command=command.trim()).length() == 0 ) {
	    return ret;
	}

	int L = 0;
	
	if(command.charAt(0) == '\"') {
	    L = command.indexOf("\"", 1);

	} else if(command.charAt(0) == '(') {
	    L = command.indexOf(")", 1);

	} else {
	    return Util.split(command, " ");
	}
	
	if(L == -1 || L == command.length()) {
	    ret[0] = command.substring(1).trim();
	    ret[1] = "";

	} else {
	    ret[0] = command.substring(1, L).trim();
	    ret[1] = command.substring(L+1).trim();
	}
	
	return ret;
    }


    public void load(IXMLElement xml) {
	Attributes att = new Attributes(xml.getAttributes());

	this.exactMatch = att.getAttribute("EXACTMATCH", false);
	this.ignoreCase = att.getAttribute("IGNORECASE", false);
    }

}
