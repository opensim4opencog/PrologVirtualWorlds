:-include('logicmoo_utils_header.pl'). %<?
%MetaPredicate
isKeptInELPredicate('#$comment').
isKeptInELPredicate(F):-surfaceInference('#$completeExtentKnown'(F)).
isKeptInELPredicate(F):-surfaceInference('#$backchainForbidden'(F)).
%:- load_foreign_library(depth_of_var).

getVarId(A,B):-point_of_var(A,B).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).

varChecks(F,Pre,P,PVP):-!,fail.

varChecks(F,Pre,P,PVP):-nondet_arg(_,Pre,V),catch((depth_of_var_local(V,D),D>13),E,true),!,fail.
varChecks(F,Pre,P,PVP):-nondet_arg(_,Pre,T),atom(T),!,P. %,(ground(PVP);!).
%varChecks(F,Pre,P,PVP):-andCall(P). %,(ground(PVP);!).
      
:-dynamic(variable/1).
:-dynamic(variable/0).
:-dynamic(variable/2).
:-dynamic('#$passengers'/2).

'#$baseSet'('#$TheEmptyList', _G279):-!,fail.
'#$baseSet'(Structure,Set):-trace.
'#$causedBy'(_,_):-fail.
'#$causes-EventEvent'(_,_):-fail.
'#$transformedInto'(_,_):-fail.
:-dynamic( '#$coordinateValuePredicate'/2).

%:-ensure_loaded('isGenlsFlat.cache').
%:-ensure_loaded('isIsaFlat.cache').

:-dynamic('#$facets-Covering'/2).
'#$and'('#$isa'(E,C),A):-!,isNearestIsaCache(E,C),andCall(A).
'#$and'(A,'#$isa'(E,C)):-!,isNearestIsaCache(E,C),andCall(A).
'#$and'(A,B):-
      free_variables(A,Avs),length(Av,An),
      free_variables(B,Bvs),length(Bv,Bn),!,
      (An > Bn -> andCall(A,B) ; andCall(B,A)).

'#$and'(A,B,C):-'#$and'(A,B),andCall(C).
'#$and'(A,B,C,D):-'#$and'(A,B),'#$and'(C,D).
'#$and'(A,B,C,D,E):-'#$and'(A,B),'#$and'(C,D),andCall(E).
'#$and'(A,B,C,D,E,F):-'#$and'(A,B),'#$and'(C,D),'#$and'(E,F).
'#$and'(A,B,C,D,E,F,G):-'#$and'(A,B),'#$and'(C,D),'#$and'(E,F),andCall(G).
'#$and'(A,B,C,D,E,F,G,H):-'#$and'(A,B),'#$and'(C,D),'#$and'(E,F),'#$and'(G,H).
'#$and'(A,B,C,D,E,F,G,H,I):-'#$and'(A,B),'#$and'(C,D),'#$and'(E,F),'#$and'(G,H),andCall(I).
'#$and'(A,B,C,D,E,F,G,H,I,J):-'#$and'(A,B),'#$and'(C,D),'#$and'(E,F),'#$and'(G,H),'#$and'(I,J).

andCall(A,B):-andCall(A),andCall(B).

'#$equals'(X,Y):-
      (var(X);var(Y)),!,unify_with_occurs_check(X,Y).

andCall([H|T]):-atom(H),G=..[H|T],!,andCallL(G,H,T).
andCall(G):-G=..[H|T],!,andCallL(G,H,T).

:-index(andCallL(1,1,1)).

andCallL('#$genls'(E,C),'#$genls',[E,C]):-!,isGenlsCache(E,C).
andCallL('#$isa'(E,C),'#$isa',[E,C]):-!,isNearestIsaCache(E,C).
andCallL(G,'#$pointOfContact',Args):-!,fail.
andCallL(G,P,[A,B]):-'#$arg1Isa'(P,C1),'#$arg2Isa'(P,C2),!,isNearestIsaCache(A,C1),isNearestIsaCache(B,C2),G.
andCallL(G,P,[A,B,C]):-'#$arg1Isa'(P,C1),'#$arg2Isa'(P,C2),!,isNearestIsaCache(A,C1),isNearestIsaCache(B,C2),G.
andCallL(G,P,[A,B,C,D]):-'#$arg1Isa'(P,C1),'#$arg2Isa'(P,C2),!,isNearestIsaCache(A,C1),isNearestIsaCache(B,C2),G.
%andCallL(G,P,Args):-arg(_,G,V),atom(V),!,dynamic(G),writeq(G),nl,G.
andCallL(G,P,Args):-G.


