package logicmoo.obj.event;

import logicmoo.obj.*;

public abstract class JamudEvent {

	public static final int
		EVENT_TICK = 0;

	public static final String
		EVENT_NAME_TICK = "onTick";

	private JamudEventTrigger src;
	private int event;
	private Object[] info;



	public JamudEvent(JamudEventTrigger source, int event, Object[] info) {
		this.src = source;
		this.event = event;
		this.info = info;
	}



	public JamudEventTrigger getSource() {
		return src;
	}


	public int getEvent() {
		return event;
	}


	public Object[] getInfo() {
		return info;
	}


	public void finalize() {
		src = null;
	}

}