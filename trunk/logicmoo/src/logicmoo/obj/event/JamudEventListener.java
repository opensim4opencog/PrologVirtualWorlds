package logicmoo.obj.event;

public abstract interface JamudEventListener {

	/** this is the method triggered whenever an event occurs. */
	public abstract void eventTriggered(JamudEvent je);

}