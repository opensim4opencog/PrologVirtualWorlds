/***********************************************************************

  OKBC implementation for Java

  Copyright (c) 1996, 1997 Stanford University KSL.

  Original code by James Rice (rice@ksl.stanford.edu)


  KSL Network Services available at:
            http://www-ksl-svc.stanford.edu:5915/

  Legal Notice for KSL Network Services

  The software made available through the Stanford Knowledge Systems
  Laboratory's Network Services was written by and is copyright (c) by
  Stanford University, Knowledge Systems Laboratory and others not
  limited to those mentioned below.
  
  NEITHER THE AUTHORS NOR THE PUBLISHER OF THESE SERVICES AND DOCUMENTS
  OFFER ANY WARRANTY OF FITNESS OR MERCHANTABILITY FOR ANY PARTICULAR
  PURPOSE OR ACCEPTS ANY LIABILITY WITH RESPECT TO THE SERVICES,
  PROGRAMS, PROCEDURES, ALGORITHMS, DOCUMENTS OR APPLICATIONS OR
  DOCUMENTS MADE AVAILABLE THROUGH THE KNOWLEDGE SYSTEMS LABORATORY'S
  NETWORK SERVICES.
  
  Written by Nigel Jacobs.
  
  ***********************************************************************/


/**
 * PushableBufferedReader.java - A BufferedReader which allows a recently read
 * string to be 'pushed' back into the reader, so that the next read will yield
 * so that the next read will get the result of this read.
 *
 * NEEDS TO BE EXTENDED !!!  Only handles readLine currently.
 * 
 */

package OKBC;

import java.io.*;

public class PushableBufferedReader {

  String pushBuffer = null;
  BufferedReader reader = null;

  public PushableBufferedReader(BufferedReader reader) {
    this.reader = reader;
  }

  public PushableBufferedReader(Reader reader) {
    this.reader = new BufferedReader(reader);
  }


  /**
   * if there is a line in the push buffer, return it & clear the buffer,
   * otherwise read a line
   */
  public String readLine() throws IOException {

    if (pushBuffer != null) {
      String ret = pushBuffer;
      pushBuffer = null;
      System.out.println("PushableBufferedReader::readLine --> " + ret);
      return ret;
    } else {
      return reader.readLine();
    }

  }

  /**
   * push a previously read line, so that a future readLine will get this
   * pushed line
   */
  public void push(String s) {
    System.out.println("PushableBufferedReader::push s = " + s);
    pushBuffer = s;
  }


  /**
   * PASS-THRU METHODS FOR BufferedReader
   */
  public int read() throws IOException { return reader.read(); }

  public int read(char cbuf[]) throws IOException { return reader.read(cbuf); }

  public int read(char cbuf[],
			   int off,
			   int len) throws IOException
  { return reader.read(cbuf, off, len); }

  public long skip(long n) throws IOException { return reader.skip(n); }

  public boolean ready() throws IOException { return reader.ready(); }

  public boolean markSupported() { return reader.markSupported(); }

  public void mark(int readAheadLimit) throws IOException
  { reader.mark(readAheadLimit); }

  public void reset() throws IOException { reader.reset(); }

  public void close() throws IOException { reader.close(); }


}
