package logicmoo.cmd;

import logicmoo.obj.AgentGeneric;

public abstract interface BodyTrusted {

	public abstract String getName();

	public abstract int getTrust();

	public abstract void execute(AgentGeneric src, String args) throws Exception;
}