% ===================================================================
% PURPOSE
% This File is the bootstrap for the Moo Infence engine For SWI-Prolog
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




% =====================================
% supplies hook specific architecture
% =====================================
:- redefine_system_predicate(with_mutex(_,_)).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

%:-use_module(library(threadutil)).        
system_dependant:prolog_thread_create(X,win32,_):-X.
prolog_thread_exit(_,_):-!.
system_dependant:prolog_thread_at_exit(_,_):-!.
system_dependant:prolog_thread_at_exit(_):-!.
getThread(win32):-!.
prolog_thread_exit(_):-!.
threads:-!. %system_dependant:prolog_statistics.

mutex_unlock_all.

mutex_create(_).
:- redefine_system_predicate(mutex_lock/2).
mutex_lock(_,_).
mutex_lock(_).
with_mutex(_,G):-G.
mutex_unlock(_,_).
mutex_unlockall.


system_dependant:prolog_current_thread(main,running).

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

:-dynamic(cpuend/1).

call_with_time_limit(Goal,Time,New):-
	retractall(cpuend(_)),
	getCputime(Now),
	Then is Now + Time,
	asserta(cpuend(Then)),!,Goal,
	getCputime(NewNow),New is Now-NewNow.
%call_with_time_limit(Goal,Time,timeout):-!.


