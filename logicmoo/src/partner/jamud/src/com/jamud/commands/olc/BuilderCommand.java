package com.jamud.commands.olc;


import jamud.command.*;
import jamud.object.*;
import jamud.util.*;
import java.util.*;
import net.n3.nanoxml.*;
import java.lang.reflect.*;


class BuilderCommand implements Command {

        public static final String
        MARKUP = "BUILDER";


        public static final String
        PROPERTY_NAME = "NAME",
        PROPERTY_OBJECTSCOPE = "OBJECTSCOPE";



        private String name;

        public String getName() {
                return this.name;
        }



        public BuilderCommand(String name) {
                this.name = name;
        }



        // returns true if the actor passes all the allow and deny rules
        public boolean allows(Actor actor) {
                return( actor != null );
        }


        private String execute(MudObject actor) {
                // Display help for @
                return "for help use: @ [create|set|unset|remove|flaglist|destroy] ";
        }

        public void execute(Actor actor, String args) throws CommandException {
                if ( actor == null ) {
                        throw new CommandException(this, actor, "Actor is null");

                } else if ( actor instanceof Player ) {
                        if ( (actor = ((Player)actor).getBody()) != null ) {
                                MudObject o = (MudObject)actor;
                                o.println( execute((MudObject)o, args) );
                        }

                } else if ( actor instanceof MudObject ) {
                        MudObject o = (MudObject)actor;
                        o.println( execute((MudObject)o, args) );

                } else {
                        throw new CommandException(this, actor,
                                                   "Actor is not a MudObject");
                }
        }

        private String execute(MudObject actor, String args) {

                // assume @ set foo66 job "disfunct"
                String cmdName = InterpreterManager.commandSplit(args)[0];
                String params = InterpreterManager.commandSplit(args)[1];
                String objName = InterpreterManager.commandSplit(params)[0];

                MudObjectViewable objTarget = null;

		if (  cmdName == null )
			  return "for help use: @ [create|set|unset|remove|flaglist|destroy] ";
                
		if ( cmdName.equals("create")||cmdName.startsWith("+") )
                        return createObject(actor,objName,InterpreterManager.commandSplit(params)[1]);

                try {
                        objTarget = BuilderCommandUtility.getPeerObject(actor,objName);
                } catch ( Exception e ) {
                        // All exceptions will just leave objTarget null and caught bellow
                        // InvalidTargetException would be more appropriate :)
                }


                if ( objTarget==null ) {
                        objTarget =  ((MudObjectViewable)(actor.attributes().get(PROPERTY_OBJECTSCOPE)));
                        if ( objTarget==null ) {
                                // No scope or valid target (request clarity)
                                // temparily assume the target was switched with command and try again
                                objTarget = BuilderCommandUtility.getPeerObject(actor,cmdName);
                                if ( objTarget!=null ) {
                                        // gees they did get them switched
                                        cmdName = objName;
                                } else {
                                        // really missed the target i.g. no default scope or valid command or target
                                        return "Unable to parse \"" + args+ "\"";
                                }
                        }
                } else {
                        // Object name in string therefore increment params
                        params =  InterpreterManager.commandSplit(params)[1];
                }

                // We have successfully loaded cmdName,objTarget,params
                return execute(actor ,cmdName, objTarget, params);
        }



        private String execute( MudObject actor, String cmdName, MudObjectViewable objTarget, String params ) {

                // objTarget specified was not found
                if ( objTarget == null )
                        return "Your toy is not here.";

                switch ( cmdName.toLowerCase().charAt(0) ) {
                        // Set
                        case 's' :
                                return this.setProperty(actor,objTarget,params);
                        case 'u' :
                                return this.unsetProperty(actor,objTarget,params);
                        case 'a' :
                                return this.addProperty(actor,objTarget,params);
                        case 'r' :
                                return this.removeProperty(actor,objTarget,params);
                        case 'c' :
                                return this.cloneObject(actor,objTarget,params);
                        case 'f' :
                                return this.flaglistEdit(actor,objTarget,params);
                        case 'd' :
                                return this.destroyObject(actor,objTarget,params);
                }
                return execute(actor);        // Displays information about command
        }

