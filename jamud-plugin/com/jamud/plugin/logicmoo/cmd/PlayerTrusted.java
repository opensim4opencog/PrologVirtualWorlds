package logicmoo.cmd;

import logicmoo.obj.Engineer;

public abstract interface PlayerTrusted {

	public abstract String getName();

	public abstract int getTrust();

	public abstract void execute(Engineer src, String args) throws Exception;
}