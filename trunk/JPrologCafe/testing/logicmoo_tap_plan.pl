/*

Re-Implementaion of the WARPLAN System (Dr. Warren 1974)
        Douglas Miles 1998

*/


% make_script(Start,Goal,Scripts)

make_scripts(_,CurrentPropValues,impossible):-
        unless(consistent_to(CurrentPropValues,true)), !.

make_scripts(Scripted,CurrentPropValues,ScriptMore):-
        good_script(CurrentPropValues,true,Scripted,ScriptMore).

%//------------------------------------------------------------------------------

make_scripts(CurrentPropValues,_) :-
        unless(consistent_to(CurrentPropValues,true)), !,
        write('impossible').

make_scripts(CurrentPropValues,Scripted) :-
        good_script(CurrentPropValues,true,Scripted,ScriptMore),
        writeDebug(make_scripts,ScriptMore).

%//------------------------------------------------------------------------------

good_script(PropValues & CurrentPropValues,NextGoalPropValues,Scripted,ScriptX) :- !,
        script_solves(PropValues,NextGoalPropValues,Scripted,GoalPropValuesFuture,ScriptMore),
        good_script(CurrentPropValues,GoalPropValuesFuture,ScriptMore,ScriptX).

good_script(PropValues,NextGoalPropValues,Scripted,ScriptMore) :-
        script_solves(PropValues,NextGoalPropValues,Scripted,_,ScriptMore).

%//------------------------------------------------------------------------------

script_solves(PropValues,NextGoalPropValues,Scripted,NextGoalPropValues,Scripted) :-
        readonly_state(PropValues).

script_solves(PropValues,NextGoalPropValues,Scripted,GoalPropValuesFuture,Scripted) :-
        does_achieve(PropValues,Scripted),
        and(PropValues,NextGoalPropValues,GoalPropValuesFuture).

script_solves(PropValues,NextGoalPropValues,Scripted,PropValues & NextGoalPropValues,ScriptMore) :-
        enable_state_with_method(PropValues,Proposed_Method),
        good_method(PropValues,Proposed_Method,NextGoalPropValues,Scripted,ScriptMore).

%//------------------------------------------------------------------------------

good_method(_,Proposed_Method,NextGoalPropValues,Scripted,ScriptMore:Proposed_Method ) :-
        preserves(Proposed_Method,NextGoalPropValues),
        enabled_method_from_state(Proposed_Method,CurrentPropValues),
        consistent_to(CurrentPropValues,NextGoalPropValues),
        good_script(CurrentPropValues,NextGoalPropValues,Scripted,ScriptMore),
        preserves(Proposed_Method,NextGoalPropValues).

good_method(PropValues,Proposed_Method,NextGoalPropValues,Scripted:Method,ScriptMore:Method) :-
        is_preserved_durring(PropValues,Method),
        rollback(NextGoalPropValues,Method,GoalPropValuesFuture),
        good_method(PropValues,Proposed_Method,GoalPropValuesFuture,Scripted,ScriptMore),
        is_preserved_durring(PropValues,Method).

%//------------------------------------------------------------------------------

does_achieve(PropValues,_:Method) :- enable_state_with_method(PropValues,Method).

does_achieve(PropValues,Scripted:Method) :- !,
        is_preserved_durring(PropValues,Method),
        does_achieve(PropValues,Scripted),
        is_still_preserved_durring( PropValues, Method ).

does_achieve(PropValues,Scripted) :- initial_given_state(Scripted,PropValues).

%//------------------------------------------------------------------------------

is_still_preserved_durring(PropValues,Method) :- is_preserved_durring(PropValues,Method).

%//------------------------------------------------------------------------------

is_preserved_durring(PropValues,Method) :-
        imagine(PropValues & Method,0,_),
        disabled_state_with_method(PropValues,Method), !, fail.

is_preserved_durring(_,_).

%//------------------------------------------------------------------------------

preserves(Proposed_Method,PropValues & CurrentPropValues) :-
        is_preserved_durring(PropValues,Proposed_Method),
        preserves(Proposed_Method,CurrentPropValues).

preserves(_,true).

%//------------------------------------------------------------------------------

rollback(NextGoalPropValues,Method,GoalPropValues) :-
        enabled_method_from_state(Method,CurrentPropValues),
        rollback1(NextGoalPropValues,Method,CurrentPropValues,GoalPropValuesFuture),
        conjoin_script(CurrentPropValues,GoalPropValuesFuture,GoalPropValues).

rollback1(PropValues & NextGoalPropValues,Method,CurrentPropValues,GoalPropValuesFuture) :-
        enable_state_with_method(OtherPropValues,Method),
        equivalent(PropValues,OtherPropValues), !,
        rollback1(NextGoalPropValues,Method,CurrentPropValues,GoalPropValuesFuture).

