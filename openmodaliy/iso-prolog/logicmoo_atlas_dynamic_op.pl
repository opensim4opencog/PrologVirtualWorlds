:-include('logicmoo_utils_header.pl').
clientEvent(Channel,Agent, message(Call, packet(Channel, Refno, Agent, Serialno, Time))):-opHowto([Verb],ToDoList),
	 Call =.. [Verb,Object,OPTokens],!,
	 writeSTDERR(opHowto([Verb],ToDoList)),
	 setCleintOpParam(Channel,Agent,verb,Verb),
	 setCleintOpParam(Channel,Agent,to,Channel),
	 setCleintOpParam(Channel,Agent,from,Agent),
	 setCleintOpParam(Channel,Agent,refno,Refno),
	 setCleintOpParam(Channel,Agent,serialno,Serialno),
	 setCleintOpParam(Channel,Agent,time,Time),
	 setCleintOpParam(Channel,Agent,object,Object),
	 setCleintOpParam(Channel,Agent,optokens,OPTokens),!,
	 invokeTodoList(Channel,Agent,ToDoList),!.

:-dynamic(ccDyn/3).
setCleintOpParam(Channel,Agent,Name,Value):-retractall(ccDyn(Channel,Agent,Name,_)),assert(ccDyn(Channel,Agent,Name,Value)),!.
getCleintOpParam(Channel,Agent,Name,Value):-ccDyn(Channel,Agent,Name,Value).
      
% =========================================================
% TODO List
% =========================================================
invokeTodoList(Channel,Agent,[]):-!.
invokeTodoList(Channel,Agent,[H|T]):-
      invokeTodo(Channel,Agent,H),!,
      invokeTodoList(Channel,Agent,T).
      
% =========================================================
% TODO Item
% =========================================================

%setEntityObject
invokeTodo(Channel,Agent,setEntityObject):-
      getCleintOpParam(Channel,Agent,object,Object),
      getIdOut(Object,Id),
      setEntity(Id,Object),!.

%sendInfoObject
invokeTodo(Channel,Agent,sendInfoObject):-
      getCleintOpParam(Channel,Agent,object,Object),
      getCleintOpParam(Channel,Agent,refno,Refno),
      getIdOut(Object,Id),
      getObjectMap(Id,Map),
      writeSTDERROnDebug(getObjectMap(Id,Map)),
      sendInfo(Channel,Agent,Refno,Map),!.

%sendInfoObjectRef
invokeTodo(Channel,Agent,sendInfoObjectRef):-
      getCleintOpParam(Channel,Agent,object,Object),
      getCleintOpParam(Channel,Agent,refno,Refno),
      getIdOut(Object,Id),
      getObjectRefInfo(Id,Map),
      writeSTDERROnDebug(getObjectRefInfo(Id,Map)),
      sendInfo(Channel,Agent,Refno,Map),!.

%deleteObject
invokeTodo(Channel,Agent,deleteObject):-
      getCleintOpParam(Channel,Agent,object,Object),
      getIdOut(Object,Id),
      writeSTDERR(deleting(Id,ToSet)),!,
      retractall((typeOf(Id,_,_))),
      retractall((valueOf(Id,_,_))),!.


%sendOpTokensPerceptionAs(What)
invokeTodo(Channel,Agent,sendOpTokensPerceptionAs(What)):-
      getCleintOpParam(Channel,Agent,object,Object),
      getCleintOpParam(Channel,Agent,from,Agent),
      getCleintOpParam(Channel,Agent,optokens,OPTokens),
      getCleintOpParam(Channel,Agent,refno,Refno),
      sendPerception(Agent,Refno,Channel,Agent,What,map(OPTokens)),!.

%sendOpTokensPerceptionFromAs(What)
invokeTodo(Channel,Agent,sendOpTokensPerceptionFromAs(What)):-
      getCleintOpParam(Channel,Agent,object,Object),
      getCleintOpParam(Channel,Agent,from,Agent),
      getCleintOpParam(Channel,Agent,optokens,OPTokens),
      getCleintOpParam(Channel,Agent,refno,Refno),
      sendPerception(Agent,Refno,Channel,Agent,What,map([from:string(Agent)|OPTokens])),!.

