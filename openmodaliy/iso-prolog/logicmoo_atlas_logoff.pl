
:-include('logicmoo_utils_header.pl').

% =================================================================================
% =================================================================================
% Execute Logoff command
% =================================================================================
% =================================================================================
clientEvent(Channel,Agent,message(logout(Obj),packet(_,_,Player,_, _))):-!,logoff(CC),asserta(hasLeft(CC)).

logoff(CC):-
      writeSTDERR('logging off ~w',[CC]),
      retractall((loggedOn(Channel,Agent,_))),!,
      asserta(hasLeft(CC)),
      showDisapear(CC),!.

showDisapear(CC):-
      (isPlayerForCC(Channel,Agent,Player)),
      getObjectMap(Player,Map),
      sendPerception(Player,0,Channel,Agent,"disappearance",Map),!.
showDisapear(CC).


