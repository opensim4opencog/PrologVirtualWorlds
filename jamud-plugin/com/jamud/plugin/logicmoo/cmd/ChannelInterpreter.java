package logicmoo.cmd;


import java.util.Enumeration;
import logicmoo.*;
import logicmoo.util.*;
import logicmoo.obj.*;

public abstract class ChannelInterpreter {

	public static boolean execute(Engineer src, String cmd, String args) {

		if("channels".startsWith(cmd)) {
			CHANNEL_LIST(src);
			return true;
		}

		Channel chan = completeChannel(src, cmd);

		if(chan == null) {
			return false;
		}
		else {
			CHANNEL(src, args, chan);
		}
		return true;
	}


   public static Channel completeChannel(Engineer src, String cmd) {
		int T = src.getTrust();
		for(Enumeration enum = Jamud.getChannelManager().elements(); enum.hasMoreElements(); ) {
			Channel c = (Channel) enum.nextElement();
			if(c.getTrust() <= T) {
				if(c.getName().toLowerCase().startsWith(cmd) || (c.getAlt() != null && c.getAlt().equals(cmd))) return c;
			}
		}
      return null;
   }


	/*
	 *	here begin the actual methods associated with
	 * the commands for this interpreter
	 *
	*/

   public static void CHANNEL(Engineer src, String args, Channel chan) {
		if (args.length() == 0) {

			String ss = "Channel [" + chan.getName() + "] is now ";
			boolean cc = src.getChannels().contains(chan.getName());
			if(cc) {
				src.getChannels().rem(chan.getName());
				chan.unsubscribe(src);
			}
			else {
				src.getChannels().add(chan.getName());
				chan.subscribe(src);
			}
			src.println(ss + (cc ? "Off" : "On"));

		}
		else {

			src.getChannels().add(chan.getName());
			chan.subscribe(src);

			chan.channelOut(src, args);			

			String you_out = Util.replace(chan.getLocal(), "%n", "You");
			you_out = Util.replace(you_out, "%t", args);
			src.println(you_out);

		}
   }

	public static void CHANNEL_LIST(Engineer src) {
		String j =   " {#+---{6Available Channels{#-------------------------------+{0\n";

		int T = src.getTrust();
		for(Enumeration enum = Jamud.getChannelManager().elements(); enum.hasMoreElements(); ) {
			Channel c = (Channel) enum.nextElement();
			if(c.getTrust() <= T) {
				j = j + "    " + Util.fixLength(c.getName(), 9, false) + (src.getChannels().contains(c.getName()) ? "ON" : "OFF" ) + "\n";
			}
		}
		j = j +    "\n {#+----------------------------------------------------+{0";

		src.println(j);
	}

}