% ===================================================================
% PURPOSE
% This File is the bootstrap for the Moo Infence engine For SWI-Prolog
% So first is loads the proper files and then starts up the system
% ===================================================================


% ===================================================================
% Prolog Dependant Code
% ===================================================================

/*
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
      prolog_thread_exit/1,
      prolog_thread_self/1,
      prolog_thread_at_exit/1,
      prolog_thread_signal/2,
      prolog_thread_join/2,
      prolog_notrace/1,
      prolog_statistics/0,
      main/1]).
      
*/      

/*
mooBaseJavaClass('logicmoo.SwiMoo').

:-use_module(library(system)).
:-use_module(library(shlib)).
:-use_module(library(listing)).
:-use_module(library(sgml)).
:-use_module(library(rdf)).
:- use_module(library(socket)).
:- use_module(library(readutil)).

%Load the TCP Library
%:- use_module(library(unix)).
:- use_module(library(shell)).
:- use_module(library(shlib)).
:- use_module(library(url)).
:- use_module(library(quintus)).
:- use_module(library(qsave)).

:- use_module((javart)).


    */

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).


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
prolog_thread_exit(Goal):-thread_exit(Goal).
prolog_thread_self(Id):-thread_self(Id).
prolog_thread_at_exit(Goal):-thread_at_exit(Goal).
prolog_thread_signal(ID,Goal):-thread_signal(ID,Goal).
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
writeSTDERR(F,A):-((
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

% :-include('moo_header.pl').

% -------------------------------------------------------------------
% Load the Moo Engine
% -------------------------------------------------------------------
% :-ensure_loaded('moo_bootstrap.pl').

% load files
processBootstrap:-!.
/*
   moduleFile(_,Filename),
   ensure_loaded(Filename),fail.
  */
processBootstrap:-!.


startLogicMoo:-
	 startJava,
	 createJamud,
	 startJamud,
	 loadJamudReferences.


% -------------------------------------------------------------------
% Start the system
% -------------------------------------------------------------------





main(Port):-
   ignore(Port=5001),
   processBootstrap,
   setMooOptionDefaults, %trace,
   startLogicMoo,
   setMooOption(client,html),
   createPrologServer(80),
   createPrologServer(Port),!.
%   callIfPlatformUnix((prologAtInitalization(['mod/mod_nani.pl']),prologAtInitalization(['mod/mod_eliza.pl']),
%         prologAtInitalization(bot),prologAtInitalization(bot2))).
   %throw(wait_now).

% ===========================================================
% SOCKET SERVER - Looks at first charicater of request and decides between:
%  Http, Native or Soap and replies accordingly
% ===========================================================
/*
:-module(moo_httpd,[
   createPrologServer/1,
   xmlPrologServer/1,
   read_line_with_nl/3,
   decodeRequest/2,
   invokePrologCommandRDF/6,
   serviceAcceptedClientSocketAtThread/1]).
*/

% :-include(moo_header).



% :-use_module(moo_threads).
%% :-ensure_loaded(system_dependant).

:-dynamic(isKeepAlive/1).

createPrologServer(Port) :-
        mooProcessCreate(nokill,'Moo XML/SOAP Server Socket',xmlPrologServer(Port),_,[]).

xmlPrologServer(Port):-
        tcp_socket(ServerSocket),
        catch(ignore(tcp_setopt(ServerSocket, reuseaddr)),_,true),
        at_halt(tcp_close_socket(ServerSocket)),
        attemptServerBind(ServerSocket, Port),
        tcp_listen(ServerSocket, 655),
        repeat,
	       acceptClientsAtServerSocket(ServerSocket),
        fail.


attemptServerBind(ServerSocket, Port):-
        catch((tcp_bind(ServerSocket, Port),
        flush_output,
        writeSTDERR('cs.\nMoo server started on port ~w. \n\nYes\n?- ',[Port]),flush_output),
        error(E,_),
        (writeSTDERR('\nWaiting for OS to release port ~w. \n(sleeping 4 secs becasue "~w")\n',[Port,E]),
        sleep(4),
        attemptServerBind(ServerSocket, Port))),!.


acceptClientsAtServerSocket(ServerSocket):-
		tcp_open_socket(ServerSocket, AcceptFd, _),
                cleanOldProcesses,!,
		tcp_accept(AcceptFd, ClientSocket, ip(A4,A3,A2,A1)),!,
                getPrettyDateTime(DateTime),
                sformat(Name,'Dispatcher for ~w.~w.~w.~w  started ~w ',[A4,A3,A2,A1,DateTime]),
                mooProcessCreate(killable,Name,serviceAcceptedClientSocketAtThread(ClientSocket),_,[detatch(true)]),!.

serviceAcceptedClientSocketAtThread(ClientSocket):-
	tcp_open_socket(ClientSocket, In, Out),!,
        setMooOption('$socket_in',In),
        setMooOption('$socket_out',Out),!,
        serviceIO(In,Out),
        flush_output,
	catch(tcp_close_socket(ClientSocket),_,true),
	prolog_thread_exit(complete).



getPrettyDateTime(String):-get_time(Time),convert_time(Time, String).

serviceIO(In,Out):-
        peek_char(In,Char),!,
	%writeSTDERR(serviceIOBasedOnChar(Char,In,Out)),
	serviceIOBasedOnChar(Char,In,Out),!.


serviceIOBasedOnChar('G',In,Out):-!,  
         serviceHttpRequest(In,Out).

serviceIOBasedOnChar('P',In,Out):-!,
         serviceHttpRequest(In,Out).

serviceIOBasedOnChar('<',In,Out):-!,
         serviceSoapRequest(In,Out).  % see moo_soap.pl

serviceIOBasedOnChar(C,In,Out):-
        serviceNativeRequest(C,In,Out).

% ===========================================================
% HTTPD SERVICE
% ===========================================================

serviceHttpRequest(In,Out):-
        readHTTP(In,Options),
        writeFmtFlushed(Out,'HTTP/1.1 200 OK\nServer: Moo-HTTPD\nContent-Type: text/html\n\n',[]),
        setMooOption(client,html),
        tell(Out),
        writeSTDERR('REQUEST: "~q" \n',[Options]), 
	processRequest(Options),
	flush_output.


readHTTP(In,Request):-
        read_line_with_nl(In, Codes, []),
        append("GET /",Stuff,Codes), %true,
        append(RequestCodes,[72,84,84,80|_],Stuff),
        atom_codes(RequestEncoded,RequestCodes),
        decodeRequest(RequestEncoded,Request).

readHTTP(In,Request):-
        read_line_with_nl(In, Codes, []),
        append("POST /",Stuff,Codes), %true,
        append(RequestCodes,[72,84,84,80|_],Stuff),
        atom_codes(RequestEncoded,RequestCodes),
        decodeRequest(RequestEncoded,Request).


read_line_with_nl(Fd, Codes, Tail) :-
        get_code(Fd, C0),
        read_line_with_nl(C0, Fd, Codes, Tail).
read_line_with_nl(end_of_file, _, Tail, Tail) :- !.
read_line_with_nl(-1, _, Tail, Tail) :- !.
read_line_with_nl(10, _, [10|Tail], Tail) :- !.
read_line_with_nl(C, Fd, [C|T], Tail) :-
        get_code(Fd, C2),
        read_line_with_nl(C2, Fd, T, Tail).



decodeRequest(RequestEncoded,[file=Request]):-
      concat_atom([X],'?',RequestEncoded),
      www_form_encode(Request,X),!.
decodeRequest(RequestEncoded,[file=Request|ENCARGS]):-
      concat_atom([X,ARGS],'?',RequestEncoded),
      www_form_encode(Request,X),
      concat_atom(ArgList,'&',ARGS),
      decodeRequestArguments(ArgList,ENCARGS).

decodeRequestArguments([],[]):-!.
decodeRequestArguments([ctx=Value|List],[ctx=CValue,theory=KValue|ARGS]):-
          concat_atom([KValue,CValue],':',Value),!,
          decodeRequestArguments(List,ARGS).
decodeRequestArguments([Arg|List],[DName=DValue|ARGS]):-
          split_nv(Arg,Name,Value),
          www_form_encode(AName,Name),
          www_form_encode(AValue,Value),!,
          decodeRequestAtom(AName,DName),
          decodeRequestAtom(AValue,DValue),
          decodeRequestArguments(List,ARGS).

%ctx=PrologMOO%3ASTRUCTURAL-ONTOLOGY&

split_nv(Arg,Name,Value):-concat_atom([Name,Value],'=',Arg),!.
split_nv(Arg,Arg,Arg).

decodeRequestAtom(A,A):-var(A),!.
decodeRequestAtom(tn,tn):-!.
decodeRequestAtom(N,N):-number(N),!.
decodeRequestAtom(A=B,AA=BB):-
                decodeRequestAtom(A,AA),
                decodeRequestAtom(B,BB),!.
decodeRequestAtom(A,T):-catch(atom_to_term(A,T,_),_,fail),number(T),!.
decodeRequestAtom(A,T):-catch(atom_to_term(A,T,_),_,fail),not(var(T)),not(compound(T)),!.
decodeRequestAtom(A,T):-atom(A),catch(atom_codes(A,[95|_]),_,fail),catch(atom_to_term(A,T,_),_,fail),!.
decodeRequestAtom(A,A):-!.

% ===========================================================
% NATIVE SERVICE
% ===========================================================

serviceNativeRequest(_,In,Out):-
        writeFmt(Out,'<?xml version="1.0" encoding="ISO-8859-1"?>\n',[]),
        getThread(Session),
        retractall(isKeepAlive(Session)),
        xmlClearTags,
        repeat,
                catch(
                        read_term(In,PrologGoal,[variable_names(ToplevelVars),character_escapes(true),syntax_errors(error)]),
                        E,
                        writeErrMsg(Out,E)),
                %writeSTDERR(PrologGoal:ToplevelVars),
                invokePrologCommandRDF(Session,In,Out,PrologGoal,ToplevelVars,Returns),
                notKeepAlive(Out,Session),!.

notKeepAlive(Out,Session):-isKeepAlive(Session),
        write(Out,
                'complete.\n'
                %'<prolog:keepalive/>\n'
                                ),catch(flush_output(Out),_,true),!,fail.
notKeepAlive(Out,Session):-catch(flush_output(Out),_,true).


keep_alive:-getThread(Me),retractall(isKeepAlive(Me)),assert(isKeepAlive(Me)),writeFmtFlushed('<keepalive/>\n',[]).
goodbye:-getThread(Me),retractall(isKeepAlive(Me)),writeFmt('<bye/>/n',[]).


invokePrologCommandRDF(Session,In,Out,PrologGoal,ToplevelVars,Returns):-var(PrologGoal),!.

invokePrologCommandRDF(Session,In,Out,PrologGoal,ToplevelVars,Returns):-
        term_to_atom(Session,Atom),concat_atom(['$answers_for_session',Atom],AnswersFlag),
        writeFmt(Out,'<prolog:solutions goal="~q">\n',[PrologGoal]),
        flag(AnswersFlag,_,0),
        set_output(Out),set_input(In),!,
        getCputime(Start),
        callNondeterministicPrologCommand(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars),
        xmlExitTags,
        getCputime(End),
        flag(AnswersFlag,Returns,Returns),
%       (Returns > 0 ->
%               writeFmt(Out,'<prolog:yes/>\n',[]) ;
%               writeFmt(Out,'<prolog:no/>\n',[])),!,
        Elapsed is End -Start,
        writeFmt(Out,'</prolog:solutions answers="~w" cputime="~g">\n',[Returns,Elapsed]),!.

callNondeterministicPrologCommand(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars):-
        ground(PrologGoal),!,
        catch(
                (PrologGoal,
                 flag(AnswersFlag,Answers,Answers+1),
                 writePrologToplevelVarsXML(Out,PrologGoal,AnswersFlag,ToplevelVars)
                 ),
           Err,writeErrMsg(Out,Err,PrologGoal)),!.





callNondeterministicPrologCommand(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars):-
        catch(
                (PrologGoal,
                 flag(AnswersFlag,Answers,Answers+1),
                 writePrologToplevelVarsXML(Out,PrologGoal,AnswersFlag,ToplevelVars),
                 fail),
           Err,writeErrMsg(Out,Err,PrologGoal)),!.
callNondeterministicPrologCommand(Session,AnswersFlag,In,Out,PrologGoal,ToplevelVars):-!.


writePrologToplevelVarsXML(Out,PrologGoal,AnswersFlag,ToplevelVars):-
         flag(AnswersFlag,Answers,Answers),
        writeFmt(Out,'<prolog:result solution="~w">\n',[Answers]),
        writePrologToplevelVarsXML2(Out,ToplevelVars),
        writeFmt(Out,'</prolog:result>\n',[]),!.

writePrologToplevelVarsXML2(Out,[]):-!.
writePrologToplevelVarsXML2(Out,[Term|REST]):-!,Term=..[_,N,V],
         writeFmtFlushed(Out,'       <prolog:p>~w = ~q</prolog:p>\n',[N,V]),
         writePrologToplevelVarsXML2(Out,REST),!.


writeFmt(A,B,C):-!.
writeFmt(A,B):-!.

writeFmt(A,B,C):-
        writeFmtFlushed(A,B,C).
writeFmt(A,B):-
        writeFmtFlushed(A,B).


throwMoo(Module,Type,Details):-
        current_prolog_flag(debug_on_error, DebugOnError),
        set_prolog_flag(debug_on_error, false),!,
        throw(mooException(Module,Type,Details,DebugOnError)),
        ifInteractive(writeDebug('Post throwMoo')),!.


% ===========================================================
% THREAD SERVICE
% ===========================================================

% imports these models from SWI-Prolog
% prolog_thread_create(Goal,Id,Options)
% prolog_current_thread(Id,Status)
% prolog_thread_at_exit(Goal)
% prolog_thread_self(Id)
% prolog_thread_at_exit(Id,Goal)
% prolog_thread_join(Id,_)

/*
:-module(moo_threads,
      [ getThread/1,
	 mooProcessCreate/1,
	 mooProcessCreate/3,
	 mooProcessCreate/4,
	 mooProcessCreate/5,
	 isMooProcess/2,
	 isMooProcess/5,
	 createProcessedGoal/1,
	 mooProcessSelfClean/0,
	 showMooStatisticsHTML/0,
	 cleanOldProcesses/0,
	 showMooProcessHTML/0]).
  */
% :-include('moo_header.pl').

:-dynamic(isMooProcess/5).


getThread(Id):-
	prolog_thread_self(Id).


createProcessedGoal(Goal):-
      mooProcessCreate((prolog_thread_at_exit((
	 (getThread(Id),prolog_thread_exit(i_am_done(Id))))),Goal),Id,[]).


mooProcessCreate(Perms,Name,Goal,Id,Options):-
        prolog_thread_create((prolog_thread_at_exit(mooProcessSelfClean),Goal),Id,Options),
        asserta(isMooProcess(Perms,Name,Goal,Id,Options)).

mooProcessCreate(Name,Goal,Id,Options):-
        prolog_thread_create((prolog_thread_at_exit(mooProcessSelfClean),Goal),Id,Options),
        asserta(isMooProcess(killable,Name,Goal,Id,Options)).

mooProcessCreate(Goal,Id,Options):-
        prolog_thread_create((prolog_thread_at_exit(mooProcessSelfClean),Goal),Id,Options),
        asserta(isMooProcess(killable,thread(Id),Goal,Id,Options)).

mooProcessCreate(Goal):-
        mooProcessCreate(Goal,_Id,[detach(true)]).

isMooProcess(ID,Goal):-
        isMooProcess(_,_,Goal,ID,_).

debugProcess(T):-
	prolog_thread_signal(T, (attach_console, true)).


mooProcessSelfClean:-
      trace, 
      getThread(Id),
      retractall(isMooProcess(_Perms,_Name,_Goal,Id,_Options)).




showMooStatisticsHTML:-
   writeFmt('<pre>'),prolog_statistics,writeFmt('</pre>').

showMooProcessHTML:-
        showMooStatisticsHTML,
        writeFmt('<hr><table border=1 width=80%><th>Id</th><th>Name</th><th>Status</th><th>Actions</th><th>Options</th><th>Goals</th>',[]),
        prolog_current_thread(Id,Status),
        isMooProcess(Perms,Name,Goal,Id,Options),
        writeMooProcessesHTML(Perms,Name,Goal,Id,Options,Status),
        fail.
showMooProcessHTML:-
        writeFmt('</table>',[]).


writeMooProcessesHTML(nokill,Name,Goal,Id,Options,Status):-
        writeFmt('<tr><td>~w</td><td><nobr>~w</td><td>~w</td><td>&nbsp;</a></td><td>~w</td><td>~w</td><tr>\n ',[Id,Name,Status,Options,Goal]),!.

writeMooProcessesHTML(Perms,Name,Goal,Id,Options,Status):-
        writeFmt('<tr><td>~w</td><td><nobr>~w</td><td>~w</td><td><A href="controlpanel.jsp?killable=~w">Kill</a></td><td>~w</td><td>~w</td><tr>\n ',[Id,Name,Status,Id,Options,Goal]),!.

cleanOldProcesses:-
        saveUserInput,
        prolog_current_thread(Id,Status),
        handleProcessStatus(Id,Status),fail.
cleanOldProcesses:-writeSavedPrompt,!.
cleanOldProcesses:-!.

handleProcessStatus(Id,running):-!. %Normal
handleProcessStatus(Id,exited(complete)):-!,prolog_thread_join(Id,_),!.
handleProcessStatus(Id,true):-!, writeSTDERR('% Process ~w complete.\n',[Id]),!,prolog_thread_join(Id,_),!.
handleProcessStatus(Id,exception(Error)):-!, writeSTDERR('% Process ~w exited with exceptions: ~q \n',[Id,Error]),!,prolog_thread_join(Id,_),!.
handleProcessStatus(Id,O):-!, writeSTDERR('% Process ~w exited "~q". \n',[Id,O]),!,prolog_thread_join(Id,_),!.


mutex_call(Goal,Id):-
                        mutex_create(Id),
                        mutex_lock(Id),!,
                        with_mutex(Id,Goal),!,
                        mutex_unlock_all.


/*
:-module(moo_globalisms,[
	 setMooOption/1,
	 setMooOption/2,
	 getMooOption/1,
	 getMooOption/2,
	 ensureMooOption/2,
	 ensureMooOption/3,
	 setMooOptionDefaults/0]).
*/


% :-ensure_loaded(moo_threads).

% Database
:-dynamic(mooCache/1).
:-dynamic(mooCache/2).
:-dynamic(mooCache/3).
:-dynamic(mooCache/4).
:-dynamic(mooCache/5).
:-dynamic(mooCache/6).
:-dynamic(mooCache/7).
:-dynamic(mooCache/8).
:-dynamic(mooCache/9).

% User Agent
:-dynamic('$MooOption'/3).
:-dynamic(saved_note/4).
:-dynamic(act_mem/3).

% Database
:-dynamic(tq_attempted_request/0).
:-dynamic(title/1).

% TQ System
:-dynamic(tq_missed_one_answer/0).
:-dynamic(tq_least_one_answer/0).
:-dynamic(t_answer_found/1).


% ===================================================================
% OPERATION PREDICATES
% ===================================================================
% Defaults
:-dynamic(getDefaultKB/1).
:-dynamic(get_default_request_context/1).
:-dynamic(get_default_assertion_context/1).
:-dynamic(version_tag/1).

:-dynamic(answer_found/1).
:-dynamic(moo_K_scenario/2).
:-dynamic(telling_prolog/0).  % If set asserts clauses into prolog database
:-dynamic(telling_file/0).   % If set write assertions to file
:-dynamic(disp_debug/5).         %PREDICATE RESOLUTON
:-dynamic(contexts/2).            %CONTEXT STATES
:-dynamic(setting_table/2).
:-dynamic(tabling/1).
:-dynamic(tabled_t/1).
:-dynamic(tabled_f/1).
:-dynamic(answer_yes/0).
:-dynamic(already_asked/2).
:-dynamic(save_all/2).
:-dynamic(moo_K_scenario/6).         %We keep a cache of forall consultations
:-dynamic(consultation_mode_on/0).
:-dynamic(resource_cache/2).
:-dynamic(debuggerize/0).

:-dynamic( le_ele/1).


:-dynamic('surface-word'/2).
:-dynamic('surface-macro'/2).
:-dynamic('browser-only'/1).
:-dynamic('not-implemented'/1).
:-dynamic('surface-atom'/1).
:-dynamic('surface-single-arity'/1).
:-dynamic('surface-multiple-arity'/1).
:-dynamic('surface-instance'/2).
:-dynamic('surface-subclass'/2).
:-dynamic('surface-class'/1).
:-dynamic('surface-quantifier'/1).



indexPredicate(positive_fact_cache(1,1,1,1)).
indexPredicate(positive_rule_cache(1,1,1,1)).
indexPredicate(negative_fact_cache(1,1,1,1)).
indexPredicate(negative_rule_cache(1,1,1,1)).

:-dynamic(have_arity/4).

% these hold prototypes of calls
:-dynamic(positive_fact_cache/4).
:-dynamic(positive_rule_cache/4).
:-dynamic(negative_fact_cache/4).
:-dynamic(negative_rule_cache/4).

:-dynamic(make_positive_cache/2).


:-dynamic('in-active-memory'/2).

:-dynamic('should_be_loaded'/2).



setMooOption([]):-!.
setMooOption([H|T]):-!,
      setMooOption(H),!,
      setMooOption(T),!.
setMooOption(Var=_):-var(Var),!.
setMooOption(_=Var):-var(Var),!.
setMooOption((N=V)):-nonvar(N),!,setMooOption_thread(N,V),!.
setMooOption(N):-atomic(N),!,setMooOption_thread(N,true).
	
setMooOption(Name,Value):-setMooOption_thread(Name,Value).
setMooOption_thread(Name,Value):-
	((getThread(Process),
	retractall('$MooOption'(Process,Name,_)),
	asserta('$MooOption'(Process,Name,Value)),!)).


unsetMooOption(Name=Value):-nonvar(Name),
	unsetMooOption_thread(Name,Value).
unsetMooOption(Name):-nonvar(Name),
	unsetMooOption_thread(Name,_).
unsetMooOption(Name):-(retractall('$MooOption'(_Process,Name,_Value))).


unsetMooOption_thread(Name):-
	unsetMooOption_thread(Name,_Value).

unsetMooOption_thread(Name,Value):-
	getThread(Process),
	retractall('$MooOption'(Process,Name,Value)).
	
getMooOption_nearest_thread(Name,Value):-
	getMooOption_thread(Name,Value),!.
getMooOption_nearest_thread(Name,Value):-
	'$MooOption'(_,Name,Value),!.
getMooOption_nearest_thread(_Name,_Value):-!.



isMooOption(Name=Value):-!,isMooOption(Name,Value).
isMooOption(Name):-!,isMooOption(Name,true).

isMooOption(Name,Value):-getMooOption_thread(Name,Value).

getMooOption_thread(Name,Value):-
	((getThread(Process),
	'$MooOption'(Process,Name,Value))),!.


getMooOption(Name=Value):-nonvar(Name),!,ensureMooOption(Name,_,Value).
getMooOption(Name=Default,Value):-nonvar(Name),!,ensureMooOption(Name,Default,Value).
getMooOption(Name,Value):-nonvar(Name),!,ensureMooOption(Name,_,Value).


ensureMooOption(Name=Default,Value):-
	ensureMooOption(Name,Default,Value),!.
	
ensureMooOption(Name,_Default,Value):-
	getMooOption_thread(Name,Value),!.

ensureMooOption(Name,Default,Default):-
	setMooOption_thread(Name,Default),!.

ensureMooOption(Name,_Default,Value):-nonvar(Name),!,   
	setMooOption_thread(Name,Value),!.

ensureMooOption(_Name,Default,Default).



setMooOptionDefaults:-
             (unsetMooOption(_)),
             setMooOption(opt_callback='sendNote'),
             setMooOption(cb_consultation='off'),
             setMooOption(opt_debug='off'),
             setMooOption(cb_error='off'),
             setMooOption(cb_result_each='off'),

% User Agent Defaults for Blank Variables
             setMooOption(opt_cxt_request='GlobalContext'),
             setMooOption(opt_ctx_assert='GlobalContext'),
             setMooOption(opt_tracking_number='generate'),
             setMooOption(opt_agent='ua_parse'),
             setMooOption(opt_precompiled='off'),
             getMooOption(opt_theory,Context),setMooOption(opt_theory=Context),
             setMooOption(opt_notation='kif'),
             setMooOption(opt_timeout=2),
             setMooOption(opt_readonly='off'),
             setMooOption(opt_debug='off'),
             setMooOption(opt_compiler='Byrd'),
             setMooOption(opt_language = 'pnx_nf'),

%Request Limits
             setMooOption(opt_answers_min=1),
             setMooOption(opt_answers_max=999), %TODO Default
             setMooOption(opt_backchains_max=5),
             setMooOption(opt_deductions_max=100),
             setMooOption(opt_backchains_max_neg=5),
             setMooOption(opt_deductions_max_neg=20),
             setMooOption(opt_forwardchains_max=1000),
             setMooOption(opt_max_breath=1000), %TODO Default

%Request Contexts
             setMooOption(opt_explore_related_contexts='off'),
             setMooOption(opt_save_justifications='off'),
             setMooOption(opt_deductions_assert='on'),
             setMooOption(opt_truth_maintence='on'),
             setMooOption(opt_forward_assertions='on'),
             setMooOption(opt_deduce_domains='on'),
             setMooOption(opt_notice_not_say=off),


%Request Pobibility
             setMooOption(opt_certainty_max=1),
             setMooOption(opt_certainty_min=1),
             setMooOption(opt_certainty=1),
             setMooOption(opt_resource_commit='on').




% ===================================================================
% File 'moo_markup.pl'
% Maintainers: Douglas Miles
% Contact: dmiles@users.sourceforge.net ;
% Version: 'moo_markup.pl' 1.0.0
% Revised At:  $Date: 2002-11-16 04:41:08 $

% ===================================================================
% Major functions:
% This file meets the needs of an external agent working for the needs of eigther an automated or human user
% Interface with Java with XML to display explaination trees and variable bindings
% ===================================================================

% :-include('moo_header.pl').
:-index(getMarkupFormula(1,1,1,1)).
:-index(toMarkUp_lang(1,1,1,1)).

%Tests

%stest3 :- toMarkUp(html, explaination(('Military':996:subclass('IntransitiveRelation', 'BinaryRelation')^B)* ('Military':836:subclass('BinaryRelation', 'Relation')^C)*forall('IntransitiveRelation', forall(D, forall('Relation', holds(subclass, 'IntransitiveRelation', D)and holds(subclass, D, 'Relation')=>holds(subclass, 'IntransitiveRelation', 'Relation'))))*sfind(instance(subclass, 'PartialOrderingRelation'))*sfind(subclass('PartialOrderingRelation', 'TransitiveRelation'))* ('Military':2756:instance(on, 'IntransitiveRelation')^E)), ['X'=on|A],O),write_ln(O).

% ================================================================
%   Transform Signals to Objects
% ================================================================

% ===================================================================
% writeMarkup(-Prolog)
%
% Replaces writeq in some cases
% ===================================================================
writeMarkup(Term):-term_to_leml(Term,Chars),write(Chars).


% ===================================================================
% toMarkUp_lang(-Markup,-Prolog,-PrologVarableList, +Output)
%
% Markup := [html,kif,pml,leml] (Expandable)
% Prolog := any prolog term
% PrologVaraibles list is the equal list as produced by read/3  [=(Name,Val)|...]
% Output is an CharicterAtom (the difference is this atom is not added the the symbol table)
% ===================================================================
% ===================================================================
% term_to_leml(-Prolog, +Output)
%
% arity 2 version (note html) is sufficient for printing values
% ===================================================================
term_to_leml(Term,Chars):-toMarkUp(html,Term,_,Chars),!.


toMarkUp(chat,Var,VS,Chars):-!,catch(toMarkUp(kif,Var,VS,Chars),_,true),!.
toMarkUp(java,Var,VS,Chars):-!,catch(toMarkUp(html,Var,VS,Chars),_,true),!.

toMarkUp(L,T,V,Chars):-!,
        ignore(catch(/**/((
        copy_term((T,V),(CT,CV)),
        numbervars((CT,CV),'$VAR',0,_),%true,
        toMarkUp_lang(L,CT,CV,Chars))),_,true)),!.

% VARIABLES
toMarkUp_lang(L,C,Vars,Out):-isSlot(C),!,toMarkUp_slotValue(L,C,Vars,Out).

toMarkUp_lang(html,'$spacer',Vars,'\n<hr>\n').
toMarkUp_lang(_,'$spacer',Vars,'\n;; ------------------------------------------------------------------------------\n\n').

tml(Form):-toMarkUp_lang(html,formula(Form),Vars,Out),write(Out),nl.

toMarkUp_lang(L,formula(C),Vars,Out):-!,
        getMarkupFormula(L,C,Vars,Out).

% ===================================================
% Pretty Print Formula
% ===================================================
%getMarkupFormula(L,C,Vars,Out):-       writeq( C=Vars),nl,fail.


getMarkupFormula(L,C,Vars,Out):-isSlot(C),!,toMarkUp_lang(L,C,Vars,Out).
getMarkupFormula(L,C,Vars,Out):-not(compound(C)),!,toMarkUp_lang(L,C,Vars,Out).

% QUOTED STRING FORMAT
getMarkupFormula(L,Atom,_VS,Chars):-((isCharCodelist(Atom);string(Atom))),!,
        catch(sformat(Chars,'"~s"',[Atom]),_,sformat(Chars,'"~w"',[Atom])).

getMarkupFormula(L,string(C),Vars,C):-!.

getMarkupFormula(L,hidden(F,Args),Vars,''):-!.

getMarkupFormula(html,colourize(Color,Thing),Vars,Chars):-!,
        getMarkupFormula(html,Thing,Vars,Chars1),!,
        sformat(Chars,'<font color="~w">~w</font>\n',[Color,Chars1]).

getMarkupFormula(L,colourize(Color,Thing),Vars,Chars):-!,
        getMarkupFormula(L,Thing,Vars,Chars),!.

/*
getMarkupFormula(L,','(A,B),Vars,Chars):-!,
        prolog_to_krlog(','(A,B),KR),
        getMarkupFormula(L,KR,Vars,Chars),!.
*/


getMarkupFormula(L,write_dollar('$v',[A|Args]),Vars,Chars):-!,
                Flag=..[getPrologVars,A|Args],!,
                getMarkupFormula(L,Flag,Vars,Chars).

getMarkupFormula(L,table_(Goal,Lits),Vars,Chars):-!,
                getMarkupFormula(L,table_p(Lits,Goal),Vars,Chars).


getMarkupFormula(L,write_dollar(F,[A|Args]),Vars,Chars):-!,
        getMarkupFormula(L,A,Vars,Chars1),
        getMarkupFormula(L,hidden(F,Args),Vars,Chars2),!,
        sformat(Chars,'~w~w',[Chars1,Chars2]).

getMarkupFormula(L,'$existential'(VarName,Name,Literal),Vars,O):-!,
        getMarkupFormula(L,'existential'(VarName),Vars,O).

getMarkupFormula(L,'$eval'(Function),Vars,O):-!,
        getMarkupFormula(L,' eval'(Function),Vars,O).


getMarkupFormula(L,functional(VarName,Domains,Literal),Vars,O):-
        toMarkUp_lang(L,Literal,Vars,O),!.

close_list_var(M,[]):-isSlot(M),!.
close_list_var([[M]|Ms],[M|Ls]):-!,
        close_list_var(Ms,Ls).
close_list_var([M|Ms],[M|Ls]):-!,
        close_list_var(Ms,Ls).

getMarkupFormula(L,Term,Vars,Chars):-
        Term=..[F,A|Args],
        atom_concat('$',_,F), !,
        getMarkupFormula(L,write_dollar(F,[A|Args]),Vars,Chars).


getMarkupFormula(L,unused(C,P),Vars,O):-!,
        getMarkupFormula(L,notused(C,writeq(P)),Vars,O).

getMarkupFormula(L,ff([]),Vars,'[]'):-!.

getMarkupFormula(L,ff([Flag|Flags]),Vars,Chars):-!,
        getMarkupFormula(L,flag(Flag),Vars,Chars1),
        getMarkupFormula(L,ff(Flags),Vars,Chars2),
        sformat(Chars,'~w, ~w',[Chars1, Chars2]).

getMarkupFormula(L,domargs([]),Vars,''):-!.

getMarkupFormula(L,domargs([(P:N)]),Vars,Chars):-!,
        getMarkupFormula(L,P,Vars,Chars1),
        sformat(Chars,'~w:~w',[Chars1,N]).

getMarkupFormula(L,domargs([(P:N)|Flags]),Vars,Chars):-!,
        getMarkupFormula(L,P,Vars,Chars1),
        getMarkupFormula(L,domargs(Flags),Vars,Chars2),
        sformat(Chars,'~s:~w,~w',[Chars1,N,Chars2]).

getMarkupFormula(L,flag(Flag),Vars,Chars):-
        Flag=..[domainV,Var,DomArgs],!,
        getMarkupFormula(L,Var,Vars,VarChars),
        getMarkupFormula(L,domargs(DomArgs),Vars,ArgChars),
        sformat(Chars,'~w(~w,[~w])',[domainV,VarChars,ArgChars]).

getMarkupFormula(L,flag(Flag),Vars,Chars):-
        Flag=..[Name,Var,Args],!,
        getMarkupFormula(L,Var,Vars,VarChars),
        sformat(Chars,'~w(~w, ~q)',[Name,VarChars,Args]).
getMarkupFormula(L,flag(Flag),Vars,Chars):-!,
        getMarkupFormula(L,writeq(Flag),Vars,Chars).







getMarkupFormula(L,writeq(Atom),_VS,Chars):-!,sformat(Chars,'~q',[Atom]).

getMarkupFormula(L,[],Vars,''):-!.
%getMarkupFormula(L,[A | B],Vars,Chars):-proper_list([A | B]),append(['('|[A | B],[')'],TRY),toMarkUp_list(L,[Su|Bj],Vars,Chars).
%getMarkupFormula(L,[A | B],Vars,Chars):-catch(TRY=..['',A | B],_,fail),getMarkupFormula(L,TRY,Varsr,Chars),!.
%getMarkupFormula(L,[A | B],Vars,Chars):-catch(TRY=..[A | B],_,fail),getMarkupFormula(L,TRY,Vars,Chars),!.
%getMarkupFormula(L,[A | B],Vars,Chars):-catch(TRY=..[A | B],_,fail),getMarkupFormula(L,TRY,Vars,Chars),!.
getMarkupFormula(L,[Su|Bj],Vars,Chars):-
        toMarkUp_list(L,[Su|Bj],Vars,Chars1),
        sformat(Chars,'(~w)',[Chars1]).

/*
getMarkupFormula(L,Term,Vars,O):-
        Term=..[holds,F|Args],isNonVar(F),not_a_function(F),!,
        NTerm=..[F|Args],
        getMarkupFormula(L,NTerm,Vars,O).
*/
getMarkupFormula(L,'$VAR'(_)* X ,Vars,Out):-!,getMarkupFormula(L, X ,Vars,Out).
getMarkupFormula(L, X * '$VAR'(_) ,Vars,Out):-!,getMarkupFormula(L, X ,Vars,Out).
getMarkupFormula(L,(A * []),Vars,Out):-!,getMarkupFormula(L,A ,Vars,Out).
getMarkupFormula(L,([] * A),Vars,Out):-!,getMarkupFormula(L,A ,Vars,Out).
getMarkupFormula(L,deduced* X ,Vars,Out):-!,getMarkupFormula(L, X ,Vars,Out).
getMarkupFormula(L, X * deduced ,Vars,Out):-!,getMarkupFormula(L, X ,Vars,Out).


getMarkupFormula(L,domainV(Var,ReqsL),Vars,Chars):-
        getMarkupFormula(L,' domainV'(Var,writeq(ReqsL)),Vars,Chars).
getMarkupFormula(L,domainC(Var,ReqsL),Vars,Chars):-
        getMarkupFormula(L,' domainC'(Var,writeq(ReqsL)),Vars,Chars).
getMarkupFormula(L,domainA(Var,ReqsL),Vars,Chars):-
        getMarkupFormula(L,' domainA'(Var,writeq(ReqsL)),Vars,Chars).
getMarkupFormula(L,existsC(Var,ReqsL),Vars,Chars):-
        getMarkupFormula(L,' existsC'(Var,writeq(ReqsL)),Vars,Chars).
getMarkupFormula(L,existsA(Var,ReqsL),Vars,Chars):-
        getMarkupFormula(L,' existsA'(Var,writeq(ReqsL)),Vars,Chars).

getMarkupFormula(L,(A * B),Vars,Chars):-!,
        getMarkupFormula(L,B,Vars,Chars2),
        getMarkupFormula(L,A,Vars,Chars1),
        sformat(Chars,'~w\n~w',[Chars2, Chars1]).

getMarkupFormula(L,formula(C),Vars,Out):-!,
        getMarkupFormula(L,C,Vars,Out).


getMarkupFormula(html,undefined_constants(UnDefinedList),_,O):-
        getMarkupFormula(kif,nv(UnDefinedList),_,I),
        sformat(O,'\n<font color=red>Warning Undefined constants: <font color=black size=+1>~w</font></font>',[I]).

getMarkupFormula(kif,undefined_constants(UnDefinedList),_,O):-
        getMarkupFormula(kif,(UnDefinedList),_,I),
        sformat(O,'\Warning Undefined constants ~w',[I]).



getMarkupFormula(L,C,Vars,Out):-is_list(C),!,make_args_out(L,C,Vars,Out1),sformat(Out,'(~w)',[Out1]).
%getMarkupFormula(L,C,Vars,Out):-not(compound(C)),!,toMarkUp_lang(L,C,Vars,Out).

/*
getMarkupFormula(L,and(A,B),VS,Chars):-
        collect_op(and(A,B),O),!,
        getMarkupFormula(L,O,VS,Chars).

collect_op(and(A,B),and(A,B)):-not(A=and(_,_)),not(B=and(_,_)).
collect_op(and(A,B
*/

% ==================================================
% Unest And/Or
% ==================================================

getMarkupFormula(L,and(and(and(and(and(F,E),D),C),B),A),VS,Chars):-!, getMarkupFormula(L,and(F,E,D,C,B,A),VS,Chars).
getMarkupFormula(L,and(and(and(and(E,D),C),B),A),VS,Chars):-!, getMarkupFormula(L,and(E,D,C,B,A),VS,Chars).
getMarkupFormula(L,and(and(and(D,C),B),A),VS,Chars):-!, getMarkupFormula(L,and(D,C,B,A),VS,Chars).
getMarkupFormula(L,and(and(B,C),A),VS,Chars):-!, getMarkupFormula(L,and(C,B,A),VS,Chars).
getMarkupFormula(L,and(A,and(B,and(C,and(D,and(E,F))))),VS,Chars):-!, getMarkupFormula(L,'and'(A,B,C,D,E,F),VS,Chars).
getMarkupFormula(L,and(A,and(B,and(C,and(D,E)))),VS,Chars):-!, getMarkupFormula(L,'and'(A,B,C,D,E),VS,Chars).
getMarkupFormula(L,and(A,and(B,and(C,D))),VS,Chars):-!, getMarkupFormula(L,'and'(A,B,C,D),VS,Chars).
getMarkupFormula(L,and(A,and(B,C)),VS,Chars):-!, getMarkupFormula(L,'and'(A,B,C),VS,Chars).
getMarkupFormula(L,or(or(or(or(D,E),D),B),A),VS,Chars):-!, getMarkupFormula(L,or(E,D,C,B,A),VS,Chars).
getMarkupFormula(L,or(or(or(C,D),B),A),VS,Chars):-!, getMarkupFormula(L,or(D,C,B,A),VS,Chars).
getMarkupFormula(L,or(or(B,C),A),VS,Chars):-!, getMarkupFormula(L,or(C,B,A),VS,Chars).
getMarkupFormula(L,or(A,or(B,or(C,or(D,E)))),VS,Chars):-!, getMarkupFormula(L,'or'(A,B,C,D,E),VS,Chars).
getMarkupFormula(L,or(A,or(B,or(C,D))),VS,Chars):-!, getMarkupFormula(L,'or'(A,B,C,D),VS,Chars).
getMarkupFormula(L,or(A,or(B,C)),VS,Chars):-!, getMarkupFormula(L,'or'(A,B,C),VS,Chars).

% ==================================================
% Mark terms as implemented in code
% ==================================================

getMarkupFormula(html,incode(X),Vars,HAtom):-!,
        getMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'<table border=0><tr><td><pre>~w</pre></td><td><pre>Implemented in code.</pre></td></tr></table>',[Atom]).

getMarkupFormula(kif,incode(X),Vars,HAtom):-!,
        getMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'~w\nImplemented in code.\n',[Atom]).


getMarkupFormula(html,incode(X,M),Vars,HAtom):-!,
        getMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'<table border=0><tr><td><pre>~w</pre></td><td><pre>Implemented in code.\n~w</pre></td></tr></table>',[Atom,M]).

