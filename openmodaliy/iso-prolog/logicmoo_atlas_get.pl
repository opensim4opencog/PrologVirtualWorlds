:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% Execute Get command
% =================================================================================
% =================================================================================
% Authorial KR for this OP
%opHowto(['get'],[sendInfoObject]).


clientEvent(Channel,Agent,english(phrase([info,Id],_),packet(Channel,Serial,Agent,Refno,Time))):-!,
      getObjectMap(Id,Map),
      sendInfo(Channel,Agent,Refno,Map),!.

clientEvent(Channel,Agent,english(phrase([what,is,Id|_],_),packet(Channel,Serial,Agent,Refno,Time))):-!,
      getObjectMap(Id,Map),
      sendInfo(Channel,Agent,Refno,Map),!.

% Get Something  %packet(Channel,Refno,Agent,Serialno,Time)

clientEvent(Channel,Agent,message(get(Object,OPTokens),packet(Channel,Serialno,Agent,Refno,Time))):-!,
      clientEvent(Channel,Agent,get(Object,packet(Channel,Serialno,Agent,Refno,Time))).

clientEvent(Channel,Agent,get(Object,packet(Channel,Serialno,Agent,Refno,Time))):-!,
      getIdOut(Object,Id),
      call_with_depth_limit(getObjectMap(Id,Map),200,_),!,
      writeMessageOp(Channel,Agent,Agent,Channel,Refno,["info"],Map).


% getObjectRefInfo 
getObjectRefInfo(Id,map(ValuesList)):-
      getValuesForProps(Id,[objtype,id,stamp,parents,loc],ValuesList).

% getObjectMap 
getObjectMap(Id,map(ValuesList)):-
      writeSTDERR(getObjectMap(Id)),
      getAllNamedProps(Id,Props1),!,
      getOnlyPropsUsedPublic(Props1,Props),!,
      getValuesForProps(Id,Props,ValuesList),!.
      
getOnlyPropsUsedPublic(Props1,Props):-
      fdelete(Props1,
	    [abstract_type,specification,
	    stamp_contains,stamp_inherit,inherit,example,auto,maker,file,script,
	    specification_file],Props).

getAllNamedProps(Id,Props):-
      getAllInstanceProps(Id,InstanceProps),
      getAllTypeProps(Id,TypeProps),
      append(InstanceProps,TypeProps,List),sort(List,Props),!.

getAllInstanceProps(Id,Props):-
   findall(Name,(wfGetPropValueInstance(Id,Name,_);wfTypeOf(Id,Name,map)),PropsInstance),
   sort(PropsInstance,Props),!.

getAllTypeProps(Id,Props):-
   wfSubclass(Id,Parent),
   findall(Name,wfGetPropValueInstance(Id,Name,_),PropsInstance),
   getAllTypeProps(Parent,ParentProps),
   append(PropsInstance,ParentProps,All),
   sort(All,Props),!.
getAllTypeProps(Id,Props):-
   findall(Name,wfGetPropValueInstance(Id,Name,_),PropsInstance),
   sort(PropsInstance,Props),!.

getValuesForProps(Id,[],[]):-!.
getValuesForProps(Id,[Prop|Sort],[Prop:PropValue|List]):-
      wfGetPropValue(Id,Prop,PropValue),
      getValuesForProps(Id,Sort,List).
getValuesForProps(Id,[Prop|Sort],List):-getValuesForProps(Id,Sort,List).

wfGetPropValue(Id,Prop,map(List)):-wfTypeOf(Id,Prop,map),!,
	    findall(PropValue,wfGetPropValueInstance(Id,Prop,PropValue),List),!.
wfGetPropValue(Id,Prop,PropValue):-wfGetPropValueInstance(Id,Prop,PropValue),!.
wfGetPropValue(Id,Prop,PropValue):-wfSubclass(Id,Parent),wfGetPropValueInstance(Parent,Prop,PropValue),!.


% =================================================================================
% =================================================================================
% Get The Position of any object
% =================================================================================
% =================================================================================
getPosition(list([float(X1),float(Y1),float(Z1)]),list([float(X1),float(Y1),float(Z1)])):-!.
getPosition(Agent,XYZ):-wfGetPropValueInstance(Agent,pos,XYZ),!.
getPosition(Agent,XYZ):-wfGetPropValueInstance(Agent,loc,T),getPosition(T,XYZ),!.
getPosition(Agent,XYZ):-wfSubclass(Agent,T),getPosition(T,XYZ),!.


      
% =================================================================================
% =================================================================================
% Get The Id of any object
% =================================================================================
% =================================================================================
getIdOut(map(Properties),Id):-!,getIdOut(Properties,Id),!.
getIdOut(list(Properties),Id):-!,getIdOut(Properties,Id),!.
getIdOut(concept(Properties),Id):-!,getIdOut(Properties,Id),!.
getIdOut(thing(Id),Id):-!.       
getIdOut(object(Type,Properties),Id):-!,getIdOut(Properties,Id),!.		      
getIdOut(Properties,Id):- (member(username:Id,Properties);member(id:Id,Properties);member(name:Id,Properties)),!.
getIdOut(Properties,Id):- member(args:Args,Properties),!,getIdOut(Args,Id).
getIdOut(_,Id):-!,getLocalSerial(Id).

