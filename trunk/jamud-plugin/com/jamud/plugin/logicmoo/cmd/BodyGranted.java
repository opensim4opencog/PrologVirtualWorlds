package logicmoo.cmd;

import logicmoo.obj.AgentGeneric;

/**
 * Base type for any grant-based body command.
 * @author siege
 */
public abstract class BodyGranted {

	/** The name of the command */
	public abstract String getName();

	/** The cost in hit-points */
	public abstract int getHpCost();

	/** The cost in mana */
	public abstract int getManaCost();

	/** The cost in movement */
	public abstract int getMoveCost();

	/** Execute the command that was issued from
	 * <code>src</code> with <code>args</code> as an argument
	 */
	public abstract void execute(AgentGeneric src, String args) throws Exception;

	public boolean equals(Object o) {
		return ((Object)this == o);
	}

}