:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% Execute Load XML command
% =================================================================================
% =================================================================================
load_xml_command(Channel,Agent,file(FileName),Refno):-
      load_xml_file(FileName,Struct),processAtlasXML(Struct),!.

%:-[o],!.

buildDefaultAtlasXML:-
      load_xml_command(Channel,Agent,file('../world/atlas.xml'),Refno),
      load_xml_command(Channel,Agent,file('../world/mason.xml'),Refno),
      load_xml_command(Channel,Agent,file('../world/basicworld.xml'),Refno),
%      load_xml_command(Channel,Agent,file('agrilan.xml'),Refno),
      load_xml_command(Channel,Agent,file('../world/moraf.xml'),Refno),
      saveWorld,
      !.

processAtlasXML(Atom):-atom(Atom),!.
processAtlasXML(element(atlas,_,List)):-!,processAtlasXML(List).
processAtlasXML(element(obj,_,List)):-!,processAtlasXML(List).
processAtlasXML(element(xml,_,List)):-!,processAtlasXML(List).
processAtlasXML([H|T]):-
      processAtlasXML(H),!,
      processAtlasXML(T).

processAtlasXML(element(map,[name=What],XML)):-!,convertToDatasystem([id:What|XML],Data),setEntity(What,map(Data)).

processAtlasXML(element(map,[],XML)):-
	 member(element(string, [name=id], [What]),XML),!,convertToDatasystem(XML,Data),setEntity(What,map(Data)).
processAtlasXML(element(map,[],XML)):-
	 member(element(uri, [name=id], [What]),XML),!,convertToDatasystem(XML,Data),setEntity(What,map(Data)).
processAtlasXML(Struct):-!,writeSTDERROnDebug(processAtlasXML(Struct)).

% =================================================================================
% =================================================================================
% Deserialze Anything (Including XML)
% =================================================================================
% =================================================================================

convertToDatasystem(Var,Var):-var(Var),!,fail.
convertToDatasystem([],[]):-!.
convertToDatasystem([H|T],TT):-atom(H),!,
      convertToDatasystem(T,TT).
convertToDatasystem([H|T],[HH|TT]):-!,
      convertToDatasystem(H,HH),
      convertToDatasystem(T,TT).

% Hack parent to list
convertToDatasystem(element(string,[name=parent],[Value]),parents:list([(Value)])):-!.
% Hack parent to list
convertToDatasystem(element(string,[name=parent],Value),O):-!,
      convertToDatasystem(element(string,[name=parents],Value),O).

convertToDatasystem(element(string(Foo),Mid,Value),O):-!,
      convertToDatasystem(element(Foo,Mid,Value),O).

%convertToDatasystem(element(O,_, _),_):-compound(O),!,fail.


% Partialy converted
convertToDatasystem(N=V,O):-!,convertToDatasystem(N:V,O).
convertToDatasystem(N:V,N:O):-!,convertToDatasystem(V,O).

      

% XML deconversion
convertToDatasystem(element(int,[],[Value]),int(ValueO)):-atom_to_term(Value,ValueO,_).
convertToDatasystem(element(float,[],[Value]),float(ValueO)):-atom_to_term(Value,ValueO,_).
convertToDatasystem(element(string,[],[]),string('')).
convertToDatasystem(element(string,[],[Value]),Value).
convertToDatasystem(element(map,[],Stuff),map(StuffO)):-!,convertToDatasystem(Stuff,StuffO).
convertToDatasystem(element(list,[],Stuff),list(StuffO)):-!,convertToDatasystem(Stuff,StuffO).

% XML deconversion for basicworld.xml
convertToDatasystem(element(EType,[],V),Out):-wfSubclass(EType,SuperType),
	    convertToDatasystem(element(SuperType,[],V),Out).

% Name value pairs
convertToDatasystem(element(Type,[name=Name],Value),Name:ValueO):-!,
      convertToDatasystem(element(Type,[],Value),ValueO),!.

% Already converterted
convertToDatasystem(string(I),I):-!.
convertToDatasystem(int(I),int(I)):-!.
convertToDatasystem(float(I),float(I)):-!.
convertToDatasystem(real(I),float(I)):-!.
convertToDatasystem(map(I),map(O)):-!,convertToDatasystem(I,O).
convertToDatasystem(list(I),list(O)):-!,convertToDatasystem(I,O).

% Needs deducced conversion
convertToDatasystem(I,int(I)):-integer(I).
convertToDatasystem(I,float(I)):-number(I).
convertToDatasystem(I,(I)):-is_string(I),!.
convertToDatasystem(I,(I)):-atom(I),!.

convertToDatasystem(Value,Value):-!,writeq(Value),nl,trace.



