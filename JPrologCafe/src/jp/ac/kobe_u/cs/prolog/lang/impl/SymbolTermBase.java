package jp.ac.kobe_u.cs.prolog.lang.impl;

import static org.objectweb.asm.Opcodes.AASTORE;
import static org.objectweb.asm.Opcodes.ACC_FINAL;
import static org.objectweb.asm.Opcodes.ACC_PRIVATE;
import static org.objectweb.asm.Opcodes.ACC_PUBLIC;
import static org.objectweb.asm.Opcodes.ACC_STATIC;
import static org.objectweb.asm.Opcodes.ACC_SUPER;
import static org.objectweb.asm.Opcodes.ALOAD;
import static org.objectweb.asm.Opcodes.ANEWARRAY;
import static org.objectweb.asm.Opcodes.ARETURN;
import static org.objectweb.asm.Opcodes.ASTORE;
import static org.objectweb.asm.Opcodes.ATHROW;
import static org.objectweb.asm.Opcodes.BIPUSH;
import static org.objectweb.asm.Opcodes.CHECKCAST;
import static org.objectweb.asm.Opcodes.DUP;
import static org.objectweb.asm.Opcodes.GETFIELD;
import static org.objectweb.asm.Opcodes.GOTO;
import static org.objectweb.asm.Opcodes.IADD;
import static org.objectweb.asm.Opcodes.ICONST_0;
import static org.objectweb.asm.Opcodes.ICONST_1;
import static org.objectweb.asm.Opcodes.ICONST_2;
import static org.objectweb.asm.Opcodes.ICONST_3;
import static org.objectweb.asm.Opcodes.ICONST_4;
import static org.objectweb.asm.Opcodes.ICONST_5;
import static org.objectweb.asm.Opcodes.ICONST_M1;
import static org.objectweb.asm.Opcodes.IFEQ;
import static org.objectweb.asm.Opcodes.IFLE;
import static org.objectweb.asm.Opcodes.IFNE;
import static org.objectweb.asm.Opcodes.IFNULL;
import static org.objectweb.asm.Opcodes.IF_ICMPEQ;
import static org.objectweb.asm.Opcodes.ILOAD;
import static org.objectweb.asm.Opcodes.IMUL;
import static org.objectweb.asm.Opcodes.INSTANCEOF;
import static org.objectweb.asm.Opcodes.INVOKESPECIAL;
import static org.objectweb.asm.Opcodes.INVOKESTATIC;
import static org.objectweb.asm.Opcodes.INVOKEVIRTUAL;
import static org.objectweb.asm.Opcodes.IRETURN;
import static org.objectweb.asm.Opcodes.ISTORE;
import static org.objectweb.asm.Opcodes.ISUB;
import static org.objectweb.asm.Opcodes.LDC;
import static org.objectweb.asm.Opcodes.NEW;
import static org.objectweb.asm.Opcodes.PUTFIELD;
import static org.objectweb.asm.Opcodes.RETURN;
import static org.objectweb.asm.Opcodes.SIPUSH;
import static org.objectweb.asm.Opcodes.V1_6;
import static org.objectweb.asm.Opcodes.*;

import java.lang.reflect.Field;
import java.util.HashMap;

import jp.ac.kobe_u.cs.prolog.lang.InternalException;
import jp.ac.kobe_u.cs.prolog.lang.Prolog;
import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;
import jp.ac.kobe_u.cs.prolog.lang.Token;

import org.objectweb.asm.AnnotationVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;

/**
 * Atom.<br>
 * The <code>SymbolTerm</code> class represents a Prolog atom.<br>
 *
 * <pre>
 *   Object t = Prolog.makeSymbol("kobe");
 *   String name = ((SymbolTerm)t).name();
 * </pre>
 *
 * @author Mutsunori Banbara (banbara@kobe-u.ac.jp)
 * @author Naoyuki Tamura (tamura@kobe-u.ac.jp)
 * @version 1.0
 */
abstract class SymbolTermBase extends TermBase implements SymbolTerm {

  /**
   * 
   */
  private static final long serialVersionUID = -6890434653855891020L;

  /* (non-Javadoc)
  * @see jp.ac.kobe_u.cs.prolog.lang.CafeTerm#equalJProlog(java.lang.Object)
  */
  @Override
  public boolean equalJProlog(Object o) {
    // TODO Auto-generated method stub
    boolean t = o == this;
    if (t) return true;
    if (!StaticProlog.isAtomTerm(o)) return false;
    if (!this.nameUQ().equals(StaticProlog.nameUQ(o))) return false;
    t = StaticProlog.arity(o) == this.arity();
    if (!t) return false;
    return true;//throw new Error();
  }

  /** Symbol table. */
  //final private static Hashtable<String,SymbolTerm> SYMBOL_TABLE = new Hashtable<String,SymbolTerm>();
  /** Holds a string representation of this <code>SymbolTerm</code>. */
  public Object value;

  /** Holds the arity of this <code>SymbolTerm</code>. */
  final private int arity;

  /** Returns a Prolog atom for the given name. */
  public static Object makeSymbol(Object _name) {
    // if (true) return _name;
    return SymbolTermBase.makeSymbol(_name, 0);
  }

  /** Returns a Prolog functor for the given name and arity. */
  public static Object makeSymbol(Object _name, int _arity) {
    final String key = _name + "/" + _arity;
    Object sym;
    synchronized (SymbolTerm.SYMBOL_TABLE) {
      sym = SymbolTerm.SYMBOL_TABLE.get(key);
      if (sym == null) {
        if (_arity == 0)
          sym = _name;//new SymbolTermBase(_name, 0);
        else
          sym = newSymbolTermBase(_name, _arity);
        SymbolTerm.SYMBOL_TABLE.put(key, sym);
      }
    }
    return sym;
  }

  /**
   * @param _name
   * @param _arity
   * @return
   */
  private static Object newSymbolTermBase(Object _name, int _arity) {
    if (_arity == 4) {
      Class c = makeClass(_name.toString(), _arity);
      if (true) return c;
      //      Class[] paramTypes = new Class[_arity];
      //      for (int i=0;i<_arity;i++) {
      //        paramTypes[i]=Object.class;
      //      }
      //      c.getConstructor(paramTypes).newInstance((Object[]) );
    }
    // TODO Auto-generated method stub
    return new SymbolTermBase(_name, _arity) {
    };
  }

