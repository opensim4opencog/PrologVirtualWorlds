package cycmoo.lang;  //tarau.jinni;
import java.io.Reader;
import java.io.StringReader;
import java.io.IOException;

/**
  Builds Jinni Fluents from Java
  Streams
*/
public class CharReader extends SourceFluent {
    public Reader reader;

    public CharReader(Reader reader,IKernel p) {
        super(p);
        this.reader=reader;
    }

    public CharReader(String f,IKernel p) {
        super(p);
        makeReader(f);
    }

    public CharReader(ITerm t,IKernel p) {
        super(p);
        this.reader=new StringReader(t.toUnquoted());
    }

    public CharReader(IKernel p) {
        this(IO.input,p);
    }

    public void makeReader(String f) {
        this.reader=IO.url_or_file(f);
    }

    public ITerm getElement() {
        if ( IO.input==reader ) {
            String s=IO.promptln(">:");
            if ( null==s||s.length()==0 ) return null;
            return parent.getBlackboard().useAtom(s);
        }

        if ( null==reader ) return null;
        int c=-1;
        try {
            c=reader.read();
        } catch ( IOException e ) {
        }
        if ( -1==c ) {
            stop();
            return null;
        } else
            return new IntTerm(c);
    }

    public void stop() {
        if ( null!=reader && IO.input!=reader ) {
            try {
                reader.close();
            } catch ( IOException e ) {
            }
            reader=null;
        }
    }
}
