:-include('logicmoo_utils_header.pl').
% =================================================
% Useragent Control Panel
% =================================================
clientEvent(Channel,Agent,Options):-memberchk(file='system.moo',Options),
        memberchk(killable=_,Options),!,
        member(killable=ID,Options),
        catch(prolog_thread_signal(ID,prolog_thread_exit(user_killed(ID))),_,true),
        clientEvent(Channel,Agent,[file='system.moo']).
clientEvent(Channel,Agent,[file='system.moo']):-
      writeHTMLStdHeader('System Statistics'),
      showMooProcessHTML,!,
      writeHTMLStdFooter.


logOnFailure(X):-catch(X,E,(say(error(X,E)),!,fail)),!.
logOnFailure(X):-say(falure(X)),!,fail.

% This Function when called from outside does the Embeding the Logic Engine into the XSB Server
initializeMooServerData :-!,
      findall(L,library_directory(L),Library_directory),
      sendNote(user,logicEngine,'Initializing and Embedding Moo Logic Engine',Library_directory),
      establish_startup_state,
      sendNote(debug,logicEngine,'Contexts are now loaded by contentManager',' '),
      ensureMooContext('MooKernel','GlobalContext'),!.


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
:-module(cyc_threads,
      [ 
      	 servantProcessCreate/1,
	 servantProcessCreate/3,
	 servantProcessCreate/4,
	 servantProcessCreate/5,
	 isMooProcess/2,
	 isMooProcess/5,
	 createProcessedGoal/1,
	 servantProcessSelfClean/0,
	 showMooStatisticsHTML/0,
	 cleanOldProcesses/0,
	 showMooStatisticsHTML/0]).
  */
% :-include('cyc_header.pl').

:-dynamic(isMooProcess/5).


createProcessedGoal(Goal):-
      servantProcessCreate((prolog_thread_at_exit((
	 (thread_self(Id),prolog_thread_exit(i_am_done(Id))))),Goal),Id,[]).


servantProcessCreate(Perms,Name,Goal,Id,Options):-
        prolog_thread_create((prolog_thread_at_exit(servantProcessSelfClean),Goal),Id,Options),
        asserta(isMooProcess(Perms,Name,Goal,Id,Options)).

servantProcessCreate(Name,Goal,Id,Options):-
        prolog_thread_create((prolog_thread_at_exit(servantProcessSelfClean),Goal),Id,Options),
        asserta(isMooProcess(killable,Name,Goal,Id,Options)).

servantProcessCreate(Goal,Id,Options):-
        prolog_thread_create((prolog_thread_at_exit(servantProcessSelfClean),Goal),Id,Options),
        asserta(isMooProcess(killable,thread(Id),Goal,Id,Options)).

servantProcessCreate(Goal):-
        servantProcessCreate(Goal,_Id,[detach(true)]).
                                % so what was mentioned with logicmoo?
isMooProcess(ID,Goal):-
        isMooProcess(_,_,Goal,ID,_).

debugProcess(T):-
	prolog_thread_signal(T, (attach_console,trace, true)).


servantProcessSelfClean:-
      thread_self(Id),
      retractall(isMooProcess(_Perms,_Name,_Goal,Id,_Options)).

