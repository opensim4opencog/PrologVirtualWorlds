;;   -----original message-----
;;   from: dmiles@users.sourceforge.net [mailto:logicmoo@attbi.com]
;;   sent: friday, august 30, 2002 4:40 pm
;;   to: 'general@worldforge.org'
;;   subject: collaborate on a lisp client or server?
;;   
;;   
;;   any lisp programmers want to collaborate on a lisp client or server?
;;   there is allot of good working rule based ai that can be leveraged.
;;   also cyccorp mentioned if we built a programmatic client in 
;;   common lisp they will help port to system.
;;   a cyc ai agent could play on a wf server:
;;   
;;   suggested work:
;;   
;;   lib/atlas-lisp/ commonest possible lisp native level 
;;   connection utilities and speaking in packed or bach2 
;;   	*open a socket to wf server
;;         *negotiate between ican/iwill
;;   	*translate between a cons and a map
;;         *each server 'op' received is translated into a cons 
;;   and called over (wf-event &value)
;;         *similar this can (wf-send &value) back to server
;;         *define basic higher level macros for create,login,look,etc
;;   
;;   servers/opencyc/ the persistence model/game/rules and 
;;   operations are already coded into a built-in system of opencyc
;;   	we'd need to 
;;   
;;   	*bind a wf serer socket at 6767
;;         *accept clients assign transaction symbol
;;         *use lib/atlas-lisp/ and receive client ops called over 
;;   (wf-event &clientsym &value)
;;         *server processes the event wf-sends replies
;;  
;; vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

;;test with 
;;   (load "atlas.lisp") (wf-send-value (wf-packet "look" nil) *standard-output*)

#|

(defpackage "atlas"
  (:use :common-lisp)
  (:export 
   "wf-poll"
   "wf-send"
   "wf-disconnect"))

(in-package "atlas")
(require "stream")
(require "comm")
(require "ext")
|#


;; ============================================================
;; wf-poll (single map)
;; ============================================================
(define wf-poll (stream)   (read-char stream)  (read-map-value stream))
;; the blank read removes the intial "["

