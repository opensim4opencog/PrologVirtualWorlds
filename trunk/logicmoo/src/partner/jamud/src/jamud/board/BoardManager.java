package jamud.board;


import java.util.*;


public class BoardManager {


    private Set boards;



    public BoardManager() {
	this.boards = Collections.synchronizedSet( new HashSet() );
    }



    public Iterator boards() {
	return this.boards.iterator();
    }


    public boolean addBoard(Board board) {
	return this.boards.add( board );
    }


    public boolean removeBoard(Board board) {
	return this.boards.remove( board );
    }


    public boolean removeBoard(String name) {
	for(Iterator i = this.boards.iterator(); i.hasNext(); ) {
	    if( ((Board)i.next()).getName().equals(name) ) {
		i.remove();
		return true;
	    }
	}
	return false;
    }


    public Board getBoard(String name) {
	for(Iterator i = this.boards.iterator(); i.hasNext(); ) {
	    Board b = (Board) i.next();
	    if( b.getName().equals(name) ) {
		return b;
	    }
	}
	return null;
    }

}
