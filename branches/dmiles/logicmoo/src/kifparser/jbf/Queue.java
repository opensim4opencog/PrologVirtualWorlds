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

public
class Queue extends Dequeue {

    public Queue() {super();}

    public void enqueue(Object o) throws DequeueException
    {
	tpush(o);
    }

    public Object dequeue() throws DequeueException
    {
	return hpop();
    }

};
