:-include('logicmoo_utils_header.pl').
% =================================================================================
% Send Sighting to Client
% =================================================================================

sendSighting(Agent,Channel,Agent,Refno,UMap):-
   sendPerception(Agent,Refno,Channel,Agent,"sight",UMap),!.
      
% =================================================================================
% Send Perception to Client
% =================================================================================
%sendPerception(player4, 3, io('$stream'(219424), '$stream'(219444)), [

sendPerception(Agent,Refno,OC,Perception,Map):-
      writeSTDERR(sendPerception(Agent,Refno,OC,Perception,Map)),
      getAllUsers(List),
      writeSTDERR(rec(List)),!,
      sendPerceptionList(Agent,Refno,OC,List,Perception,Map),!.

getAllUsers(ListY):-
      findall(CC:Id,wfQuery(controls(Id,CC)),L),sort(L,List),
      recFix(List,ListY),!.

%((wfQuery(controls(Channel,Agent,Player)),wfSubclass(Player,'account'));Player=CC)


recFix(List,ListO):-member(A1:A2,List),atom(A1),atom(A2),!,
      delete(List,A1:A2,ListX),delete(ListX,_:A1,ListY),!,recFix(ListY,ListO).
recFix(List,List).
   
sendPerceptionList(Agent,Refno,OC,[],Perception,Map):-!.
sendPerceptionList(Agent,Refno,OC,[H|T],Perception,Map):- !,
      sendPerceptionList(Agent,Refno,OC,H,Perception,Map),!,
      sendPerceptionList(Agent,Refno,OC,T,Perception,Map),!.

%sendPerception(Agent,Refno,OC,CC:Channel,Perception,Map):-wfSubclass(Channel,account),!.
%sendPerception(Agent,Refno,OC,CC:Channel,Perception,Map):-wfSubclass(Channel,account),!.
%         sendPerception(Agent,Refno,Channel,Agent,'appearance',map([from:string(Agent)|OPTokens])),!.

sendPerceptionList(Agent,Refno,OC,Channel:Channel,Agent,Perception,Map):-
	 writeMessageOp(Channel,Agent,Channel,Agent,Refno,[Perception],Map),!.

sendPerceptionList(Agent,Refno,OC,Channel,Agent,Perception,Map):-
      writeSTDERR(sendPerceptionList(Agent,Refno,OC,Channel,Agent,Perception,Map)).

% =================================================================================
% Send Info to Client
% =================================================================================

sendInfo(Channel,Agent,Refno,Map):-
      writeSTDERR(sendInfo(Channel,Agent,Refno,Map)),
      getPlayerForCC(Channel,Agent,Channel),
      writeMessageOp(Channel,Agent,Channel,'',Refno,["info"],Map).

% =================================================================================
% Write Message OP (makes serial number)
% =================================================================================
writeMessageOp(Channel,Agent,Channel,Agent,Refno,ParentsList,Args):-
      getLocalSerial(Serialno),
      writeMessageOp(Channel,Agent,Channel,Agent,Serialno,Refno,ParentsList,Args),!.

% =================================================================================
% Write Message OP (with serial number)
% =================================================================================
writeMessageOp(Channel,Agent,Channel,Agent,Serialno,Refno,ParentsList,Args):-
   constructMessageOp(Channel,Agent,Serialno,Refno,ParentsList,Args,Msg),!,
   say(Channel,Agent,Msg),!.

% =================================================================================
% Construct Message OP (with serial number)
% =================================================================================
constructMessageOp(Channel,Agent,Serialno,Refno,ParentsList,Args,
      map([serialno:int(Serialno),refno:int(Refno),from:string(Agent),to:string(Channel),seconds:float(S1970),future_seconds:float(0.0),time_string:string(TimeString),args:Map,parents:list(ParentsList),id:string(''),objtype:"op",name:string('server_reply')])):-
   %getPrettyDateTime(TimeString),
   TimeString='',
   mapFix(Args,Map), get_time(X),sformat(S1970,'~3f',X),!.

% =================================================================================
% Send Error to Client
% =================================================================================

sendError(Channel,Agent,Ref,Msg,UMap):-
      mapFix(UMap,Map),
      getLocalSerial(LocalSerial),
      getPlayerForCC(Channel,Agent,Channel),
      getPrettyDateTime(TimeString),get_time(X),sformat(S1970,'~3f',X),!,
      say(Channel,Agent,map([serialno:int(LocalSerial),refno:int(Ref),from:string(''),to:string(Channel),seconds:float(S1970),future_seconds:float(0.0),time_string:string(TimeString),args:list([map([message:string(Msg)]),Map]),parents:list(["error"]) ,id:string(''),objtype:"op",name:string('')])).


toAtom(OP,Chars):-toMarkup(chat,OP,VS,Chars).