        protected static String createObject(MudObject actor,String type, String params) {
                
                if (type==null) {
                         return "usage: @ create <room|objectname> ";
                }
                
                if (type==null) {
                         return "usage: @ create <room|objectname> ";
                }

                if ( type.equals("room") )
                        return createRoomObject( actor, params);

                MudObjectViewable newObj =null;

                MudObjectViewable example = BuilderCommandUtility.getPeerObject(actor,type);
                String name = InterpreterManager.commandSplit(params)[0];

                if ( example!=null ) {
                        try {
                                newObj = (MudObjectViewable)BuilderCommandUtility.createObjectFromExample(actor.getParentContainer(),example,name);
                        } catch ( Exception e ) {
                                return "" + e;
                        }
                        BuilderCommandUtility.inform(actor,"copy created from " + ((MudObjectViewable)example).getName()+ "." );
                        return setProperty(actor,newObj, InterpreterManager.commandSplit(params)[1]);
                } else {
                        try {
                                newObj = (MudObjectViewable)BuilderCommandUtility.createObjectFromType(actor.getParentContainer(),type,name);
                        } catch ( Exception e ) {
                                return "" + e;
                        }
                        BuilderCommandUtility.inform(actor,"created " + ((MudObjectViewable)newObj).getName()+ "." );
                        return setProperty(actor,newObj, InterpreterManager.commandSplit(params)[1]);
                }
        }

        protected static void  setScope(MudObject actor, MudObjectViewable newScope) {
                //  if ( actor instanceof Player ) {
                BuilderCommandUtility.inform(actor,"Your scope is now " + newScope.getName());
                //BuilderCommandUtility.setProperty(actor,PROPERTY_OBJECTSCOPE,newScope);
                actor.attributes().setAttribute(PROPERTY_OBJECTSCOPE,newScope.getName());
                //  }
        }

        protected static void  ensureUnique(MudObjectViewable newThing) {
                if ( newThing instanceof MudObject ) {
                        ((MudObject)newThing).nextLoadId();
                } else if ( newThing instanceof MudObjectContainer ) {
                        // TODO: ((MudObjectContainer)newThing).setTemplateId(...);
                }
        }


        protected static String createRoomObject(MudObject actor, String params) {
                
                // TODO: This will change with Sieges new object design
                MudObjectContainer actorsRoom = actor.getParentContainer();
                MudObject actorsBuilding = actorsRoom.parentObject();
                String roomName = InterpreterManager.commandSplit(params)[0];
                
                if (roomName==null) 
                        return "usage: @ create room <name> <direction>";
		
		MudObjectContainer newRoom = null;
                try {
                        newRoom = (MudObjectContainer)BuilderCommandUtility.createRoomObject(actorsBuilding,roomName);
                } catch ( Exception e ) {
                        e.printStackTrace(System.err);
                        return ""+e;
                }
                setScope(actor,newRoom);
                String roomparams = InterpreterManager.commandSplit(params)[1];

                //if ( actor instanceof Player )
                actor.setParentContainer(newRoom);
                if ( roomparams.length()==0 ) {
                        return "You have been placed in a your new room (with no escape!)";
                } else {
                        createTunnel(actor,actorsRoom,newRoom,roomparams);
                        return "You have been placed in a your new room";
                }
        }


