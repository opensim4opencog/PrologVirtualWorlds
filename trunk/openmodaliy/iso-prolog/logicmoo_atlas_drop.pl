:-include('logicmoo_utils_header.pl').

% ==================================================
% Drop
% ==================================================
clientEvent(Channel,Agent,english(phrase([drop,Id],_),packet(Channel,Serial,Agent,Refno,Time))):-!,
      drop(Channel,Agent,Id).
clientEvent(Channel,Agent,english(phrase([drop,_,Id],_),packet(Channel,Serial,Agent,Refno,Time))):-!,
      drop(Channel,Agent,Id).

drop(Channel,Agent,Thing):-not(have(Channel,Agent,Thing)),sayIfFeedback(Channel,Agent,[i,dont,have,Thing]),!.
drop(Channel,Agent,Thing):-retract(have(Channel,Agent,Thing)),!,locationOfChar(Channel,Agent,L),assert(location(Thing,L)),sayIfFeedback(Channel,Agent,[dropped,Thing,in,L]),!.

'?'(_,_).
