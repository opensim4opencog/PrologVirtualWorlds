package com.jamud.commands.olc;


import jamud.command.*;
import jamud.object.*;
import jamud.util.*;
import jamud.JamudException;
import java.util.*;
import net.n3.nanoxml.*;
import java.lang.reflect.Method;
import java.lang.reflect.*;
import java.lang.*;


public class BuilderCommandUtility {

    
    /***********************************************************
     *  Searching
     *
     **********************************************************/

    public static MudObjectViewable getPeerObject(MudObject s,
						  String keyword) {

	
        int len = keyword.length();
	
	if ( len==0 )
	    return null;
	
	int ampersand = keyword.indexOf('@');
	
	try {
	    switch ( ampersand ) {
		
	    case 0:
		// is @LID
		return
		    findByLID(s.getParentContainer(),Integer.parseInt(keyword.substring(1)));
	    case -1:
		// is name
		return
		    findByName( s,  keyword);
	    default:
		if ( len==ampersand ) {
		    // is TID@
		    return findByTID(s.getParentContainer(),keyword.substring(0,len-1));
		} else {
		    // is TID@LID
		    return findByLID(s.getParentContainer(),Integer.parseInt(keyword.substring(ampersand+1)));
		}
	    }
	} catch ( NumberFormatException e ) {
	    return null;
	}
    }
    
    
    
    public static MudObjectViewable findByLID(MudObjectContainer room, int lid) {
	if ( room == null ) {
	    return null;
	}
	
	for ( Iterator i = room.childObjects(); i.hasNext(); ) {
	    MudObjectViewable m = (MudObjectViewable) i.next();
	    if ( m instanceof MudObject ) {
		if ( ((MudObject)m).getLoadId()==lid ) return m;
	    } else {
		if ( m instanceof MudObjectContainer ) {
		    if ( ((MudObjectContainer)m).getLoadId()==lid ) return m;
		    MudObjectViewable pos = findByLID((MudObjectContainer)m,lid);
		    if ( pos!=null ) return pos;
		}
	    }
	}
	return null;
    }
    
    
    public static  MudObjectViewable findByTID(MudObjectContainer room, String tid) {
	if ( room == null ) {
	    return null;
	}
	
	for ( Iterator i = room.childObjects(); i.hasNext(); ) {
	    MudObjectViewable m = (MudObjectViewable) i.next();
	    if ( m instanceof MudObject ) {
		if ( ((MudObject)m).isTemplate() && ((MudObject)m).getTemplateId().equalsIgnoreCase(tid) )
		    return m;
	    } else {
		if ( m instanceof MudObjectContainer ) {
		    if ( ((MudObjectContainer)m).isTemplate() && ((MudObjectContainer)m).getTemplateId().equalsIgnoreCase(tid) ) return m;
		    MudObjectViewable pos = findByTID((MudObjectContainer)m,tid);
		    if ( pos!=null ) return pos;
		}
	    }
	}
	return null;
	
    }
    
    public static  MudObjectViewable findByName(MudObject s, String keyword) {
	
	MudObjectContainer  room = s.getParentContainer();
	if ( room == null ) {
	    return null;
	}
	
	for ( Iterator i = room.childObjects(keyword); i.hasNext(); ) {
	    MudObject m = (MudObject) i.next();
	    if ( m != s && m.matchKeyword(keyword) ) {
		return m;
	    }
	}
	
	// TODO fix loop bug ;>
	if ( keyword.equalsIgnoreCase("this") && s.attributes().containsKey(BuilderCommand.PROPERTY_OBJECTSCOPE) )
	    return findByName(s,s.attributes().getAttribute(BuilderCommand.PROPERTY_OBJECTSCOPE,null));
	
	if ( keyword.equalsIgnoreCase("room") ||
	     keyword.equalsIgnoreCase("parent") )
	    return s.getParentContainer();
	
	if ( keyword.equalsIgnoreCase("self") ||
	     keyword.equalsIgnoreCase("me") )
	    return s;
	
	return null;
    }
    
    
    /***********************************************************
     *  Creation
     *
     **********************************************************/
    
    // Create By TypeName
    public static Object createObjectFromType(MudObjectContainer room, String type, String name)
        throws JamudException {
	
	if ( type.equals("room") )
	    return createRoomObject(room.parentObject(), name);
	
	if ( name==null || name.length()<1 )
	    name = "new toy in " + room.getName();
	
	MudObject newObj = new MudObject();
	ensureUnique(newObj);
	room.addChildObject(newObj);
	newObj.setName(name);
	newObj.setShortDescription(name);
	FlagList fl = new FlagList();
	fl.add(name);
	newObj.setKeywords(fl);
	return newObj;
    }
    
