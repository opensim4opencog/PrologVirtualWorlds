% ===================================================================
% PURPOSE
% This File is the bootstrap for the Moo Infence engine For CIAO-Prolog
% So first is loads the proper files and then starts up the system
% ===================================================================

% ===================================================================
% Prolog Dependant Code
% ===================================================================

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


% numbervars/1 (just simpler numbervars.. will use a rand9ome start point so if a partially numbered getPrologVars wont get dup getPrologVars

numbervars(X):-get_time(T),convert_time(T,A,B,C,D,E,F,G),!,numbervars(X,'$VAR',G,_).

once(X,Y):-once(X),once(Y).

moduleEnsureLoaded(X):-
        catch(ensure_loaded(X),_,(catch((atom_concat('mod/',X,Y),
        ensure_loaded(Y)),_,format(user_error,';; file find error ~q ~q',[X,E])))).

callIfPlatformWin32(G):-prolog_flag(windows,true),!,ignore(G).
callIfPlatformWin32(G):-!.

callIfPlatformUnix(G):-not(prolog_flag(windows,true)),!,ignore(G).
callIfPlatformUnix(G):-!.

/*
:- callIfPlatformWin32(set_prolog_flag(debug_on_error,true)).
:- callIfPlatformUnix(set_prolog_flag(debug_on_error,false)).
:- callIfPlatformUnix(set_prolog_flag(debug_on_error,true)).
*/

% Using getCputime/1 (in Moo code) since Eclipse prolog (another port for Moo)  chokes on getCputime/1
getCputime(Start):-statistics(cputime,Start).

isCharCodelist([]).  isCharCodelist([A|T]):-integer(A),A>9,A<128,isCharCodelist(T).


% Each prolog has a specific way it could unnumber the result of a numbervars
% TODO find optimal solution

unnumbervars(X,Y):-term_to_atom(X,A),atom_to_term(A,Y,_).


% Prolog specific code choices
if_prolog(swi,G):-call(G).  % Run B-Prolog Specifics
if_prolog(_,_):-!.  % Dont run SWI Specificd or others

% used like if_prolog(bp,do_bp_stuff),if_prolog(swi,do_swi_stuff) inline in Moo code

% ========================================================================================
% Some prologs have a printf() type predicate.. so I made up fmtString/writeFmt in the Moo code that calls the per-prolog mechaism
% in SWI it's formzat/N and sformat/N
% ========================================================================================

writeFmtFlushed(X,Y,Z):-catch((format(X,Y,Z),flush_output(X)),_,true).
writeFmtFlushed(X,Y):-catch((format(X,Y),flush_output),_,true).

writeFmt(X,Y,Z):-catch(format(X,Y,Z),_,true).
writeFmt(X,Y):-format(X,Y).
writeFmt(X):-format(X,[]).
fmtString(X,Y,Z):-sformat(X,Y,Z).
fmtString(Y,Z):-sformat(Y,Z).

prologAtInitalization(V):-at_initialization(V),!,logOnFailureIgnore(V).


% ===================================================================
% Semi-Prolog Dependant Code
% ===================================================================

% -------------------------------------------------------------------
% Load the Moo header
% -------------------------------------------------------------------
:-include('moo_header.pl').

% -------------------------------------------------------------------
% Load the Moo Engine
% -------------------------------------------------------------------
:-ensure_loaded('moo_bootstrap.pl').

% dynamic predicates
processBootstrap:-
   dynamicPredicate(X),
   dynamic(X),
  % multifile(X),
  % module_transparent(X),
   fail.

% index predicates
/*
processBootstrap:-
      indexPredicate(X),
      index(X),
      fail.
*/

% load files

processBootstrap:-
   moduleFile(_,Filename),
   use_module(Filename),fail.

processBootstrap:-!.

%:-initialization(processBootstrap).



% -------------------------------------------------------------------
% Set Defaults
% -------------------------------------------------------------------

%:-initialization(setMooOptionDefaults).

% -------------------------------------------------------------------
% Start the system
% -------------------------------------------------------------------

/*
:-callIfPlatformUnix(prologAtInitalization(cs)).

:-callIfPlatformUnix((prologAtInitalization(['mod/mod_nani.pl']),prologAtInitalization(['mod/mod_eliza.pl']),
         prologAtInitalization(kbot),prologAtInitalization(kbot2))).
*/

startJava:-
	 java_start("/opt/sourceforge/logicmoo/src:/opt/sourceforge/logicmoo/src/partner/jamud/src:/opt/sourceforge/logicmoo/lib/:cos.jar:/opt/sourceforge/logicmoo/lib/ecs-1.4.1.jar:/opt/sourceforge/logicmoo/lib/jdom.jar:/opt/sourceforge/logicmoo/lib/jaxp.jar:/opt/sourceforge/logicmoo/lib/rdf-api-2001-01-19.jar:/opt/sourceforge/logicmoo/lib/plc.jar:/opt/sourceforge/logicmoo/lib/cpj.jar:/opt/sourceforge/logicmoo/lib/nanoxml.jar:/opt/sourceforge/logicmoo/lib/jpl.jar:/opt/sourceforge/logicmoo/lib/crawler.jar").

startJamud:-
	 jamud_object(JAMUD),!,
	 java_invoke_method(JAMUD,startJamud(X)).

createJamud:-
	 java_create_object('logicmoo.LogicMoo',JAMUD),
	 %java_object(JAMUD)
	 format('\njamud.Jamud=~q\n',[JAMUD]),!,
	 assert(jamud_object(JAMUD)),!.
			
createJamud:-
	    format('\nCould not create the LogicMOO object\n',[]).
	 

loadJamudReferences:-
	 jamud_object(JAMUD),
	 java_invoke_method(JAMUD,getJamudInstance(Instance)),
	 java_invoke_method(JAMUD,getJamudMudObjectRoot(MudRoot)),!,
	 assert(jamud_instance(Instance)),
	 assert(jamud_root(MudRoot)),
	 writeFmt('Jamud started\n').

startLogicMoo:-
	 startJava,
	 createJamud,
	 startJamud,
	 loadJamudReferences.



%:-initialization(startLogicMoo).


doTopLoop:-
    eng_status.



main(_):-
   %processBootstrap,
   %setMooOptionDefaults,
   startLogicMoo,
   throw(wait_now).








