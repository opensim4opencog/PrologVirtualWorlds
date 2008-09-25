/* KeySortComparator - at Sun Sep 21 05:39:00 PDT 2008 */
package jp.ac.kobe_u.cs.prolog.builtin;

import java.util.Comparator;

import jp.ac.kobe_u.cs.prolog.lang.StaticProlog;

/* used 2 more classes */
/* import jp.ac.kobe_u.cs.prolog.builtin.KeySortComparator; */
/* import java.lang.Object; */

class KeySortComparator extends StaticProlog implements Comparator<Object> {
  public int compare(Object t1, Object t2) {
    /* flow_0_0_LocalStoreOperator
    */
    final Object arg1 = deref(args(t1)[0]);
    final Object arg2 = deref(args(t2)[0]);
    return compareTerm(arg1, arg2);
  }

  //  /* synthetic */public/*volatile*/int compareVolitile(Object object, Object object_0_) {
  //    /* flow_0_0_INVOKE_VIRTUAL
  //    */
  //    return this.compare((Object) object, (Object) object_0_);
  //  }
}
