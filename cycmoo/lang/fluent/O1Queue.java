package cycmoo.lang;  

import java.util.Vector;
import java.util.Enumeration;

/**
  Generic dynamic Queeue with (amortized) O(1)
  enq/deq (add and remove) operations
*/
public class O1Queue extends PersistentFluent implements Cloneable {
    final static int MIN_QUEUE=4;
    final static int MAX_QUEUE=1<<24;

    private int head,tail;
    public boolean busy;
    private Object queue[];

    public O1Queue(int size) {
        makeIt(size);
    }

    public O1Queue() {
        this(0);
    }

    public O1Queue(Vector V) {
        this(V.size()+MIN_QUEUE);
        for ( int i=0;i<V.size();i++ ) {
            enq(V.elementAt(i));
        }
    }

    synchronized final private void makeIt(int size) {
        size=(size<MIN_QUEUE)?MIN_QUEUE:size;
        queue = new Object[size];
        head = tail = 0;
        busy=false;
    }

    final private int count() {
        return(head<=tail)?tail-head:queue.length-head+tail;
    }

    /**
      Dynamically resizes the queue
    */
    final public boolean requeue(String Mes) {
        int newSize=2*count();
        if ( newSize>MAX_QUEUE || newSize<MIN_QUEUE ) return false;
        //KernelAgent.masterio.traceln(Mes+"!!! "+toString());
        Object[] nqueue=new Object[newSize];
        int j=0;
        for ( int i=head; i!=tail; i=inc(i) )
            nqueue[j++]=queue[i];
        queue=nqueue;
        head=0;
        tail=j;
        return true;
    }

    final private void enterCritical() {
        while ( busy ) {
            try {
                wait();
            } catch ( InterruptedException e ) {
            }
        }
        busy=true;
    }

    final private void exitCritical() {
        busy=false;
        notifyAll();
    }

    public int putElement(ITerm T) {
        return enq(T)?1:0;
    }
    public ITerm getElement() {
        return (ITerm)deq();
    }

    /**
      Adds an element to the end of the queue
    */
    synchronized public final boolean enq(Object V) {
        enterCritical();
        if ( inc(tail) == head ) { // full !!!
            if ( !requeue("expanding") ) {
                //KernelAgent.masterio.errmes("Warning: queue overflow at:"+V);
                return false;
            }
        }
        queue[tail] = V;
        tail=inc(tail);
        exitCritical();
        return true;
    }

    /**
      Removes the first element of the queue 
    */
    synchronized public final Object deq() {
        enterCritical();
        if ( tail == head ) // empty !!!
            return null;
        if ( 4*count()<queue.length )
            requeue("shrinking");
        Object V = queue[head];
        head  = inc(head);
        exitCritical();
        return V;
    }

    final private int inc(int val) {
        return(val + 1) % queue.length; 
    }

    synchronized public final boolean isEmpty() {
        boolean empty;
        enterCritical();
        empty=tail == head;
        exitCritical();
        return empty;
    }

    synchronized public Vector toVector() {
        //enterCritical(); DEADLOCKS!
        Vector v=new Vector();
        for ( int i=head; i!=tail; i=inc(i) )
            v.addElement(queue[i]);
        //exitCritical();
        return v;
    }

    public Enumeration elements() {
        return toVector().elements();
    }

    public Enumeration toEnumeration() {
        return toVector().elements();
    }

    //public String toString() {
    //    return count()+"/"+queue.length+"=>"+toVector().toString();
    //}

    synchronized public O1Queue toClone() {
        O1Queue R=null;
        try {
            R=(O1Queue)clone();
        } catch ( CloneNotSupportedException e ) {
            //KernelAgent.masterio.errmes("O1Queue:toClone() "+e);
        }
        return R;
    }

}


/*
public class QueueIterator {
  QueueIterator(O1Queue Q) {
    this.Q=Q.toClone();
  }
   
  private O1Queue Q;

  Object getNext() {
    return Q.deq();
  }   
}
*/