rollback1(PropValues & NextGoalPropValues,Method,CurrentPropValues,GoalPropValuesFuture) :-
        element_of(OtherPropValues,CurrentPropValues),
        equivalent(PropValues,OtherPropValues), !,
        rollback1(NextGoalPropValues,Method,CurrentPropValues,GoalPropValuesFuture).

rollback1(PropValues & NextGoalPropValues,Method,CurrentPropValues,PropValues & GoalPropValuesFuture) :-
        rollback1(NextGoalPropValues,Method,CurrentPropValues,GoalPropValuesFuture).

rollback1(true,_,_,true).

%//------------------------------------------------------------------------------

consistent_to(CurrentPropValues,NextGoalPropValues) :-
        imagine(CurrentPropValues & NextGoalPropValues,0,_),
        impossible(PropValues),
        unless(unless(intersection_of(CurrentPropValues,PropValues))),
        implied(PropValues,CurrentPropValues & NextGoalPropValues),
        !, fail.

consistent_to(_,_).

% and

%//------------------------------------------------------------------------------

and(PropValues,NextGoalPropValues,NextGoalPropValues) :-
        element_of(OtherPropValues,NextGoalPropValues),
        equivalent(PropValues,OtherPropValues), !.

and(PropValues,NextGoalPropValues,PropValues & NextGoalPropValues).

%//------------------------------------------------------------------------------

conjoin_script(PropValues & CurrentPropValues,NextGoalPropValues,PropValues & GoalPropValuesFuture) :- !,
        conjoin_script(CurrentPropValues,NextGoalPropValues,GoalPropValuesFuture).

conjoin_script(PropValues,NextGoalPropValues,PropValues & NextGoalPropValues).

%//------------------------------------------------------------------------------

element_of(PropValues,OtherPropValues & _) :- element_of(PropValues,OtherPropValues).

element_of(PropValues,_ & CurrentPropValues) :- !, element_of(PropValues,CurrentPropValues).

element_of(PropValues,PropValues).

%//------------------------------------------------------------------------------

intersection_of(LeftHandPropValues,RightHandPropValues) :-
        element_of(PropValues,LeftHandPropValues),
        element_of(PropValues,RightHandPropValues).

%//------------------------------------------------------------------------------

implied(LeftHandPropValues & RightHandPropValues,CurrentPropValues) :- !,
        implied(LeftHandPropValues,CurrentPropValues),
        implied(RightHandPropValues,CurrentPropValues).

implied(PropValues,CurrentPropValues) :- element_of(PropValues,CurrentPropValues).

implied(PropValues,_) :- catch2(PropValues).

implied(PropValues,_) :- instance_known(PropValues).

%//------------------------------------------------------------------------------

notequal(PropValues,OtherPropValues) :-
        unless(PropValues=OtherPropValues),
        unless(PropValues=imaginary(_)),
        unless(OtherPropValues=imaginary(_)).

%//------------------------------------------------------------------------------

equivalent(PropValues,OtherPropValues) :- unless(nonequivalent(PropValues,OtherPropValues)).

%//------------------------------------------------------------------------------

nonequivalent(PropValues,OtherPropValues) :-
        imagine(PropValues & OtherPropValues,0,_),
        PropValues=OtherPropValues, !, fail.

nonequivalent(_,_).

%//------------------------------------------------------------------------------

imagine(imaginary(N1),N1,N2) :- !, N2 is N1+1.

imagine(imaginary(_),N1,N1) :- !.

imagine(PropValues,N1,N2) :- try(PropValues =.. [_|PropValuesList]), multi_imagine(PropValuesList,N1,N2).

%//------------------------------------------------------------------------------

multi_imagine([PropValues|PropValuesList],N1,N3) :- imagine(PropValues,N1,N2), multi_imagine(PropValuesList,N2,N3).
multi_imagine([],N1,N1).

%//------------------------------------------------------------------------------

unless(PropValues) :-  catch2(PropValues), !, fail.
unless(_).

%//------------------------------------------------------------------------------

make_ground(X):-imagine(X,0,_).

make_ground(X,Y):-X1=X,copy_term(X1,Y1),imagine(Y1,0,_),Y=Y1.

%//------------------------------------------------------------------------------

impossible(_) :- fail.



%//------------------------------------------------------------------------------
/*

test1 :- make_scripts( state(state_machine(1),enabled), start).
test2 :- make_scripts( nextto(thing(1),thing(2)) & nextto(thing(2),thing(3)), start).
test3 :- make_scripts( at(avatar,vector(5,5,6)), start).
test4 :- make_scripts( nextto(thing(2),thing(3)) & nextto(thing(3),portal(1)) &
		state(state_machine(1),enabled) & nextto(thing(1),thing(2)) &
		contents(avatar,room(2)), start).
*/                
%%make_script

%//------------------------------------------------------------------------------
                                           
                                           
triad_rule(
		exemplar_prop_value(avatar(T),position,vector(A,X,Y,Z)), 
		invoke_method([ avatar(T), property("position"),vector(A,X,Y,Z) ]),
		onproperty(avatar(T), position, vector(A,X,Y,Z))
		).

