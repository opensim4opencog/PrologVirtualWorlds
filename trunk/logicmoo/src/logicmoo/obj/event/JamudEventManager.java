package logicmoo.obj.event;


import logicmoo.obj.*;
import java.util.*;

public class JamudEventManager {

	private Hashtable objToListeners = null;



	public JamudEventManager() {
		//empty
	}



	public void addListener(JamudEventListener listener, int event, JamudEventTrigger target) {
		if(objToListeners == null)
			synchronized(this) {
				if(objToListeners == null)
					objToListeners = new Hashtable();
			}
	
		Vector v = (Vector) objToListeners.get(target);
		if(v == null)
			synchronized(objToListeners) {
				v = (Vector) objToListeners.get(target);
				if(v == null) {
					v = new Vector();
					objToListeners.put(target, v);
				}
			}

		EventNotif en = new EventNotif(listener, event);

		synchronized(v) {
			if(! v.contains(en))
				v.add(en);
		}
	}


	public void removeListener(JamudEventListener listener, int event, JamudEventTrigger target) {
		if(objToListeners != null) {
			Vector v = (Vector) objToListeners.get(target);
			if(v != null) {
				v.remove(new EventNotif(listener, event));
				if(v.size() == 0)
					objToListeners.remove(target);
			}
		}
	}


	public void removeAllListeners(JamudEventTrigger target) {
		if(objToListeners != null) {
			objToListeners.remove(target);
		}
	}


	public void triggerListeners(JamudEvent jEvent) {
		if(objToListeners != null) {
			Vector v = (Vector) objToListeners.get(jEvent.getSource());
			if(v != null && v.size() > 0)
				for(Enumeration enum = v.elements(); enum.hasMoreElements(); ) {
					EventNotif en = (EventNotif) enum.nextElement();
					if(en.event == jEvent.getEvent())
						en.listener.eventTriggered(jEvent);
					
				}
		}
	}


	class EventNotif {
		protected JamudEventListener listener;
		protected int event;

		public EventNotif(JamudEventListener listener, int event) {
			this.listener = listener;
			this.event = event;
		}

		public boolean equals(Object o) {
			if(o instanceof EventNotif) {
				EventNotif j = (EventNotif)o;
				o = null;
				return (j.listener == this.listener && j.event == this.event);
			}
			else
				return ((Object)this == o);
		}
		public void finalize() {
			listener = null;
		}
	}

}
