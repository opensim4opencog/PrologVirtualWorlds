package logicmoo;

import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;

import jamud.*;
import jamud.command.*;
import jamud.object.*;
import jamud.object.event.*;
import jamud.util.*;
import com.jamud.commands.*;
import com.jamud.connections.*;
import com.jamud.communications.*;
import com.jamud.creation.*;
//import net.n3.nanoxml.*;


public class LogicMoo {

		public void LogicMoo() {
				try {
				LogfileWriter = new FileWriter(new File("/logicmoo.debug.txt"));
				} catch (Exception e) {

				}
				
				logWrite("Start jamud");
				//startJamud();


		}


		public FileWriter LogfileWriter;
		public Jamud jamudInstance;
		public MudObjectRoot jamudMudObjectRoot; 
		public MudObject jamudMudObjectBelowRoot; 

		public void logWrite(String msg) {
				try {
						System.out.println(msg);
					   LogfileWriter.write(msg);
				} catch (Exception e) {

				}
		}

		public void startJamud() {
				logWrite("Start jamud");
				if ( jamudInstance==null ) {
						try {
								String[] args = { "jamud.xml"} ; 
								Jamud.main( args );
								jamudInstance = Jamud.currentInstance();
								jamudMudObjectRoot = jamudInstance.mudObjectRoot();
							 //   Iterator cobjs = jamudMudObjectRoot.childObjects();
						 	 ///   jamudMudObjectBelowRoot = (MudObject)cobjs.next();
						} catch ( Exception e ) {
								logWrite("" +e );
								e.printStackTrace(System.out);
						}
				}
		}


}


