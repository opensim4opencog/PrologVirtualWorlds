package jamud;


public class StateException extends JamudException {


    private Initializable i;

    public Initializable getInitializable() {
	return this.i;
    }


    public StateException(Initializable cause, String text) {
	super(text);
	this.i = cause;
    }

}
