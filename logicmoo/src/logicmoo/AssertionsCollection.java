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
import ViolinStrings.*;



/**
 * Provides the behavior and attributes of OpenCyc AssertionsCollection.<p>
 * <p>
 * Assertions are read from a file load(File file) or added in via addSentence()
 * The assertions may then be added to a OpenCyc server in the given setdefaultMt
 *
 *   Most of the routines are based on StreamTokenizer with settings:
 *
 *	commentChar(';');
 *      quoteChar('"');
 *	eolIsSignificant(false);
 *
 *   Unless the public void load(StreamTokenizer st) is dirrectly called
 *
 * @version $Id: AssertionsCollection.java,v 1.8 2002-04-10 03:19:33 dmiles Exp $
 * @author Douglas R. Miles
 *
 */

public class AssertionsCollection {

	/**
	 * Constructs a new AssertionsCollection.
	 *
	 * @param ca the CycAccess for this AssertionsCollection
	 */

	public AssertionsCollection(CycAccess ca) {
		cycAccess = ca;
		resetAll();
	}

	/**
	 * Constructs a new AssertionsCollection.
	 *
	 * uses a default new CycAccess()
	 */

	public AssertionsCollection() {
		try {
			cycAccess = new CycAccess();
		} catch ( Exception e ) {
		}
		resetAll();
	}

	/**
	 * Cyc api support.
	 */
	protected CycAccess cycAccess;

	/*
	 *  Assertion SubCollections
	 */

	protected ArrayList cycAssertionsMtDefs;
	protected ArrayList cycAssertionsDefintional;
	protected ArrayList cycAssertionsPredicateDefs;
	protected ArrayList cycAssertionsCollectionDefs;
	protected ArrayList cycAssertionsSpecialCollectionDefs;
	protected ArrayList cycAssertionsSecondaryConstantDefs;
	protected ArrayList cycAssertionsOfRest;
	protected ArrayList kifToCycLMap;

	/*
	 *  The defaultMt defaulted to BaseKB
	 */

	protected CycFort defaultMt = null;       

	/*
	 *  The isSuoKif flag (defaulted to false) tells this AssertionCollection
	 *  to try to convert some constants and expressions that may map to CycL from IEEE SUO-KIF
	 *
	 *  Examples:
	 *    subclass -> genls
	 *    documentation -> comment
	 * 
	 */

	protected boolean isSuoKif = false;

	/*
	 *  The CycListKifParser
	 */

	protected CycListKifParser cycListKifParser;


	protected boolean isAssertedDuringLoad = true;

	public void resetAll() {
		cycAssertionsMtDefs = new ArrayList();
		cycAssertionsDefintional = new ArrayList();
		cycAssertionsPredicateDefs = new ArrayList();
		cycAssertionsCollectionDefs = new ArrayList();
		cycAssertionsSpecialCollectionDefs = new ArrayList();
		cycAssertionsSecondaryConstantDefs = new ArrayList();
		cycAssertionsOfRest = new ArrayList();
		cycListKifParser = new CycListKifParser(cycAccess);
		cycListKifParser.verbosity= 0;
		kifToCycLMap = new ArrayList();
		makeStdSUOKIFMap();
		System.out.println("resetAll()");
	}


	/**
	 * Sets the defaultMt for this AssertionsCollection.
	 */
	public void setDefaultMt(CycFort mt) {
		defaultMt=mt;
	}

	/**
	 * Sets the defaultMt for this AssertionsCollection.
	 */
	public void setDefaultMt(String mt) throws Exception {
		defaultMt = cycAccess.makeCycConstant("#$" + mt);
		cycAccess.assertIsa(defaultMt,cycAccess.getConstantByName("#$Microtheory"),cycAccess.baseKB);
	}

	/**
	 * Returns the defaultMt for this AssertionsCollection.
	 *
	 * @return the defaultMt for this AssertionsCollection
	 */

	public CycFort getDefaultMt() {
		return defaultMt;
	}


	/**
	 * Sets the isSuoKif for this AssertionsCollection.
	 */
	public void setSuoKif(boolean kifbit) {
		isSuoKif=kifbit;
	}

	/**
	 * Returns the isSuoKif for this AssertionsCollection.
	 *
	 * @return the isSuoKif for this AssertionsCollection
	 */

