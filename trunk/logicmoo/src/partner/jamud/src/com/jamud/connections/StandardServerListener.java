package com.jamud.connections;


public abstract interface StandardServerListener {


   public abstract void serverAccept(StandardServerThread source,
				     java.net.Socket socket);


   public abstract void serverError(StandardServerThread source,
				    Exception e);


}
