/*
 * @(#)Regexp.java	1.10 96/02/02
 *
 * Copyright (c) 1995 Starwave Corporation.  All Rights Reserved.
 *
 * A perl-like regular expression matcher for java.
 *
 * @version 1.10, 02 Feb 1996
 * @author Jonathan Payne
 */

/* Regular expression compiler for java. */

package regexp;

import java.io.FileInputStream;	/* for debugging */
import java.util.Vector;

/** State of in-progress matches. */
abstract class State {
    int offset;
    int limit;
    int pstart[];
    int pend[];

    State(int offset, int limit) {
	this.offset = offset;
	this.limit = limit;
    }

    int charsLeft() {
	return limit - offset;
    }

    abstract int getchar();

    abstract int getchar(int offset);

    abstract int indexOf(int c, int offset);

    abstract int lastIndexOf(int c, int offset);

    final int getLimit() {
	return limit;
    }

    final void ensureGroup(int n) {
	if (pstart == null || n >= pstart.length) {
	    int np[] = new int[n + 4];
	    if (pstart != null)
		System.arraycopy(pstart, 0, np, 0, pstart.length);
	    pstart = np;

	    np = new int[n + 4];
	    if (pend != null)
		System.arraycopy(pend, 0, np, 0, pend.length);
	    pend = np;
	}
    }

    final void startGroup(int n) {
	ensureGroup(n);
	pstart[n] = offset + 1;
    }

    final void endGroup(int n) {
	ensureGroup(n);
	pend[n] = offset + 1;
    }

    final void validateGroup (int n) {
	if (pstart == null || n >= pstart.length
	    || pstart[n] == 0 || pend[n] == 0)
	    throw new NoSuchMatchException(": " + n);
    }

    final void clearGroup(int n) {
	validateGroup(n);
	pstart[n] = pend[n] = 0;
    }

    abstract String getGroupString(int n);

    final int getGroupLength(int n) {
	validateGroup(n);
	return pend[n] - pstart[n];
    }

    final int getGroupStart(int n) {
	validateGroup(n);
	return pstart[n] - 1;
    }

    final int getGroupEnd(int n) {
	validateGroup(n);
	return pend[n] - 1;
    }

    public String toString() {
	return "offset = " + offset + ", limit = " + limit;
    }
}

class StringState extends State {
    String s;

    StringState(String s, int offset) {
	super(offset, s.length());
	this.s = s;
    }

    int getchar() {
	return s.charAt(offset);
    }

    int getchar(int offset) {
	return s.charAt(offset);
    }

    int indexOf(int c, int offset) {
	return s.indexOf(c, offset);
    }

    int lastIndexOf(int c, int offset) {
	return s.lastIndexOf(c, offset);
    }

    public String getGroupString(int n) {
	validateGroup(n);
	return s.substring(pstart[n] - 1, pend[n] - 1);
    }
}

class CharArrayState extends State {
    char data[];

    CharArrayState(char data[], int offset, int length) {
	super(offset, length);
	this.data = data;
    }

    int getchar() {
	if (offset >= limit)
	    throw new ArrayIndexOutOfBoundsException(offset + " >= " + limit);
	return data[offset];
    }

    int getchar(int offset) {
	if (offset >= limit)
	    throw new ArrayIndexOutOfBoundsException(offset + " >= " + limit);
	return data[offset];
    }

    int indexOf(int c, int offset) {
	char data[] = this.data;
	int limit = this.limit;

	while (offset < limit) {
	    if (data[offset] == c)
		return offset;
	    offset += 1;
	}
	return -1;
    }

    int lastIndexOf(int c, int offset) {
	char data[] = this.data;

	while (offset >= 0) {
	    if (data[offset] == c)
		return offset;
	    offset -= 1;
	}
	return -1;
    }

    public String getGroupString(int n) {
	validateGroup(n);
	return String.copyValueOf(data, pstart[n] - 1,
				  pend[n] - pstart[n]);
    }
}

