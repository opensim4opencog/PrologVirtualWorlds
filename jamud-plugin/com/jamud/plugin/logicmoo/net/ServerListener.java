package  logicmoo.net;

public abstract interface ServerListener {
   public void serverAccept(ServerThread source, java.net.Socket socket);
   public void serverError(ServerThread source, Exception e);
}