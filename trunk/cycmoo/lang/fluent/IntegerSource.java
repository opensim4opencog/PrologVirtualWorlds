package cycmoo.lang.fluent;  
import cycmoo.lang.*;
import cycmoo.lang.object.*;

/**
 * creates a source of integers based on x=a*x+b formula
 */
public class IntegerSource extends SourceFluent {

    public IntegerSource(long fuel,long a,long x,long b,IKernel p) {
        super(p);
        this.fuel=fuel;
        this.a=a;
        this.b=b;
        this.x=x;
    }

    private long fuel;
    private long a;
    private long b;
    private long x;

    public ITerm getElement() {
        if ( fuel<=0 ) return null;
        IntTerm R=new IntTerm(x);
        x=a*x+b;
        --fuel;
        return R;
    }

    public void stop() {
        fuel=0;
    }

   // public String toString() {
   //     return "{(x->"+a+"*x+"+b+")["+fuel+"]="+x+"}";
    //}

}