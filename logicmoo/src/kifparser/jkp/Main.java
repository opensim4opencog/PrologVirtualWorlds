
/**
   test the lexer/parser on a variety of objects
   This file was originally provided by the jb package, 
   modified by X.Luan.
*/

package jkp;

import java.io.*;
import jbf.*;

/**
   Usage(to run): java jkp.Main < filename
   where filename is name of the file containing KIF sentences.
*/
public class Main {

	public static void main(String argv[])
	{
		String	buf;
		YYlex	yyl;
		YYparse	yyp;

		DataInputStream DIS = new DataInputStream(System.in);

		while(true) {
			try {
				if(DIS.available() <= 0) 
					break;
				buf = DIS.readLine();
			}
			catch (IOException e) {
				System.out.println("IOException");
				break;
			}
			if(buf.length() < 1)
				continue;
		
			try {
				//  KifParser.parse(System.in);
				KifObj obj = KifParser.parse(buf);
				KifParser.printResult();
				/*
				String rule = Kif2Jess.toJess((KifSent)obj, null, null);
				System.out.println("rule: " + rule);
				*/
			}
			catch (Exception e) {
				System.out.println("exception: detected");
			}
		}
	}
}
