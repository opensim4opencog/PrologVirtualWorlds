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
 * @version $Id: AssertionsCollection.java,v 1.4 2002-04-07 04:42:35 dmiles Exp $
 * @author Douglas R. Miles
 *
 */

public class AssertionsCollection {

	private ArrayList cycAssertionsMtDefs;
	private ArrayList cycAssertionsDefintional;
	private ArrayList cycAssertionsPredicateDefs;
	private ArrayList cycAssertionsCollectionDefs;
	private ArrayList cycAssertionsSpecialCollectionDefs;
	private ArrayList cycAssertionsSecondaryConstantDefs;
	private ArrayList cycAssertionsOfRest;
	private String defaultMt;

	/**
	 * Cyc api support.
	 */
	CycAccess cycAccess;

	public void AssertionsCollection(CycAccess ca) {
		cycAccess = ca;
		resetAssertions();
	}

	public void resetAssertions() {
		cycAssertionsMtDefs = new ArrayList();
		cycAssertionsDefintional = new ArrayList();
		cycAssertionsPredicateDefs = new ArrayList();
		cycAssertionsCollectionDefs = new ArrayList();
		cycAssertionsSpecialCollectionDefs = new ArrayList();
		cycAssertionsSecondaryConstantDefs = new ArrayList();
		cycAssertionsOfRest = new ArrayList();
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
		return "AssertionsCollection:" + defaultMt + ":" + this.hashCode();
	}


	/**
	 * Adds the File to KB in defaultMt.
	 *
	 * @param file for formulas for this AssertionsCollection
	 */
	public void addFile(File file)  throws Exception  {
		addFile(file, defaultMt);
	}

	/**
	 * Adds the File to KB into Mt.
	 *
	 * @param file for formulas 
	* @param mt for formulas 
	 */
	public void addFile(File file, String mt)  throws Exception  {
		resetAssertions();
		loadFile(file);
		commitAssertions(mt);
	}


	/**
	 * Adds the File to this AssertionCollection.
	 *
	* @param file for formulas for this AssertionsCollection
	 */
	public void loadFile(File file) throws Exception {
		Reader r = new BufferedReader(new FileReader(file));
		StreamTokenizer st = new StreamTokenizer(r);
		while ( r.ready() )
			addFormula(st);
	}

	/**
	 * Adds the formula (StreamTokenizer) to this AssertionCollection.
	 *
	 * @param StreamTokenizer of the formula for this AssertionsCollection
	 */
	public void addFormula(StreamTokenizer formula) throws Exception {
		CycListParser cycListParser = new CycListParser(cycAccess);
		addFormula((CycList) cycListParser.read(formula));
	}

	/**
	 * Adds the formula (String) to this AssertionCollection.
	 *
	 * @param String of the formula for this AssertionsCollection
	 */
	public void addFormula(String formula) throws Exception {
		CycListParser cycListParser = new CycListParser(cycAccess);
		addFormula((CycList) cycListParser.read(formula));
	}


	/**
	 * Adds the formula (CycList) to this AssertionCollection.
	 *
	 * @param (CycList) of the formula for this AssertionsCollection
	 */
	public void addFormula(CycList formula) throws Exception {

		// Catches anded Assertions
		ArrayList simpler = ConstraintRule.simplifyConstraintRuleExpression(formula);
		if ( simpler.size()>1 ) {
			Iterator simpleIterations = simpler.iterator();
			while ( simpleIterations.hasNext() )
				addFormula((CycList) simpleIterations.next());
			return;
		}

		ConstraintRule cycRule = new ConstraintRule(formula);

		// Variables Or predicate argument is not a constant cause this formula to be asserted last 
		if ( cycRule.getVariables().size()>0 || ( !(formula.first() instanceof CycConstant)) ) {
			cycAssertionsOfRest.add(formula);
			return;
		}

		// Can assume only Gafs beyond here
		CycConstant cycPredicate = cycRule.getPredicate();

		if (cycPredicate.equals(cycAccess.genlMt)) {
			cycAssertionsMtDefs.add(formula);
			return;
		}
		
		if (cycPredicate.equals(cycAccess.isa)) {

				if ( cycRule.getArity()!=3 )
					throw new CycApiException("'isa' is not arity 3? - " + formula.toString());
				Object cycCollection = formula.third();

				if ( !(cycCollection instanceof CycConstant) ) {
					cycAssertionsOfRest.add(formula);
					return;
				}

				switch ( ((CycConstant)cycCollection).getId().intValue() ) {
					
					case 666:    
						// (isa ?C Microtheory)
						cycAssertionsMtDefs.add(0,formula);
					case 667:    
						// (isa ?C Collection)
						cycAssertionsDefintional.add(0,formula);
					case 668:    
						// (isa ?C Thing)
						cycAssertionsDefintional.add(0,formula);
					default:    
						// (isa ?C ?Other)
						cycAssertionsDefintional.add(formula);
				}

		}
	}

	/**
	 * Adds the AssertionCollection to defaultMt
	 *
	 */
	public void commitAssertions()  throws Exception  {
		commitAssertions(defaultMt);

	}

	/**
	 * Adds the AssertionCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 *
	 * [08:09] <eca-home> All microtheory definitions 
	 * [08:09] <eca-home>  isa and other definitional assertions) second.
	 * [08:10] <eca-home> All predicate definitions (isa, arity, argIsa, and other definitional assertions) third.
	 * [08:11] <eca-home> All collection definitions, in descending order of type, fourth.
	 * [08:11] <eca-home> e.g. ThirdOrderCollections, then SecondOrderCollections (CollectionTypes), then Collections which genls Individual.
	 * [08:11] <eca-home> All other constant definitions fifth
	 * [08:11] <eca-home> Then all other assertions -- the non-definitional ones.
	 * (and (isa ?GAF CycLClosedAtomicSentence) (operatorFormulas ?PRED ?GAF) (isa ?PRED OpenCycDefinitionalPredicate))
	 */

	public void commitAssertions(String mt)  throws Exception  {
		CycFort mtFort = ensureMt(mt);
		commitAssertions(cycAssertionsMtDefs,mtFort);
		commitAssertions(cycAssertionsDefintional,mtFort);
		commitAssertions(cycAssertionsPredicateDefs,mtFort);
		commitAssertions(cycAssertionsCollectionDefs,mtFort);
		commitAssertions(cycAssertionsSpecialCollectionDefs,mtFort);
		commitAssertions(cycAssertionsSecondaryConstantDefs,mtFort);
		commitAssertions(cycAssertionsOfRest,mtFort);
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
	public void commitAssertions(ArrayList cycListCollection,CycFort mt ) throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			CycList cycList = (CycList)  forms.next();

			if ( cycAccess.isWellFormedFormula(cycList) )
				cycAccess.assertWithTranscript(cycList,mt);


		}
	}


}







	