;; ============================================================
;; single map
;; ============================================================
(define read-map-value (stream)  
  (loop  with mymap = (make-hash-table 0) 
      for char  = (read-char stream)
      do (puthash (read-name-string stream) mymap 
                  (case char 
                    ((#\() (read-list-value stream))
                    ((#\$) (read-string-value stream))
                    ((#\@) (read-int-value stream))
                    ((#\#) (read-float-value stream))))
      finally (return mymap))
  
;; ============================================================
;; single list
;; ============================================================
(define read-list-value (stream)  
  (loop for char  = (read-char stream)
  collect 
    (case char 
      ((#\[) (read-map-value stream))
      ((#\() (read-list-value stream))
      ((#\$) (read-string-value stream))
      ((#\#) (read-float-value stream))
      ((#\@) (read-int-value stream))))))

;; ============================================================
;; datatype-next-p
;; ============================================================
(define datatype-next-p (stream)
  (member (stream-peek-char stream) '(#\$ #\@ #\# #\[ #\( #\) #\] )))

;; ============================================================
;; read single name and make it into a keyword
;; ============================================================      
(define read-name-string (stream) 
  (intern (string-upcase
           (coerce 
            (loop for char = (read-char stream) 
	            until (char= char #\=) 
		    collect char) 'simple-string))) "KEYWORD")

;; ============================================================      
;; Intern if not already (Most situations where symbols are used would work well with #$synonomousExternalConcpt)
;; ============================================================      
(define wf-to-name (name)  
  (if (symbolp name) name 
    (intern (string-upcase (coerce name 'simple-string)) "KEYWORD")))


;; ============================================================
;; single string
;; ============================================================
(define read-string-value (stream) 
  (coerce 
   (loop for char = (read-char stream) until (datatype-next-p stream) collect char) 'simple-string))

;; ============================================================
;; single integer
;; ============================================================
(define read-int-value (stream)
  (parse-integer (read-string-value stream)))

;; ============================================================
;; single float
;; ============================================================
(define read-float-value (stream)  
  (coerce (read-from-string (read-string-value stream)) 'Float))

;; ============================================================
;; define wf-send-value (value stream) send arbitrary value
;; ============================================================
(define wf-send-value (value stream) 
  (if (stringp value) (progn (princ #\$ stream)(wf-send-string value stream))
    (if (null value) (princ #\$ stream)
      (if (integerp value) (progn (princ #\@ stream)(princ value stream))
        (if (numberp value) (progn (princ #\# stream)(princ value stream))
          (if (hash-table-p value) (progn (princ #\[ stream)(wf-send-map value stream)(princ #\] stream))
            (progn (princ #\( stream)(wf-send-list value stream)(princ #\) stream))))))))

;; ============================================================
;;  define wf-send-name-and-value  (name value stream)  sends name and value
;; ============================================================
(define wf-send-name-and-value  (name value stream) 
  (if (stringp value) 
      (progn (princ #\$ stream)(wf-send-name name stream) (princ #\= stream) (wf-send-string value stream))
    (if (null value) 
        (progn (princ #\$ stream)(wf-send-name name stream) (princ #\= stream))
      (if (integerp value) 
          (progn (princ #\@ stream)(wf-send-name name stream) (princ #\= stream) (princ value stream))
        (if (numberp value) 
            (progn (princ #\# stream)(wf-send-name name stream) (princ #\= stream) (princ value stream))
        (if (hash-table-p value) 
            (progn (princ #\( stream)(wf-send-name name stream) (princ "=[" stream) (wf-send-map value stream)(princ "])" stream))
          (progn 
             (princ #\( stream)(wf-send-name name stream) (princ #\= stream)
	        (wf-send-list value stream)(princ #\) stream))))))))

;; ============================================================
;;  define wf-send-string (strng stream) sends strng escaped	(TODO)
;; ============================================================
(define wf-send-string (strng stream) 
  (princ strng stream))

;; ============================================================
;;  define wf-send-name (symbol stream) sends symbol lowercased without the ":"
;; ============================================================
(define wf-send-name (name stream) 
  (wf-send-string (string-downcase (symbol-name (wf-to-name name))) stream))

;;(define wf-send-name (name stream) 
;;	(let ((*print-case* :downcase))
;;		(wf-send-string (symbol-name (wf-to-name name)) stream)))

;; ============================================================
;;  define wf-send-map (lst stream) sends lst with no protocol symbol on left or right
;; ============================================================
(define wf-send-map (hshtbl stream)  
  (maphash (lambda (key value) (wf-send-name-and-value key value stream)) hshtbl))

;;test with 
;;   (load "atlas.lisp") (wf-send-value (wf-packet "look" '()) *standard-output*)

;; ============================================================
;;  define wf-send-list (lst stream) sends lst with no protocol symbol on left or right
;; ============================================================
(define wf-send-list (lst stream)
  (dolist (data lst)(wf-send-value data stream)))

;; ============================================================
;;      *define basic higher level macros for create,login,look,etc  
;; ============================================================
(defmacro define-wf-op (opfun opname requiredkeys)
  (list 'define opfun
        (list '&key 
              (loop for symbolname1 in requiredkeys
                  collect (list symbolname1 (make-symbol (symbol-name symbolname1)))))
        (list 'wf-packet opname 
              (loop for symbolname2 in requiredkeys
                  collect (cons symbolname2 (make-symbol (symbol-name symbolname2)))))))

;;(macro-expand (define-wf-op wf-login "login"  (:USERNAME :PASSWORD)))
;;(define-wf-op wf-login "login" '(:USERNAME :PASSWORD))
;;(define-wf-op "wf-look" "look" '(:ID))
;;(define-wf-op "wf-delete" "delete" '(:ID))
;;(define-wf-op "wf-talk" "talk" '(:SAY))		


;; ============================================================
;; Construct a wf-packet for storage
;; ============================================================
(define wf-packet (opname argmap  &key 
                         (to (wf-get-default :TOSERVER))
                         (serialnum (make-serial-num))
                         (from (wf-get-default :FROMACTOR))
                         (refnum 0))
  (wf-map
   (list 
    (cons :OBJTYPE  "op")
    (cons :SERIALNUM  serialnum)
    (cons :REFNUM  refnum)
    (cons :TO  to)
    (cons :FROM  from)
    (cons :PARENTS  (list opname))
    (cons :ARGS  (wf-map argmap))
    (cons :SECONDS  (wf-get-seconds))
    (cons :FUTURE_SECONDS 0.0))))


;; ============================================================
;; Construct a wf-object for storage
;; ============================================================
(define wf-object (id argmap) 
  (wf-map
   (append 
    (list 
      (:OBJTYPE .  "object")
      (cons :ID id)) 
     argmap)))
    
;; ============================================================
;; Construct a wf-map for storage
;; ============================================================

(define wf-map (alist)  
  (if (hash-table-p alist) alist
    (loop  with mymap = (make-hash-table)
        for keyvalue in alist 
        do (ifn hash-map-p alist setf (gethash (wf-to-name (car keyvalue)) mymap) (cdr keyvalue) (map-add-to-map alist mymap))
        finally (return mymap))))

(define make-serial-num nil (get-internal-real-time))

(define wf-get-seconds nil (get-universal-time))

(define wf-get-default (name) "")

;; ============================================================
;; Construct a wf-logon for storage
;; ============================================================
(define wf-op-logon (username password) 
  (wf-packet "logon" 
      (wf-map (list (cons :USERNAME username)(cons :PASSWORD password)))))

;; ============================================================
;; Send a wf-logon to stream
;; ============================================================
(define wf-send-logon (username password stream) 
  (wf-send-value (wf-op-logon username password) stream))
  
;; ============================================================
;; Construct a wf-talk for storage
;; ============================================================
(define wf-op-talk (say) 
  (wf-packet "talk" 
      (wf-map (list (cons :SAY say)))))

;; ============================================================
;; Send a wf-talk to stream
;; ============================================================
(define wf-send-talk (say stream) 
  (wf-send-value (wf-op-talk say) stream))

;; ============================================================
;; Construct a wf-look for storage
;; ============================================================
(define wf-op-look (id) 
  (wf-packet "look" 
      (wf-map (list (cons :id id)))))

;; ============================================================
;; Send a wf-look to stream
;; ============================================================
(define wf-send-look (id stream) 
  (wf-send-value (wf-op-look id) stream))
  
;; ============================================================
;; Construct a wf-touch for storage
;; ============================================================
(define wf-op-touch (id) 
  (wf-packet "touch" 
      (wf-map (list (cons :id id)))))

;; ============================================================
;; Send a wf-touch to stream
;; ============================================================
(define wf-send-touch (id stream) 
  (wf-send-value (wf-op-touch id) stream))
  
;; ============================================================
;; Construct a wf-delete for storage
;; ============================================================
(define wf-op-delete (id) 
  (wf-packet "delete" 
      (wf-map (list (cons :id id)))))

;; ============================================================
;; Send a wf-delete to stream
;; ============================================================
(define wf-send-delete (id stream) 
  (wf-send-value (wf-op-delete id) stream))
  
;; ============================================================
;; Construct a wf-info for storage
;; ============================================================
(define wf-op-info (id) 
  (wf-packet "info" 
      (wf-map (list (cons :id id)))))

;; ============================================================
;; Send a wf-info to stream
;; ============================================================
(define wf-send-info (id stream) 
  (wf-send-value (wf-op-info id) stream))
  


  

