package jamud.object;

public class EntranceException extends Exception {


    private Entrance entrance;

    public Entrance getEntrance() {
	return this.entrance;
    }


    private MudObject object;

    public MudObject getMudObject() {
	return this.object;
    }



    public EntranceException(Entrance entrance, MudObject object) {
	super("Entrance Exception");
	this.entrance = entrance;
	this.object = object;
    }



    public void finalize() {
	this.entrance = null;
	this.object = null;
    }

}