  /** Check whether this term is an empty list. */
  @Override
  public boolean isNil() {
    return StaticProlog.prologEquals(Prolog.Nil, this);
  }

  /** Constructs a new Prolog atom (or functor) with the given symbol name and arity. */
  protected SymbolTermBase(Object _name, int _arity) {
    this.value = _name;
    this.arity = _arity;
  }

  /** Returns the arity of this <code>SymbolTerm</code>.
   * @return the value of <code>arity</code>.
   * @see #arity
   */
  public int arity() {
    return this.arity;
  }

  /** Returns the string representation of this <code>SymbolTerm</code>.
   * @return the value of <code>name</code>.
   * @see #value
   */
  @Override
  public String nameUQ() {
    return this.value.toString();
  }

  /* Object */
  @Override
  public boolean unify(Object t) {
    if (StaticProlog.isVariable(t)) return StaticProlog.unify(t, this);
    //    if (!isAtomTerm(t)) // ???
    //      return false;
    return this.equalJProlog(t);
    //	return name.equals(((SymbolTerm)t).name());
  }

  /**
   * @return the <code>boolean</code> whose value is
   * <code>convertible(String.class, type)</code>.
   * @see Object#convertible(Class, Class)
   */
  @Override
  public boolean convertible(Class type) {
    return StaticProlog.convertible(String.class, type);
  }

  /**
   * Returns a <code>java.lang.String</code> corresponds to this <code>SymbolTerm</code>
   * according to <em>Prolog Cafe interoperability with Java</em>.
   * @return a <code>java.lang.String</code> object equivalent to
   * this <code>SymbolTerm</code>.
   */
  @Override
  public Object toJava() {
    return this.value;
  }

  @Override
  public String toQuotedString(int depth) {
    return Token.toQuotedString(this.value.toString());
  }

  /** Returns a string representation of this <code>SymbolTerm</code>. */
  @Override
  public String toStringImpl(int d) {
    if (this.arity == 0) return "" + this.value;
    return "" + this.value;//+"/"+arity;
  }

  @Override
  public String toString() {
    if (this.arity == 0) return "" + this.value;//+"/"+arity;
    return this.value + "/" + this.arity;
  }

  /* Comparable */
  /**
   * Compares two terms in <em>Prolog standard order of terms</em>.<br>
   * It is noted that <code>t1.compareTo(t2) == 0</code> has the same
   * <code>boolean</code> value as <code>t1.equals(t2)</code>.
   * @param anotherTerm the term to compared with. It must be dereferenced.
   * @return the value <code>0</code> if two terms are identical;
   * a value less than <code>0</code> if this term is <em>before</em> the <code>anotherTerm</code>;
   * and a value greater than <code>0</code> if this term is <em>after</em> the <code>anotherTerm</code>.
   */
  public int compareTo(Object anotherTerm) { // anotherTerm must be dereferenced.
    if (StaticProlog.isVariable(anotherTerm) || StaticProlog.isNumber(anotherTerm)) return Term.AFTER;
    if (!StaticProlog.isAtomTerm(anotherTerm)) return Term.BEFORE;
    if (this == anotherTerm) return Term.EQUAL;
    final int x = this.value.toString().compareTo(StaticProlog.nameUQ((anotherTerm)));
    if (x != 0) return x;
    final int y = this.arity - StaticProlog.arity(anotherTerm);
    if (y != 0) return y;
    throw new InternalException("SymbolTerm is not unique");
  }

  /* (non-Javadoc)
   * @see jp.ac.kobe_u.cs.prolog.lang.TermBase#hashCode(int)
   */
  @Override
  public int hashCode(int depth) {
    // TODO Auto-generated method stub
    return System.identityHashCode(this);
  }

  static Class makeClass(String name, int arity) {
    return smc.defClass(dump(name, arity));
  }

  /**
   * 0-5 have a shorten alias  
   * others bave to be pushed nomrally
   * 
   * @param mv Method Visitor for Const ints
   * @param i
   */
  private static void pushIntConst(MethodVisitor mv, int i) {
    switch (i) {
      case -1:
        mv.visitInsn(ICONST_M1);
        break;
      case 0:
        mv.visitInsn(ICONST_0);
        break;
      case 1:
        mv.visitInsn(ICONST_1);
        break;
      case 2:
        mv.visitInsn(ICONST_2);
        break;
      case 3:
        mv.visitInsn(ICONST_3);
        break;
      case 4:
        mv.visitInsn(ICONST_4);
        break;
      case 5:
        mv.visitInsn(ICONST_5);
        break;
      default:
        if (i < Byte.MAX_VALUE && i > Byte.MIN_VALUE) {
          mv.visitIntInsn(BIPUSH, i);
        } else if (i < Short.MAX_VALUE && i > Short.MIN_VALUE) {
          mv.visitIntInsn(SIPUSH, i);
        } else if (i < Integer.MAX_VALUE && i > Integer.MIN_VALUE) {
          mv.visitIntInsn(LDC, i);
        } else {
          throw new VerifyError("pushIntConst " + i);
        }
    }
  }