getMarkupFormula(kif,incode(X,M),Vars,HAtom):-!,
        getMarkupFormula(L,bullet(X),Vars,Atom),
        sformat(HAtom,'~w\nImplemented in code.\n (~w)\n',[Atom,M]).

% ==================================================
% Finds the clausification then displays the explaination
% ==================================================

getMarkupFormula(L,cfind(entails(Pre,Post)),Vars,Out):-
        mooCache(PredR,Post,Pre,T,true,Context,Explaination),
        getMarkupFormula(L,Explaination,Vars,Out),!.

% ==================================================
% Show explaination of cross reference optimization
% ==================================================
getMarkupFormula(L,g_h(_),Vars,''):-!.
getMarkupFormula(L,tid(_),Vars,''):-!.

getMarkupFormula(L,crossref(X,Y),Vars,Atom):-!,
        crossref_to_explaination(crossref(X,Y),P),
        getMarkupFormula(L,P,Vars,Atom).

getMarkupFormula(L,crossref(X),Vars,Atom):-!,
        crossref_to_explaination(crossref(X),P),
        getMarkupFormula(L,P,Vars,Atom).


% ==================================================
% Surface Find
% ==================================================

getMarkupFormula(L,sfind(X),Vars,Out):- nonvar(X),
        mooCache(PredR, surface, X,V,Context, Ctx, TN, Auth, State),!,
        var_merge(Vars,V,TVars),!,
        getMarkupFormula(L,surf(Context,TN),TVars,Out).

