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
 * Assertions are read from a file load(File file) or added in via addOrder()
 * The assertions may then be added to a OpenCyc server in the given setdefaultMt
 *
 *   Most of the routines are based on StreamTokenizer with settings:
 *
 *	commentChar(';');
 *      quoteChar('"');
 *	eolIsSignificant(false);
 *
 *   Unless the public boolean load(StreamTokenizer st) is dirrectly called
 *
 * @version $Id: AssertionsCollection.java,v 1.9 2002-04-12 15:29:24 dmiles Exp $
 * @author Douglas R. Miles
 *
 */

public class AssertionsCollection {

    /**
     * Constructs a new AssertionsCollection.
     *
     * @param ca the CycMooAccess for this AssertionsCollection
     */

    public AssertionsCollection(CycMooAccess ca) throws Exception {
	cycMooAccess = ca;
	if ( cycMooAccess == null ) throw new Exception("cycMooAccess is null");
	resetAll();
    }


    /**
     * Verbosity indicator <tt>0</tt> indicates quiet on a range of
     * <tt>0</tt> ... <tt>10</tt>
     */
    public static int verbosity = 0;

    /**
     * Cyc api support.
     */
    protected CycMooAccess cycMooAccess;
    protected CycConstant defaultCyclist = null;
    protected CycConstant defaultProject = null;
    protected CycFort defaultMt = null;     
    /*
     *  Assertion SubCollections
     */

    protected ArrayList cycAssertionsForBaseKB;
    protected ArrayList cycAssertionsDefintional;
    protected ArrayList cycAssertionsPredicateDefs;
    protected ArrayList cycAssertionsCollectionDefs;
    protected ArrayList cycAssertionsSpecialCollectionDefs;
    protected ArrayList cycAssertionsSecondaryConstantDefs;
    protected ArrayList cycAssertionsOfRest;
    protected ArrayList kifToCycLMap;
    protected ArrayList cycOrderedAssertions;
    protected ArrayList cycUnOrderedAssertions;





    /*
     *  The isSuoKif flag (defaulted to false) tells this AssertionCollection
     *  to try to convert some constants and expressions that may map to CycL from IEEE SUO-KIF
     *
     *  Examples:
     *    subclass -> genls
     *    documentation -> comment
     * 
     */

    public boolean isSuoKif = true;
    public boolean isOrderedLoad = true;
    public boolean isTestedClean = false;

    /*
     *  The CycListKifParser
     */

    protected CycListKifParser cycListKifParser;


    protected boolean isAssertedDuringLoad = false;

