package com.jamud.commands.olc;


import jamud.plugin.*;
import jamud.util.SortedList;
import jamud.command.*;
import jamud.*;
import java.util.*;


public class BuilderInterpreter extends AbstractInterpreter  {


    public BuilderInterpreter(String name, int priority) {
	super(name, priority);
    }

    public BuilderInterpreter(String name, int priority, JamudPlugin owner) {
	super(name, priority, owner);
    }

    public boolean allows(Actor a) {
	return (a != null ) ; // Allows anything Mobs/Robots/etc.. to use these commands
        // If this causes problems we can tighten
    }

}
