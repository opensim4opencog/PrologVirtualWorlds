package logicmoo;

import java.lang.*;
import java.lang.reflect.*;
import java.io.*;
import java.util.*;
import java.awt.*;

import org.opencyc.cycobject.*;
import org.opencyc.api.*;
import org.opencyc.constraintsolver.*;
import org.opencyc.inferencesupport.*;
import org.opencyc.util.*;
import org.opencyc.xml.*;


/**
 * Provides the behavior and attributes of OpenCyc AssertionsCollection.<p>
 * <p>
 * Assertions are communicated over the binary API using their Id number (an int).
 * The associated formula, microtheory, truth-value, direction, and remaining attributes are
 * is fetched later.
 *
 * @version $Id: AssertionsCollection.java,v 1.1 2002-04-06 22:05:42 dmiles Exp $
 * @author Douglas R. Miles
 *
 * <p>Copyright 2001 Cycorp, Inc., license is open source GNU LGPL.
 * <p><a href="http://www.opencyc.org/license.txt">the license</a>
 * <p><a href="http://www.opencyc.org">www.opencyc.org</a>
 * <p><a href="http://www.sourceforge.net/projects/opencyc">OpenCyc at SourceForge</a>
 * <p>
 * THIS SOFTWARE AND KNOWLEDGE BASE CONTENT ARE PROVIDED ``AS IS'' AND
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OPENCYC
 * ORGANIZATION OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE AND KNOWLEDGE
 * BASE CONTENT, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

public class AssertionsCollection {

	private ArrayList cycListCollection;
	private String defaultMt;
	/**
	 * Cyc api support.
	 */
	CycAccess cycAccess;

	public void AssertionsCollection(CycAccess ca) {
		cycAccess = ca;
		cycListCollection = new java.util.ArrayList();
	}

	/**
	 * Sets the defaultMt for this AssertionsCollection.
	 */
	public void setDefaultMt(String mt) {
		defaultMt=mt;
	}

	/**
	 * Returns the defaultMt for this AssertionsCollection.
	 *
	 * @return the defaultMt for this AssertionsCollection
	 */

	public String getDefaultMt() {
		return defaultMt;
	}

	/**
	 * Indicates whether the object is equal to this object.
	 *
	 * @return <tt>true</tt> if the object is equal to this object, otherwise
	 * returns <tt>false</tt>
	 */
	public boolean equals (Object object) {
		if ( !(object instanceof AssertionsCollection) )
			return  false;
		return(this == (AssertionsCollection) object);
	}

	/**
	 * Returns a <tt>String</tt> representation of the <tt>AssertionsCollection</tt>.
	 *
	 * @return a <tt>String</tt> representation of the <tt>AssertionsCollection</tt>
	 */
	public String toString () {
		return "AssertionsCollection:" + defaultMt + ":" + cycListCollection.size() + ":" + this.hashCode();
	}

	/**
	 * Adds the formula (String) to this AssertionCollection.
	 *
	 * @param String of the formula for this AssertionsCollection
	 */
	public void addFormula(String formula) throws CycApiException {
		CycListParser cycListParser = new CycListParser(cycAccess);
		addFormula((CycList) cycListParser.read(formula));
	}

	/**
	 * Adds the formula (StreamTokenizer) to this AssertionCollection.
	 *
	 * @param StreamTokenizer of the formula for this AssertionsCollection
	 */
	public void addFormula(StreamTokenizer formula) throws CycApiException {
		CycListParser cycListParser = new CycListParser(cycAccess);
		addFormula((CycList) cycListParser.read(formula));
	}

	/**
	 * Adds the formula (CycList) to this AssertionCollection.
	 *
	 * @param (CycList) of the formula for this AssertionsCollection
	 */
	public void addFormula(CycList formula) {
		cycListCollection.add(formula);
	}


	/**
	 * Adds the File to this AssertionCollection.
	 *
	* @param file for formulas for this AssertionsCollection
	* @param mt for formulas for this AssertionsCollection
	 */
	public void loadFile(File file) throws Exception {
		FileReader fr  = new FileReader(file);
		Reader r = new BufferedReader(fr);
		StreamTokenizer st = new StreamTokenizer(r);
		while ( r.ready() )
			addFormula(st);
	}


	/**
	 * Adds the File to KB in defaultMt.
	 *
	 * @param file for formulas for this AssertionsCollection
	 */
	public void addFile(File file)  throws Exception  {
		cycListCollection = new java.util.ArrayList();
		loadFile(file);
		commitAssertions();
	}

	/**
	 * Adds the File to KB into Mt.
	 *
	 * @param file for formulas 
	* @param mt for formulas 
	 */
	public void addFile(File file, String mt)  throws Exception  {
		cycListCollection = new java.util.ArrayList();
		loadFile(file);
		commitAssertions(mt);
	}


	/**
	 * Adds the AssertionCollection to defaultMt
	 *
	 * @param file for formulas for this AssertionsCollection
	 */
	public void commitAssertions()  throws Exception  {
		commitAssertions(defaultMt);

	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param file for formulas for this AssertionsCollection
	 */
	public void commitAssertions(String mt)  throws Exception  {
		CycFort mtFort = ensureMt(mt);
		commitAssertions1(mtFort);
		commitAssertions2(mtFort);
		commitAssertions3(mtFort);
		commitAssertions4(mtFort);
		commitAssertions5(mtFort);
		commitAssertions6(mtFort);
		commitAssertions7(mtFort);
	}

	/**
	 * Ensures the Mt is a valid Microtheory
	 *
	 * @param mt for formulas for this AssertionsCollection
	 */
	public CycFort ensureMt(String mt)  throws Exception  {
		return cycAccess.getConstantByName(mt);
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 */
	public void commitAssertions1(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
			if (cycAccess.isWellFormedFormula(cycList)) 
					cycAccess.assertWithTranscript(cycList,mt);
			
					
		}
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 */
	public void commitAssertions2(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 */
	public void commitAssertions3(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param file for formulas for this AssertionsCollection
	 */
	public void commitAssertions4(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 */
	public void commitAssertions5(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 */
	public void commitAssertions6(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 */
	public void commitAssertions7(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}

	}
