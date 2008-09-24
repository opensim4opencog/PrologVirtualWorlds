:-include('logicmoo_utils_header.pl'). %<?
% =================================================================================
% =================================================================================
% Execute Load KIF command
% =================================================================================
% =================================================================================
xml_to_kif_command(Channel,Agent,file(FileName),Refno):-
      load_xml_file(FileName,Struct),convertAtlasXML(Struct),!.

%:-[o],!.

saveBBToKIF:-saveBBToKIF('MorafMt.kif').




saveBBToKIF(DataFile):-
   tell(DataFile),
   (Data),
   formatBB(Data),
   fail.
saveBBToKIF(DataFile):-told.

formatBB(typeOf(E,N,DType)):-!.
/*
formatBB(typeOf(E,N,DType)):-!,
      bbPredToKif(E,EKIF),
      formatBB(isa(EKIF,'#$Thing')),
      bbPredToKif(N,Pred),
      formatBB(isa(Pred,'#$ObjectPredicate')),
      bbPredToKif(DType,Type),
      formatBB(isa(Type,'#$ComputerDataType')),
      formatBB(wfArgIsa(Pred,2,Type)).
*/

% Any -> KIF
formatBB(valueOf(E,N,LValue)):-!,
      bbPredToKif(E,Entity),
      bbPredToKif(N,Pred),
      bbPredToKif(LValue,Value),
      KIF=..[Pred,Entity,Value],
      formatBB(KIF),!.

formatBB(V):-
     toMarkup(kif,V,_,O),
      say('~w~n',O).
            
      
bbPredToKif([],'#$TheEmptyList'):-!.
bbPredToKif([H|T],List):-!,bbPredToKifS([H|T],[HH|TT]),!,List=..['#$TheList',HH|TT].
bbPredToKif(list([]),'#$TheEmptyList'):-!.
bbPredToKif(list([H|T]),List):-!,bbPredToKifS([H|T],[HH|TT]),!,List=..['#$TheList',HH|TT].
bbPredToKif(map([]),'#$TheEmptySet'):-!.
bbPredToKif(map([H|T]),List):-!,bbPredToKifS([H|T],[HH|TT]),!,List=..['#$TheSet',HH|TT].
bbPredToKif(int(V),V):-!.
bbPredToKif(float(0),0.0):-!.
bbPredToKif(float(1),1.0):-!.
bbPredToKif(float(V),A):-!,sformat(S,'~f',V),!,string_to_atom(S,A).
bbPredToKif((N:V),'.'(NN,VV)):-!,bbPredToKif(N,NN),bbPredToKif(V,VV),!.
bbPredToKif(BB,BB):-not(atom(BB)),!.
bbPredToKif('',KIF):-string_to_atom(KIF,''),!.
bbPredToKif(BB,KIF):-atom_codes(BB,Codes),( member(S,"+ .,/"),member(S,Codes) ),string_to_atom(KIF,BB),!.
bbPredToKif(BB,KIF):-concat_atom(['#$',BB,'_wf'],KIF).

bbPredToKifS([],[]).
bbPredToKifS([H|T],[HH|TT]):-
      bbPredToKif(H,HH),
      bbPredToKifS(T,TT).

%(typeOf(world_0, contains, list)).
%(valueOf(world_0, bbox, list([float(0), float(0), float(0)]))).
%(typeOf(world_0, bbox, list)).
%(valueOf('_569_map', stamp, float(1.03004e+09))).
%(typeOf('_569_map', stamp, float)).
%(valueOf('_569_map', pos, list([float(87.0528), float(-172.061), float(3.36042e-11)]))).
%(typeOf('_569_map', pos, list)).
%(valueOf('_569_map', parents, list([hickory]))).


