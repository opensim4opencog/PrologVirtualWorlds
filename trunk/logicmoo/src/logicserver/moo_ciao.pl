% ===================================================================
% PURPOSE
% This File is the bootstrap for the Moo Infence engine For CIAO-Prolog
% So first is loads the proper files and then starts up the system
% ===================================================================
% ===================================================================
% Prolog Dependant Code
% ===================================================================

:-module(system_dependant,
      [getCputime/1,
      numbervars/1,
      unnumbervars/2,
      writeSTDERR/1,
      writeSTDERR/2,
      writeFmt/1,
      writeFmt/2,
      writeFmt/3,
      fmtString/2,
      fmtString/3,
      writeFmtFlushed/1,
      writeFmtFlushed/2,
      writeFmtFlushed/3,
      saveUserInput/0,
      writeSavedPrompt/0,
      if_prolog/2,
      callIfPlatformWin32/1,
      callIfPlatformUnix/1,
      prologAtInitalization/1,
      prolog_thread_create/3,
      prolog_current_thread/2,
      prolog_thread_at_exit/1,
      prolog_thread_self/1,
      prolog_thread_at_exit/2,
      prolog_thread_join/2,
      prolog_notrace/1,
      prolog_statistics/0,
      main/1]).


:-use_module(library(system)).
:-use_module(library(read)).
:-use_module(library(prolog_sys)).
:-use_module(library(sort)).


:- use_module(library(concurrency)). 
:- use_module(library(file_utils)).
:- use_module(library(aggregates)).
:- use_module(library(prompt)).
:- use_module(library(errhandle)).
:- use_module(library(format)).
:- use_module(library('graphs/ugraphs')).
:- use_module(library(write)).
:- use_module(library(atom2term)).
:- use_module(library(term_to_string)).
:- use_module(library(terms_vars)).
%:- use_module(library(idlists)).
:- use_module(library(use_url)).
:- use_module(library(vndict)).
:- use_module(library(queues)).
:- use_module(library(lsets)).
:- use_module(library(lists)).
:- use_module(library(llists)).
:- use_module(library(listing)).
:- use_module(library(listing)).
:- use_module(library(sets)).
:- use_module(library(keys)).
:- use_module(library(arrays)).
%:- use_module(library(dcg)).
%:- use_module(library(loading_code)).
%:- use_module(library(builtin_directives)).
%:- use_module(library(trace)).
:- use_module(library('sockets/sockets')).
:- use_module(library('javall/javart')).
:- use_module(library('javall/jtopl')).


% ===================================================================
% PURPOSE
% This File is the bootstrap for the Moo Infence engine For SWI-Prolog
% So first is loads the proper files and then starts up the system
% ===================================================================


% ===================================================================
% Prolog Dependant Code
% ===================================================================

% ========================================================================================
% Using getCputime/1 (in Moo code) since Eclipse prolog (another port for Moo)  chokes on getCputime/1
% ========================================================================================
getCputime(Start):-statistics(cputime,Start).
prolog_statistics:-statistics.
prolog_notrace(G):-notrace(G).

% ========================================================================================
% Threads 
% ========================================================================================
prolog_thread_create(Goal,Id,Options):-thread_create(Goal,Id,Options).
prolog_current_thread(Id,Status):-current_thread(Id,Status).
prolog_thread_at_exit(Goal):-thread_at_exit(Goal).
prolog_thread_self(Id):-thread_self(Id).
prolog_thread_at_exit(Id,Goal):-thread_at_exit(Id,Goal).
prolog_thread_join(Id,X):-thread_join(Id,X).

% ========================================================================================
% Some prologs have a printf() type predicate.. so I made up fmtString/writeFmt in the Moo code that calls the per-prolog mechaism
% in SWI it's formzat/N and sformat/N
% ========================================================================================
:-dynamic(isConsoleOverwritten/0).

writeFmtFlushed(X,Y,Z):-catch((format(X,Y,Z),flush_output(X)),_,true).
writeFmtFlushed(X,Y):-catch((format(X,Y),flush_output),_,true).
writeFmtFlushed(X):-catch((format(X,[]),flush_output),_,true).

writeFmt(X,Y,Z):-catch(format(X,Y,Z),_,true).
writeFmt(X,Y):-format(X,Y).
writeFmt(X):-format(X,[]).

fmtString(X,Y,Z):-sformat(X,Y,Z).
fmtString(Y,Z):-sformat(Y,Z).