/** A Regexp is a piece of a regular expression.  The system is
 * designed to be at once flexible and efficient.  Therefore, very
 * little allocation is done while matching a regexp.  It's mostly all
 * done during the compilation stage.
 * 
 * Here's an example of how to use this class:
 *	import regexp.*;
 *
 *	Regexp reg = Regexp.compile("^([a-z]*) ([0-9]*)");
 *	String buffer = readFileIntoString("somefile.text");
 *	Result result;
 *	int pos = 0;
 *
 *	while ((result = reg.searchForward(buffer, pos)) != null) {
 *	    System.out.println("Matched " + result.getMatch(1)
 *	        + " and " + result.getMatch(2));
 *	    pos = result.matchEnd() + 1;
 *	}
 */

public class Regexp {
    /* The way this class works is fairly simple.  Regexp's are
     * subclassed for particular kinds of regular expressions.  They
     * are linked together in a doubly-linked list, but really only
     * the next pointer is needed at runtime (I was just being lazy at
     * compile time).  To match a regular expression, you start with
     * the first Regexp, and call the advance method.  Advance()
     * returns null on failure or "next" on success
     *
     * Some regexp subclasses are more complicated than others,
     * obviously.  Multi, which is how "*" "+" and "?" are
     * implemented, is complicated because it has to advance its child
     * regexp (the one which is being '*'d) repeatedly, and then match
     * (not advance over just one) the entire rest of the regexp.  The
     * reason it has to do that is that it has to know if the entire
     * rest of the regexp works, because if it doesn't it has to
     * backup and try again.  The alternation guy is similar: it tries
     * to "match" the first alternative (which actually continues to
     * the very end of the regexp), and if that fails, backs up and
     * tries again with the next alternative.
     *
     * The static variable "success" is used to terminate a regexp.
     * The matching code knows it has successed by whether or not it
     * sees a "success" node. */

    static final boolean debug = false;
    static Regexp success = new SuccessRegexp(null);

    public static Regexp compile(String expr) {
	return compile(expr, false);
    }

    /** Return a compiled regular expression.  A compiled expression
     * consists of a bunch of Regexp subclasses linked together in a
     * double linked list.  The only reason for prev pointers is to
     * easily handled Multi processing, where you have to splice into
     * the list sometimes.  */
    public static Regexp compile(String expr, boolean mapCase) {
	return RegexpCompiler.compile(expr, mapCase);
    }

    /** Next in re. */
    Regexp next;

    /** Previous in re (for compilation purposes only). */
    Regexp prev;

    /** Package private constructor for Regexp.  Users of this package
     * should use Regexp.compile(String expr) to create a compiled
     * regular expression, and then use match and searchForward to use
     * it.
     */
    Regexp(Regexp prev) {
	this.prev = prev;
	if (prev != null)
	    prev.next = this;
    }

    /** Returns <em>next</em> if we successfully advanced to a new
     * state, null otherwise.  This must not modify state unless it
     * advances.  To differentitate between failure and reaching the
     * end, the global Regexp <em>success</em> is used to end
     * all expressions.
     */
    Regexp advance(State state) {
	return next;
    }

    /** Backs up the state by one advance.  By default this backs up
     * by one character.  For paren back references, this might back up
     * big chunks at once.  It's up to an outer layer whether or not it
     * should even try to backup.  This exists soley for *, + and ?
     * processing.
     */
    void backup(State state) {
	state.offset -= 1;
    }

    /** Returns the single literal character that can match the
     * beginning of this Regexp.  If there is no unique first
     * character, returns -1.  This is an optimization.
     */
    int firstCharacter() {
	return -1;
    }

    /** Turns this regexp into a Multi, e.g., a *, + or ? modified
     * regexp.  For most regexp, this is simply wrapping this inside
     * a Multi, but a Literal string has a more complicated task.
     */
    Regexp makeMulti(int kind) {
	return new Multi(prev, this, kind);
    }

