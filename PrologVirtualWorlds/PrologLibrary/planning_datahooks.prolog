%//------------------------------------------------------------------------------

test1 :- make_scripts( state(state_machine(1),enabled), start).
test2 :- make_scripts( nextto(thing(1),thing(2)) & nextto(thing(2),thing(3)), start).
test3 :- make_scripts( at(avatar,vector(5,5,6)), start).
test4 :- make_scripts( nextto(thing(2),thing(3)) & nextto(thing(3),portal(1)) &
		state(state_machine(1),enabled) & nextto(thing(1),thing(2)) &
		contents(avatar,room(2)), start).
                
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


t1 :- make_scripts( start, state(state_machine(1),enabled), Plan),write_debug(Plan).

t2 :- make_scripts( start, state(state_machine(1),enabled), Plan),write_debug(Plan).



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

