package logicmoo.cmd;


import logicmoo.obj.*;
import logicmoo.util.*;
import java.lang.reflect.*;
import java.util.*;


public abstract class PlayerInterpreter {

	public static final PlayerTrustedTable trustedTable = new PlayerTrustedTable();

	public static final PlayerGrantedTable grantedTable = new PlayerGrantedTable();


   public static boolean execute(Engineer src, String cmd, String args) {
		if( executeTrusted(src, cmd, args) )
			return true;
		else if( executeGranted(src, cmd, args) )
			return true;
		else if( PostInterpreter.execute(src, cmd, args) )
			return true;
		else
			return ChannelInterpreter.execute(src, cmd, args);
   }


	private static boolean executeTrusted(Engineer src, String cmd, String args) {
      PlayerTrusted meth = trustedTable.complete(cmd, src.getTrust());
		if(meth != null) {
			try {
				meth.execute(src, args);
				return true;
			}
			catch(Exception e) {
				System.err.println("Exception in PlayerInterpreter.executeTrusted");
				System.err.println(" -" + cmd + " " + args);
				e.printStackTrace();
			}
		}
      return false;
	}


   private static boolean executeGranted(Engineer src, String cmd, String args) {
		PlayerGranted command = src.getGranted(cmd);
		if(command!=null) {
			try {
				command.execute(src, args);
				return true;
			}
			catch(Exception e) {
				System.err.println("Exception in PlayerInterpreter.executeGranted");
				System.err.println(" -" + cmd + " " + args);
				e.printStackTrace();
			}
		}
		return false;
   }

}