    /** Walks as far as it can down a regular expression, returning
     * true if it made it all the way to the end, and false otherwise.
     * When false, restores state to original state.
     */
    protected boolean match(State state) {
	Regexp reg = this;
	Regexp next;
	int offset = state.offset;

	try {
	    while ((next = reg.advance(state)) != null) {
		if (next == success)
		    return true;
		reg = next;
	    }
	} catch (StringIndexOutOfBoundsException e) {
	} catch (ArrayIndexOutOfBoundsException e) {
	}
	state.offset = offset;

	return false;
    }

    /** Indicates whether or not this type of regular expression can
     * have a *, + or ? modifier.  Most can, but ones which don't
     * advance the match offset cannot (since the * would match it
     * forever).
     */
    boolean canStar() {
	return true;
    }

    /** Returns true if the specified String is matched by this
     * regular expression.  This is not to be confused with search,
     * which looks all through the string for a match.  This just looks
     * to see if the beginning of the string matches.
     * 
     * @param data string to match
     * @return a regexp.Result on success, null on failure
     * @see Result
     */
    public Result match(String data, int offset) {
	State s = new StringState(data, offset);

	if (match(s))
	    return new Result(s);
	return null;
    }

    /** Returns true if the specified String is matched by this
     * regular expression.  This is not to be confused with search,
     * which looks all through the string for a match.  This just looks
     * to see if the beginning of the string matches.
     * 
     * @param data string to match
     * @return a regexp.Result on success, null on failure
     * @see Result
     */
    public Result match(char data[], int offset, int length) {
	State s = new CharArrayState(data, offset, length);

	if (match(s))
	    return new Result(s);
	return null;
    }

    /** Returns true if the specified String is matched anywhere by
     * this regular expression.  This is not like match, which only
     * matches at the beginning of the string.  This searches through
     * the whole string starting at the specified offset looking for a
     * match.
     * 
     * @param data string to match
     * @parem offset position to start search
     * @return a regexp.Result on success, null on failure
     * @see Result
     */
    public final Result searchForward(String data, int offset) {
	State state = new StringState(data, offset);

	return searchForward(state);
    }

    /** Returns true if the specified char array is matched anywhere by
     * this regular expression.  This is not like match, which only
     * matches at the beginning of the string.  This searches through
     * the whole string starting at the specified offset looking for a
     * match.
     * 
     * @param data string to match
     * @parem offset position to start search
     * @return a regexp.Result on success, null on failure
     * @see Result
     */
    public final Result searchForward(char data[], int offset, int length) {
	State state = new CharArrayState(data, offset, length);

	return searchForward(state);
    }

    private final Result searchForward(State state) {
	int firstc = firstCharacter();

	if (firstc != -1) {
	    /* we can do first character optimization */
	    int i;

	    while ((i = state.indexOf(firstc, state.offset)) != -1) {
		state.offset = i;
		if (match(state))
		    return new Result(state);
		state.offset = i + 1;
	    }
	} else {
	    int limit = state.getLimit();

	    while (state.offset < limit) {
		if (match(state))
		    return new Result(state);
		state.offset += 1;
	    }
	}
	return null;
    }

    /** Returns true if the specified String is matched from the
     * specified offset backward by this regular expression.  This is
     * not like match, which only matches at the beginning of the
     * string.  This searches through the whole string starting at the
     * specified offset looking for a match.
     * 
     * @param data string to match
     * @parem offset position to start search
     * @return a regexp.Result on success, null on failure
     * @see Result
     */
    public Result searchBackward(String data, int offset) {
	State state = new StringState(data, offset);

	return searchBackward(state);
    }

    /** Returns true if the specified char array is matched from the
     * specified offset backward by this regular expression.  This is
     * not like match, which only matches at the beginning of the
     * string.  This searches through the whole string starting at the
     * specified offset looking for a match.
     * 
     * @param data string to match
     * @parem offset position to start search
     * @return a regexp.Result on success, null on failure
     * @see Result
     */
    public Result searchBackward(char data[], int offset, int length) {
	State state = new CharArrayState(data, offset, length);

	return searchBackward(state);
    }

