% =====================================
% supplies hook specific architecture
% =====================================


:- use_module(library(socket)).  
% Load the TCP Library
:- use_module(library(unix)).      
:- use_module(library(shell)).      
:- use_module(library(shlib)).        
:- use_module(library(url)).        
:- use_module(library(quintus)).        
:-ensure_loaded(moo_alarm).

      
:-use_module(library(threadutil)).        

call_with_time_limit(Goal,Time,_):-
	retractall(cpuend(_)),
	getCputime(Now),
	Then is Now + Time,
	assert(cpuend(Then)),!,Goal.
%call_with_time_limit(Goal,Time,timed_out):-!.
%getCleanCharsWhitespaceProper(X,X):-!.
	
getCleanCharsWhitespaceProper(X,Z):-string_clean(X,C),!,string_to_list(C,List),!,ltrim(List,Z),!.


%:-assert((thread_util:open_xterm(T,In,Out):-moo_server_break(T,In,Out))).


/*
:- redefine_system_predicate(message_hook(_,_,_)).

:- asserta(((user:message_hook(A,B,C) :- moo_server_message_hook(A,B,C)))).

moo_server_message_hook(trace_mode(on),B,Lines):-
	catch(getThread(Id),_,fail),
	'$get_pid'(Pid),
	fmtString(Title, 'SWI-Prolog Process ~w (pid ~d) interactor', [Id, Pid]),
	tty_in(Stream),
	system_dependant:prolog_thread_at_exit(main,(close(Stream))),
	set_input(Stream),
	set_output(user_output),
	system_dependant:prolog_thread_at_exit(system_dependant:prolog_thread_at_exit(main,set_input(Stream))).
*/
:-current_input(Stream),assert(tty_in(Stream)).
:-current_output(Stream),assert(tty_out(Stream)).


bt(ID):-system_dependant:prolog_thread_at_exit(ID,trace)			.

moo_server_message_hook(A,B,Lines):-
	'$get_pid'(Pid),
	writeFmt(user_error,'% Moo ~w (~w): ~w \n',[Pid,B,A]),flush_output(user_error),
	print_message_lines(user_error, '% ', Lines),flush_output(user_error).



moo_server_break(T,user_input,user_output):-
		writeFmt(user_error,'Switching main thread to sleep mode. \n ~w \n',[T]).

                      