	public boolean getSuoKif() {
		return isSuoKif;
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
	 * @param file for sentences for this AssertionsCollection
	 */
	public void addFile(Writer wr,File file)  throws Exception  {
		addFile( wr,file, defaultMt);
	}

	/**
	 * Adds the File to KB into Mt.
	 *
	 * @param file for sentences 
	 * @param mt for sentences 
	 */
	public void addFile(Writer wr,File file, String mt)  throws Exception  {
		setDefaultMt(mt);
		resetAll();
		load(file);
		commitAssertions( wr);
	}

	/**
	 * Adds the File to KB into Mt.
	 *
	 * @param file for sentences 
	 * @param mt for sentences 
	 */
	public void addFile(Writer wr,File file, CycFort mt)  throws Exception  {
		setDefaultMt(mt);
		resetAll();
		load(file);
		commitAssertions(wr);
	}


	/**
	 * Adds the File to this AssertionCollection.
	 *
	* @param file to load sentences for this AssertionsCollection
	 */

	public void load(File file) throws Exception {
		load((Reader)new FileReader(file));
	}

	/**
	 * Adds the contents of the InputStream to this AssertionCollection.
	 *
	* @param is for InputStream of sentences for this AssertionsCollection
	 */
	public void load(InputStream is) throws Exception {
		load((Reader)new InputStreamReader(is)); 
	}

	/**
	 * Adds the contents of the Reader to this AssertionCollection.
	 *
	 * @param r for Reader of sentences for this AssertionsCollection
	 */

	public void load(Reader r) throws Exception {
		BufferedReader br = new BufferedReader(r);
		StringBuffer sb = new StringBuffer("");
		String line;
		String[] findreplace;

		while ( (line=br.readLine()) != null ) {
			line = line.trim();
			if ( !line.startsWith(";") ) {
				if ( isSuoKif ) {
					line = Strings.change(line,"("," ( ");
					line = Strings.change(line,")"," ) ");
					line = Strings.change(line,"   "," ");
					line = Strings.change(line,"  "," ");
					line = Strings.change(line,"  "," ");
					line = Strings.change(line,"(<=>","(#$equiv ");
					line = Strings.change(line,"(=>","(#$implies ");
					line = Strings.change(line," <=> "," #$equiv ");
					line = Strings.change(line," => "," #$implies ");
					line = Strings.change(line," forall "," #$forAll ");
					line = Strings.change(line," exists "," #$thereExists ");
					line = Strings.change(line," @ROW "," ?ROWVAR1 ?ROWVAR2 ");
					Iterator it = kifToCycLMap.iterator();
					while ( it.hasNext() ) {
						findreplace = (String[]) it.next();
						line = Strings.change(line," " + findreplace[0] + " " ," " + findreplace[1] + " ");
					}
				}
				sb.append(line).append(" ");
			}
		}

		//  System.out.println(sb.toString());
		load(new StreamTokenizer(new StringReader(sb.toString())));
	}

	/**
	 * Adds the contents of the StreamTokenizer to this AssertionCollection.
	 * If loading a KIF file be sure first to:
	 *
	 *	st.commentChar(';');
	 *       st.quoteChar('"');
	 *	st.eolIsSignificant(false);
	 *
	 * @param st for sentences for this AssertionsCollection
	 */

	public void load(StreamTokenizer st) throws Exception {
		st.commentChar( ';' );
		st.ordinaryChar( '(' );
		st.ordinaryChar( ')' );
		st.ordinaryChar( '\'' );
		st.ordinaryChar( '`' );
		st.ordinaryChar( '.' );
		st.wordChars( '=', '=' );
		st.wordChars( '+', '+' );
		st.wordChars( '-', '-' );
		st.wordChars( '_', '_' );
		st.wordChars( '<', '<' );
		st.wordChars( '>', '>' );
		st.wordChars( '*', '*' );
		st.wordChars( '/', '/' );
		st.wordChars( '.', '.' );
		st.wordChars( '#', '#' );
		st.wordChars( ':', ':' );
		st.wordChars( '!', '!' );
		st.wordChars( '$', '$' );
		st.wordChars( '?', '?' );
		st.wordChars( '%', '%' );
		st.wordChars( '&', '&' );
		st.quoteChar('"');
		st.eolIsSignificant(false);
		while ( st.ttype != st.TT_EOF ) 
			addSentence(cycListKifParser.read(st));
	}

	public ArrayList getKifCycLMap() {
		return kifToCycLMap;
	}

	public void setKifCycLMap(ArrayList kifmap) {
		kifToCycLMap = kifmap;
	}

	public void clearKifCycLMap(ArrayList kifmap) {
		kifToCycLMap.clear();
	}

	public void addKifCycLMap(String kif, String cycl) {
		String[] vect = {kif,cycl};
		kifToCycLMap.add(vect);
	}

	public void makeStdSUOKIFMap() {
		addKifCycLMap("domainSubclass","argGenls");
		addKifCycLMap("domain","argIsa");
		addKifCycLMap("rangeSubclass","resultGenls");
		addKifCycLMap("range","resultIsa");
		addKifCycLMap("instance","isa");
		addKifCycLMap("subclass","genls");
		addKifCycLMap("documentation","comment");
		addKifCycLMap("Attribute","AttributeValue");
		addKifCycLMap("Collection","KIFcollection");
		addKifCycLMap("Class","Collection");
		addKifCycLMap("KIFcollection","Group");
		addKifCycLMap("Object","Indivigual");
		addKifCycLMap("Entity","Thing");                
		addKifCycLMap("subrelation","genlPreds");                
		addKifCycLMap("inverse","inversePreds");                

	}


	/**
	 * Adds the sentence (String) to this AssertionCollection.
	 *
	 * @param String of the sentence for this AssertionsCollection
	 */
	public void addSentence(String sentence) throws Exception {
		if ( sentence==null ) return;
		load(new StringReader(sentence));
	}

	/**
	 * Adds the sentence (CycList) to this AssertionCollection.
	 *
	 * @param (CycList) of the sentence for this AssertionsCollection
	 */
	public void addSentence(CycList sentence) throws Exception {
		if ( sentence==null ) return;
		System.out.println("addSentence('"+sentence+"'");


		/*
		   // Catches anded Assertions
		   ArrayList simpler = ConstraintRule.simplifyConstraintRuleExpression(sentence);
		   if ( simpler.size()>1 ) {
			   Iterator simpleIterations = simpler.iterator();
			   while ( simpleIterations.hasNext() )
				   addSentence((CycList) simpleIterations.next());
			   return;
		   }
   
		*/
		ConstraintRule cycLiteral = new ConstraintRule(sentence);
		
		System.out.println(cycLiteral.cyclify());

		// Variables Or predicate argument is not a constant cause this sentence to be asserted last 
		if ( !cycLiteral.isGround()	   /*|| ( !(sentence.first() instanceof CycConstant))*/ ) {
			addSentenceNonGround(sentence);
			return;
		}

		System.out.print("Gaf for ");

		// Can assume only Gafs beyond here

		//CycFort cycPredicate = cycLiteral.getPredicate();
		Object cycPredicate = sentence.first();

		System.out.println(cycPredicate);

		if ( !(cycPredicate instanceof CycFort) ) {
			System.out.print("!CycFort -> ");
			cycAssertionsOfRest.add(sentence);
			System.out.println("cycAssertionsOfRest");
			return;
		}

		if ( cycPredicate.equals(cycAccess.genlMt) ) {
			System.out.println("cycAssertionsMtDefs");
			cycAssertionsMtDefs.add(sentence);
			return;
		}

		if ( cycPredicate.equals(cycAccess.isa) ) {
			addSentenceIsa(sentence);
			return;
		}

		if ( cycPredicate.equals(cycAccess.genls) ) {
			addSentenceGenls(sentence);
			return;
		}

		if ( cycPredicate.equals(cycAccess.comment) ) {
			addSentenceComment(sentence);
			return;
		}

		if ( cycAccess.isa((CycFort)cycPredicate,cycAccess.getConstantByName("#$MetaRelation"))) {
			cycAssertionsPredicateDefs.add(sentence);
			return;
		}
		
		// Since Gaf add it to first part of cycAssertionsOfRest
		addSentenceGround(sentence);

	}

	/**
	 * Adds the gaf based sentence (CycList) to this AssertionCollection.
	 *
	 * @param sentence (CycList) to be added to this AssertionsCollection
	 */
	public void addSentenceGround(CycList sentence) throws Exception {
		if ( isAssertedDuringLoad )
			try {
				cycAccess.assertGaf(sentence,defaultMt);
				return;
			} catch ( Exception e ) {
			}
		cycAssertionsOfRest.add(0,sentence);
		System.out.println("addSentenceGround");
	}
	/**
	 * Adds the non gaf based sentence (CycList) to this AssertionCollection.
	 *
	 * @param sentence (CycList) to be added to this AssertionsCollection
	 */
	public void addSentenceNonGround(CycList sentence) throws Exception {
		cycAssertionsOfRest.add(sentence);
		System.out.println("addSentenceNonGround");
	}
	/**
	 * Adds the comment based sentence (CycList) to this AssertionCollection.
	 *
	 * @param sentence (CycList) to be added to this AssertionsCollection
	 */
	public void addSentenceComment(CycList sentence) throws Exception {
		if ( isAssertedDuringLoad )
			try {
			String cmt = sentence.third().toString();
				cycAccess.assertComment((CycConstant)sentence.second(),cmt,defaultMt);
				return;
			} catch ( Exception e ) {
			}

		cycAssertionsDefintional.add(0,sentence);
		System.out.println("addSentenceComment");
	}

	/**
	 * Adds the genls based sentence (CycList) to this AssertionCollection.
	 *
	 * @param sentence (CycList) to be added to this AssertionsCollection
	 */
	public void addSentenceGenls(CycList sentence) throws Exception {
		if ( isAssertedDuringLoad ) {
			try {
				cycAccess.assertIsaCollection((CycConstant)sentence.third(),defaultMt);
			} catch ( Exception e ) {
			}
			try {
				cycAccess.assertIsaCollection((CycConstant)sentence.second(),defaultMt);
			} catch ( Exception e ) {
			}
			try {
				cycAccess.assertGenls((CycFort)sentence.second(),(CycFort)sentence.third(),defaultMt);
				return;
			} catch ( Exception e ) {
			}
		}
		Object cycCollection = sentence.third();
		System.out.println("addSentenceGenls " + cycCollection);

		if ( !(cycCollection instanceof CycConstant) ) {
			cycAssertionsDefintional.add(sentence);
			return;
		}

		// Is this already a Known Collection?
		if ( cycAccess.isa((CycFort) cycCollection,(CycFort) cycAccess.collection) ) {
			cycAssertionsDefintional.add(0,sentence);
			return;
		}

		// This is not a known about collection yet
		cycAssertionsDefintional.add(sentence);
	}

	/**
	 * Adds the isa based sentence (CycList) to this AssertionCollection.
	 *
	 * @param (CycList) of the sentence for this AssertionsCollection
	 */

	public void addSentenceIsa(CycList sentence) throws Exception {

		if ( isAssertedDuringLoad ) {
			try {
				cycAccess.assertIsaCollection((CycConstant)sentence.third(),defaultMt);
			} catch ( Exception e ) {
			}
			try {
				cycAccess.assertIsa((CycFort)sentence.second(),(CycFort)sentence.third(),defaultMt);
				return;
			} catch ( Exception e ) {
			}
		}

		Object cycCollection = sentence.third();

		System.out.println("addSentenceIsa " + cycCollection);

		if ( !(cycCollection instanceof CycConstant) ) {
			cycAssertionsOfRest.add(sentence);
			return;
		}

		// Is this a Mt Defintion?
		if ( cycCollection.equals(cycAccess.getConstantByName("#$Microtheory")) ) {
			cycAccess.assertIsa((CycFort)sentence.second(),(CycFort)sentence.third(),defaultMt);
			//cycAssertionsMtDefs.add(0,sentence);
			return;
		}

		// Is this a CollectionType Defintion?
		if ( cycCollection.equals(cycAccess.getConstantByName("#$CollectionType")) ) {
			cycAssertionsDefintional.add(0,sentence);
			return;
		}

		// Is this a Collection Defintion?
		if ( cycCollection.equals(cycAccess.getConstantByName("#$Collection")) ) {
			cycAssertionsDefintional.add(0,sentence);
			return;
		}

		// Is this already a Known Collection?
		if ( cycAccess.isa((CycFort) cycCollection,(CycFort) cycAccess.collection) ) {
			cycAssertionsDefintional.add(0,sentence);
			return;
		}

		// This is not a known about collection yet
		cycAssertionsDefintional.add(sentence);

	}

	/**
	 * Adds the AssertionCollection to Mt
	*
	* @param mt for sentences for this AssertionsCollection
	*
	* [08:09] <eca-home> All microtheory definitions 
	* [08:09] <eca-home>  isa and other definitional assertions) second.
	* [08:10] <eca-home> All predicate definitions (isa, arity, argIsa, and other definitional assertions) third.
	* [08:11] <eca-home> All collection definitions, in descending order of type, fourth.
	* [08:11] <eca-home> e.g. ThirdOrderCollections, then SecondOrderCollections (CollectionTypes), then Collections which genls Individual.
	* [08:11] <eca-home> All other constant definitions fifth
	* [08:11] <eca-home> Then all other assertions -- the non-definitional ones.
	* (and (isa ?GAF CycLClosedAtomicSentence) (operatorSentences ?PRED ?GAF) (isa ?PRED OpenCycDefinitionalPredicate))
	 */

	public void commitAssertions(Writer wr,String mt)  throws Exception  {
		setDefaultMt(mt);
		commitAssertions( wr);
	}


	/**
	 * Adds the AssertionCollection to defaultMt
	 *
	 */
	public void commitAssertions(Writer wr)  throws Exception  {
		commitAssertions( wr,cycAssertionsMtDefs);
		commitAssertions( wr,cycAssertionsDefintional);
		commitAssertions( wr,cycAssertionsPredicateDefs);
		commitAssertions( wr,cycAssertionsCollectionDefs);
		commitAssertions( wr,cycAssertionsSpecialCollectionDefs);
		commitAssertions( wr,cycAssertionsSecondaryConstantDefs);
		commitAssertions( wr,cycAssertionsOfRest);
	}

	/**
	 * Adds the cycListCollection to Mt
	 *
	* @param cycListCollection of sentences for this AssertionsCollection
	* @param mt for sentences for this AssertionsCollection
	 */
	public void commitAssertions(Writer wr, ArrayList cycListCollection ) throws Exception  {
		PrintWriter pw = new PrintWriter(wr);
		Iterator forms = cycListCollection.iterator();
		if ( cycAccess == null )
			throw new CycApiException("cycAccess == null");

		while ( forms.hasNext() ) {
			CycList cycList = (CycList) forms.next();
			try {
				if (cycList != null) 
					cycAccess.assertWithTranscript(cycList,defaultMt);				
				// if ( cycAccess.isWellFormedSentence(cycList) )
			} catch ( Exception e ) {
				pw.println((((CycList)  forms.next()).cyclify()));
				e.printStackTrace( pw );
				pw.println("\n\n\n" );
			}
		}
	}


	public void writeAssertions(Writer wr)  throws Exception  {
		writeAssertions( wr,cycAssertionsMtDefs);
		writeAssertions( wr,cycAssertionsDefintional);
		writeAssertions( wr,cycAssertionsPredicateDefs);
		writeAssertions( wr,cycAssertionsCollectionDefs);
		writeAssertions( wr,cycAssertionsSpecialCollectionDefs);
		writeAssertions( wr,cycAssertionsSecondaryConstantDefs);
		writeAssertions( wr,cycAssertionsOfRest);
	}

	/**
	 * Displays the cycListCollection to Mt
	 *
	 * @param mt for sentences for this AssertionsCollection
	 */
	public void writeAssertions(Writer wr,ArrayList cycListCollection) throws Exception  {
		PrintWriter pw = new PrintWriter(wr);
		Iterator forms = cycListCollection.iterator();
		while ( forms.hasNext() )
			pw.println( (((CycList)  forms.next()).toString()));

	}

	/**
	 * Tests the File loading
	 *
	 * @param mt for sentences for this AssertionsCollection
	 */

	public void testFileLoad() {
		testFileLoad(new PrintWriter(System.out));
	}


	public void testFileLoad(Writer wr) {
		try {
			File f = new File("/testkifload.kif");
			isSuoKif = true;
			load(f);
			setDefaultMt("TestkifloadMt");
			commitAssertions( wr);
		} catch ( Exception e ) {
			try {
				wr.write(e + "\n");
			} catch ( Exception ee ) {
			}
			PrintWriter pw = new PrintWriter(wr);
			e.printStackTrace( pw );
		}

	}

}