    // Create By TID
    public static Object createObjectFromTID(MudObjectContainer room,String tid, String name)
        throws JamudException {
	
	
	MudObjectViewable example = findByTID(jamud.Jamud.currentInstance().mudObjectRoot(),tid);
	return createObjectFromExample( room, example,  name) ;
	
    }
    // Create By Example
    public static Object createObjectFromExample(MudObjectContainer room,MudObjectViewable example, String name)
        throws JamudException {
	
	if ( name==null || name.length()<1 )
	    name = "new toy from " + example.getName();
	
	Object newObj = null;
	
	if ( example instanceof MudObjectContainer ) {
	    newObj = new MudObjectContainer(room.parentObject());
	    ((MudObjectContainer)newObj).copycat(example);
	    
	} else if ( example instanceof MudObject ) {
	    newObj = new MudObject();
	    ((MudObject)newObj).copycat(example);
	    room.addChildObject((MudObject)newObj);
	} else {
	    throw new JamudException("example " + example + " is not available for cloning");
	}
	
	ensureUnique((MudObjectViewable)newObj);
	((MudObjectViewable)newObj).setName(name);
	((MudObjectViewable)newObj).setShortDescription(name);
	FlagList fl = new FlagList();
	fl.add(name);
	((MudObjectViewable)newObj).setKeywords(fl);
	return newObj;
	
	
    }
    
    
    public static Object createRoomObject(MudObject parent,  String roomName)
        throws JamudException {
	
	if ( roomName==null || roomName.length()<1 )
	    roomName = "new room in " + parent.getName();
	
	MudObjectContainer newRoom = new MudObjectContainer(parent);
	ensureUnique(newRoom);
	newRoom.setName(roomName);
	newRoom.setShortDescription(roomName);
	FlagList fl = new FlagList();
	fl.add(roomName);
	newRoom.setKeywords(fl);
	return newRoom;
    }
    
    private static void ensureUnique(MudObjectViewable newThing) {
	if ( newThing instanceof MudObject ) {
	    ((MudObject)newThing).nextLoadId();
	} else if ( newThing instanceof MudObjectContainer ) {
	    // TODO: ((MudObjectContainer)newThing).setTemplateId(...);
	    // ((MudObjectContainer)newThing).setTemplateId(incrementTID(((MudObjectContainer)newThing).getTemplateId()));
	    
	}
    }
    
    public static String incrementTID(String tid) {
	int sep = tid.lastIndexOf('-');
	if (sep<0)
	    return tid + "-1";
	else{
	    try {
		return tid.substring(0,sep-1) + "-" + Integer.parseInt(tid.substring(sep+1));
	    } catch (Exception e) {
		return tid + "-1";
	    }
	}
    }
    
    /***********************************************************
     *  Directionals
     *
     **********************************************************/
    public static Entrance getNewDirectionalEntrance(String clue) {
	switch ( clue.toLowerCase().charAt(0) ) {
	case 'u':
	    return new Entrance.Up();
	case 'd':
	    return new Entrance.Down();
	case 'n':
	    return new Entrance.North();
	case 's':
	    return new Entrance.South();
	case 'e':
	    return new Entrance.East();
	case 'w':
	    return new Entrance.West();
	}
	return new Entrance.Down();
    }
    
    public static String reverseDirection(String clue) {
	switch ( clue.toLowerCase().charAt(0) ) {
	case 'u':
	    return "d";
	case 'd':
	    return "u";
	case 'n':
	    return "s";
	case 's':
	    return "n";
	case 'e':
	    return "w";
	case 'w':
	    return "e";
	}
	return "u";
    }
    
    /***********************************************************
     *  Doors and Exits
     *
     **********************************************************/
    
    public static Entrance createExit(MudObjectContainer srcRoom,MudObjectContainer targetRoom, String direction , String nameForExit)
        throws JamudException {
	
	if ( srcRoom.getExit(direction)!=null ) {
	    throw new JamudException("There is already an exit " + direction + " in container " + srcRoom.getName());
	}
	
	Entrance newEntrance = getNewDirectionalEntrance(direction);
	newEntrance.setName(nameForExit);
	newEntrance.setDestination(targetRoom);
	newEntrance.setDestinationTid(targetRoom.getTemplateId());
	srcRoom.addExit(newEntrance);
	return newEntrance;
    }
    
    public static String getDirection(Entrance newEntrance ) {
	//return ((DefaultEntrance)newEntrance).defaultName();
	if ( newEntrance instanceof Entrance.North ) return "North";
	if ( newEntrance instanceof Entrance.South ) return "South";
	if ( newEntrance instanceof Entrance.East ) return "East";
	if ( newEntrance instanceof Entrance.West ) return "West";
	if ( newEntrance instanceof Entrance.Up ) return "Up";
	if ( newEntrance instanceof Entrance.Down ) return "Down";
	return "Unknown";
    }
    
    /***********************************************************
     * Attribute Editing
     *
     **********************************************************/
    
    public static boolean addProperty(MudObjectViewable target, String property)
        throws JamudException {
	return addProperty( target,  property,  "" );
    }
    
    public static boolean addProperty(MudObjectViewable target, String property, Object value)
        throws JamudException {
	return setProperty(target,  property , value);
    }
    
    public static boolean setProperty(Object target, String property, Object value)
        throws JamudException {
	
	if ( attemptJavaSetMethod(target,property,value) )
	    return true;
	
	if ( target instanceof MudObject ) {
	    ((MudObject)target).attributes().setProperty(property,value.toString());
	    return true;
	}
	
	throw new JamudException("The type " + target.getClass().getName() + " did not have the interface to add the property " +value );
	
    }
    
