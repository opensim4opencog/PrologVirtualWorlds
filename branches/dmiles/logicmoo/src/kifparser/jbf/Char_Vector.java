package jbf;


import java.util.*;

public
class Char_Vector {

    protected static final int defaultsize = 16;

    protected int _length;
    protected char _contents[];
    protected int _delta;

    //////////////////////////////////////////////////

    public Char_Vector(int initsize) {this(initsize,0);}
    public Char_Vector() {this(defaultsize);}

    public
    Char_Vector(int initsize, int delta)
    {
	super();
	_contents = new char[initsize];
	_delta = delta;
    }

    public
    Char_Vector(Char_Vector v)
    {
	this(v.size());
	System.arraycopy(v.contents(),0,_contents,0,v.size());
    }

    //////////////////////////////////////////////////

    public int capacity() {return _contents.length;}
    public int size() {return _length;}
    public int length() {return _length;}
    public boolean isEmpty() {return _length == 0;}

    //////////////////////////////////////////////////

    public char[] contents() { return _contents;}
    public void contents(char v[]) { _contents = v; }
    public void setLength(int newlen) {setSize(newlen);}
    public boolean contains(char e) {return indexOf(e, 0) >= 0;}
    public int indexOf(char e) {return indexOf(e, 0);}
    public int lastIndexOf(char e) {return lastIndexOf(e, _length);}
    public char firstElement() {return elementAt(0);}
    public char lastElement() {return elementAt(_length-1);}
    public void copy(Char_Vector v) {fill(v.contents(),v.size());}
    public void fill(char v[]) {fill(v,v.length);}
    public void fill(char v[], int count) {fill(v,0,count);}
    public void removeAllElements() {removeElementRange(0,_length);}
    public void removeLastElement() {removeElementAt(_length-1);}
    public void addVector(Char_Vector v) {insertVectorAt(v,_length);}

    //////////////////////////////////////////////////

    protected
    void
    extendto(int sz)
    {
	int oldsz = _contents.length;
	if(sz > oldsz) {
	    char newbuf[];
	    if(oldsz == 0) oldsz = defaultsize;
	    int newsz = oldsz;
	    if(_delta > 0) {
		while(newsz < sz) newsz += _delta;
	    } else {
		while(newsz < sz) newsz *= 2;
	    }
	    newbuf = new char[newsz];
	    if(oldsz > 0) System.arraycopy(_contents, 0, newbuf, 0, _length);
	    _contents = newbuf;
	}
    }

    public
    void
    setSize(int newsz)
    {
	if(newsz > _length) {
	    extendto(newsz);
// the following is suppressed on the assumption
// that the elementype is a scalar
//	} else {
//	    int i;
//	    for(i=newsz;i < _length;i++) {
//		_contents[i] = 0;
//	    }
	}
	_length = newsz;
    }

    public
    int
    indexOf(char e, int index)
    {
	int i;
	for(i=index;i < _length;i++) {
	    if(e == _contents[i]) return i;
	}
	return -1;
    }

    public
    int
    lastIndexOf(char e, int index)
    {
	int i;
	for(i=index-1;i >= 0;i--) {
	    if(e == _contents[i]) return i;
	}
	return -1;
    }

    public
    char
    elementAt(int index)
    {
	if(index < 0 || index >= _length) {
	    throw new ArrayIndexOutOfBoundsException(""+index);
	}
	return _contents[index];
    }

    public
    void
    setElementAt(char e, int index)
    {
	if(index < 0 || index >= _length) {
	    throw new ArrayIndexOutOfBoundsException(index + "");
	}
	_contents[index] = e;
    }

    public
    void
    removeElementAt(int index)
    {
	if(index < 0 || index >= _length) {
	    throw new ArrayIndexOutOfBoundsException(index + "");
	}
	int i = _length - index - 1;
	if(i > 0) {
	    System.arraycopy(_contents, index+1, _contents, index, i);
	}
	_length--;
// again, assume elemtype is scalar
//	_contents[_length] = 0;
    }

    public
    void
    storeElementAt(char e, int index)
    {
	if(index < 0 || index >= _length) {
	    throw new ArrayIndexOutOfBoundsException(index + "");
	}
	_contents[index] = e;
    }

    public
    void
    insertVectorAt(Char_Vector v, int index)
    {
	int l = v.size();
	if(index < 0 || index > _length ) {
	    throw new ArrayIndexOutOfBoundsException(index + "");
	}
	extendto(_length+l);
	System.arraycopy(_contents,index,_contents,index+l,_length - index);
	System.arraycopy(v.contents(),0,_contents,index,l);
	_length += l;
    }

    public
    void
    addElement(char e)
    {
	extendto(_length+1);
	_contents[_length++] = e;
    }

    public
    boolean
    removeElement(char e)
    {
	int i = indexOf(e);
	if(i >= 0) {
	    removeElementAt(i);
	    return true;
	}
	return false;
    }

    public
    void
    removeElementRange(int first, int count)
    {
	if(count < 0 || first < 0 || first+count > _length) {
	    throw new ArrayIndexOutOfBoundsException("range "+first+"/"+count);
	}
	int mv = (_length - count);
	if(count > 0 && mv > 0) {
	    System.arraycopy(_contents,first+count,_contents,0,mv);
	}
	_length -= count;
// again, assume elemtype is scalar
//	for(int i=0;i<count;i++) {_contents[_length+i] = 0;}
    }

    public
    Object
    clone() throws CloneNotSupportedException
    {
	Char_Vector v = (Char_Vector)super.clone();
	char newbuf[] = new char[_length];
	System.arraycopy(_contents, 0, newbuf, 0, _length);
	v.contents(newbuf);
	return v;
    }

    public
    void
    fill(char v[], int offset, int count)
    {
	setSize(count);
	System.arraycopy(v, offset, _contents, 0, count);
    }

    public
    String
    toString()
    {
	int i;
	int len = size();
	StringBuffer b = new StringBuffer();

	b.append("[");
	for(i=0;i < len;i++) {
	    if(i > 0) b.append(", ");
	    b.append(_contents[i]);
	}
	b.append("]");
	return b.toString();
    }


    public
    void
    insertElementAt(char e, int index)
    {
	if(index < 0 || index >= _length + 1) {
	    throw new ArrayIndexOutOfBoundsException(index + "");
	}
	extendto(_length+1);
	System.arraycopy(_contents,index,_contents,index+1,_length - index);
	_contents[index] = e;
	_length++;
    }

}
