:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% Execute Move command
% =================================================================================
% =================================================================================
% Authorial KR for this OP
%opHowto(['move'],[setEntityObject,sendOpTokensPerceptionAs("sight")]).


clientEvent(Channel,Agent,[go,_,Thing]):-goto(Channel,Agent,Thing).
clientEvent(Channel,Agent,[go,D]):-atom_codes(D,[_]),!.
clientEvent(Channel,Agent,[go,Thing]):-goto(Channel,Agent,Thing).


clientEvent(Channel,Agent,imperative_text(Agent,[go,X,Room],packet(Channel,Serial,Agent,Refno,Time))):-
	 clientEvent(Channel,Agent,go(Agent,X,Room,Refno)).

clientEvent(Channel,Agent,imperative_text(Agent,[go,Room],packet(Channel,Serial,Agent,Refno,Time))):-
	 clientEvent(Channel,Agent,go(Agent,to,Room,Refno)).


clientEvent(Channel,Agent,go(Agent,X,Room,Refno)):-!.
	    
clientEvent(Channel,Agent,message(move(concept(Ccpt), OPTokens), packet(Channel, Refno, Agent, Serialno, Time))):-
	 member(mode:Mode,Ccpt),
	 member(loc:Room,Ccpt),!,
         setEntityProp(Agent,loc,Room),!,
         sendPerception(Agent,Refno,Channel,Agent,'appearance',map([from:string(Agent)|OPTokens])),!.


      

% touch Something 
clientEvent(Channel,Agent, message(move(Set,OPTokens), packet(Channel, Refno, Agent, Serialno, Time))):-!,
      setEntityProp(Id,path_todo,list([])),
      setEntityProp(Id,action,standing),
      setEntityProp(Id,velocity,list([float(0),float(0),float(0)])),
      getIdOut(Set,Id),
      getListOut(Set,List),
      doMove(Channel,Agent,Id,List,OPTokens,packet(Channel, Refno, Agent, Serialno, Time)).


% velocity only
doMove(Channel,Agent,Id,Set,OPTokens,packet(Channel, Refno, Agent, Serialno, Time)):-
      member(velocity:list([float(Vx),float(Vy),float(Vz)]),Set),not(member(pos:_,Set)),!,
      setEntityProp(Id,velocity,list([float(Vx),float(Vy),float(Vz)])),
      getPosition(Id,list([float(X1),float(Y1),float(Z1)])),
      vectorPlusVelocityToVector(X1,Y1,Z1,Vx,Vy,Vz,PosAfter),
      setEntityProp(Id,pos,PosAfter),
      stampObject(Id),
      setEntityProp(Id,action,walking),
      sendPerception(Agent,Refno,Channel,Agent,'sight',map([from:string(Agent)|OPTokens])),
      sendApearances(Agent,Refno,Channel,Agent,Id,list([float(X1),float(Y1),float(Z1)]),PosAfter).

% pos (ignore velocity cuz of frost)
doMove(Channel,Agent,Id,Set,OPTokens,packet(Channel, Refno, Agent, Serialno, PTime)):-
      member(pos:End,Set),!,
      setEntityProp(Id,action,standing),
      setEntityProp(Id,velocity,list([float(0.0),float(0.0),float(0.0)])),
      getPosition(Id,Start),
      wfGetPropValue(Id,move_speed,float(Rate)),
      writeSTDERR(doMovePos(Id,Set,Start,End,Rate)),
      toFromToQuatAndPosList(Start,End,Rate,Orientation,[FirstMove|PosList]),
      (member(orientation:SetOrientation,Set);SetOrientation=Orientation),!,
      setEntityProp(Id,orientation,SetOrientation),
      setEntityProp(Id,path_todo,list(PosList)),
      setEntityProp(Id,pos,FirstMove),
      stampObject(Id),
      sendPerception(Agent,Refno,Channel,Agent,'sight',map([from:string(Agent)|OPTokens])),
      sendApearances(Agent,Refno,Channel,Agent,Id,Start,FirstMove).