% =================================================================================
% =================================================================================
% Get The List of any object
% =================================================================================
% =================================================================================
getListOut(map(Properties),Id):-!.
getListOut(list(Properties),Id):-!,getListOut(Properties,Id),!.
getListOut(concept(Properties),Id):-!,getListOut(Properties,Id),!.
getListOut(thing(Id),[id:Id]):-!.       
getListOut(object(Type,Properties),Id):-!,getListOut(Properties,Id),!.		      
getListOut(List,List):-!.

% =================================================================================
% =================================================================================
% Database Access
% =================================================================================
% =================================================================================
wfNearestSubclass(Id,Class):-wfGetPropValueInstance(Id,parents,list(List)),member(Class,List).
%wfNearestSubclass(Id,Class):-wfQuery(valueOf(Class,children,list(List))),member(Id,List).
wfSubclass(Id,Class):-wfNearestSubclass(Id,A),(Class=A;wfSubclass(A,Class)).
%wfSubclass(Id,Class):-var(Id),wfSubclass(A,Class),wfNearestSubclass(Id,A).
%wfSubclass(Id,Class):-nonvar(Id),wfNearestSubclass(Id,A),wfSubclass(A,Class).
%wfSubclass(Id,Class):-wfNearestSubclass(Id,A),wfNearestSubclass(A,Class).
%wfSubclass(Id,Class):-wfNearestSubclass(Id,A),wfNearestSubclass(A,B),wfNearestSubclass(B,Class).
%wfSubclass(Id,Class):-wfNearestSubclass(Id,A),wfNearestSubclass(A,B),wfNearestSubclass(B,C),wfNearestSubclass(C,Class).

wfTypeOf(Id,Name,Type):-wfQuery(typeOf(Id,Name,Type)).
wfGetPropValueInstance(Id,Name,Value):-wfQuery(valueOf(Id,Name,Value)).
wfGetPropValueInstance(Id,loc,Value):-wfQuery(valueOf(Value,contains,list(List))),member(Id,List).


% =================================================================================
% =================================================================================
% Utilities for login tracking
% =================================================================================
% =================================================================================

valueOfQ(X,P,list(Y)):-pc(P1,C),P==P1,
      findall(AL,valueOf(X,P,list([AL])),List),
      findall(Parent,(valueOf(Parent,C,list(XL)),member(X,XL)),List2),
      flatten([List,List2],U),sort(U,Y).

valueOfQ(X,C,list(Y)):-pc(P,C1),C==C1,
      findall(AL,valueOf(X,C,list([AL])),List),
      findall(Parent,(valueOf(Parent,P,list(XL)),member(X,XL)),List2),
      flatten([List,List2],U),sort(U,Y).

valueOfQ(X,C,list(Y)):-cl(C1,L),C1==C,
      findall(AL,valueOf(X,C,list([AL])),List),
      findall(In,valueOf(In,L,X),List2),
      flatten([List,List2],U),sort(U,Y).

valueOfQ(X,Loc,Y):-cl(Contains,Loc1),Loc1==Loc,
      valueOf(Room,Contains,list([AL])),
      member(X,AL),!.
valueOfQ(X,P,Y):-valueOf(X,P,Y).

pc(parents,children).
cl(contains,loc).

% =================================================================================
% =================================================================================
% Utilities for login tracking
% =================================================================================
% =================================================================================

getPlayerForCC(Channel,Agent,Id):-(isPlayerForCC(Channel,Agent,Id)),!.
getPlayerForCC(Channel,Agent,'').

getCharDefaultForCC(Channel,Agent,Id):-(isActiveForCC(Channel,Agent,Id)),wfSubclass(Id,account).

isUserNameValid(Username):-nonvar(Username),wfSubclass(Username,'account'),!.





mapFix(object(Type,Properties),Map):-
     (member(id:Id,Properties);member(name:Id,Properties)),!,      
      getObjectMap(Id,Map),!.
mapFix(objectRef(_,Type,Properties),Map):-
     (member(id:Id,Properties);member(name:Id,Properties)),!,      
      getObjectMap(Id,Map),!.
      
mapFix(Map,Map).


