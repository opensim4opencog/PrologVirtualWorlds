:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% Execute Set command
% =================================================================================
% =================================================================================

% Authorial KR for this OP
opHowto(['set'],[setEntityObject,sendInfoObjectRef]).


setObject(Id,Atom):-setEntity(Id,Atom).

% Save Entity
setEntity(Id,Atom):-
      writeSTDERR(setEntity(Id,Atom)),!,
	 setEntityR(Id,Atom),!,
	 stampObject(Id).


stampObject(Id):-get_time(S1970),setEntityProp(Id,stamp:float(S1970)),!.



setEntityR(X,Y):-not(ground((X,Y))),writeSTDERR(setEntityR(X,Y)),!.
setEntityR(Id,Atom):-atom(Atom),!.
setEntityR(Id,concept(Props)):-!,setEntityR(Id,Props).
setEntityR(Id,map(Props)):-!,setEntityR(Id,Props).
setEntityR(Id,[H|T]):-!,setEntityProp(Id,H),!,setEntityR(Id,T).


setEntityProp(N,P,V):-setEntityProp(N,P:V),!.
% Save Entity Proporties
setEntityProp(X,Y):-not(ground((X,Y))),writeSTDERR(setEntityProp(X,Y)),!.
setEntityProp(Id,N=V):-!,setEntityProp(Id,N:V).
setEntityProp(Id,Name:string(Value)):-!,sTypeOf(Id,Name,string),setIdNameValue(Id,Name,Value),!.
setEntityProp(Id,Name:int(Value)):-!,sTypeOf(Id,Name,int),atom_to_term(Value,ValueO,_),setIdNameValue(Id,Name,int(ValueO)),!.
setEntityProp(Id,Name:float(Value)):-!,sTypeOf(Id,Name,float),atom_to_term(Value,ValueO,_),setIdNameValue(Id,Name,float(ValueO)),!.
setEntityProp(Id,Name:list(List)):-!,sTypeOf(Id,Name,list),
	 cleanEntityProp(List,ListO),setIdNameValue(Id,Name,list(ListO)).
setEntityProp(Id,attributes:map(Map)):-!,setEntityR(Id,Map).
setEntityProp(Id,Name:map(Map)):-!,wfAssert(typeOf(Id,Name,map)),sTypeOf(Id,Name,_),setEntityItems(Id,Name,Map),!.
setEntityProp(Id,Name:Struct):-sTypeOf(Id,Name,string),setIdNameValue(Id,Name,Struct),!.

sTypeOf(Id,Name,_):-retractall((valueOf(Id,Name,_))).


cleanEntityProp([],[]).
cleanEntityProp(list(T),list(TT)):-!,cleanEntityProp(T,TT).
cleanEntityProp(map(T),map(TT)):-!,cleanEntityProp(T,TT).
cleanEntityProp(string(T),T):-!.
cleanEntityProp(':'(H,T),':'(HH,TT)):-!,
      cleanEntityProp(H,HH),
      cleanEntityProp(T,TT).
cleanEntityProp([H|T],[HH|TT]):-!,
      cleanEntityProp(H,HH),
      cleanEntityProp(T,TT).
cleanEntityProp(X,X).

% Save a list
setIdNameValue(Id,Name,list(Value)):-!,
	 wfAssert(valueOf(Id,Name,list(Value))).

setIdNameValue(Id,Name,string(Value)):-!,setIdNameValue(Id,Name,Value).

% Save all others
setIdNameValue(Id,Name,Value):-
	 convertToDatasystem(Value,ValueO),
	 wfAssert(valueOf(Id,Name,ValueO)).

% Save Maps unordered
setEntityItems(Id,Name,[Value|More]):-setEntityItems(Id,Name,Value),setEntityItems(Id,Name,More).
setEntityItems(Id,Name,Value):-atom(Value),!.
setEntityItems(Id,Name,Value):-setIdNameValue(Id,Name,Value),!.
setEntityItems(Id,Name,Value):-writeSTDERR(setEntityList(Id,Name,Value)).



