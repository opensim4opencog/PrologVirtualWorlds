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
	write_debug(make_scripts,ScriptMore).

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
	conjoin(CurrentPropValues,GoalPropValuesFuture,GoalPropValues).

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

conjoin(PropValues & CurrentPropValues,NextGoalPropValues,PropValues & GoalPropValuesFuture) :- !,
	conjoin(CurrentPropValues,NextGoalPropValues,GoalPropValuesFuture).

conjoin(PropValues,NextGoalPropValues,PropValues & NextGoalPropValues).

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


