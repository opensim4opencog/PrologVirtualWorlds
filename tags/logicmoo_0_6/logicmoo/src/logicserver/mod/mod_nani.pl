% ==================================================
% Nani Search Adapted for IRC (see: http://www.amzi.com/AdventureInProlog/ )
% ==================================================

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

:-dynamic(do_irc_nd/1).
:-multifile(do_irc_nd/1).


:-ensure_loaded('mod/nani.data').

% ==================================================
% Mud Dictionary
% ==================================================

:-dynamic(mud_dictionary/2).

mud_dictionary([dinning,room],['dinning room']).
mud_dictionary([washing,machine],['washing machine']).
mud_dictionary([my,body],['self']).
mud_dictionary([back,yard],['back yard']).
mud_dictionary([stand,table],['stand table']).
mud_dictionary([light,switch],[light_switch]).
mud_dictionary([living,room],['living room']).
mud_dictionary([easy,chair],[easy_chair]).
mud_dictionary([coffee,table],[coffee_table]).
mud_dictionary([coffee,cup],[coffee_cup]).
mud_dictionary([top,drawer],[top_drawer]).
mud_dictionary([middle,drawer],[middle_drawer]).
mud_dictionary([bottom,drawer],[bottom_drawer]).
mud_dictionary([anthurium,crystallinum],[anthurium_crystallinum]).
	
					

%mud_dictionary([],[]).

% add new words here

% ==================================================
% Going Places
% ==================================================

:-dynamic(here/1).
:-dynamic(goto/1).

here(Place):-
	corpius(Body),
	location(Body,Place).
here('kitchen').
corpius('me').


do_irc(G):- 
	format(user_error,'~q.~n',[G]),
	do_irc_nd(G),!.

% when this program loads it starts you in the kitchen

do_irc_nd([go,_,Thing]):-goto(Thing).
do_irc_nd([go,Thing]):-goto(Thing).

goto(Place):-
	here(Place),!,
	say([i,am,already,in,Place]).
	      
goto(Place):-
	room(Place), % room exists
	not(connect(_, Place)), % no doors
	move(Place),look.

goto(Place):-
	room(Place), % room exists
	connect(_, Place), % has doors
	here(Located),
	connect(Located, Place),
	move(Place),!,look.

/*
goto(Place):-
	room(Place),!, % room exists
	connect(_, Place),!, % has doors
	say('I can''t get there from here.'),!.
*/	
goto(Place):-
	create_new_room(Place),
	move(Place).
			

move(Place):-	
	corpius(Body),
	place_thing_at(Body,Place).

create_new_room(Place):-
	asserta_if_new(room(Place)),
	asserta_if_new(door(Place,'irc')).
	
		
	
% ==================================================
% Eating stuff
% ==================================================
:-dynamic(eat/1).

:-dynamic(have/1).

do_irc_nd([eat,_,Thing]):-eat(Thing).
do_irc_nd([eat,Thing]):-eat(Thing).

eat(Thing):-not(have(Thing)),say([i,dont,have,Thing]),!.
eat(Thing):-have(Thing),tastes_yucky(Thing),say([barffff,!,Thing]),!.
eat(Thing):-have(Thing),edible(Thing),say([yumyum,!,Thing]),retract(have(Thing)),!.
eat(Thing):-have(Thing),say([no,way,!,i,cant,eat,Thing]),!.

tastes_yucky(Thing):-has_switch(Thing).

where_food(Thing,Noun2) :-  
  location(Thing,Noun2),
  edible(Thing).

where_food(Thing,Noun2) :-
  location(Thing,Noun2),
  tastes_yucky(Thing).
  
:-dynamic(drink/1).

do_irc_nd([drink,_,Thing]):-drink(Thing).
do_irc_nd([drink,Thing]):-drink(Thing).

drink(Thing):-not(have(Thing)),say([i,dont,have,Thing]),!.
drink(Thing):-have(Thing),tastes_yucky(Thing),say([barffff,!,Thing]),!.
drink(Thing):-have(Thing),edible(Thing),say([yumyum,!,Thing]),retract(have(Thing)),!.
drink(Thing):-have(Thing),say([no,way,!,i,cant,eat,Thing]),!.



:-dynamic(edible/1).
:-dynamic(tastes_yucky/1).

edible(apple).
edible(coffee).
edible(broccoli).
edible(crackers).
edible(Thing) :- tastes_yucky(Thing).

edible(_) .

tastes_yucky(vomit).


% ==================================================
% Get/Take
% ==================================================

:-dynamic(take/1).

do_irc_nd([get,_,Thing]):-take(Thing),!.
do_irc_nd([get,Thing]):-take(Thing),!.
do_irc_nd([take,_,Thing]):-take(Thing),!.
do_irc_nd([take,Thing]):-take(Thing),!.

do_irc_nd([what,do,_,have]):-findall(Thing,have(Thing),L),say(L).

do_irc_nd([_,have,Thing]):-asserta(have(Thing)),!.

take(Thing):-not(exists_somewhere(Thing)),say(i,am,not,sure,i,know,where,to,find,a,Thing).

exists_somewhere(Thing).


unreachable(Room,Thing):-not(location(Thing,Room)).
unreachable(Room,Thing):-not(location_s(object(Thing, _, _, _), Room)).

take(Thing):-
	  here(Room),
	  location_s(object(Thing, _, big, _), Room),
	  say('The',Thing,'is too big to carry.'),!.

take(Thing):-
       %   here(Room), 
	  remove_from_location(Thing,Room),
	  asserta_if_new(have(Thing)),
	  say(['I now have ',Thing]),!.

remove_from_location(Thing,Room):-
	once(retract(location(Thing,Room))),fail.
remove_from_location(Thing,Room):-
	once(retract(location_s(object(Thing, _, _, _), Room))),fail.
remove_from_location(Thing,Room):-!.

take(Thing):-
	here(Room),
	unreachable(Room,Thing),say([i,see,no,Thing,in,Room]).

  
% ==================================================
% Drop
% ==================================================

:-dynamic(drop/1).

do_irc_nd([drop,_,Thing]):-drop(Thing).
do_irc_nd([drop,Thing]):-drop(Thing).

drop(Thing):-not(have(Thing)),say([i,dont,have,Thing]),!.
drop(Thing):-retract(have(Thing)),!,here(L),assert(location(Thing,L)),say([dropped,Thing,in,L]),!.


% ==================================================
% Turning Things On/Off
% ==================================================

do_irc_nd([turn,Thing,OnOff]):-switch(Thing,OnOff).

switch(Thing,Status):-
	retractall(switch_status(Thing,_)),	% remove the old value
	asserta_if_new(has_switch(Thing)),  % add the new status
	assert(switch_status(Thing,Status)).  % add the new status


:-dynamic(has_switch/1).
:-dynamic(switch_status/2).

has_switch('washing machine'). % the washing machine has a switch
switch_status('washing machine',off). % the washing machine is off


% ==================================================
% Looking at Location
% ==================================================

do_irc_nd([where,is,X]):-location(X,Y),say([X,is,'located: ',Y]).
do_irc_nd([where,are,you]):-say([right,here]).
do_irc_nd([where,am,i]):-look.
do_irc_nd([where,_,i]):-look.



look :-	     % looks at the room
  here(Place),
  list_things(Place,Things),
  say((['I am in',Place,'  I see:'|Things])),
  list_connections(Place,Places),
  say((['I can go to:'|Places])),!.

is_contained_in(Thing1,Thing2) :-  
  location(Thing1,Thing2).

is_contained_in(Thing1,Thing2) :-
  location(Thing,Thing2),
  is_contained_in(Thing1,Thing).


list_things(Place,Set) :-  
  findall(Thing,location(Thing, Place),Things),
  sort(Things,Set).

list_connections(Place,Set) :-  
  findall(Thing,connect(Place, Thing),Things),
  sort(Things,Set).


% ==================================================
% Looking at Thing
% ==================================================

do_irc_nd([look,in,Thing]):-look_in(Thing).
do_irc_nd([look,_,Thing]):-look(Thing).
do_irc_nd([look,Thing]):-look(Thing).

:-dynamic(look/1).  

look_in(Thing):-
	location(Noun2,Thing),say([i,see,Noun2]),fail.

look(Thing):-
	property(Thing,Noun2),
	say(Noun2),fail.

look(Thing):-
	has_switch(Thing),!,
	give_switch_status(Thing).

give_switch_status(Thing):-
	switch_status(Thing,Noun2),!,
	say([Thing,is,Noun2]).

look(Thing):-
	subpart(Thing,Noun2),!,
	look(Noun2).
	
look(Thing):-say([i,dont,see,anything,special,about,Thing]).

% ==================================================
% Create a Thing  at 
% ==================================================

do_irc_nd([the,Thing,is,_,_,Noun2]):-
	asserta_if_new(location(Thing,Noun2)),fail.

nani_event_from(Channel,Who,join(X)):-
	codes_to_input(Channel,[SChannel]),
	codes_to_input(Who,[SWho]),
	remember_join(SWho,SChannel),!.

nani_event_from(Channel,Who,mode(Location,Mode,Target)):-
	remember_mode(Channel,Who,Location,Mode,Target),!.

nani_event_from(Channel,Who,part(X)):-
	codes_to_input(Channel,[SChannel]),
	codes_to_input(Who,[SWho]),
	remember_part(SWho,SChannel),!.

nani_event_from(Channel,Who,quit(How)):-
	codes_to_input(Channel,[SChannel]),
	codes_to_input(Who,[SWho]),
	codes_to_input(How,[SHow]),
	remember_quit(SWho,SChannel,SHow),!.

nani_event_from(Channel,Who,kick(_,How)):-
	codes_to_input(Channel,[SChannel]),
	codes_to_input(Who,[SWho]),
	remember_part(SWho,SChannel),!.

nani_event_from(Channel,Who,say(How)):-
	codes_to_input(Channel,[SChannel]),
	codes_to_input(Who,[SWho]),
	remember_join(SWho,SChannel),!.

	
remember_mode(Channel,Who,Location,Mode,Target).

remember_join(Who,Channel):-
	create_irc_room(Channel),
	place_thing_at(Who,Channel).

remember_part(Who,Channel):-!,
	create_irc_room(Channel),
	retractall(location(Who,Channel)).

remember_quit(Who,Channel,Msg):-!,
	create_irc_room(Channel),
	place_thing_at(Who,irl).
	

place_thing_at(Who,Where):-
	retractall(location(Who,_)),
	asserta_if_new(location(Who,Where)).

create_irc_room(Channel):-
	asserta_if_new(room(Channel)),
	asserta_if_new(door(Channel,'irc')).

	

