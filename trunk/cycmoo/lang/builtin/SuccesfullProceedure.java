package cycmoo.lang;  //tarau.jinni;

/**
  Always succeeds
*/
public class SuccesfullProceedure extends AtomProceedure {
    SuccesfullProceedure() {
        super("true");
    }

    public int exec(IKernel p) {
        return 1;
    }
}
