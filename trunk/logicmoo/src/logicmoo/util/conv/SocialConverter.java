package logicmoo.util.conv;

import java.io.*;
import logicmoo.cmd.Social;
import logicmoo.util.Util;

public class SocialConverter implements Converter {

	BufferedReader fin;
	PrintWriter fout;

	public void convert(int c_type, String srcFile, String destFile) throws Exception {

		File src = new File(srcFile), dest = new File(destFile);
		boolean ok_go = !(dest.exists());
		System.out.println(MODE[c_type]);
		if(!ok_go) {
			//prompt if OK to over-write.
			System.out.print("Destination File (" + destFile + ") exists. Over-write (Y/N) ?");
			char c = (char) System.in.read();
			ok_go = (c=='y' || c=='Y');
		}
		else {
			System.out.print("creating... ");
			dest.createNewFile();
			System.out.println("created.");
			dest = new File(destFile);
		}

		if(ok_go) {
			System.out.println("ok...");

			switch(c_type) {
				case MODE_ROT_JAMUD: {
					rotToJamud(src, dest);
					break;
				}
				case MODE_JAMUD_ROT: {
					logicmooToRot(src, dest);
					break;
				}
				case MODE_ROM_JAMUD: {
					romToJamud(src, dest);
					break;
				}
				case MODE_JAMUD_ROM: {
					logicmooToRom(src, dest);
					break;
				}
			}
		}
	}

	public void rotToJamud(File src, File dest) {
		System.out.println("rot to logicmoo...");

		System.out.println(src + " to " + dest);

		try {
			fout = new PrintWriter(new FileOutputStream(dest));
			fin = new BufferedReader(new InputStreamReader(new FileInputStream(src)));

			System.out.println("Created fin and fout");

			String i;
			boolean started = false;
			while(! started) {
				i = fin.readLine().trim();
				System.out.println(i);
				if(i.equals("#SOCIALS")) started = true;
			}
			fout.println("<SOCIALS>");

			i = fin.readLine().trim();
			while(! i.equals("#0")) {
				if(i.length() > 0) {
					Social s = new Social(Util.split(i, " ")[0]);
					System.out.println(" Starting " + s.getName());
					i = fin.readLine().trim();
					for(int c=0; ((! i.equals("#")) && (c < Social.PART.length)); c++) {
						if(! i.equals("$")) {
							System.out.println("  " + c + " : " + i);
							int C = (c < 2 ? c : (c==2 ? 5 : (c==3 ? 7 : (c==4 ? 6 : (c==5 ? 2 : (c==6 ? 3 : (c==7 ? 4 : -1)))))));
							s.setPart(C, Util.replace(i, "$", "%"));
						}
						if((! i.equals("#")) && (c < Social.PART.length - 1)) i = fin.readLine().trim();
					}
					System.out.println(" ... added.");
					fout.println(s.toXMLElement());
				}
				i = fin.readLine().trim();
			}
			fout.println("</SOCIALS>");
			fin.close();
			fout.close();
		}
		catch(Exception e) {
			System.err.println(e);
		}
	}

	public void logicmooToRot(File src, File dest){}
	public void romToJamud(File src, File dest){}
	public void logicmooToRom(File src, File dest){}

	public void printUse() {
		System.out.println("R.T.F.M");
	}



	public static void main(String[] args) {
		SocialConverter c = new SocialConverter();

		if(args.length != 3) {
			c.printUse();
		}
		else {
			String s = args[0];
			int I = 0;
			boolean l = true;
			while(I<MODE.length && l) {
				l = !(s.equals(MODE[I]));
				if(l) I++;
			}
			try {
				c.convert(I, args[1], args[2]);
			}
			catch(Exception e) {
				System.err.println("Error; could not complete.\n" + e);
			}
		}
	}

}