callOnSentence(Goal,Mt):-Goal=..[F|Args],
    %  hashOnPred('#$backchainRequired', '%gaf#$Predicate', ['#$Predicate']).
	       hashOnPred(F,Hash, ArgList),
	       append([Hash,F|Args],[Mt],List),
	       Call=..List,!,(Call).
	       



%isGenlsClosureCache(E,S):-isNearestIsaCache(E,S).
isGenlsClosureCache(E,S):-isNearestIsaCache(E,C),!,isGenlsCache(C,S).

getPredicate(F):-argIsaTop(F, 1, _).


getHolds(Call,Mt):-call_with_depth_limit(getHolds2(Call,Mt),40,E),number(E).

getHolds2(Call,Mt):-var(Call),!,
      active('#$arity',F, A, _, _),
      atom(F),functor(Call,F,A),Call=..List,
      getlSavedHoldsList(List,Mt).

getHolds2(Call,Mt):-getlSavedHolds(Call,Mt).

getlSavedHolds([L|IST],Mt):-!,atom(L),getlSavedHoldsList([L|IST],Mt).
getlSavedHolds(Call,Mt):-compound(Call),Call=..List,!,getlSavedHoldsList(List,Mt). 

getlSavedHoldsList([L,I,S|T],Mt):-
      append([active|[L,I,S|T]],[Mt,Proof],RList),
      RCall=..RList,RCallRule=..RListRule,!,
      savedAccess(RCall).