% ==================================================
% Find a surface form, Display its explaination, show instanced version
% ==================================================

getMarkupFormula(L,sfindi(X),Vars,Out):- nonvar(X),
        mooCache(PredR, surface, X,V,Context, Ctx, TN, Auth, State),!,
        var_merge(Vars,V,TVars),!,
        getMarkupFormula(L,surf(Context,TN) * bullet_a(X),TVars,Out).

getMarkupFormula(L,sfindi(X),Vars,Out):- nonvar(X),
        getMarkupFormula(L,bullet_a(X),Vars,Out).


% ==================================================
% VIA
% ==================================================

getMarkupFormula(L,via(Form,V),Vars,Out):-
        (var_merge(Vars,V,TVars)),
        getMarkupFormula(L,via(Form),TVars,Out),!.


getMarkupFormula(L,via(entails(Pre,(Post))),Vars,Out):-not(isMooOption(show_entails,true)),!,
        getMarkupFormula(L,(  via('=>'(Pre,Post)) ),Vars,Out).


getMarkupFormula(L,'-'(Form),Vars,Out):-
        getMarkupFormula(L,not(Form),Vars,Out).

getMarkupFormula(L,'+'(Form),Vars,Out):-
        getMarkupFormula(L,(Form),Vars,Out).

getMarkupFormula(L,via(Form),Vars,Out):-
        getMarkupFormula(L,bullet_a(Form),Vars,Out).


getMarkupFormula(L,(entails(CList,UConsq,false)),Vars,Out):-!,
        getMarkupFormula(L,entails(CList,not(UConsq)),Vars,Out).

getMarkupFormula(L,(entails(CList,UConsq,true)),Vars,Out):-!,
        getMarkupFormula(L,entails(CList,(UConsq)),Vars,Out).

getMarkupFormula(L,(entails(true,(Post))),Vars,Out):-!,
        getMarkupFormula(L,(Post),Vars,Out).

% ==================================================
% nv(_) Print list as non-vecorted
% ==================================================

getMarkupFormula(L,nv(Subj),Vars,Chars):-!,toMarkUp_list(L,Subj,Vars,Chars).

% ==========================
% Maintainerial writing
% ==========================

getMarkupFormula(L,surf(Context,TN),Vars,Atom):-
        mooCache(PredR,surface, OForm, OVars,Context,TN,_, _),!,
        getMarkupFormula(L,OForm,OVars,Orig),
        flag(explaination_linenumber,LN,LN+1),
        getMarkupFormula(L,bullet(Context,TN,LN,Orig),Vars,Atom).
getMarkupFormula(L,surf(Context,TN),Vars,Atom):-!,
        getMarkupFormula(L,bullet('assertion lookup failure'(Context,TN)),Vars,Atom).

% ==========================
% Bullet writing
% ==========================

getMarkupFormula(L,bullet_a(X),Vars,S):-
        flag(indent,_,0),
        getMarkupFormula(L,X,Vars,SStatement),
        flag(explaination_linenumber,LN,LN),
        LNB is LN-1,
        sformat(S,'~wa. ~w\n',[LNB,SStatement]).

getMarkupFormula(L,bullet(X),Vars,Atom):-!,
        flag(explaination_linenumber,LN,LN+1),
        getMarkupFormula(L,X,Vars,Orig),
        getMarkupFormula(L,bullet('Kernel','GlobalContext',9100000,LN,Orig),Vars,Atom).

getMarkupFormula(html,bullet(Context,TN,LN,Orig),Vars,Atom):-!,%true,
        flag(indent,_,0),
        (catch((TN < 100000),_,fail) ->
                sformat(Atom,'~w <A href="stheory.jsp?req=SA&stheory=~w&id=~w" title="~w ~w ~w" ><img border=0 src="bullet.gif"/></A> ~w',[LN,Context,TN,TN,Context,Orig]);
                sformat(Atom,'~w <img border=0 src="bullet.gif" title="Not added to browser ~w (~w)"> ~w',[LN,Context,Orig])),!.

getMarkupFormula(kif,bullet(Context,TN,LN,Orig),Vars,Atom):-!,
        flag(indent,_,0),
%       getMarkupFormula(kif,asserted(Ctx,Orig),Vars,F),
        getMarkupFormula(kif,Orig,Vars,F),
        sformat(Atom,'~w. ~w',[LN,F]).

% ==========================
% Slolem  rewriting
% ==========================

getMarkupFormula(L,(X),Vars,Out):- nonvar(X),X=..['E',Sk|ArgS],!,
        Y=..[Sk|ArgS],!,
        getMarkupFormula(L,Y,Vars,Out).

% =====================
% remove_nonvars
% =====================

remove_nonvars(V,V):-isSlot(V),!.
remove_nonvars([],[]):-!.
remove_nonvars([V|L],LL):-isNonVar(V),!,remove_nonvars(L,LL).
remove_nonvars([V|L],[V|LL]):-remove_nonvars(L,LL).



% =====================
% Forall
% =====================

getMarkupFormula(L,forall(V,F),Vars,Chars):-not(is_list(V)),!,
        group_forall(forall(V,F),Next),!,
        cleanQuantifierConversionForWrite_forall(Next,O),
        getMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_forall(forall(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=forall(NL,F))).

getMarkupFormula(L,forall(V,F),Vars,Chars):- not(is_list(V)),!,
        getMarkupFormula(L,forall([V],F),Vars,Chars).

group_forall(forall(V1,forall(V2,forall(V3,forall(V4,forall(V5,F))))),forall([V1,V2,V3,V4,V5],F)):-!.
group_forall(forall(V1,forall(V2,forall(V3,forall(V4,F)))),forall([V1,V2,V3,V4],F)):-!.
group_forall(forall(V1,forall(V2,forall(V3,F))),forall([V1,V2,V3],F)):-!.
group_forall(forall(V1,forall(V2,F)),forall([V1,V2],F)):-!.
group_forall(forall(V1,F),forall([V1],F)):-!.

% =====================
% Exists
% =====================



getMarkupFormula(L,exists(V,F),Vars,Chars):-not(is_list(V)),!,
        group_exists(exists(V,F),Next),!,
        cleanQuantifierConversionForWrite_exists(Next,O),
        getMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_exists(exists(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=exists(NL,F))).

getMarkupFormula(L,exists(V,F),Vars,Chars):- not(is_list(V)),!,
        getMarkupFormula(L,exists([V],F),Vars,Chars).

group_exists(exists(V1,exists(V2,exists(V3,exists(V4,exists(V5,F))))),exists([V1,V2,V3,V4,V5],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,exists(V4,F)))),exists([V1,V2,V3,V4],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,F))),exists([V1,V2,V3],F)):-!.
group_exists(exists(V1,exists(V2,F)),exists([V1,V2],F)):-!.
group_exists(exists(V1,F),exists([V1],F)):-!.
% =====================
% Exists
% =====================

getMarkupFormula(L,exists(V,F),Vars,Chars):-not(is_list(V)),!,
        group_exists(exists(V,F),Next),!,
        cleanQuantifierConversionForWrite_exists(Next,O),
        getMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_exists(exists(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=exists(NL,F))).

getMarkupFormula(L,exists(V,F),Vars,Chars):- not(is_list(V)),!,
        getMarkupFormula(L,exists([V],F),Vars,Chars).

group_exists(exists(V1,exists(V2,exists(V3,exists(V4,exists(V5,F))))),exists([V1,V2,V3,V4,V5],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,exists(V4,F)))),exists([V1,V2,V3,V4],F)):-!.
group_exists(exists(V1,exists(V2,exists(V3,F))),exists([V1,V2,V3],F)):-!.
group_exists(exists(V1,exists(V2,F)),exists([V1,V2],F)):-!.
group_exists(exists(V1,F),exists([V1],F)):-!.

% =====================
% Findall
% =====================
        /*
getMarkupFormula(L,findall(V,F),Vars,Chars):-not(is_list(V)),!,
        group_findall(findall(V,F),Next),!,
        cleanQuantifierConversionForWrite_findall(Next,O),
        getMarkupFormula(L,O,Vars,Chars).

cleanQuantifierConversionForWrite_findall(findall(VL,F),O):-
        remove_nonvars(VL,NL),
        ((NL=[],!,O=F);(!,O=findall(NL,F))).

getMarkupFormula(L,findall(V,F),Vars,Chars):- not(is_list(V)),!,
        getMarkupFormula(L,findall([V],F),Vars,Chars).

group_findall(findall(V1,findall(V2,findall(V3,findall(V4,findall(V5,F))))),findall([V1,V2,V3,V4,V5],F)):-!.
group_findall(findall(V1,findall(V2,findall(V3,findall(V4,F)))),findall([V1,V2,V3,V4],F)):-!.
group_findall(findall(V1,findall(V2,findall(V3,F))),findall([V1,V2,V3],F)):-!.
group_findall(findall(V1,findall(V2,F)),findall([V1,V2],F)):-!.
group_findall(findall(V1,F),findall([V1],F)):-!.
                                 */
