package cycmoo.lang;  
import cycmoo.lang.fluent.*;
import cycmoo.lang.object.*;
import cycmoo.lang.builtin.*;  //
import java.util.*;
import java.io.*;
/**
  All blackboard Objects implement Term.
*/
public interface ITermReader {
    abstract public void setReader(Reader r);
    abstract public Reader getReader();
    abstract public boolean isError(HornClause C);
    abstract public boolean atEOF();
    abstract public boolean atEOC();
    abstract public int lineno();
    abstract public HornClause readClause();

}