  public static byte[] dump(String name, int arity4) {

    if (arity4 != 4) throw new AbstractMethodError("TODO non arity 4 ;-P");

    ClassWriter cw = new ClassWriter(0);
    FieldVisitor fv;
    MethodVisitor mv;
    AnnotationVisitor av0;
    final String I_LANG = "jp/ac/kobe_u/cs/prolog/lang";
    final String PREDNAME_4 = I_LANG + "/impl/PRED_" + name + "_" + arity4;
    final String LNAME_4 = "L" + PREDNAME_4 + ";";
    final String LOBJ = "Ljava/lang/Object;";

    cw.visit(V1_6, ACC_PUBLIC + ACC_SUPER, PREDNAME_4, null, I_LANG + "/impl/TermBase", new String[] { I_LANG + "/impl/StructureTerm" });

    cw.visitSource("StructureTermObject_4.java", null);

    for (int i = 1; i <= arity4; i++) {
      {
        cw.visitInnerClass(PREDNAME_4 + "$" + i, null, null, 0);
      }
    }
    //    cw.visitInnerClass(INAME_4+"$1", null, null, 0);
    //
    //    cw.visitInnerClass(INAME_4+"$2", null, null, 0);
    //
    //    cw.visitInnerClass(INAME_4+"$3", null, null, 0);
    //
    //    cw.visitInnerClass(INAME_4+"$4", null, null, 0);

    {
      fv = cw.visitField(ACC_PRIVATE + ACC_FINAL + ACC_STATIC, "serialVersionUID", "J", null, new Long(6905572459626027669L));
      fv.visitEnd();
    }
    for (int i = 1; i <= arity4; i++) {
      {
        fv = cw.visitField(ACC_PUBLIC, "arg" + i, LOBJ, null, null);
        fv.visitEnd();
      }
    }
    //    {
    //      fv = cw.visitField(ACC_PUBLIC, "arg1", LOBJ, null, null);
    //      fv.visitEnd();
    //    }
    //    {
    //      fv = cw.visitField(ACC_PUBLIC, "arg2", LOBJ, null, null);
    //      fv.visitEnd();
    //    }
    //    {
    //      fv = cw.visitField(ACC_PUBLIC, "arg3", LOBJ, null, null);
    //      fv.visitEnd();
    //    }
    //    {
    //      fv = cw.visitField(ACC_PUBLIC, "arg4", LOBJ, null, null);
    //      fv.visitEnd();
    //    }

    String constructorArgs = "";
    for (int i = 1; i <= arity4; i++) {
      {
        constructorArgs += LOBJ;
      }
    }

    {
      mv = cw.visitMethod(ACC_PUBLIC, "<init>", "(" + constructorArgs + ")V", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(49, l0);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKESPECIAL, PREDNAME_4, "<init>", "()V");
      //      Label l1 = new Label();
      //      mv.visitLabel(l1);
      //      mv.visitLineNumber(50, l1);
      //      mv.visitVarInsn(ALOAD, 0);
      //      mv.visitVarInsn(ALOAD, 1);
      //      mv.visitFieldInsn(PUTFIELD, INAME_4, "arg1", LOBJ);
      //      Label l2 = new Label();
      //      mv.visitLabel(l2);
      //      mv.visitLineNumber(51, l2);
      //      mv.visitVarInsn(ALOAD, 0);
      //      mv.visitVarInsn(ALOAD, 2);
      //      mv.visitFieldInsn(PUTFIELD, INAME_4, "arg2", LOBJ);
      //      Label l3 = new Label();
      //      mv.visitLabel(l3);
      //      mv.visitLineNumber(52, l3);
      //      mv.visitVarInsn(ALOAD, 0);
      //      mv.visitVarInsn(ALOAD, 3);
      //      mv.visitFieldInsn(PUTFIELD, INAME_4, "arg3", LOBJ);
      //      
      //      Label l4 = new Label();
      //      mv.visitLabel(l4);
      //      mv.visitLineNumber(53, l4);
      //      mv.visitVarInsn(ALOAD, 0);
      //      mv.visitVarInsn(ALOAD, 4);
      //      mv.visitFieldInsn(PUTFIELD, INAME_4, "arg4", LOBJ);
      for (int i = 1; i <= arity4; i++) {
        {
          Label l4 = new Label();
          mv.visitLabel(l4);
          mv.visitLineNumber(53, l4);
          mv.visitVarInsn(ALOAD, 0);
          mv.visitVarInsn(ALOAD, i);
          mv.visitFieldInsn(PUTFIELD, PREDNAME_4, "arg" + i, LOBJ);
        }
      }

      Label l5 = new Label();
      mv.visitLabel(l5);
      mv.visitLineNumber(54, l5);
      mv.visitInsn(RETURN);
      //      Label l6 = new Label();
      //      mv.visitLabel(l6);
      //      mv.visitLocalVariable("this", "L"+INAME_4+";", null, l0, l6, 0);
      //      mv.visitLocalVariable("arg1", LOBJ, null, l0, l6, 1);
      //      mv.visitLocalVariable("arg2", LOBJ, null, l0, l6, 2);
      //      mv.visitLocalVariable("arg3", LOBJ, null, l0, l6, 3);
      //      mv.visitLocalVariable("arg4", LOBJ, null, l0, l6, 4);
      mv.visitMaxs(2, 5);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "<init>", "()V", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(69, l0);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKESPECIAL, I_LANG + "/impl/TermBase", "<init>", "()V");
      Label l1 = new Label();
      mv.visitLabel(l1);
      mv.visitLineNumber(75, l1);
      mv.visitInsn(RETURN);
      Label l2 = new Label();
      mv.visitLabel(l2);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l2, 0);
      mv.visitMaxs(1, 1);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "functor", "()" + LOBJ, null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(84, l0);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
      mv.visitInsn(ARETURN);
      Label l1 = new Label();
      mv.visitLabel(l1);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l1, 0);
      mv.visitMaxs(1, 1);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "arity", "()I", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(94, l0);
      pushIntConst(mv, arity4);
      mv.visitInsn(IRETURN);
      Label l1 = new Label();
      mv.visitLabel(l1);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l1, 0);
      mv.visitMaxs(1, 1);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "args", "()[" + LOBJ, null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(104, l0);
      pushIntConst(mv, arity4);
      mv.visitTypeInsn(ANEWARRAY, "java/lang/Object");

      //      mv.visitInsn(DUP);
      //      mv.visitInsn(ICONST_0);
      //      mv.visitVarInsn(ALOAD, 0);
      //      mv.visitFieldInsn(GETFIELD, INAME_4, "arg1", LOBJ);     
      //      mv.visitInsn(AASTORE);
      //
      //      mv.visitInsn(DUP);
      //      mv.visitInsn(ICONST_1);
      //      mv.visitVarInsn(ALOAD, 0);
      //      mv.visitFieldInsn(GETFIELD, INAME_4, "arg2", LOBJ);      
      //      mv.visitInsn(AASTORE);
      //      
      //      mv.visitInsn(DUP);
      //      mv.visitInsn(ICONST_2);
      //      mv.visitVarInsn(ALOAD, 0);
      //      mv.visitFieldInsn(GETFIELD, INAME_4, "arg3", LOBJ);     
      //      mv.visitInsn(AASTORE);
      //      
      //    mv.visitInsn(DUP);
      //    mv.visitInsn(ICONST_3);
      //    mv.visitVarInsn(ALOAD, 0);
      //    mv.visitFieldInsn(GETFIELD, INAME_4, "arg4", LOBJ);
      //    mv.visitInsn(AASTORE);
      for (int i = 0; i < arity4; i++) {
        {
          mv.visitInsn(DUP);
          pushIntConst(mv, i);//0-3
          mv.visitVarInsn(ALOAD, 0);
          mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg" + (i + 1), LOBJ);
          mv.visitInsn(AASTORE);
        }
      }

      mv.visitInsn(ARETURN);
      Label l1 = new Label();
      mv.visitLabel(l1);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l1, 0);
      mv.visitMaxs(4, 1);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "nameUQ", "()Ljava/lang/String;", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(118, l0);
      mv.visitLdcInsn(name);
      mv.visitInsn(ARETURN);
      Label l1 = new Label();
      mv.visitLabel(l1);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l1, 0);
      mv.visitMaxs(1, 1);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "unify", "(" + LOBJ + ")Z", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(124, l0);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitTypeInsn(INSTANCEOF, PREDNAME_4);
      Label l1 = new Label();
      mv.visitJumpInsn(IFEQ, l1);
      Label l2 = new Label();
      mv.visitLabel(l2);
      mv.visitLineNumber(125, l2);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitTypeInsn(CHECKCAST, PREDNAME_4);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "unify", "(" + LNAME_4 + ")Z");
      mv.visitInsn(IRETURN);
      mv.visitLabel(l1);
      mv.visitLineNumber(127, l1);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isVariable", "(" + LOBJ + ")Z");
      Label l3 = new Label();
      mv.visitJumpInsn(IFEQ, l3);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "unify", "(" + LOBJ + LOBJ + ")Z");
      mv.visitInsn(IRETURN);
      mv.visitLabel(l3);
      mv.visitLineNumber(128, l3);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isCompound", "(" + LOBJ + ")Z");

      Label l4 = new Label();
      mv.visitJumpInsn(IFNE, l4);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l4);
      mv.visitLineNumber(129, l4);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "functor", "(" + LOBJ + ")" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "unify", "(" + LOBJ + LOBJ + ")Z");

      Label l5 = new Label();
      mv.visitJumpInsn(IFNE, l5);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l5);
      mv.visitLineNumber(130, l5);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_0);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "unify", "(" + LOBJ + LOBJ + ")Z");

      Label l6 = new Label();
      mv.visitJumpInsn(IFNE, l6);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l6);
      mv.visitLineNumber(131, l6);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "unify", "(" + LOBJ + LOBJ + ")Z");

      Label l7 = new Label();
      mv.visitJumpInsn(IFNE, l7);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l7);
      mv.visitLineNumber(132, l7);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "unify", "(" + LOBJ + LOBJ + ")Z");

      Label l8 = new Label();
      mv.visitJumpInsn(IFNE, l8);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l8);
      mv.visitLineNumber(133, l8);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg4", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_3);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "unify", "(" + LOBJ + LOBJ + ")Z");

      Label l9 = new Label();
      mv.visitJumpInsn(IFNE, l9);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l9);
      mv.visitLineNumber(134, l9);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitInsn(ICONST_1);
      mv.visitInsn(IRETURN);

      Label l10 = new Label();
      mv.visitLabel(l10);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l10, 0);
      mv.visitLocalVariable("t", LOBJ, null, l0, l10, 1);
      mv.visitMaxs(3, 2);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "unify", "(" + LNAME_4 + ")Z", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(139, l0);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_0);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "arg0", "(I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "unify", "(" + LOBJ + LOBJ + ")Z");
      Label l1 = new Label();
      mv.visitJumpInsn(IFNE, l1);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l1);
      mv.visitLineNumber(140, l1);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_1);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "arg0", "(I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "unify", "(" + LOBJ + LOBJ + ")Z");
      Label l2 = new Label();
      mv.visitJumpInsn(IFNE, l2);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l2);
      mv.visitLineNumber(141, l2);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_2);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "arg0", "(I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "unify", "(" + LOBJ + LOBJ + ")Z");
      Label l3 = new Label();
      mv.visitJumpInsn(IFNE, l3);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l3);
      mv.visitLineNumber(142, l3);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg4", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_3);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "arg0", "(I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "unify", "(" + LOBJ + LOBJ + ")Z");
      Label l4 = new Label();
      mv.visitJumpInsn(IFNE, l4);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l4);
      mv.visitLineNumber(143, l4);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitInsn(ICONST_1);
      mv.visitInsn(IRETURN);
      Label l5 = new Label();
      mv.visitLabel(l5);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l5, 0);
      mv.visitLocalVariable("t", LNAME_4, null, l0, l5, 1);
      mv.visitMaxs(3, 2);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "copy", "()" + LOBJ, null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(148, l0);
      mv.visitTypeInsn(NEW, PREDNAME_4);
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "copy", "(" + LOBJ + ")" + LOBJ);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "copy", "(" + LOBJ + ")" + LOBJ);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "copy", "(" + LOBJ + ")" + LOBJ);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg4", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "copy", "(" + LOBJ + ")" + LOBJ);
      mv.visitMethodInsn(INVOKESPECIAL, PREDNAME_4, "<init>", "(" + LOBJ + LOBJ + LOBJ + LOBJ + ")V");
      mv.visitInsn(ARETURN);
      Label l1 = new Label();
      mv.visitLabel(l1);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l1, 0);
      mv.visitMaxs(6, 1);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "isGround", "()Z", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(153, l0);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isGround", "(" + LOBJ + ")Z");
      Label l1 = new Label();
      mv.visitJumpInsn(IFNE, l1);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l1);
      mv.visitLineNumber(154, l1);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isGround", "(" + LOBJ + ")Z");
      Label l2 = new Label();
      mv.visitJumpInsn(IFNE, l2);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l2);
      mv.visitLineNumber(155, l2);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isGround", "(" + LOBJ + ")Z");
      Label l3 = new Label();
      mv.visitJumpInsn(IFNE, l3);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l3);
      mv.visitLineNumber(156, l3);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg4", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isGround", "(" + LOBJ + ")Z");
      Label l4 = new Label();
      mv.visitJumpInsn(IFNE, l4);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l4);
      mv.visitLineNumber(157, l4);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitInsn(ICONST_1);
      mv.visitInsn(IRETURN);
      Label l5 = new Label();
      mv.visitLabel(l5);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l5, 0);
      mv.visitMaxs(1, 1);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "toQuotedString", "(I)Ljava/lang/String;", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(162, l0);
      mv.visitVarInsn(ILOAD, 1);
      mv.visitInsn(ICONST_1);
      mv.visitInsn(ISUB);
      mv.visitVarInsn(ISTORE, 2);
      Label l1 = new Label();
      mv.visitLabel(l1);
      mv.visitLineNumber(163, l1);
      mv.visitLdcInsn("");
      mv.visitVarInsn(ASTORE, 3);
      Label l2 = new Label();
      mv.visitLabel(l2);
      mv.visitLineNumber(164, l2);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "toQuotedString", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitLdcInsn("(");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l3 = new Label();
      mv.visitLabel(l3);
      mv.visitLineNumber(165, l3);
      mv.visitVarInsn(ILOAD, 2);
      Label l4 = new Label();
      mv.visitJumpInsn(IFLE, l4);
      Label l5 = new Label();
      mv.visitLabel(l5);
      mv.visitLineNumber(166, l5);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitVarInsn(ALOAD, 3);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      mv.visitVarInsn(ILOAD, 2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "toQuotedString", "(" + LOBJ + "I)Ljava/lang/String;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l6 = new Label();
      mv.visitLabel(l6);
      mv.visitLineNumber(167, l6);
      mv.visitLdcInsn(",");
      mv.visitVarInsn(ASTORE, 3);
      Label l7 = new Label();
      mv.visitLabel(l7);
      mv.visitLineNumber(168, l7);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitVarInsn(ALOAD, 3);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      mv.visitVarInsn(ILOAD, 2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "toQuotedString", "(" + LOBJ + "I)Ljava/lang/String;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l8 = new Label();
      mv.visitLabel(l8);
      mv.visitLineNumber(169, l8);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitVarInsn(ALOAD, 3);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitVarInsn(ILOAD, 2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "toQuotedString", "(" + LOBJ + "I)Ljava/lang/String;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l9 = new Label();
      mv.visitLabel(l9);
      mv.visitLineNumber(170, l9);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitVarInsn(ALOAD, 3);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitVarInsn(ILOAD, 2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "toQuotedString", "(" + LOBJ + "I)Ljava/lang/String;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l10 = new Label();
      mv.visitJumpInsn(GOTO, l10);
      mv.visitLabel(l4);
      mv.visitLineNumber(172, l4);
      mv.visitFrame(Opcodes.F_APPEND, 3, new Object[] { Opcodes.INTEGER, "java/lang/String", "java/lang/String" }, 0, null);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitLdcInsn("......");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      mv.visitLabel(l10);
      mv.visitLineNumber(174, l10);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitLdcInsn(")");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l11 = new Label();
      mv.visitLabel(l11);
      mv.visitLineNumber(175, l11);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitInsn(ARETURN);
      Label l12 = new Label();
      mv.visitLabel(l12);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l12, 0);
      mv.visitLocalVariable("depth", "I", null, l0, l12, 1);
      mv.visitLocalVariable("d0", "I", null, l1, l12, 2);
      mv.visitLocalVariable("delim", "Ljava/lang/String;", null, l2, l12, 3);
      mv.visitLocalVariable("s", "Ljava/lang/String;", null, l3, l12, 4);
      mv.visitMaxs(3, 5);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "arg0", "(I)" + LOBJ, null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(183, l0);
      mv.visitVarInsn(ILOAD, 1);
      Label l1 = new Label();
      Label l2 = new Label();
      Label l3 = new Label();
      Label l4 = new Label();
      Label l5 = new Label();
      mv.visitTableSwitchInsn(0, 3, l5, new Label[] { l1, l2, l3, l4 });
      mv.visitLabel(l1);
      mv.visitLineNumber(185, l1);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      Label l6 = new Label();
      mv.visitJumpInsn(IFNULL, l6);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      Label l7 = new Label();
      mv.visitJumpInsn(GOTO, l7);
      mv.visitLabel(l6);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "varg0", "(I)L" + I_LANG + "/VariableTermLocation;");
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "makeVariableLoc", "(L" + I_LANG + "/VariableTermLocation;)" + LOBJ);
      mv.visitLabel(l7);
      mv.visitFrame(Opcodes.F_SAME1, 0, null, 1, new Object[] { "java/lang/Object" });
      mv.visitInsn(ARETURN);
      mv.visitLabel(l2);
      mv.visitLineNumber(188, l2);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      Label l8 = new Label();
      mv.visitJumpInsn(IFNULL, l8);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      Label l9 = new Label();
      mv.visitJumpInsn(GOTO, l9);
      mv.visitLabel(l8);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "varg0", "(I)L" + I_LANG + "/VariableTermLocation;");
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "makeVariableLoc", "(L" + I_LANG + "/VariableTermLocation;)" + LOBJ);
      mv.visitLabel(l9);
      mv.visitFrame(Opcodes.F_SAME1, 0, null, 1, new Object[] { "java/lang/Object" });
      mv.visitInsn(ARETURN);
      mv.visitLabel(l3);
      mv.visitLineNumber(191, l3);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      Label l10 = new Label();
      mv.visitJumpInsn(IFNULL, l10);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      Label l11 = new Label();
      mv.visitJumpInsn(GOTO, l11);
      mv.visitLabel(l10);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "varg0", "(I)L" + I_LANG + "/VariableTermLocation;");
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "makeVariableLoc", "(L" + I_LANG + "/VariableTermLocation;)" + LOBJ);
      mv.visitLabel(l11);
      mv.visitFrame(Opcodes.F_SAME1, 0, null, 1, new Object[] { "java/lang/Object" });
      mv.visitInsn(ARETURN);
      mv.visitLabel(l4);
      mv.visitLineNumber(194, l4);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg4", LOBJ);
      Label l12 = new Label();
      mv.visitJumpInsn(IFNULL, l12);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg4", LOBJ);
      Label l13 = new Label();
      mv.visitJumpInsn(GOTO, l13);
      mv.visitLabel(l12);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "varg0", "(I)L" + I_LANG + "/VariableTermLocation;");
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "makeVariableLoc", "(L" + I_LANG + "/VariableTermLocation;)" + LOBJ);
      mv.visitLabel(l13);
      mv.visitFrame(Opcodes.F_SAME1, 0, null, 1, new Object[] { "java/lang/Object" });
      mv.visitInsn(ARETURN);
      mv.visitLabel(l5);
      mv.visitLineNumber(197, l5);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitTypeInsn(NEW, "java/lang/NoSuchFieldError");
      mv.visitInsn(DUP);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "toString", "()Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitLdcInsn(" doesnt have a arg");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(I)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/NoSuchFieldError", "<init>", "(Ljava/lang/String;)V");
      mv.visitInsn(ATHROW);
      Label l14 = new Label();
      mv.visitLabel(l14);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l14, 0);
      mv.visitLocalVariable("i", "I", null, l0, l14, 1);
      mv.visitMaxs(5, 2);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "varg0", "(I)L" + I_LANG + "/VariableTermLocation;", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(201, l0);
      mv.visitVarInsn(ILOAD, 1);
      Label l1 = new Label();
      Label l2 = new Label();
      Label l3 = new Label();
      Label l4 = new Label();
      Label l5 = new Label();
      mv.visitTableSwitchInsn(0, 3, l5, new Label[] { l1, l2, l3, l4 });
      mv.visitLabel(l1);
      mv.visitLineNumber(203, l1);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitTypeInsn(NEW, PREDNAME_4 + "$1");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKESPECIAL, PREDNAME_4 + "$1", "<init>", "(" + LNAME_4 + ")V");
      mv.visitInsn(ARETURN);
      mv.visitLabel(l2);
      mv.visitLineNumber(223, l2);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitTypeInsn(NEW, PREDNAME_4 + "$2");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKESPECIAL, PREDNAME_4 + "$2", "<init>", "(" + LNAME_4 + ")V");
      mv.visitInsn(ARETURN);
      mv.visitLabel(l3);
      mv.visitLineNumber(245, l3);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitTypeInsn(NEW, PREDNAME_4 + "$3");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKESPECIAL, PREDNAME_4 + "$3", "<init>", "(" + LNAME_4 + ")V");
      mv.visitInsn(ARETURN);
      mv.visitLabel(l4);
      mv.visitLineNumber(267, l4);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitTypeInsn(NEW, PREDNAME_4 + "$4");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKESPECIAL, PREDNAME_4 + "$4", "<init>", "(" + LNAME_4 + ")V");
      mv.visitInsn(ARETURN);
      mv.visitLabel(l5);
      mv.visitLineNumber(289, l5);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitTypeInsn(NEW, "java/lang/NoSuchFieldError");
      mv.visitInsn(DUP);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "toString", "()Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitLdcInsn(" doesnt have a arg");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(I)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/NoSuchFieldError", "<init>", "(Ljava/lang/String;)V");
      mv.visitInsn(ATHROW);
      Label l6 = new Label();
      mv.visitLabel(l6);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l6, 0);
      mv.visitLocalVariable("i", "I", null, l0, l6, 1);
      mv.visitMaxs(5, 2);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "equalJProlog", "(" + LOBJ + ")Z", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(309, l0);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitTypeInsn(INSTANCEOF, I_LANG + "/impl/StructureTerm");
      Label l1 = new Label();
      mv.visitJumpInsn(IFNE, l1);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l1);
      mv.visitLineNumber(310, l1);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "functor", "(" + LOBJ + ")" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "prologEquals", "(" + LOBJ + LOBJ + ")Z");
      Label l2 = new Label();
      mv.visitJumpInsn(IFNE, l2);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l2);
      mv.visitLineNumber(311, l2);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_0);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "prologEquals", "(" + LOBJ + LOBJ + ")Z");
      Label l3 = new Label();
      mv.visitJumpInsn(IFNE, l3);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l3);
      mv.visitLineNumber(312, l3);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "prologEquals", "(" + LOBJ + LOBJ + ")Z");
      Label l4 = new Label();
      mv.visitJumpInsn(IFNE, l4);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l4);
      mv.visitLineNumber(313, l4);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "prologEquals", "(" + LOBJ + LOBJ + ")Z");
      Label l5 = new Label();
      mv.visitJumpInsn(IFNE, l5);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l5);
      mv.visitLineNumber(314, l5);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg4", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_3);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "prologEquals", "(" + LOBJ + LOBJ + ")Z");
      Label l6 = new Label();
      mv.visitJumpInsn(IFNE, l6);
      mv.visitInsn(ICONST_0);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l6);
      mv.visitLineNumber(315, l6);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitInsn(ICONST_1);
      mv.visitInsn(IRETURN);
      Label l7 = new Label();
      mv.visitLabel(l7);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l7, 0);
      mv.visitLocalVariable("obj", LOBJ, null, l0, l7, 1);
      mv.visitMaxs(3, 2);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "hashCode", "(I)I", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(320, l0);
      mv.visitInsn(ICONST_1);
      mv.visitVarInsn(ISTORE, 2);
      Label l1 = new Label();
      mv.visitLabel(l1);
      mv.visitLineNumber(321, l1);
      mv.visitIntInsn(BIPUSH, 31);
      mv.visitVarInsn(ILOAD, 2);

      mv.visitInsn(IMUL);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "hashCode", "(" + LOBJ + "I)I");
      mv.visitInsn(IADD);
      mv.visitVarInsn(ISTORE, 2);
      Label l2 = new Label();
      mv.visitLabel(l2);
      mv.visitLineNumber(322, l2);
      mv.visitIntInsn(BIPUSH, 31);
      mv.visitVarInsn(ILOAD, 2);

      mv.visitInsn(IMUL);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "deref", "(" + LOBJ + ")" + LOBJ);
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "hashCode", "(" + LOBJ + "I)I");
      mv.visitInsn(IADD);
      mv.visitVarInsn(ISTORE, 2);
      Label l3 = new Label();
      mv.visitLabel(l3);
      mv.visitLineNumber(323, l3);
      mv.visitIntInsn(BIPUSH, 31);
      mv.visitVarInsn(ILOAD, 2);

      mv.visitInsn(IMUL);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "deref", "(" + LOBJ + ")" + LOBJ);
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "hashCode", "(" + LOBJ + "I)I");
      mv.visitInsn(IADD);
      mv.visitVarInsn(ISTORE, 2);
      Label l4 = new Label();
      mv.visitLabel(l4);
      mv.visitLineNumber(324, l4);
      mv.visitIntInsn(BIPUSH, 31);
      mv.visitVarInsn(ILOAD, 2);

      mv.visitInsn(IMUL);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "deref", "(" + LOBJ + ")" + LOBJ);
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "hashCode", "(" + LOBJ + "I)I");
      mv.visitInsn(IADD);
      mv.visitVarInsn(ISTORE, 2);
      Label l5 = new Label();
      mv.visitLabel(l5);
      mv.visitLineNumber(325, l5);
      mv.visitIntInsn(BIPUSH, 31);
      mv.visitVarInsn(ILOAD, 2);

      mv.visitInsn(IMUL);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg4", LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "deref", "(" + LOBJ + ")" + LOBJ);
      mv.visitVarInsn(ILOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "hashCode", "(" + LOBJ + "I)I");
      mv.visitInsn(IADD);
      mv.visitVarInsn(ISTORE, 2);
      Label l6 = new Label();
      mv.visitLabel(l6);
      mv.visitLineNumber(326, l6);
      mv.visitVarInsn(ILOAD, 2);

      mv.visitInsn(IRETURN);
      Label l7 = new Label();
      mv.visitLabel(l7);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l7, 0);
      mv.visitLocalVariable("d", "I", null, l0, l7, 1);
      mv.visitLocalVariable("h", "I", null, l1, l7, 2);
      mv.visitMaxs(3, 3);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "toStringImpl", "(I)Ljava/lang/String;", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(332, l0);
      mv.visitVarInsn(ILOAD, 1);
      mv.visitInsn(ICONST_1);
      mv.visitInsn(ISUB);
      mv.visitVarInsn(ISTORE, 2);
      Label l1 = new Label();
      mv.visitLabel(l1);
      mv.visitLineNumber(333, l1);
      mv.visitLdcInsn("");
      mv.visitVarInsn(ASTORE, 3);
      Label l2 = new Label();
      mv.visitLabel(l2);
      mv.visitLineNumber(334, l2);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKEVIRTUAL, PREDNAME_4, "nameUQ", "()Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "toString", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitLdcInsn("(");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l3 = new Label();
      mv.visitLabel(l3);
      mv.visitLineNumber(335, l3);
      mv.visitVarInsn(ILOAD, 2);
      Label l4 = new Label();
      mv.visitJumpInsn(IFLE, l4);
      Label l5 = new Label();
      mv.visitLabel(l5);
      mv.visitLineNumber(336, l5);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitVarInsn(ALOAD, 3);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      mv.visitVarInsn(ILOAD, 2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "toString", "(" + LOBJ + "I)Ljava/lang/String;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l6 = new Label();
      mv.visitLabel(l6);
      mv.visitLineNumber(337, l6);
      mv.visitLdcInsn(",");
      mv.visitVarInsn(ASTORE, 3);
      Label l7 = new Label();
      mv.visitLabel(l7);
      mv.visitLineNumber(338, l7);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitVarInsn(ALOAD, 3);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      mv.visitVarInsn(ILOAD, 2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "toString", "(" + LOBJ + "I)Ljava/lang/String;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l8 = new Label();
      mv.visitLabel(l8);
      mv.visitLineNumber(339, l8);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitVarInsn(ALOAD, 3);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitVarInsn(ILOAD, 2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "toString", "(" + LOBJ + "I)Ljava/lang/String;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l9 = new Label();
      mv.visitLabel(l9);
      mv.visitLineNumber(340, l9);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitVarInsn(ALOAD, 3);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitVarInsn(ILOAD, 2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "toString", "(" + LOBJ + "I)Ljava/lang/String;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l10 = new Label();
      mv.visitJumpInsn(GOTO, l10);
      mv.visitLabel(l4);
      mv.visitLineNumber(342, l4);
      mv.visitFrame(Opcodes.F_APPEND, 3, new Object[] { Opcodes.INTEGER, "java/lang/String", "java/lang/String" }, 0, null);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitLdcInsn("......");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      mv.visitLabel(l10);
      mv.visitLineNumber(344, l10);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitTypeInsn(NEW, "java/lang/StringBuilder");
      mv.visitInsn(DUP);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitMethodInsn(INVOKESTATIC, "java/lang/String", "valueOf", "(" + LOBJ + ")Ljava/lang/String;");
      mv.visitMethodInsn(INVOKESPECIAL, "java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V");
      mv.visitLdcInsn(")");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;");
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/StringBuilder", "toString", "()Ljava/lang/String;");
      mv.visitVarInsn(ASTORE, 4);
      Label l11 = new Label();
      mv.visitLabel(l11);
      mv.visitLineNumber(345, l11);
      mv.visitVarInsn(ALOAD, 4);
      mv.visitInsn(ARETURN);
      Label l12 = new Label();
      mv.visitLabel(l12);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l12, 0);
      mv.visitLocalVariable("d", "I", null, l0, l12, 1);
      mv.visitLocalVariable("d0", "I", null, l1, l12, 2);
      mv.visitLocalVariable("delim", "Ljava/lang/String;", null, l2, l12, 3);
      mv.visitLocalVariable("s", "Ljava/lang/String;", null, l3, l12, 4);
      mv.visitMaxs(3, 5);
      mv.visitEnd();
    }
    {
      mv = cw.visitMethod(ACC_PUBLIC, "compareTo", "(" + LOBJ + ")I", null, null);
      mv.visitCode();
      Label l0 = new Label();
      mv.visitLabel(l0);
      mv.visitLineNumber(367, l0);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isVariable", "(" + LOBJ + ")Z");
      Label l1 = new Label();
      mv.visitJumpInsn(IFNE, l1);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isNumber", "(" + LOBJ + ")Z");
      mv.visitJumpInsn(IFNE, l1);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isAtomTerm", "(" + LOBJ + ")Z");
      Label l2 = new Label();
      mv.visitJumpInsn(IFEQ, l2);
      mv.visitLabel(l1);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitInsn(ICONST_1);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l2);
      mv.visitLineNumber(368, l2);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isListTerm", "(" + LOBJ + ")Z");
      Label l3 = new Label();
      mv.visitJumpInsn(IFEQ, l3);
      Label l4 = new Label();
      mv.visitLabel(l4);
      mv.visitLineNumber(369, l4);
      mv.visitIntInsn(SIPUSH, 264);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l3);
      mv.visitLineNumber(376, l3);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "isCompound", "(" + LOBJ + ")Z");
      Label l5 = new Label();
      mv.visitJumpInsn(IFEQ, l5);
      Label l6 = new Label();
      mv.visitLabel(l6);
      mv.visitLineNumber(377, l6);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "arity", "(" + LOBJ + ")I");
      mv.visitVarInsn(ISTORE, 3);
      Label l7 = new Label();
      mv.visitLabel(l7);
      mv.visitLineNumber(378, l7);
      pushIntConst(mv, arity4);
      mv.visitVarInsn(ILOAD, 3);
      Label l8 = new Label();
      mv.visitJumpInsn(IF_ICMPEQ, l8);
      pushIntConst(mv, arity4);
      mv.visitVarInsn(ILOAD, 3);
      mv.visitInsn(ISUB);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l8);
      mv.visitLineNumber(379, l8);
      mv.visitFrame(Opcodes.F_FULL, 4, new Object[] { PREDNAME_4, "java/lang/Object", Opcodes.TOP, Opcodes.INTEGER }, 0, new Object[] {});
      mv.visitVarInsn(ALOAD, 1);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "functor", "(" + LOBJ + ")" + LOBJ);
      mv.visitVarInsn(ASTORE, 2);
      Label l9 = new Label();
      mv.visitLabel(l9);
      mv.visitLineNumber(380, l9);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitMethodInsn(INVOKEVIRTUAL, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
      mv.visitVarInsn(ALOAD, 2);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "compareTerm", "(" + LOBJ + LOBJ + ")I");
      mv.visitVarInsn(ISTORE, 4);
      Label l10 = new Label();
      mv.visitLabel(l10);
      mv.visitLineNumber(381, l10);
      mv.visitVarInsn(ILOAD, 4);
      Label l11 = new Label();
      mv.visitJumpInsn(IFEQ, l11);
      mv.visitVarInsn(ILOAD, 4);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l11);
      mv.visitLineNumber(382, l11);
      mv.visitFrame(Opcodes.F_FULL, 5, new Object[] { PREDNAME_4, "java/lang/Object", "java/lang/Object", Opcodes.INTEGER, Opcodes.INTEGER }, 0, new Object[] {});
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg1", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_0);
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "deref", "(" + LOBJ + ")" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "compareTerm", "(" + LOBJ + LOBJ + ")I");
      mv.visitVarInsn(ISTORE, 4);
      Label l12 = new Label();
      mv.visitLabel(l12);
      mv.visitLineNumber(383, l12);
      mv.visitVarInsn(ILOAD, 4);
      Label l13 = new Label();
      mv.visitJumpInsn(IFEQ, l13);
      mv.visitVarInsn(ILOAD, 4);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l13);
      mv.visitLineNumber(384, l13);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg2", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_1);
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "deref", "(" + LOBJ + ")" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "compareTerm", "(" + LOBJ + LOBJ + ")I");
      mv.visitVarInsn(ISTORE, 4);
      Label l14 = new Label();
      mv.visitLabel(l14);
      mv.visitLineNumber(385, l14);
      mv.visitVarInsn(ILOAD, 4);
      Label l15 = new Label();
      mv.visitJumpInsn(IFEQ, l15);
      mv.visitVarInsn(ILOAD, 4);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l15);
      mv.visitLineNumber(386, l15);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg3", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_2);
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "deref", "(" + LOBJ + ")" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "compareTerm", "(" + LOBJ + LOBJ + ")I");
      mv.visitVarInsn(ISTORE, 4);
      Label l16 = new Label();
      mv.visitLabel(l16);
      mv.visitLineNumber(387, l16);
      mv.visitVarInsn(ILOAD, 4);
      Label l17 = new Label();
      mv.visitJumpInsn(IFEQ, l17);
      mv.visitVarInsn(ILOAD, 4);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l17);
      mv.visitLineNumber(388, l17);
      mv.visitFrame(Opcodes.F_SAME, 0, null, 0, null);
      mv.visitVarInsn(ALOAD, 0);
      mv.visitFieldInsn(GETFIELD, PREDNAME_4, "arg4", LOBJ);
      mv.visitVarInsn(ALOAD, 1);
      mv.visitInsn(ICONST_3);
      mv.visitMethodInsn(INVOKESTATIC, PREDNAME_4, "arg0", "(" + LOBJ + "I)" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "deref", "(" + LOBJ + ")" + LOBJ);
      mv.visitMethodInsn(INVOKESTATIC, I_LANG + "/StaticProlog", "compareTerm", "(" + LOBJ + LOBJ + ")I");
      mv.visitVarInsn(ISTORE, 4);
      Label l18 = new Label();
      mv.visitLabel(l18);
      mv.visitLineNumber(389, l18);
      mv.visitVarInsn(ILOAD, 4);
      mv.visitInsn(IRETURN);
      mv.visitLabel(l5);
      mv.visitLineNumber(391, l5);
      mv.visitFrame(Opcodes.F_CHOP, 3, null, 0, null);
      mv.visitInsn(ICONST_M1);
      mv.visitInsn(IRETURN);
      Label l19 = new Label();
      mv.visitLabel(l19);
      mv.visitLocalVariable("this", LNAME_4, null, l0, l19, 0);
      mv.visitLocalVariable("anotherTerm", LOBJ, null, l0, l19, 1);
      mv.visitLocalVariable("functor2", LOBJ, null, l9, l5, 2);
      mv.visitLocalVariable("arity2", "I", null, l7, l5, 3);
      mv.visitLocalVariable("rc", "I", null, l10, l5, 4);
      mv.visitMaxs(3, 5);
      mv.visitEnd();
    }
    cw.visitEnd();

    return cw.toByteArray();
  }

  final static SingleMethodClass smc = new SingleMethodClass();

  static class SingleMethodClass extends ClassLoader {

    public HashMap classLoaderCache = new HashMap();

    /**
     * @param data
     * @return
     */
    public Class defClass(byte[] data) {

      try {
        // TODO Auto-generated method stub
        Class c = super.defineClass(data, 0, data.length);
        this.resolveClass(c);
        //   getParent().resolveClass(c);
        ///Compiler.compileClass(c);
        return c;
      } catch (Throwable t) {
        t.printStackTrace();
        throw new VerifyError("defClass " + t);
      }
    }

    @Override
    public synchronized Class loadClass(String name, boolean resolve) throws ClassNotFoundException {
      Class c = (Class) this.classLoaderCache.get(name);
      if (c == null) {
        c = this.findSystemClass(name);
        this.classLoaderCache.put(name, c);
      }
      if (resolve) {
        this.resolveClass(c);
      }
      return c;
    }
  }

}
