package logicmoo.cmd;

public abstract interface ChannelListener {
	public void channelOut(Channel channel, logicmoo.obj.PlayerMask mask, String text);
}