% =====================
% Indentation
% =====================

getMarkupFormula(L,C,Vars,Out):-
                C=..[Pred|ARGS],!,
                flag(indent,X,X+1),
                indent_it_x(X,PreOut),!,
                toMarkUp_lang(L,Pred,Vars,PredOut),!,
                make_args_out(L,ARGS,Vars,ArgsOut),!,
                sformat(Out,'~w(~w ~w)',[PreOut,PredOut,ArgsOut]), !,
                flag(indent,NX,NX-1).

make_args_out(L,[],Vars,''):-!.
make_args_out(L,[C],Vars,ArgsOut):-
                getMarkupFormula(L,C,Vars,ArgsOut).
make_args_out(L,[C|GS],Vars,ArgsOut):-
                getMarkupFormula(L,C,Vars,Out1),
                make_args_out(L,GS,Vars,Out2),!,
                sformat(ArgsOut,'~w ~w',[Out1,Out2]).

indent_it_x(0,''):-!.
indent_it_x(1,'\n         '):-!.
indent_it_x(X,Out):-XX is X -1,!, indent_it_x(XX,OutP),!,sformat(Out,'~w   ',[OutP]),!.

% =====================
% Prolog Tr./ansformation
% =====================

toMarkUp_lang(L,':-'(C,true),Vars,Out):-prolog_to_krlog(C,KR),!,toMarkUp_lang(L,KR,Vars,Out).
toMarkUp_lang(L,':-'(C,A),Vars,Out):-prolog_to_krlog(C,KRC),prolog_to_krlog(A,KRA),!,toMarkUp_lang(L,'=>'(KRA,KRC),Vars,Out).

toMarkUp_lang(L,(T^V),Vars,Out):-var_merge(Vars,V,TVars),!,toMarkUp_lang(L,T,TVars,Out).

%Terminal Control
toMarkUp_lang(html,lparen,Vars,'('):-!.
toMarkUp_lang(html,rparen,Vars,')'):-!.
toMarkUp_lang(kif,lparen,Vars,'('):-!.
toMarkUp_lang(kif,rparen,Vars,')'):-!.
toMarkUp_lang(html,nl,Vars,'<br>'):-!.
toMarkUp_lang(html,tab,Vars,'<li>'):-!.
toMarkUp_lang(kif,nl,Vars,'\n'):-!.
toMarkUp_lang(kif,tab,Vars,'\t'):-!.

% No parens (nv = no vector)
toMarkUp_lang(L,nv(Subj),Vars,Chars):-is_list(Subj),!,toMarkUp_list(L,Subj,Vars,Chars).
toMarkUp_lang(L,nv(Subj),Vars,Chars):-!,toMarkUp_lang(L,Subj,Vars,Chars).

toMarkUp_lang(_,writeFmt(F,A),Vars,Out):-sformat(Out,F,A),!.
toMarkUp_lang(_,surf,Vars,''):-!.
toMarkUp_lang(_,end_of_file,Vars,''):-!.

toMarkUp_lang(_,',',Vars,'and'):-!.
toMarkUp_lang(_,';',Vars,'or'):-!.
toMarkUp_lang(_,'=',Vars,'equal'):-!.
toMarkUp_lang(_,'deduced',Vars,' ').

% QUOTED STRING FORMAT
toMarkUp_lang(kif,Atom,_VS,Chars):-isCharCodelist(Atom),!,
        catch(sformat(Chars,'"~s"',[Atom]),_,sformat(Chars,'"~w"',[Atom])).
toMarkUp_lang(kif,Atom,_VS,Chars):-string(Atom),!,
        catch(sformat(Chars,'"~s"',[Atom]),_,sformat(Chars,'"~w"',[Atom])).


%LISTS
%toMarkUp_lang(LANG,[COMP],Vars,Atom)

toMarkUp_lang(L,[],Vars,Atom):-toMarkUp_lang(L,'NullSet',Vars,Atom).
%toMarkUp_lang(html,[Su|Bj],Vars,Chars):-toMarkUp_list(html,[Su|Bj],Vars,Chars1),sformat(Chars,'<div>(<ul>~w </ul>)</div>',[Chars1]).
toMarkUp_lang(kif,[Su|Bj],Vars,Chars):-toMarkUp_list(kif,[Su|Bj],Vars,Chars1),sformat(Chars,'(~w)',[Chars1]).


close_varlist([]):-!.
close_varlist('$VAR'(_)):-!.
close_varlist([V|VV]):-close_varlist(VV),!.

% SPECIAL FORMATS

toMarkUp_lang(_,writeq(Term),Vars,Atom):-!,sformat(Atom,'~q',[Term]).
toMarkUp_lang(kif,maillink(Title,Address,Subject),Vars,Address):-!.
toMarkUp_lang(kif,weblink(Title,URL),Vars,Title):-!.
toMarkUp_lang(kif,helplink(Title,URL),Vars,Title):-!.
toMarkUp_lang(L,explaination(PB),Vars,Atom):-
        flag(explaination_linenumber,_,1),
        getMarkupFormula(L,PB,Vars,AtomS),!,
        sformat(Atom,'\nExplaination:\n~w\n',[AtomS]).

toMarkUp_lang(LANG,krlog(COMP),Vars,Atom):-!,prolog_to_krlog(COMP,KR),toMarkUp_lang(LANG,KR,Vars,Atom).

toMarkUp_lang(LANG,kif(COMP),Vars,Atom):-!,toMarkUp_lang(kif,COMP,Vars,Atom).
toMarkUp_lang(LANG,html(COMP),Vars,Atom):-!,toMarkUp_lang(html,COMP,Vars,Atom).

toMarkUp_lang(html,select(Name,OptionList),Vars,Out):-toMarkUp_lang(html,options(OptionList),Vars,Options),sformat(Out,'<select sort name="~w" id="~w" size="1">~w</select>',[Name,Name,Options]).
toMarkUp_lang(html,chectheoryox(Name,on),Vars,Out):-
                sformat(Out,'<input type=chectheoryox name="~w" id="~w" checked>',[Name,Name]),!.
toMarkUp_lang(html,chectheoryox(Name,_),Vars,Out):-
                sformat(Out,'<input type=chectheoryox name="~w" id="~w">',[Name,Name]),!.
toMarkUp_lang(html,options([]),Vars,'').

toMarkUp_lang(L,getPrologVars(Form),Vars,Chars):-markUpVARLIST(L,Form,Vars,SChars),sformat(Chars,'~w',[SChars]),!.

toMarkUp_lang(L,getPrologVars(Form),Vars,Chars):-!,sformat(Chars,'; var_post_err (~q). ',[Form]).


toMarkUp_lang(html,qresult(Res),Vars,Chars):-!,sformat(Chars,'Result ',[Res]).

toMarkUp_lang(kif,qresult(Res),Vars,''):-!. %,sformat(Chars,'res="~w"\n',[Res]).

% Back into Standard Terms

format_o(Format,Stuff):-
        toMarkUp_lang(html,Stuff,_,Out),writeFmt(Format,[Out]).


toMarkUp_lang(html,options([Option|List]),Vars,Out):-
               toMarkUp_lang(html,option(Option),Vars,Out2),
               toMarkUp_lang(html,options(List),Vars,Out3),
               atom_concat(Out2,Out3,Out).

toMarkUp_lang(html,option(Option),Vars,Out):-sformat(Out,'<option value="~w">~w</option>',[Option,Option]).

% Numbers
toMarkUp_lang(_,Atom,_VS,Chars):-number(Atom),!,sformat(Chars,'~w',[Atom]).

toMarkUp_lang(L,Value,Vars,Chars):-
        mooCache(PredR, skolem, Value = x(Name,Expression),SKVARS,Context, Ctx, TN, Auth, State),!,
            toMarkUp_lang(kif,Name,Vars,NameQ),  prependQuestionMark(NameQ,NameQM),
            subst(x(Sk,Expression),Sk,NameQM,x(NSk,NExpression)),!,
            toMarkUp_lang(L,exists([NSk],NExpression),SKVARS,Chars).

% all other Formulas get intercepted here
toMarkUp_lang(L,Term,Vars,Chars):-compound(Term),!,
        getMarkupFormula(L,Term,Vars,Chars),!.

% PRETTYNESS
toMarkUp_lang(_,';',Vars,'or ').
toMarkUp_lang(_,',',Vars,'and ').
toMarkUp_lang(_,'neg',Vars,'neg ').
%toMarkUp_lang(_,entails,Vars,'modus-ponens ').
%toMarkUp_lang(_,entails,Vars,'modus-tollens ').



% Not compound - TEXT
toMarkUp_lang(html,Atom,Vars,Chars):-
        atom_codes(Atom,[115,107|_]),!,
                atom_lookup_theory_ctx(html,Atom,Context,Result,ID,Color,Page),!,
                (Result=ml(This) -> toMarkUp_lang(html,This,Vars,SResult) ; SResult=Result),
                (Context=none ->
                        sformat(Chars,'<font color=~w>~w</font>',[Color,SResult]);
                        sformat(Chars,'<A HREF="~w.jsp?logicforms=logicforms&submit=All%20Forms&data=~w&theory=~w">~w</A>',[Page,ID,Context,SResult])
                ).

toMarkUp_lang(html,Atom,Vars,Chars):-
                atom_lookup_theory_ctx(html,Atom,Context,Result,ID,Color,Page),!,
                (Result=ml(This) -> toMarkUp_lang(html,This,Vars,SResult) ; SResult=Result),
                (Context=none ->
                        sformat(Chars,'<font color=~w>~w</font>',[Color,SResult]);
                        sformat(Chars,'<A HREF="~w.jsp?req=SC&term=~w&stheory=~w">~w</A>',[Page,ID,Context,SResult])
                ).

toMarkUp_lang(kif,Atom,Vars,Chars):-
                atom_lookup_theory_ctx(kif,Atom,Context,Result,ID,Color,Page),!,
                (Result=ml(This) -> toMarkUp_lang(html,This,Vars,SResult) ; SResult=Result),
                        sformat(Chars,'~w',[SResult]).

% Lookup Proc
atom_lookup_theory_ctx(kif,Atom,none,none,Atom,Atom,black,stheory):-!.

atom_lookup_theory_ctx(_,Atom,Context,'GlobalContext',Atom,Atom,purple,skolems):-
        hlPredicateAttribute(Atom,'SkolemFunction'),!,isMooOption(opt_theory=Context),!.

atom_lookup_theory_ctx(Lang,Atom,Context,Atom,B,C,stheory):-
        atom_lookup_theory_ctx(Lang,Atom,Context,Atom,B,C).

atom_lookup_theory_ctx(kif,Atom,none,none,Atom,Atom,black):-!.
atom_lookup_theory_ctx(L,Atom,none,none,Atom,Atom,black):-once(atom_codes(Atom,Codes)),
        once((memberchk(34,Codes);memberchk(63,Codes);memberchk(32,Codes);memberchk(37,Codes))),!. % String
atom_lookup_theory_ctx(_,Atom,Context,'GlobalContext',Atom,Atom,blue):-!,isMooOption(opt_theory=Context),!. % Leftover must be PrologMOO (TODO)
atom_lookup_theory_ctx(_,Atom,'PrologMOO','GlobalContext',Atom,Atom,blue):-!.

codes_to_links(Codes,PrettyAtom):-
        getUnquotedCodes(Codes,UCodes),
        getKIFTokens(UCodes,WordList),
        concat_atom(WordList,'-',PrettyAtom),!.

getUnquotedCodes([34|Codes],UCodes):-
        (reverse(Codes,RCodes)),
        (ltrim(RCodes,[34|RUCodes])),
        reverse(RUCodes,UCodes).

getUnquotedCodes(UCodes,UCodes):-!.


%TODO Number?

% ================================================
%      toMarkUp_list
% ================================================

toMarkUp_list(L,Var,VS,Chars):-isSlot(Var),!,toMarkUp_slotValue(L,Var,VS,Chars).
toMarkUp_list(_,[],VS,''):-!.
toMarkUp_list(LANG,[H],VS,Chars):-!,
        toMarkUp_lang(LANG,H,VS,Chars).
toMarkUp_list(LANG,[H|T],VS,Chars):-!,
        toMarkUp_lang(LANG,H,VS,Chars1),
        toMarkUp_list(LANG,T,VS,Chars2),
        sformat(Chars,'~w ~w',[Chars1,Chars2]).

markUpVARLIST(L,[],Vars,''):-!.
markUpVARLIST(L,'$VAR'(_),Vars,''):-!.

markUpVARLIST(L,[VV|Varnames],Vars,Chars):-
                  VV=..[_,Name,Value],!,
                  toMarkupVarEquals(L,Name,Value,Vars,Chars1),
                  markUpVARLIST(L,Varnames,Vars,Chars2),
                  sformat(Chars,'~w\n~w',[Chars1,Chars2]).

toMarkupVarEquals(_,Name,Value,Vars,Chars):-
            toMarkUp_lang(kif,Name,Vars,NameQ),
            toMarkUp_slotValue(L,Value,Vars,ValChars),
            sformat(Chars,'~w = ~w',[NameQ,ValChars]).


% Real Prolog Var
toMarkUp_slotValue(L,Slot,VarList,Chars):- isVarProlog(Slot),!,
        toMarkUp_makeNamePrologVar(L,VarList,Slot,Name),
        atom_concat('?',Name,Chars),!.
% Slot 'Typed'
toMarkUp_slotValue(L,Slot,VarList,Chars):-isQualifiedAs(Slot,BaseType,Value,Subtype), !,
        toMarkUp_makeName(L,VarList,Slot,Subtype,Value,Name),
        close_freeVars(VarList,NVarList),
        append(NVarList,[Name=Value],NV),
        toMarkUp_lang(L,Value,NV,VChars),
        sformat(Chars,'<div title="~w">~w</div>',[Subtype,VChars]).

toMarkUp_makeNamePrologVar(L,VarList,Value,Name):-member(Name=Var,VarList),Var==Value,!.
toMarkUp_makeNamePrologVar(L,VarList,Value,Name):-getVarAtom(Value,NUame),atom_concat('?',NUame,Name).

getVarAtom(Value,Name):-var(Value),!,term_to_atom(Value,Vname),atom_codes(AVAR,[95,_|CODES]),atom_codes(Name,CODES),!.
getVarAtom('$VAR'(VNUM),Name):-concat_atom([VNUM],Name),!.



toMarkUp_makeName(L,VarList,Slot,BaseType,Value,Name):-
        member(Name=Var,VarList),Var==Slot,!.
toMarkUp_makeName(L,VarList,Slot,BaseType,Value,Name):-
        member(Name=Var,VarList),Var==Value,!.
toMarkUp_makeName(L,VarList,Slot,BaseType,Value,Name):-atom_concat('?',BaseType,Name).



close_freeVars(V,V):-proper_list(V),!.
close_freeVars(V,[]):-isSlot(V),!. %Closing List if there are no free getPrologVars
close_freeVars([X|XX],[X|More]):- close_freeVars(XX,More).





toMarkup_varProlog(kif,Var,_VS,NameQ):- _VS=[VV|_],nonvar(VV),VV=..[_,Name,VarRef],number(Name),Var==VarRef,!,sformat(NameQ,'?~d',[Name]).
toMarkup_varProlog(kif,Var,_VS,NameQ):- _VS=[VV|_],nonvar(VV),VV=..[_,Name,VarRef],Var==VarRef,!,sformat(NameQ,'?~w',[Name]).

toMarkup_varProlog(html,Var,_VS,NameQ):- _VS=[VV|_],nonvar(VV),VV=..[_,Name,VarRef],number(Name),Var==VarRef,!,sformat(NameQ,'?~d',[Name]).
toMarkup_varProlog(html,Var,_VS,NameQ):- _VS=[VV|_],nonvar(VV),VV=..[_,Name,VarRef],Var==VarRef,!,sformat(NameQ,'?~w',[Name]).

toMarkup_varProlog(T,Var,[_|Rest],Name):-nonvar(Rest),toMarkup_varProlog(T,Var,Rest,Name).
toMarkup_varProlog(kif,VAR,_,VarName):-term_to_atom(VAR,AVAR),atom_codes(AVAR,[95|CODES]),!,catch(sformat(VarName,'?HYP-~s',[CODES]),_,VarName='?HYP-AVAR').
toMarkup_varProlog(kif,VAR,_,VarName):-term_to_atom(VAR,AVAR),atom_codes(AVAR,CODES),!,catch(sformat(VarName,'?HYP-~s',[CODES]),_,VarName='?HYP-AVAR').
toMarkup_varProlog(html,VAR,VS,VarName):-toMarkup_varProlog(kif,VAR,VS,VarName).

prependQuestionMark(Name,NameQ):-atom_concat('?',Name,NameQ).


