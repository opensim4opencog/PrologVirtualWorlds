package jp.ac.kobe_u.cs.prolog.lang;

import static jp.ac.kobe_u.cs.prolog.lang.StaticProlog.*;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.io.StreamCorruptedException;
import java.util.AbstractCollection;
import java.util.AbstractMap;
import java.util.AbstractSet;
import java.util.Collection;
import java.util.Collections;
import java.util.ConcurrentModificationException;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Set;

/**
 * <code>Hashtable&lt;Object,Object&gt;</code>.<br>
 * <font color="red">This document is under construction.</font>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
public class HashtableOfTerm<V> extends AbstractMap<Object, V> implements Map<Object, V>, Cloneable, java.io.Serializable {

  /**
   * The hash table data.
   */
  private transient Entry[] table;

  /**
   * The total number of entries in the hash table.
   */
  private transient int count;

  /**
   * The table is rehashed when its size exceeds this threshold.  (The
   * value of this field is (int)(capacity * loadFactor).)
   *
   * @serial
   */
  private int threshold;

  /**
   * The load factor for the hashtable.
   *
   * @serial
   */
  private float loadFactor;

  /**
   * The number of times this Hashtable has been structurally modified
   * Structural modifications are those that change the number of entries in
   * the Hashtable or otherwise modify its internal structure (e.g.,
   * rehash).  This field is used to make iterators on Collection-views of
   * the Hashtable fail-fast.  (See ConcurrentModificationException).
   */
  private transient int modCount = 0;

  /** use serialVersionUID from JDK 1.0.2 for interoperability */
  private static final long serialVersionUID = 1421746759512286392L;

  /**
   * Constructs a new, empty hashtable with the specified initial
   * capacity and the specified load factor.
   *
   * @param      initialCapacity   the initial capacity of the hashtable.
   * @param      loadFactor        the load factor of the hashtable.
   * @exception  IllegalArgumentException  if the initial capacity is less
   *             than zero, or if the load factor is nonpositive.
   */
  public HashtableOfTerm(int initialCapacity, float loadFactor) {
    if (initialCapacity < 0) throw new IllegalArgumentException("Illegal Capacity: " + initialCapacity);
    if ((loadFactor <= 0) || Float.isNaN(loadFactor)) throw new IllegalArgumentException("Illegal Load: " + loadFactor);

    if (initialCapacity == 0) initialCapacity = 1;
    this.loadFactor = loadFactor;
    this.table = new Entry[initialCapacity];
    this.threshold = (int) (initialCapacity * loadFactor);
  }

  /**
   * Constructs a new, empty hashtable with the specified initial capacity
   * and default load factor (0.75).
   *
   * @param     initialCapacity   the initial capacity of the hashtable.
   * @exception IllegalArgumentException if the initial capacity is less
   *              than zero.
   */
  public HashtableOfTerm(int initialCapacity) {
    this(initialCapacity, 0.75f);
  }

  /**
   * Constructs a new, empty hashtable with a default initial capacity (11)
   * and load factor (0.75).
   */
  public HashtableOfTerm() {
    this(11, 0.75f);
  }

  /**
   * Constructs a new hashtable with the same mappings as the given
   * Map.  The hashtable is created with an initial capacity sufficient to
   * hold the mappings in the given Map and a default load factor (0.75).
   *
   * @param t the map whose mappings are to be placed in this map.
   * @throws NullPointerException if the specified map is null.
   * @since   1.2
   */
  public HashtableOfTerm(Map<? extends Object, ? extends V> t) {
    this(Math.max(2 * t.size(), 11), 0.75f);
    this.putAll(t);
  }

  /**
   * Returns the number of keys in this hashtable.
   *
   * @return  the number of keys in this hashtable.
   */
  @Override
  public synchronized int size() {
    return this.count;
  }

  /**
   * Tests if this hashtable maps no keys to values.
   *
   * @return  <code>true</code> if this hashtable maps no keys to values;
   *          <code>false</code> otherwise.
   */
  @Override
  public synchronized boolean isEmpty() {
    return this.count == 0;
  }

  /**
   * Returns an enumeration of the keys in this hashtable.
   *
   * @return  an enumeration of the keys in this hashtable.
   * @see     Enumeration
   * @see     #elements()
   * @see     #keySet()
   * @see     Map
   */
  public synchronized Enumeration<Object> keys() {
    return this.<Object> getEnumeration(HashtableOfTerm.KEYS);
  }

  /**
   * Returns an enumeration of the values in this hashtable.
   * Use the Enumeration methods on the returned object to fetch the elements
   * sequentially.
   *
   * @return  an enumeration of the values in this hashtable.
   * @see     java.util.Enumeration
   * @see     #keys()
   * @see     #values()
   * @see     Map
   */
  public synchronized Enumeration<V> elements() {
    return this.<V> getEnumeration(HashtableOfTerm.VALUES);
  }

  /**
   * Tests if some key maps into the specified value in this hashtable.
   * This operation is more expensive than the {@link #containsKey
   * containsKey} method.
   *
   * <p>Note that this method is identical in functionality to
   * {@link #containsValue containsValue}, (which is part of the
   * {@link Map} interface in the collections framework).
   *
   * @param      value   a value to search for
   * @return     <code>true</code> if and only if some key maps to the
   *             <code>value</code> argument in this hashtable as
   *             determined by the <tt>equals</tt> method;
   *             <code>false</code> otherwise.
   * @exception  NullPointerException  if the value is <code>null</code>
   */
  public synchronized boolean contains(Object value) {
    if (value == null) {
      throw new NullPointerException();
    }

    final Entry tab[] = this.table;
    for (int i = tab.length; i-- > 0;) {
      for (Entry<Object, V> e = tab[i]; e != null; e = e.next) {
        if (e.value.equals(value)) {
          return true;
        }
      }
    }
    return false;
  }

  /**
   * Returns true if this hashtable maps one or more keys to this value.
   *
   * <p>Note that this method is identical in functionality to {@link
   * #contains contains} (which predates the {@link Map} interface).
   *
   * @param value value whose presence in this hashtable is to be tested
   * @return <tt>true</tt> if this map maps one or more keys to the
   *         specified value
   * @throws NullPointerException  if the value is <code>null</code>
   * @since 1.2
   */
  @Override
  public boolean containsValue(Object value) {
    return this.contains(value);
  }

  /**
   * Tests if the specified object is a key in this hashtable.
   *
   * @param   key   possible key
   * @return  <code>true</code> if and only if the specified object
   *          is a key in this hashtable, as determined by the
   *          <tt>equals</tt> method; <code>false</code> otherwise.
   * @throws  NullPointerException  if the key is <code>null</code>
   * @see     #contains(Object)
   */
  @Override
  public synchronized boolean containsKey(Object key) {
    final Entry tab[] = this.table;
    final int hash = StaticProlog.hashCode(key);
    final int index = (hash & 0x7FFFFFFF) % tab.length;
    for (Entry<Object, V> e = tab[index]; e != null; e = e.next) {
      if ((e.hash == hash) && StaticProlog.prologEquals(e.key, key)) {
        return true;
      }
    }
    return false;
  }

  /**
   * Returns the value to which the specified key is mapped,
   * or {@code null} if this map contains no mapping for the key.
   *
   * <p>More formally, if this map contains a mapping from a key
   * {@code k} to a value {@code v} such that {@code (key.equals(k))},
   * then this method returns {@code v}; otherwise it returns
   * {@code null}.  (There can be at most one such mapping.)
   *
   * @param key the key whose associated value is to be returned
   * @return the value to which the specified key is mapped, or
   *         {@code null} if this map contains no mapping for the key
   * @throws NullPointerException if the specified key is null
   * @see     #put(Object, Object)
   */
  @Override
  public synchronized V get(Object keyIn) {
    final Object key = keyIn;
    final Entry tab[] = this.table;
    final int hash = StaticProlog.hashCode(key);
    final int index = (hash & 0x7FFFFFFF) % tab.length;
    for (Entry<Object, V> e = tab[index]; e != null; e = e.next) {
      if ((e.hash == hash) && StaticProlog.prologEquals(e.key, key)) {
        return e.value;
      }
    }
    return null;
  }

  /**
   * Increases the capacity of and internally reorganizes this
   * hashtable, in order to accommodate and access its entries more
   * efficiently.  This method is called automatically when the
   * number of keys in the hashtable exceeds this hashtable's capacity
   * and load factor.
   */
  protected void rehash() {
    final int oldCapacity = this.table.length;
    final Entry[] oldMap = this.table;

    final int newCapacity = oldCapacity * 2 + 1;
    final Entry[] newMap = new Entry[newCapacity];

    this.modCount++;
    this.threshold = (int) (newCapacity * this.loadFactor);
    this.table = newMap;

    for (int i = oldCapacity; i-- > 0;) {
      for (Entry<Object, V> old = oldMap[i]; old != null;) {
        final Entry<Object, V> e = old;
        old = old.next;

        final int index = (e.hash & 0x7FFFFFFF) % newCapacity;
        e.next = newMap[index];
        newMap[index] = e;
      }
    }
  }

  /**
   * Maps the specified <code>key</code> to the specified
   * <code>value</code> in this hashtable. Neither the key nor the
   * value can be <code>null</code>. <p>
   *
   * The value can be retrieved by calling the <code>get</code> method
   * with a key that is equal to the original key.
   *
   * @param      key     the hashtable key
   * @param      value   the value
   * @return     the previous value of the specified key in this hashtable,
   *             or <code>null</code> if it did not have one
   * @exception  NullPointerException  if the key or value is
   *               <code>null</code>
   * @see     Object#equals(Object)
   * @see     #get(Object)
   */
  @Override
  public synchronized V put(Object key, V value) {
    // Make sure the value is not null
    if (value == null) {
      throw new NullPointerException();
    }

    // Makes sure the key is not already in the hashtable.
    Entry tab[] = this.table;
    final int hash = StaticProlog.hashCode(key);
    int index = (hash & 0x7FFFFFFF) % tab.length;
    for (Entry<Object, V> e = tab[index]; e != null; e = e.next) {
      if ((e.hash == hash) && StaticProlog.prologEquals(e.key, key)) {
        final V old = e.value;
        e.value = value;
        return old;
      }
    }

    this.modCount++;
    if (this.count >= this.threshold) {
      // Rehash the table if the threshold is exceeded
      this.rehash();

      tab = this.table;
      index = (hash & 0x7FFFFFFF) % tab.length;
    }

    // Creates the new entry.
    final Entry<Object, V> e = tab[index];
    tab[index] = new Entry<Object, V>(hash, key, value, e);
    this.count++;
    return null;
  }

  /**
   * Removes the key (and its corresponding value) from this
   * hashtable. This method does nothing if the key is not in the hashtable.
   *
   * @param   key   the key that needs to be removed
   * @return  the value to which the key had been mapped in this hashtable,
   *          or <code>null</code> if the key did not have a mapping
   * @throws  NullPointerException  if the key is <code>null</code>
   */

  @Override
  public synchronized V remove(Object key) {
    final Entry tab[] = this.table;
    final int hash = StaticProlog.hashCode(key);
    final int index = (hash & 0x7FFFFFFF) % tab.length;
    for (Entry<Object, V> e = tab[index], prev = null; e != null; prev = e, e = e.next) {
      if ((e.hash == hash) && StaticProlog.prologEquals(e.key, key)) {
        this.modCount++;
        if (prev != null) {
          prev.next = e.next;
        } else {
          tab[index] = e.next;
        }
        this.count--;
        final V oldValue = e.value;
        e.value = null;
        return oldValue;
      }
    }
    return null;
  }

  /**
   * Copies all of the mappings from the specified map to this hashtable.
   * These mappings will replace any mappings that this hashtable had for any
   * of the keys currently in the specified map.
   *
   * @param t mappings to be stored in this map
   * @throws NullPointerException if the specified map is null
   * @since 1.2
   */
  @Override
  public synchronized void putAll(Map<? extends Object, ? extends V> t) {
    for (final Map.Entry<? extends Object, ? extends V> e : t.entrySet())
      this.put(e.getKey(), e.getValue());
  }

  /**
   * Clears this hashtable so that it contains no keys.
   */
  @Override
  public synchronized void clear() {
    final Entry tab[] = this.table;
    this.modCount++;
    for (int index = tab.length; --index >= 0;)
      tab[index] = null;
    this.count = 0;
  }

  /**
   * Creates a shallow copy of this hashtable. All the structure of the
   * hashtable itself is copied, but the keys and values are not cloned.
   * This is a relatively expensive operation.
   *
   * @return  a clone of the hashtable
   */
  @Override
  public synchronized Object clone() {
    try {
      final HashtableOfTerm<V> t = (HashtableOfTerm<V>) super.clone();
      t.table = new Entry[this.table.length];
      for (int i = this.table.length; i-- > 0;) {
        t.table[i] = (this.table[i] != null) ? (Entry<Object, V>) this.table[i].clone() : null;
      }
      t.keySet = null;
      t.entrySet = null;
      t.values = null;
      t.modCount = 0;
      return t;
    } catch (final CloneNotSupportedException e) {
      // this shouldn't happen, since we are Cloneable
      throw new InternalError();
    }
  }

  /**
   * Returns a string representation of this <tt>Hashtable</tt> object
   * in the form of a set of entries, enclosed in braces and separated
   * by the ASCII characters "<tt>,&nbsp;</tt>" (comma and space). Each
   * entry is rendered as the key, an equals sign <tt>=</tt>, and the
   * associated element, where the <tt>toString</tt> method is used to
   * convert the key and element to strings.
   *
   * @return  a string representation of this hashtable
   */
  @Override
  public synchronized String toString() {
    final int max = this.size() - 1;
    if (max == -1) return "{}";

    final StringBuilder sb = new StringBuilder();
    final Iterator<Map.Entry<Object, V>> it = this.entrySet().iterator();

    sb.append('{');
    for (int i = 0;; i++) {
      final Map.Entry<Object, V> e = it.next();
      final Object key = e.getKey();
      final V value = e.getValue();
      sb.append(key == this ? "(this Map)" : key.toString());
      sb.append('=');
      sb.append(value == this ? "(this Map)" : value.toString());
      if (i == max) return sb.append('}').toString();
      sb.append(", ");
    }
  }

  private static class EmptyEnumeration<E> implements Enumeration<E> {
    static final EmptyEnumeration<Object> EMPTY_ENUMERATION = new EmptyEnumeration<Object>();

    public boolean hasMoreElements() {
      return false;
    }

    public E nextElement() {
      throw new NoSuchElementException();
    }
  }

  private <T> Enumeration<T> getEnumeration(int type) {
    if (this.count == 0) {
      return (Enumeration<T>) EmptyEnumeration.EMPTY_ENUMERATION;
      //return Collections.emptyEnumeration();
    } else {
      return new Enumerator<T>(type, false);
    }
  }

  private <T> Iterator<T> getIterator(int type) {
    if (this.count == 0) {
      return Collections.EMPTY_SET.iterator();
    } else {
      return new Enumerator<T>(type, true);
    }
  }

  // Views

  /**
   * Each of these fields are initialized to contain an instance of the
   * appropriate view the first time this view is requested.  The views are
   * stateless, so there's no reason to create more than one of each.
   */
  private transient volatile Set<Object> keySet = null;
  private transient volatile Set<Map.Entry<Object, V>> entrySet = null;
  private transient volatile Collection<V> values = null;

  static <T> Set<T> synchronizedSet(Set<T> s, Object mutex) {
    return new SynchronizedSet<T>(s, mutex);
  }

  /**
   * @serial include
   */
  static class SynchronizedCollection<E> implements Collection<E>, Serializable {
    private static final long serialVersionUID = 3053995032091335093L;

    final Collection<E> c; // Backing Collection
    final Object mutex; // Object on which to synchronize

    SynchronizedCollection(Collection<E> c) {
      if (c == null) throw new NullPointerException();
      this.c = c;
      this.mutex = this;
    }

    SynchronizedCollection(Collection<E> c, Object mutex) {
      this.c = c;
      this.mutex = mutex;
    }

    public int size() {
      synchronized (this.mutex) {
        return this.c.size();
      }
    }

    public boolean isEmpty() {
      synchronized (this.mutex) {
        return this.c.isEmpty();
      }
    }

    public boolean contains(Object o) {
      synchronized (this.mutex) {
        return this.c.contains(o);
      }
    }

    public Object[] toArray() {
      synchronized (this.mutex) {
        return this.c.toArray();
      }
    }

    public <T> T[] toArray(T[] a) {
      synchronized (this.mutex) {
        return this.c.toArray(a);
      }
    }

    public Iterator<E> iterator() {
      return this.c.iterator(); // Must be manually synched by user!
    }

    public boolean add(E e) {
      synchronized (this.mutex) {
        return this.c.add(e);
      }
    }

    public boolean remove(Object o) {
      synchronized (this.mutex) {
        return this.c.remove(o);
      }
    }

    public boolean containsAll(Collection<?> coll) {
      synchronized (this.mutex) {
        return this.c.containsAll(coll);
      }
    }

    public boolean addAll(Collection<? extends E> coll) {
      synchronized (this.mutex) {
        return this.c.addAll(coll);
      }
    }

    public boolean removeAll(Collection<?> coll) {
      synchronized (this.mutex) {
        return this.c.removeAll(coll);
      }
    }

    public boolean retainAll(Collection<?> coll) {
      synchronized (this.mutex) {
        return this.c.retainAll(coll);
      }
    }

    public void clear() {
      synchronized (this.mutex) {
        this.c.clear();
      }
    }

    @Override
    public String toString() {
      synchronized (this.mutex) {
        return this.c.toString();
      }
    }

    private void writeObject(ObjectOutputStream s) throws IOException {
      synchronized (this.mutex) {
        s.defaultWriteObject();
      }
    }
  }

  /**
   * @serial include
   */
  static class SynchronizedSet<E> extends SynchronizedCollection<E> implements Set<E> {
    private static final long serialVersionUID = 487447009682186044L;

    SynchronizedSet(Set<E> s) {
      super(s);
    }

    SynchronizedSet(Set<E> s, Object mutex) {
      super(s, mutex);
    }

    @Override
    public boolean equals(Object o) {
      synchronized (this.mutex) {
        return this.c.equals(o);
      }
    }

    @Override
    public int hashCode() {
      synchronized (this.mutex) {
        return this.c.hashCode();
      }
    }
  }

  /**
   * Returns a {@link Set} view of the keys contained in this map.
   * The set is backed by the map, so changes to the map are
   * reflected in the set, and vice-versa.  If the map is modified
   * while an iteration over the set is in progress (except through
   * the iterator's own <tt>remove</tt> operation), the results of
   * the iteration are undefined.  The set supports element removal,
   * which removes the corresponding mapping from the map, via the
   * <tt>Iterator.remove</tt>, <tt>Set.remove</tt>,
   * <tt>removeAll</tt>, <tt>retainAll</tt>, and <tt>clear</tt>
   * operations.  It does not support the <tt>add</tt> or <tt>addAll</tt>
   * operations.
   *
   * @since 1.2
   */
  @Override
  public Set<Object> keySet() {
    if (this.keySet == null) this.keySet = HashtableOfTerm.synchronizedSet(new KeySet(), this);
    return this.keySet;
  }

  private class KeySet extends AbstractSet<Object> {
    @Override
    public Iterator<Object> iterator() {
      return HashtableOfTerm.this.getIterator(HashtableOfTerm.KEYS);
    }

    @Override
    public int size() {
      return HashtableOfTerm.this.count;
    }

    @Override
    public boolean contains(Object o) {
      return HashtableOfTerm.this.containsKey(o);
    }

    @Override
    public boolean remove(Object o) {
      return HashtableOfTerm.this.remove(o) != null;
    }

    @Override
    public void clear() {
      HashtableOfTerm.this.clear();
    }
  }

  /**
   * Returns a {@link Set} view of the mappings contained in this map.
   * The set is backed by the map, so changes to the map are
   * reflected in the set, and vice-versa.  If the map is modified
   * while an iteration over the set is in progress (except through
   * the iterator's own <tt>remove</tt> operation, or through the
   * <tt>setValue</tt> operation on a map entry returned by the
   * iterator) the results of the iteration are undefined.  The set
   * supports element removal, which removes the corresponding
   * mapping from the map, via the <tt>Iterator.remove</tt>,
   * <tt>Set.remove</tt>, <tt>removeAll</tt>, <tt>retainAll</tt> and
   * <tt>clear</tt> operations.  It does not support the
   * <tt>add</tt> or <tt>addAll</tt> operations.
   *
   * @since 1.2
   */
  @Override
  public Set<Map.Entry<Object, V>> entrySet() {
    if (this.entrySet == null) this.entrySet = HashtableOfTerm.synchronizedSet(new EntrySet(), this);
    return this.entrySet;
  }

  private class EntrySet extends AbstractSet<Map.Entry<Object, V>> {
    @Override
    public Iterator<Map.Entry<Object, V>> iterator() {
      return HashtableOfTerm.this.getIterator(HashtableOfTerm.ENTRIES);
    }

    @Override
    public boolean add(Map.Entry<Object, V> o) {
      return super.add(o);
    }

    @Override
    public boolean contains(Object o) {
      if (!(o instanceof Map.Entry)) return false;
      final Map.Entry<Object, V> entry = (Map.Entry) o;
      final Object key = entry.getKey();
      final Entry[] tab = HashtableOfTerm.this.table;
      final int hash = key.hashCode();
      final int index = (hash & 0x7FFFFFFF) % tab.length;

      for (Entry e = tab[index]; e != null; e = e.next)
        if ((e.hash == hash) && e.equals(entry)) return true;
      return false;
    }

    @Override
    public boolean remove(Object o) {
      if (!(o instanceof Map.Entry)) return false;
      final Map.Entry<Object, V> entry = (Map.Entry<Object, V>) o;
      final Object key = entry.getKey();
      final Entry[] tab = HashtableOfTerm.this.table;
      final int hash = key.hashCode();
      final int index = (hash & 0x7FFFFFFF) % tab.length;

      for (Entry<Object, V> e = tab[index], prev = null; e != null; prev = e, e = e.next) {
        if ((e.hash == hash) && e.equals(entry)) {
          HashtableOfTerm.this.modCount++;
          if (prev != null)
            prev.next = e.next;
          else
            tab[index] = e.next;

          HashtableOfTerm.this.count--;
          e.value = null;
          return true;
        }
      }
      return false;
    }

    @Override
    public int size() {
      return HashtableOfTerm.this.count;
    }

    @Override
    public void clear() {
      HashtableOfTerm.this.clear();
    }
  }

  /**
   * Returns a {@link Collection} view of the values contained in this map.
   * The collection is backed by the map, so changes to the map are
   * reflected in the collection, and vice-versa.  If the map is
   * modified while an iteration over the collection is in progress
   * (except through the iterator's own <tt>remove</tt> operation),
   * the results of the iteration are undefined.  The collection
   * supports element removal, which removes the corresponding
   * mapping from the map, via the <tt>Iterator.remove</tt>,
   * <tt>Collection.remove</tt>, <tt>removeAll</tt>,
   * <tt>retainAll</tt> and <tt>clear</tt> operations.  It does not
   * support the <tt>add</tt> or <tt>addAll</tt> operations.
   *
   * @since 1.2
   */
  @Override
  public Collection<V> values() {
    if (this.values == null) this.values = HashtableOfTerm.synchronizedCollection(new ValueCollection(), this);
    return this.values;
  }

  static <T> Collection<T> synchronizedCollection(Collection<T> c, Object mutex) {
    return new SynchronizedCollection<T>(c, mutex);
  }

  private class ValueCollection extends AbstractCollection<V> {
    @Override
    public Iterator<V> iterator() {
      return HashtableOfTerm.this.getIterator(HashtableOfTerm.VALUES);
    }

    @Override
    public int size() {
      return HashtableOfTerm.this.count;
    }

    @Override
    public boolean contains(Object o) {
      return HashtableOfTerm.this.containsValue(o);
    }

    @Override
    public void clear() {
      HashtableOfTerm.this.clear();
    }
  }

  // Comparison and hashing

  /**
   * Compares the specified Object with this Map for equality,
   * as per the definition in the Map interface.
   *
   * @param  o object to be compared for equality with this hashtable
   * @return true if the specified Object is equal to this Map
   * @see Map#equals(Object)
   * @since 1.2
   */
  @Override
  public synchronized boolean equals(Object o) {
    if (o == this) return true;

    if (!(o instanceof Map)) return false;
    final Map<Object, V> t = (Map<Object, V>) o;
    if (t.size() != this.size()) return false;

    try {
      final Iterator<Map.Entry<Object, V>> i = this.entrySet().iterator();
      while (i.hasNext()) {
        final Map.Entry<Object, V> e = i.next();
        final Object key = e.getKey();
        final V value = e.getValue();
        if (value == null) {
          if (!((t.get(key) == null) && t.containsKey(key))) return false;
        } else {
          if (!value.equals(t.get(key))) return false;
        }
      }
    } catch (final ClassCastException unused) {
      return false;
    } catch (final NullPointerException unused) {
      return false;
    }

    return true;
  }

  /**
   * Returns the hash code value for this Map as per the definition in the
   * Map interface.
   *
   * @see Map#hashCode()
   * @since 1.2
   */
  @Override
  public synchronized int hashCode() {
    /*
     * This code detects the recursion caused by computing the hash code
     * of a self-referential hash table and prevents the stack overflow
     * that would otherwise result.  This allows certain 1.1-era
     * applets with self-referential hash tables to work.  This code
     * abuses the loadFactor field to do double-duty as a hashCode
     * in progress flag, so as not to worsen the space performance.
     * A negative load factor indicates that hash code computation is
     * in progress.
     */
    int h = 0;
    if ((this.count == 0) || (this.loadFactor < 0)) return h; // Returns zero

    this.loadFactor = -this.loadFactor; // Mark hashCode computation in progress
    final Entry[] tab = this.table;
    for (int i = 0; i < tab.length; i++)
      for (Entry e = tab[i]; e != null; e = e.next)
        h += e.key.hashCode() ^ e.value.hashCode();
    this.loadFactor = -this.loadFactor; // Mark hashCode computation complete

    return h;
  }

  /**
   * Save the state of the Hashtable to a stream (i.e., serialize it).
   *
   * @serialData The <i>capacity</i> of the Hashtable (the length of the
   *             bucket array) is emitted (int), followed by the
   *             <i>size</i> of the Hashtable (the number of key-value
   *             mappings), followed by the key (Object) and value (Object)
   *             for each key-value mapping represented by the Hashtable
   *             The key-value mappings are emitted in no particular order.
   */
  private synchronized void writeObject(java.io.ObjectOutputStream s) throws IOException {
    // Write out the length, threshold, loadfactor
    s.defaultWriteObject();

    // Write out length, count of elements and then the key/value objects
    s.writeInt(this.table.length);
    s.writeInt(this.count);
    for (int index = this.table.length - 1; index >= 0; index--) {
      Entry entry = this.table[index];

      while (entry != null) {
        s.writeObject(entry.key);
        s.writeObject(entry.value);
        entry = entry.next;
      }
    }
  }

  /**
   * Reconstitute the Hashtable from a stream (i.e., deserialize it).
   */
  private void readObject(java.io.ObjectInputStream s) throws IOException, ClassNotFoundException {
    // Read in the length, threshold, and loadfactor
    s.defaultReadObject();

    // Read the original length of the array and number of elements
    final int origlength = s.readInt();
    int elements = s.readInt();

    // Compute new size with a bit of room 5% to grow but
    // no larger than the original size.  Make the length
    // odd if it's large enough, this helps distribute the entries.
    // Guard against the length ending up zero, that's not valid.
    int length = (int) (elements * this.loadFactor) + (elements / 20) + 3;
    if ((length > elements) && ((length & 1) == 0)) length--;
    if ((origlength > 0) && (length > origlength)) length = origlength;

    final Entry[] table = new Entry[length];
    this.count = 0;

    // Read the number of elements and then all the key/value objects
    for (; elements > 0; elements--) {
      final Object key = s.readObject();
      final V value = (V) s.readObject();
      // synch could be eliminated for performance
      this.reconstitutionPut(table, key, value);
    }
    this.table = table;
  }

  /**
   * The put method used by readObject. This is provided because put
   * is overridable and should not be called in readObject since the
   * subclass will not yet be initialized.
   *
   * <p>This differs from the regular put method in several ways. No
   * checking for rehashing is necessary since the number of elements
   * initially in the table is known. The modCount is not incremented
   * because we are creating a new instance. Also, no return value
   * is needed.
   */
  private void reconstitutionPut(Entry[] tab, Object key, V value) throws StreamCorruptedException {
    if (value == null) {
      throw new java.io.StreamCorruptedException();
    }
    // Makes sure the key is not already in the hashtable.
    // This should not happen in deserialized version.
    final int hash = key.hashCode();
    final int index = (hash & 0x7FFFFFFF) % tab.length;
    for (Entry<Object, V> e = tab[index]; e != null; e = e.next) {
      if ((e.hash == hash) && e.key.equals(key)) {
        throw new java.io.StreamCorruptedException();
      }
    }
    // Creates the new entry.
    final Entry<Object, V> e = tab[index];
    tab[index] = new Entry<Object, V>(hash, key, value, e);
    this.count++;
  }

  /**
   * Hashtable collision list.
   */
  private static class Entry<Object, V> implements Map.Entry<Object, V> {
    int hash;
    Object key;
    V value;
    Entry<Object, V> next;

    protected Entry(int hash, Object key, V value, Entry<Object, V> next) {
      this.hash = hash;
      this.key = key;
      this.value = value;
      this.next = next;
    }

    @Override
    protected Object clone() {
      return (Object) new Entry<Object, V>(this.hash, this.key, this.value, (this.next == null ? null : (Entry<Object, V>) this.next.clone()));
    }

    // Map.Entry Ops

    public Object getKey() {
      return this.key;
    }

    public V getValue() {
      return this.value;
    }

    public V setValue(V value) {
      if (value == null) throw new NullPointerException();

      final V oldValue = this.value;
      this.value = value;
      return oldValue;
    }

    //    @Override
    public boolean equals(Entry o) {
      if (!(o instanceof Map.Entry)) return false;
      final Map.Entry e = o;

      return (this.key == null ? e.getKey() == null : this.key.equals(e.getKey())) && (this.value == null ? e.getValue() == null : this.value.equals(e.getValue()));
    }

    @Override
    public int hashCode() {
      return this.hash ^ (this.value == null ? 0 : this.value.hashCode());
    }

    @Override
    public String toString() {
      return this.key.toString() + "=" + this.value.toString();
    }
  }

  // Types of Enumerations/Iterations
  private static final int KEYS = 0;
  private static final int VALUES = 1;
  private static final int ENTRIES = 2;

  /**
   * A hashtable enumerator class.  This class implements both the
   * Enumeration and Iterator interfaces, but individual instances
   * can be created with the Iterator methods disabled.  This is necessary
   * to avoid unintentionally increasing the capabilities granted a user
   * by passing an Enumeration.
   */
  private class Enumerator<T> implements Enumeration<T>, Iterator<T> {
    Entry[] table = HashtableOfTerm.this.table;
    int index = this.table.length;
    Entry<Object, V> entry = null;
    Entry<Object, V> lastReturned = null;
    int type;

    /**
     * Indicates whether this Enumerator is serving as an Iterator
     * or an Enumeration.  (true -> Iterator).
     */
    boolean iterator;

    /**
     * The modCount value that the iterator believes that the backing
     * Hashtable should have.  If this expectation is violated, the iterator
     * has detected concurrent modification.
     */
    protected int expectedModCount = HashtableOfTerm.this.modCount;

    Enumerator(int type, boolean iterator) {
      this.type = type;
      this.iterator = iterator;
    }

    public boolean hasMoreElements() {
      Entry<Object, V> e = this.entry;
      int i = this.index;
      final Entry[] t = this.table;
      /* Use locals for faster loop iteration */
      while ((e == null) && (i > 0)) {
        e = t[--i];
      }
      this.entry = e;
      this.index = i;
      return e != null;
    }

    public T nextElement() {
      Entry<Object, V> et = this.entry;
      int i = this.index;
      final Entry[] t = this.table;
      /* Use locals for faster loop iteration */
      while ((et == null) && (i > 0)) {
        et = t[--i];
      }
      this.entry = et;
      this.index = i;
      if (et != null) {
        final Entry<Object, V> e = this.lastReturned = this.entry;
        this.entry = e.next;
        return this.type == HashtableOfTerm.KEYS ? (T) e.key : (this.type == HashtableOfTerm.VALUES ? (T) e.value : (T) e);
      }
      throw new NoSuchElementException("Hashtable Enumerator");
    }

    // Iterator methods
    public boolean hasNext() {
      return this.hasMoreElements();
    }

    public T next() {
      if (HashtableOfTerm.this.modCount != this.expectedModCount) throw new ConcurrentModificationException();
      return this.nextElement();
    }

    public void remove() {
      if (!this.iterator) throw new UnsupportedOperationException();
      if (this.lastReturned == null) throw new IllegalStateException("Hashtable Enumerator");
      if (HashtableOfTerm.this.modCount != this.expectedModCount) throw new ConcurrentModificationException();

      synchronized (HashtableOfTerm.this) {
        final Entry[] tab = HashtableOfTerm.this.table;
        final int index = (this.lastReturned.hash & 0x7FFFFFFF) % tab.length;

        for (Entry<Object, V> e = tab[index], prev = null; e != null; prev = e, e = e.next) {
          if (e == this.lastReturned) {
            HashtableOfTerm.this.modCount++;
            this.expectedModCount++;
            if (prev == null)
              tab[index] = e.next;
            else
              prev.next = e.next;
            HashtableOfTerm.this.count--;
            this.lastReturned = null;
            return;
          }
        }
        throw new ConcurrentModificationException();
      }
    }
  }

}
