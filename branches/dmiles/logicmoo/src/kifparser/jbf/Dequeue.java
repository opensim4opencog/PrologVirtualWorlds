/*
Copyright (c) 1996 Dennis Heimbigner
All rights reserved.

This software was developed as part of the Serl project
at the University of Colorado, Boulder.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
and other materials related to such
distribution and use acknowledge that the software was developed
by Dennis Heimbigner.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

package jbf;
import java.util.*;

// Head is element 0
// Tail is element length()-1

public
class Dequeue extends Vector {

    public
    Dequeue() {super();}

    public void clear() { setSize(0); }

    public int length() { return size(); }

    // actions on tail
    public void tpush(Object o) throws DequeueException
    {
	try {
	    addElement(o);
	} catch(Exception e) {throw new DequeueException("tpush");}
    }

    public Object tpop() throws DequeueException
    {
	Object o = lastElement();
	try {
	    removeElementAt(length() - 1);
	} catch(Exception e) {throw new DequeueException("tpop");}
	return o;
    }

    // actions on head
    public void hpush(Object o) throws DequeueException
    {
	try {
	    insertElementAt(o,0);
	} catch(Exception e) {throw new DequeueException("hpush");}
    }

    public Object hpop() throws DequeueException
    {
	Object o = firstElement();
	try {
	    removeElementAt(0);
	} catch(Exception e) {throw new DequeueException("tpop");}
	return o;
    }


    // return i'th element starting at the head as zero
    public Object ith(int index) throws DequeueException
    {
	try {
	    return elementAt(index);
	} catch(Exception e) {throw new DequeueException("Queue.enqueue");}
    }

    // return i'th element starting at the tail as zero
    public Object tth(int index) throws DequeueException
    {
	try {
	    return elementAt(size() - (index + 1));
	} catch(Exception e) {throw new DequeueException("Queue.enqueue");}
    }

    //replace i'th element starting at the head as zero
    public void setith(Object o, int index) throws DequeueException
    {
	try {
	    setElementAt(o,index);
	} catch(Exception e) {throw new DequeueException("Queue.enqueue");}
    }

    //replace i'th element starting at the tail as zero
    public void settth(Object o, int index) throws DequeueException
    {
	try {
	    setElementAt(o,size() - (index + 1));
	} catch(Exception e) {throw new DequeueException("Queue.enqueue");}
    }

};