/*


% ==============================================
%	Create the ADO connection
% ==============================================

adoConnect :-!.
adoConnect :- adoobj(connection,IP),!.

adoConnect :-
      ado_cnnstr(Connection),
 %     Connection="Driver={SQL Server};Server=MBUILD;UID=sa;PWD=;Database=MooWFS"',
      %sendNote(debug,contentManager,'Making ODBC Connecton',Connection),
		actx_errors_as_exceptions(true),
      catch((
            actx_create_object('ADODB.Connection',IPConn),
      		assert(adoobj(connection,IPConn)),
            actx_invoke_object(IPConn,'Open',[Connection],_),
      		actx_create_object('ADODB.Recordset',IPRset),
      		actx_create_object('ADODB.Command',IPCmd),
      		assert(adoobj(recordset,IPRset)),
      		assert(adoobj(command,IPCmd)),
      		adoobj(connection,IPConn),
      		actx_invoke_object(IPRset,'Open',['PrologMemory',IPConn],_),
            assert(ado_connected)
      ),E, sendNote(debug,contentManager,'ODBC Connecton Failed',E) ),
            actx_errors_as_exceptions(false).

			

			

adoUpdate_odbc(NO_ODBC_TODO,String,Context,TN,Maintainer):-!.
adoUpdate_odbc(auth,String,Context,TN,Maintainer):-!,
      execSQLf('Insert into PrologMemory (AssertionID,SourceForm,SourceText,KnowledgeBase,Context,Creator) VALUES ("~q","auth","~s","~q","~q","~q")',[TN,String,Context,Maintainer]).
adoUpdate_odbc(Form,CL,Context,TN,Maintainer):- !, 
      execSQLf('Insert into PrologMemory (AssertionID,SourceForm,SourceText,KnowledgeBase,Context,Creator) VALUES ("~q","~q","~q","~q","~q","~q")',[TN,Form,CL,Context,Maintainer]).

%adoUpdate_memory(Form,CL,Context,TN,Maintainer):-not('in-active-memory'(Context,Ctx)),!.
adoUpdate_memory(prolog,CL,Context,TN,Maintainer):-
            unnumbervars(CL,UNCL),logOnFailure(assert(UNCL)).
            %sendNote(extreme_debug,contentManager,'asserted to working memory',CL).

adoUpdate_memory(Form,CL,Context,TN,Maintainer):-!.

% ==============================================
%	Retrieve from ADO
% ==============================================
purge_entire_theory(Context):-retractall(mooCache(PredR,Form,Source,Prolog,Context,TN,Maintainer,_)),execSQLf('DELETE FROM PrologMemory WHERE KnowledgeBase LIKE "~q"',[Context]) .



get_store(cache_only,Form,Source,Context,TN,Maintainer):-mooCache(PredR,Format,PrologFormS,CL,Context,TN,Maintainer,_).

get_store(odbc,Form,Source,Context,TN,Maintainer):-
         once(select_rs(Form,Source,Context,TN,Maintainer,IPRset)),
         once(save_rs(IPRset,mooCache)),
         actx_release_object(IPRset),
         fail.
get_store(odbc,Form,Source,Context,TN,Maintainer):-mooCache(PredR,Form,Source,Context,TN,Maintainer).

:-dynamic(wc/2).

%sync_ado_cache:-!.
sync_ado_cache:-
        adoConnect,
         retractall(mooCache(PredR,_,_,_,_,_,_)),
         sendNote(power_user,contentManage,'Refreshing ADO/ODBC Cache',' '),
         once(select_rs(Form,Source,Context,TN,Maintainer,IPRset)),
         once(save_rs(IPRset,mooCache)),
         actx_release_object(IPRset).
sync_ado_cache:-!.
         

select_rs(Form,Source,Context,TN,Maintainer,IPRset):-         
         retractall(wc(_,_)),
         (var(Form) -> true ; assert(wc('SourceForm',Form))),
         (var(Context) -> true ; assert(wc('KnowledgeBase',Context))),
         (var(Ctx) -> true ; assert(wc('Context',Ctx))),
         (var(TN) -> true ; assert(wc('AssertionID',TN))),
         (var(Maintainer) -> true ; assert(wc('Creator',Maintainer))),
         get_where_clause(WC),
         execSQLf('SELECT SourceForm,SourceText,KnowledgeBase,Context,AssertionID,Creator FROM PrologMemory WHERE ~w Retracted=False ',[WC],IPRset).

get_where_clause(WC):-retract(wc(N,V)),get_where_clause(Next),fmtString(WC,'~w ~w LIKE "~q" AND ',[Next,N,V]).
get_where_clause(' ').   

save_rs(IPRset,mooCache):-
		      actx_invoke_object(IPRset,'moveFirst',[],_),
            save_rs_until_end(IPRset).
            
save_rs_until_end(IPRset):-save_rs_until_end_0(IPRset),!,save_rs_until_end(IPRset).
save_rs_until_end(IPRset).

save_rs_until_end_0(IPRset):-
            actx_invoke_object(IPRset,['EOF',propget],[],false),
            actx_invoke_object(IPRset,'GetString',[2,1,',','  ','_'],Value),!,
            catch(fmtString(S,'mooCache(PredR, ~s )',[Value]),_,true),
            %catch((atom_to_term(S,Term,V),assert(Term)),_,save_rs_until_end_1(IPRset)).
            catch((atom_to_term(S,Term,V),assert(Term)),_,true).

save_rs_until_end_1(IPRset):-
            actx_invoke_object(IPRset,['Fields',propget],[],IPFields),
            get_name_value(IPFields,'AssertionID',AssertionID),
            get_name_value(IPFields,'SourceForm',SourceForm),
            get_name_value(IPFields,'SourceText',SourceText),
            get_name_value(IPFields,'KnowledgeBase',KnowledgeBase),
            get_name_value(IPFields,'Context',Context),
            get_name_value(IPFields,'Creator',Creator),
            assert(mooCache(PredR,SourceForm,SourceText,KnowledgeBase,Context,AssertionID,Creator)),
		      actx_invoke_object(IPRset,'MoveNext',[],_).


get_name_value(IPFields,Name,Value):-
            actx_invoke_object(IPFields,['Item','propget'],[Name],IPField1),
            actx_invoke_object(IPField1,['Value','propget'],[],Value),
            actx_release_object(IPField1). %,char_code(ValueA,Value).
            



% ==============================================
%	execSQL % 259-6225
% ==============================================
      
execSQL(Cmd):-
		execSQL(Cmd,IPRset),!,actx_release_object(IPRset),!.

execSQL(Cmd,IPRset):-
		adoobj(connection,IPConn),!,
		actx_invoke_object(IPConn,['execute',func],[Cmd],IPRset),!.

execSQLf(Format,Args):-
      fmtString(S,Format,Args),
      string_to_atom(S,A),
      execSQL(A),!.

execSQLf(Format,Args,IPRset):-
      fmtString(S,Format,Args),
      string_to_atom(S,A),
      execSQL(A,IPRset),!.
		
actx_run(RUN):-
      actx_errors_as_exceptions(true),
		catch(RUN,
				activex_error(StrFunc,StrDesc,TermInError),
				(sendNote(debug,contentManger,['ActiveX Error: ',StrFunc],[StrDesc,TermInError]),actx_errors_as_exceptions(true))),actx_errors_as_exceptions(true).

                      


*/