/*
backchain('#$northeastOf', A, B, '#$GeographyMt', '#$implies'('#$and'('#$greaterThanOrEqualTo'(C, 0.5), '#$greaterThanOrEqualTo'(2, C), '#$greaterThan'(D, 0), '#$greaterThan'(E, 0), '#$latitude'(A, F), '#$longitude'(A, G), '#$different'(A, B), '#$latitude'(B, H), '#$longitude'(B, I), '#$evaluate'(C, '#$QuotientFn'(D, E)), '#$evaluate'(D, '#$DifferenceFn'(G, I)), '#$evaluate'(E, '#$DifferenceFn'(F, H))), '#$northeastOf'(A, B))):-callProtected('$AND'(11, (active('#$greaterThanOrEqualTo', C, 0.5, J, K), active('#$greaterThanOrEqualTo', 2, C, J, L), active('#$greaterThan', D, 0, J, M), active('#$greaterThan', E, 0, J, N), active('#$latitude', A, F, J, O), active('#$longitude', A, G, J, P), active('#$different', A, B, J, Q), active('#$latitude', B, H, J, R), active('#$longitude', B, I, J, S), active('#$evaluate', C, '#$QuotientFn'(D, E), J, T), active('#$evaluate', D, '#$DifferenceFn'(G, I), J, U), active('#$evaluate', E, '#$DifferenceFn'(F, H), J, V))), [A, B], '#$GeographyMt', J).
backchain('#$junctionInSystem', A, B, '#$BaseKB', '#$implies'('#$and'('#$greaterThanOrEqualTo'(C, 2), '#$cardinality'('#$SetOfLoopsAtNodeFn'(A, B), C)), '#$junctionInSystem'(A, B))):-callProtected('$AND'(1, (active('#$greaterThanOrEqualTo', C, 2, D, E), active('#$cardinality', '#$SetOfLoopsAtNodeFn'(A, B), C, D, F))), [A, B], '#$BaseKB', D).
backchain('#$junctionInSystem', A, B, '#$BaseKB', '#$implies'('#$and'('#$greaterThanOrEqualTo'(C, 3), '#$cardinality'('#$SetOfLinksAtNodeFn'(A, B), C)), '#$junctionInSystem'(A, B))):-callProtected('$AND'(1, (active('#$greaterThanOrEqualTo', C, 3, D, E), active('#$cardinality', '#$SetOfLinksAtNodeFn'(A, B), C, D, F))), [A, B], '#$BaseKB', D).
backchain('#$temporallySubsumes', A, '#$TimeIntervalInclusiveFn'('#$DateInSituationFn'(B, C, A), '#$DateInSituationFn'(B, D, A)), '#$BaseKB', '#$implies'('#$and'('#$greaterThanOrEqualTo'(D, C), '#$isa'(D, '#$PositiveInteger'), '#$greaterThan'(E, D), '#$isa'(C, '#$PositiveInteger'), '#$greaterThan'(C, 1), '#$endingDate'(A, '#$DateInSituationFn'(B, E, A))), '#$temporallySubsumes'(A, '#$TimeIntervalInclusiveFn'('#$DateInSituationFn'(B, C, A), '#$DateInSituationFn'(B, D, A))))):-callProtected('$AND'(5, (active('#$greaterThanOrEqualTo', D, C, F, G), active('#$isa', D, '#$PositiveInteger', F, H), active('#$greaterThan', E, D, F, I), active('#$isa', C, '#$PositiveInteger', F, J), active('#$greaterThan', C, 1, F, K), active('#$endingDate', A, '#$DateInSituationFn'(B, E, A), F, L))), [A, B, C, D], '#$BaseKB', F).
backchain('#$equals', A, '#$JoinListsFn'('#$SublistFromToFn'(A, 1, '#$DifferenceFn'(B, 1)), '#$SublistFromToFn'(A, B, C), '#$SublistFromToFn'(A, '#$PlusFn'(C, 1), D)), '#$BaseKB', '#$implies'('#$and'('#$greaterThanOrEqualTo'(C, B), '#$isa'(C, '#$PositiveInteger'), '#$greaterThan'(D, C), '#$isa'(B, '#$PositiveInteger'), '#$greaterThan'(B, 1), '#$evaluate'(D, '#$LengthOfListFn'(A))), '#$equals'(A, '#$JoinListsFn'('#$SublistFromToFn'(A, 1, '#$DifferenceFn'(B, 1)), '#$SublistFromToFn'(A, B, C), '#$SublistFromToFn'(A, '#$PlusFn'(C, 1), D))))):-callProtected('$AND'(5, (active('#$greaterThanOrEqualTo', C, B, E, F), active('#$isa', C, '#$PositiveInteger', E, G), active('#$greaterThan', D, C, E, H), active('#$isa', B, '#$PositiveInteger', E, I), active('#$greaterThan', B, 1, E, J), active('#$evaluate', D, '#$LengthOfListFn'(A), E, K))), [A, B, C, D], '#$BaseKB', E).
backchain('#$equals', A, [B, C, D], '#$BaseKB', '#$implies'('#$and'('#$greaterThanOrEqualTo'(D, C), '#$maxQuantValue'(A, [B, D]), '#$minQuantValue'(A, [B, C])), '#$equals'(A, [B, C, D]))):-callProtected('$AND'(2, (active('#$greaterThanOrEqualTo', D, C, E, F), active('#$maxQuantValue', A, [B, D], E, G), active('#$minQuantValue', A, [B, C], E, H))), [A, B, C, D], '#$BaseKB', E).
backchain('#$equals', A, [B, C, D], '#$BaseKB', '#$implies'('#$and'('#$greaterThanOrEqualTo'(D, C), '#$maxQuantValue'(A, [B, D]), '#$minQuantValue'(A, [B, C])), '#$equals'(A, [B, C, D]))):-callProtected('$AND'(2, (active('#$greaterThanOrEqualTo', D, C, E, F), active('#$maxQuantValue', A, [B, D], E, G), active('#$minQuantValue', A, [B, C], E, H))), [A, B, C, D], '#$BaseKB', E).
backchain('#$budgetExpenditureFractionOfGDP', A, B, C, '#$QuantityConversionFn'('#$Percent', '#$QuotientFn'(D, E)), '#$EconomyMt', '#$implies'('#$and'('#$grossDomesticProduct'(A, C, E), '#$budgetExpenditures'(A, B, C, D)), '#$budgetExpenditureFractionOfGDP'(A, B, C, '#$QuantityConversionFn'('#$Percent', '#$QuotientFn'(D, E))))):-callProtected('$AND'(1, (active('#$grossDomesticProduct', A, C, E, F, G), active('#$budgetExpenditures', A, B, C, D, F, H))), [A, B, C, D, E], '#$EconomyMt', F).
backchain('#$productionValueOfIndustryDuring', A, B, C, '#$TimesFn'(D, E), '#$EconomyMt', '#$implies'('#$and'('#$grossDomesticProduct'(A, C, D), '#$industryFractionOfGDP'(A, B, C, E)), '#$productionValueOfIndustryDuring'(A, B, C, '#$TimesFn'(D, E)))):-callProtected('$AND'(1, (active('#$grossDomesticProduct', A, C, D, F, G), active('#$industryFractionOfGDP', A, B, C, E, F, H))), [A, B, C, D, E], '#$EconomyMt', F).
backchain('#$industryFractionOfGDP', A, B, C, '#$QuantityConversionFn'('#$Percent', '#$QuotientFn'(D, E)), '#$EconomyMt', '#$implies'('#$and'('#$grossDomesticProduct'(A, C, E), '#$productionValueOfIndustryDuring'(A, B, C, D)), '#$industryFractionOfGDP'(A, B, C, '#$QuantityConversionFn'('#$Percent', '#$QuotientFn'(D, E))))):-callProtected('$AND'(1, (active('#$grossDomesticProduct', A, C, E, F, G), active('#$productionValueOfIndustryDuring', A, B, C, D, F, H))), [A, B, C, D, E], '#$EconomyMt', F).
backchain('#$grossDomesticProduct', A, '#$YearFn'('#$DifferenceFn'(B, 1)), '#$QuotientFn'(C, '#$PlusFn'(D, 1)), '#$EconomyMt', '#$implies'('#$and'('#$grossDomesticProduct'(A, '#$YearFn'(B), C), '#$realGDPGrowthRateDuring'(A, '#$YearFn'(B), D)), '#$grossDomesticProduct'(A, '#$YearFn'('#$DifferenceFn'(B, 1)), '#$QuotientFn'(C, '#$PlusFn'(D, 1))))):-callProtected('$AND'(1, (active('#$grossDomesticProduct', A, '#$YearFn'(B), C, E, F), active('#$realGDPGrowthRateDuring', A, '#$YearFn'(B), D, E, G))), [A, B, C, D], '#$EconomyMt', E).
backchain('#$numInhabitants', '#$Bahrain', A, '#$CIAWorldFactbook1997Mt', '#$implies'('#$and'('#$grossDomesticProduct'('#$Bahrain', '#$YearFn'(1996), ['#$USDollarFn'(1996), B]), '#$evaluate'(13000, '#$QuotientFn'(B, A))), '#$numInhabitants'('#$Bahrain', A))):-callProtected('$AND'(1, (active('#$grossDomesticProduct', '#$Bahrain', '#$YearFn'(1996), ['#$USDollarFn'(1996), B], C, D), active('#$evaluate', 13000, '#$QuotientFn'(B, A), C, E))), [A], '#$CIAWorldFactbook1997Mt', C).
backchain('#$above-Higher', A, B, '#$BaseKB', '#$implies'('#$and'('#$groupMembers'(C, A), '#$above-Higher'(C, D), '#$groupMembers'(D, B)), '#$above-Higher'(A, B))):-callProtected('$AND'(2, (active('#$groupMembers', C, A, E, F), active('#$above-Higher', C, D, E, G), active('#$groupMembers', D, B, E, H))), [A, B], '#$BaseKB', E).
backchain('#$behind-Generally', A, B, '#$BaseKB', '#$implies'('#$and'('#$groupMembers'(C, A), '#$behind-Generally'(C, B)), '#$behind-Generally'(A, B))):-callProtected('$AND'(1, (active('#$groupMembers', C, A, D, E), active('#$behind-Generally', C, B, D, F))), [A, B], '#$BaseKB', D).
backchain('#$colorOfObject', A, B, '#$BaseKB', '#$implies'('#$and'('#$groupMembers'(C, A), '#$colorOfObject'(C, B)), '#$colorOfObject'(A, B))):-callProtected('$AND'(1, (active('#$groupMembers', C, A, D, E), active('#$colorOfObject', C, B, D, F))), [A, B], '#$BaseKB', D).

*/