% ===================================================================
% Prolog Dependant Code
% ===================================================================

    
/*
:-module(system_dependant,
      [getCputime/1,
      numbervars/1,
      unNVar/2,
      writeSTDERR/1,
      writeSTDERR/2,
      say/1,
      say/2,
      say/3,
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



% ========================================================================================
% Using getCputime/1 (in Cyc code) since Eclipse prolog (another port for Cyc)  chokes on getCputime/1
% ========================================================================================
getCputime(Start):-statistics(cputime,Start).
prolog_statistics:-statistics.
prolog_notrace(G):-notrace(G).

% ========================================================================================
% Threads 
% ========================================================================================
prolog_thread_create(Goal,Id,Options):-
	 (current_prolog_flag(threads,true) -> thread_create(Goal,Id,Options) ; Goal).
prolog_current_thread(Id,Status):-
	 (current_prolog_flag(threads,true) -> current_thread(Id,Status) ; (Id=main,Status=running)).	 
prolog_thread_exit(Goal):-
	 (current_prolog_flag(threads,true) -> thread_exit(Goal) ; true).
prolog_thread_self(Id):-
	 (current_prolog_flag(threads,true) -> thread_self(Id) ; Id=main).
prolog_thread_at_exit(Goal):-
	 (current_prolog_flag(threads,true) -> thread_at_exit(Goal) ; true).
prolog_thread_signal(ID,Goal):-thread_signal(ID,Goal).
prolog_thread_join(Id,X):-thread_join(Id,X).


% ========================================================================================
% Some prologs have a printf() type predicate.. so I made up fmtString/say in the Cyc code that calls the per-prolog mechaism
% in SWI it's formzat/N and sformat/N
% ========================================================================================
:-dynamic(isConsoleOverwritten/0).
writeDebug(_,_,_).
writeDebug(_,_).
writeDebug(_).

fmtString(X,Y,Z):-sformat(X,Y,Z).
fmtString(Y,Z):-sformat(Y,Z).

saveUserInput:-retractall(isConsoleOverwritten),catch(flush_output,_,true).
writeSavedPrompt:-not(isConsoleOverwritten),!.
writeSavedPrompt:-catch(flush_output,_,true).
writeOverwritten:-isConsoleOverwritten,!.
writeOverwritten:-assert(isConsoleOverwritten).

writeFmtFlushed(Channel,Format,Args):-catch((format(Channel,Format,Args),flush_output(Channel)),_,true).
writeFmtFlushed(Format,Args):-catch((format(Format,Args),flush_output),_,true).
writeFmtFlushed(Format):-catch((format(Format,[]),flush_output),_,true).

writeSTDERR(F):-writeSTDERR('~q',[F]).
writeSTDERR(F,A):-((
        format(user_error,F,A),
        nl(user_error),
        flush_output(user_error))).

writeErrMsg(Out,E):-!. %message_to_string(E,S),writeFmtFlushed(Out,'<prolog:error>~s</prolog:error>\n',[S]),!.
writeErrMsg(Out,E,Goal):-!. %message_to_string(E,S),writeFmtFlushed(Out,'<prolog:error>goal "~q" ~s</prolog:error>\n',[Goal,S]),!.
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

%numbervars(X):-get_time(T),convert_time(T,A,B,C,D,E,F,G),!,numbervars(X,'$VAR',G,_).
%unNVar(X,Y):-term_to_atom(X,A),atom_to_term(A,Y,_).


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

prologAtInitalization(V):-at_initialization(V),!,logOnFailureIgnore(V).





getPrettyDateTime(String):-get_time(Time),convert_time(Time, String).


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


showMooStatisticsHTML:-
   say('<pre>'),prolog_statistics,say('</pre>').

showMooProcessHTML:-
        showMooStatisticsHTML,
        say('<hr><table border=1 width=80%><th>Id</th><th>Name</th><th>Status</th><th>Actions</th><th>Options</th><th>Goals</th>'),
        ignore((prolog_current_thread(Id,Status),
        isMooProcess(Perms,Name,Goal,Id,Options),
        writeMooProcessesHTML(Perms,Name,Goal,Id,Options,Status),
        fail)),say('</table>'),!.


writeMooProcessesHTML(nokill,Name,Goal,Id,Options,Status):-
        say('<tr><td>~w</td><td><nobr>~w</td><td>~w</td><td>&nbsp;</a></td><td>~w</td><td>~w</td><tr>\n ',[Id,Name,Status,Options,Goal]),!.

writeMooProcessesHTML(Perms,Name,Goal,Id,Options,Status):-
        say('<tr><td>~w</td><td><nobr>~w</td><td>~w</td><td><A href="system.moo?killable=~w">Kill</a></td><td>~w</td><td>~w</td><tr>\n ',[Id,Name,Status,Id,Options,Goal]),!.

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


rememberFact(X):-catch(X,_,fail).
rememberFact(X):-assertz(X).

throwMoo(Module,Type,Details):-
        current_prolog_flag(debug_on_error, DebugOnError),
        set_prolog_flag(debug_on_error, false),!,
        throw(mooException(Module,Type,Details,DebugOnError)),
        ifInteractive(writeDebug('Post throwMoo')),!.


% ===============================================================================================
% ===============================================================================================


bot(ps):-!,mooThreadCreate_data(Perms,Name,Goal,Id,Options),
	prolog_current_thread(Id,Status),
	say((Id:Name:Goal:(Status):(Perms):Options)),fail.
bot(killable(Id)):-nonvar(Id),mooThreadCreate_data(killable,Name,Goal,Id,Options),
	prolog_thread_signal(Id,prolog_thread_exit(killed)).

% ===============================================================================================
% ===============================================================================================
		
yn(P):-catch(P,E,say(E)),!,say(yes).
yn(P):-say(no).

timeit(X):-getCputime(S),yn(X),getCputime(E),Time is E-S,say(Time).
mtry(G):-catch(G,E,say(E)).

listpr(P/A):-
	functor(H,P,A),!,
	listpr(H),!.	
listpr(H):-
	clause(H,Y),
		(Y=true -> sayQ(H);sayQ((H:-Y))),fail.
listpr(H).		

pp(P/A):-atom_concat('#$',_,P),AA is A+2,functor(H,P,AA),!,pp(H),!.	
pp(P/A):-functor(H,P,A),!,pp(H),!.	
pp(H):-findall(Y,predicate_property(H,Y),L),L=[_|_],say(L),!.

pp(H):-say(nv(['i don''t know about',H])),!.

