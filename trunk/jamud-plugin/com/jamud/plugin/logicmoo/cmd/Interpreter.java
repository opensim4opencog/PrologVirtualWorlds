package logicmoo.cmd;


import java.util.Enumeration;
import logicmoo.Jamud;
import logicmoo.obj.*;
import logicmoo.util.*;


public abstract class Interpreter {

	public static final String MESSAGE_UNKNOWN = "interpreter.unknown";

   public static void execute(Engineer source, String arg, boolean doAlias) {
      String S = arg.trim(),
			P = (String) source.getProperty(Engineer.PROPERTY_LAST_COMMAND);

		if(P != null) {
			if(S.length() > 1 && S.substring(0, 2).equals("!!"))
				S = P + S.substring(2);
			else if(S.equals("!"))
				S = P;

			P = null;
		}

      if(S.length() == 0)
         source.println("");

		else if(S.charAt(0)== '#')
			source.println(S.substring(1));

      else {
			if(doAlias)
				source.putProperty(Engineer.PROPERTY_LAST_COMMAND, S);
			String[] in = Util.split(S, " ");
         executeMethod(source, in[0].toLowerCase(), in[1], doAlias);
      }

   }


   private static void executeMethod(Engineer source, String cmd, String args, boolean doAlias) {

		boolean ex = false;

		// Alias
		if(doAlias) {
			Alias a = source.getAlias(cmd);
			if(a != null) {
				a.execute(source, args);
				return;
			}
		}

		// AgentGeneric related
		if(! ex)
			ex = ((AgentGeneric)source.getProperty(Engineer.PROPERTY_CURRENT_BODY)).enact(cmd, args, false);

		// Engineer related
		if(! ex)
			ex = PlayerInterpreter.execute(source, cmd, args);

		// huh??
      if(! ex)
			source.println(Jamud.getMessage(MESSAGE_UNKNOWN));
   }

}