     %:-module(moo_ciao,[]).

% ===================================================================
% PURPOSE
% This File is the bootstrap for the Moo Infence engine For SWI-Prolog
% So first is loads the proper files and then starts up the system
% ===================================================================
   :-use_module(library(dynamic),[asserta/1, asserta/2, assertz/1, assertz/2, assert/1, assert/2, retract/1, retractall/1, abolish/1, clause/2, clause/3, current_predicate/1, current_predicate/2, dynamic/1, data/1, wellformed_body/3]).
:-use_module(library(format)).

/*
:-use_module(library(system)).
:-use_module(library(read)).
:-use_module(library(prolog_sys)).
%%:-use_module(library(iso)).
:- use_module(library(libpaths)). 

*/

:- use_module(library(linda)). 
%:- use_module(library(io)). 
:- use_module(library(iso)). 
                
%main(_):-write(hello),nl.

%:- use_package(debug).
%:- use_package(trace).
%:- use_package(nodebug).


/*

:-use_module(library(listing)).
:-use_module(library(sgml)).
:-use_module(library(rdf)).
:- use_module(library(socket)).  
 %Load the TCP Library
%:- use_module(library(unix)).      
:- use_module(library(shell)).      
:- use_module(library(shlib)).        
:- use_module(library(url)).        
:- use_module(library(quintus)).        
:- use_module(library(qsave)).        

*/

%:- use_module(library(backcomp)).        


/*
:-make.
*/	
index(_).

style_check( _).
style_check( _).



multi_module(X):-
	dynamic(X),
	multifile(X),
	module_transparent(X).

register_moo_file_type(Filename,Type) :-
	(moo_file_type(Filename,Type) ->
		true;
		assertz(moo_file_type(Filename,Type))),!.

writeModeSet(Mode):-
	(unsetMooOption(client=_)),
	(setMooOption(client=Mode)),!.

:-ensure_loaded('moo_utility.pl').

%:-absolute_file_name(.,Local),assert('LOGIC_ENGINE_RT'(Local)),assert(library_directory(Local)).

%:-absolute_file_name('../..',X),assert('ROOT_RT'(X)).

% numbervars/1 (just simpler numbervars.. will use a rand9ome start point so if a partially numbered getPrologVars wont get dup getPrologVars

numbervars(X):-get_time(T),convert_time(T,A,B,C,D,E,F,G),!,numbervars(X,'$VAR',G,_).

once(X,Y):-once(X),once(Y).

:- dynamic mooCache/1.
:- dynamic mooCache/6.
:- dynamic mooCache/5.
:- dynamic mooCache/9.
	


moduleEnsureLoaded(X):-!,[(X)].

moduleEnsureLoaded(X):-
	catch([X],_,(
        atch((atom_concat('mod/',X,Y),
	ensure_loaded(Y)),_,format(user_error,';; file find error ~q ~q',[X,E])))).

moduleEnsureLoaded(X):-
	catch(ensure_loaded(X),_,(catch((atom_concat('mod/',X,Y),
	ensure_loaded(Y)),_,format(user_error,';; file find error ~q ~q',[X,E])))).

	

                    /*

:- set_prolog_flag(optimise,false).
:- set_prolog_flag(file_name_variables,false).
:- set_prolog_flag(agc_margin,0).
:- set_prolog_flag(trace_gc,false).
:-set_prolog_flag(character_escapes,true).
%:-set_prolog_flag(double_quotes,string).
:-set_prolog_flag(double_quotes,codes).
:-set_prolog_flag(report_error,true).
:-set_prolog_flag(verbose,normal).
:-set_prolog_flag(unknown,error).

                       */
prolog_flag(windows,false).

%set_prolog_flag(_,_).

%current_prolog_flag(_,_).

% (If -> Then ; Else) :- (If,Then,!);Else.



% callIfPlatformWin32(G):-prolog_flag(windows,true),!,ignore(G).
callIfPlatformWin32(G):-G.

% callIfPlatformUnix(G):-not(prolog_flag(windows,true)),!,ignore(G).
callIfPlatformUnix(G):-G.

/*
:- callIfPlatformWin32(set_prolog_flag(debug_on_error,true)).
:- callIfPlatformUnix(set_prolog_flag(debug_on_error,false)).
:- callIfPlatformUnix(set_prolog_flag(debug_on_error,true)).
*/

% Using getCputime/1 (in Moo code) since Eclipse prolog (another port for Moo)  chokes on getCputime/1
getCputime(Start):-statistics(cputime,Start).

isCharCodelist([]).  isCharCodelist([A|T]):-integer(A),A>9,A<128,isCharCodelist(T).

% Using retractall/1 (in Moo code) since Eclipse prolog (another port for Moo)  chokes on retractall/1

/*
:- current_prolog_flag(arch,'i386-win32') -> 
      ensure_loaded('moo_swiprolog_win32.pl') ; ensure_loaded('moo_swiprolog_unix.pl').
*/

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

%:-mode(writeFmtFlushed(+,+)).
%:-mode(writeFmtFlushed(+,+,+)).

writeFmtFlushed(X,Y):-catch((format(X,Y),flush_output),_,true).
writeFmtFlushed(X,Y,Z):-catch((format(X,Y,Z),flush_output(X)),_,true).

writeFmt(X,Y,Z):-catch(format(X,Y,Z),_,true).
writeFmt(X,Y):-format(X,Y).
writeFmt(X):-format(X,[]).
fmtString(X,Y,Z):-sformat(X,Y,Z).
fmtString(Y,Z):-sformat(Y,Z).

% =============================================
% Sets Moo Defaults (After intialization)
% =============================================
/*
assert( show_entails ).
assert( show_all_explainations ).
assert( debug_tests ).
  */

% =============================================
% Load the Moo header and Engine 
% =============================================
         
prologAtInitalization(V):-at_initialization(V),!,logOnFailureIgnore(V).

%:-setMooOptionDefaults.


%:-callIfPlatformUnix(prologAtInitalization(cs)).	 	

%:-callIfPlatformUnix((prologAtInitalization(['mod/nani.sig']),prologAtInitalization(['mod/eliza.sig']),prologAtInitalization(kbot),prologAtInitalization(kbot2))).	 	



                
%:-module(moo_ciao,[]).

% ===================================================================
% PURPOSE
% This File is the bootstrap for the Moo Infence engine For SWI-Prolog
% So first is loads the proper files and then starts up the system
% ===================================================================

:-include('moo_header.pl').


main(_):-moduleEnsureLoaded('moo_bootstrap.pl'),cs.
