package jamud.util;

import java.io.*;
import java.util.LinkedList;


/**
 * Implementation of a blocking Queue (FIFO) of objects
 * @author siege
 */
public class Queue {

	private LinkedList ll;

	private boolean cleared = false;



	public Queue() {
		ll = new LinkedList();
	}


	/**
	 * Put an object into the queue.
	 */
	public synchronized void put(Object o) {
		ll.addFirst(o);
		cleared = false;
		notify();
	}


	/**
	 * Get, but do not remove, the oldest object in the queue
	 */
	public Object peek() {
		if(cleared)
			return null;
		else
			return ll.getLast();
	}


	/**
	 * Get and remove the oldest object in the queue.
	 */
	public synchronized Object get() {
		if(cleared)
			return null;

		else if(ll.size() > 0) {
			Object o = ll.getLast();
			ll.removeLast();
			return o;
		}

		else
			try {
				wait();
				return get();
			}
			catch(Exception e) {
				System.err.println("Exception at Queue.get()");
				System.err.println(e);
				return null;
			}
	}


	/**
	 * Number of objects in the queue
	 */
	public int size() {
		return ll.size();
	}


	/**
	 * Removes all objects from queue. If a thread was blocked in the <code>get</code
	 * or <code>peek</code> methods, they will return <code>null</code> and will
	 * continue to return <code>null</code> until an object is added to the queue.
	 */
	public synchronized void clear() {
		if(! cleared) {
			ll.clear();
			this.cleared = true;
			notify();
		}
	}


	/** Overridden from Object */
	public void finalize() throws Exception {
		if(! cleared) {
			ll.clear();
			this.cleared = true;
			notify();
		}
	}

}