    public static boolean unsetProperty(MudObjectViewable target, String property)
        throws JamudException {
	return removeProperty(  target,  property);
    }
    
    public static boolean removeProperty(MudObjectViewable target, String property)
        throws JamudException {
	if ( property==null || property.length()<1 )
	    throw new JamudException("removeProperty: no property name ");
	
	if ( target instanceof MudObject ) {
	    ((MudObject)target).attributes().remove(property) ;
	    return true;
	}
	throw new JamudException("The type " + target.getClass().getName() + " did not have the interface to remove the property " +property );
	
    }
    
    
    /***********************************************************
     * Java Editing
     *
     **********************************************************/
    
    
    public static String findNamedProperty(Object target, String property) {
	// Normally more deeper analysis then string concatination (that is why the target object is here)
	return "set" + toProperCase(property);
    }
    
    public static boolean attemptJavaSetMethod(Object target,String property,Object value) {
	if ( value==null ) {
	    try {
		target.getClass().getMethod(findNamedProperty(target,property),null).invoke(target,null);
		return true;
	    } catch ( Exception e ) {
		return false;
	    }
	    
	} else {
	    
	    Class[] sc = { value.getClass()};
	    Object[] sa = { value};
	    
	    try {
		target.getClass().getMethod(findNamedProperty(target,property),sc).invoke(target,sa);
		return true;
	    } catch ( Exception e ) {
		return false;
	    }
	}
    }
    
    public static Method findJavaMethod(Class cls, String methodname)
        throws NoSuchMethodError {
	Method[] meths = cls.getMethods();
	int len = meths.length;
	for ( int i = 0; i < len ; i++ )
	    if ( meths[i].getName().equals(methodname) ) return meths[i];
	
	throw new NoSuchMethodError(methodname +" for " + cls.getName());
    }
    
    /***********************************************************
     *  Flag Editing
     *
     **********************************************************/
    
    public static boolean flaglistEdit(MudObjectViewable target, String flagname , boolean value )
        throws JamudException {
	if ( flagname==null || flagname.length()<1 )
	    throw new JamudException("removeProperty: no property name ");
	FlagList fl = ((MudObject)target).flags();
	if ( value ) {
	    if ( !fl.contains(flagname) ) fl.add(flagname);
	    return true;
	} else {
	    if ( fl.contains(flagname) ) fl.remove(flagname);
	    return true;
	}
    }
    
    public static boolean destroyObject(MudObjectViewable target)
        throws JamudException {
	throw new JamudException("destroyObject: not immplemented ");
    }
    
    /***********************************************************
     * Java Typing
     *
     **********************************************************/
    
    
    public static Object mktype(String theType,String theData)
        throws JamudException {
	try {
	    if ( theType.equals("Long") )
		return new java.lang.Long(theData);
	    if ( theType.equals("Integer") )
		return new java.lang.Integer(theData);
	    if ( theType.equals("Short") )
		return new Short(theData);
	    if ( theType.equals("Float") )
		return new java.lang.Float(theData);
	    if ( theType.equals("Byte") )
		return new Byte(theData);
	    if ( theType.equals("Boolean") )
		return new Boolean(theData);
	    if ( theType.equals("Char") || theType.equals("Character") )
		return new Character(theData.charAt(0));
	    if ( theType.equals("Date") )
		return new MudDate(Long.parseLong(theData));
	    if ( theType.equals("Class") )
		return Class.forName(theData);
	    if ( theType.equals("String") ) return theData;
	    
	    return makeInstanceFromClass( theType, theData);
	    
	} catch ( Exception e ) {
	    throw new JamudException("Cannot construct "+ theType +" from \"" + theData + "\" (" + e + ")" );
	}
    }
    
    public static Object makeInstanceFromClass(String theType,Object value)
        throws Exception {
	if ( value==null ) {
	    Class newClass = Class.forName(theType);        // Can Throw ClassNotFound
	    return newClass.newInstance();        // Can Throw Instanciation
	} else {
	    Class newClass = Class.forName(theType);
	    Class[] sc = { value.getClass()};
	    Object[] sa = { value};
	    
	    try {
		return newClass.getConstructor(sc).newInstance(sa);        //  throw == second chance
	    } catch ( Exception e ) {
		// Second Chance
		return newClass.newInstance();        // Can Throw Instanciation
	    }
	}
    }
    
    /***********************************************************
     * Strings
     *
     **********************************************************/
    
    public static String toProperCase(String property) {
	return property.substring(0,0).toUpperCase()+property.substring(1).toLowerCase();
    }
    
    
    /***********************************************************
     *  Messaging
     *
     **********************************************************/
    
    public static void inform(MudObject actor, String msg) {
	if ( ((Object)actor) instanceof Player ) {
	    actor.println(msg);
	} else {
	    System.out.println("  BuilderCommand: tells object " + actor.getName() + " \"" + msg + "\"");
	}
    }
    
   
}
