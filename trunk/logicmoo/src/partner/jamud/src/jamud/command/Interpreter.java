package jamud.command;

import java.util.Iterator;

public interface Interpreter extends Comparable {


    public abstract String getName();


    public abstract int getPriority();


    public abstract boolean allows(Actor whom);


    public abstract Iterator commands();

}
