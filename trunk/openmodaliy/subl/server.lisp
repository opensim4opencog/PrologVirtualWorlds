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


(define run-atlas-server (port) 
  (let ((sever-socket (create-inet-listener port :stream))))
  (create-task
    (loop (accept-and-task sever-socket))))
  
(define accept-and-task (sever-socket)
   (create-task (service-client-stream (accept-client-make-stream sever-socket))))

(define service-client-stream (stream) 
   (progn 
     (format "ATLAS OpenCYC-Server   ~%" stream)
     (read-line stream)	 ;; Remove the client type from stream
     (loop until (equal (read-line stream) ""))	 ;;Get all ingnore all headers
      (loop 
      cdo (atlas-respond (wf-poll stream) stream)
      until (end-of-stream stream))))
      ;; Loop until the client is there no more
      
(define atlas-respond (message stream) 
  (case (car (gethash :PARENTS message))
    (("logon") (wf-serv-logon message stream))
    (("logoff") (wf-serv-logoff message stream))
    (("look") (wf-serv-look message stream))
    (("get") (wf-serv-get message stream))
    (("set") (wf-serv-set message stream))
    (("create") (wf-serv-create message stream))
    (("info") (wf-serv-info message stream))
    (("talk") (wf-serv-talk message stream))
    (("move") (wf-serv-move message stream)))
     
;;(define wf-serv-logon (message stream) 


(define create-inet-listener (port &optional (kind :stream))
  "Create and return a new inet-socket, listening on PORT.  KIND can either be :stream or :datagram."
  (declare #.*std-optimize-declarations*)
  (let ((socket (create-inet-socket kind)))
    (progn
      ;; Set an sockoption:
      ;; Avoid EADDRINUSE for sockets in TIME_WAIT or CLOSE_WAIT state.
      ;; Sockets in LISTEN or ESTABLISHED state still will yield an error.
      (setf (sockopt-reuse-address socket) t)
      (let ((sockaddr (allocate-inet-sockaddr)))
        (unwind-protect
          (progn
            (setf (inet-sockaddr-family sockaddr) af-inet)
            (setf (inet-sockaddr-port sockaddr) (htons port))
            (setf (inet-sockaddr-addr sockaddr) 0)
            (when (minusp (unix-bind socket
                                     sockaddr
                                     size-of-inet-sockaddr))
              (unix-close socket)
              (socket-error "create-inet-listener")))
          (free-inet-sockaddr sockaddr)))
      (when (eq kind :stream)
        (when (minusp (unix-listen socket 5))
          (unix-close socket)
          (socket-error "create-inet-listener")))
      socket)))


(define accept-tcp-connection (unconnected-fd)
  "Create and return a new socket connected to the incoming TCP connection on UNCONNECTED-FD."
  (declare #.*std-optimize-declarations*)
  (declare (fixnum unconnected-fd))
  #+:sb-mp (sb-mp:process-wait-until-fd-usable unconnected :input)
  (let ((sockaddr (allocate-inet-sockaddr)))
    (unwind-protect
      (let ((connected (unix-accept unconnected-fd sockaddr size-of-inet-sockaddr)))
        (when (minusp connected)
          (socket-error "accept-tcp-connection"))
        (values connected (ntohl (inet-sockaddr-addr sockaddr))))
      (free-inet-sockaddr sockaddr))))
