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
;; vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

(defvar *all-client-stream* (make-hash-table 10))	 


(define wf-new-client-socket (cycname server port) 
   (progn 
  (setq *client-stream* (open-tcp-stream name port :buffering :none))
  ;;;;;(setq stream (system:make-fd-stream   (ext:connect-to-inet-socket name port) :input t :output t :buffering :none))
  (setq stream *client-stream* )
  (read-line stream) ;; atlas server{nl}       
  (format stream "ATLAS OpenCyc-Client~%")
  (format stream "ICAN Packed~%~%")
  (read-line stream) ;; PWILL packed{nl}
  (read-line stream) ;; {nl}
  (setf (gethash (wf-to-name cycname) *all-client-stream*) stream)
  stream))

(define wf-new-client-player (cycname) 
   (progn 
	  (setq username (wf-to-name cycname))
	  (setq stream (gethash username  *all-client-stream*))
	  (setq clientrefno (wf-send-logon username username stream)
	  (setq response (wf-poll stream))
	  (case response
	    (
	  

  

		

(defun wf-test nil (print (wf-connect "localhost" 6767)))
