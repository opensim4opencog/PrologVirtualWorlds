package cycmoo;


import java.util.*;
import cycmoo.obj.*;
import cycmoo.util.*;


/**
 * A class containing the methods used in new player creation.
 * This class doesn't need to ever be instantiated.
 *
 *@author Siege
 */
public abstract class Creation {

    /**
     * The main method to begin player creation.
     *
     */
    public static boolean creation(Player player) {

	//CAPITALIZE THE NAME (ie: "joeCool" -> "JoeCool")
	String name = player.getName();
	name = name.substring(0,1).toUpperCase() + name.substring(1);
	player.setShortName(name);

	try {

	    //OBTAIN PASSWORD
	    String password = promptPassword(player);

	    //OBTAIN EMAIL ADDY
	    String email = promptEmail(player);

	    //PROMPT FOR GENDER
	    Gender gender = promptGender(player);

	    //PROMPT FOR RACE
	    Race race = promptRace(player);

	    //PROMPT FOR JOB
	    Job job = promptJob(player);

	    //PROMPT FOR ALIGNMENT
	    //int align = promptAlign(player);

	    //PROMPT FOR ETHOS
	    //int ethos = promptEthos(player);

	    //ROLL FOR STATS
	    //Hashtable stats = promptStats(player);

	    //GAIN SKILLS & SPELLS
	    //Hashtable skills = promptSkills(player);

	    //CREATE BODY FOR PLAYER
	    System.out.println("creating player's body");
	    Body body = new Body();
	    body.setName(name);
	    body.setDesc("A Player");
	    body.setShortDesc(name + " is here.");
	    body.setKeywords(new FlagList(body.getMt(),body.getCycFort(),body.makeCycFort("mudKeyword"),null,name));
	    body.getStats().set(Body.STAT_LEVEL, 1);
	    body.setGender( gender );
	    body.setRace( race );
	    body.setJob( job );
	    body.setPlayer(player);
	    body.setRoom(Jamud.defaultRoom);
	    System.out.println("Jamud.defaultRoom = " + Jamud.defaultRoom);

	    //CREATE PLAYER SETTINGS
	    System.out.println("creating player's player");
	    player.putProperty(Player.PROPERTY_PASSWORD, password);
	    player.putProperty(Player.PROPERTY_EMAIL, email);
	    player.setTitle("The Player");
	    player.putProperty(Player.PROPERTY_PROMPT, Player.DEFAULT_PROMPT);
	    player.setTrust(1);
	    player.getFlags().add(Player.PROPERTY_FLAGS_COLOUR);
	    player.putProperty(Player.PROPERTY_NATIVE_BODY, body);
	    player.putProperty(Player.PROPERTY_CURRENT_BODY, body);
	    
	    System.out.println("creating player's location");
	    player.putProperty(Player.PROPERTY_LOCATION, "midga_0001");

	    player.setLoggedOn(true);
	    System.out.println("creating done.");
	    return true;
	} catch ( Exception e ) {
	    System.err.println(e);
	    return false;
	}
    }

    private static String promptPassword(Player src) throws Exception {
	//System.out.println(" prompting for password");
	String pass1 = "", pass2 = "";
	for ( boolean foo = true; foo; ) {

	    pass1 = src.prompt("New Password : {-").trim();
	    src.print("{+");

	    if ( pass1.length() < 1 ) {
		foo = false;
		throw new Exception("empty return, gave up?");
	    }

	    pass2 = src.prompt("Confirm Pass : {-").trim();
	    src.print("{+");

	    if ( pass1.equals(pass2) ) {
		pass2 = (new MD5(src.getName().toLowerCase() + pass2)).asHex();
		foo = false;
	    } else {
		src.print("\n Passwords Do Not Match.\n");
	    }
	}
	return pass2;
    }

    private static String promptEmail(Player src) throws Exception {
	//System.out.println(" prompting for email");
	String s = null;
	while ( s == null ) {
	    s = src.prompt("\nEmail address [leave blank for none] :");
	    if ( s == null ) s = "";
	    else s = s.trim();

	    if ( s.length() == 0 ) {
		return s;
	    } else if ( s.equalsIgnoreCase("none") ) {
		src.print("Look fool, I said leave BLANK for none, not 'type none'");
		s = null;
	    } else {
		//really basic check for validity

		String sp[] = Util.split(s, '@');
		String a = sp[0];
		sp = Util.split(sp[1], '.');

		if ( s.indexOf(" ") != -1 || a.length() < 3 || sp[0].length() < 3 || sp[1].length() < 3 ) {
		    src.print("Invalid email address.");
		    s = null;
		}
	    }

	}
	return s;
    }

    private static Gender promptGender(Player src) throws Exception {
	//System.out.println(" prompting for sex");
	/* That sounds dirty  -ed. */

	src.print("\nSelect a gender for this character");

	while ( true ) {

	    // Dynamically grab all the different genders
	    String s = "\n [";
	    for ( Enumeration enum = Jamud.getGenderManager().elements(); enum.hasMoreElements(); ) {
		Gender g = (Gender) enum.nextElement();
		s = s + g.getInitial().toUpperCase() + ", ";
	    }

	    s = src.prompt(s + "? for help] :");

	    if ( s.length() > 0 ) {
		char c = s.charAt(0);
		if ( c == '?' ) {
		    String prompt = "";
		    for ( Enumeration enum = Jamud.getGenderManager().elements(); enum.hasMoreElements(); ) {
			Gender g = (Gender) enum.nextElement();
			prompt += "\n " + g.getInitial().toUpperCase() + " = " + g.getName();
		    }
		    src.print( prompt );
		} else {
		    for ( Enumeration enum = Jamud.getGenderManager().elements(); enum.hasMoreElements(); ) {
			Gender g = (Gender) enum.nextElement();
			if ( g.getInitial().toLowerCase().equals( String.valueOf( c ).toLowerCase() ) )	return( g );
		    }
		}
	    }
	}
    }

    private static Race promptRace(Player src) {
	src.print("\nSelect a race for this character");

	while ( true ) {

	    String s = "";
	    for ( Enumeration enum = Jamud.getRaceManager().elements(); enum.hasMoreElements(); ) {
		Race r = (Race) enum.nextElement();
		s = s + "\n " + r.getName();
	    }

	    s = s + "\n? for help\n:";
	    s = src.prompt( s );

	    if ( s.length() > 0 ) {
		for ( Enumeration enum = Jamud.getRaceManager().elements(); enum.hasMoreElements(); ) {
		    Race r = (Race) enum.nextElement();
		    if ( r.getName().startsWith( s ) ) {
			return( r );
		    }
		}

	    }

	}
    }

    private static Job promptJob(Player src) {
	src.print("\nSelect a job for this character");

	while ( true ) {

	    String s = "";
	    for ( Enumeration enum = Jamud.getJobManager().elements(); enum.hasMoreElements(); ) {
		Job r = (Job) enum.nextElement();
		s = s + "\n " + r.getName();
	    }

	    s = s + "\n? for help\n:";
	    s = src.prompt( s );

	    if ( s.length() > 0 ) {
		for ( Enumeration enum = Jamud.getJobManager().elements(); enum.hasMoreElements(); ) {
		    Job r = (Job) enum.nextElement();
		    if ( r.getName().startsWith( s ) ) {
			return( r );
		    }
		}

	    }

	}
    }

    private static int promptAlign(Player src) {
	return 0;
    }

    private static int promptEthos(Player src) {
	return 0;
    }

    private static Hashtable promptStats(Player src) {
	return new Hashtable();
    }

    private static Hashtable promptSkills(Player src) {
	return new Hashtable();
    }

}