triad_rule(
		exemplar_prop_value(avatar,position,vector(A,X,Y,Z)), 
		invoke_method([ avatar(T), property("position"),vector(A,X,Y,Z) ]),
		onproperty(avatar(T), position, vector(A,X,Y,Z))
		).

%% enable_state_with_method(State,Method):- catch(triad_rule(State,Method,_UI)).


/*
t1 :- make_scripts( start, state(state_machine(1),enabled), Plan),writeDebug(Plan).

t2 :- make_scripts( start, state(state_machine(1),enabled), Plan),writeDebug(Plan).

*/


enable_state_with_method( at(avatar,P), 	go_to(P,_)).

enable_state_with_method( nextto(avatar,X),	approach(X,_)).

enable_state_with_method( nextto(X,Y), 	carry(X,Y,_)).

enable_state_with_method( nextto(Y,X),	carry(X,Y,_)).

enable_state_with_method( state(S,enabled),	change_state(S)).

enable_state_with_method( helpfull(avatar,B),	get_help(B)).

enable_state_with_method( needhelp,		thank_leave(_)).

enable_state_with_method( contents(avatar,R2), 	gothru(_,_,R2)).

%//------------------------------------------------------------------------------

disabled_state_with_method( at(X,_),U) :- onuievent(X,U).

disabled_state_with_method( nextto(Z,avatar),U) :- !, disabled_state_with_method(nextto(avatar,Z),U).

disabled_state_with_method( nextto(avatar,X), carry(X,_,_)) :- !, fail.

disabled_state_with_method( nextto(avatar,B), get_help(B)) :- !, fail.

disabled_state_with_method( nextto(avatar,B), thank_leave(B)) :- !, fail.

disabled_state_with_method( nextto(X,_),U) :- onuievent(X,U).

disabled_state_with_method( nextto(_,X),U) :- onuievent(X,U).

disabled_state_with_method( helpfull(X,_),U) :- onuievent(X,U).

disabled_state_with_method( needhelp,get_help(_)).

disabled_state_with_method( contents(avatar,_), gothru(_,_,_)).

disabled_state_with_method( state(S,_), change_state(S)).

%//------------------------------------------------------------------------------

onuievent( avatar, go_to(_,_)).

onuievent( avatar, approach(_,_)).

onuievent( avatar, carry(_,_,_)).

onuievent( X, carry(X,_,_)).

onuievent( avatar, get_help(_)).

onuievent( avatar, thank_leave(_)).

onuievent( avatar, gothru(_,_,_)).

%//------------------------------------------------------------------------------

enabled_method_from_state( go_to(P,R), loccontents(P,R) & contents(avatar,R) & needhelp).

enabled_method_from_state( approach(X,R), contents(X,R) & contents(avatar,R) & needhelp).

enabled_method_from_state( carry(X,Y,R),
	istakable(X) & contents(Y,R) & contents(X,R) & nextto(avatar,X) & needhelp).

enabled_method_from_state( change_state(state_machine(S)),
	helpfull(avatar,thing(1)) & nextto(thing(1), state_machine(S))).

enabled_method_from_state( get_help(thing(B)), nextto(avatar,thing(B)) & needhelp).

enabled_method_from_state( thank_leave(thing(B)), helpfull(avatar,thing(B))).

enabled_method_from_state( gothru(D,R1,R2),
	connects(D,R1,R2) & contents(avatar,R1) & nextto(avatar,D) & needhelp).

%//------------------------------------------------------------------------------

readonly_state(thing_is_type(Exemplar,Thing)) :- getpropstr(Thing,'exemplar',Exemplar).



readonly_state( connects(D,R1,R2)) :- connects1(D,R1,R2).

readonly_state( connects(D,R2,R1)) :- connects1(D,R1,R2).

readonly_state( contents(D,R1)) :- readonly_state(connects(D,_,R1)).

readonly_state( istakable(thing(_))).

readonly_state( loccontents(vector(5,5,6),room(4))).

readonly_state( contents(state_machine(1),room(1))).

readonly_state( at(state_machine(1),vector(5,5,4))).

connects1(portal(N),room(N),room(5)) :- range(N,1,4).

range(M,M,_).
range(M,L,N) :- L < N, L1 is L+1, range(M,L1,N).

%//------------------------------------------------------------------------------

initial_given_state( start, at(thing(N), vector(5,5,N))) :- range(N,1,3).

initial_given_state( start, at(avatar,vector(5,5,5))).

initial_given_state( start, contents(thing(N),room(1))) :- range(N,1,3).

initial_given_state( start, contents(avatar,room(1))).

initial_given_state( start, needhelp).

initial_given_state( start, state(state_machine(1),off)).

%//------------------------------------------------------------------------------