    private final Result searchBackward(State state) {
	int firstc = firstCharacter();

	if (firstc != -1) {
	    /* we can do first character optimization */
	    int i;

	    while (--state.offset >= 0 &&
		   (i = state.lastIndexOf(firstc, state.offset)) != -1) {
		state.offset = i;
		if (match(state))
		    return new Result(state);
	    }
	} else {
	    int limit = state.getLimit();

	    while (--state.offset >= 0) {
		if (match(state))
		    return new Result(state);
	    }
	}
	return null;
    }

    public static void main(String args[]) throws Exception {
	FileInputStream fin
	    = new FileInputStream(args[0]);
	byte bytes[] = new byte[fin.available()];
	if (fin.read(bytes) != bytes.length)
	    System.out.println("Huh?");
	fin.close();
	String data = new String(bytes, 0);
	boolean reverse = false;
	boolean timeit = false;

	boolean mapCase = false;
	int arg = 1;
	Regexp reg;
	while (args[arg].charAt(0) == '-') {
	    if (args[arg].equals("-time"))
		timeit = true;
	    else if (args[arg].equals("-reverse"))
		reverse = true;
	    else if (args[arg].equals("-ignore"))
		mapCase = true;
	    else
		System.out.println("Unknown option: " + args[arg]);
	    arg += 1;
	}
	reg = Regexp.compile(args[arg], mapCase);
	System.out.println("Processing " + args[arg] + " = "
			   + reg.toString());
	arg += 1;
	if (reg.firstCharacter() != -1)
	    System.out.println("First Character: "
			       + new Character((char) reg.firstCharacter()));
	if (timeit) {
	    int count = 50;

	    long start = System.currentTimeMillis();
	    for (int i = 0; i < count; i++) {
		if (reg.searchForward(data, 0) == null)
		    break;
	    }
	    System.out.println((float) ((1000.0 * count)
					/ (System.currentTimeMillis() - start))
			       + " per second.");
	} else {
	    int which = Integer.parseInt(args[arg]);
	    Result r;
	    int pos = 0;
	    while ((r = reg.searchForward(data, pos)) != null) {
		pos = r.getMatchEnd(0);
		System.out.println("Match = " + r + " match " + which + " = "
				   + r.getMatch(which));
	    }
	}
    }

    public final String toString() {
	if (next != null && next != success)
	    return toStringThis() + next.toString();
	return toStringThis();
    }

    public String toStringThis() {
	return this == success ? "<!>" : "";
    }
}

/** This subclass always matches.  This avoids a test in Regexp.match
    for "success". */
class SuccessRegexp extends Regexp {
    SuccessRegexp(Regexp prev) {
	super(prev);
    }

    protected boolean match(State state) {
	return true;
    }
}

/** Implements alternations. */
class Alternatives extends Regexp {
    Vector alts = new Vector(2);

    Alternatives(Regexp prev) {
	super(prev);
    }

    void addAlt(Regexp alt) {
	alts.addElement(alt);
    }

    int firstCharacter() {
	int limit = alts.size();
	int firstc = -1;
	for (int i = 0; i < limit; i++) {
	    Regexp reg = (Regexp) alts.elementAt(i);
	    int c = reg.firstCharacter();
	    if (firstc == -1)
		firstc = c;
	    else if (c != firstc)
		return -1;
	}
	return firstc;
    }

    Regexp advance(State state) {
	int offset = state.offset;
	int limit = alts.size();

	for (int i = 0; i < limit; i++) {
	    Regexp reg = (Regexp) alts.elementAt(i);
	    if (reg.match(state))
		return success;
	    state.offset = offset;
	}
	return null;
    }

    public boolean canStar() {
	return false;
    }

    public String toStringThis() {
	StringBuffer buf = new StringBuffer();
	int limit = alts.size();
	for (int i = 0; i < limit; i++) {
	    buf.append(alts.elementAt(i).toString());
	    if (i < limit - 1)
		buf.append("|");
	}
	return buf.toString();
    }
}

