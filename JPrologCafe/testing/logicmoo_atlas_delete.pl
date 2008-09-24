:-include('logicmoo_utils_header.pl'). %<?
% =================================================================================
% =================================================================================
% Execute Delete command
% =================================================================================
% =================================================================================
% Authorial KR for this OP
opHowto(['delete'],[deleteObject]).

clientEvent(Channel,Agent,english(phrase([delete,Id],_),packet(Channel,Serial,Agent,Refno,Time))):-!,
      writeSTDERR(deleting(Id,ToSet)),!,
      retractall((typeOf(Id,_,_))),
      retractall((valueOf(Id,_,_))),!.

      % delete Something 
clientEvent(Channel,Agent,message(delete(Object,OPTokens),packet(_,_,_,Refno,_))):-!,
      getIdOut(Object,Id),
      writeSTDERR(deleting(Id,ToSet)),!,
      retractall((typeOf(Id,_,_))),
      retractall((valueOf(Id,_,_))),!.


killConstantInSurface(Const):-      
      retractall(saved(_,Const,_,_)),
      saved(Data,Mt,Vars,Lits),
      memberchk(Const,Lits),
      retract(saved(Data,Mt,Vars,Lits)),fail.
killConstantInSurface(Const):-!.     % how much does it cost to host your own world on your own server machine?



best:-ensure_loaded('cyc.el'),
      retractall(saved('IL', _,_,_)),
      retractall(saved('#$afterAdding'(_,_),_,_,_)),
      retractall(saved('#$afterRemoving'(_,_),_,_,_)),
      retractall(saved(_, '#$JamudMt',_,_)),
      retractall(saved(_, '#$InfernencePSC',_,_)),
      retractall(saved(_, '#$PrologDataMt',_,_)),
      retractall(saved(_, '#$ElizaMt',_,_)),
      retractall(saved(_, '#$EverythingPSC',_,_)),
      retractall(saved('#$termOfUnit'(_,_),_,_,_)),
      retractall(saved(_, '#$LogicMoo',_,_)),
      retractall(saved('#$isa'(_, '#$Thing'),_,_,_)),
      retractall(saved('#$genls'(X,X),_,_,_)),
      retractall(saved('#$myCreationTime'(_,_),_,_,_)),
      retractall(saved('#$myCreator'(_,_),_,_,_)),
      retractall(saved('#$oldConstantName'(_,_),_,_,_)),
/*
      killConstantInSurface('#$EIAPagesMt'),
      killConstantInSurface('#$CSIS-CreepingProliferationMt'),
      killConstantInSurface('#$ModelFragmentT24Mt'),
      killConstantInSurface('#$SaudiArabia-IranMilitaryBalanceMt'),
      killConstantInSurface('#$CSIS-WarfightingMt'),
      killConstantInSurface('#$NDU-DenialAndJeopardy-Ch5Mt'),
      killConstantInSurface('#$TerrorismThreatInMiddleEastMt'),
      killConstantInSurface('#$StrategicAssessment1997Mt'),
      killConstantInSurface('#$INTERPOLMt'),
      killConstantInSurface('#$HPKBScenario-AllStagesMt'),
      killConstantInSurface('#$HPKBScenario-Stage1Mt'),
      killConstantInSurface('#$HPKBScenario-Stage2Mt'),
      killConstantInSurface('#$HPKBScenario-Stage3Mt'),
      killConstantInSurface('#$IAEAMt'),
      killConstantInSurface('#$US-GCCSecurityRelationsMt'),
      killConstantInSurface('#$Stage1-LibyanScientist1'),
      killConstantInSurface('#$Stage1-LibyanScientist2'),
*/
            !.
%:-ensure_loaded('save.image').

% he mtch # is 874-676-8291 you'll need ID

%go:-startCycAPIServer.


