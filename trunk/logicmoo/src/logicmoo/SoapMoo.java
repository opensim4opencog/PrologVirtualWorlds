package logicmoo;

import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;
import java.awt.*;

import java.net.URL;
import java.net.*;
import org.w3c.dom.*;
import org.xml.sax.*;
import javax.xml.parsers.*;
import org.apache.soap.util.xml.*;
import org.apache.soap.*;
import org.apache.soap.encoding.*;
import org.apache.soap.encoding.soapenc.*;
import org.apache.soap.rpc.*;
import org.apache.soap.transport.http.SOAPHTTPConnection;



/**
 * See \samples\stockquote\readme for info.
 *
 * @author Sanjiva Weerawarana (sanjiva@watson.ibm.com)
 */
public class SoapMoo {

	public static void main (String[] args) throws Exception {
		if ( args.length != 2
			 && (args.length != 3 || !args[0].startsWith ("-")) ) {
			System.err.println ("Usage: java " + SoapMoo.class.getName () +
								" [-encodingStyleURI] SOAP-router-URL symbol");
			System.exit (1);
		}

		// Process the arguments.
		int offset = 3 - args.length;
		String encodingStyleURI = args.length == 3
								  ? args[0].substring(1)
								  : Constants.NS_URI_SOAP_ENC;
		URL url = new URL (args[1 - offset]);
		String symbol = args[2 - offset];

		// Build the call.
		Call call = new Call ();
		call.setTargetObjectURI ("urn:xmltoday-delayed-quotes");
		call.setMethodName ("getQuote");
		call.setEncodingStyleURI(encodingStyleURI);
		Vector params = new Vector ();
		params.addElement (new Parameter("symbol", String.class, symbol, null));
		call.setParams (params);

		// make the call: note that the action URI is empty because the 
		// XML-SOAP rpc router does not need this. This may change in the
		// future.
		Response resp = call.invoke (	 /* router URL */ url,	  /* actionURI */ "" );

		// Check the response.
		if ( resp.generatedFault () ) {
			Fault fault = resp.getFault ();
			System.out.println ("Ouch, the call failed: ");
			System.out.println ("  Fault Code   = " + fault.getFaultCode ());  
			System.out.println ("  Fault String = " + fault.getFaultString ());
		} else {
			Parameter result = resp.getReturnValue ();
			System.out.println (result.getValue ());
		}
	}

}



