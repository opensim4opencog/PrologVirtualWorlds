
% This is only used by SWI as a drop-in for CIAO-Prolog bidirrectional interface for Java

:-module(javart,[
	 java_start/0,
	 java_start/1,
	 java_start_local/0,
	 java_start_local/1,
	 java_create_object/2,
	 java_invoke_method/2,
	 java_object/1,
	 java_test/0,
	 java_get_value/2
      ]).

:- use_module(library(shlib)).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

:-module_transparent('$java_machine_thread'/1).
:-dynamic('$java_machine_thread'/1).



:- initialization(load_foreign_library(foreign(javart))).

java_start:-
       ('$java_machine_thread'(_) ; 
       (thread_create(java_start_local,X,[]),
	 assert('$java_machine_thread'(X)))),!.

% Main thread

java_start_local :- 
	 thread_self(main),!,
	 (java_create_vm -> 
	       assert('$java_machine_thread'(main)) ; 
	       throw(('could not create JVM'))).

% Other threads
java_start_local:- 
   java_create_vm,!,
    repeat,
      once(thread_get_message(Client:Term)),
      once((catch((Term),E,(format(user_error,'~q',[E]),fail)),Return=Term);Return=true),
      once(thread_send_message(Client,Return)),
      fail.

java_start_local:-
      retractall('$java_machine_thread'(_)),
      throw(('could not create JVM')).


java_test:-
      java_start,
      java_create_object('java.lang.String',Object),writeq(Object),nl.

   
java_start(AtomPath):-	atom(AtomPath),!,
   setenv('CLASSPATH',AtomPath),
   java_start.

java_start(ClassPath):- notrace(( 	is_list(ClassPath),!,
   string_to_atom(ClassPath,AtomPath),
   java_start(AtomPath))).

java_start_local(AtomPath):- notrace((   atom(AtomPath),!,
   setenv('CLASSPATH',AtomPath),
   java_start_local)).

java_start_local(ClassPath):-   notrace(( is_list(ClassPath),!,
   string_to_atom(ClassPath,AtomPath),
   java_start_local(AtomPath))).

java_create_object(Class,Object):-
	notrace(java_invoke_object_at_thread('$$$$',createObject,[Class],Object)).

java_destroy_object(Object):-
	notrace(java_invoke_object_at_thread('java_object'(0),forgetObject,[Object],_True)).

java_object(0).
java_object(X):-var(X),!,
	 java_invoke_object_at_thread('java_object'(0),'#g',[allObjects],All),!,
	 All=..[_,List],
	 member(X,List).

java_object(X):-compound(X),!.





java_invoke_method(Object,Method):-
	notrace((
	  % java_object(Object),
	   nonvar(Method),
	   Method=..[MethodName|ParamsRes],
	   append(Params,[Result],ParamsRes))),
	   java_invoke_object_at_thread(Object,MethodName,Params,JResult),!,
	JResult=Result.
	 
java_get_value(Object,FA):-
	 nonvar(FA),
	 FA=..[Field,Value],
	notrace(java_invoke_object_at_thread(Object,'#g',[Field],Value)).

java_set_value(Object,FA):-
	 nonvar(FA),
	 FA=..[Field,Value],
	notrace(java_invoke_object_at_thread(Object,'#s',[Field,Value],_)).
	      

java_invoke_object_at_thread(Obj,Method,Args,Return):-
      '$java_machine_thread'(JavaThread),
      thread_self(Client),!,
      java_call_to_thread(Client,JavaThread,
	       java_invoke_object_protected(Obj,Method,Args,Return)).

java_call_to_thread(JavaThread,JavaThread,Goal):-!,Goal.

java_call_to_thread(Client,JavaThread,Goal):-
      thread_send_message(JavaThread,(Client:Goal)),!,
      thread_get_message(Result),!,
      Result=Goal.





  
