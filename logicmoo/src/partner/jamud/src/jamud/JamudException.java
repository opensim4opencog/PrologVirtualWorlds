package jamud;

public class JamudException extends Exception {

    public JamudException() {
	super();
    }
    
    public JamudException(String msg) {
	super(msg);
    }
    
    public JamudException(Exception e) {
	super(e.getMessage());
    }
    
}
