// File: KifSeq.java

/*  Set tabstop = 4 to read this file.
*
*   Date Created: Nov 96
*   Maintanence History:
*/


package jkp;

import jbf.*;
import java.lang.*;
import java.util.*;



/**
 *  The class for KIF object sequence, such as term sequence(in a relational
 *  sentence), sentence sequence(in conjuction/disjunction), etc. 
 *  A very important class in this parser.
 *  <p>
 *  Copyright (c) 1996, Xiaocheng Luan.
 *  All rights reserved.
 *  <p>
 *  @author  <a href="http://www.cs.umbc.edu/~xluan1/">Xiaocheng Luan</A>
 */
public class KifSeq extends KifObj {

	private Vector	_content;

	/**
	   Create an empty sequence.
	*/
	public KifSeq()
	{
		setType(KifObj.KIF_SEQ);
		_content = new Vector();
	}

	
	/**
	 * Create an instance of KifSeq with only one object in the sequence.
	 * @param obj The only one object in the sequence.
	 */
	public KifSeq(KifObj obj)
	{
		setType(KifObj.KIF_SEQ);
		_content = new Vector();
		_content.addElement(obj);
	}
	

	/**
	 * Create an instance of KifSeq with the given object as the first
	 * object in the sequence, and objects in the given sequence as the 
	 * rest of the created sequence.
	 * @param obj the first object in the created sequence.
	 * @param seq the objects of which will be the second part of the new
	 *            sequence.
	 */
	public KifSeq(KifObj obj, KifSeq seq)
	{
		this(obj);
		append(seq);
	}


	/**
	 * Create an instance of KifSeq by appending the given object to the 
	 * given sequence.
	 * @param seq the given sequence.
	 * @param obj the given object.
	 */
	public KifSeq(KifSeq seq, KifObj obj)
	{
		this(seq);
		append(obj);
	}

	
	/**
	 * Create a new copy of the given sequence, copy constructor.
	 * @param seq the given sequence.
	 */
	public KifSeq(KifSeq seq)
	{
		setType(KifObj.KIF_SEQ);
		_content = new Vector();
		for(int i = 0; i< seq.size(); i++)
			_content.addElement(seq.ithObj(i));
	}


	/**
	 * Get the ith object in the sequence.
	 * @param i the object index, indicating which object to get.
	 * @return the ith Kif object in the sequence.
	 */
	public KifObj
	ithObj(int i) throws KifRTException
	{
		if( i>= size()) {
			throw (new KifRTException("ithObj: bad index"));
		}

		return (KifObj)_content.elementAt(i);
	}


	/**
	   Append a given Kif object to this sequence.
	   @param obj the given object to append.
	*/
	public void
	append(KifObj obj)
	{
		_content.addElement(obj);
	}


	/**
	   Append a given Kif sequence to this sequence.
	   @param seq the given sequence.
	*/
	public void
	append(KifSeq seq)
	{
		for(int i=0; i<seq.size(); i++)
			_content.addElement(seq.ithObj(i));
	}


	/**
	   Test if this sequence is empty.
	   @return: true if the sequence is empty; false otherwise.
	*/
	public boolean 
	isEmpty()
	{
		return _content.isEmpty();
	}


	/**
	 * Get the first element of this sequence, as in Lisp.
	 * @return the first element of the sequence; return null if empty.
	 */
	public KifObj
	car()
	{
		if(size() <= 0)
			return null;
		
		return (KifObj)_content.elementAt(0);
	}


	/**
	 * Get the subsquence(from 2nd element to end) of the sequence, as in Lisp.
	 * @return the subsquence(from 2nd element to end) of the sequence;
	 * return null if there is less than 2 elements in the sequence.
	 */
	public KifSeq 
	cdr()
	{
		if(size()<2)
			return null;

		KifSeq seq = new KifSeq();

		for(int i=1; i<size(); i++)
			seq.append(ithObj(i));
		
		return seq;
	}


	/**
	   Get the size (# of kif objects in the sequence) of the sequence.
	   @return the size of the sequence.
	*/
	public int 
	size()
	{
		return _content.size();

	}


	/**
	   Convert this KIF sequence into String format.
	   @return this KIF sequence in String format.
	*/
	public String
	toString()
	{
		StringBuffer buf = new StringBuffer();
		
		if(size() > 0)
			buf.append(ithObj(0).toString());
		for(int i=1; i<size(); i++) 
			buf.append(" " + ithObj(i).toString());

		return buf.toString();
	}
}