% normal velocity + pos 
doMove(Channel,Agent,Id,Set,OPTokens,packet(Channel, Refno, Agent, Serialno, Time)):-
      member(velocity:_,Set),member(pos:PosAfter,Set),!,
      getPosition(Id,PosBefore),
      setObject(Id,Set),
      sendPerception(Id,Refno,Channel,Agent,'sight',map([from:string(Agent)|OPTokens])),
      sendApearances(Agent,Refno,Channel,Agent,Id,PosBefore,PosAfter).

sendApearances(Agent,Refno,Channel,Agent,Id,list([float(Sx),float(Sy),float(Sz)]),list([float(Vx),float(Vy),float(Vz)])):-
      wfGetPropValue(Id,orientation,Orientation),
      wfGetPropValue(Id,velocity,Velocity),
      wfGetPropValue(Id,stamp,Stamp),
      wfGetPropValue(Id,action,Action),
      sendPerception(Id,Refno,Channel,Agent,'sight',map([id:string(Id),action:Action,stamp:Stamp,pos:list([float(Vx),float(Vy),float(Vz)]),velocity:Velocity,orientation:Orientation])),!.
      

%sendApearances(Id,PosBefore,PosAfter):-getAllPossibleSeers(Id,Seers).
      


% getActualSeers

notifyPercept(This,OP,MovedThing,X1,Y1,Z1):-
       getPosition(This,list([float(X2),float(Y2),float(Z2)])),
       Distance is sqrt((X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2) + (Z1-Z2)*(Z1-Z2)),
       notifyPercept(OP,MovedThing,This,Distance),
       notifyPercept(OP,This,MovedThing,Distance),!.

notifyPercept("sight",This,MovedThing,Distance):-
      wfSubclass(This,'character'),
      wfGetPropValue(MovedThing,objtype,'object'),
      getObjectLookInfo(This,10,MovedThing,Map),
      (canSeeDistance(This,Distance) ->
          writeMessageOp(Channel,Agent,This,MovedThing,0,['appearance'],Map);
          writeMessageOp(Channel,Agent,This,MovedThing,0,['disappearance'],Map)).

notifyPercept(This,OP,MovedThing,Distance).

canSeeDistance(Agent,Distance):-wfGetPropValue(Agent,sight_sense,Dist),(Distance=Dist;Distance>Dist),!.



goto(Channel,Agent,Place):-locationOfChar(Channel,Agent,Place),!,sayIfFeedback(Channel,Agent,[i,am,already,in,Place]).
	      
goto(Channel,Agent,Place):-wfSubclass(Place,'dirrection'),
      moveDirection(Channel,Agent,Place).

moveDirection(Channel,Agent,Dir):-sayIfFeedback(Channel,Agent,[you,move,Dir]).

goto(Channel,Agent,Place):-
	room(Place), % room exists
	not(connect(_, Place)), % no doors
	move(Channel,Agent,Place),look.

goto(Channel,Agent,Place):-
	room(Place), % room exists
	connect(_, Place), % has doors
	locationOfChar(Channel,Agent,Located),
	connect(Located, Place),
	move(Channel,Agent,Place),!,look.

/*
goto(Channel,Agent,Place):-
	room(Place),!, % room exists
	connect(_, Place),!, % has doors
	sayIfFeedback(Channel,Agent,'I can''t get there from here.'),!.
*/	
goto(Channel,Agent,Place):-
	create_new_room(Place),
	move(Channel,Agent,Place).
			

move(Channel,Agent,Place):-	
	corpius(Channel,Agent,Body),
	place_thing_at(Body,Place).

create_new_room(Place):-
	asserta_if_new(room(Place)),
	asserta_if_new(door(Place,'irc')).
	

eventEnter(Agent,Channel):-
	create_irc_room(Channel),
	place_thing_at(Agent,Channel).

eventLeave(Agent,Channel):-!,
	create_irc_room(Channel),
	retractall(location(Agent,Channel)).

eventLogout(Agent,Channel,Msg):-!,
	create_irc_room(Channel),
	place_thing_at(Agent,irl).
	

place_thing_at(Agent,Where):-
	retractall(location(Agent,_)),
	asserta_if_new(location(Agent,Where)).

create_irc_room(Channel):-
	asserta_if_new(room(Channel)),
	asserta_if_new(door(Channel,'irc')).


