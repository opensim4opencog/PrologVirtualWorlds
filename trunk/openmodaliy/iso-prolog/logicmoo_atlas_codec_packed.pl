:-include('logicmoo_utils_header.pl').
% =================================================================================
% Make packets higher level
% =================================================================================

reducePackedTok(In,Out):-convertPackedTok(In,Mid),!,
   (In=Mid -> Mid=Out ; reducePackedTok(Mid,Out)),!.
      
convertPackedTok([],[]).
convertPackedTok([H|T],[HH|TT]):-
   convertPackedTok(H,HH),
   convertPackedTok(T,TT),!.

%Maps to message
convertPackedTok(map(PackedTok),message(Concept,Packet)):-
      member(serialno:_,PackedTok),member(objtype:op,PackedTok),!,
      getPacketOf(PackedTok,Packet),
      removeData([to,from,serialno,name,id,refno,time_string,future_seconds,seconds],PackedTok,PackedTokO),!,
      convertPackedTok(concept(PackedTokO),Concept).

getPacketOf(PackedTok,packet(Channel,Refno,Agent,Serialno,Time)):-
      atlasMemberDefaulted(to,Channel,PackedTok,''),
      atlasMemberDefaulted(from,Agent,PackedTok,''),
      atlasMemberDefaulted(serialno,Serialno,PackedTok,0),
      atlasMemberDefaulted(refno,Refno,PackedTok,0),!,
      getTimeOf(PackedTok,Time).

%Maps to concept
convertPackedTok(map(PackedTok),O):-!,convertPackedTok(concept(PackedTok),O).

getTimeOf(PackedTok,Time):-
      atlasMemberDefaulted(seconds,Seconds,PackedTok,'0'),!,
      atlasMemberDefaulted(time_string,_,PackedTok,''),
      atlasMemberDefaulted(future_seconds,Future_seconds,PackedTok,'0'),!,
      atom_to_term(Seconds,Secs,_),
      atom_to_term(Future_seconds,FSecs,_),
      Time is Secs + FSecs,!.
getTimeOf(PackedTok,0).

%concept to objtyped
convertPackedTok(concept(PackedTok),Out):-
      member(objtype:Type,PackedTok),
      member(parents:list(Parents),PackedTok),
      convertPackedTok(objtyped(Type,Parents,PackedTok),Out).

%concept to id thing
convertPackedTok(concept([id:Thing]),thing(Thing)):-!. 
 
%

% Convert a typed entity to OP
convertPackedTok(objtyped(op, [OP], PackedTok),Out):-
      member(args:list([map(List)]),PackedTok),
      convertPackedTok(concept(List),ArgsO),
      Out=..[OP,ArgsO,PackedTok].

convertPackedTok(objtyped(op, [OP], PackedTok),Out):-Out=..[OP,PackedTok],!.

% Convert a typed entity to Object
convertPackedTok(objtyped(OBJ, [Parent], PackedTok),object(Parent,PackedTokO)):-member(OBJ,[object,obj]),!,
	 convertPackedTok(PackedTok,PackedTokO).

% Convert a typed entity to Object
convertPackedTok(objtyped(entity, [Parent], PackedTok),entity(Parent,PackedTokO)):-member(OBJ,[object,obj]),!,
	 convertPackedTok(PackedTok,PackedTokO).

% All others
convertPackedTok(O,O).
	 

% =================================================================================
% =================================================================================
% Utilities grabbing out list info
% =================================================================================
% =================================================================================

% get a argument
atlasMemberValue(Name,Value,List):-member(Name:Thing,List),Thing=..[_,Value],!.
atlasMemberValue(Name,Value,List):-member(Name:Value,List).

% get a argument but fall back to a default
atlasMemberDefaulted(Name,OValue,PackedTok,Default):-member(Name:Value,PackedTok),Value=..[_,OValue],!.
atlasMemberDefaulted(Name,Value,PackedTok,Default):-member(Name:Value,PackedTok),!.
atlasMemberDefaulted(Name,Default,PackedTok,Default).


removeData(Attribs,[],[]).
removeData(Attribs,[In:_|InS],Out):-
	   member(In,Attribs),!,removeData(Attribs,InS,Out).
removeData(Attribs,[In|InS],[In|Out]):-
	 removeData(Attribs,InS,Out).


