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

callIfPlatformUnix(G):- \+ (prolog_flag(windows,true)),!,ignore(G).
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
	 showInstanceValue(MudRoot).
	 
showInstanceValue(Object):-
   getJavatypeValueFn(Object,Name,ActualName,Value),
   format('\n ~q \n',[equal('JavaMemberFn'('JavaObjectFn'(Object),Name),Value)]),
   fail.
showInstanceValue(Object):-
   inferJavatypeMemberFnTuples(Term),
   format('\n~q\n',[Term]),
   fail.

showInstanceValue(Object):-!.

:-dynamic(classToPLStructure/2).

java_classToPLStructure(Object,Term):-
	 nonvar(Object),classToPLStructure(Object,Term),!.

java_classToPLStructure(Object,Term):-
      jamud_object(JAMUD),
      java_invoke_method(JAMUD,classToPLStructure(Object,Term)),!,
      assert(classToPLStructure(Object,Term)).

getInstanceValue(Object,ActualName,Value):-
      getInstanceValue(Object,Name,ActualName,Value),!.
getInstanceValue(Object,Name,Value):-
      getInstanceValue(Object,Name,ActualName,Value),!.

getInstanceValue(Object,Name,ActualName,Value):-
      getInstanceValue(Object,Name,ActualName,Value,Type).

:-dynamic(java_object_known/1).

inferJavatypeMemberFnTuples(equal('JavaMemberFn'('JavaObjectFn'(Object),Name),Result)):-
      java_object(Object),java_object_known(Object),
      getJavatypeValueFn(Object,Name,ActualName,Result).

getJavatypeValueFn(Object,ActualName,Value):-
      getJavatypeValueFn(Object,Name,ActualName,Value),!.
getJavatypeValueFn(Object,Name,Value):-
      getJavatypeValueFn(Object,Name,ActualName,Value),!.
getJavatypeValueFn(Object,Name,ActualName,'JavaValueFn'(Type,Value)):-
      getInstanceValue(Object,Name,ActualName,Value,Type).


getInstanceValue(Object,Name,ActualName,Value,Type):-
      java_classToPLStructure(Object,Term),
      get_call_term_from_class(Object,Term,Name,Value,Type,ActualName,CallTerm),
      catch(CallTerm,E,format('\nAttempting "~q" resulted in ~q\n',[CallTerm,E])).

get_call_term_from_class(Object,Term,Name,Value,Type,Name,java_get_value(Object,MethodCall)):-
      catch(arg(1,Term,Arg1),_,fail),
      arg(1,Arg1,FieldsList),
      member(Field,FieldsList),
      functor(Field,ActualName,1),
      arg(1,Field,Type),
      MethodCall=..[Name,Value].

get_call_term_from_class(Object,Term,Name,Value,Type,ActualName,java_invoke_method(Object,MethodCall)):-
      catch(arg(2,Term,Arg1),_,fail),
      arg(1,Arg1,MethodsList),
      member(Method,MethodsList),
      functor(Method,ActualName,MA),
      arg(MA,Method,Type),
      match_method(MethodsList,Method,ActualName,MA,Name,Value,MethodCall).

match_method(MethodsList,Method,ActualName,MA,Name,Value,MethodCall):-
      var(Name),
      % Make sure we don't walk blindly into some side-effect method when the Name is a variable
      member(Lead,['load','save','set','add','wait','equals','toString',
	 'unload','create','destroy','kill','stop','terminate','final','write','print','append',
	 ('['),('_'),('(') ]),atom_concat(Lead,_,ActualName),!,fail.
      
% Not that zero length happens but just incase fail. 
match_method(MethodsList,Method,ActualName,0,Name,Value,MethodCall):-!,fail.

% Explore Simple No parameter methods
match_method(MethodsList,Method,ActualName,1,Name,Value,MethodCall):-!,
           % Look for an identical match
      ((Name == ActualName) ;
           % Or no match, guess what was wanted by prepending 'get' or 'to'
      ((atom_concat('get',Name,ActualName);atom_concat('to',Name,ActualName)),
   	   % If we did a prepend, confirm that the Name was not already in list 
	 \+ (member(Term,MethodsList),functor(Term,Name,_)))),!,
      MethodCall=..[ActualName,Value].

% Explore Explicitly called 
match_method(MethodsList,Method,ActualName,MA,Name,Value,MethodCall):-
      nonvar(Name),functor(Name,ActualName,TA),
      Name =.. [ActualName|Rest],!,
      ((TA is MA -1 , Left= Rest);append(Left,[_],Rest)),!,
      append(Left,[Value],NewRest),!,MethodCall =.. [F|NewRest].



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








