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


