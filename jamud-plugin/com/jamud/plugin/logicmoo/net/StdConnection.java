package  logicmoo.net;


import java.net.Socket;
import java.io.*;
import logicmoo.util.Util;
import logicmoo.cmd.Interpreter;
import logicmoo.obj.Engineer;

public class StdConnection extends Thread implements PlyrConnection {


   private Engineer player;
      public void setEngineer(Engineer player) { this.player = player; }

	private long created;
		public long connectedAt() { return this.created; }

	private String host;
		public String getHost() { return this.host; }

	private int lPort;
		public int getLocalPort() { return this.lPort; }

	private int rPort;
		public int getRemotePort() { return this.rPort; }

	private boolean continueRunning;
   private Socket socket;
   private BufferedReader netin;
   private OutputStream netout;

	int agg = 1;
	int aggCount = 0;

   public StdConnection(Socket socket) {
		try {
	      this.socket = socket;
			socket.setKeepAlive(true);
	      this.start();
		}
		catch(Exception e) {
			System.err.println("Exception in StdConnection constructor");
			System.err.println(e);
		}
   }


   public void run() {
		this.created = System.currentTimeMillis();
		this.continueRunning = true;

		try {
			netout = socket.getOutputStream();

			host = "[err]"; //in case host lookup farts out
			host = socket.getInetAddress().getHostAddress();
			rPort = socket.getPort();
			lPort = socket.getLocalPort();

	      System.out.println(" -Engineer Connection Accepted (" + host + ")");
			player.logIn();
		}
		catch (Exception f) {
			System.err.println("Exception Starting Engineer Connection (" + host + ")");
			System.err.println(f);
		}

		if(player != null && player.getLoggedOn()) {
			while(this.continueRunning) {
				try {
					String in = readLine();

					if(in.length() + agg == 0)
						aggCount++;
					else
						aggCount=0;

					if(aggCount < 2 && this.continueRunning) {
						player.encueCommand(in);
						agg = in.length();
	   	         this.yield();
					}
				}
				catch(Exception e) {
					if(this.continueRunning) {
						System.err.println(e);
						this.logOut(LOGOUT_ERROR);
					}
				}
  	      }
		}
		else
			if(socket != null) this.logOut(LOGOUT_QUIT);

		System.out.println(" -Engineer Connection Closed (" + host + ")");
   }

   public void logOut(int mode) {
      //if mode == 0 logged out legally (quit)
      //if mode == 1 go linkdead (error)
      //if mode == 2 logged in elsewhere


		this.continueRunning = false;

      try {
         switch (mode) {
            case LOGOUT_QUIT:
					System.out.println("  LOGOUT_QUIT (" + host + ")");
               break;

            case LOGOUT_ERROR:
					System.out.println("  LOGOUT_ERROR (" + host + ")");
					if(player != null) player.setLoggedOn(false);
               break;

            case LOGOUT_ELSEWHERE:
					System.out.println("  LOGOUT_ELSEWHERE (" + host + ")");
               break;
         }
			netout = null;
			netin = null;

			socket.shutdownInput();
			socket.shutdownOutput();
			socket.close();
			socket = null;
      }
      catch(Exception e) {
         String n = null;
			if(player != null) n = player.getName();
         System.err.println("Exception at logOut(" + mode + ")" + ((n != null) ?  " for " + n : ""));
         System.err.println(e);
      }
		finally {
			if(player != null) {
				player.setConnection(null);
				player = null;
			}
		}
   }

   public boolean goodConnection() {
      return this.isAlive();
   }


   public void print(String text) {
		if(text == null) return;
		aggCount = 0;
      try {
         netout.write(Util.replace(text, "\n", "\n\r").getBytes());
			netout.flush();
      }
      catch(Exception e) {
			System.err.println("Exception at print(String)");
         System.err.println(e);
         logOut(1);
      }
   }

   public void print(byte[] text) {
		if(text == null) return;
		aggCount = 0;
      try {
         netout.write(text);
			netout.flush();
      }
      catch(Exception e) {
			System.err.println("Exception at print(byte[])");
         System.err.println(e);
         logOut(1);
      }
   }

   public void println(String text) {
		if(text == null) return;
		aggCount = 0;
      try {
         netout.write((Util.replace(text, "\n", "\n\r") + "\n\r").getBytes());
			netout.flush();
      }
      catch(Exception e) {
			System.err.println("Exception at println(String)");
         System.err.println(e);
         logOut(1);
      }
   }

   public String prompt(String msg) {
      try {
         print(msg);
         if(socket != null) return readLine();
			else return null;
      }
      catch(Exception e) {
			System.err.println("Exception at prompt(String)");
         System.err.println(e);
         logOut(1);
         return null;
      }
   }

   private String readLine() throws Exception {
      netin = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		return Util.cleanup(netin.readLine());
	}

}