saveUserInput:-retractall(isConsoleOverwritten),flush_output.
writeSavedPrompt:-not(isConsoleOverwritten),!.
writeSavedPrompt:-flush_output.
writeOverwritten:-isConsoleOverwritten,!.
writeOverwritten:-assert(isConsoleOverwritten).

writeSTDERR(F):-writeSTDERR('~q',[F]).
writeSTDERR(F,A):-system_dependant:prolog_notrace((
        format(user_error,F,A),
        nl(user_error),
        flush_output(user_error))).

writeErrMsg(Out,E):-message_to_string(E,S),writeFmtFlushed(Out,'<prolog:error>~s</prolog:error>\n',[S]),!.
writeErrMsg(Out,E,Goal):-message_to_string(E,S),writeFmtFlushed(Out,'<prolog:error>goal "~q" ~s</prolog:error>\n',[Goal,S]),!.
writeFileToStream(Dest,Filename):-
        catch((
        open(Filename,'r',Input),
        repeat,
                get_code(Input,Char),
                put(Dest,Char),
        at_end_of_stream(Input),
        close(Input)),E,
        writeFmtFlushed('<prolog:error goal="~q">~w</prolog:error>\n',[writeFileToStream(Dest,Filename),E])).


% ========================================================================================
% numbervars/1 (just simpler numbervars.. will use a rand9ome start point so if a partially numbered getPrologVars wont get dup getPrologVars
% Each prolog has a specific way it could unnumber the result of a numbervars
% ========================================================================================

numbervars(X):-get_time(T),convert_time(T,A,B,C,D,E,F,G),!,numbervars(X,'$VAR',G,_).
unnumbervars(X,Y):-term_to_atom(X,A),atom_to_term(A,Y,_).


% ========================================================================================
% Ensure a Module is loaded
% ========================================================================================
moduleEnsureLoaded(X):-
        catch(ensure_loaded(X),_,(catch((atom_concat('mod/',X,Y),
        ensure_loaded(Y)),_,format(user_error,';; file find error ~q ~q',[X,E])))).

% ========================================================================================
% Platform specifics
% ========================================================================================
callIfPlatformWin32(G):-prolog_flag(windows,true),!,ignore(G).
callIfPlatformWin32(G):-!.

callIfPlatformUnix(G):-not(prolog_flag(windows,true)),!,ignore(G).
callIfPlatformUnix(G):-!.

/*
:- callIfPlatformWin32(set_prolog_flag(debug_on_error,true)).
:- callIfPlatformUnix(set_prolog_flag(debug_on_error,false)).
:- callIfPlatformUnix(set_prolog_flag(debug_on_error,true)).
*/

% ========================================================================================
% Prolog specific code choices
% ========================================================================================
if_prolog(swi,G):-call(G).  % Run B-Prolog Specifics
if_prolog(_,_):-!.  % Dont run SWI Specificd or others

% used like if_prolog(bp,do_bp_stuff),if_prolog(swi,do_swi_stuff) inline in Moo code


prologAtInitalization(V):-at_initialization(V),!,logOnFailureIgnore(V).


% ===================================================================
% Semi-Prolog Dependant Code
% ===================================================================
sigma_ua(X):-processRequest(X).


% -------------------------------------------------------------------
% Load the Moo header
% -------------------------------------------------------------------
:- current_prolog_flag(arch,'i386-win32') ->
      ensure_loaded('moo_swiprolog_win32.pl') ; ensure_loaded('moo_swiprolog_unix.pl').

:-include('moo_header.pl').

% -------------------------------------------------------------------
% Load the Moo Engine
% -------------------------------------------------------------------
:-ensure_loaded('moo_bootstrap.pl').

% load files
processBootstrap:-
   moduleFile(_,Filename),
   ensure_loaded(Filename),fail.

processBootstrap:-!.


startLogicMoo:-
	 startJava,
	 createJamud,
	 startJamud,
	 loadJamudReferences.


java_start(_).
java_create_object(_,_).
java_invoke_method(_,_).
java_object(_).
% -------------------------------------------------------------------
% Start the system
% -------------------------------------------------------------------



main(Port):-
   ignore(Port=5001),
   processBootstrap,
   setMooOptionDefaults,
   startLogicMoo,
   setMooOption(client,html),
   createPrologServer(80),
   createPrologServer(Port),
   callIfPlatformUnix((prologAtInitalization(['mod/mod_nani.pl']),prologAtInitalization(['mod/mod_eliza.pl']),
         prologAtInitalization(bot),prologAtInitalization(bot2))).
   %throw(wait_now).






