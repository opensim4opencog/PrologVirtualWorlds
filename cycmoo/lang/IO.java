package cycmoo.lang;  //
import cycmoo.lang.fluent.*;
import cycmoo.lang.builtin.*;  //
import cycmoo.lang.object.*;  //
/*
* Copyright (C) Paul Tarau 1996-1999*/

import java.io.*;
import java.net.*;
import java.applet.*;
import java.util.*;

public class IO {

    static public IOPeer peer=null;
    static public Applet applet=null;

    static public boolean showOutput=true;
    static public boolean showErrors=true;
    final static public int showTrace=0;
    static public long maxAnswers=0; // 0 means all, >0 means ask

    static public final Reader input=toReader(System.in);
    static public Writer output=toWriter(System.out);

    static public Reader toReader(InputStream f) {
        return new BufferedReader(new InputStreamReader(f));
    }

    static public Reader toFileReader(String fname) {
        return url_or_file(fname);
    }

    static public Writer toWriter(OutputStream f) {
        return new BufferedWriter(new OutputStreamWriter(f));
    }

    static public Writer toFileWriter(String s) {
        Writer f=null;
        //mes("HERE"+s);
        try {
            f=toWriter(new FileOutputStream(s));
        } catch ( IOException e ) {
            errmes("write error, to: "+s);
        }
        return f;
    }

    static public Reader getStdInput() {
        return input;
    }

    static public Writer getStdOutput() {
        return output;
    }

    synchronized static public final void print(Writer f,String s) {
        if ( !showOutput ) return;
        if ( peer==null ) {
            try {
                f.write(s);f.flush();
            } catch ( IOException e ) {
                System.err.println("*** error in printing: "+e);
            }
        } else
            peer.print(s);
        return;
    }

    static public final void println(Writer o,String s) {
        print(o,s+"\n");
    }

    static public final void print(String s) {
        print(getStdOutput(),s);
    }

    static public final void println(String s) {
        println(getStdOutput(),s);
    }
    
    static public final void println(Iterator s) {
        while (s.hasNext()) println(s.next().toString());
    }
    static public final void println(Enumeration s) {
        while (s.hasMoreElements()) println(s.nextElement().toString());
    }

    // for now just stubs: usable if IO comes from elswere i.e. sockets
    static public final String read_from(Reader f) {
        return readln(f);
    }

    // for now just stubs: usable if IO comes from elswere i.e. sockets
    static public final void write_to(Writer f,String s) {
        println(f,s);
    }

    static public final int MAXBUF=1<<30;

    static public String readLine(Reader f) throws IOException {
        StringBuffer s=new StringBuffer();
        for ( int i=0; i<MAXBUF; i++ ) {
            int c=f.read();
            if ( c=='\0'||c=='\n'||c==-1||
                 (c=='\r'&&'\n'==f.read())
               ) {
                if ( i==0 && c==-1 ) return null;
                break;
            }
            s.append(c);
        }
        return s.toString();
    }

    static public final String readln(Reader f) {
        traceln(2,"READLN TRACE: entering");
        String s=null;
        try {
            if ( f instanceof BufferedReader ) {
                s=((BufferedReader)f).readLine();
            } else
                s=readLine(f);
        } catch ( IOException e ) {
            errmes("error in readln: e.toString()");
        }
        traceln(2,"READLN TRACE:"+"<"+s+">");
        return s;
    }

    static public final String readln() {
        String s;
        if ( peer==null ) {
            s=readln(getStdInput());
        } else {
            s=peer.readln();
        }
        return s;
    }

    static public final String promptln(String prompt) {
        print(prompt);
        return readln();
    }

    static public final void mes(String s) {
        println(getStdOutput(),s);
    }

    static public final void traceln(int level,String s) {
        if ( !showOutput || showTrace<level ) return;
        if ( peer==null ) {
            println(getStdOutput(),s);
        } else
            peer.traceln(s);
    }

    static public final void traceln(String s) {
        if ( showTrace>=1 ) println(getStdOutput(),s);
    }

    static public void printStackTrace(Throwable e) {
        if ( showErrors ) {
            //ByteArrayOutputStream b=new ByteArrayOutputStream();
            //PrintWriter fb=new PrintWriter(b);   
            CharArrayWriter b=new CharArrayWriter();
            PrintWriter fb=new PrintWriter(b);
            e.printStackTrace(fb);
            IO.errmes(b.toString());
            fb.close();
        }
    }

    static public final void errmes(String s) {
        if ( showErrors ) println(getStdOutput(),s);
    }

    synchronized static public final void errmes(String s,Throwable e) {
        errmes(s);
        printStackTrace(e);
    }


    static public final void assertion(String Mes) {
        IO.errmes("assertion failed",(new java.lang.Exception(Mes)));
    }

    static public final int system(String cmd) {
        //IO.mes("executing: <"+cmd+">");
        try {
            Runtime.getRuntime().exec(cmd);
        } catch ( IOException e ) {
            IO.errmes("error in system cmd: "+cmd,e);
            return 0;
        }
        return 1;
    }

    static public final Reader url2stream(String f) {
        return url2stream(f,false);
    }

    static public final Reader url2stream(String f,boolean quiet) {
        Reader stream=null;
        try {
            URL url=new URL(f);
            stream=toReader(url.openStream());
        } catch ( MalformedURLException e ) {
            if ( quiet ) return null;
            IO.errmes("bad URL: "+f,e);
        } catch ( IOException e ) {
            if ( quiet ) return null;
            IO.errmes("unable to read URL: "+f,e);
        }

        return stream;
    }

    static public String getBaseDir() {
        if ( null == applet ) return "";
        String appletURL=applet.getCodeBase().toString();
        int slash=appletURL.lastIndexOf('/');
        if ( slash>=0 ) appletURL=appletURL.substring(0,slash+1);
        //IO.mes("APPLET can load files from: "+baseDir+"\n");
        return appletURL;
    }

    static public final Reader url_or_file(String s) {
        Reader stream=null;
        try {
            if ( applet!=null ) {
                String baseDir=getBaseDir();
                //mes("$$ baseDir: "+baseDir);
                stream=url2stream(baseDir+s,true);
            }

            if ( null==stream )
                stream=url2stream(s,true);

            if ( null==stream && null==IO.applet )
                stream=toReader(new FileInputStream(s));
        } catch ( IOException e ) {
        }
        return stream;
    }

    static public final Reader string_to_stream(String s) throws IOException {
        StringReader stream=new StringReader(s);
        return stream;
    }

    static public final URL find_url(String s) {
        String valid=null;
        Reader stream;

        String baseDir=getBaseDir();
        valid=baseDir+s;
        stream=url2stream(valid,true);

        if ( null==stream ) {
            valid=s;
            stream=url2stream(valid,true);
        }
        try {
            stream.close();
        } catch ( IOException e ) {
            valid=null;
        }

        URL url=null;

        try {
            url=new URL(valid);
        } catch ( MalformedURLException e ) {
        }

        return url;
    }
}