getlSavedHoldsListS([L],Mt):-!,getHolds2(L,Mt).
getlSavedHoldsListS([H|L],Mt):-getHolds2(H,Mt),getlSavedHoldsListS(L,Mt).

savedAccess(RCall,RCallRule,List,Ante):-RCall.
savedAccess(RCall,RCallRule,List,Ante):-RCallRule,
	    %(ground(List);ground(Ante)),
	    format(user_error,'~q.~n',[Ante]),
	    getlSavedHoldsAnte(Ante,Mt).

getlSavedHoldsAnte(['$call'(N,List)],Mt):-!,getlSavedHoldsListS(List,Mt).
getlSavedHoldsAnte('$call'(N,List),Mt):-!,getlSavedHoldsListS(List,Mt).
getlSavedHoldsAnte(List, Mt):-getlSavedHolds(List,Mt).


% ====================================================================
% Memo
% ====================================================================
:-dynamic(callCache/2).
% ====================================================================
%callProtected(backcall('#$isa', A, '#$NpcCol1003', B), 0, [A], varArgIsas(['#$isa'-1, '#$mudGold'-1], A), '#$JamudMt', B).

callProtected(AA,LemmaKey,HVsStruct,HVExec,Mt,MtNex):-
      nondet_arg(Num,HVsStruct,Arg),atom(Arg),
      not(recorded(LemmaKey,Num:Arg)),
      recordz(LemmaKey,Num:Arg),
      writeSTDERR(recordz(LemmaKey,HVsStruct:Num,AA)),
      getLegalGenlMt(Mt,MtNex),AA,
      (recorded(LemmaKey,HVsStruct) -> true ; recordz(LemmaKey,HVsStruct)),
      fail.

