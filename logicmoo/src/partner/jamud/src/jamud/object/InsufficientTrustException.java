package jamud.object;

public class InsufficientTrustException extends Exception {

    private int trust;

    public int minimumTrust() {
	return this.trust;
    }


    public InsufficientTrustException(int minimumTrust) {
	super("Insufficient trust");
	this.trust = minimumTrust;
    }

}