/** Regexp subclass that matches a literal string. */
class Literal extends Regexp {
    static char caseMap[];
    static {
        caseMap = new char[128];
        int i;
        for (i = 0; i < 128; i++)
            caseMap[i] = (char) i;
        for (i = 'a'; i <= 'z'; i++)
            caseMap[i] = (char) (i + ('A' - 'a'));
    }

    char data[] = new char[0];
    int count;
    boolean mapCase = false;

    Literal(Regexp prev, int c, boolean mapCase) {
	super(prev);
	this.mapCase = mapCase;
	appendChar(c);
    }

    void appendChar(int c) {
	if (count >= data.length) {
	    char nd[] = new char[data.length + 16];

	    System.arraycopy(data, 0, nd, 0, data.length);
	    data = nd;
	}
	data[count++] = (char) c;
    }

    Regexp advance(State state) {
	int cnt = count;
	int offset, i;

	offset = state.offset;

	if (state.charsLeft() < cnt)
	    return null;
	i = 0;
	if (mapCase) {
	    while (--cnt >= 0)
		if (caseMap[data[i++]] != caseMap[state.getchar(offset++)])
		    return null;
	} else {
	    while (--cnt >= 0)
		if (data[i++] != state.getchar(offset++))
		    return null;
	}
	/* success! */
	state.offset = offset;
	return next;
    }

    /** Makes a multi out of us.  If we have 1 character, we replace
        ourselves with a Multi of one character.  If we have more than
        one, then the * only applies to the last character, so we
        strip it off, leave ourselves intact, and append the Multi to
        us.  Get it? */
    Regexp makeMulti(int kind) {
	if (count == 1)
	    return new Multi(prev, this, kind);
	else {
	    count -= 1;		/* strip off last char */	    
	    return new Multi(this, new Literal(null, data[count], mapCase),
			     kind);
	}
    }			 

    int firstCharacter() {
	if (mapCase)
	    return -1;
	return data[0];
    }

    public String toStringThis() {
	return new String(data, 0, count);
    }
}

/** Regexp subclass that matches any character except \n.  So, .* will
  not move beyond a newline character. */
class Dot extends Regexp {
    public Dot(Regexp prev) {
	super(prev);
    }

    Regexp advance(State state) {
	if (state.getchar() != '\n') {
	    state.offset += 1;
	    return next;
	}
	return null;
    }

    public String toStringThis() {
	return ".";
    }
}

/** Regexp subclass that matches various contexts, such as beginning
    of line, end of line, word boundary, etc. */
class ContextMatch extends Regexp {
    static CharClass word;

    static {
	word = CharClass.cloneCharClass(null, 'w');
    }
    int kind;

    public ContextMatch(Regexp prev, int kind) {
	super(prev);
	this.kind = kind;
    }

    Regexp advance(State state) {
	boolean wordLeft, wordRight;
	int offset = state.offset;

	switch (kind) {
	  case '$':
	    if (state.charsLeft() > 0 && state.getchar() != '\n')
		return null;
	    break;

	  case '^':
	    if (offset > 0 && state.getchar(offset - 1) != '\n')
		return null;
	    break;

	  case 'b':
	  case 'B':
	    wordLeft = (offset > 0 && state.charsLeft() > 0
			&& word.charInClass(state.getchar(offset - 1)));
	    wordRight = (state.charsLeft() > 0
			 && word.charInClass(state.getchar()));
	    if ((kind == 'B') != (wordLeft == wordRight))
		return null;
	    break;
		    
	  default:
//	    throw new Exception("\\"
//				+ new Character((char) kind)
//				+ ": not supported");
	    return null;
	}
	return next;
    }

    int firstCharacter() {
	if (kind == '^')
	    return next.firstCharacter();
	return -1;
    }

    void backup(State state) {}

    boolean canStar() {
	return false;
    }

    public String toStringThis() {
	String value = "" + new Character((char) kind);

	return ((kind == 'b' || kind == 'B') ? "\\" + value : value);
    }
}

