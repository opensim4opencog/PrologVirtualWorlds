
:- load_foreign_library(alarm).

%:- (current_prolog_flag(arch,'i386-win32') -> 
%     on_signal(2, throw,throw).
%	on_signal(alrm, throw,throw)).

%:-	on_signal(int, _throw,write).
%:-	on_signal(2, _throw,write).
%:-	on_signal(4, _throw,write).

:- module_transparent
        alarm/1,string_clean/2.
        


:-dynamic(cpuend/1).


/*
call_with_time_limit(Goal, MaxTime) :-
        alarm(MaxTime),
        catch(Goal, error(signal(alrm, _), _), fail), 
        alarm(0).
call_with_time_limit(_, _) :-
        alarm(0),
        fail.

call_with_time_limit(Goal, MaxTime,Result) :-
        alarm(MaxTime),
        catch((Goal,Result=true), error(signal(alrm, _), _), (Result=timed_out,!)),alarm(0).
        

call_with_time_limit(_, _ , false) :- alarm(0).

*/