callProtected(AA,LemmaKey,HVsStruct,HVExec,Mt,MtNext):-
      %flag(LemmaKey,X,X+1),X<2,
      %writeSTDERR(using(LemmaKey,HVsStruct)),
      recorded(LemmaKey,HVsStruct).

      					   %lemmaB4726

varArgIsas(List,Var):-depth_of_var_local(Var,Depth),Depth<15.

%varArgIsas(List,Var):-depth_of_var_local(Var,Depth),(Depth<10;(format('~w~n',[depth_of_var_local(Var,Depth)]),!,fail)),!.

getLegalGenlMt(X,X).
%getLegalGenlMt(X,H):-active('#$genlMt',X,H,'#$BaseKB').

gafVars(Lemma,Vs,BindingRules):-!,fail.

% ====================================================================
% Tests
% ====================================================================
per:-isas('#$Person').
hum:-isas('#$Human').
pred:-isas('#$Predicate').
bk:-
   killAllCaches,
   bk2.

killAllCaches:-current_key(F),atom(F),atom_concat(lemma,_,F),recorded(F,_,Ref),erase(Ref),fail.
killAllCaches:-current_flag(F),atom(F),atom_concat(lemma,_,F),flag(F,_,0),fail.
killAllCaches.

bk2:-
   backchain(A,B,C,D),writeSTDERR(backchain(A,B,C,D)),fail.

isas(B):-topT('#$isa',I,B,D).


% ====================================================================
%  
% ====================================================================
topT(A,B,C,D):-topc(A,B,C,D),writeq(topc(A,B,C,D)),nl,fail.

%topc(A,B,C,D):-call_with_depth_limit(backcall(A,B,C,D),300,N),((N==depth_limit_exceeded,!);true).
topc(A,B,C,D):-backcall(A,B,C,D).

% Special Modules
backcall('#$holdsIn',B,C,D):-!,fail.

%backcall('#$hasDenotatum', '#$Quote'('#$EscapeQuote'(_G185)), '#$BaseKB')
% Arity 1
backcall(A,B,C):-active(A,B,C).


