package logicmoo.util;


import java.io.*;

public abstract class FileAccess {

   /**
    * @author Siege
    * @param fileName fully-qualified string filename
    * of file to be loaded
    * @return String contents of file.
    */
   public static String loadFile(String fileName) throws Exception {
		//System.out.println(" * loading file from '" + fileName + "'");
      File file = new File(fileName);

      int I = (int) file.length();
      byte J[] = new byte[I];

      FileInputStream stream = new FileInputStream(file);

      stream.read(J, 0, I);
      stream.close();

      return new String(J);
   }

	
	/**
	 * Fills file indicated by filename with content. All original
	 * content is replaced.
	 * @author Siege
	 * @param fileName fully-qualified name of file to be written to
	 * @param content String to write to file
	 */
	public static void fillFile(String fileName, String content) throws Exception {
		FileOutputStream S = (new FileOutputStream(fileName));
		try {
			S.write(content.getBytes());
			S.close();
		}
		catch(Exception e) {
			try {
				S.close();
			}
			catch(Exception f) {
				//only one Exception at a time, thanks.
			}
			throw(e);
		}
	}

}