/***********************************************************************

  OKBC implementation for Java

  Copyright (c) 1996, 1997 Stanford University KSL.

  Original code by JonL White.  Translated into Java by
  James Rice (rice@ksl.stanford.edu) from an algorithm described as:
  `A "traditional" list merge sort' by Guy Steele in MIT AI memo 587 (aug '80).


  KSL Network Services available at:
            http://www-ksl-svc.stanford.edu:5915/

  Legal Notice for KSL Network Services

  The software made available through the Stanford Knowledge Systems
  Laboratory's Network Services was written by and is copyright (c) by
  Stanford University, Knowledge Systems Laboratory and others not
  limited to those mentioned below.
  
  NEITHER THE AUTHORS NOR THE PUBLISHER OF THESE SERVICES AND DOCUMENTS
  OFFER ANY WARRANTY OF FITNESS OR MERCHANTABILITY FOR ANY PARTICULAR
  PURPOSE OR ACCEPTS ANY LIABILITY WITH RESPECT TO THE SERVICES,
  PROGRAMS, PROCEDURES, ALGORITHMS, DOCUMENTS OR APPLICATIONS OR
  DOCUMENTS MADE AVAILABLE THROUGH THE KNOWLEDGE SYSTEMS LABORATORY'S
  NETWORK SERVICES.
  
  
  ***********************************************************************/

package OKBC;
import java.util.*;

public class ListSorter {

  public static final SortPredicate default_sort_predicate
    = new SortPredicate();
  public static final SortKey default_sort_key = new SortKey();

// List-Sort reorders the cons cells of the argument 'list' to be in the
//  order prescribed by 'predicate' (this is a "destructive" operation.)
// In theory, this is based on an algorithm described as:  `A "traditional"
//   list merge sort' by Guy Steele in AI memo 587 (aug '80).
// 
// This sort is stable.

  public static Node sort (Node list) {
    return sort(list, default_sort_predicate, default_sort_key);
  }

  public static Node sort (Node list, SortPredicate predicate) {
    return sort(list, predicate, default_sort_key);
  }

  public static Node sort (Node list, SortPredicate predicate, SortKey key) {
    if (list == null) { list = Node._NIL; }  // reset to real NIL.
    if (list == Node._NIL || list == null || list instanceof Cons)
      {
	NodeLocation unsorted_tail = new NodeLocation(list);
	int height = 0;
	Node ll = Node._NIL;
	while (unsorted_tail.non_null()) {
	  ll = list_merge(ll, sort_initial_segment(height, unsorted_tail,
						   predicate, key),
			  predicate, key);
	  height = height + 1;
	  //System.out.println("Sort: remaining tail = " + unsorted_tail.node);
	}
	return ll;
      }
    else
      { throw new GenericError
	   (Node._T, new OKBCString(list + " is not a list")).signal();
      }
  }


  static Node bite_off_one_cell (NodeLocation loc) {
    Node cell = loc.get_node();
    loc.pop();
    ((Cons)cell).set_cdr(Node._NIL);
    return cell;
  }


  static Node sort_initial_segment (int height, NodeLocation unsorted_tail,
				    SortPredicate predicate, SortKey key) {
    Node result = null;
    // System.out.println("Enter sort_initial_segment(" + height + ", " + unsorted_tail.node + ") ->");
    if (unsorted_tail.non_null())
      {
	if (height < 1)
	  { result = bite_off_one_cell(unsorted_tail); }
	else
	  {
	    height = height - 1;
	    result = list_merge(sort_initial_segment
				(height,unsorted_tail, predicate, key),
				sort_initial_segment
				(height,unsorted_tail, predicate, key),
				predicate, key);
	  }
      }
    else { result = Node._NIL; }
    // System.out.println("Exit sort_initial_segment() ->" + result);
    return result;
  }


// Destructively merges list x with list y.  In the resultant list, 
//  the elements of y are guaranteed to come after equivalent elements
//  of x (i.e. it is "stable".)

  static Node list_merge
           (Node x, Node y, SortPredicate predicate, SortKey key) {
    Node result = null;
    // System.out.println("Enter list_merge(" + x + ", " + y + ") ->");
    if (x == null || x == Node._NIL || y == null || y == Node._NIL)
      {
	if (x != null && x != Node._NIL) { result = x; }
	else { result = y; }
      }
    else 
      {
      // Note: The equivalence test must be performed for y < x, so that 
      // the merge will be stable for x.
      Node first_cell = null;
      // We do this longhand here because we don't have a NodeLocation
      // for x or y.  Bring back macros all is forgiven....
      if (in_order_p(y, x, predicate, key))
	{ 
	  Cons cell = (Cons)y;
	  y = y.cdr();
	  cell.set_cdr(Node._NIL);
	  first_cell = cell;
	}
      else 
	{ 
	  Cons cell = (Cons)x;
	  x = x.cdr();
	  cell.set_cdr(Node._NIL);
	  first_cell = cell;
	}
      Cons current_cell = (Cons)first_cell;
      while (x != null && y != null && x != Node._NIL && y != Node._NIL) {
	if (in_order_p(y, x, predicate, key))
	  {
	    current_cell.set_cdr(y);
	    current_cell = (Cons)y;
	    y = y.cdr();
	  }
	else
	  { 
	    current_cell.set_cdr(x);
	    current_cell = (Cons)x;
	    x = x.cdr();
	  }
      }
      // Exits only when at least one of x and y is null; so then
      // append the other as the remainder of the result.
      current_cell.set_cdr((x == null || x == Node._NIL) ? y : x);
      result = first_cell;
    }
    // System.out.println("Exit list_merge() ->" + result);
    return result;
  }

