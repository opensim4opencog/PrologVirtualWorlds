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
 * Assertions are read from a file loadFile(File file) or added in via addFormula()
 * The assertions may then be added to a OpenCyc server in the given setDefaultKB
 *
 * @version $Id: AssertionsCollection.java,v 1.6 2002-04-08 03:08:18 dmiles Exp $
 * @author Douglas R. Miles
 *
 */

public class AssertionsCollection {

	/**
	 * Constructs a new AssertionsCollection.
	 *
	 * @param ca the CycAccess for this AssertionsCollection
	 */
	
	public void AssertionsCollection(CycAccess ca) {
		cycAccess = ca;
		resetAssertions();
	}
	
	/**
	 * Constructs a new AssertionsCollection.
	 *
	 * uses a default new CycAccess()
	 */
	
	public void AssertionsCollection() {
		try {
			cycAccess = new CycAccess();
		} catch (Exception e) {
		}
		resetAssertions();
	}

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
	private CycAccess cycAccess;


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

		ConstraintRule cycLiteral = new ConstraintRule(formula);

		// Variables Or predicate argument is not a constant cause this formula to be asserted last 
		if ( !cycLiteral.isGround() || ( !(formula.first() instanceof CycConstant)) ) {
			cycAssertionsOfRest.add(formula);
			return;
		}

		// Can assume only Gafs beyond here
		CycConstant cycPredicate = cycLiteral.getPredicate();

		if ( cycPredicate.equals(cycAccess.genlMt) ) {
			cycAssertionsMtDefs.add(formula);
			return;
		}

		if ( cycPredicate.equals(cycAccess.isa) ) {
			if ( cycLiteral.getArity()!=3 )
				throw new CycApiException("'isa' is not arity 3? - " + formula.toString());
			addFormulaIsa(formula);
			return;
		}

		if ( cycPredicate.equals(cycAccess.genls) ) {
			if ( cycLiteral.getArity()!=3 )
				throw new CycApiException("'genls' is not arity 3? - " + formula.toString());
			addFormulaGenls(formula);
			return;
		}

		// Since Gaf add it to first part of cycAssertionsOfRest
		cycAssertionsOfRest.add(0,formula);
	}

	/**
	 * Adds the genls based formula (CycList) to this AssertionCollection.
	 *
	 * @param formula (CycList) to be added to this AssertionsCollection
	 */
	public void addFormulaGenls(CycList formula) throws Exception {
		Object cycCollection = formula.third();

		if ( !(cycCollection instanceof CycConstant) ) {
			cycAssertionsOfRest.add(formula);
			return;
		}

		// Is this already a Known Collection?
		if ( cycAccess.isa((CycFort) cycCollection,(CycFort) cycAccess.collection) ) {
			cycAssertionsDefintional.add(0,formula);
			return;
		}

		// This is not a known about collection yet
		cycAssertionsDefintional.add(formula);
	}

	/**
	 * Adds the isa based formula (CycList) to this AssertionCollection.
	 *
	 * @param (CycList) of the formula for this AssertionsCollection
	 */

	public void addFormulaIsa(CycList formula) throws Exception {

		Object cycCollection = formula.third();

		if ( !(cycCollection instanceof CycConstant) ) {
			cycAssertionsOfRest.add(formula);
			return;
		}

		// Is this a Mt Defintion?
		if ( cycCollection.equals(cycAccess.getConstantByName("Microtheory")) ) {
			cycAssertionsMtDefs.add(0,formula);
			return;
		}

		// Is this a CollectionType Defintion?
		if ( cycCollection.equals(cycAccess.getConstantByName("CollectionType")) ) {
			cycAssertionsDefintional.add(0,formula);
			return;
		}

		// Is this a Collection Defintion?
		if ( cycCollection.equals(cycAccess.getConstantByName("Collection")) ) {
			cycAssertionsDefintional.add(0,formula);
			return;
		}

		// Is this already a Known Collection?
		if ( cycAccess.isa((CycFort) cycCollection,(CycFort) cycAccess.collection) ) {
			cycAssertionsDefintional.add(0,formula);
			return;
		}

		// This is not a known about collection yet
		cycAssertionsDefintional.add(formula);

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
	 * Adds the AssertionCollection to defaultMt
	 *
	 */
	public void commitAssertions()  throws Exception  {
		commitAssertions(defaultMt);

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
	 * Adds the cycListCollection to Mt
	 *
	* @param cycListCollection of formulas for this AssertionsCollection
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


	public void writeAssertions(Writer pw,String mt)  throws Exception  {
		CycFort mtFort = ensureMt(mt);
		writeAssertions( pw,cycAssertionsMtDefs,mtFort);
		writeAssertions( pw,cycAssertionsDefintional,mtFort);
		writeAssertions( pw,cycAssertionsPredicateDefs,mtFort);
		writeAssertions( pw,cycAssertionsCollectionDefs,mtFort);
		writeAssertions( pw,cycAssertionsSpecialCollectionDefs,mtFort);
		writeAssertions( pw,cycAssertionsSecondaryConstantDefs,mtFort);
		writeAssertions( pw,cycAssertionsOfRest,mtFort);
	}

	/**
	 * Displays the cycListCollection to Mt
	 *
	 * @param mt for formulas for this AssertionsCollection
	 */
	public void writeAssertions(Writer pw,ArrayList cycListCollection,CycFort mt ) throws Exception  {
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() ) {
			pw.write( (((CycList)  forms.next()).toString()) + "\n" );
		}
	}

	/**
	 * Tests the File loading
	 *
	 * @param mt for formulas for this AssertionsCollection
	 */

	public void testFileLoad() {
		testFileLoad(new PrintWriter(System.out));
	}


	public void testFileLoad(Writer pw) {
		try {
			File f = new File("/testkifload.kif");
			loadFile(f);
			setDefaultMt("testMt");
			writeAssertions( pw,defaultMt);
		} catch ( Exception e ) {
			try {
				pw.write(e + "\n");
			} catch (Exception ee) {
			}
			PrintWriter ppw = new PrintWriter(pw);
			e.printStackTrace( ppw );
		}

	}

}










