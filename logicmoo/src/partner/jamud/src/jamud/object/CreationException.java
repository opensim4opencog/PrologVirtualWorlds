package jamud.object;

import jamud.JamudException;

public class CreationException extends JamudException {

    public CreationException(String message) {
	super( message );
    }

    public CreationException(Exception e) {
	super( e );
    }

}
