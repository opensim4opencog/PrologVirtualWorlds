package jbf;

public class CharVector extends Char_Vector {

    protected void makespace(int index, int len)
    {
       if((index < 0) || (index > _length)) {
            throw new ArrayIndexOutOfBoundsException();
        }
        extendto(_length+len);
        System.arraycopy(_contents,index,_contents,index+len,_length - index);
        _length += len;
    }

    public CharVector() {super();}

    public CharVector(int initsize) {super(initsize);}

    public CharVector(String s) {super(s.length()); append(s);}
    public CharVector(StringBuffer s) {super(s.length()); append(s);}

    public void ensureCapacity(int siz) {extendto(siz);}

    public char charAt(int index) {return elementAt(index);}

    public void getChars(int srcBegin, int srcEnd, char dst[], int dstBegin)
    {
        if((srcBegin < 0) || (srcBegin >= _length)) {
            throw new ArrayIndexOutOfBoundsException(srcBegin);
        }
        if((srcEnd < 0) || (srcEnd > _length)) {
            throw new ArrayIndexOutOfBoundsException(srcEnd);
        }
        if(srcBegin < srcEnd) {
            System.arraycopy(_contents, srcBegin, dst, dstBegin, srcEnd - srcBegin);
        }
    }

    public void setCharAt(int index, char ch) {storeElementAt(ch,index);}

    public CharVector append(Object obj) {return append(String.valueOf(obj));}

    public CharVector append(String s)
    {
        if(s == null) {return this;}
        int l = s.length();
        extendto(_length+l);
	if(l > 0) s.getChars(0, l, _contents, _length);
        _length += l;
        return this;
    }

    public CharVector append(StringBuffer s)
    {
        if(s == null) {return this;}
        int l = s.length();
        extendto(_length+l);
        s.getChars(0, l, _contents, _length);
        _length += l;
        return this;
    }

    public CharVector append(char s[]) {return append(s,0,s.length);}

    public CharVector append(char s[], int offset, int len)
    {
        extendto(_length + len);
        System.arraycopy(s, offset, _contents, _length, len);
        _length += len;
        return this;
    }

    public CharVector append(char c) {addElement(c); return this;}

    public CharVector append(boolean b) {return append(String.valueOf(b));}

    public CharVector append(int i) {return append(String.valueOf(i));}

    public CharVector append(long l) {return append(String.valueOf(l));}

    public CharVector append(float f) {return append(String.valueOf(f));}

    public CharVector append(double d) {return append(String.valueOf(d));}

    public CharVector insert(int offset, Object obj)
	{return insert(offset, String.valueOf(obj));}

    public CharVector insert(int index, String s)
    {
	int l = s.length();
	makespace(index,l);
        s.getChars(0, l, _contents, index);
        return this;
    }

    public CharVector store(int index, String s)
    {
	if((index < 0) || (index > _length)) {
            throw new ArrayIndexOutOfBoundsException();
        }
	int l = s.length();
        extendto(_length+l);
        System.arraycopy(_contents,index,_contents,index+l,_length - index);
        _length += l;
        s.getChars(0, l, _contents, index);
	if(index + l > _length) _length = index+l;
        return this;
    }

    public CharVector insert(int index, char s[])
	{return insert(index,s,0,s.length);}

    public CharVector insert(int index, char s[], int offset, int len)
    {
	if(offset+len > s.length) {
            throw new ArrayIndexOutOfBoundsException(offset+"");
	}
	makespace(index,len);
        System.arraycopy(s, offset, _contents, index, len);
        return this;
    }

    public CharVector store(int index, char s[], int offset, int len)
    {
	if(offset+len > s.length) {
            throw new ArrayIndexOutOfBoundsException(offset+"");
	}
        if(index < 0 || index > _length) {
            throw new ArrayIndexOutOfBoundsException(index+"");
        }
        extendto(index+len);
        System.arraycopy(s, offset, _contents, index, len);
	if(index + len > _length) _length = index+len;
        return this;
    }

    public CharVector insert(int index, boolean b)
	{return insert(index, String.valueOf(b));}

    public CharVector insert(int index, char c)
    {
	makespace(index,1);
        _contents[index] = c;
        return this;
    }

    public CharVector insert(int index, int i)
	{return insert(index, String.valueOf(i));}

    public CharVector insert(int index, long l)
	{return insert(index, String.valueOf(l));}

    public CharVector insert(int index, float f)
	{return insert(index, String.valueOf(f));}

    public CharVector insert(int index, double d)
	{return insert(index, String.valueOf(d));}

    public String toString() {return new String(_contents,0,_length);}

    public StringBuffer toStringBuffer()
    {
	StringBuffer sb = new StringBuffer(_length);
	return sb.append(_contents,0,_length);
    }
    public void inStringBuffer(StringBuffer sb)
	{sb.setLength(0); sb.append(_contents,0,_length);}

    //////////////////////////////////////////////////
    // Allow this object to be used iin hash tables

    public int hashCode() {
	int h = 0;
	int l = _length;
	int i = 0;

	if(l < 16) {
	    for (i=0, l=_length;l > 0;l--) {h = (h * 37) + _contents[i++];}
	} else {
	    // only sample some characters
	    int skip = _length / 8;
	    for (i=1, l=_length;l > 0;l -= skip, i += skip) {
		h = (h * 39) + _contents[i];
	    }
	}
	return h;
    }

    // match strings, stringbuffers, or or CharVector
    public boolean equals(Object o)
    {
	int l;
	if(o == null) return false;
	if(o instanceof CharVector) {
	    CharVector x = (CharVector)o;
	    if(x.length() != _length) return false;
	    char[] y = x.contents();
	    for(l = _length - 1;l >= 0;l--) {
		if(_contents[l] != y[l]) return false;
	    }
	    return true;
	} else if(o instanceof String) {
	    String x = (String)o;
	    if(x.length() != _length) return false;
	    for(l = _length - 1;l >= 0;l--) {
		if(_contents[l] != x.charAt(l)) return false;
	    }
	    return true;
	} else if(o instanceof StringBuffer) {
	    StringBuffer x = (StringBuffer)o;
	    if(x.length() != _length) return false;
	    for(l = _length - 1;l >= 0;l--) {
		if(_contents[l] != x.charAt(l)) return false;
	    }
	    return true;
	}
	return false;
    }


};