% Arity 2
backcall('#$termOfUnit',B,B,D):-!.
backcall(A,B,C,D):-active(A,B,C,D).
/*
backcall('#$isa',E,PC,Mt):-%nonvar(PC),!,
	active('#$genls',C,PC,_),%atom(C),atom(PC),
	backcall('#$isa',E,C,Mt),ground(E).
*/
backcall(Pred,_,_,_):-isNoBackchain2(Pred),!,fail.


backcall(A,B,C,D):-
      backchain(A,B,C,D),
   %   acceptedArg(A,'$predicate',1),
    %  acceptedArg(B,A,1),
     % acceptedArg(C,A,2),
      writeSTDERR(backchain_succeeded(A,B,C,D)).

isNoBackchain2(X):-not(atom(X)),!.
isNoBackchain2(X):-surfaceInference('#$isa'(X,'#$MetaPredicate')),!.
isNoBackchain2(X):-surfaceInference('#$completeExtentKnown'(X)),!.
isNoBackchain2(X):-surfaceInference('#$isa'(X,'#$MetaKnowledgePredicate')),!.
isNoBackchain2(X):-surfaceInference('#$isa'(X,'#$MetaLanguagePredicate')),!.
isNoBackchain2(X):-surfaceInference('#$isa'(X,'#$DocumentationPredicate')),!.


isNoBackchain2(X):-atom_codes(X,[35, 36|Name]),isNoBackchainCodes(Name). %"#$"
isNoBackchainCodes([97, 114, 103|_]). %"arg"
isNoBackchainCodes([114, 101, 115, 117, 108, 116|_]). %"result"
isNoBackchainCodes([105, 110, 116, 101, 114|_]). %"inter"
isNoBackchainCodes([103, 101, 110, 108|_]). %"genl"
isNoBackchainCodes([116, 101, 114, 109|_]). %"term"
isNoBackchainCodes([101, 113, 117|_]). %"equ"
                               

/*
isNoBackchain2('#$resultIsa').
isNoBackchain2('#$arg1Isa').
isNoBackchain2('#$arg2Isa').
isNoBackchain2('#$arg3Isa').
isNoBackchain2('#$arg4Isa').
isNoBackchain2('#$arg5Isa').
isNoBackchain2('#$resultGenl').
isNoBackchain2('#$arg1Genl').
isNoBackchain2('#$arg2Genl').
isNoBackchain2('#$arg3Genl').
isNoBackchain2('#$arg4Genl').
isNoBackchain2('#$arg5Genl').
*/

%isNoBackchain2Codes([X|_]):-X<97. %<"a"

% Arity 3
backcall(A,B,C,D,E):-active(A,B,C,D,E).
backcall(Pred,_,_,_,_):-isNoBackchain3(Pred),!,fail.
backcall(A,B,C,D,E):-backchain(A,B,C,D,E).

/*
backcall(A,B,C,D,E):-backchain(A,B,C,D,E),
      acceptedArg(A,'$predicate',1),
      acceptedArg(B,A,1),
      acceptedArg(C,A,2),
      acceptedArg(D,A,3).
*/

isNoBackchain3(X):-not(atom(X)),!.
isNoBackchain3(X):-atom_codes(X,[35, 36|Name]),isNoBackchainCodes(Name). %"#$"

% Arity 4

backcall(A,B,C,D,E,F):-active(A,B,C,D,E,F).
backcall(A,B,C,D,E,F):-backchain(A,B,C,D,E,F).
'$AND'(2,(A,B,C)):-'$AND'(1,(A,B)),C.
'$AND'(3,(A,B,C,D)):-'$AND'(1,(A,B)),'$AND'(1,(C,D)).
'$AND'(4,(A,B,C,D,E)):-'$AND'(1,(A,B)),'$AND'(1,(C,D)),E.
   
%'$AND'(2,(A,B,C)):-'$AND'(1,(A,B)),C.



acceptedArg(Arg,P,N):-not(compound(Arg)).

:-killAllCaches.


q :-q(X),
   writeln(X).

q(X) :-
   depth_of_var_local(X, D),
	format('Depth = ~w~n', [D]),
	D < 5,
	q(X),
	notail.