    public void resetAll() throws Exception {

	cycOrderedAssertions = new ArrayList();
	cycUnOrderedAssertions = new ArrayList();
	cycAssertionsForBaseKB = new ArrayList();
	cycAssertionsDefintional = new ArrayList();
	cycAssertionsPredicateDefs = new ArrayList();
	cycAssertionsCollectionDefs = new ArrayList();
	cycAssertionsSpecialCollectionDefs = new ArrayList();
	cycAssertionsSecondaryConstantDefs = new ArrayList();
	cycAssertionsOfRest = new ArrayList();
	setDefaultCyclist("CycAdministrator");
	setDefaultProject("OpenCycProject");
	setDefaultMt("BaseKB");
	cycListKifParser = new CycListKifParser(cycMooAccess);
	cycListKifParser.verbosity= 0;
	this.clearKifCycLMap();
	this.makeStdSUOKIFMap();

	// System.out.println("resetAll()");
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
     * Sets the defaultMt for this AssertionsCollection.
     */
    public void setDefaultMt(String mt) throws Exception {
	setDefaultMt(cycMooAccess.makeCycConstant("#$" + mt));
    }

    /**
     * Sets the defaultMt for this AssertionsCollection.
     */
    public void setDefaultMt(CycFort mt) throws Exception {
	cycMooAccess.assertIsa(mt,cycMooAccess.microtheory,cycMooAccess.baseKB);
	defaultMt=mt;
    }

    /**
     * Returns the defaultProject for this AssertionsCollection.
     *
     * @return the defaultProject for this AssertionsCollection
     */

    public CycConstant getDefaultProject() {
	return defaultProject;
    }

    /**
     * Sets the defaultProject for this AssertionsCollection.
     */
    public void setDefaultProject(String project) throws Exception {
	setDefaultProject(cycMooAccess.makeCycConstant("#$" + project));
    }

    /**
     * Sets the defaultProject for this AssertionsCollection.
     */
    public void setDefaultProject(CycConstant project) throws Exception {
	cycMooAccess.setKePurpose("OpenCycProject");
	cycMooAccess.setCyclist("#$CycAdministrator");
	cycMooAccess.assertIsa(project,cycMooAccess.cycBasedProject,cycMooAccess.baseKB);
	defaultProject=project;
	cycMooAccess.setKePurpose(defaultProject);
    }


    public CycConstant getDefaultCyclist() {
	return defaultCyclist;
    }

    /**
     * Sets the defaultCyclist for this AssertionsCollection.
     */
    public void setDefaultCyclist(String cyclist) throws Exception {
	setDefaultCyclist(cycMooAccess.makeCycConstant("#$" + cyclist));
    }

    /**
     * Sets the defaultCyclist for this AssertionsCollection.
     */
    public void setDefaultCyclist(CycConstant cyclist) throws Exception {
	cycMooAccess.setKePurpose("OpenCycProject");
	cycMooAccess.setCyclist("#$CycAdministrator");
	cycMooAccess.assertIsa(cyclist,cycMooAccess.cyclist,cycMooAccess.cyclistDefinitionalMt);
	defaultCyclist=cyclist;
	cycMooAccess.setCyclist(defaultCyclist);
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
	StringWriter sr = new StringWriter(255);
	PrintWriter pw = new PrintWriter(sr);
	pw.println("; AssertionsCollection: " + defaultMt + ": " + this.hashCode() );
	writeAssertions(pw);
	return sr.toString();
    }

    /**
     * Returns a cyclified <tt>String</tt> representation of the <tt>AssertionsCollection</tt>.
     *
     * @return a cyclified <tt>String</tt> representation of the <tt>AssertionsCollection</tt>
     */
    public String cyclify () {
	StringWriter sr = new StringWriter(255);
	PrintWriter pw = new PrintWriter(sr);
	pw.println("; AssertionsCollection: " + defaultMt.cyclify() + ": " + this.hashCode() );
	writeAssertions(pw);
	return sr.toString();
    }


    /**
     * Adds the File to KB in defaultMt.
     *
     * @param file for sentences for this AssertionsCollection
     */
    public void addFile(Writer feedbackMl,File file)  throws Exception  {
	addFile( feedbackMl,file, defaultMt);
    }

    /**
     * Adds the File to KB into Mt.
     *
     * @param file for sentences 
     * @param mt for sentences 
     */
    public void addFile(Writer feedbackMl,File file, String mt)  throws Exception  {
	setDefaultMt(mt);
	resetAll();
	load(file);
	commitAssertions( feedbackMl);
    }

    /**
     * Adds the File to KB into Mt.
     *
     * @param file for sentences 
     * @param mt for sentences 
     */
    public void addFile(Writer feedbackMl,File file, CycFort mt)  throws Exception  {
	setDefaultMt(mt);
	resetAll();
	load(file);
	commitAssertions(feedbackMl);
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
	    line = line.trim() + " ";
	    if ( !line.startsWith(";") ) {
		if ( isSuoKif ) {
		    line = Strings.change(" " + line,"("," ( ");
		    line = Strings.change(line,")"," ) ");
		    line = Strings.change(line,"     "," ");
		    line = Strings.change(line,"   "," ");
		    line = Strings.change(line,"  "," ");
		    line = Strings.change(line,"  "," ");
		    line = Strings.change(line," <=> "," equiv ");
		    line = Strings.change(line," => "," implies ");
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
		// System.out.println(line);
	    }
	}

	//  // System.out.println(sb.toString());
	load(new StreamTokenizer(new StringReader(sb.toString())));
    }

    /**
     * Adds the contents of the StreamTokenizer to this AssertionCollection.
     * If loading a KIF file be sure first to:
     *
     *	st.commentChar(';');
     *      st.quoteChar('"');
     *	st.eolIsSignificant(false);
     *
     * @param st for sentences for this AssertionsCollection
     */

    public void load(StreamTokenizer st) throws Exception {
	st.commentChar( ';' );   st.ordinaryChar( '(' ); st.ordinaryChar( ')' ); st.ordinaryChar( '\'' );
	st.ordinaryChar( '`' );  st.ordinaryChar( '.' ); st.wordChars( '=', '=' ); st.wordChars( '+', '+' );
	st.wordChars( '-', '-' );  st.wordChars( '_', '_' ); st.wordChars( '<', '<' ); st.wordChars( '>', '>' );
	st.wordChars( '*', '*' );  st.wordChars( '/', '/' );  st.wordChars( '.', '.' );  st.wordChars( '#', '#' );
	st.wordChars( ':', ':' );  st.wordChars( '!', '!' );  st.wordChars( '$', '$' );  st.wordChars( '?', '?' );
	st.wordChars( '%', '%' );  st.wordChars( '&', '&' );  st.quoteChar('"');  st.eolIsSignificant(false);
	while ( st.ttype != st.TT_EOF )
	    addSentence(cycListKifParser.read(st));
    }

    public ArrayList getKifCycLMap() {
	return kifToCycLMap;
    }

    public void setKifCycLMap(ArrayList kifmap) {
	kifToCycLMap = kifmap;
    }

    public void clearKifCycLMap() {
	if ( kifToCycLMap == null )  kifToCycLMap = new ArrayList();
	kifToCycLMap.clear();
    }

    public void addKifCycLMap(String kif, String cycl) {
	if ( kifToCycLMap == null )  kifToCycLMap = new ArrayList();
	String[] vect = {kif,cycl};
	kifToCycLMap.add(vect);
    }

    public void makeStdSUOKIFMap() {
	/*
	addKifCycLMap("equal","equals");
	addKifCycLMap("domainSubclass","argGenl");
	addKifCycLMap("domain","argIsa");
	addKifCycLMap("rangeSubclass","resultGenl");
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
	addKifCycLMap("inverse","genlInverse");                
	addKifCycLMap("Formula","ELFormula");                
	addKifCycLMap("valence","arity");                
	addKifCycLMap("subAttribute","genlAttributes");   
	addKifCycLMap("agent","actors");   
	addKifCycLMap("holds"," ");   
	addKifCycLMap("Process","Event");   
	addKifCycLMap("MegaByte","Megabyte");  
	addKifCycLMap("SymbolicString","TextString"); 
	*/ 
    }


    /**
     * Adds the sentence (String) to this AssertionCollection.
     *
     * @param String of the sentence for this AssertionsCollection
     */
    public boolean addSentence(String sentence) throws Exception {
	if ( sentence==null ) return false;
	load(new StringReader(sentence));
	return true;
    }

    /**
     * Adds the sentence (String) to this AssertionCollection.
     *
     * @param String of the sentence for this AssertionsCollection
     */
    public boolean addSentence(CycList sentence) throws Exception {
	if ( sentence==null ) return false;
	cycUnOrderedAssertions.add(sentence);
	return true;
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

    public void orderAssertions() {
	try {

	    if ( isOrderedLoad ) {
		cycOrderedAssertions = cycUnOrderedAssertions;
	    } else {
		Iterator unordered = cycUnOrderedAssertions.iterator();
		while ( unordered.hasNext() ) addOrder((CycList)unordered.next());
		cycOrderedAssertions = new ArrayList();
		concatArrayList(cycOrderedAssertions,cycAssertionsCollectionDefs);
		concatArrayList(cycOrderedAssertions,cycAssertionsDefintional);
		concatArrayList(cycOrderedAssertions,cycAssertionsPredicateDefs);
		concatArrayList(cycOrderedAssertions,cycAssertionsSpecialCollectionDefs);
		concatArrayList(cycOrderedAssertions,cycAssertionsSecondaryConstantDefs);
		concatArrayList(cycOrderedAssertions,cycAssertionsOfRest);
	    }
	} catch ( Exception e ) {
	    System.err.println(e);
	}

    }


    /**
     * Adds the sentence (CycList) to this AssertionCollection.
     *
     * @param (CycList) of the sentence for this AssertionsCollection
     */

    public boolean addOrder(CycList sentence) throws Exception {
	if ( sentence==null ) return false;

	if ( isOrderedLoad ) {
	    cycAssertionsOfRest.add(sentence);
	    return true;
	}

	/* ArrayList simpler = ConstraintRule.simplifyConstraintRuleExpression(sentence);
	       if ( simpler.size()>1 ) {
	       Iterator simpleIterations = simpler.iterator();
	       while ( simpleIterations.hasNext() )
	       addOrder((CycList) simpleIterations.next());
	       return;
	       } */

	// Variables cause this sentence to be asserted last 
	if ( !(new ConstraintRule(sentence).isGround()) ) return addOrderNonGround(sentence);

	Object cycPredicate = sentence.first();

	// Predicate argument is not a constant cause (Can assume only Gafs beyond here)
	if ( !(cycPredicate instanceof CycFort) )  return addOrderNonFortPred(sentence);

	CycFort cycFortPredicate = (CycFort) cycPredicate;

	if ( cycFortPredicate.equals(cycMooAccess.genlMt) ) return  addOrderGenlMt(sentence);

	if ( cycFortPredicate.equals(cycMooAccess.isa) )  return addOrderIsa(sentence);

	if ( cycFortPredicate.equals(cycMooAccess.genls) ) return  addOrderGenls(sentence);

	if ( cycFortPredicate.equals(cycMooAccess.comment) )  return addOrderComment(sentence);

	if ( cycMooAccess.isMetaRelation(cycFortPredicate ))
	    return addOrderMetaRelation(cycFortPredicate.cyclify(), sentence);

	// Since Gaf add it to first part of cycAssertionsOfRest
	return addOrderGround(sentence);
    }


    /**
     * Adds the MetaRelation based sentence (CycList) to this AssertionCollection.
     *
     * @param sentence (CycList) to be added to this AssertionsCollection
     */
    public boolean addOrderGenlMt(CycList sentence) {
	// System.out.println("cycAssertionsForBaseKB: " + sentence.cyclify());
	cycAssertionsForBaseKB.add(sentence);
	return true;
    }

    /**
     * Adds the MetaRelation based sentence (CycList) to this AssertionCollection.
     *
     * @param sentence (CycList) to be added to this AssertionsCollection
     */
    public boolean addOrderMetaRelation(String cycPredicate, CycList sentence) throws Exception  {
	/*
	try {
	    if (cycPredicate.equals("#$argIsa") || cycPredicate.equals("#$argGenl")) {
	 //   addIsa((CycConstant)sentence.second(),"#$Relation");
	    addCollection((CycConstant)sentence.fourth());
	}
	if (cycPredicate.equals("#$resultIsa") || cycPredicate.equals("#$resultGenl")) {
	    addFunction((CycConstant)sentence.second());
	    addCollection((CycConstant)sentence.third());
	}
	if (cycPredicate.equals("#genlInverse") || cycPredicate.equals("#genlPreds")) {
	    addPredicate((CycConstant)sentence.second());
	    addPredicate((CycConstant)sentence.third());
	}
	} catch ( Exception e) {
	
	}
	 */   
	cycAssertionsPredicateDefs.add(sentence);
	// System.out.println("addOrderMetaRelation: " + sentence.cyclify());

	return true;
    }

    /**
     * Adds the gaf based sentence (CycList) to this AssertionCollection.
     *
     * @param sentence (CycList) to be added to this AssertionsCollection
     */
    public boolean addOrderNonFortPred(CycList sentence) {
	cycAssertionsOfRest.add(sentence);
	// System.out.println("addOrderNonFortPred: " + sentence.cyclify());
	return true;
    }


    /**
     * Adds the gaf based sentence (CycList) to this AssertionCollection.
     *
     * @param sentence (CycList) to be added to this AssertionsCollection
     */
    public boolean addOrderGround(CycList sentence) throws Exception {
	Iterator args = sentence.iterator();
	CycConstant pred = (CycConstant) args.next();
	addPredicate(pred);

	cycAssertionsOfRest.add(0,sentence);
	// System.out.println("addOrderGround:"  + sentence.cyclify());
	return true;
    }
    /**
     * Adds the non gaf based sentence (CycList) to this AssertionCollection.
     *
     * @param sentence (CycList) to be added to this AssertionsCollection
     */
    public boolean addOrderNonGround(CycList sentence) throws Exception {
	cycAssertionsOfRest.add(sentence);
	// System.out.println("addOrderNonGround:" + sentence.cyclify());
	return true;
    }
    /**
     * Adds the comment based sentence (CycList) to this AssertionCollection.
     *
     * @param sentence (CycList) to be added to this AssertionsCollection
     */
    public boolean addOrderComment(CycList sentence) throws Exception {

	cycAssertionsDefintional.add(sentence);
	// System.out.println("addOrderComment: "  + sentence.cyclify());
	return true;
    }

    /**
     * Adds the genls based sentence (CycList) to this AssertionCollection.
     *
     * @param sentence (CycList) to be added to this AssertionsCollection
     */
    public boolean addOrderGenls(CycList sentence) throws Exception {
	addCollection((CycConstant)sentence.second());
	addCollection((CycConstant)sentence.third());
	cycAssertionsDefintional.add(sentence);
	return true;
    }

    public void addCollection(CycConstant collection)   throws Exception {
	cycAssertionsCollectionDefs.add(cycMooAccess.makeGaf(cycMooAccess.isa,collection,cycMooAccess.collection));
    }

    public void addPredicate(CycConstant predicate)   throws Exception {
	addIsa(predicate,"#$Predicate");
    }

    public void addIsa(CycConstant term, String type)   throws Exception {
	cycAssertionsCollectionDefs.add(cycMooAccess.makeIsa(term,type));
    }


    /**
     * Adds the isa based sentence (CycList) to this AssertionCollection.
     *
     * @param (CycList) of the sentence for this AssertionsCollection
     */

    public boolean addOrderIsa(CycList sentence) throws Exception {
	try {
	    Object cycCollection = sentence.third();

	    // System.out.println("addOrderIsa: " + sentence.cyclify());

	    if ( !(cycCollection instanceof CycConstant) ) {
		cycAssertionsOfRest.add(sentence);
		return true;
	    }

	    // Is this a Mt Defintion?
	    if ( cycCollection.equals(cycMooAccess.microtheory) ) {
		cycAssertionsForBaseKB.add(0,sentence);
		return true;
	    }

	    addCollection((CycConstant)sentence.third());
	    cycAssertionsDefintional.add(sentence);

	    // Is this a Collection Defintion?
	    if ( cycCollection.equals(cycMooAccess.getConstantByName("#$Collection")) ) {
		addCollection((CycConstant)sentence.second());
		return true;
	    }

	    // Is this already a Known Collection?
	    if ( cycMooAccess.isa((CycFort) cycCollection,(CycFort) cycMooAccess.collection) ) {
		cycAssertionsDefintional.add(0,sentence);
		return true;
	    }

	    // This is not a known about collection yet
	    cycAssertionsDefintional.add(sentence);
	} catch ( Exception e ) {
	}

	return true;

    }


    public static void concatArrayList(ArrayList holder, ArrayList list) {
	if ( list == null ) return;
	Iterator listit = list.iterator();

	while ( listit.hasNext() ) {
	    Object insert  = listit.next();
	    if ( !holder.contains(insert) )
		holder.add(insert);
	}
    }

    /**
     * Adds the AssertionCollection to defaultMt
     *
     */
    private int numberText = 0;

    public void commitAssertions(Writer feedbackMl)  throws Exception  {
	PrintWriter pw;

	if (!isTestedClean) throw new Exception("Assertions not tested as clean.");
	
	if ( feedbackMl == null )
	    pw = new PrintWriter(System.out);
	else
	    pw = new PrintWriter(feedbackMl);
	commitArrayList( pw,cycAssertionsForBaseKB,cycMooAccess.baseKB);
	commitArrayList( pw,cycOrderedAssertions, defaultMt);
    }

    public boolean testAssertionsHtml(Writer feedbackMl)  throws Exception  {
	PrintWriter pw ;

	if ( feedbackMl == null )
	    pw = new PrintWriter(System.out);
	else
	    pw = new PrintWriter(feedbackMl);

	numberText = 0;
	pw.println("<h2>;  Testing Assertions...  </h2>");
	startTextArea(pw,20);

	orderAssertions();

	CycFort saveMt = defaultMt;
	isTestedClean = true;
	setDefaultMt("TestKifLoadTempMt");
	CycConstant hypMt = (CycConstant) defaultMt;
	cycMooAccess.assertGenlMt(hypMt,saveMt);
	setDefaultCyclist("CycAdministrator");
	setDefaultProject("OpenCycProject");
	testArrayList(pw,cycAssertionsForBaseKB, cycMooAccess.baseKB);
	testArrayList(pw,cycOrderedAssertions , hypMt);
	try {
	    // pw.println("Killing hypothetical Mt: " + hypMt.cyclify());
	    cycMooAccess.kill(hypMt);
	} catch ( Exception e ) {
	    pw.println(e);
	}
	setDefaultMt(saveMt);
	endTextArea(pw);

	if ( isTestedClean ) {
	    pw.println("<br><h2>; Assertions Consistant</h2>");
	} else {
	    pw.println("<br><h2>;  Assertions Inconsistant </h2>");
	}
	return isTestedClean;
    }

    private void startTextArea(PrintWriter pw ,int rows) {
	pw.println("<p><textarea cols=89 rows=" + rows + " name=ta"+numberText+">");
	numberText++;
    }

    private void endTextArea(PrintWriter pw) {
	pw.println("</textarea>");
    }

    private void commitArrayList(PrintWriter pw, ArrayList cycArrayList , CycFort mt) {
	Iterator forms;
	forms = cycArrayList.iterator();

	while ( forms.hasNext() ) {
	    try {
		cycMooAccess.assertWithTranscriptNoWffCheck( (CycList) forms.next(),mt);
	    } catch ( Exception e ) {
		pw.println("<pre><font color=red>"+e+"</font></pre>");
	    }
	}
    }

    private void testArrayList(PrintWriter pw, ArrayList cycArrayList , CycFort mt ) {
	String pretty = "(error)";
	int prettyLines=2;
	Iterator forms;
	CycList cycList = null;
	int assertN = 0;
	forms = cycArrayList.iterator();

	while ( forms.hasNext() ) {
	    try {
		assertN++;
		cycList = (CycList) forms.next();
		pretty = cycList.toPrettyString("   ");
		prettyLines = Strings.occurrencesOf(pretty,"\n");
		cycMooAccess.assertWithTranscript(cycList,mt);
		pw.println("; " + assertN + "\n" + cycList.toPrettyString("   ") +"\n");
	    } catch ( Exception e ) {
		isTestedClean = false;
		pw.println("</textarea><pre><font color=red>"+e+"</font></pred>");
		startTextArea(pw,prettyLines + 5);
		pw.println("; " + assertN +"\n" + pretty);
	    }
	}

    }

    public void writeAssertions(Writer feedbackMl) {
	orderAssertions();
	writeAssertions(feedbackMl,cycAssertionsForBaseKB);
	writeAssertions(feedbackMl,cycOrderedAssertions);
    }

    public void writeAssertionsCycL(Writer feedbackMl) {
	orderAssertions();
	writeAssertionsCycL(feedbackMl,cycAssertionsForBaseKB);
	writeAssertionsCycL(feedbackMl,cycOrderedAssertions);
    }

    /**
     * Displays the cycArrayList to Mt
     *
     * @param mt for sentences for this AssertionsCollection
     */
    public void writeAssertions(Writer feedbackMl,ArrayList cycArrayList) {
	PrintWriter pw = new PrintWriter(feedbackMl);
	try {
	    Iterator forms = cycArrayList.iterator();
	    while ( forms.hasNext() )
		pw.println(((CycList)forms.next()).toString());
	} catch ( Exception e ) {
	    pw.println("; exception: " + e);
	}
    }

    /**
     * Displays the cycArrayList to Mt
     *
     * @param mt for sentences for this AssertionsCollection
     */
    public void writeAssertionsCycL(Writer feedbackMl,ArrayList cycArrayList) {
	PrintWriter pw = new PrintWriter(feedbackMl);
	try {
	    Iterator forms = cycArrayList.iterator();
	    while ( forms.hasNext() )
		pw.println(((CycList)forms.next()).cyclify());
	} catch ( Exception e ) {
	    pw.println("; exception: " + e);
	}
    }
}










