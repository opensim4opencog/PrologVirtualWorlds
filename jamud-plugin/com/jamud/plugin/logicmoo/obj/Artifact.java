package  logicmoo.obj;


import java.util.Enumeration;
import logicmoo.Jamud;
import logicmoo.obj.event.*;
import logicmoo.util.*;
import net.n3.nanoxml.*; import org.opencyc.cycobject.*; import logicmoo.*;


public class Artifact extends PartiallyTangible implements Flagged, GuidTracable, JamudEventTrigger {

    public static final String MARKUP = "ARTIFACT";

    private static final String
    PROPERTY_MICROTHEORY = "MICROTHEORY",
    PROPERTY_TYPE = "TYPE",
    PROPERTY_STATLIST = "STATLIST",
    PROPERTY_WEARLOCATIONS = "WEARLOCATIONS";

    public static final String
    FLAG_ANTI_EVIL = "ANTI_EVIL",
    FLAG_ANTI_GOOD = "ANTI_GOOD",
    FLAG_ANTI_NEUT = "ANTI_NEUT",
    FLAG_CLOTH = "CLOTH",
    FLAG_CONTAIN = "CONTAIN",
    FLAG_DRINK = "DRINK",
    FLAG_EAT = "EAT",
    FLAG_ENTER = "ENTER",
    FLAG_EVIL = "EVIL",
    FLAG_FLAME = "FLAME",
    FLAG_FLOAT = "FLOAT",
    FLAG_FLY = "FLY",
    FLAG_GLOW = "GLOW",
    FLAG_GOOD = "GOOD",
    FLAG_HIDE = "HIDE",
    FLAG_HUMM = "HUMM",
    FLAG_ICE = "ICE",
    FLAG_INVIS = "INVIS",
    FLAG_KEY = "KEY",
    FLAG_MAGIC = "MAGIC",
    FLAG_MELT_DROP = "MELT_DROP",
    FLAG_METAL = "METAL",
    FLAG_NEUT = "NEUT",
    FLAG_NO_DROP = "NO_DROP",
    FLAG_NO_REMOVE = "NO_REMOVE",
    FLAG_NULLIFY = "NULLIFY",
    FLAG_REST_AT = "REST_AT",
    FLAG_REST_IN = "REST_IN",
    FLAG_REST_ON = "REST_ON",
    FLAG_RIDE_IN = "RIDE_IN",
    FLAG_RIDE_ON = "RIDE_ON",
    FLAG_SHOCK = "SHOCK",
    FLAG_SIT_AT = "SIT_AT",
    FLAG_SIT_IN = "SIT_IN",
    FLAG_SIT_ON = "SIT_ON",
    FLAG_SLEEP_AT = "SLEEP_AT",
    FLAG_SLEEP_IN = "SLEEP_IN",
    FLAG_SLEEP_ON = "SLEEP_ON",
    FLAG_TAKE = "TAKE",
    FLAG_VORPAL = "VORPAL",
    FLAG_WEAR = "WEAR",
    FLAG_WOOD = "WOOD";


    private FlagList wear = new FlagList();
    public FlagList getWearLocations() { return this.wear;}
    public void setWearLocations(FlagList wear) { this.wear = wear;}

    public boolean isWearableOn(String location) { return this.wear.contains(location);}

    private Group contained;
    public Group getArtifacts() { return contained;}




    private Artifact() {
	//empty
    }

    public Artifact(CycFort cycFort) {
	this.cycFort = cycFort;
    }


    private GeographicalRegion room;
    public GeographicalRegion getGeographicalRegion() { return room;}
    public void setGeographicalRegion(GeographicalRegion room) { this.room = room;}


    public void tick() {
	//trigger our onTick
	triggerOnTick();

	//trigger contained Artifact ticks...
	if ( contained != null )
	    for ( Enumeration enum = contained.elements(); enum.hasMoreElements(); )
		((Artifact)enum.nextElement()).tick();

    }


    public void triggerOnTick() {
	Jamud.getEventManager().triggerListeners(new ArtifactEvent(this, JamudEvent.EVENT_TICK, new Object[0]));
    }


    public Artifact getSemiClone() {
	Artifact i = new Artifact();

	//PartiallyTangible
	i.setName(getName());
	i.setComment(getComment());
	i.setPrettyName(getPrettyName());
	i.setKeywords(new FlagList(getKeywords()));

	//GuidTracable
	i.microtheory = microtheory;
	i.cycFort = cycFort;

	//Flagged
	i.flags = new FlagList(flags);

	i.wear = new FlagList(wear);

	return i;
    }


}
