import file.io.FileWriter;

public class w
{
  public static void main (String[] args) 
  {
    // f is the "letter"; below, we put it in an "envelope"
    FileWriter f = new FileWriter("foo.txt");

    // we can only do limited things to f: 4-5 forms of write(),
    // a flush() and a close()... and that's it

    // make the "envelope", put the "letter" in it
    PrintWriter p = new PrintWriter(f);
    // still, can do only limited things to f... but p has println()
    // and will forward that to f in terms of f.write()
  } // end of main (String[] args)
}
