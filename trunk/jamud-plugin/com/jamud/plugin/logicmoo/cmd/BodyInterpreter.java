package logicmoo.cmd;


import logicmoo.obj.*;
import logicmoo.util.Util;
import java.util.*;


public abstract class BodyInterpreter {

	public static final BodyTrustedTable trustedTable = new BodyTrustedTable();

	public static final BodyGrantedTable grantedTable = new BodyGrantedTable();


	public static boolean execute(AgentGeneric src, String cmd, String args) {
		return execute(src, cmd, args, false);
	}


	public static boolean execute(AgentGeneric src, String cmd, String args, boolean fromFight) {

		if(executeTrusted(src, cmd, args, fromFight))
			return true;
		else if(executeGranted(src, cmd, args, fromFight))
			return true;
		else
			return Social.trySocial(src, cmd, args);

	}


	private static boolean executeTrusted(AgentGeneric src, String cmd, String args, boolean fromFight) {
		Engineer p = src.getEngineer();
		int trust = ((p == null) ? 3 : p.getTrust());
		p = null;

      BodyTrusted meth = trustedTable.complete(cmd, trust);
      if(meth != null) {
			try {
				if( (!fromFight) && (src.getFight() != null) )
					src.getFight().enqueueCommand(meth.getName() + " " + args);
				else
					meth.execute(src, args);
				return true;
			}
			catch(Exception e) {
				System.err.println("Exception in BodyInterpreter.executeTrusted");
				System.err.println(" - '" + cmd + " " + args + "'");
				e.printStackTrace();
			}
		}
		return false;
	}


   private static boolean executeGranted(AgentGeneric src, String cmd, String args, boolean fromFight) {
		BodyGranted skill = src.getGranted(cmd);
		if(skill != null) {
			try {
				if( (!fromFight) && (src.getFight() != null) )
					src.getFight().enqueueCommand(skill.getName() + " " + args);
				else
					skill.execute(src, args);
				return true;
			}
			catch(Exception e) {
				System.err.println("Exception in BodyInterpreter.executeGranted");
				System.err.println(" - '" + cmd + " " + args + "'");
				e.printStackTrace();
			}
		}
		return false;
   }


}