% ===================================================================
% File 'moo_utility.pl' 
% Maintainer: Douglas Miles 
% Contact: dmiles@users.sourceforge.net; 
% Version: 'moo_utility.pl' 1.0.0 
% ===================================================================


:-multifile(expireOptimizationsInContext/3).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).
            
% ===================================================================
% This File is the bootstrap for the Moo Infence engine one first calls "[belief_module]"
% So first is loads the proper files and then starts up the system
% There are no predicates defined in this file (it just uses other files' predicates)
% ===================================================================
% ===================================================================
% EXPORTS
% ===================================================================
isNonVar(Denotation):-not(isSlot(Denotation)).

% Is var means to Moo this is a Slot
isSlot(Denotation):-((isVarProlog(Denotation);isVarObject(Denotation))),!.

isSlot(Denotation,Denotation):- isVarProlog(Denotation),!.
isSlot(Denotation,PrologVar):- isVarObject(Denotation,PrologVar),!.

isHiddenSlot(Term):-fail.

isVarProlog(A):-((var(A);A='$VAR'(_))).

isVarObject(Denotation):-((
		  isObject(Denotation,BaseType),
		  arg(1,Denotation,Value),!,isSlot(Value))).

isVarObject(Denotation,Value):-((
		  isObject(Denotation,BaseType),
		  arg(1,Denotation,Value),!,isSlot(Value))).
	
isObject(Denotation,BaseType):-
	(((atom(BaseType) ->
		  (atom_concat('$',BaseType,F),functor(Denotation,F,2));
		  (functor(Denotation,F,2),atom_concat('$',BaseType,F))
		 ),!)).

isQualifiableAsClass(Atom):-atom(Atom),!.
isQualifiableAsClass('$Class'(Atom,_)):-atom(Atom),!.

isQualifiableAs(Denotation,BaseType,Value):-
		  isObject(Denotation,BaseType),
		  arg(1,Denotation,Value).


isQualifiedAs(Denotation,_,_):-not(compound(Denotation)),!,fail.
isQualifiedAs(Denotation,BaseType,Value):-
		  isQualifiedAs(Denotation,BaseType,Value,SubType).
isQualifiedAs(Denotation,BaseType,Value,SubType):-
		  isObject(Denotation,BaseType),
		  arg(1,Denotation,Value),
		  arg(2,Denotation,List),
		  lastImproperMember(BaseType,SubType,List).

lastImproperMember(Default,Default,List):-isVarProlog(List),!.
lastImproperMember(Default,Default,[]):-!.
lastImproperMember(Default,SubType,List):-proper_list(List),last(SubType,List).
lastImproperMember(Default,SubType,[SubType|End]):-isVarProlog(End),!.
lastImproperMember(Default,SubType,[_|Rest]):-
	lastImproperMember(Default,SubType,Rest),!.
	
		  

isQualifiedAndKnownAs(Denotation,BaseType,Value):-
		  isQualifiedAs(Denotation,BaseType,Value),!,
		  not(isVarProlog(Value)).

isQualifiedAndVarAs(Denotation,BaseType,Value):-
		  isQualifiedAs(Denotation,BaseType,Value),!,
		  isVarProlog(Value).

isQualifiedAndVarAndUnifiable(Denotation,BaseType,NValue):-
		  isQualifiedAs(Denotation,BaseType,Value),!,
		  (isVarProlog(Value);
		  (\+ \+ NValue=Value)),!.

isBodyConnective(Funct):-atom_concat(_,'_',Funct),!.
isBodyConnective(Funct):-atom_concat('t~',_,Funct),!.
isBodyConnective(Funct):-atom_concat('f~',_,Funct),!.
isBodyConnective(Funct):-member(Funct,[and,or,until,',',';',':-',unless,xor,holdsDuring]). % Other Propositional Wrappers

isEntityref(Var,Var):-isSlot(Var),!.
isEntityref(Term,A):-Term=..[F,A,B],!,atom_concat('$',_,F),!.





isLiteralTerm(A):-isLiteralTerm_util(A),!.
isLiteralTerm(not(A)):-isLiteralTerm_util(A),!.

isLiteralTerm_util(A):-var(A),!.
isLiteralTerm_util('$VAR'(_)):-!.
isLiteralTerm_util(string(_)):-!.
isLiteralTerm_util(A):-not(compound(A)),!.
isLiteralTerm_util(A):-string(A).


isEntitySlot(Term):-isSlot(Term),!.
isEntitySlot(Term):-not(compound(Term)),!.
isEntitySlot(Term):-isEntityFunction(Term,FnT,ArgsT),!.

isEntityFunction(Term,FnT,ArgsT):-isSlot(Term),!,fail.
isEntityFunction(Term,FnT,ArgsT):-atomic(Term),!,fail.
isEntityFunction(Term,FnT,ArgsT):-Term=..[FnT|ArgsT],hlPredicateAttribute(FnT,'Function'),!.



% ===================================================================
% getPrologVars/4. 
% ===================================================================

getPrologVars(Term, Vars, Singletons, Multiples) :-
    ((getPrologVars(Term, VarList, []),
    close_list(VarList),
    keysort(VarList, KeyList),
    split_key_list(KeyList, Vars, Singletons, Multiples))).

getPrologVars(Term,  [Term - x|V], V) :-isVarProlog(Term),!.
getPrologVars(Term, V, V) :-not(compound(Term)),!.
getPrologVars(Term,  V0, V) :-
	isQualifiedAs(Term,Type,PrologVar),
	Type \= existential,!,
	(isVarProlog(PrologVar) -> V0=[PrologVar - x|V]; V0=V),!.
	
getPrologVars(Term, V0, V) :- 
	 functor(Term, F, N),
	 getPrologVars(1, N, Term, V0, V).
getPrologVars(I, N, Term, V0, V) :-
    (  (I > N) -> V=V0
    ;   arg(I, Term, Arg),
	getPrologVars(Arg, V0, V1),
	J is I + 1,
	getPrologVars(J, N, Term, V1, V)
    ).

% ===================================================================
% getPrologVars/4. 
% ===================================================================

getAllPrologVars(Term, Vars, Singletons, Multiples) :-
    ((getAllPrologVars(Term, VarList, []),
    close_list(VarList),
    keysort(VarList, KeyList),
    split_key_list(KeyList, Vars, Singletons, Multiples))).

getAllPrologVars(Term,  [Term - x|V], V) :-isVarProlog(Term),!.
getAllPrologVars(Term, V, V) :-not(compound(Term)),!.
getAllPrologVars(Term, V0, V) :- 
	 functor(Term, F, N),
	 getAllPrologVars(1, N, Term, V0, V).
getAllPrologVars(I, N, Term, V0, V) :-
    (  (I > N) -> V=V0
    ;   arg(I, Term, Arg),
	getAllPrologVars(Arg, V0, V1),
	J is I + 1,
	getAllPrologVars(J, N, Term, V1, V)
    ).

% ===================================================================
% getSlots/4. Returns no Existential Body Vars
% ===================================================================

getSlots(Term, Vars, Singletons, Multiples) :-
    ((getSlots(Term, VarList, []),
    close_list(VarList),
    keysort(VarList, KeyList),
    split_key_list(KeyList, Vars, Singletons, Multiples))).

getSlots(Term,  [Term - x|V], V) :-isVarProlog(Term),!.
getSlots(Term, V, V) :-not(compound(Term)),!.
getSlots(Term, V, V) :-isHiddenSlot(Term),!.
getSlots(Term,  VO, V) :-
	isQualifiedAs(Term,existential,EVar),!,
	getSlots(EVar,  VO, V).
getSlots(Term,  V0, V) :-
	isQualifiedAs(Term,Type,PrologVar),!,
	(isVarProlog(PrologVar) -> V0=[Term - x|V]; V0=V),!.
getSlots(Term, V0, V) :- 
	 functor(Term, F, N),
	 getSlots(1, N, Term, V0, V).
getSlots(I, N, Term, V0, V) :-
    (  (I > N) -> V=V0
    ;   arg(I, Term, Arg),
	getSlots(Arg, V0, V1),
	J is I + 1,
	getSlots(J, N, Term, V1, V)
    ).


% ===================================================================
% getConstants/4. 
% ===================================================================

getConstants(Types,Term, Vars, Singletons, Multiples) :-
    ((getConstants(Types,Term, VarList, []),
    close_list(VarList),
    keysort(VarList, KeyList),
    split_key_list(KeyList, Vars, Singletons, Multiples))).

getConstants(Types,Term, [Term - x|V], V) :- getConstants(Types,Term),!.
getConstants(Types,Term, V, V) :- var(Term),!.
getConstants(Types,Term,  FOUND, V) :-
            Term=..[L,I|ST],
            getConstants(Types,L, VL, []),
            consts_l(Types,[I|ST], FLIST),
            append(V,FLIST,UND),
            append(VL,UND,FOUND),!.

getConstants(Types,Term, V, V) :- !.
    
consts_l(Types,[],[]).
consts_l(Types,[L|IST], FLIST):-
         getConstants(Types,L, FOUND,[]), 
         consts_l(Types,IST, FOUNDMore), !,
         append(FOUND,FOUNDMore,FLIST).

    
getConstants(_,Term):- (var(Term) ; Term='$VAR'(_)),!,fail.
getConstants(atomic,A):-atomic(A).
getConstants(skolems,'zzskFn'(_)).
getConstants(funct,'AssignmentFn'(_,_)).
getConstants(_,A):-!,fail.







retractallLogged(T):-
	format(':-retractall(~q).~n',[T]),
	retractall(T).

assertLogged(T):-
	format(':-assert(~q).~n',[T]),
	assert(T).



asserta_if_new(A):-A,!.
asserta_if_new(A):-asserta(A),!.


pvar_gen('$VAR'(N)):-idGen(N),!.


tn_link(Clause,ETracking,Context,ETracking):-!.

tn_link(Clause,ETracking,Context,theorya(Context,Num)):-atomic(ETracking),atom_codes(ETracking,[84,45|Rest]),!,number_codes(Num,Rest).
tn_link(Clause,ETracking,Context,theoryb(Context,ETracking)):-!.


ignore(X,Y):-ignore((X,Y)).


%   select(?Element, ?List, ?List2)
%   is true when the result of removing an occurrence of Element in List
%   is List2.
     /*
select(Element, [Element|Tail], Tail).
select(Element, [Head|Tail1], [Head|Tail2]) :-
        select(Element, Tail1, Tail2).
       */
	      
%======================================================================
% CLSID Generation
% idGen(+Var)
% Creates a new unique number   TODO
%
% Example:
% | ?- idGen(X).
% X = 2234
%======================================================================


idGen(X):-nonvar(X),!.
idGen('$VAR'(N)):-nonvar(N),!,sendNote(user,'MooKernel','Prolog Code Bug','Attempt to Gen CLSID on a frozen variable'),!,fail.
idGen(X):-retract(clsid(N)),X is N+1, assert(clsid(X)),!.
idGen(X):-get_time(T),convert_time(T,A,B,C,D,E,F,G),X  is 20000 + D *1000 + G * 1000 + F * 10 + E * 40,XX is X // 3 ,!,assert(clsid(X)).

getPrettyDateTime(String):-get_time(Time),convert_time(Time, String).

% ===================================================================
% has_singleton_vars(-Term).
% Checks for singleton variables
%
% Example:
%| ?- has_singleton_vars(a(X)).
% yes
%
%| ?- has_singleton_vars(a(X,X)).
% no
% ===================================================================


var_merge(L1,L2,O):-
	close_ll(L1,L11),
	close_ll(L2,L22),
	append(L1,L2,L4),
	catch(sort(L4,O),_,L4=O),!.
var_merge(L1,L2,L2):-!.

close_ll([],[]):-!.
close_ll(L1,[]):-isSlot(L1),!.
close_ll([H|T],[H|TT]):-close_ll(T,TT),!.


has_singleton_vars(Clause):-getPrologVars(Clause,_,[_|_],_).
has_singleton_vars(Clause):-getPrologVars(Clause,[_|_],_,[]).

% ===================================================================
% any_to_string(+Term,?String).
% converts any term to a list of prolog chars
%
% Example:
% | ?- any_to_string(X,Y).
% X = _h83
% Y = [95,104,56,51]
% | ?- any_to_string("X",Y).
% Y = [91,56,56,93]
% | ?- any_to_string("t(r)",Y).
% Y = [91,49,49,54,44,52,48,44,49,49,52,44,52,49,93]
% ===================================================================

any_to_string(Term,String) :- !,fmt_write_string(Atom,"%S",args(Term)),atom_codes(Atom,String).

% ===================================================================
% conv(?List,?Term) 
% conv_det(+List,-Term)
% term_to_list(?Term,?List)
% term_to_atomlist(?Term,?AtomList)

% Based on Conjuctions
% term_to_list/2 is the inverse of  conv(?List,?Term) and conv_det(+List,-Term)  

% term_to_atomlist/2 is a more agressive form of term_to_list
% Examples:
/*
| ?- conv(X,Y).
X = []
Y = true

| ?- conv([a,b],Y).
Y = (a  ','  b)

| ?- conv(Out,(a :- b)).
Out = [(a :- b)]


*/
% ===================================================================

conv([],true):-!.
conv([X|T],(X,R)) :- T \== [],!,conv(T,R).
conv([X],(X)).

conv_det([],true):-!.
conv_det(V1,_V2):-var(V1),!,fail.
conv_det([X|T],(X,R)) :- T \== [],!,conv_det(T,R).
conv_det([X],(X)).

term_to_list(Var,[]):-var(Var).
term_to_list(','(A,B,C,D,E,F),List):-!,term_to_list(((A,B),C),AL),term_to_list(((D,E),F),BL),append(AL,BL,List).
term_to_list(','(A,B,C,D,E),List):-!,term_to_list(((A,B),C),AL),term_to_list((D,E),BL),append(AL,BL,List).
term_to_list(','(A,B,C,D),List):-!,term_to_list(((A,B),C),AL),term_to_list((D),BL),append(AL,BL,List).
term_to_list(','(A,B,E),List):-!,term_to_list(((A,B)),AL),term_to_list((E),BL),append(AL,BL,List).
term_to_list((A,B),List):-!,term_to_list(A,AL),term_to_list(B,BL),append(AL,BL,List).
term_to_list(true,[]):-!.
term_to_list(List,List):-is_list(List).
term_to_list(Term,[Term]):-compound(Term).
term_to_list(Term,[Term]):-atomic(Term).

term_to_atomlist([Var],[Var]) :- ( var(Var) ; atomic(Var) ),!.
term_to_atomlist([TERM],[H|T]):-!,
         TERM=..[H|Args],
         term_to_atomlist(Args,T).
term_to_atomlist([H|T],List):-!,
         term_to_atomlist([H],HList),
         term_to_atomlist(T,TList),
         append(HList,TList,List).
term_to_atomlist(Term,AtomList):-!,
         conv(TermList,Term),
         term_to_atomlist(TermList,AtomList). 


% ===================================================================
% getSharedVariables(Term1,Term2).
% tests to see if Varables are shared between two Terms
% ===================================================================

getSharedVariables(Left,Right):-copy_term((Left,Right),(VLP,VRP)),numbervars((VLP,VRP)),
            term_to_atomlist(VLP,VLPP),term_to_atomlist(VRP,VRPP),
             member('$VAR'(N),VLPP),member('$VAR'(N),VRPP).
                                          
                                          
% ===================================================================
% delete_once/3.
/*
delete_once(+List1, ?Elem, ?List2)
Delete forall members of List1 that simultaneously equal with Elem and equal the result with List2. 
*/

% ===================================================================

delete_once([],X,X):-!.
delete_once(_,[],[]):-!.
delete_once([DeleteThisItem],[DeleteThisItem|ListOfMore],ListOfMore):-!.
delete_once([DeleteThisItem],[Skip|ListOfMore],[Skip|ListAfterDelete]):- !,
         delete_once([DeleteThisItem],ListOfMore,ListAfterDelete).
delete_once([DeleteThisItem|DeleteThese],SourceList,ResultList):-!,
         delete_once([DeleteThisItem],SourceList,DeleteThisItemResult),
         delete_once(DeleteThese,DeleteThisItemResult,ResultList).


% ===================================================================
% clean_true(+DirtyTerm,-CleanTerm).
% removes resundant true atoms from terms
% ===================================================================

clean_true(X,Y):-once((term_to_list(X,L))),once((delete(L,true,L2))),once((conv_det(L2,Y))).

% ===================================================================
% optional_bound(+Term1,+Term2).
% Prepairs an option binding and alway succeeds
% ===================================================================
optional_bound(Left,Right):-ignore(Left=Right).



% ===================================================================
% Global Variable Manipuitalion for Moo
% ===================================================================

%global_set(Name,Value):-flag(Name,_,Value).

global_increment(Name):-flag(Name,N,N+1).

%global_get(Name,Value):- flag(Name,Value,Value).

copy_term(Term,TermCopy,CopyOfVars):-copy_term(Term,TermCopy),
               getPrologVars(TermCopy,CopyOfVars,_,_).


% ===================================================================
%    moo_B_consult/1
% ===================================================================

moo_B_consult(FileName):-
      [FileName],!.

moo_B_consult(FileName):-real_prolog_file_name(FileName,AbsoluteFile),
      [AbsoluteFile].

% ===================================================================
%    moo_B_load_dyn/1
% ===================================================================

moo_B_load_dyn(FileName):-
               real_file_name(FileName,LocalFile),
               load_dyn(LocalFile).
/*
               file_open(LocalFile,'r',INPUT),
               repeat,
               file_read(INPUT,Term),
               assert(Term),
               Term=end_of_file,!,
               moo_B_seen,!.
  */

assert_prolog(X,_) :- (var(X)),!.
assert_prolog([H|T],Vars) :-list_to_term([H|T],Term),!,assert_prolog(Term,Vars).
assert_prolog(PTERM_NATIVE,Vars):-PTERM_NATIVE=..[C,X,Y],'surface-instance'(C,'Junctive',_),!,
         assert_prolog(X,Vars),
         assert_prolog(Y,Vars).
assert_prolog(X,_Vars) :- predicate_property(X,built_in),!. 
assert_prolog(X,_Vars) :- ground(X),retract(X),fail.
assert_prolog(X,_Vars) :- /* not(exists_in_database(X)),!, */ assert(X). %, writeIfOption(modification,(X),Vars).
assert_prolog(_X,_Vars) :- !. %,not(exists_in_database(X)), assert_prolog(X). %writeIfOption(disp_modification,assert_prolog(X),Vars)

assert_prolog(Context_atom,WFF,Vars):-
         add_context(Context_atom,WFF,WFFAC),
         assert_prolog(WFFAC,Vars).



do_to_conjuncts(Var,G):- (var(Var);var(G)),!.
do_to_conjuncts((A,B),G):- !,
         ignore(once(do_to_conjuncts(A,G))),
         ignore(once(do_to_conjuncts(B,G))).
do_to_conjuncts(A,G):- !,
         ignore((C=..[G,A],once(C))).  

do_to_conjuncts(Var,Var2,G):- (var(Var);var(Var2   );var(G)),!.



do_to_conjuncts(V,(A,B),G):- !, %numbervars((A,B)),
         ignore(once(do_to_conjuncts(V,A,G))),
         ignore(once(do_to_conjuncts(V,B,G))).

do_to_conjuncts(V,A,G):- !,
         ignore((C=..[G,V,A],once(C))).  
                        


mretractall(TERM):-!, functor(TERM,F,A),predicate_property(TERM,Z),!,abolish(F/A),predicate_property(TERM,Q),!,dynamic(F/A),predicate_property(TERM,P),!. 


set_for_hilog_table(PrologForm):- 
            functor(PrologForm,OP,_A), 
            (predicate_property(PrologForm,(built_in)) ; 'surface-instance'(OP,'Connective',_)),!.

set_for_hilog_table(PrologForm):- telling_file,
            functor(PrologForm,OP,A), 
         	write_clause_to_file(assert(functsymbol(OP/A))), % Asserts to be known for Non
         	write_clause_to_file((table(OP/A))), % 
            write_clause_to_file(hilog(OP)).
set_for_hilog_table(_PrologForm).

exists_in_database((Y:-_)):-predicate_property(Y,built_in).
exists_in_database((X)):-predicate_property(X,built_in).
exists_in_database((Y:-X)):-!,not(predicate_property(Y,built_in)),clause(Y,X).
exists_in_database((Y)):-!,not(predicate_property(Y,built_in)),clause(Y,true).



clause_id_gen(CLID):-idGen(CLID).



% not assertzble objects
non_assertzble(Var):-var(Var),!.
non_assertzble([]):-!.
non_assertzble(end_of_file):-!.
non_assertzble('end-of-file'):-!.
non_assertzble(fail):-!.
non_assertzble(false):-!.
non_assertzble(true):-!.
non_assertzble(comment(_C)):-!.
non_assertzble(browser_only(_C)):-!.
non_assertzble(List):-is_list(List),length(List,X),X<8,!. 

once_ignore(X):-once(ignore(X)).

get_storage_file(PrivateName,RD):-
   'LOGIC_ENGINE_RT'(RealDir),
   atom_codes(RealDir,RealDirS),
   append(RealDirS,[47|PrivateName],RDS),
   atom_codes(RD,RDS).

get_storage_exists_file(PrivateName,RD):-
   'LOGIC_ENGINE_RT'(RealDir),
   atom_codes(RealDir,RealDirS),
   append(RealDirS,[47|PrivateName],RDS),
   atom_codes(RD,RDS),
   (not(exists_file(RD)) -> 
      (file_open(RD,'w',IOPort),fmt_write(IOPort,"end_of_file.\n",_),file_close(IOPort))  % Make the file
      ; true ).


/*
% SWI Builtin
union([X|L1],L2,L3) :-
	identical_member(X,L2),
	!,
	union(L1,L2,L3).
union([X|L1],L2,[X|L3]) :-
	union(L1,L2,L3).
union([],L,L).
*/
% ===================================================================
%  safe_theory_info_db(Context_Name,Can,WFS,PFile) Creates file paths
% ===================================================================


safe_theory_info_db(Context_Name,Can,WFS,PFile):-
                  theory_make_status_start(theory(Context_Name,Can)=_),!,
                  add_file_extension(".wfs",Can,WFS),
                  add_file_extension(".P",Can,PFile),!.

safe_theory_info_db(Context_Name,error,error,error):- sendNote(error,ioSubsystem,'Context file not found',['the Context is like not correct ',Context_Name]),!,fail.

                  
actual_file_name(SourceFile,SourceFileLocal):-
            atom_codes(SourceFile,[99,58,47,47|Rest]),!,
            atom_codes(NewSourceFile,[99,58,47|Rest]),
            actual_file_name(NewSourceFile,SourceFileLocal).

actual_file_name(SourceFileLocal,SourceFileLocal):-!.


safe_file_open(SourceFile,MODE,HANDLE):-
                  actual_file_name(SourceFile,SourceFileLocal),
                  file_open(SourceFileLocal,MODE,TH),
                        (TH = (-1) ->
                                    (sendNote(error,ioSubsystem,'file not found',['the file or path ',SourceFile,' is not accessable ']),!,fail )
                                    ;
                                   HANDLE=TH) .





% ===================================================================
% split_key_list/4. (Adapted from Richard O'Keefe). 
% ===================================================================


split_key_list([], [], [], []).
split_key_list([V-_,W-_|Vs], Vars, Singletons, Multiples) :- W == V, !,
    Vars = [V|Vars1],
    Multiples = [V|Multiples1],
    split_key_list(Vs, V, Vs1),
    split_key_list(Vs1, Vars1, Singletons, Multiples1).
split_key_list([V-_|Vs], [V|Vars], [V|Singletons], Multiples) :-
    split_key_list(Vs, Vars, Singletons, Multiples).
split_key_list([W - _|Vs], V, Vs1) :- W == V, !,
    split_key_list(Vs, V, Vs1).
split_key_list(Vs1, _, Vs1).




close_list([]):-!.
close_list([_]):-!.
close_list([_,_]):-!.
close_list([_,_|CLOSE]):-!,close_list(CLOSE).
close_list([_,_,_|CLOSE]):-!,close_list(CLOSE).
close_list([_,_,_,_,_|CLOSE]):-!,close_list(CLOSE).
close_list([_,_,_,_,_,_,_|CLOSE]):-!,close_list(CLOSE).

open_list(A,B):-!,append(A,_,B).



unbind_numbers('$VAR'(P),'$VAR'(P)):-!.
unbind_numbers('AssignmentFn'(P,Q),'AssignmentFn'(P,Q)):-!.
unbind_numbers(P,_):-number(P),!.
unbind_numbers(P,P):-var(P),!.
unbind_numbers(P,Q):- !,P =.. [F|ARGS],!,
                   unbind_numbers_l(ARGS,UARGS),
                    Q=..[F|UARGS],!.

unbind_numbers_l([],[]).
unbind_numbers_l([A|RGS],[U|ARGS]):-!,
         unbind_numbers(A,U),
         unbind_numbers_l(RGS,ARGS),!.


consult_as_dynamic(FilenameLocal):- 
         open(FilenameLocal,'read',R),   
         repeat,
         read(R,TERM),
         ((TERM = end_of_file -> (true,!) ; 
               ((
                 assert(TERM),fail
                 ))
         )),!,
         close(R).

% ===================================================================
% Substitution based on ==
% ===================================================================

% Usage: subst(+Fml,+X,+Sk,?FmlSk)

subst(A,B,C,D):-(nd_subst(A,B,C,D)),!.

nd_subst(  Var, VarS,SUB,SUB ) :- Var==VarS,!.
nd_subst(        P, X,Sk,        P1 ) :- functor(P,_,N),nd_subst1( X, Sk, P, N, P1 ).

nd_subst1( _,  _, P, 0, P  ).

nd_subst1( X, Sk, P, N, P1 ) :- N > 0, P =.. [F|Args], nd_subst2( X, Sk, Args, ArgS ),
            nd_subst2( X, Sk, [F], [FS] ),
            P1 =.. [FS|ArgS].

nd_subst2( _,  _, [], [] ).
nd_subst2( X, Sk, [A|As], [Sk|AS] ) :- X == A, !, nd_subst2( X, Sk, As, AS).
nd_subst2( X, Sk, [A|As], [A|AS]  ) :- var(A), !, nd_subst2( X, Sk, As, AS).
nd_subst2( X, Sk, [A|As], [Ap|AS] ) :- nd_subst( A,X,Sk,Ap ),nd_subst2( X, Sk, As, AS).
nd_subst2( X, Sk, L, L ).

weak_nd_subst(  Var, VarS,SUB,SUB ) :- Var=VarS,!.
weak_nd_subst(        P, X,Sk,        P1 ) :- functor(P,_,N),weak_nd_subst1( X, Sk, P, N, P1 ).

weak_nd_subst1( _,  _, P, 0, P  ).

weak_nd_subst1( X, Sk, P, N, P1 ) :- N > 0, P =.. [F|Args], weak_nd_subst2( X, Sk, Args, ArgS ),
            weak_nd_subst2( X, Sk, [F], [FS] ),
            P1 =.. [FS|ArgS].

weak_nd_subst2( _,  _, [], [] ).
weak_nd_subst2( X, Sk, [A|As], [Sk|AS] ) :- X = A, !, weak_nd_subst2( X, Sk, As, AS).
weak_nd_subst2( X, Sk, [A|As], [A|AS]  ) :- var(A), !, weak_nd_subst2( X, Sk, As, AS).
weak_nd_subst2( X, Sk, [A|As], [Ap|AS] ) :- weak_nd_subst( A,X,Sk,Ap ),weak_nd_subst2( X, Sk, As, AS).
weak_nd_subst2( X, Sk, L, L ).


% ===================================================================
% Destructive Freezing/Melting
% ===================================================================

call_frozen(Goal):-
	copy_term(Goal,Copy),
	crossref_vars(Goal,Copy,CopyKey),
	call(Copy),
	setarg_vars(Copy,CopyKey,Goal).
	

freeze_vars(Fml,Frozen,MeltKey):-
	copy_term(Fml,Frozen),
	crossref_vars(Fml,Frozen,MeltKey),
	numbervars(Frozen),!.
	
melt_vars(Frozen,[]=[],Frozen):- !.
melt_vars(Frozen,[OV|OL]=[FV|VL],Thawed):-
	subst(Frozen,FV,OV,Thawing),
	melt_vars(Thawing,OL=VL,Thawed),!.

setarg_vars(Frozen,[]=[],Frozen):- !.
setarg_vars(Frozen,OL=NL,Thawed):-
	OT=..[getPrologVars|OL],
	NT=..[getPrologVars|NL],
	setarg_vars(1,Frozen,OT,OL,NT,NL,Thawed).
	
setarg_vars(_,Thawed,OT,OL,_,[],Thawed):-!.
setarg_vars(N,Frozen,OT,[OH|OL],NT,[NH|NL],Thawed):-
	setarg(N,OT,NH),
	NN is N +1,
	setarg_vars(NN,Frozen,OT,OL,NT,NL,Thawed),!.
	

crossref_vars(Fml,Frozen,FmlVars = FrozenVars):-
	free_variables(Fml,FmlVars),
	free_variables(Frozen,FrozenVars),!.


% ===================================================================
% Substitution based on =
% ===================================================================

% Usage: repl(+Fml,+X,+Sk,?FmlSk)

replc(Fml,X,Sk,FmlSk):-
	(repl(Fml,X,Sk,FmlSk)),!.
/*
	copy_term(Fml,FmlX),
	numbervars(FmlX),
	repl(FmlX,X,Sk,FmlSk),!.
*/	
		

repl(  Var, VarS,SUB,SUB ) :- Var=VarS,!.
repl(        P, X,Sk,        P1 ) :- functor(P,_,N),repl1( X, Sk, P, N, P1 ).

repl1( _,  _, P, 0, P  ).

repl1( X, Sk, P, N, P1 ) :- N > 0, P =.. [F|Args], repl2( X, Sk, Args, ArgS ),
            repl2( X, Sk, [F], [FS] ),
            P1 =.. [FS|ArgS].

repl2( _,  _, [], [] ).
repl2( X, Sk, [A|As], [Sk|AS] ) :- X = A, !, repl2( X, Sk, As, AS).
repl2( X, Sk, [A|As], [A|AS]  ) :- var(A), !, repl2( X, Sk, As, AS).
repl2( X, Sk, [A|As], [Ap|AS] ) :- repl( A,X,Sk,Ap ),repl2( X, Sk, As, AS).
repl2( X, Sk, L, L ).

weak_repl(  Var, VarS,SUB,SUB ) :- Var=VarS,!.
weak_repl(        P, X,Sk,        P1 ) :- functor(P,_,N),weak_repl1( X, Sk, P, N, P1 ).

weak_repl1( _,  _, P, 0, P  ).

weak_repl1( X, Sk, P, N, P1 ) :- N > 0, P =.. [F|Args], weak_repl2( X, Sk, Args, ArgS ),
            weak_repl2( X, Sk, [F], [FS] ),
            P1 =.. [FS|ArgS].

weak_repl2( _,  _, [], [] ).
weak_repl2( X, Sk, [A|As], [Sk|AS] ) :- X = A, !, weak_repl2( X, Sk, As, AS).
weak_repl2( X, Sk, [A|As], [A|AS]  ) :- var(A), !, weak_repl2( X, Sk, As, AS).
weak_repl2( X, Sk, [A|As], [Ap|AS] ) :- weak_repl( A,X,Sk,Ap ),weak_repl2( X, Sk, As, AS).
weak_repl2( X, Sk, L, L ).


%Shared with XSB




file_newer(F1,F2):- time_file(F1,T1),time_file(F2,T2),!,T1>T2.


%:-getenv('LOGIC_ENGINE_RT',RealDir),assert('LOGIC_ENGINE_RT'(RealDir)) .

reconsult(F):-consult(F).

set_global_compiler_options(_Ignore).
load_dyn(X):-[X].
assert_new(X):-retractall(X),assert(X).

%%:-load_library(library(quintus)).


file_open(FileName,r,IOPort):- open(FileName,read,IOPort,[type(binary)]).
file_open(FileName,w,IOPort):- open(FileName,write,IOPort,[type(binary)]).
file_close(IOPort):-close(IOPort).

fmt_write_string(String,Format,Args):- catch(Args=..[_|FARGS],_,FARGS=[]),
      string_to_atom(Format,FMTSTR),
      fmtString(String,FMTSTR,FARGS).

fmt_write(Format,Args):- catch(Args=..[_|FARGS],_,FARGS=[]),
      string_to_atom(Format,FMTSTR),
      writeFmt(FMTSTR,FARGS).

fmt_write(OUTPUT,Format,Args):- catch(Args=..[_|FARGS],_,FARGS=[]),
      string_to_atom(Format,FMTSTR),
      writeFmt(OUTPUT,FMTSTR,FARGS).

real_file_name(X,X).

setPrologFlag(X,Y):-catch(current_prolog_flag(X,Y),_,fail).
setPrologFlag(_,' ').

%import(_ignore).
%export(_ignore).

real_kif_file_name(_FileName,AbsoluteFile):-
      once(add_file_extension(".kif",_FileName,SeeThisFile)),
      once(add_file_user_lib_directory(SeeThisFile,AbsoluteFile)).
                                                         
real_prolog_file_name(_FileName,AbsoluteFile):-
      once(add_file_extension(".P",_FileName,SeeThisFile)),
      once(add_file_user_lib_directory(SeeThisFile,AbsoluteFile)).
                                                         

add_file_user_lib_directory(_LocalFile,AbsoluteFile):- 
         name(_LocalFile,FileNameString),
         once((('LOGIC_ENGINE_RT'(RTD),!,name(RTD,RTDString));((writeIfOption(cb_error,'MOO_XSB_RT Not Set in Environment',_),RTDString=[])))),
         once(append(RTDString,[47|FileNameString],LEPAth)),
         name(AbsoluteFile,LEPAth).

add_file_extension(EXT,FileName,LocalFile):-atom_codes(Extension,EXT),
            safe_file_name_extension(FileName, Extension, LocalFile).

safe_file_name_extension(SourceFile,Ext,SurfaceFile):-
            actual_file_name(SourceFile,File),
            file_name_extension(Base,_,File), 
            file_name_extension(Base,Ext,SurfaceFile). 


file_get0(IOPort,end_of_file):-  at_end_of_stream(IOPort),!.
file_get0(IOPort,Char):- get_code(IOPort,Char),ignore((Char<0)).
 

file_getbuf(IOPort,1,Char,1):-get_char(IOPort,Char).


max(X,Y,Max) :-
	X >= Y ->
		Max = X;
	%true ->
		Max = Y.

min(X,Y,Min) :-
	X =< Y ->
		Min = X;
	%true ->
		Min = Y.



isCharCodelist([]).  isCharCodelist([A|T]):-integer(A),A>9,A<128,isCharCodelist(T).


nop.
nop(_).

conjoin(A,B,C) :-
	A == true ->
		C = B;
	B == true ->
		C = A;
	A == false ->
		C = false;
	B == false ->
		C = false;
	%true ->
		C = (A , B).

disjoin(A,B,C) :-
	A == true ->
		C = true;
	B == true ->
		C = true;
	A == false ->
		C = B;
	B == false ->
		C = A;
	%true ->
		C = (A ; B).




delete_ident(V,_,V):-isSlot(V),!.
delete_ident([],_,[]):-!.
delete_ident([Item|L1],ItemT,L2):-Item==ItemT, !,delete_ident(L1,ItemT,L2).
delete_ident([A|L1],ItemT,[A|L2]):-!,delete_ident(L1,ItemT,L2).


nth_identical_member(X,[],_):-!,fail.
nth_identical_member(X,[XX|_],1):-X==XX,!.
nth_identical_member(X,[_|XXs],NN):- nth_identical_member(X,XXs,N),NN is N + 1.


% ==========================================================
% TEMP DB
% ==========================================================

assert_conj(end_of_file,0).
assert_conj(end_of_file:_,0).
assert_conj(true,0).
assert_conj((X,Y):V,(XX,YY):V):-!,
      assert_conj(X,XX),
      assert_conj(Y,YY).
assert_conj((X,Y),(XX,YY)):-!,
      assert_conj(X,XX),
      assert_conj(Y,YY).

assert_conj(Y,0):- catch(in_pl_db(Y),E,fail),!.
assert_conj(Y,Ref):-!,unnumbervars(Y,Z),logOnFailure(assert(Z,Ref)).

in_pl_db((IH:-IB)):-!,
	renumbervars((IH:-IB),(H:-B)),!, 
	clause(H,B,CLDEX),
	clause(RH,RBody,CLDEX),
	numbervars((RH,RBody),'Test',0,_),numbervars((H,Body),'Test',0,_),
	(RH,RBody)==(H,Body),!.

in_pl_db(IH):-!,
	renumbervars((IH),(H)),!,
	clause(H,true,CLDEX),
	clause(RH,true,CLDEX),
	numbervars((RH),'Test',0,_),numbervars((H),'Test',0,_),
	(RH)==(H),!.
	
renumbervars(N,U):-
	unnumbervars(N,U),
	numbervars(U,'Test',0,_),!.

retract_ref(0):-!.
retract_ref((X,Y)):-!,
	retract_ref(X),
	retract_ref(Y).
retract_ref(X):-!,erase(X);true.


retract_conj(end_of_file,true).
retract_conj(end_of_file:_,true).
retract_conj(true,true).
retract_conj((X,Y):V,(XX,YY):V):-!,
      retract_conj(X,XX),
      retract_conj(Y,YY).
retract_conj((X,Y),(XX):V):-!,
      retract_conj(X,XX),
      retract_conj(Y,YY).

retract_conj((IH:-IB),(IH:-IB)):-!,
	renumbervars((IH:-IB),(H:-B)),!, 
	clause(H,B,CLDEX),
	clause(RH,RBody,CLDEX),
	numbervars((RH,RBody),'Test',0,_),numbervars((H,Body),'Test',0,_),
	(RH,RBody)==(H,Body),erase(CLDEX).

retract_conj((IH),(IH)):-!,
	renumbervars((IH),(H)),!, 
	clause(H,B,CLDEX),
	clause(RH,RBody,CLDEX),
	numbervars((RH),'Test',0,_),numbervars((H),'Test',0,_),
	(RH)==(H),erase(CLDEX).

retract_conj((IH),true):-!.



listing_template(T):-clause(T,N),
	format('~q:-~q.\n',[T,N]),
	fail.
	
listing_template(T).



assert_conj_count(A,C):-
         flag('Assert Conj',_,0),
         assert_conj_c(A),!,
         flag('Assert Conj',C,C),!.

assert_conj_c(end_of_file).
assert_conj_c(true).
assert_conj_c((X,Y)):-
      assert_conj_c(X),
      assert_conj_c(Y).
assert_conj_c(Y):-logOnFailure(assert(Y)),global_increment('Assert Conj').

retract_conj_count(A,C):-
         flag('Retract Conj',_,0),
         retract_conj_c(A),!,
         flag('Retract Conj',C,C),!.

retract_conj_c(end_of_file).
retract_conj_c(true).
retract_conj_c((X,Y)):-
      retract_conj_c(X),
      retract_conj_c(Y).
retract_conj_c(Y):-retractall(Y),global_increment('Retract Conj').

assert_conj_q(true).
assert_conj_q((X,Y)):-!,
      assert_conj_q(X),
      assert_conj_q(Y).
assert_conj_q(Y):-logOnFailure(assert(Y)). % ,post_each_can_form(Y).

retract_conj_q(true).
retract_conj_q((X,Y)):-!,
      retract_conj_q(X),
      retract_conj_q(Y).
retract_conj_q(Y):-retractall(Y),post_each_can_form(Y).



concat_shell(Shell,yes):-logOnFailure(concat_atom(Shell,ShellCmd)),ignore(shell(ShellCmd)).

sandbox(G):-catch(G,_,fail). % TODO .. add to ISO Standard :)


my_catch(X,E,G):-catch(X,E,G).

deduce_by(_).

unifiable_member(X,[Y|_]) :-			% run-time predicate to
	unify_with_occurs_check(X,Y),!.				% find complementary ancestor
unifiable_member(X,[_|L]) :-
	unifiable_member(X,L).

identical_member(X,[Y|_])  :-			% run-time predicate to
	X == Y,					% find identical ancestor
	!.

identical_member(X,[_|L]) :-
	identical_member(X,L).
	
fidentical_member(X,[Y|_])  :-			% run-time predicate to
	X == Y -> ! ;identical_member(X,[_|L]).
	 		
	

nonidentical_member(X,[Y|L]) :-			% run-time predicate to
	X \== Y,				% find identical ancestor
	nonidentical_member(X,L).
nonidentical_member(_X,[]).

differing_member(X,[Y|L]) :-			% run-time predicate to
	dif(X,Y),				% constrain literal to not
	differing_member(X,L).			% be identical to ancestor
differing_member(_X,[]).


apply_to_conjuncts(Wff,P,Wff1) :-
	Wff = (A , B) ->
		apply_to_conjuncts(A,P,A1),
		apply_to_conjuncts(B,P,B1),
		conjoin(A1,B1,Wff1);
	%true ->
		P =.. G,
		append(G,[Wff,Wff1],G1),
		T1 =.. G1,
		call(T1).

apply_to_elements([X|L],P,Result) :-
	P =.. G,
	append(G,[X,X1],G1),
	T1 =.. G1,
	call(T1),
	apply_to_elements(L,P,L1),
	conjoin(X1,L1,Result).
apply_to_elements([],_,true).

apply_to_elements2([X|L],P,[X1|L1]) :-
	T1 =.. [P,X,X1],
	call(T1),
	apply_to_elements2(L,P,L1).
apply_to_elements2([],_,[]).


apply_to_list([],_,[]).
apply_to_list([Elem|List],P,[Elem1|List1]) :-
	T =.. [P,Elem,Elem1],
	call(T),
	apply_to_list(List,P,List1).


apply_to_list_flat([],_,[]).
apply_to_list_flat([Elem|List],P,ResList) :-
	T =.. [P,Elem,Result1],
	call(T),
	apply_to_list(List,P,List1),
	append(Result1,List1,ResList).




mreplc(F,F,R,R):-!.
mreplc(B,_,_,B):-not(compound(B)),!.
mreplc([H|Body],F,R,[NH|NBody]):-!,
	mreplc(H,F,R,NH),!,
	mreplc(Body,F,R,NBody),!.
mreplc(A,F,R,Rs):-!,
	A=..[H|Body],%  true,
	mreplc(H,F,R,NH),
	mreplc(Body,F,R,NBody),
	Rs=..[NH|NBody].
mreplc(B,_,_,B).



getfunctor(not(Fact),A):-!,getfunctor(Fact,A).
getfunctor('~'(Fact),A):-!,getfunctor(Fact,A).
getfunctor(Fact,holds):-Fact=..[holds,A|_],isSlot(A),!.
getfunctor(Fact,A):-Fact=..[holds,A|_],!.
getfunctor(Fact,A):-functor(Fact,A,_).


% uses getPrologVars(Goal, Vars, Singletons, Multiples)
		


list_to_comma(Var,Var):-isSlot(Var),!.
list_to_comma([Var],Var):-isSlot(Var),!.
list_to_comma([],true).
list_to_comma([X],X).
list_to_comma([X|Y],(XX,YY)):-!,
	list_to_comma(X,XX),
	list_to_comma(Y,YY).
list_to_comma(and(X,Y),(XX,YY)):-!,
	list_to_comma(X,XX),
	list_to_comma(Y,YY).
list_to_comma(X,X).
	


conjunctsToList(Ante,Ante):-isSlot(Ante),!.
conjunctsToList([],[]).
conjunctsToList(and(A,B),List):-
	conjunctsToList(A,AL),		
	conjunctsToList(B,BL),
	append(AL,BL,List).		
conjunctsToList([A|B],List):-
	conjunctsToList(A,AL),		
	conjunctsToList(B,BL),
	append(AL,BL,List).		
conjunctsToList((A,B),List):-
	conjunctsToList(A,AL),		
	conjunctsToList(B,BL),
	append(AL,BL,List).		
conjunctsToList((Ante),[(Ante)]).




prologEach([],Item,_):-!.
prologEach([Item|Rest],Test,Goal):-((
	not(not((Item=Test,Goal))),!,
	prologEach(Rest,Test,Goal),!)).


prologAtLeastOne([],Item,_):-!.
prologAtLeastOne([Item|Rest],Item,Goal):-once(Goal),!.
prologAtLeastOne([_|Rest],Item,Goal):-
	prologAtLeastOne(Rest,Item,Goal),!.

prologPartitionList(Pos,Item,Goal,Passed,Unshared):-
	(prologPartitionList1(Pos,Item,Goal,Passed,Unshared)).

prologPartitionList1([],_,_,[],[]):-!.
prologPartitionList1([Item|Rest],Test,Goal,[Item|Passed],Failures):-
	not(not((Item=Test,Goal))),!,
	prologPartitionList1(Rest,Test,Goal,Passed,Failures).
prologPartitionList1([Failed|Rest],Item,Goal,Passed,[Failed|Failures]):-!,
	prologPartitionList1(Rest,Item,Goal,Passed,Failures).


remove_numbers([],[]):-!.
remove_numbers([N|L],LL):-number(N),remove_numbers(L,LL),!.
remove_numbers([N|L],[N|LL]):-remove_numbers(L,LL),!.

%% :-include('moo_header.pl').
% This file changes Well Founded Semantic Prolog into a more normaized form of WFS-Prolog

/*
For example:

Special predicates...

		not('instance'('Fido','Cat')) to

		~'instance''('Fido','Cat'))

Holdable (Hilog) predicates...

		nextto('Fido','Cat')

		'Q'(nextto,'Fido','Cat')

Meta/Aggregate Predicates

Logical Connective Predicates are not re-literated but explored the equal way Aggregates are

Inline-Predicates (Implemented outside the scope of the Logic Engine)


All predicates have an additional property about how negation is preformed:

Eigther by failure or by explicit negative forms

Explicit negative forms are created internally by taking the root operator and appending '~' to it's name
Failure based predicates are wrapped with 'not/1' and wont be checked for explicit negation


possible(X) -> (X ; not(not(X))
consistent(X) -> (not(not(X))
forall(X) -> (X , not(not(X)) )
any(X) -> (X)
exists(X) -> (X ; not(not(X) )
never(X) -> not(X)
known(X) -> (X ; not(X))

matrix (X/not(X))  (and/or)  (not(X)/not(not(X)))


   X 
  ~X
  -X
 -~X
   X
  ~X
  -X
 -~X




X ; not(X)
not(X) ; not(X)
not(not(X)) ; not(X)
X ; not(X)
not(X) ; not(X)
not(not(X)) ; not(X)


*/


/*
:-module(
      moo_krlog,
      [krlog_to_prolog/2,prolog_to_krlog/2,pterm_to_bt/3]
      ).
*/

% ===================================================================
% krlog_to_prolog(KRProlog,Prolog)  Converts And/Or/Implies terms to Proper Prolog
% ====================================================================

krlog_to_prolog(A,A):-isSlot(A),!.
krlog_to_prolog(<=>(A,B),<=>(AA,BB)):- !,
      krlog_to_prolog(A,AA),
      krlog_to_prolog(B,BB).
krlog_to_prolog(if(A,B),':-'(AA,BB)):- !,
      krlog_to_prolog(A,AA),
      krlog_to_prolog(B,BB).
krlog_to_prolog(entails(B,A),':-'(AA,BB)):- !,
      krlog_to_prolog(A,AA),
      krlog_to_prolog(B,BB).
krlog_to_prolog(LIST,OLIST):- is_list(LIST),!,
      krlog_to_prolog_l(LIST,OLIST).
krlog_to_prolog(=>(A,B),O):- !,
      krlog_to_prolog((B:-A),O).


krlog_to_prolog(consistent(A),consistent(O)):- !,           
      krlog_to_prolog(A,O).
krlog_to_prolog(inconsistent(A),consistent(O)):- !,           
      krlog_to_prolog(not(A),O).
krlog_to_prolog(known(A),known(O)):- !,           
      krlog_to_prolog((A),O).

krlog_to_prolog(and(A,B),O):- !,
      krlog_to_prolog((A,B),O).
krlog_to_prolog(and(A,B,C),O):- !,
      krlog_to_prolog((A,B,C),O).
krlog_to_prolog(and(A,B,C,D),O):- !,
      krlog_to_prolog((A,B,C,D),O).
krlog_to_prolog(and(A,B,C,D,E),O):- !,
      krlog_to_prolog((A,B,C,D,E),O).
krlog_to_prolog(and(A,B,C,D,E,F),O):- !,
      krlog_to_prolog((A,B,C,D,E,F),O).
krlog_to_prolog(or(A,B),O):- !,
      krlog_to_prolog((A;B),O).
krlog_to_prolog(or(A,B,C),O):- !,
      krlog_to_prolog((A;B;C),O).
krlog_to_prolog(or(A,B,C,D),O):- !,
      krlog_to_prolog((A;B;C;D),O).
krlog_to_prolog(or(A,B,C,D,E),O):- !,
      krlog_to_prolog((A;B;C;D;E),O).
krlog_to_prolog(or(A,B,C,D,E,F),O):- !,
      krlog_to_prolog((A;B;C;D;E;F),O).

krlog_to_prolog(not(A),not(AA)):- !,
      krlog_to_prolog(A,AA).
krlog_to_prolog('neg'(A),'neg'(AA)):- !,
      krlog_to_prolog(A,AA).
krlog_to_prolog('not'(A),'not'(AA)):- !,
      krlog_to_prolog(A,AA).
krlog_to_prolog('not'(A),'not'(AA)):- !,
      krlog_to_prolog(A,AA).

krlog_to_prolog(','(A,B),','(AA,BB)):- !,
      krlog_to_prolog(A,AA),
      krlog_to_prolog(B,BB).

krlog_to_prolog(';'(A,B),';'(AA,BB)):- !,
      krlog_to_prolog(A,AA),
      krlog_to_prolog(B,BB).

krlog_to_prolog(':-'((A1,A2),B), O):- isNonVar(A1),!,
      krlog_to_prolog((':-'(A1,B),':-'(A2,B)),O).

krlog_to_prolog(':-'((A1;A2),B), O):-  isNonVar(A1),!,
      krlog_to_prolog((':-'(A1,B),':-'(A2,B)),O).

krlog_to_prolog(':-'(A,B),':-'(AA,BB)):- !,
      krlog_to_prolog(A,AA),
      krlog_to_prolog(B,BB).

krlog_to_prolog(COMP,OCOMP):-compound(COMP),!,
      COMP=..[F|ARGS],
      krlog_to_prolog_l(ARGS,OARGS),
      OCOMP=..[F|OARGS].

krlog_to_prolog(A,A).


krlog_to_prolog_l([],[]):-!.
krlog_to_prolog_l([HK|TK],[HP|TP]):-!,
               krlog_to_prolog(HK,HP),
               krlog_to_prolog_l(TK,TP).


% ===================================================================
% prolog_to_krlog(KRProlog,Prolog)  Converts And/Or/Implies terms to Proper Prolog
% ====================================================================

prolog_to_krlog(A,A):-isSlot(A),!.

prolog_to_krlog(LIST,OLIST):- is_list(LIST),!,
      prolog_to_krlog_l(LIST,OLIST).
prolog_to_krlog(or(B,A),or(BB,AA)):- !,
      prolog_to_krlog(A,AA),
      prolog_to_krlog(B,BB),!.
prolog_to_krlog((B:-A),'=>'(BB,AA)):- !,
      prolog_to_krlog(A,AA),
      prolog_to_krlog(B,BB).
prolog_to_krlog((B:-true),BB):- !,
      prolog_to_krlog(B,BB).
prolog_to_krlog(<=>(B,A),<=>(BB,AA)):- !,
      prolog_to_krlog(A,AA),
      prolog_to_krlog(B,BB).
prolog_to_krlog(and(B,A),and(BB,AA)):- !,
      prolog_to_krlog(A,AA),
      prolog_to_krlog(B,BB).
prolog_to_krlog(=>(B,A),=>(BB,AA)):- !,
      prolog_to_krlog(A,AA),
      prolog_to_krlog(B,BB).
prolog_to_krlog(forall(B,A),forall(B,AA)):- !,
      prolog_to_krlog(A,AA).
prolog_to_krlog(exists(B,A),exists(B,AA)):- !,
      prolog_to_krlog(A,AA).
prolog_to_krlog((A,B),and(AA,BB)):- !,
      prolog_to_krlog(A,AA),
      prolog_to_krlog(B,BB).
prolog_to_krlog((A;B),or(AA,BB)):- !,
      prolog_to_krlog(A,AA),
      prolog_to_krlog(B,BB).
prolog_to_krlog(not(not(B)),consistent(BB)):- !,
      prolog_to_krlog(B,BB).
prolog_to_krlog(not(B),not(BB)):- !,
      prolog_to_krlog(B,BB),!.
prolog_to_krlog(call(B),(BB)):- !,
      prolog_to_krlog(B,BB).

prolog_to_krlog(COMP,OCOMP):-compound(COMP),
      COMP =.. [F,P|ARGS], memberchk(F,['M','N','O','P','Q','R','holds','holds']),
      prolog_to_krlog_l([P|ARGS],[PO|OARGS]),
      OCOMP =.. [holds,PO|OARGS].

prolog_to_krlog(COMP,not(OCOMP)):-compound(COMP),
      COMP=.. [F,P|ARGS], memberchk(F,['~M','~N','~O','~P','~Q','~R','~holds','~holds']),
      prolog_to_krlog_l([P|ARGS],[PO|OARGS]),
      OCOMP =.. [holds,PO|OARGS].


prolog_to_krlog(A,A).

prolog_to_krlog_l([],[]):-!.
prolog_to_krlog_l([HK|TK],[HP|TP]):-!,
               prolog_to_krlog(HK,HP),
               prolog_to_krlog_l(TK,TP).


conj_set(CNF,Set):-
	conjunctsToList(CNF,CNFList),
	disj_list(CNFList,Set).
	
disj_list([],[]).

disj_list([CNF|List],[Set|SetL]):-!,
	or_to_list(CNF,Set),
	disj_list(List,SetL).

or_to_list(or(A,B),CC):-
	or_to_list(A,AA),
	or_to_list(B,BB),
	append(AA,BB,CC).
	
or_to_list(CNF,[CNF]).


list_to_and([A],A):-!.
list_to_and([A|L],and(A,O)):-!,
		  list_to_and(L,O).




	             	 	
fdelete([],T,[]):-!.

fdelete([Replace|Rest],[H|T],Out):-
	functor(Replace,F,_),memberchk(F,[H|T]),!,
       fdelete(Rest,[H|T],Out),!.

fdelete([Replace|Rest],[H|T],[Replace|Out]):-!,
       fdelete(Rest,[H|T],Out),!.

fdelete([Replace|Rest],F,Out):-
	functor(Replace,F,_),!,%F=FF,
       fdelete(Rest,F,Out),!.

fdelete([Replace|Rest],F,[Replace|Out]):-
       fdelete(Rest,F,Out),!.



% ===================================================================
% getCleanCharsWhitespaceProper/2.. Cleans String Up before parser uses it
% ===================================================================

getCleanCharsWhitespaceProper([],[]):-!.
getCleanCharsWhitespaceProper(X,Z) :- !,logOnFailure(ascii_clean(X,Y)),!,logOnFailure(getCleanCharsWhitespaceProper3(Y,Z)),!.

% Converts not ANSI Chars to whitespace 
ascii_clean([],[]):-!.
ascii_clean([X|String],[Y|Out]) :- transpose_char(X,Y),!,ascii_clean(String,Out).


string_clean(X,X).

transpose_char(10,32).
%transpose_char(32,32).
%transpose_char(X,32):-not(integer(X)),!.
%transpose_char(X,32):-X<33,!.
transpose_char( X , X).
   
% Blocks of Spaces are removed from a Charlist 
getCleanCharsWhitespaceProper3([],[]).
getCleanCharsWhitespaceProper3([32],[]).
getCleanCharsWhitespaceProper3([10],[]).
getCleanCharsWhitespaceProper3([13],[]).
getCleanCharsWhitespaceProper3([32,32],[]).
getCleanCharsWhitespaceProper3([32,32,32],[]).
getCleanCharsWhitespaceProper3([X],[X]):-!.
getCleanCharsWhitespaceProper3([32,32,32,32,32,32,32|String],[32|Out]) :-!, getCleanCharsWhitespaceProper3(String,Out),!.
getCleanCharsWhitespaceProper3([32,32,32,32,32|String],[32|Out]) :- !,getCleanCharsWhitespaceProper3(String,Out),!.
getCleanCharsWhitespaceProper3([32,32,32|String],[32|Out]) :-!, getCleanCharsWhitespaceProper3(String,Out),!.
getCleanCharsWhitespaceProper3([32,32|String],[32|Out]) :- !,getCleanCharsWhitespaceProper3(String,Out),!.
getCleanCharsWhitespaceProper3([X|String],[X|Out]) :- !,getCleanCharsWhitespaceProper3(String,Out),!.
getCleanCharsWhitespaceProper3(X,X):-!.



% ===========================================================
% NATIVE SOAPD SERVER FOR SWI-PROLOG
% ===========================================================

			    
%:-module(moo_soap,[]).

% :-include('moo_header.pl').

:-dynamic(xmlCurrentOpenTags/2).

serviceSoapRequest(In,Out):-
      writeSTDERR('SOAP Request'),
        catch(read_do_soap(stream(In),Out),E,
        writeFmt(Out,'<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n<error>~w</error>\n',[E])),
        catch(flush_output(Out),_,true).


read_do_soap(Source):-
        open(Source,read,Stream),
        read_do_soap(Stream,user_output).

read_do_soap(Source,Out):-
       getThread(Self),
        write(Out,'<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n'),
       % writeFmt(Out,'<?xml version="1.0" encoding="ISO-8859-1"?>\n<answer thread="~w">\n',[Self]),
        catch(flush_output(Out),_,true),
        load_structure(Source,RDF,[]),
        structure_to_options(RDF,Options),
%       writeFmt(user_error,'structure="~q"\noptions="~q"\n',[RDF,Options]),
        catch(flush_output(user_error),_,true),
        processRequest([client=soap|Options]).
        %writeFmt(Out,'</answer>\n',[]).


% request
structure_to_options([element(request, Options, [Atom])],[submit=ask,sf=Atom|Options]):-!.

% assert
structure_to_options([element(assert, Options, [Atom])],[submit=assert,sf=Atom|Options]):-!.
structure_to_options([element(asssertion, Options, [Atom])],[submit=assert,sf=Atom|Options]):-!.
structure_to_options([element(assertion, Options, [Atom])],[submit=assert,sf=Atom|Options]):-!.

% get inner
structure_to_options([element(Ptag, ['xmlns:moo'=Server], Inner)],[opt_server=Server,opt_outter=Ptag|Out]):-!,
        structure_to_options(Inner,Out).



xmlOpenTag(Name):-getThread(Self),asserta(xmlCurrentOpenTags(Self,A)),writeFmtServer('<~w>',[Name]),!.
xmlOpenTagW(Out,Name,Text):-getThread(Self),asserta(xmlCurrentOpenTags(Self,A)),writeFmtServer(Out,'~w',[Text]),!.

xmlCloseTag(Name):-getThread(Self),ignore(retract(xmlCurrentOpenTags(Self,A))),writeFmtServer('</~w>',[Name]),!.
xmlCloseTagW(Name,Text):-getThread(Self),ignore(retract(xmlCurrentOpenTags(Self,A))),writeFmtServer('~w',[Text]),!.
xmlCloseTagW(Out,Name,Text):-getThread(Self),ignore(retract(xmlCurrentOpenTags(Self,A))),writeFmtServer(Out,'~w',[Text]),!.

xmlClearTags:-getThread(Self),retractall(xmlCurrentOpenTags(Self,A)).

xmlExitTags:-getThread(Self),retract(xmlCurrentOpenTags(Self,A)),writeFmtServer('</~w>',[Name]),fail.
xmlExitTags.


% ===========================================================
% Insert
% ===========================================================
parse_moo_soap(Options):-memberchk(submit=assert,Options),!,
        getMooOption(opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(opt_theory='PrologMOO',Context),
        getMooOption(sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        getMooOption(user='Web',User),
        getMooOption(interp='kif',Interp),
        logOnFailure(getMooOption(tn=_,EXTID)),
        %sendNote(user,'Assert',formula(NEWFORM),'Ok.'). %,logOnFailure(saveMooCache)
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        xml_assert(Show,Ctx,Context,User).

xml_assert(Show,Ctx,Context,User):-
        getSurfaceFromChars(Show,STERM,Vars),
        getMooTermFromSurface(STERM,NEWFORM),
        xml_assert(Show,NEWFORM,Vars,Ctx,Context,User).

xml_assert(Show,Ctx,Context,User):-!,
        writeFmt('<assertionResponse accepted="false">\nUnable to parse: "~s"\n</assertionResponse>\n',[Show]).

xml_assert(Show,NEWFORM,Vars,Ctx,Context,User):-
        logOnFailure(getTruthCheckResults(tell,[untrusted],surface,NEWFORM,Ctx,STN,Context,Vars,Maintainer,Result)),
        (Result=accept(_) ->
                        (
                        once(invokeInsert([trusted,canonicalize,to_mem],surface,NEWFORM,Ctx,EXTID,Context,Vars,User)),
                        write('<assertionResponse accepted="true">\nOk.\n</assertionResponse>\n')
                        )
                        ;
                        (
                        Result=notice(FormatStr,Args),
                        write('<assertionResponse accepted="false">\n'),
                        writeFmt(FormatStr,Args),
                        write('\n</assertionResponse>\n')
                        )
        ),!.

xml_assert(Show,NEWFORM,Vars,Ctx,Context,User):-!.


% ===========================================================
% Ask a Request
% ===========================================================
parse_moo_soap(Options):-memberchk(submit=ask,Options),!,make,
        %write('<!DOCTYPE moo:ask SYSTEM "/opt/tomcat-4.0/webapps/moo-1.4b1/dtd/java_prolog.dtd">\n'),
        write('<moo:ask xmlns:moo="http://localhost">\n'),
        getMooOption(opt_ctx_request='GlobalContext',Ctx),
        getMooOption(opt_theory='PrologMOO',Context),
        getMooOption(sf=surf,Askion),
        atom_codes(Askion,Askion_Chars),
        getMooOption(user='Web',User),
        getMooOption(interp='kif',Interp),
         logOnFailure(getCleanCharsWhitespaceProper(Askion_Chars,Show)),!,
         logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
         logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
              logOnFailure(once(( NEWFORM=comment(_) ->
                     (writeFmt('<error>Syntax Error: Unmatched parentheses in "~s"</error>\n',[Show]),!,FORM=_) ;(!,
                     logOnFailure(invokeRequest_xml(NEWFORM,ChaseVars,Ctx,TrackingAtom,Context,User,Vars,CPU))
                     )))),
        write('</moo:ask>\n').

invokeRequest_xml(NEWFORM,ChaseVars,Ctx,TrackingAtom,Context,User,Vars,CPU):-
        invokeRequestToBuffer(NEWFORM,ChaseVars,Ctx,TrackingAtom,Context,User,Vars,CPU),
        final_answer(Logic:How),
        invoke_final_answer(Logic,How,CPU).

invoke_final_answer(possible,How,CPU):-!,
        writeFmt('<requestResponse yesno="~w" numBindings="0" seconds="~w"/>\n',[How,CPU]).

invoke_final_answer(Logic,How,CPU):-
        writeFmt('<requestResponse yesno="~w" numBindings="~w" seconds="~w">\n<bindings>\n',[Logic,How,CPU]),
        cite_xml_buffered_answers,
        write('</bindings>\n</requestResponse>\n').


cite_xml_buffered_answers:-
        retract(requestBuffer_db(UResultsSoFar,Result,Explaination,Status)),
        once(inform_xml_agent(UResultsSoFar,Result,Explaination,Status)),fail.

% Call to write Summary
/*
cite_xml_buffered_answers:-
        final_answer(Logic:How),
        writeDebug(final_answer(Logic:How)),
        inform_xml_agent(How, ['Summary'=Logic|_G14093],final_answer(Logic:How),final_answer(Logic:How) ).
*/
cite_xml_buffered_answers:-!.

% ===========================================================
% Send to debugger
% ===========================================================
inform_xml_agent(UResultsSoFar,Result,InExplaination,Status):-
        writeDebug(inform_xml_agent(UResultsSoFar,Result,InExplaination,Status)),fail.

% ===========================================================
% Hide certain returns
% ===========================================================
inform_xml_agent(-1,Result,Explaination,Status).

inform_xml_agent(0, ['Result'=none|A], 'Unproven', done(possible:searchfailed)).
inform_xml_agent(_, ['Result'=true|A], found(_), done(true:_)).
inform_xml_agent(_, ['Summary'=_|_G5892], _, _).

% ===========================================================
% Write Answers
% ===========================================================
inform_xml_agent(UResultsSoFar,Result,InExplaination,Status):-
        writeFmt('<binding>\n',[]),
        inform_xml_vars(Result,Vars),
        length_explaination(InExplaination,InLength),
        findall(Length-Explaination,
                (retract(inform_xml_agent_buffer_db(_,Result,Explaination,_)),
                 length_explaination(Explaination,Length)
                 ),KeyList),

        keysort([(InLength-InExplaination)|KeyList],[(_-ChoiceExplaination)|_]),
        inform_xml_explaination(InLength,ChoiceExplaination,Result),
        writeFmt('</binding>\n',[]).

inform_xml_vars(Result,Vars):-
        length_var(Result,NumVar),
        writeFmt('<variables numVars="~w">\n',[NumVar]),
        inform_each_variable(Result,Vars),
        writeFmt('</variables>\n',[]).

length_var([],0).
length_var([A|'$VAR'(_)],1).
length_var([A|L],N):-
          length_var(L,NN),
          N is NN +1.

inform_each_variable([],Vars).
inform_each_variable('$VAR'(_),Vars).
inform_each_variable([NV|Rest],Vars):-
        inform_nv(NV,Vars),
        inform_each_variable(Rest,Vars).


inform_nv('$VAR'(_),Vars).
inform_nv(Name=Value,Vars):-
        toMarkUp(kif,Name,Vars,OName),
        toMarkUp(kif,Value,Vars,OValue),
        writeFmt('<var varName="~w" value="~w"/>\n',[OName,OValue]).


inform_xml_explaination(InLength,ChoiceExplaination,Result):-
        writeFmt('<explaination numSteps="~w">',[InLength]),
        flag(explaination_linenumber,_,0),
        writeObject_explaination(ChoiceExplaination,Result),
        writeFmt('</explaination>\n').

writeObject_explaination(deduced,_).
writeObject_explaination('$VAR'(_),_).
writeObject_explaination(explaination(Choice1) ,Result):-!,
        writeObject_explaination(Choice1,Result),!.
writeObject_explaination(Choice1 * Choice2 ,Result):-!,
        writeObject_explaination(Choice1,Result), !,
        writeObject_explaination(Choice2,Result),!.
writeObject_explaination(Choice1,Result):-!,
             write('<explainationStep isRule="true">\n<originalRule>\n'),
             toMarkUp(html,Choice1,Result,Out),!,
             ignore(write_escaped(Out)),
             write('\n</originalRule>\n</explainationStep>\n').

write_escaped([O|T]):-!,
        write_e_codes([O|T]),!.
write_escaped(Out):-atom(Out),!,
        atom_codes(Out,Codes),!,write_escaped(Codes),!.
write_escaped(String):- !,
        string_to_atom(String,Atom),
         atom_codes(Atom,Codes),!,
        write_e_codes(Codes),!.

write_e_codes([]):-!.
write_e_codes([E|Cs]):-!,
        write_e(E),!,
        write_e_codes(Cs),!.
write_e(34):-write('&qt;'),!.
write_e(60):-write('&lt;'),!.
write_e(62):-write('&gt;'),!.
write_e(C):-put_code(C),!.




