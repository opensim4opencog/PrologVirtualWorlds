:-include('logicmoo_utils_header.pl'). %<?
% ==================================================
% Get/Take
% ==================================================
:-dynamic(have/3).

clientEvent(Channel,Agent,[get,_,Thing]):-take(Channel,Agent,Thing),!.
clientEvent(Channel,Agent,[get,Thing]):-take(Channel,Agent,Thing),!.
clientEvent(Channel,Agent,[take,_,Thing]):-take(Channel,Agent,Thing),!.
clientEvent(Channel,Agent,[take,Thing]):-take(Channel,Agent,Thing),!.

clientEvent(Channel,Agent,[what,do,_,have|_]):-findall(Thing,have(Channel,Agent,Thing),L),say(Channel,Agent,L).

clientEvent(Channel,Agent,[Someone,have,Thing]):-!,sendEvent(Channel,Agent,kif(have(Someone,Thing))),!.
clientEvent(Channel,Agent,[Someone,has,Thing]):-!,sendEvent(Channel,Agent,kif(have(Someone,Thing))),!.


possessObject(Channel,Agent,Type,Id):-
      wfSubclass(Type,'account'),!,
      wfAssert(controls(Channel,Agent,Id)),
      wfAssert(account(Channel,Agent,Id)),
      writeSTDERROnDebug('~n playerCharInGameNow ~w~n',[Id]).

possessObject(Channel,Agent,Type,Id):-
      wfSubclass(Type,'character'),!,
      wfAssert(controls(Channel,Agent,Id)),
      wfAssert(character(Channel,Agent,Id)),
      writeSTDERROnDebug('~n playerCharInGameNow ~w~n',[Id]).

possessObject(Channel,Agent,Type,Id):-
      wfAssert(controls(Channel,Agent,Id)),
      writeSTDERROnDebug('~n Object in game ~w~n',[Id]).



unreachable(Room,Thing):-not(location(Thing,Room)).
unreachable(Room,Thing):-not(location_s(object(Thing, _, _, _), Room)).

take(Channel,Agent,Thing):-
	 not(exists_somewlocationOfChar(Channel,Agent,Thing)),
	 sayIfFeedback(Channel,Agent,[i,am,not,sure,i,know,where,to,find,a,Thing]).

exists_somewlocationOfChar(Channel,Agent,Thing).
take(Channel,Agent,Thing):-
	  locationOfChar(Channel,Agent,Room),
	  location_s(object(Thing, _, big, _), Room),
	  sayIfFeedback(Channel,Agent,'The',Thing,'is too big to carry.'),!.

take(Channel,Agent,Thing):-
       %   locationOfChar(Channel,Agent,Room), 
	  remove_from_location(Thing,Room),
	  asserta_if_new(have(Channel,Agent,Thing)),
	  sayIfFeedback(Channel,Agent,['I now have',Thing]),!.

remove_from_location(Thing,Room):-
	once(retract(location(Thing,Room))),fail.
remove_from_location(Thing,Room):-
	once(retract(location_s(object(Thing, _, _, _), Room))),fail.
remove_from_location(Thing,Room):-!.

take(Channel,Agent,Thing):-
	locationOfChar(Channel,Agent,Room),
	unreachable(Room,Thing),sayIfFeedback(Channel,Agent,[i,see,no,Thing,in,Room]).

  

