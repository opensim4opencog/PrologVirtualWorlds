package cycmoo.lang.builtin;  //
import cycmoo.lang.*;
import cycmoo.lang.object.*;

/**
  Always succeeds
*/
public class SuccesfullProceedure extends AtomProceedure {
    public SuccesfullProceedure() {
        super("true");
    }

    public int exec(IKernel p) {
        return 1;
    }
}
