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
 * @version $Id: AssertionsCollection.java,v 1.2 2002-04-06 22:19:26 dmiles Exp $
 * @author Douglas R. Miles
 *
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
		commitAssertionsDefintional(mtFort);
		commitAssertionsPredicateDefs(mtFort);
		commitAssertionsCollectionDefs(mtFort);
		commitAssertionsSpecialCollectionDefs(mtFort);
		commitSecondaryConstantDefs(mtFort);
		commitAssertionsOfRest(mtFort);
	}

	/**
	 * Ensures the Mt is a valid Microtheory
	 *
	 * @param mt for formulas for this AssertionsCollection
	 * <eca-home> All microtheory definitions 
	 */
	public CycFort ensureMt(String mt)  throws Exception  {
		return cycAccess.getConstantByName(mt);
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 * [08:09] <eca-home> All microtheory definitions (isa and other definitional assertions) second.
	 */
	public void commitAssertionsDefintional(CycFort mt)  throws Exception  {
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
     * [08:10] <eca-home> All predicate definitions (isa, arity, argIsa, and other definitional assertions) third.
	 */
	public void commitAssertionsPredicateDefs(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 * [08:11] <eca-home> All collection definitions, in descending order of type, fourth.
	 */
	public void commitAssertionsCollectionDefs(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 * [08:11] <eca-home> e.g. ThirdOrderCollections, then SecondOrderCollections (CollectionTypes), then Collections which genls Individual.
	 */
	public void commitAssertionsSpecialCollectionDefs(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param file for formulas for this AssertionsCollection
	 * [08:11] <eca-home> All other constant definitions fifth
	 */
	public void commitSecondaryConstantDefs(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 * [08:11] <eca-home> Then all other assertions -- the non-definitional ones.
	 * (and (isa ?GAF CycLClosedAtomicSentence) (operatorFormulas ?PRED ?GAF) (isa ?PRED OpenCycDefinitionalPredicate))
	 */
	public void commitAssertionsOfRest(CycFort mt)  throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();
		}
	}


 
	}
