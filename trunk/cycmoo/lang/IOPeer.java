package cycmoo.lang;  //
import cycmoo.lang.fluent.*;
import cycmoo.lang.object.*;
import cycmoo.lang.builtin.*;  //
/*
* Copyright (C) Paul Tarau 1996-1999*/

public interface IOPeer {

    public abstract void print(String s);

    public abstract void traceln(String s);

    public abstract void println(String s);

    //public abstract boolean addReader(Thread readThread);

    //public abstract String getReadString();

    public abstract String readln();

    public abstract void halt();
}