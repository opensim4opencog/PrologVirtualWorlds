:-include('logicmoo_utils_header.pl').

% ==================================================
% Eating stuff
% ==================================================
:-dynamic(edible/1).
:-dynamic(tastes_yucky/1).
:-dynamic(eat/3).

clientEvent(Channel,Agent,[eat,_,Thing]):-eat(Channel,Agent,Thing).
clientEvent(Channel,Agent,[eat,Thing]):-eat(Channel,Agent,Thing).

eat(Channel,Agent,Thing):-not(have(Channel,Agent,Thing)),sayIfFeedback(Channel,Agent,[i,dont,have,Thing]),!.
eat(Channel,Agent,Thing):-have(Channel,Agent,Thing),tastes_yucky(Thing),sayIfFeedback(Channel,Agent,[barffff,!,Thing]),!.
eat(Channel,Agent,Thing):-have(Channel,Agent,Thing),edible(Thing),sayIfFeedback(Channel,Agent,[yumyum,!,Thing]),retract(have(Channel,Agent,Thing)),!.
eat(Channel,Agent,Thing):-have(Channel,Agent,Thing),sayIfFeedback(Channel,Agent,[no,way,!,i,cant,eat,Thing]),!.

tastes_yucky(Thing):-has_switch(Thing).

where_food(Thing,Noun2) :-  
  location(Thing,Noun2),
  edible(Thing).

where_food(Thing,Noun2) :-
  location(Thing,Noun2),
  tastes_yucky(Thing).
  

clientEvent(Channel,Agent,[drink,_,Thing]):-drink(Channel,Agent,Thing).
clientEvent(Channel,Agent,[drink,Thing]):-drink(Channel,Agent,Thing).

drink(Channel,Agent,Thing):-not(have(Channel,Agent,Thing)),sayIfFeedback(Channel,Agent,[i,dont,have,Thing]),!.
drink(Channel,Agent,Thing):-have(Channel,Agent,Thing),tastes_yucky(Thing),sayIfFeedback(Channel,Agent,[barffff,!,Thing]),!.
drink(Channel,Agent,Thing):-have(Channel,Agent,Thing),edible(Thing),sayIfFeedback(Channel,Agent,[yumyum,!,Thing]),retract(have(Channel,Agent,Thing)),!.
drink(Channel,Agent,Thing):-have(Channel,Agent,Thing),sayIfFeedback(Channel,Agent,[no,way,!,i,cant,eat,Thing]),!.




edible(apple).
edible(coffee).
edible(broccoli).
edible(crackers).
edible(Thing) :- tastes_yucky(Thing).

edible(_) .

tastes_yucky(vomit).


