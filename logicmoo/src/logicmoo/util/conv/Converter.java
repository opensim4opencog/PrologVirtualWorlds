package logicmoo.util.conv;

import java.io.File;
import logicmoo.util.FileAccess;

public interface Converter {

	public static final String[] MODE = { "-r", "-R", "-m", "-M" };

	public static final int MODE_ROT_JAMUD = 0,
									 MODE_JAMUD_ROT = 1,
									 MODE_ROM_JAMUD = 2,
									 MODE_JAMUD_ROM = 3;	


	public void rotToJamud(File src, File dest);

	public void logicmooToRot(File src, File dest);

	public void romToJamud(File src, File dest);

	public void logicmooToRom(File src, File dest);

	public void printUse();

}