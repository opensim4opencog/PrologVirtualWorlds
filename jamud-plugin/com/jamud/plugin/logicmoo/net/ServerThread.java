package  logicmoo.net;

import java.net.*;
import java.io.InterruptedIOException;

public class ServerThread extends Thread {

	private static int TIMEOUT = 500;

   private ServerListener listener;
   private ServerSocket serv_sock = null;

	private boolean keepGoing;
	public boolean isAccepting() { return keepGoing; }

	private int port;
	public int getPort() { return port; }


   public ServerThread(ServerListener listener, int port) {
      this.listener = listener;
		this.port = port;
   }

   public ServerThread(ServerListener listener, ServerSocket serv_sock) {
      this.listener = listener;
      this.serv_sock = serv_sock;
		this.port = serv_sock.getLocalPort();
		try {
			if(serv_sock.getSoTimeout() == 0)
				serv_sock.setSoTimeout(TIMEOUT);
		}
		catch(Exception e) {
			//empty ?
		}
   }


   public void run() {
      try{

			this.keepGoing = true;

			if(serv_sock == null) {
	         this.serv_sock = new ServerSocket(port);
				this.serv_sock.setSoTimeout(TIMEOUT);
			}

         while(this.keepGoing) {
				try {
					Socket s = serv_sock.accept();
					if(this.keepGoing) listener.serverAccept(this, s);
				}
				catch(InterruptedIOException f) {
					//nothing, just to keep us from waiting forever
				}
            this.yield();
         }

			serv_sock.close();

      }
      catch(Exception e) {
			halt();
         listener.serverError(this, e);
      }

		serv_sock = null;

   }

	public void halt() {
		if(this.keepGoing) {
			this.keepGoing = false;
			try {
				if(serv_sock != null) sleep((long) serv_sock.getSoTimeout());
			}
			catch(Exception e) {
				//empty ?
			}
		}
	}

}