/** Regexp subclass that handles re's of the form [a-z], otherwise
    known as character classes. */
class CharClass extends Regexp {
    static final int	NCHARS = 256;
    static final int	SHIFT = 5;
    static final int	MASK = 31;

    static CharClass sClass = new CharClass(null, "[ \t\n\r]");
    static CharClass wClass = new CharClass(null, "[a-zA-Z0-9_]");
    static CharClass dClass = new CharClass(null, "[0-9]");

    /** Return a copy of a premade character class. */
    static CharClass cloneCharClass(Regexp prev, int kind) {
	CharClass cc;

	switch (kind) {
	  case 's':
	  case 'S':
	    cc = sClass;
	    break;

	  case 'w':
	  case 'W':
	    cc = wClass;
	    break;

	  case 'd':
	  case 'D':
	    cc = dClass;
	    break;

	  default:
	    throw new MalformedRegexpException("Internal exception");
	}
	cc = new CharClass(cc);
	if (Character.isUpperCase((char) kind))
	    cc.in = false;
	cc.prev = prev;
	if (prev != null)
	    prev.next = cc;
	return cc;
    }

    /** characters to match */
    int	bits[];
    
    /** boolean indicating whether characters are in the set or out of
        the set */
    boolean in = true;

    CharClass(CharClass orig) {
	super(null);
	bits = orig.bits;
	in = orig.in;
    }

    CharClass(Regexp prev, String spec) {
	super(prev);
	bits = new int[NCHARS / 32];
	process(spec);
    }

    final boolean charInClass(int c) {
	return (bits[c >> SHIFT] & (1 << (c & MASK))) != 0;
    }

    /* add a single character to this class */
    final void addChar(int c) {
	bits[c >> SHIFT] |= (1 << (c & MASK));
    }

    /* add characters ranging from c0 to c1 inclusive */
    final void addChars(int c0, int c1) {
	if (c0 > c1) {
	    int tmp = c0;
	    c0 = c1;
	    c1 = tmp;
	}
	while (c0 <= c1)
	    addChar(c0++);
    }

    /** Includes all characters from other in this class. */
    final void merge(CharClass other, boolean invert) {
	for (int i = 0; i < bits.length; i++) {
	    int otherbits = other.bits[i];

	    if (invert)
		otherbits = ~otherbits;
	    bits[i] |= otherbits;
	}
    }	

    void process(String spec) {
	int i = 1;
	int limit = spec.length() - 1;
	int c;

	if (spec.charAt(i) == '^') {
	    i += 1;
	    in = false;
	}
	while (i < limit) {
	    switch (c = spec.charAt(i++)) {
	      case '\\':
		switch(c = spec.charAt(i++)) {
		  case 'w':
		  case 'W':
		    merge(wClass, c == 'W');
		    continue;

		  case 's':
		  case 'S':
		    merge(sClass, c == 'S');
		    continue;

		  case 'd':
		  case 'D':
		    merge(dClass, c == 'D');
		    continue;

		  case 'n':
		    c = '\n';
		    break;

		  case 'r':
		    c = '\r';
		    break;

		  case 'f':
		    c = '\f';
		    break;

		  case 't':
		    c = '\t';
		    break;

		  case 'b':
		    c = '\b';
		    break;

		  default:
		    break;
		}
		/* falls through */

	      default:
		addChar(c);
		break;

	      case '-':
		if (i < limit)
		    addChars(spec.charAt(i - 2), spec.charAt(i++));
		else
		    addChar('-');
		break;
	    }
	}
    }

    Regexp advance(State state) {
	int c = state.getchar();

	if (charInClass(c) == in) {
	    state.offset += 1;
	    return next;
	}
	return null;
    }

    final String ppChar(int c) {
	String str;

	switch (c) {
	  case '\n':
	    str = "\\n";
	    break;

	  case '\r':
	    str = "\\r";
	    break;

	  case '\t':
	    str = "\\t";
	    break;

	  default:
	    if (c < ' ')
		str = "^" + new Character((char) (c + '@'));
	    else
		str = String.valueOf(new Character((char) c));
	    break;
	}
	return str;
    }

