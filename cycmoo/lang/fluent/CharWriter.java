package cycmoo.lang;  //tarau.jinni;
import java.io.Writer;
import java.io.FileOutputStream;
import java.io.IOException;

/**
  Writer
*/
public class CharWriter extends SinkFluent {
    public Writer writer;

    public CharWriter(String f,IKernel p) {
        super(p);
        this.writer=IO.toFileWriter(f);
    }

    public CharWriter(IKernel p) {
        super(p);
        this.writer=IO.output;
    }

    public int putElement(ITerm t) {
        if ( null==writer ) return 0;
        try {
            char c=(char)((IntTerm)t).intValue();
            writer.write(c);
        } catch ( IOException e ) {
            return 0;
        }
        return 1;
    }

    public void stop() {
        if ( null!=writer && IO.output!=writer ) {
            try {
                writer.close();
            } catch ( IOException e ) {
            }
            writer=null;
        }
    }

}