  static boolean in_order_p
        (Node lx, Node ly, SortPredicate predicate, SortKey key) {
    Node a = key.key(lx.car()); // extract the sort key if need be
    Node b = key.key(ly.car());
    return predicate.test(a, b);
  }


  // Test harness
  public static void main (String arg[]) {
    Node test = Cons.list(new OKBCInteger(42),
			  new OKBCInteger(43),
			  new OKBCInteger(0),
			  new OKBCInteger(2),
			  new OKBCInteger(2001),
			  new OKBCInteger(2),
			  new OKBCInteger(-10),
			  new OKBCInteger(10),
			  new Real(3.14));
    System.out.println("Test = ");
    Cons.pretty_print(test);
    System.out.println();
    System.out.println("Result = ");
    Cons.pretty_print(sort(test));
    System.out.println();
  }
}




/**********
// Here's the original Lisp code.....


(in-package "USER")


;;; List-Sort reorders the cons cells of the argument 'list' to be in the
;;;  order prescribed by 'predicate' (this is a "destructive" operation.)
;;; In theory, this is based on an algorithm described as:  `A "traditional"
;;;   list merge sort' by Guy Steele in AI memo 587 (aug '80).
;;; 
;;; This sort is stable.


(defun list-sort (list predicate &key (key #'identity))
  ;;*unsorted-tail* updated  in sort-initial-segment
  (when (and (symbolp predicate) (fboundp (the symbol predicate)))
    (setq predicate (symbol-function predicate)))
  (check-type list list)
  (check-type predicate function)
  (when (and (symbolp key) (fboundp (the symbol key)))
    (setq key (symbol-function key)))
  (check-type key function)
  (let ((*unsorted-tail* list)
	(*ordering-fn* predicate)
	(*selection-fn* key)
	(height 0)
	(ll '()))
    (declare 
	(fixnum height) 
	(special *unsorted-tail* *ordering-fn* *selection-fn*))
    ;; Iteratively merge the sorted partial lists.
    (loop while *unsorted-tail*
	  do (setq ll (list-merge ll (sort-initial-segment height)))
	     (incf height))
    ll))
      

(eval-when (eval compile)
(defmacro bite-off-one-cell (var)
  `(LET ((CELL ,var))
     (SETQ ,var (CDR CELL))
     (RPLACD CELL NIL)
     CELL))
)
  

(defun sort-initial-segment (height)
  (declare (special *unsorted-tail* *ordering-fn* *selection-fn*)
	   (fixnum height))
  (unless (null *unsorted-tail*)
    (if (< height 1)
	(bite-off-one-cell *unsorted-tail*)
	(progn
	  (decf height)
	  (list-merge (sort-initial-segment height)
		      (sort-initial-segment height))))))


;;; Destructively merges list x with list y.  In the resultant list, 
;;;  the elements of y are guaranteed to come after equivalent elements
;;;  of x (i.e. it is "stable".)

(defun list-merge (x y)
  (declare (special *ordering-fn* *selection-fn*)
	   (type function *ordering-fn* *selection-fn*))
  (if (or (null x) (null y))
      (or x y)
      (flet ((in-order? (lx ly)
		(let ((a (car lx))
		      (b (car ly)))
		  (unless (eq *selection-fn* #'identity)
		    (setq a (funcall *selection-fn* a)
			  b (funcall *selection-fn* b)))
		  ;; A little optimization for the common numerical case.
		  (cond 
		   ((eq *ordering-fn* #'<)  (< a b))
		   ((eq *ordering-fn* #'<=) (<= a b))
		   ((eq *ordering-fn* #'>)  (> a b))
		   ((eq *ordering-fn* #'>=) (>= a b))
		   (t  (funcall *ordering-fn* a b))))))
	;; Note: The equivalence test must be performed for 
	;;     y < x,  so that the merge will be stable for x.
	(loop with first-cell = (if (in-order? y x)
				    (bite-off-one-cell y)
				    (bite-off-one-cell x))
	      with current-cell = first-cell
	  while (and x y)
	  if (in-order? y x)
	    then
	     do (rplacd current-cell y)
		(setq current-cell y)
		(setq y (cdr y))
	    else
	     do (rplacd current-cell x)
		(setq current-cell x)
		(setq x (cdr x))
	  finally
	   do ;; Exits only when at least one of x and y is null; so then
	      ;;  append the other as the remainder of the result.
	      (rplacd current-cell (if (null x) y x))
	      (return first-cell)))))


*********/