    public String toStringThis() {
	StringBuffer value = new StringBuffer("[");

	if (!in)
	    value.append("^");
	for (int i = 0; i < 255; i++) {
	    if (charInClass(i)) {
		int j;
		for (j = i + 1; j < 255; j++)
		    if (!charInClass(j))
			break;
		int count = j - i;
		value.append(ppChar(i));
		switch (count) {
		  case 1:
		    break;

		  default:
		  case 3:
		    value.append('-');

		  case 2:
		    value.append(ppChar(j - 1));
		    break;
		}
		i = j - 1;
	    }
	}
	value.append(']');
	return value.toString();
    }
}

/** Regexp subclass that handles *, + and ? processing.  The 0 or
  more, 1 or more, 0 or 1 processing is applied to another regexp,
  which is the child of this one. */
class Multi extends Regexp {
    Regexp child;
    int kind;

    Multi(Regexp prev, Regexp child, int kind) {
	super(prev);
	this.child = child;
	child.next = success;
	this.kind = kind;
    }

    Regexp advance(State state) {
	int count = 0;
	int offset = state.offset;

	switch (kind) {
	  case '*':
	    /* 1 or more matches */
	  case '+':
	    /* 0 or more matches */
	    while (child.match(state))
		count += 1;
	    if (kind == '+' && count == 0)
		return null;
	    break;

	  case '?':
	    /* zero or one matches */
	    child.match(state);
	    break;
	}
	if (debug)
	    System.out.println("Multi " + this.toStringThis()
			       + " advances from " + offset
			       + " to " + state.offset);
	while (state.offset > offset) {
	    if (next.match(state)) {
		if (debug)
		    System.out.println("Multi succeeds at " + state.offset);
		return success;
	    }
	    child.backup(state);
	    if (debug)
		System.out.println("Multi backs up to " + state.offset);
	}
	/* If we're here, we matched 0 times.  That's OK if we're
	   STAR or QUESTION, but not PLUS. */
	return (kind != '+') ? next : null;
    }

    boolean canStar() {
	return false;
    }

    public String toStringThis() {
	return child.toString() + new Character((char) kind);
    }
}

/** Regexp subclass that marks a region of a match as a group.  This
    handles the \( \) constructs. */
class Group extends Regexp {
    int kind;
    int n;

    Group(Regexp prev, int kind, int n) {
	super(prev);
	this.kind = kind;
	this.n = n;
    }

    public String toStringThis() {
	return "" + (char) kind;
    }

    Regexp advance(State s) {
	if (kind == '(')
	    s.startGroup(n);
	else
	    s.endGroup(n);
	return next;
    }

    void backup(State s) {
	try {
	    s.clearGroup(n);
	} catch (NoSuchMatchException e){}
    }

    boolean canStar() {
	return false;
    }

    int firstCharacter() {
	return next != null ? next.firstCharacter() : -1;
    }
}

/** A Regexp subclass that handles group references of the form \1 \2
    \3 constructs. */
class GroupReference extends Regexp {
    int n;

    GroupReference(Regexp prev, int n) {
	super(prev);
	this.n = n;
    }

    public String toStringThis() {
	return "\\" + new Character((char) ('0' + n));
    }

    Regexp advance(State state) {
	String group;

	try {
	    group = state.getGroupString(n);
	} catch (NoSuchMatchException e) {
	    return null;
	}
	int cnt = group.length();

	if (state.charsLeft() < cnt)
	    return null;

	int offset = state.offset;
	int i = 0;

	while (--cnt >= 0)
	    if (group.charAt(i++) != state.getchar(offset++))
		return null;
	state.offset = offset;
	return next;
    }

    void backup(State s) {
	try {
	    int len = s.getGroupLength(n);
	    s.offset -= len;
	} catch (NoSuchMatchException e) {}
    }
}
