package  logicmoo.net;


import logicmoo.obj.Engineer;

public interface PlyrConnection {

   public static final int
		LOGOUT_QUIT = 0,
      LOGOUT_ERROR = 1,
      LOGOUT_ELSEWHERE = 2;

   public void setEngineer(Engineer player);

   public void logOut(int mode);

   public boolean goodConnection();

	public String getHost();
	public int getLocalPort();
	public int getRemotePort();
	public long connectedAt();

   public void print(String text);
   public void print(byte[] text);
   public void println(String text);

	public String prompt(String msg);
}