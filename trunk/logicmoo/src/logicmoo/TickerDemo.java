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
public class TickerDemo {
	
	DocumentBuilder xdb = XMLParserUtils.getXMLDocBuilder();

	public float getQuote(String symbol) throws Exception {
		// get a real (delayed by 20min) stockquote from 
		// http://www.xmltoday.com/examples/stockquote/. The IP addr 
		// below came from the host that the above form posts to ..
		URL url = new URL ("http://www.xmltoday.com/examples/stockquote/getxmlquote.vep?s="+symbol);
		InputStream is = url.openStream ();
		Document d = xdb.parse(is);
		Element e = d.getDocumentElement ();
		NodeList nl = e.getElementsByTagName ("price");
		e = (Element) nl.item (0);
		String quoteStr = e.getAttribute ("value");
		try {
			return Float.valueOf (quoteStr).floatValue ();
		} catch ( NumberFormatException e1 ) {
			// mebbe its an int?
			try {
				return Integer.valueOf (quoteStr).intValue () * 1.0F;
			} catch ( NumberFormatException e2 ) {
				return -1.0F;
			}
		}
	}

}
 


