end_of_file.
:-make.


protrace(X):-catch((atom_to_term(X,Prolog,_),stack_prolog(Prolog)),Error,post(Error)).

get_goal:-get_goal(X),write(X).
do_goal:-callnext.


:-assert(protected).

:-assert(next_goal(stack_empty)).

callp(true):-!.
callp(protect):-!,retractall(protected),assert(protected).
callp(unprotect):-!,retractall(protected).
callp(X):-not(protected),!,X.

callp((X,Y)) :- !,callp(X) , stack_prolog(Y).
callp((X;Y)) :- !,callp(X) ; stack_prolog(Y).
callp(X) :- nonvar(X),X=not(Y),!,not(callp(Y)).

callp(X):-nonvar(X),built_in(X),!,syscall(X).
callp(X):-!,clause(X,Y),stack_prolog(Y) . % , callnext.
callp(_):-!,fail.

get_goal(X):-next_goal(X),!.
get_goal(stack_empty).

stack_prolog(X):-assertz(next_goal(X)).

callnext:- retract(next_goal(X)),!, reallycallp(X).
callnext:- write(stack_empty),nl.

reallycallp(X):-callp(X).

syscall(X):-catch(X,Error,(!,write(Error),fail)).

:- dynamic(built_in/1).

built_in(X):-predicate_property(X,built_in).

:-dynamic(result/1).

plquery(AtomTerm):-catch( (atom_to_term(AtomTerm,Prolog,_) ,Prolog),Error,post(Error)).

getresult(X):-retract(result(X)).

post(X):-term_to_atom(X,R),assertz(result(R)).