        protected static void createTunnel(MudObject actor,MudObjectContainer actorsRoom,MudObjectContainer newRoom, String roomparams) {
                String direction = InterpreterManager.commandSplit(roomparams)[0];
                String nameForExit = InterpreterManager.commandSplit(roomparams)[1];

                Entrance newEntrance = null;

                try {
                        newEntrance = BuilderCommandUtility.createExit(actorsRoom,newRoom,direction,nameForExit);

                        BuilderCommandUtility.inform(actor,("A passageway " + BuilderCommandUtility.getDirection(newEntrance) + " was created from " + actorsRoom.getName() + " to " + newRoom.getName()) ) ;

                        newEntrance = BuilderCommandUtility.createExit(newRoom,actorsRoom,BuilderCommandUtility.reverseDirection(direction),nameForExit);

                        BuilderCommandUtility.inform(actor,("A passageway " + BuilderCommandUtility.getDirection(newEntrance) + " was created from " + newRoom.getName() + " to " + actorsRoom.getName() )) ;

                } catch ( Exception e ) {

                        BuilderCommandUtility.inform(actor,""+e);

                }
        }



        protected static String addProperty(MudObject actor,MudObjectViewable target, String params) {
                return setProperty( actor,  target,  params);
        }

        protected static String setProperty(MudObject actor, Object target, String params) {

                if (target == null || params == null ) {
			return "@ set [object] <propertyname> <value>";
                }

                String property = InterpreterManager.commandSplit(params)[0];
                String value = InterpreterManager.commandSplit(params)[1];

                if ( value.length()<1 ) {
                        return "@ set [object] <propertyname> <value>";
                }

                if ( BuilderCommandUtility.attemptJavaSetMethod(target,property,value) )
                        return("object property " +property+ " set to \"" + value + "\"");

                if ( target instanceof MudObject ) {
                        ((MudObject)target).attributes().setAttribute(property,value);
                        return "property " +property+ " set to \"" + value + "\"";
                }

                return "class not supported: " + target.getClass().getName();
        }


        protected static String unsetProperty(MudObject actor,
                                              MudObjectViewable target, 
                                              String params) {

                return removeProperty( actor,  target,  params);
        }


        protected static String removeProperty(MudObject actor,
                                               MudObjectViewable target,
                                               String params) {

                String property = InterpreterManager.commandSplit(params)[0];

                if ( property.length() < 1 ) {
                        return "@ unset [object] <propertyname>";

                } else {
                        if ( target instanceof MudObject ) {
                                ( (MudObject)target ).attributes().remove( property ) ;
                                return "property " + property + " removed";
                        }

                        return target.getClass() + " is does not have these kinds of unsetable attributes. (yet)";
                }
        }


        protected static String cloneObject(MudObject actor,
                                            MudObjectViewable target, 
                                            String params) {
                return "cloneObject not yet implemented.";
        }


        protected static String flaglistEdit(MudObject actor,
                                             MudObjectViewable target,
                                             String params) {

                String flagname = InterpreterManager.commandSplit(params)[0];
                if ( flagname.length()<1 ) {
                        return "@ flaglist [object] <flagname> [false,0|true,1] (default is to true). \n" +
                        "@ flaglist [object] Clear";
                }

                FlagList fl = ((MudObject)target).flags();
                if ( flagname.equals("Clear") ) {
                        fl.clear();
                        return "All " + (target.getName() + " flags have been cleared.");
                }

                char value =  InterpreterManager
                              .commandSplit(params)[1].toLowerCase().charAt(0);

                if ( value =='0' || value=='f' ) {
                        if ( fl.contains(flagname) )
                                fl.remove(flagname);

                        return(target.getName() + " flag " + flagname + " is unset.");

                } else {
                        if ( !fl.contains(flagname) )
                                fl.add(flagname);

                        return(target.getName() + " flag " + flagname + " is set.");
                }
        }


        protected static String destroyObject(MudObject actor,
                                              MudObjectViewable target,
                                              String params) {
                return "destroyObject not yet implemented.";
        }


        public int compareTo(Object o) {
                if ( o instanceof Command ) {
                        return this.compareTo( (Command)o );
                } else {
                        return -1;
                }
        }


        public int compareTo(Command p) {
                return this.name.compareTo(p.getName());
        }


        public final boolean equals(Object o) {
                return(this == o);
        }

}
