package logicmoo.cmd;

import logicmoo.obj.Engineer;

/**
 * Base type for any grant-based player command.
 * @author siege
 */
public abstract class PlayerGranted {

	/** The name of the command */
	public abstract String getName();

	/** Execute the command that was issued from
	 * <code>src</code> with <code>args</code> as an argument
	 */
	public abstract void execute(Engineer src, String args) throws Exception;

	public boolean equals(Object o) {
		return ((Object)this == o);
	}

}