:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% Execute Create command
% =================================================================================
% =================================================================================
% Server English
englishServerInterface([create,Type,Name]):- !.
%      creationMap(Type,[Name|Properties],Map),
%      clientEvent(Channel,Agent,message(create(object(Type,[parents:list([string(Type)])|Map]),create(Type,Name,Properties)),Packet)).


% English
clientEvent(Channel,Agent,english(phrase([create,Type,Name|Properties],_),packet(Channel,Serial,Agent,Refno,Time))):-!,
      creationMap(Type,[Name|Properties],Map),
      clientEvent(Channel,Agent,create(Type,Properties,Agent,Refno)).
      
% Atlas
clientEvent(Channel,Agent,message(create(concept(CreateArgs),OpToks),packet(Channel,Serial,Agent,Refno,Time))):-
      member(parents:list([Type]),CreateArgs),!,
      clientEvent(Channel,Agent,create(Type,CreateArgs,Agent,Refno)).

clientEvent(Channel,Agent,message(create(object(Type,Properties),OPTokens),packet(Channel,Serial,Agent,Refno,Time))):-!,
      clientEvent(Channel,Agent,create(Type,Properties,Agent,Refno)).
      
% ===============================================
% CREATE EVENT
% ===============================================

clientEvent(Channel,Agent,create(Type,Properties,Agent,Refno)):-
      createObject(Channel,Agent,Type,Properties,Id),
      getObjectMap(Id,Map),!,
%      ignore(sendPerception(Id,Refno,Channel,Agent,"info",Map)),!,
      ignore(sendPerception(Id,Refno,Channel,Agent,"appearance",Map)),!.

% ===============================================
% CREATE PROC
% ===============================================
      
createObject(Channel,Agent,Type,Properties,Id):-
      ensureTypeExists(Channel,Agent,Type),
      gensym(Type,Id),
      wfAssert(valueOf(Id,id,Id)),!,
      ((wfQuery(controls(Channel,Agent,Player)),wfSubclass(Player,'account'));Player=CC),
      setEntity(Id,Properties),!,
      wfAssert(valueOf(Id,id,Id)),!,
      possessObject(Player,Type,Id),      
      wfAssert(valueOf(Id,parents,list([Type]))),
      wfAssert(valueOf(Id,objtype,object)),
      writeSTDERR(createObject(Channel,Agent,Type,Properties,Id)),!.

% ==================================================
% Creation Types
% ==================================================
ensureTypeExists(Channel,Agent,Type):-wfSubclass(Type,_),!.
ensureTypeExists(Channel,Agent,Type):-wfAssert(valueOf(Type,parents,list(['game_entity']))),wfAssert(typeOf(Type,parents,list)),!.

% ==================================================
% Creation Templates
% ==================================================
creationMap(Type,ListIn,ListOut):-
   creationMapData(Type,Properties),!,
      applyCreationMapData(Properties,ListIn,ListOut).
      
creationMapData(account,[username,password]).
creationMapData(character,[name]).
creationMapData(game_entity,[name]).
creationMapData(Type,List):-wfSubclass(Type,What),creationMapData(What,List),!.

applyCreationMapData([],_,[]).
applyCreationMapData([H|HH],[V|VV],[H:V|HV]):-
	 applyCreationMapData(HH,VV,HV).





