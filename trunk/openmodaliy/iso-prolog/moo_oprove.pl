% ==========================================
% BELIEF
% ==========================================

% :-include('moo_header.pl').

:-dynamic(complete_goal/1).

:-multifile(expireOptimizationsInContext/3).

:-index(prove_goal(0,0,0,1,0,0,0)).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

% :-ensure_loaded(library(occurs)).

% ======================================================
% REQUEST PROC POSITIVE/NEGATIVE
% ======================================================
/*
agentRequest(KIFCharsIn,FmlInOpen,Vars,Ctx,Context,User,UResultsSoFar,Result,ExplainationOut,Status):-
	TN = User, % Tracks request based on 'User'
	destroyTN(Context,TN,_Ctx),	% Removes previous Request
	getOpenVariablesWFF(FmlInOpen,Vars,ChaseVars),
	getPrologVars(ChaseVars,QVars,_,_),
	RequestPrototype=..[request|QVars],
	getRequestClauses(Context,'<=>'(FmlInOpen,RequestPrototype),ConjAssertsClauses,Vars,Flags),	
	createRequestAssertions(true,ConjAssertsClauses,CAN),!,
	assert(mooDatabase('<=>'(FmlInOpen,RequestPrototype),CAN,Flags,Vars,Context,TN,User,on)),
	once(recanonicalizeTN(Context,TN)),    % Compile Belief
	assert(tq_attempted_request), 
        unnumbervars((FmlInOpen,RequestPrototype,ChaseVars),(UFmlInOpen,URequest,UChaseVars)),
	writeDebug(blue,'Stage 1 - Compilation ':CAN:Flags),!,
	agentRequestEach(UFmlInOpen,URequest,UChaseVars,Ctx,Context,User,UResultsSoFar,Result,Explaination,Status),
	commitCleanExplaination(Explaination,ExplainationOut).

% Check For Theorem
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,User,1,['Result' = 'True'],formula(instance(FormulaIN,'Theorem')),done(true:thereom)):-
	resetTableFlags,
	writeDebug(purple,'Stage 2 - Theorem Check ':FmlInOpen),
	isTheorem(FmlInOpen,_),!,
	retain_yes,sendNote('(theorem)'),!.
  
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,User,UA, UVars,Explaination,Result):-
	agentBelief(FmlInOpen,URequest,UVars,Ctx,Context,User,UA, UVars, Explaination,Result).

% Request Failed
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,User,0,['Result'='none'|_],'Unproven',done(possible:searchfailed)):-
	flag('UAnswers',UA,UA),UA<1,!.

% Request Succeeded
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,User,UA,['Result'='true'|_],found(UA),done(true:UA)):-!,
	flag('UAnswers',UA,UA).      


getRequestDefaults(URequest,OAnswers,BackchainsMax,Deductions):-
        (getMooOption(opt_backchains_max=BackchainsMax)),
        (getMooOption(opt_deductions_max=Deductions)),!,
        ignore((ground(URequest) -> Answers=1 ; Answers=PAnswers)),
	(getMooOption(opt_answers_max=Answers)),!,
	ignore(BackchainsMax=30),ignore(Answers=60),OAnswers is Answers,!.

set_quit_time(Num):-
	(getMooOption(opt_timeout=QuitTime)),!,ignore(QuitTime=5),!,
	retractall(cpuend(_)),    
	getCputime(Now),
	Then is Now + QuitTime*Num,
	asserta(cpuend(Then)),!.

:-dynamic(noexistencials/0).



%edify_vars(X,X):-!.
edify_vars(Var,Var):-var(Var),!.
edify_vars([],[]):-!.
edify_vars([N=V|T],[N=RV|NT]):-
            eval_lr(V,RV),!,retain_answer(RV),
            edify_vars(T,NT),!.
edify_vars([H|T],[H|NT]):-
            edify_vars(T,NT),!.
*/





/*

agentBelief(FmlInOpen,Literal,VarsRequested,Ctx,Context,User,UA, UVars,[All|Explaination],done(true:UA)):- 
	writeDebug(green,'Stage 3 - Positive ':FmlInOpen ),
	deduceGoalOProve(findAllUsingGafsAndOrBackResolution(Literal,VarsRequested,10),VarsRequested,Ctx,Context, [All|Explaination]),
	length([All|Explaination],UA),!.

agentBelief(FmlInOpen,NLiteral,VarsRequested,Ctx,Context,User,UA, UVars,[All|Explaination],done(false:UA)):- 
	NLiteral=..[F|Args],
	(atom_concat('~',FN,F);atom_concat('~',F,FN)),
	Literal=..[FN|Args],!,
	writeDebug(red,'Stage 4 -Negative ':FmlInOpen ),
	deduceGoalOProve(findSingleUsingGafsAndOrBackResolution(Literal,VarsRequested,10),VarsRequested,Ctx,Context, [All|Explaination]),
	length([All|Explaination],UA),!.
*/

/*

mooCache('~subclass'(v('Abstract', A, ['Class'|B]), v('Abstract', 'Entity', ['Class'|C])), guard([A, B, C], [D, E, F], '~instance'(v('Abstract', D, ['Class'|E]), v('Abstract', 'Class', ['Class', 'Abstract', 'Class'|G])), 4, ['CLASS'=D], [D], [], [], [], [D], '~subclass'(v('Abstract', D, ['Class'|E]), v('Abstract', 'Entity', ['Class'|F]))), not(subclass), 'PrologMOO', 'BASE ONTOLOGY', 637).
mooCache(instance(v('Abstract', A, ['Class'|B]), v('Abstract', 'Class', ['Class', 'Abstract', 'Class'|C])), guard([A, B, C], [D, E, F], subclass(v('Abstract', D, ['Class'|E]), v('Abstract', 'Entity', ['Class'|G])), 3, ['CLASS'=D], [D], [], [], [], [D], instance(v('Abstract', D, ['Class'|E]), v('Abstract', 'Class', ['Class', 'Abstract', 'Class'|F]))), instance, 'PrologMOO', 'BASE ONTOLOGY', 637).
mooCache('~instance'(v('Abstract', A, ['Class'|B]), v('Abstract', 'Class', ['Class', 'Abstract', 'Class'|C])), guard([A, B, C], [D, E, F], '~subclass'(v('Abstract', D, ['Class'|E]), v('Abstract', 'Entity', ['Class'|G])), 2, ['CLASS'=D], [D], [], [], [], [D], '~instance'(v('Abstract', D, ['Class'|E]), v('Abstract', 'Class', ['Class', 'Abstract', 'Class'|F]))), not(instance), 'PrologMOO', 'BASE ONTOLOGY', 637).
mooCache('~subclass'(v('Abstract', A, ['Class'|B]), v('Abstract', 'Entity', ['Class'|C])), guard([A, B, C], [D, E, F], '~instance'(v('Abstract', D, ['Class'|E]), v('Abstract', 'Class', ['Class', 'Abstract', 'Class'|G])), 4, ['CLASS'=D], [D], [], [], [], [D], '~subclass'(v('Abstract', D, ['Class'|E]), v('Abstract', 'Entity', ['Class'|F]))), not(subclass), 'PrologMOO', 'BASE ONTOLOGY', 637).
mooCache(instance(v('Abstract', A, ['Class'|B]), v('Abstract', 'Class', ['Class', 'Abstract', 'Class'|C])), guard([A, B, C], [D, E, F], subclass(v('Abstract', D, ['Class'|E]), v('Abstract', 'Entity', ['Class'|G])), 3, ['CLASS'=D], [D], [], [], [], [D], instance(v('Abstract', D, ['Class'|E]), v('Abstract', 'Class', ['Class', 'Abstract', 'Class'|F]))), instance, 'PrologMOO', 'BASE ONTOLOGY', 637).
mooCache('~instance'(v('Abstract', A, ['Class'|B]), v('Abstract', 'Class', ['Class', 'Abstract', 'Class'|C])), guard([A, B, C], [D, E, F], '~subclass'(v('Abstract', D, ['Class'|E]), v('Abstract', 'Entity', ['Class'|G])), 2, ['CLASS'=D], [D], [], [], [], [D], '~instance'(v('Abstract', D, ['Class'|E]), v('Abstract', 'Class', ['Class', 'Abstract', 'Class'|F]))), not(instance), 'PrologMOO', 'BASE ONTOLOGY', 637).

	[mooCache(RealHead,guard(
		RFVH,FVH,
			Body,
			CLID,KRVars,RuleVars,
			UnivHead,
			BodyUniv,
			BodySelfConnected,Shared,Head),Key,Context,TN)]):-!, 


     %   recorda(Functor,RealHead,FRef),
	%stepQualifier(Ref,Functor,RealHead:Head,RFVH:FVH,Body,TN,CID,KRVars,RuleVars,UnivHead,BodyUniv,BodySelfConnected,RealShared,Qualifier),
	%findall(Body,call_with_depth_limit(block(Ref,callBody(Body,true),Result),47,_),Sols),!,	
	%findall(RFVH,(deduceGoalOProve(Ref,Body)),Sols)
	%findall(Body,Body,Sols),!,
	%sort(Sols,SolsS)

*/

%ensureKey(Literal,Proto,HashKey,Depth,HashKey):-nonvar(HashKey),!.
%ensureKey(Literal,Proto,HashKey,Depth,HashKey):-nonvar(Proto),!,hash_term(Proto,HashKey).
ensureKey(Literal,Proto,HashKey,Depth,HashKey):-
	copy_term(Literal,Proto),numbervars(Proto,'$',0,Depth),hash_term(Proto,HashKey),!.

% deduceGoalOProve(Todo,VarsIn,Ctx,Context, AccumulatedOut).
deduceGoalOProve(Todo,VarsIn,Ctx,Context, AccumulatedOut):-
	writeDebug((Todo-VarsIn-Ctx)),fail.
	
      
% Find Single (UsingGafs/UsingBackResolution) Gaf Part
deduceGoalOProve(findSingleUsingGafsAndOrBackResolution(Literal,UVars,Depth),VarsRequested,Ctx,Context, Result):-
	deduceGoalOProve(findSingleUsingGafs(Literal,UVars,Depth),VarsRequested,Ctx,Context, Result),!.

% Find Single (UsingGafs/UsingBackResolution) BackResolution Part
deduceGoalOProve(findSingleUsingGafsAndOrBackResolution(Literal,UVars,Depth),VarsRequested,Ctx,Context, Result):-
	deduceGoalOProve(findSingleUsingBackResolution(Literal,UVars,Depth),VarsRequested,Ctx,Context, Result),!.
	
% Find Single (UsingGafs) mooDatabase Part
deduceGoalOProve(findSingleUsingGafs(Literal,UVars,Depth),VarsRequested,Ctx,Context, [Accum]):-
	mooDatabase(Literal, _, Context, PCtx, Explaination),
	putIntoZ(Literal,Key,HashKey,proved(Literal,Explaination),Accum),!.

mooDatabase(Literal, How, Context, Ctx, Explaination):-
	mooCache(Literal, How, Context, Ctx, Explaination).
mooDatabase(Literal, ReQlist, How, Context, Ctx, Explaination):-
	mooCache(Literal, How,ReQlist, Context, Ctx, Explaination).


% Find Single (UsingBackResolution) mooDatabase Part
deduceGoalOProve(findSingleUsingBackResolution(Literal,UVars,Depth),VarsRequested,Ctx,Context,AccumulatedOut):- !,
	shouldCanBackChainOn(Literal,UVars,Depth),
	not(not(mooDatabase(Literal,Reqs,_,Context,_Ctx,Explaination))),
	ensureKey(Literal,Key,HashKey,_,Pointer),!,
	findall(Sum-Info,(
			mooDatabase(Literal, ReqList,Type,Context,__Ctx,Explaination),
			examineRuleForInfo(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,Explaination,ReqList,Info,Sum)
		),[All|Infomation]),
	keysort([All|Infomation],RAllInfomation),
	deduceGoalOProve(nowProveOneWithInfoFromDisj([All|Infomation]),VarsRequested,Ctx,Context,AccumulatedOut).



% Find Single (UsingBackResolution) With Lits
deduceGoalOProve(nowProveOneWithInfoFromDisj(Infomation),VarsRequested,Ctx,Context,AccumulatedOut):-
	member(First,Infomation),
	deduceGoalOProve(nowProveWithInfoConj(First),VarsRequested,Ctx,Context,AccumulatedOut),!.
	
% Find All (UsingGafs/UsingBackResolution) 
deduceGoalOProve(findAllUsingGafsAndOrBackResolution(Literal,UVars,Depth),VarsRequested,Ctx,Context, Result):-
	deduceGoalOProve(findAllUsingGafs(Literal,UVars,Depth),VarsRequested,Ctx,Context, GafResults),!,
	deduceGoalOProve(findAllUsingBackResolution(Literal,UVars,Depth),VarsRequested,Ctx,Context, BKResults),
	append(GafResults,BKResults,Result).

% Find All UsingGafs
deduceGoalOProve(findAllUsingGafs(Literal,UVars,Depth),VarsRequested,Ctx,Context, All):-
	ensureKey(Literal,Key,HashKey,VCount,Pointer),
	findall(Accum,(mooDatabase(Literal, _, Context, _Ctx, Explaination),putIntoZ(Literal,Key,HashKey,proved(Literal,Explaination)),Accum),All),!.

% Find All UsingBackResolution
deduceGoalOProve(findAllUsingBackResolution(Literal,UVars,Depth),VarsRequested,Ctx,Context, AccumulatedOut):-
	%true,
	ensureKey(Literal,Key,HashKey,VCount,Pointer),!,
	%VCount < Depth,
	not(recorded(findAllUsingBackResolution,Literal)),
	recorda(findAllUsingBackResolution,Literal),
	%shouldCanBackChainOn(Literal,UVars,Depth),!,
	findall(Sum - Info,(
			mooDatabase(Literal, ReqList,Type,Context,__Ctx,Explaination),
			examineRuleForInfo(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,Explaination,ReqList,Info,Sum)
		),[All|Infomation]),
	keysort([All|Infomation],RAllInfomation),
	reverse(RAllInfomation,AllInfomation),!,
	deduceGoalOProve(proveAllWithInfoDisj(AllInfomation),VarsRequested,Ctx,Context,AccumulatedOut).
			

recordaLogged(A,B):-recordaLogged(A,B,C).
recordaLogged(A,B,C):-
	recorda(A,B,C),writeDebug(recorda(A,B)).

recordzLogged(A,B):-recordzLogged(A,B,C).
recordzLogged(A,B,C):-
	recordz(A,B,C),writeDebug(recordz(A,B)).

recordedLogged(A,B):-recordedLogged(A,B,C).
recordedLogged(A,B,C):-recorded(A,B,C),!,writeDebug(recorded(A,B,C)).
	
%recordedLogged(A,B,C):- writeDebug(recordedChk(A,B)),recorded(A,B,C),writeDebug(recorded(C)).

% recordAwakensReq(ReqPointer,Req,ReqKey,ReqKeys).

:-dynamic(u/2).

count_unique_keys(X):-
	retractall(u(A,B)),
	mooDatabase(Literal, ReqList,Type,Context,__Ctx,Explaination),
	ensureKey(Literal,ReqKey,Pointer,VCount,_Pointer),
	not(u(ReqKey,ReqKey)),
	assert(u(ReqKey,ReqKey)),
	write(ReqKey=Literal),nl,
	fail.
	
count_unique_keys(X):-predicate_property(u(_,_),X).
count_unique_keys(X):-predicate_property(mooDatabase(Literal, ReqList,Type,Context,__Ctx,Explaination),X).
	
	



recordAwakensReq(ReqPointer,Req,ReqKey,[ReqKey]).
recordAwakensReq(ReqPointer,Req,ReqKey,Original):-
	delete(Original,ReqKey,Neibs),
	recordaLogged(ReqPointer,awakens(Req,Neibs),RefGives),!.

testReqListViability([Req],[lit(Req,ReqKey,VCount,Pointer)],[ReqKey]):-!,
	ensureKey(Req,ReqKey,Pointer,VCount,_Pointer),!.
	%findall(GeneralFalses,(recordedLogged(Pointer,mostGeneralFalses(GeneralFalses)),Req=GeneralFalses),[]),!.
			
testReqListViability([Req|List],[lit(Req,ReqKey,VCount,Pointer)|Pointers],[ReqKey|ReqKeyS]):-
	ensureKey(Req,ReqKey,Pointer,VCount,_Pointer),!,
       %findall(GeneralFalses,(recordedLogged(Pointer,mostGeneralFalses(GeneralFalses)),Req=GeneralFalses),[]),
	testReqListViability(List,Pointers,ReqKeyS),!.

examineRuleForInfo(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,Explaination,ReqList,Info,Sum):-
	not(recorded(Pointer,Explaination)),
	recorda(Pointer,Explaination),
	testReqListViability(ReqList,RecPointers,ReqKeys),
	%true,
	%write(ReqList),nl,nl,
	examineRuleForProving(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,RecPointers,ReqKeys,Explaination,KeyInfo),
	keysort(KeyInfo,RKeyInfo),
	reverse(RKeyInfo,Info),
	sumkeys(Info,Sum),!.
		
examineRuleForProving(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,[Item],ReqKeys,Explaination,[Info]):-    
	examineRuleForProving(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,Item,ReqKeys,Explaination,Info),!.
examineRuleForProving(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,[Item|More],ReqKeys,Explaination,[Info|InfoR]):-!,
	examineRuleForProving(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,Item,ReqKeys,Explaination,Info),
	examineRuleForProving(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,More,ReqKeys,Explaination,InfoR),!.

examineRuleForProving(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,lit(Req,ReqKey,RVCount,ReqPointer),ReqKeys,Explaination,Info):-
	recorded(ReqPointer,'-'(Len,OtherLitKeys),Ref),
	true,nonvar(Len),!,
	examineRuleForProving2(Literal,Key,Len,OtherLitKeys,Ref,VCount,Pointer,VarsRequested,UVars,Depth,lit(Req,ReqKey,RVCount,ReqPointer),ReqKeys,Explaination,Info),!.

examineRuleForProving2(Literal,Key,Len,OtherLitKeys,Ref,VCount,Pointer,VarsRequested,UVars,Depth,lit(Req,ReqKey,RVCount,ReqPointer),ReqKeys,Explaination,
	nonvar(Ref),
	NLen - delayed(Req,ReqPointer,UVars,[Key|OtherLitKeys])):-
	not(not(memberchk(Key,OtherLitKeys))),erase(Ref),
        true,
	NLen is Len + 1,recordaLogged(ReqPointer,(NLen-[Key|OtherLitKeys])),!.

examineRuleForProving2(Literal,Key,Len,OtherLitKeys,Ref,VCount,Pointer,VarsRequested,UVars,Depth,lit(Req,ReqKey,RVCount,ReqPointer),ReqKeys,Explaination,
	Len - checkTable(Req,ReqPointer,UVars,OtherLitKeys)):-
	recordAwakensReq(ReqPointer,Req,ReqKey,ReqKeys),!.
		
examineRuleForProving(Literal,Key,VCount,Pointer,VarsRequested,UVars,Depth,lit(Req,ReqKey,RVCount,ReqPointer),ReqKeys,Explaination,
	(RVCount - req(Req,ReqPointer,UVars))):-!.

% Prove One or More
deduceGoalOProve(proveAllWithInfoDisj(Infomation),VarsRequested,Ctx,Context,AccumulatedOut):-
	findall([Accu|MulatedOut],( 
			member(Cost-First,Infomation),
			findall(VarsRequested,deduceGoalOProve(nowProveWithInfoConj(Cost,First),VarsRequested,Ctx,Context,AccumulatedMid),[Accu|MulatedOut])
		),AccumulatedOut).
	

% Prove Single Item in Conj Then do the rest
deduceGoalOProve(nowProveWithInfoConj(_,[]),VarsRequested,Ctx,Context,[]):-!.
deduceGoalOProve(nowProveWithInfoConj(Cost,[First|Rest]),VarsRequested,Ctx,Context,Accumulated):-
	deduceGoalOProve((First),VarsRequested,Ctx,Context,Results),
	deduceGoalOProve(nowProveWithInfoConj(Cost,FirsRest),VarsRequested,Ctx,Context,AccumulatedMid),
	append(Results,AccumulatedMid,Accumulated).

% ==================================================	
% Literal Resoltuon
% ==================================================	

% Prove For Self Only  Means no goals where blocked on this generalization
deduceGoalOProve((Priorty-req(Req,ReqPointer,UVars)),VarsRequested,Ctx,Context,[Re|Sults]):-!,
	(varintersection(VarsRequested,UVars) ->
		deduceGoalOProve(findAllUsingGafsAndOrBackResolution(Req,UVars,Depth),VarsRequested,Ctx,Context, [Re|Sults]) 
		;deduceGoalOProve(findSingleUsingGafsAndOrBackResolution(Req,UVars,Depth),VarsRequested,Ctx,Context, [Re|Sults])).
	
	
% Prove For Self First Time (And/Or Others) goals where blocked on this generalization
deduceGoalOProve((Priorty-delayed(Req,ReqPointer,UVars,OtherLitKeys)),VarsRequested,Ctx,Context,[Re|Sults]):-!,
	(varintersection(VarsRequested,UVars) ->
		deduceGoalOProve(findAllUsingGafsAndOrBackResolution(Req,UVars,Depth),VarsRequested,Ctx,Context, [Re|Sults]) 
		;deduceGoalOProve(findSingleUsingGafsAndOrBackResolution(Req,UVars,Depth),VarsRequested,Ctx,Context, [Re|Sults])).

% Prove For Self Second Time (And/Or Others) goals where blocked on this generalization and on this path
deduceGoalOProve((Priorty-checkTable(Req,ReqPointer,UVars,OtherLitKeys)),VarsRequested,Ctx,Context,[Re|Sults]):-!,
	(varintersection(VarsRequested,UVars) ->
		deduceGoalOProve(findAllUsingGafsAndOrBackResolution(Req,UVars,Depth),VarsRequested,Ctx,Context, [Re|Sults]) 
		;deduceGoalOProve(findSingleUsingGafsAndOrBackResolution(Req,UVars,Depth),VarsRequested,Ctx,Context, [Re|Sults])).

varintersection(_,_).
	

bindFrom(Literal,Key,HashKey,Data):-
	ensureKey(Literal,Key,HashKey,_,Pointer),
	recordedLogged(Pointer,Data).

bindFromChk(Literal,Key,HashKey,Data):-
	ensureKey(Literal,Key,HashKey,_,Pointer),
	recordedLogged(Pointer,Data),!.

deleteFrom(Literal,Key,HashKey,Data):-
	ensureKey(Literal,Key,HashKey,_,Pointer),
	ignore((recordedLogged(Pointer,Data,Ref),erase(Ref),fail)).

pullFrom(Literal,Key,HashKey,Data):-
	ensureKey(Literal,Key,HashKey,_,Pointer),
	recordedLogged(Pointer,Data,Ref),erase(Ref).


pushInto(Literal,Key,HashKey,Data):-
	ensureKey(Literal,Key,HashKey,_,Pointer),
	recordaLogged(Pointer,Data),!.

putIntoZ(Literal,Key,HashKey,Data):-
	ensureKey(Literal,Key,HashKey,_,Pointer),
	recordzLogged(Pointer,Data),!.

putIntoZ(Literal,Key,HashKey,Data,Ref):-
	ensureKey(Literal,Key,HashKey,_,Pointer),
	recordzLogged(Pointer,Data,Ref),!.


putInto(Literal,Key,HashKey,Data):-
	putInto(Literal,Key,HashKey,_,Ref).

putIntochk(Literal,Key,HashKey,Data,Ref):-
	ensureKey(Literal,Key,HashKey,_,Pointer),
	((recordedLogged(Pointer,Data),Ref=old);recordaLogged(Pointer,Data,Ref)).
	
putInto(Literal,Key,HashKey,Data,Ref):-
	ensureKey(Literal,Key,HashKey,_,Pointer),
	(recordedLogged(Pointer,Data,Ref);recordaLogged(Pointer,Data,Ref)).




% Precall Hooks
%pre_deduce_true(Context,Ctx):-!.

pre_deduce_true(Context,Ctx):-
		retractall(is_tabled_true(_,_,_,_)),
		retractall(is_tabled_incomplete(_,_,_)).

pre_deduce_false(Context,Ctx):-
		listing(is_tabled_true),
		listing(is_tabled_incomplete).


shouldCanBackChainOn(Literal,UVars,_):-not(not(mooDatabase(Literal,Reqs,_,Context,Explaination))).


sumkeys([],0).
sumkeys([K-_],K).
sumkeys([K1-_,K2-_],N):-
	N is K1 + K2,!.
sumkeys([K1-_,K2-_,K3-_],N):-
	N is K1 + K2 + K3,!.

sumkeys([K-_|More],N):-
	sumkeys(More,Keys),
	N is K + Keys,!.
	


/*

% Positive Request       
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,User,UA, UVars,bullet(FmlInOpen) * Explaination,Result):-
	set_quit_time(1.1),
	writeDebug(green,'Stage 3 - Positive ':FmlInOpen ),
	pre_deduce_true(Context,Ctx),
	getRequestDefaults(URequest,Answers,BackchainsMax,Deductions),
	free_variables(URequest,FUVars),length(FUVars,H),
	writeDebug(deduce_Stage_3(deduceGoalOProve(theoryForm(TodoOrs,DoneTrue-DoneFail),H:FUVars,Depth,[],URequest,Ctx,Context,Explaination))),
	deduceGoalOProve(DB,H:FUVars,VO,Depth /* BackchainsMax */,[],URequest,Ctx,Context,Explaination),
	format('~n<hr> ~w: ~q~n<hr>~n',[URequest,Explaination]),
	flag('Returned Answers',RA,RA+1),
	is_unique(URequest,UA),
	should_cut(RA,UA,Deductions,Answers,Result). %UA>0.
	
is_unique(URequest,UA):-copy_term(URequest,Copy),numbervars(Copy,'$VAR',0,_),!,is_unique2(Copy,UA),!.
is_unique2(Copy,-1):-t_answer_found(Copy),writeDebug(used(Copy)),!.
is_unique2(Copy,UA):-asserta(t_answer_found(Copy)),flag('UAnswers',UA,UA+1),!.

should_cut(RA,UA,Deductions,Answers,done(true:UA)):-
	(UA >= Answers; RA >= Deductions),!,retractall(cpuend(Then)),asserta(cpuend(-1)).
should_cut(RA,UA,Deductions,Answers,more(true:UA)):-!.


% Negative Request       
agentRequestEach(FmlInOpen,URequest,UVars,Ctx,Context,User,UA2,['Result'='false'],Explaination,done(false:FmlInOpen:Explaination)):-%true,
	flag('UAnswers',UA,UA),UA<1, % only calls in no answers found previous    
	URequest=..[F|Args],
	NegURequest=..['~request'|Args],
	set_quit_time(1.1),
	writeDebug(red,'Stage 4 - Negation ':not(FmlInOpen)),
	pre_deduce_false(Context,Ctx),
        (getMooOption(opt_backchains_max_neg=BackchainsMax)),
	ignore(BackchainsMax=10),!,
	free_variables(Args,FUVars),
	length(FUVars,H),
	writeDebug(deduce_Stage_3(deduceGoalOProve(DB,H:FUVars,BackchainsMax,[],NegURequest,Ctx,Context,Explaination))),
	deduceGoalOProve(DB,H:FUVars,VO,BackchainsMax,[],NegURequest,Ctx,Context,Explaination),
	flag('Returned Answers',RA,RA+1),!.
*/



% ===============================================================
% REQUEST COMPILER
% Converts a KIF Surface Request to Request Macro in Heuristic Language
% ===============================================================

request_compile(not requestyn(PLOG),(PROLOG),Context,VarsIn,VarsOut,Given):- !,                        
                        request_compile(not(PLOG),PROLOG,Context,VarsIn,VarsOut,Given),!.                       
request_compile(requestyn(PLOG),(PROLOG),Context,VarsIn,VarsOut,Given):- !,                        
                        request_compile(PLOG,PROLOG,Context,VarsIn,VarsOut,Given),!.                       
request_compile(not(not(PLOG)),(PROLOG),Context,VarsIn,VarsOut,Given):- !,                       
                        request_compile(PLOG,PROLOG,Context,VarsIn,VarsOut,Given),!.                       

request_compile(This,This,Context,VarsIn,VarsOut,true).


/*
			
			Info=  ,
			% Awakens Rules:  [Key|OtherLitKeys]
			% Awakens Reqs: ReqKeys 
*/
        		
% No Others waiting Return its 'complexity rating'


% Hack in foreign calls
deduceGoalOProve(DB,VarsIn,VarsOut,Predicate,Logic,Depth,Table,(NewRequest),Agent,Context,Explaination):-
	deduceGoalOProve(DB,VarsIn,VarsOut,Depth,Table,(NewRequest),Agent,Context,Explaination).

bad_fact(holds(foo,_,_)).
bad_fact(holds(_,X,_)):-X==instance.
%bad_fact(holds(instance,_,X)):-X=='SymmetricRelation';X=='EquivalenceRelation';X=='ReflexiveRelation'.
bad_fact(holds(foo,_,_,_)).
bad_fact(holds(foo,_,_,_,_)).
bad_fact(holds(foo,_,_,_,_,_)).
bad_fact(holds(_,X,_)):-X==element.

/*
bad_fact(holds(Foo,_,Foo2)):- % This also stops getPrologVars in Foo position
	(disabled(Foo);(rd(Foo3),Foo3==Foo2)),!.
	
	
disabled(subsumesContentInstance).
disabled(equivalentContentInstance).
disabled(subsumesContentClass).
disabled(equivalentContentClass).
disabled(equal).
disabled(element).
disabled(piece).

rd('TrichotomizingRelation').
rd('TotalOrderingRelation').
rd('PartialOrderingRelation').

*/

	
% Datastructure Of deduceGoalOProve

%deduceGoalOProve(DB,PG,NG,VG,PGD,NGD,PRD,NRD,VD,PGC,NGC,PRC,NRC,VC,Depth)
%PG PosGoals
%NG NegGoals
%VG Vars Goals
%PGD PosGoalsDelayed List of Most Specific Versions that are delayed
%NGD NegGoalsDelayed
%PRD PosRulesDelayed
%NRD NegRulesDelayed
%VD Vars Delayed
%PGC PosGoalsComplete  List of Most General Versions that are completed
%NGC NegGoalsComplete
%PRC PosRulesComplete
%NRC NegRulesComplete
%VC VarsComplete
%Depth

deduceGoalOProve(DB,~ NG):-!,
	deduceGoalOProve(DB,[],NG,VG,[],[],[],[],[],[],[],[],[],[],Depth).

deduceGoalOProve(DB,PG):-	!,
	deduceGoalOProve(DB,PG,[],VG,[],[],[],[],[],[],[],[],[],[],Depth).



putDBZ(DB,M,TypeGoalsComplete):-
	ToMatch=..[TypeGoalsComplete,M],!,
	recordzLogged(M,ToMatch).

isMoreGeneral(DB,Lit,TypeGoalsComplete,Match):-
	ToMatch=..[TypeGoalsComplete,Cmp],!,
	mrecordedLogged(Lit,ToMatch),
	not(not(Lit=Cmp)),
	once(compareVariant(Lit,Cmp,M,C1,C2)),nonvar(M),
	C1<C2.
	
mrecordedLogged(X,Y):-recordedLogged(X,Y).
mrecordedLogged(X,Y):-
	Y=..[posGoalsComplete,Literal],
	functor(X,F,A),
	functor(Literal,F,A),
	mooCache(Literal,_,Context,Explaination).
	


isLessGeneral(DB,Lit,TypeGoalsComplete,Match):-
	ToMatch=..[TypeGoalsComplete,Cmp],!,
	mrecordedLogged(Lit,ToMatch),
	not(not(Lit=Cmp)),
	once(compareVariant(Lit,Cmp,M,C1,C2)),nonvar(M),
	C1>=C2.

getDelayer(DB,posGoalsDelayed,M,Undelay):-
	posGoalsDelayed(M,Undelay).

getDelayer(DB,negGoalsDelayed,M,Undelay):-
	negGoalsDelayed(M,Undelay).
	
putDelayer(DB,negGoalsDelayed,NG,delayedOn(M)):-
	asserta(negGoalsDelayed(NG,M)).

putDelayer(DB,posGoalsDelayed,NG,delayedOn(M)):-
	asserta(posGoalsDelayed(NG,M)).
	

		
findWaiting(DB, ~NG,M):-!,negGoalsDelayed(NG,M).      
findWaiting(DB, NG,M):-posGoalsDelayed(NG,M).      
 
putSupport(DB,M,support(POS)).

putSolution(DB,M,solution(UPOS)).

deduceGoalOProve(DB,[G],Depth):-!,
	deduceGoalOProve(DB,G,Depth).

deduceGoalOProve(DB,[G|Rest],Depth):-!,
	deduceGoalOProve(DB,G,Depth),
	deduceGoalOProve(DB,Rest,Depth).

deduceGoalOProve(DB,~ NG,Depth):-deduceGoalNeg(DB,NG,Depth).

deduceGoalOProve(DB,Pos,Depth):-not(Pos = ~ _),deduceGoalPos(DB,Pos,Depth).

% Negative Stack

% Found less general Refution completed
deduceGoalNeg(DB,NG,Depth):-
	isLessGeneral(DB,NG,posGoalsComplete,M),
	putDBZ(DB,~M,negGoalsDead),!,fail. % Get Next Job

% Found more general Refution completed
deduceGoalNeg(DB,NG,Depth):-
	isMoreGeneral(DB,NG,posGoalsComplete,M),
	putDBZ(DB,~NG,negGoalsDead),!,fail. % Get Next Job

% Found more general Refution delayed
deduceGoalNeg(DB,NG,Depth):-
	isLessGeneral(DB,NG,posGoalsDelayed,M),
	getDelayer(DB,posGoalsDelayed,M,Undelay),
	putDelayer(DB,negGoalsDelayed,NG,delayedOn(M)),!,
	deduceGoalOProve(DB,M,Depth). %New Job

% Found less general Refution delayed
deduceGoalNeg(DB,NG,Depth):-
	isMoreGeneral(DB,NG,posGoalsDelayed,M),
	getDelayer(DB,posGoalsDelayed,M,Undelay),
	putDelayer(DB,negGoalsDelayed,NG,delayedOn(M)),!,
	deduceGoalOProve(DB,Undelay,Depth). %New Job

% Found less general goal delayed
deduceGoalNeg(DB,NG,Depth):-
	isMoreGeneral(DB,NG,negGoalsDelayed,M),
	getDelayer(DB,negGoalsDelayed,M,Undelay),
	putDelayer(DB,negGoalsDelayed,NG,delayedOn(~M)),!,
	deduceGoalOProve(DB,Undelay,Depth).

% Found more general goal delayed (informational) Add support link
deduceGoalNeg(DB,NG,Depth):-
	isLessGeneral(DB,NG,negGoalsDelayed,M),
	putSupport(DB,~M,support(~NG)),fail.

% Found more general goal complete (informational) Add solution link
deduceGoalNeg(DB,NG,Depth):-
	isMoreGeneral(DB,NG,negGoalsComplete,M),
	putSolution(DB,~M,solution(~NG)),fail.	
	
% Found more general goal completed (find those waiting on it and call it)
deduceGoalNeg(DB,NG,Depth):-
	isLessGeneral(DB,NG,negGoalsComplete,M),
	putDBZ(DB, NG, negGoalsComplete), %TODO guard
	((findWaiting(DB, ~NG,NWaiter),deduceGoalOProve(DB,NWaiter,Depth));
	(findWaiting(DB, ~NG,PWaiter),deduceGoalOProve(DB,PWaiter,Depth)));
	true.


% Positive Stack

% Found less general Refution completed
deduceGoalPos(DB,POS,Depth):-
	isLessGeneral(DB,POS,negGoalsComplete,M),
	putDBZ(DB,M,posGoalsDead),!,fail. % Get Next Job

% Found more general Refution completed
deduceGoalPos(DB,POS,Depth):-
	isMoreGeneral(DB,POS,negGoalsComplete,M),
	putDBZ(DB,POS,posGoalsDead),!,fail. % Get Next Job

% Found more general Refution delayed
deduceGoalPos(DB,POS,Depth):-
	isLessGeneral(DB,POS,negGoalsDelayed,M),
	getDelayer(DB,negGoalsDelayed,M,Undelay),
	putDelayer(DB,posGoalsDelayed,POS,delayedOn(~M)),!,
	deduceGoalOProve(DB,M,Depth). %New Job

% Found less general Refution delayed
deduceGoalPos(DB,POS,Depth):-
	isMoreGeneral(DB,POS,negGoalsDelayed,M),
	getDelayer(DB,negGoalsDelayed,M,Undelay),
	putDelayer(DB,posGoalsDelayed,POS,delayedOn(~M)),!,
	deduceGoalOProve(DB,Undelay,Depth). %New Job

% Found less general goal delayed
deduceGoalPos(DB,POS,Depth):-
	isMoreGeneral(DB,POS,posGoalsDelayed,M),
	getDelayer(DB,posGoalsDelayed,M,Undelay),
	putDelayer(DB,posGoalsDelayed,POS,delayedOn(M)),!,
	deduceGoalOProve(DB,Undelay,Depth).

% Found more general goal delayed (informational) Add support link
deduceGoalPos(DB,POS,Depth):-
	isLessGeneral(DB,POS,posGoalsDelayed,M),
	putSupport(DB,M,support(POS)),fail.

% Found more general goal complete (informational) Add solution link
deduceGoalPos(DB,POS,Depth):-
	isMoreGeneral(DB,POS,posGoalsComplete,M),
	putSolution(DB,M,solution(POS)),fail.	
	
% Found more general goal completed (find those waiting on it and call it)
deduceGoalPos(DB,POS,Depth):-
	isLessGeneral(DB,POS,posGoalsComplete,M),
	putDBZ(DB, POS, posGoalsComplete), %TODO guard
	((findWaiting(DB, POS, posGoalsDelayed,NWaiter),deduceGoalOProve(DB,NWaiter,Depth));
	(findWaiting(DB, POS, negGoalsDelayed,PWaiter),deduceGoalOProve(DB,PWaiter,Depth)));
	true.


% Look for Rules
deduceGoalOProve(DB,Goal,Depth):- 
	 isLessGeneralRule(DB,Goal,Head,Body,UHead,UBody),
	 taskRule(DB,Head,Body,UHead,UBody,Body,UBody,TODOLIST),
	 deduceGoalOProve(DB,TODOLIST,Depth).

taskRule(DB,Head,Body,UHead,UBody,[H],[UH],TODOLIST):-!,
	taskItem(DB,Head,Body,UHead,UBody,H,UH,TODOLIST),!.	

taskRule(DB,Head,Body,UHead,UBody,[H|T],[UH|UT],TODOLIST):-
	taskItem(DB,Head,Body,UHead,UBody,H,UH,TH),!,
	taskRule(DB,Head,Body,UHead,UBody,T,UT,TT),
	append(TH,TT,TODOLIST),!.
	
taskItem(DB,Head,Body,UHead,UBody,~ NG, ~UNG,TH):-!,
	getSingleVars(NG,UNG,Head,Body,UHead,UBody,Singletons),
	taskItemNeg(DB,Head,Body,UHead,UBody,NG,UNG,TH).
taskItem(DB,Head,Body,UHead,UBody,POS,UPOS,TH):-
	getSingleVars(POS,UPOS,Head,Body,UHead,UBody,Singletons),
	taskItemPos(Singletons,DB,Head,Body,UHead,UBody,POS,UPOS,TH).
	
getSingleVars(POS,UPOS,Head,Body,UHead,UBody,Singletons).

% Found more general Refution completed
taskItemPos(Singletons,DB,Head,Body,UHead,UBody,POS,UPOS,[POS]):-
	isMoreGeneral(DB,UPOS,negGoalsComplete,M),!,fail. % Rule Dead

% Found less general Refution completed
taskItemPos(Singletons,DB,Head,Body,UHead,UBody,POS,UPOS,[POS]):-
	isLessGeneral(DB,UPOS,negGoalsComplete,M),!,fail. % Rule too expensive for this item

% Found more general Refution delayed
taskItemPos(Singletons,DB,Head,Body,UHead,UBody,POS,UPOS,[M]):-
	isLessGeneral(DB,UPOS,negGoalsDelayed,M),
	getDelayer(DB,negGoalsDelayed,M,Undelay),
	putDelayer(DB,posGoalsDelayed,UPOS,delayedOn(M)),!.
	
% Found less general Refution delayed
taskItemPos(Singletons,DB,Head,Body,UHead,UBody,POS,UPOS,[M]):-
	isMoreGeneral(DB,UPOS,negGoalsDelayed,M),
	getDelayer(DB,negGoalsDelayed,M,Undelay),
	putDelayer(DB,posGoalsDelayed,UPOS,delayedOn(M)),!.
	
% Found less general goal delayed
taskItemPos(Singletons,DB,Head,Body,UHead,UBody,POS,UPOS,[M]):-
	isMoreGeneral(DB,UPOS,posGoalsDelayed,M),
	getDelayer(DB,posGoalsDelayed,M,Undelay),
	putDelayer(DB,posGoalsDelayed,UPOS,delayedOn(M)),!.
	
% Found more general goal delayed (informational) Add support link
taskItemPos(Singletons,DB,Head,Body,UHead,UBody,POS,UPOS,[UPOS]):-
	isLessGeneral(DB,UPOS,posGoalsDelayed,M),
	putSupport(DB,M,support(UPOS)),fail.

% Found more general goal complete (informational) Add solution link
taskItemPos(Singletons,DB,Head,Body,UHead,UBody,POS,UPOS,[]):-
	isMoreGeneral(DB,UPOS,posGoalsComplete,M),
	putSolution(DB,M,solution(UPOS)),fail.	
	
% Found more general goal completed (find those waiting on it and call it)
taskItemPos(Singletons,DB,Head,Body,UHead,UBody,POS,UPOS,[NWaiter,PWaiter]):-
	isLessGeneral(DB,UPOS,posGoalsComplete,M),
	putDBZ(DB, UPOS, posGoalsComplete), %TODO guard
	((findWaiting(DB, UPOS, posGoalsDelayed,NWaiter));
	(findWaiting(DB, UPOS, negGoalsDelayed,PWaiter))).
	

% Found more general Refution completed
taskItemNeg(Singletons,DB,Head,Body,UHead,UBody,NEG,UNEG,[NEG]):-
	isMoreGeneral(DB,UNEG,posGoalsComplete,M),!,fail. % Rule Dead

% Found less general Refution completed
taskItemNeg(Singletons,DB,Head,Body,UHead,UBody,NEG,UNEG,[NEG]):-
	isLessGeneral(DB,UNEG,posGoalsComplete,M),!,fail. % Rule too expensive for this item

% Found more general Refution delayed
taskItemNeg(Singletons,DB,Head,Body,UHead,UBody,NEG,UNEG,[M]):-
	isLessGeneral(DB,UNEG,posGoalsDelayed,M),
	getDelayer(DB,posGoalsDelayed,M,Undelay),
	putDelayer(DB,negGoalsDelayed,UNEG,delayedOn(~M)),!.
	
% Found less general Refution delayed
taskItemNeg(Singletons,DB,Head,Body,UHead,UBody,NEG,UNEG,[M]):-
	isMoreGeneral(DB,UNEG,posGoalsDelayed,M),
	getDelayer(DB,posGoalsDelayed,M,Undelay),
	putDelayer(DB,negGoalsDelayed,UNEG,delayedOn(~M)),!.
	
% Found less general goal delayed
taskItemNeg(Singletons,DB,Head,Body,UHead,UBody,NEG,UNEG,[M]):-
	isMoreGeneral(DB,UNEG,negGoalsDelayed,M),
	getDelayer(DB,negGoalsDelayed,M,Undelay),
	putDelayer(DB,negGoalsDelayed,UNEG,delayedOn(~M)),!.
	
% Found more general goal delayed (informational) Add support link
taskItemNeg(Singletons,DB,Head,Body,UHead,UBody,NEG,UNEG,[UNEG]):-
	isLessGeneral(DB,UNEG,negGoalsDelayed,M),
	putSupport(DB,~M,support(~UNEG)),fail.

% Found more general goal complete (informational) Add solution link
taskItemNeg(Singletons,DB,Head,Body,UHead,UBody,NEG,UNEG,[]):-
	isMoreGeneral(DB,UNEG,negGoalsComplete,M),
	putSolution(DB,~M,solution(~UNEG)),fail.	
	
% Found more general goal completed (find those waiting on it and call it)
taskItemNeg(Singletons,DB,Head,Body,UHead,UBody,NEG,UNEG,[NWaiter,PWaiter]):-
	isLessGeneral(DB,UNEG,negGoalsComplete,M),
	putDBZ(DB, UNEG, negGoalsComplete), %TODO guard
	((findWaiting(DB, UNEG, negGoalsDelayed,NWaiter));
	(findWaiting(DB, UNEG, posGoalsDelayed,PWaiter))).
	     
	
	 

	/*
	
% divideReqlist(ReqList,PGD,NGD,PGC,NGC,RPGD,RNGD,RPGC,RNGC).
divideReqlist([H],PGD,NGD,PGC,NGC,HPGD,HNGD,HPGC,HNGC,NewGoals):-
	divideReq(H,PGD,NGD,PGC,NGC,HPGD,HNGD,HPGC,HNGC,NewGoals),!.
divideReqlist([H|T],PGD,NGD,PGC,NGC,RPGD,RNGD,RPGC,RNGC,NewGoals):-
	divideReq(H,PGD,NGD,PGC,NGC,HPGD,HNGD,HPGC,HNGC,NewGoalsH),!,
	divideReqlist(T,HPGD,HNGD,HPGC,HNGC,RPGD,RNGD,RPGC,RNGC,NewGoalsT),
	append(NewGoalsH,NewGoalsT,NewGoals).
divideReq(~ NH,PGD,NGD,PGC,NGC,HPGD,HNGD,HPGC,HNGC,NewGoals):-
	more_specific_than_listed(NH,NGD,MSNGD), % then delay this as well and make the MSNGD wait on NH
	more_specific_than_listedc(NH,NGC,MSNGC), % then this is already true and add to NGC
	less_specific_than_listed(NH,NGD,LSNGD), % then delay this until LSNGD is complete
	less_specific_than_listedc(NH,NGC,LSNGC), % then this is true (add complete) NewGoals=[]
	
	more_specific_than_listedc(NH,PGC,MSPGC), % then this rule is worthless
	more_specific_than_listed(NH,PGD,MSPGD), % then delay this until MSPGD is complete 
	less_specific_than_listed(NH,PGD,LSPGD), % then delay LSPGD until the delayed NH is complete
	less_specific_than_listedc(NH,PGC,LSPGC), % then this rule is worthless
	
divideReq(PH,PGD,NGD,PGC,NGC,HPGD,HNGD,HPGC,HNGC,NewGoals):-
	more_specific_than_listed(PH,PGD,MSPGD), % then delay this as well and make the MSPGD wait on PH
	more_specific_than_listedc(PH,PGC,MSPGC), % then this is already true and add to PGC
	less_specific_than_listed(PH,PGD,LSPGD), % then delay this until LSPGD is complete
	less_specific_than_listedc(PH,PGC,LSPGC), % then this is true (add complete) NewGoals=[]
	
	more_specific_than_listedc(PH,NGC,MSNGC), % then this rule is worthless
	more_specific_than_listed(PH,NGD,MSNGD), % then delay this until MSNGD is complete 
	less_specific_than_listed(PH,NGD,LSNGD), % then delay LSNGD until the delayed PH is complete
	less_specific_than_listedc(PH,NGC,LSNGC), % then this rule is worthless
	
divideReq(PH,PGD,NGD,PGC,NGC,HPGD,HNGD,HPGC,HNGC,NewGoals):- 
	less_specific_than_listedc(PH,NGC,LSNGC),!,fail.  % then this rule is worthless

divideReq(PH,PGD,NGD,PGC,NGC,HPGD,HNGD,HPGC,HNGC,NewGoals):- 
	less_specific_than_listedc(PH,NGC,LSNGC),!,fail.  % then this rule is worthless
	
	more_specific_than_listedc(PH,NGC,MSNGC),!,fail.
	
	
	

divideReq(PH,PGD,NGD,PGC,NGC,PGD,NGD,[PH|PGC],NGC,[]):-	
	less_specific_than_listedc(PH,PGC,LSPGC),!. % then this is true (add complete) NewGoals=[]

divideReq(PH,PGD,NGD,PGC,NGC,PGD,NGD,[PH|PGC],NGC,[]):-	
	more_specific_than_listedc(PH,PGC,MSPGC), 
	% then this *will be* true but too general and will need to wait until all subvariant delays are completed.
	% this means that all answers to clauses that are subsumed by this will be the final value of this when it completes.
	% how will we know when this gets accomplised?
	% First enummerate all possible specific Possibilities and delay each until someone proves them true
	
	more_specific_than_listed(PH,PGD,MSPGD), % then delay this as well and make the MSPGD wait on PH
	less_specific_than_listed(PH,PGD,LSPGD), % then delay this until LSPGD is complete
	
	more_specific_than_listed(PH,NGD,MSNGD), % then delay this until MSNGD is complete 
	less_specific_than_listed(PH,NGD,LSNGD), % then delay LSNGD until the delayed PH is complete
	*/
	

	
	                    /*
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,or([]),Ctx,Context,ExplainationOut):-!.
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,[],Ctx,Context,ExplainationOut):-!.

deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,[Literal],Ctx,Context,ExplainationOut):-!,
	deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,Literal,Ctx,Context, ExplainationOut ). % Completeion
	
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,[Literal|LiteralList],Ctx,Context,ExplainationOut):-
	deduceGoalOProve(DB,VarsIn,VarsMid,BackchainsMax,ExplainationIn, Literal,Ctx,Context, ExplainationMid ),!, %true,
	deduceGoalOProve(DB,VarsMid,VarsOut,BackchainsMax,ExplainationMid, LiteralList,Ctx,Context,ExplainationOut).

deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,tabled_or([LiteralList]),Ctx,Context,ExplainationOut):-!,
	deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,LiteralList,Ctx,Context, ExplainationOut). %true,
	
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,tabled_or([LiteralList|DisjLiteralList]),Ctx,Context,ExplainationOut):-
	deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn, LiteralList,Ctx,Context, ExplainationOut)
	;
	deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn, tabled_or(DisjLiteralList),Ctx,Context,ExplainationOut).

deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,proved_gaf_or([LiteralList]),Ctx,Context,[ExplainationIn|LiteralList]):-!.
	
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,proved_gaf_or([LiteralList|_]),Ctx,Context,[ExplainationIn|LiteralList]).
	
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,proved_gaf_or([_,N|More]),Ctx,Context,ExplainationOut):-!,
	deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,proved_gaf_or([N|More]),Ctx,Context,ExplainationOut).

deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,or([LiteralList]),Ctx,Context,ExplainationOut):-!,
	deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,LiteralList,Ctx,Context, ExplainationOut).  %true,
	
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,or([LiteralList|DisjLiteralList]),Ctx,Context,ExplainationOut):-
	deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn, LiteralList,Ctx,Context, ExplainationOut)
	;
	deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn, or(DisjLiteralList),Ctx,Context,ExplainationOut).
					*/
% Equals
%deduceGoalOProve(DB,VarsOut,VarsOut,BackchainsMax,Table,equal(U,W),Ctx,Context,Explaination):-!,
%	prove_ground_goal(equal,Logic,BackchainsMax,Table,equal(U,W),Ctx,Context,Explaination).
  /*
        
deduceGoalOProve(DB,VarsOut,VarsOut,BackchainsMax,ExplainationIn, Literal,Ctx,Context, _ ):-	
	mrecordedLogged(failed,Literal),!,fail.
  */
/*
deduceGoalOProve(DB,VarsOut,VarsOut,BackchainsMax,ExplainationIn, Literal,Ctx,Context, _ ):-	
	mrecordedLogged(trying,Literal),!,fail.
*/
	
	
    /*
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,Literal,Ctx,Context, ExplainationOut):-
	no_rules_about(Literal,Ctx,Context),!,fail.
      */  
:-dynamic(no_rules_about/3).

/*
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,Literal,Ctx,Context, ExplainationOut):-fail,!,
	ado_cache_access_smallest_first(Literal, ReqList, ThisCost,Context,_Ctx,Explaination),
	not(memberchk(Explaination,ExplainationIn)),
	BackchainsMax > ThisCost,
	BackchainsNext is BackchainsMax - ThisCost,
	vars_legal(VarsIn,Literal,GettingThereFaster,MVarsOut),
	makeSubexplaination(ExplainationIn,Literal,GettingThereFaster,Explaination,ExplainationMid),
	deduceGoalOProve(DB,MVarsOut,VarsOut,BackchainsNext,ExplainationMid,ReqList,Ctx,Context,ExplainationOut).

%group_access_legal(N:VarsIn,VarsOut,BackchainsMax,ExplainationIn,Literal,Ctx,Context):-fail.

group_access_legal(N:VarsIn,VarsOut,BackchainsMax,ExplainationIn,Literal,Ctx,Context):-!.

*/





	


/*
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,ExplainationIn,Literal,Ctx,Context, ExplainationOut):-
	%true,
      %  not(memberchk(Literal,ExplainationIn)),
   %     not(mrecordedLogged(trying_rules,Literal)),
	recordaLogged(trying_rules,Literal),
     %   group_access_legal(N:VarsIn,VarsOut,BackchainsMax,ExplainationIn,Literal,Ctx,Context),!,
	findall(ReqList,((
			ado_cache_access_largest_first(Literal, ReqList, ThisCost,Context,_Ctx,Explaination),
			ThisCost < BackchainsMax
			%not(memberchk(Explaination,ExplainationIn))
		       % not(memberchk(Literal,ReqList))
			)),GettingThere),!, %true,
		list_to_set(GettingThere,[S|ET]),
		writeDebug(cyan,Literal =  [S|ET]),
		length(Set,Cost),  %true,
		Cost < BackchainsMax,
		make_junts(VarsIn,ExplainationIn,Literal, [S|ET],GettingThereBetter,ExplainationJunt,MVarsOut),!,
		writeDebug(cyan,new_goal(GettingThereBetter)),
		%writeObject(new_goal(GettingThereBetter),_),
		BackchainN is BackchainsMax - Cost,
		makeSubexplaination(ExplainationIn,Literal,GettingThereFaster,ExplainationJunt,ExplainationMid), %true,
		deduceGoalOProve(DB,MVarsOut,VarsOut,BackchainN,ExplainationMid,GettingThereBetter,_Ctx,Context,ExplainationOut).
*/		
makeSubexplaination(ExplainationIn,Literal,[[GettingThereFaster]],Explaination,ExplainationMid):-!,
	append([Explaination|ExplainationIn],[Literal|GettingThereFaster],ExplainationMid).
makeSubexplaination(ExplainationIn,Literal,GettingThereFaster,Explaination,ExplainationMid):-!,
	append([Explaination|ExplainationIn],[Literal|GettingThereFaster],ExplainationMid).
makeSubexplaination(ExplainationIn,Literal,GettingThereFaster,Explaination,[Literal|ExplainationMid]).


	
mj0:-mj([]).
mj1:-mj([[b(1),c,x,b(Y)]]).
mj2:-mj([[b,c,x],[q,c,y]]).
mj2a:-mj(head(T), ([[b,c,x(T)],[b,c,y(P)]])).

mj2b:-mj([[b,c,x],[b,c,y(_)]]).


mj3:-mj([[b,c,x],[b,c,y],[b,c,z]]).
mj3a:-mj([[b,t,x],[b,c,y],[b,c,z]]).
mj3b:-mj([[b,t,x],[b,t,x],[b,c,z]]).
mj3c:-mj([[a,b,c],[d,e,f],[g,h,i]]).
mj4:-mj([[a(X,T),b(X,T),c(X,T)],[d(X,T),e(Y,T),f(Y,T)],[d(Y,T),d(X,T),b(T,T),f(b)],[g(Q,T),h(Q,T),i(9,T)]]).

mj2b:-mj([[a(X),b(N),c(X,T)],[g(Q,T),h,b(N),a(X)]]).

mj(MJ):-
	mj(head(X),MJ).

mj(Lit,MJ):-
	list_to_set(MJ,MJ1),
	writeq((Lit:-(-(MJ1) : MV))),nl,nl,
	make_junts(6,[],Lit,MJ1,G,PO,MV),
	writeq((Lit:-(+(G) : MV))),nl,nl.
	


/*

cleanGettingThereBetter(or(L),or(LL)):-
	cleanGettingThereBetter(L,LL).

cleanGettingThereBetter([[],H|L],LL):-
	cleanGettingThereBetter([H|L],LL).
	
cleanGettingThereBetter([A|L],[AA|LL]):-
	cleanGettingThereBetter(A,AA),
	cleanGettingThereBetter(L,LL).

cleanGettingThereBetter([A],AA):-
	cleanGettingThereBetter(A,AA).
*/

cleanGettingThereBetter(A,A).

/*
cleanGettingThereBetter([[H]|A],AA):-
	cleanGettingThereBetter([H|A],AA).
cleanGettingThereBetter(or([[H]|A]),AA):-
	cleanGettingThereBetter(or([H|A]),AA).
*/
	

/*
vars_legal(N:VarsIn,Literal,GettingThereFaster,NewN:MVarsOut):-
	N < 10,!,
	free_variables(GettingThereFaster,MVarsOut),length(MVarsOut,NewN).
*/

vars_legal(N:VarsIn,Literal,GettingThereFaster,NewN:MVarsOut):-
	free_variables(GettingThereFaster,MVarsOut),length(MVarsOut,NewN),!. %,(N<12).
	
	



make_junts(N,ExplainationIn,Literal, [],GettingThere,[GettingThere|ExplainationIn],NewN:MVarsOut):-!,fail.

make_junts(VarsIn,ExplainationIn,Literal, [[GettingThere]],GettingThereFaster,[GettingThereFaster|ExplainationIn],MVarsOut):-!,
	optimize_keys(Literal,GettingThere,GettingThereFaster),
	vars_legal(VarsIn,Literal,GettingThereFaster,MVarsOut).
	

make_junts(N,ExplainationIn,Literal, GettingThere, GettingThereFaster, ExplainationIn, NewN:MVarsOut):-
	shorted_paths(Literal,GettingThere,GettingThereFaster),
	vars_legal(VarsIn,Literal,GettingThereFaster,MVarsOut).


shorted_paths(Literal,[],[]).
shorted_paths(Literal,[GettingThere],GettingThereFaster):-
	optimize_keys(Literal,GettingThere,GettingThereFaster),!.
shorted_paths(Literal,[Conj1,Conj2],Best):-
	set_partition(Conj1,Conj2,NewConj1,NewConj2,Intersection12),
	optimize_keys(Literal,Intersection12,Intersection12Faster),
	optimize_keys(Intersection12,NewConj1,NewConj1Faster),
	optimize_keys(Intersection12,NewConj2,NewConj2Faster),
	optimize_keys(Intersection12Faster,[NewConj1Faster,NewConj2Faster],Faster),
	rewrite_leftovers(Intersection12Faster,Faster,Best),!.
shorted_paths(Literal,ConjS,Best):-
	length(ConjS,Depth),
	make_frames(Frame,Depth),
	number_sublits(1,ConjS,Numbered),
	keysort(Numbered,Keyed),
	combine_consts(Keyed,NumKeys),
	optimize_keys(Literal,NumKeys,OptKeys),
	keys_into_targets(Frame,OptKeys,Table,FreeGoals),
	optimize_keys(OptKeys,FreeGoals,FasterFreeGoals),
	rewrite_leftovers(Table,FasterFreeGoals,Best),!.

optimize_keys(Literal,[],[]):-!.
optimize_keys(Literal,[Lit],[Lit]):-!.
optimize_keys(Literal,List1,Opt):-predsort(optimize_keys(Literal),List1,Opt),!.
optimize_keys(Literal,Lit,Lit):-!.
optimize_keys(Literal,Ret,Lit1-L1,Lit2-L2):-length(L1,N1),length(L2,N2),(N1 \= N2,compare(Ret,N1,N2)).
optimize_keys(Literal,Ret,Lit1,Lit2):-free_variables((Lit1:(Literal:Var)),[FV1|_]),free_variables((Lit2:(Literal:Var)),[FV2|_]),!,
			(FV2\==FV1 -> compare(Ret,FV1,FV2) ; compare(Ret,Lit1,Lit2)).



make_frames([[]],1).
make_frames([[],[]],2).
make_frames([[],[],[]],3).
make_frames([[],[],[],[]],4).
make_frames([[],[],[],[],[]],5).
make_frames([[],[],[],[],[],[]],6).
make_frames(Result,Depth):-
	NN is N -6,make_frames(First,NN),
	append(First,[[],[],[],[],[],[]],Result).



keys_into_targets(FreeGoals,[],[],FreeGoals).
keys_into_targets(Frame,[Lit-[N]|Keys],Table,FreeGoals):-
	put_key_in((Lit),Depth,Frame,FrameMid),
	keys_into_targets(FrameMid,Keys,Table,FreeGoals).
keys_into_targets(Frame,[Lit-List|Keys],[Lit|Table],FreeGoals):-
	put_key_in_l(ex(Lit),List,Frame,FrameMid),
	keys_into_targets(FrameMid,Keys,Table,FreeGoals).

put_key_in_l(Key,[Slot],Frame,Framed):-
	put_key_in(Key,Slot,Frame,Framed).	
put_key_in_l(Key,[Slot|Slots],Frame,Framed):-
	put_key_in(Key,Slot,Frame,Framing),
	put_key_in_l(Key,Slots,Framing,Framed).
	

put_key_in(Key,1,[Role|Frame],[[Key|Role]|Frame]).
put_key_in(Key,2,[H1,Role|Frame],[H1,[Key|Role]|Frame]).
put_key_in(Key,3,[H1,H2,Role|Frame],[H1,H2,[Key|Role]|Frame]).
put_key_in(Key,4,[H1,H2,H3,Role|Frame],[H1,H2,H3,[Key|Role]|Frame]).
put_key_in(Key,Depth,[H1,H2,H3|FrameIn],[H1,H2,H3|FrameOut]):-
	NN is N - 4,put_key_in(Key,NN,FrameIn,FrameOut).	




rewrite_leftovers([],FreeGoals,or(FreeGoals)):-!.
rewrite_leftovers(Table,[],or(Table)):-!.
rewrite_leftovers(Table,FreeGoals,[tabled_or(Table),or(FreeGoals)]):-!.

	
	
combine_consts([E],[E]).
combine_consts([(Lit - NL),(Lit2 - [N])|Keyed],NumKeys):-
	Lit == Lit2,
	combine_consts([Lit-[N|NL]|Keyed],NumKeys).
combine_consts([E|Keyed],[E|NumKeys]):-
	combine_consts(Keyed,NumKeys).


			 
			
number_sublits(N,[],[]).
number_sublits(N,[Conj],Numbered):-
	number_each(N,Conj,Numbered).

number_sublits(N,[Conj|More],NumberedAll):-
	number_each(N,Conj,Numbered),
	NN is N + 1,
	number_sublits(NN,More,NumberedMore),
	append(Numbered,NumberedMore,NumberedAll).
	
number_each(N,[Lit],[Lit-[N]]).
number_each(N,[Lit|LitS],[Lit-[N]|NLitS]):-
	number_each(N,LitS,NLitS).

/*

shorted_paths(Literal,[Conj1,Conj2,Conj3],Simpler):-!,
	set_partition(Conj1,Conj2,NewConj12,NewConj21,Intersection12),
	set_partition(Conj1,Conj3,NewConj13,NewConj31,Intersection13),
	set_partition(Conj2,Conj3,NewConj23,NewConj32,Intersection23),
   pick_best(NewConj12,NewConj21,Intersection12,NewConj13,NewConj31,Intersection13,NewConj23,NewConj32,Intersection23,Simpler).

pick_best(NewConj12,NewConj21,[],NewConj13,NewConj31,[],NewConj23,NewConj32,[],or([NewConj12,NewConj21,NewConj32])).
   
*/

	

/*
shorted_paths(Literal,[Conj1,Conj2,Conj3],Simple):-!,
	shorted_paths(Literal,[Conj1,Conj2],Shortest12),
	shorted_paths(Literal,[Conj3,Shortest12],Simple).

shorted_paths(Literal,[Conj1,Conj2,Conj3|ConjS],Simple):-!,
	shorted_paths(Literal,[Conj1,Conj2],Shortest12),
	shorted_paths(Literal,[Conj3|ConjS],Shortest3S),
	shorted_paths(Literal,[Shortest12,Shortest3S],Simple).
*/

shorted_paths(Literal,Conj,Conj):-!.

/*
*/
	
		
	

simpler(or([]),[]):-!.
simpler(or(Conj),or(Simple)):-simpler(Conj,Simple).
simpler([Conj,[]],Simple):-simpler(Conj,Simple).
simpler([[],Conj],Simple):-simpler(Conj,Simple).
simpler([Conj1,Conj2],[SConj1,SConj2]):-
       simpler(Conj1,SConj1),
	simpler(Conj2,SConj2).
	
simpler(Simple,Simple):-!.


       
		

shorted_paths(Literal,ConjS,ConjS):-!.
     

	


ado_cache_access_smallest_first(Literal, ReqList, 1,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 1,Context,Explaination).
ado_cache_access_smallest_first(Literal, ReqList, 2,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 2,Context,Explaination).
ado_cache_access_smallest_first(Literal, ReqList, 3,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 3,Context,Explaination).
ado_cache_access_smallest_first(Literal, ReqList, 4,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 4,Context,Explaination).
ado_cache_access_smallest_first(Literal, ReqList, 5,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 5,Context,Explaination).
ado_cache_access_smallest_first(Literal, ReqList, 6,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 6,Context,Explaination).
ado_cache_access_smallest_first(Literal, ReqList, 7,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 7,Context,Explaination).
ado_cache_access_smallest_first(Literal, ReqList, 8,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 8,Context,Explaination).
ado_cache_access_smallest_first(Literal, ReqList, 9,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 9,Context,Explaination).



ado_cache_access_largest_first(Literal, ReqList, 9,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 9,Context,Explaination).
ado_cache_access_largest_first(Literal, ReqList, 8,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 8,Context,Explaination).
ado_cache_access_largest_first(Literal, ReqList, 7,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 7,Context,Explaination).
ado_cache_access_largest_first(Literal, ReqList, 6,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 6,Context,Explaination).
ado_cache_access_largest_first(Literal, ReqList, 5,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 5,Context,Explaination).
ado_cache_access_largest_first(Literal, ReqList, 4,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 4,Context,Explaination).
ado_cache_access_largest_first(Literal, ReqList, 3,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 3,Context,Explaination).
ado_cache_access_largest_first(Literal, ReqList, 2,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 2,Context,Explaination).
ado_cache_access_largest_first(Literal, ReqList, 1,Context, Ctx,Explaination):-
	mooCache(Literal, ReqList, 1,Context,Explaination).

	

%mooCache(instance('exists '([63, 67, 79, 76, 79, 82, 50], exists([63, 67, 79, 76, 79, 82, 49], exists([63, 80, 65, 82, 84, 50], exists([63, 80, 65, 82, 84, 49], and(superficialPart([63, 80, 65, 82, 84, 49], A), and(superficialPart([63, 80, 65, 82, 84, 50], A), and(attribute([63, 80, 65, 82, 84, 49], [63, 67, 79, 76, 79, 82, 49]), and(attribute([63, 80, 65, 82, 84, 50], [63, 67, 79, 76, 79, 82, 50]), and(instance([63, 67, 79, 76, 79, 82, 49], 'ColorProperty'), and(instance([63, 67, 79, 76, 79, 82, 50], 'ColorProperty'), not(equal([63, 67, 79, 76, 79, 82, 49], [63, 67, 79, 76, 79, 82, 50])))))))))))), 'ColorProperty'), +instance(+, +), [attribute(A, 'Polychromatic'), domainV(A, [:(attribute, 1), :(superficialPart, 2)])], [+attribute(-, +)], 2, 'PrologMOO', 'QUALITIES', surf('PrologMOO', 6917, 12, ['OBJ'=A, 'PART1'=B, 'PART2'=C, 'COLOR1'=D, 'COLOR2'=E|F])).
	
not_in(N,Ele,List):-my_nth_gen(List,Ele,2,O),!,N < O.
my_nth_gen([Elem|_], Elem, Base, Base).
my_nth_gen([_|Tail], Elem, N, Base) :-
        succ(N, M),
        my_nth_gen(Tail, Elem, M, Base).

	

my_nth0(Index, List, Elem) :-
        integer(Index), !,
        Index >= 0,
        my_nth0_det(Index, List, Elem).    %% take my_nth deterministically
my_nth0(Index, List, Elem) :-
        var(Index), !,
        my_nth_gen(List, Elem, 0, Index).  %% match

my_nth0_det(0, [Elem|_], Elem) :- !.
my_nth0_det(1, [_,Elem|_], Elem) :- !.
my_nth0_det(2, [_,_,Elem|_], Elem) :- !.
my_nth0_det(3, [_,_,_,Elem|_], Elem) :- !.
my_nth0_det(4, [_,_,_,_,Elem|_], Elem) :- !.
my_nth0_det(5, [_,_,_,_,_,Elem|_], Elem) :- !.
my_nth0_det(N, [_,_,_,_,_,_   |Tail], Elem) :-
        M is N - 6,
        my_nth0_det(M, Tail, Elem).

/*

% specialization of disjoint true //TODO Specialized
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,Table,holds(disjoint,S,C),Ctx,Context, g_h(holds(disjoint,S,C)) * Explaination ):-!,
	deduceSurfaceGuarded(disjoint,Logic,holds(disjoint,S,C),Agent,Context,Explaination).

% specialization of subrelation true //TODO Specialized
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,Table,holds(subrelation,S,C),Ctx,Context, g_h(holds(subrelation,S,C)) * Explaination ):-!,
	deduceSurfaceGuarded(subrelation,Logic,holds(subrelation,S,C),Agent,Context,Explaination).

% specialization of Sentence ops //TODO Specialized
deduceGoalOProve(DB,VarsIn,VarsOut,Depth,Table,Goal,Agent,Context,P):-functor(Goal,F,_),
	hlPredicateAttribute(F,connective),!,
	writeDebug(blue,deduceGoal_sentence_op(F,Logic,Depth,Goal,Agent,Context,Table)),
	deduceGoal_sentence_op(F,Logic,Depth,Table,Goal,Agent,Context,P).

% ===================================================
% IrreflexiveRelation True
% ===================================================
deduceGoalOProve(DB,VarsIn,VarsOut,true,Depth,Table,holds(Predicate,Arg1,Arg2),Agent,Context,_):-
	Predicate\=instance,
	deduceInstanceTable(Context,Predicate,'IrreflexiveRelation',_),
	Arg1==Arg2,!,fail.

% ===================================================
% IrreflexiveRelation False
% ===================================================
deduceGoalOProve(DB,VarsIn,VarsOut,false,Depth,Table,holds(Predicate,Arg1,Arg2),Agent,Context,P * Explaination * bullet(not(holds(Predicate,Arg1,Arg2)))):-
	Predicate\=instance,
	deduceInstanceTable(Context,Predicate,'IrreflexiveRelation',_),
	Arg1==Arg2,!,
	Explaination=sfindi((instance(Predicate, 'IrreflexiveRelation')=>
		forall(Arg1, not holds(Predicate, Arg1, Arg2)))).

deduceGoalOProve(DB,VarsIn,VarsOut,true,Depth,Table,holds(Predicate,Arg1,Arg2),Agent,Context,P * Explaination * bullet(not(holds(Predicate,Arg1,Arg2)))):-
	Predicate\=instance,
	Arg1==Arg2,
	deduceInstanceTable(Context,Predicate,'SymmetricRelation'),
	Explaination=sfindi((instance(Predicate, 'SymmetricRelation')=>
		forall(Arg1, holds(Predicate, Arg1, Arg2)))).


% Ground goal
%deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,Table,Fact,Ctx,Context,Explaination):-ground(Fact),
%	get_pred(Fact,Predicate,_),!,
%	prove_ground_goal(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context,Explaination).

% Instance goal
deduceGoalOProve(DB,VarsIn,VarsOut,true,BackchainsMax,Table,holds(instance,E,C),Ctx,Context,Explaination):-
	atom(C),!,
	((deduceInstanceTable(Context,E,C,Explaination))
	%;prove_finite(instance,Logic,BackchainsMax,Table,holds(instance,E,C),Ctx,Context,Explaination)
	).

% Instance goal
deduceGoalOProve(DB,VarsIn,VarsOut,true,BackchainsMax,Table,holds(instance,E,C),Ctx,Context,Explaination):-var(E),var(C),!,
	deduceInstanceTable(Context,C,'Class',_),not(C='Entity'),
	prove_finite(instance,true,BackchainsMax,Table,holds(instance,E,C),Ctx,Context,Explaination).

% Prove Non Ground Goal
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,Table,Fact,Ctx,Context, Explaination):-
	get_pred(Fact,Predicate,[A|Rgs]),
	(
	non_constrained(Context,Predicate,[A|Rgs]);
	mathmatic(Predicate)
	),
	deduceSurfaceDomainVector(N,Predicate,VectS,Agent,Context,_),!,
	enforce_type_for_bound(Context,Predicate,VectS,[A|Rgs]),!,
	prove_ground_goal(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context,Explaination).

enforce_type_for_bound(Context,Predicate,[],[]).
enforce_type_for_bound(Context,Predicate,[Vect|S],[A|L]):-
	enforce_type_for_bound_arg(Context,Predicate,Vect,A),
	enforce_type_for_bound(Context,Predicate,S,L).

enforce_type_for_bound_arg(Context,Predicate,_,A):-var(A),!.
enforce_type_for_bound_arg(Context,Predicate,Class,A):-
	deduceGoalOProve(DB,VarsIn,VarsOut,true,4,g_h([]),holds(instance,Arg,Class),Ctx,Context,Explaination).
	


non_constrained(Context,domain,[A|Rgs]).
non_constrained(Context,equal,[A|Rgs]).
non_constrained(Context,documentation,[A|Rgs]).
non_constrained(Context,_,[A|Rgs]).

% Prove Non Ground Goal
deduceGoalOProve(DB,VarsIn,VarsOut,BackchainsMax,Table,Fact,Ctx,Context, Explaination):-
	get_pred(Fact,Predicate,[A|Rgs]),
%	atom(Predicate),!,
	instance_all_predicate(Context,Predicate,[A|Rgs]),
	writeDebug(green,instanced_all(Context,Predicate,[A|Rgs])),%ground([A|Rgs]),
	%Predicate\=A,
	%safe_args([A|Rgs]),
	prove_ground_goal(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context,Explaination).
	*/
	
instance_all_predicate(Context,Predicate,Args):-
	deduceSurfaceDomainVector(N,Predicate,VectS,Agent,Context,_),!,
	p_deducePossibleInstancesFromClasslist(Context,VectS,Args).
	
p_deducePossibleInstancesFromClasslist(Context,[],[]).
p_deducePossibleInstancesFromClasslist(Context,[Class|Classes],[Arg|ArgS]):-
	p_deduceInstanceTable(Context,Arg,Class),
	p_deducePossibleInstancesFromClasslist(Context,Classes,ArgS).
	
p_deduceInstanceTable(Context,Arg,Class):- % true,
	deduceGoalOProve(DB,VarsIn,VarsOut,true,4,g_h([]),holds(instance,Arg,Class),Ctx,Context,Explaination),safe_arg(Arg).
	
% every description is eigther too general or too specific in most scenario cases.. 
% the real feat is to adapt to the user's mental pixelation

:-dynamic(is_tabled_true/4).
:-dynamic(is_tabled_incomplete/3).

% Prove Ground Goal
prove_ground_goal(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context,_):-
	is_tabled_incomplete(Logic,Fact,Context),!,fail.

prove_ground_goal(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context,Explaination):-
	is_tabled_true(Logic,Fact,Context,Explaination),!.


prove_ground_goal(equal,Logic,BackchainsMax,Table,Fact,Ctx,Context,Explaination):-!,
	prove_finite(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context,Explaination),
	ignore((ground((Fact)),asserta(is_tabled_true(Logic,Fact,Context,Explaination)))),!.

prove_ground_goal(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context,Explaination):-
	prove_finite(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context,Explaination),
	ignore((ground((Fact,Explaination)),asserta(is_tabled_true(Logic,Fact,Context,Explaination)))).
	
prove_ground_goal(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context,_):-
	ground(Fact),asserta(is_tabled_incomplete(Logic,Fact,Context)),!.
	

nochain(documentation).
%nochain(disjoint).
nochain(finishes).
nochain(part).
nochain(exhaustiveDecomposition).
nochain(domainSubclass).
nochain(subrelation).
nochain(domain).


get_pred(G,Predicate,Args):- G=..[holds,Predicate|Args],!.
get_pred(G,Predicate,Args):- G=..[Predicate|Args].


safe_args([]):-!.
safe_args([H|T]):-
	safe_arg(H),!,
	safe_args(T),!.
	
safe_arg(H):-atomic(H).
safe_arg(H):-var(H).
safe_arg(zzfn(_,S)):-!,not(funcs(S)).
safe_arg(zzskFn(_,S)):-not(funcs(S)).
safe_arg(_).

funcs([H|T]):-
	funct(H);funcs(T).

funct(foo):-!,fail.
funct(zzfn(_,_)).
funct(zzskFn(_,_)).
	
 % Prove Goal
prove_finite(_,Logic,BackchainsMax,Table,G,Ctx,Context,Explaination):-
	writeDebug(prove_finite(Logic,BackchainsMax,G,Ctx,Context,Table)),
	fail.
      
prove_finite(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context, g_h(Fact) * Table * Explaination ):-
	deduceSurface_gaf(Fact,Context,Ctx_,Explaination). %,ground(Fact).

prove_finite(equal,true,Depth,Table,equal(U,W),Agent,Context,P):-!,equal(U,W,P),!. %defined in moo_equal.P
prove_finite(equal,false,Depth,Table,equal(U,W),Agent,Context,P):-!,not_equal(U,W,P),!. %defined in moo_equal.P

prove_finite(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context, g_h(Fact) * Table * Explaination ):-
	mathmatic(Predicate),!,	
	belief_math(BackchainsMax,Table,Fact,Agent,Context,Explaination).
	
mathmatic(greaterThan).
mathmatic(lessThan).
mathmatic(greaterThanOrEqualTo).
mathmatic(lessThanOrEqualTo).
mathmatic(subset).
mathmatic(element).

	belief_math(Depth,Table,Goal,Agent,Context,_):-
		once(writeDebug(pos(Depth,Table,Goal,Agent,Context))),fail.
	
	%belief_math(Depth,Table,ANY,Agent,Context,_):-!,fail.
	
	belief_math(Depth,Table,'greaterThan'(U,W),Agent,Context,('greaterThan'(U,W))):-'greaterThan'(U,W).
	belief_math(Depth,Table,'lessThan'(U,W),Agent,Context,('greaterThan'(U,W))):-'lessThan'(U,W).
	belief_math(Depth,Table,'greaterThanOrEqualTo'(U,W),Agent,Context,('greaterThanOrEqualTo'(U,W))):-'greaterThanOrEqualTo'(U,W).
	belief_math(Depth,Table,'lessThanOrEqualTo'(U,W),Agent,Context,('lessThanOrEqualTo'(U,W))):-'lessThanOrEqualTo'(U,W).
	
	exact_memberchk(UWFS,Table,exact):-exact_memberchk(UWFS,Table),!.
	exact_memberchk(UWFS,Table,unify):-not(not(memberchk(UWFS,Table))),!.
	exact_memberchk(UWFS,Table,unique):-!.

prove_finite(Predicate,Logic,BackchainsMax,Table,Fact,Ctx,Context, VIA * Explaination ):-
	%ground(Fact),
	not(nochain(Predicate)),!,
	writeDebug(green,(Logic:Fact)),
	ado_cache_rule(Predicate,Fact, Conds, RT, Logic, Context, _Agent, (TID * Vars ), Props),
	not(member(on(_),Props)),
	writeDebug(teal,TID:Fact:Logic:Conds),
	check_loop(Table,Conds,Result),
	invoke_bakchain(Result,Fact,Conds,Table,TID,BackchainsMax,Ctx,Context,Explaination),
	ground(Fact),
	make_via(Logic,TID,VarsIn,VarsOut,Fact,Conds,VIA).
      
invoke_bakchain(yes,Fact,Conds,Table,TID,BackchainsMax,Ctx,Context,Explaination):-
	not(my_member(tid(TID),Table)),
	not(my_member(g_h(Fact),Table)),
        Backchains2 is BackchainsMax-3, %Since a Loop should be explored a little
	deduceGoalOProve(DB,VarsIn,VarsOut,true,Backchains2,(tid(TID)  * g_h(Fact) * Table  ),Conds,Ctx,Context, Explaination ).

invoke_bakchain(no,Fact,Conds,Table,TID,BackchainsMax,Ctx,Context,Explaination):-
	not(my_member(tid(TID),Table)),
	Backchains2 is BackchainsMax-1,
	deduceGoalOProve(DB,VarsIn,VarsOut,true,Backchains2,(tid(TID)  * g_h(Fact) * Table  ),Conds,Ctx,Context, Explaination ).

make_via(true,TID,VarsIn,VarsOut,Fact,Conds,via(entails(Conds,Fact),Vars) * TID).
make_via(false,TID,VarsIn,VarsOut,Fact,Conds,via(entails(Conds,not(Fact)),Vars) * TID).

check_loop(Table,and(Conds,R),yes):-not(ground(Conds)),!.
check_loop(Table,Conds,yes):-
	not(not(has_loop(Table,Conds))),!.
check_loop(Table,Conds,no).

has_loop(Table,and(A,B)):-!,
	(has_loop(Table,A);has_loop(Table,B)).
has_loop(Table,not(A)):-!,has_loop(Table,A),!.
has_loop(Table,C):-my_member(C,Table),!.
%has_loop(Table,C):-my_member(g_h(C),Table),!.


my_member(X,X *  _):-!.
my_member(X,((_ *  X) * _)):-!.
my_member(X,Y1 *  Y2):-!,my_member(X,Y2),!.
my_member(X,X).



% Computational Intelligence: a logical approach. 
% Prolog Code. 
% A CSP solver using arc consistency (Figure 4.8)
% Copyright (c) 1998, Poole, Mackworth, Goebel and Oxford University Press.
                                    
% csp(Domains, Relations) means that each variable has
% an instantiation to one of the values in its Domain 
% such that all the Relations are satisfied.
% Domains represented as list of 
% [dom(V,[c1,...,cn]),...]
% Relations represented as [rel([X,Y],r(X,Y)),...]
%  for some r

csp(Doms,Relns) :-
   ac(Doms,Relns).


% ac(Dom,Relns) is true if the domain constrants
% specified in Dom and the binary relations
% constraints specified in Relns are satisfied.
ac(Doms,Relns) :-
   make_arcs(Relns,A),
   consistent(Doms,[],A,A).

% make_arcs(Relns, Arcs) makes arcs Arcs corresponding to
% relations Relns. There are acrs for each ordering of
% variables in a relations.
make_arcs([],[]).
make_arcs([rel([X,Y],R)|O],
          [rel([X,Y],R),rel([Y,X],R)|OA]) :-
   make_arcs(O,OA).

% consistent(Doms,CA,TDA,A) is true if
% Doms is a set of domains
% CA is a set of consistent arcs,
% TDA is a list of arcs to do
% A is a list of all arcs
consistent(Doms,CA,TDA,A) :-
   consider(Doms,RedDoms,CA,TDA),
   solutions(RedDoms,A).            

% consider(D0,D1,CA,TDA)
% D0 is the set of inital domains
% D1 is the set of reduced domains
% CA = consistent arcs, 
% TDA = to do arcs
consider(D,D,_,[]).
consider(D0,D3,CA,[rel([X,Y],R)|TDA]) :-
   choose(dom(XV,DX),D0,D1),X==XV,
   choose(dom(YV,DY),D1,_),Y==YV, !,
   prune(X,DX,Y,DY,R,NDX),
   ( NDX = DX
   ->
     consider(D0,D3,[rel([X,Y],R)|CA],TDA)
   ; acc_todo(X,Y,CA,CA1,TDA,TDA1),
     consider([dom(X,NDX)|D1],D3,
              [rel([X,Y],R)|CA1],TDA1)).

% prune(X,DX,Y,DY,R,NDX)
% variable X had domain DX
% variable Y has domain DY
% R is a relation on X and Y
% NDX = {X in DX | exists Y such that R(X,Y) is true}
prune(_,[],_,_,_,[]).
prune(X,[V|XD],Y,YD,R,XD1):-
   \+ (X=V,member(Y,YD),R),!,
   prune(X,XD,Y,YD,R,XD1).
prune(X,[V|XD],Y,YD,R,[V|XD1]):-
   prune(X,XD,Y,YD,R,XD1).

% acc_todo(X,Y,CA,CA1,TDA,TDA1)
% given variables X and Y,
% updates consistent arcs from CA to CA1 and
% to do arcs from TDA to TDA1
acc_todo(_,_,[],[],TDA,TDA).        
acc_todo(X,Y,[rel([U,V],R)|CA0],
         [rel([U,V],R)|CA1],TDA0,TDA1) :-
   ( X \== V
   ; X == V,
     Y == U),   
   acc_todo(X,Y,CA0,CA1,TDA0,TDA1).
acc_todo(X,Y,[rel([U,V],R)|CA0],
         CA1,TDA0,[rel([U,V],R)|TDA1]) :-
   X == V,
   Y \== U,
   acc_todo(X,Y,CA0,CA1,TDA0,TDA1).

% solutions(Doms,Arcs) given a reduced set of
% domains, Dome, and arcs Arcs, solves the CSP.
solutions(Doms,_) :-
   solve_singletons(Doms).
solutions(Doms,A) :-
   select(dom(X,[XV1,XV2|XVs]),Doms,ODoms),
   split([XV1,XV2|XVs],DX1,DX2),
   acc_todo(X,_,A,CA,[],TDA),
   ( consistent([dom(X,DX1)|ODoms],CA,TDA,A)
   ; consistent([dom(X,DX2)|ODoms],CA,TDA,A)).

% solve_singletons(Doms) is true if Doms is a
% set of singletom domains, with the variables
% assigned to the unique values in the domain
solve_singletons([]).
solve_singletons([dom(X,[X])|Doms]) :-
   solve_singletons(Doms).

% select(E,L,L1) selects the first element of
% L that matches E, with L1 being the remaining
% elements.
select(D,Doms,ODoms) :-
   remove(D,Doms,ODoms), !.

% choose(E,L,L1) chooses an element of
% L that matches E, with L1 being the remaining
% elements.
choose(D,Doms,ODoms) :-
   remove(D,Doms,ODoms).

% split(L,L1,L2) splits list L into two lists L1 and L2
% with the about same number of elements in each list.
split([],[],[]).
split([A],[A],[]).
split([A,B|R],[A|R1],[B|R2]) :-
   split(R,R1,R2).



% Computational Intelligence: a logical approach. 
% Prolog Code. 
% A CSP solver using generate and test. 
% Copyright (c) 1998, Poole, Mackworth, Goebel and Oxford University Press.
                                    
% csp(Domains, Relations) means that each variable has
% an instantiation to one of the values in its Domain 
% such that all the Relations are satisfied.
% Domains represented as list of 
% [dom(V,[c1,...,cn]),...]
% Relations represented as [rel([X,Y],r(X,Y)),...]
%  for some r
%csp(Doms,Relns) :-
%   generate_and_test(Doms,Relns).

% generate_and_test(Doms,Relns) is true if we can
% find a value for each variable that satisfies
% Relns by generating and testing.
generate_and_test(Doms,Relns) :-
   generate(Doms),
   test(Relns).

generate([]).
generate([dom(X,D)|Ds]) :-
   member(X,D),
   generate(Ds).

test([]).
test([rel(_,R)|Rs]) :-
   call(R),
   test(Rs).

/*
member(A,[A|_]).
member(A,[_|L]) :-
   member(A,L).
*/


% Computational Intelligence: a logical approach. 
% Prolog Code. 
% CSP solver by picking random assignments
% Copyright (c) 1998, Poole, Mackworth, Goebel and Oxford University Press.

% this assumes that standard.pl and random.pl are also loaded.

% random_csp(Doms,Relns,Maxiter,Asst) chooses at most Maxiter assignments
% at random, until one satisfies Relns.
random_csp(Doms,Relns,_,Asst) :-
   random_assign(Doms,Asst),
   writeln(['Trying Assignment: ',Asst]),
   number_unsat_relns(Relns,Asst,0).
random_csp(Doms,Relns,Maxiter,Asst) :-
   Maxiter>1,
   IterLeft is Maxiter-1,
   random_csp(Doms,Relns,IterLeft,Asst).


   % number_unsat_relns(Relns,Asst,Depth) means N is the number of unsatisfied
% relations of Relns using assignment Asst
number_unsat_relns([],_,0).
number_unsat_relns([rel([X,Y],R)|Rels],Asst,Depth) :-
    number_unsat_relns(Rels,Asst,N0),
    (\+ (val_in_asst(X,Asst), val_in_asst(Y,Asst), R) 
    -> N is N0+1
     ; N=N0).


%%%% INTERFACE TO STANDARD CSP SETUP %%%%

% csp(Domains, Relations) means that each variable has
% an instantiation to one of the values in its Domain 
% such that all the Relations are satisfied.

/*
csp(Doms,Relns) :-
   random_csp(Doms,Relns,Depth,Ans),      % Depth is arbitrary setting
   set_all(Ans).
*/


% set_all(Asst) sets all of the variables in Asst to their values

% Computational Intelligence: a logical approach. 
% Prolog Code. 
% A CSP solver using GSAT. 
% Copyright (c) 1998, Poole, Mackworth, Goebel and Oxford University Press.

% this assumes that random.pl and standard.pl are also loaded.
                                    
% DATA STRUCTURES

% Domains represented as list of 
% [dom(X,[c1,...,cn]),...]

% Relations represented as [rel([X,Y],r(X,Y)),...]
%  for some r

% An assignment is represented as a list
% [val(X,V),...] where X is a variable
% and V is a value in the domain of X
% If variable X appears more than once, the first value is meant

% RELATIONS
/*
csp(Doms,Relns) :-
   gsat(Doms,Relns,Depth,Depth,Ans),      % Depth Depth is arbitrary setting
   set_all(Ans).
*/


% gsat(Doms,Relns,Depth,M,Ass) is true if Ass is an
% assignment of a value for each variable that satisfies
% Relns using GSAT. 
% N is the maxumum number of restarts.
% Each restart does M steps of hill climbing.
gsat(Doms,Relns,_,M,SAss) :-
   random_assign(Doms,Asst),
%   writeln(['Given Doms = ',Doms]),
   writeln(['   Random Assignment: ',Asst]),
   it_improve(Asst,Doms,Relns,M,SAss).
gsat(Doms,Relns,Depth,M,SAss) :-
   N>0,
   N1 is N-1,
   gsat(Doms,Relns,N1,M,SAss).

% random_assign(Doms,Asst) is true if Asst is a random assignment
% of a value to each variable, given domains Doms
random_assign([],[]).
random_assign([dom(X,D)|Ds],[val(X,E)|Vs]) :-
   random_elt(E,D),
   random_assign(Ds,Vs).

% it_improve(Asst,Doms,Relns,M,SAss)
% is true if, given
% Asst is a random Assignment,
% Doms is a set of domains
% Relns is a set of Relations
% M is a bound on the number of iterations,
% we can find an satisfying assignment SAss of values to variables that
% satisfies all of the relations.
it_improve(Asst,_,Relns,_,Asst) :-
   number_unsat_relns(Relns,Asst,NSat),
   writeln(['   Value = ',NSat]),
   NSat=0.                     % it randomly chose satisfying asst
it_improve(Asst,Doms,Relns,M,SAss) :-
   getPrologVars(Doms,Vars),
   improve_one(Vars,Doms,Relns,Asst,99999,Asst,BVal,BAss),
   it_improve_new(Asst,Doms,Relns,Vars,M,BVal,BAss,SAss).

% it_improve_new(Asst,Doms,Relns,Vars,M,BVal,BAss,SAss).
% is true if, given
% Asst is a random Assignment,
% Doms is a set of domains
% Relns is a set of Relations
% Vars is the list of all assignments
% M is a bound on the number of iterations,
% BVal is the value of the previous iteration
% BAss is the best assignment of the previous iteration
% we can find an satisfying assignment SAss of values to variables that
% satisfies all of the relations.
% Note that this is seperate from it_improve as we have to do
%  something different in the first iteration.
it_improve_new(_,_,_,_,_,BVal,val(Var,Val),_) :-
   writeln([' Assign ',Val,' to ',Var,'. New value = ',BVal]),
   fail.
it_improve_new(Asst,_,_,_,_,0,BAss,SAss) :-
   update_asst(Asst,BAss,SAss).
it_improve_new(Asst,Doms,Relns,Vars,M,_,val(Var,Val),SAss) :-
   M>0,
   rem_id(Var,Vars,RVars),
   update_asst(Asst,val(Var,Val),NAss),
   writeln(['    New Asst: ',NAss]),
   M1 is M-1,
   improve_one(RVars,Doms,Relns,NAss,99999,NAss,BVal2,BAss2),
   it_improve_new(NAss,Doms,Relns,Vars,M1,BVal2,BAss2,SAss).

% update_asst(Assts,Ass,SAss) given a list of assignments Assts,
% and a new assignment Ass returns the updated assignment list SAss.
update_asst([val(Var1,_)|Vals],val(Var,Val),[val(Var,Val)|Vals]) :-
   Var==Var1,!.
update_asst([val(Var1,Val1)|Vals],val(Var,Val),[val(Var1,Val1)|RVals]) :-
   update_asst(Vals,val(Var,Val),RVals).

% finds the best assignment to improve for this iteration
% improve_one(RemVars,             remaining variables to check
%         Doms,                    domains list
%         Relns,                   relations list
%         CurrentTotalAssignment,
%         CurrentBestValue,        
%         CurrentBestAssign,       current best val(Var,Val) to change
%         FinalBestValue,          final best value
%         FinalBestAssign)         val(Var,Val) for the best one to change
improve_one([],_,_,_,BV,BA,BV,BA).
improve_one(Vars,Doms,Relns,CTA,CBV0,CBA0,FBV,FBA) :-
   random_rem(Var,Vars,Vars2),
   domain_var_check(Var,Dom,Doms),
   lookup(Var,CTA,Val),
   remove(Val,Dom,RDom),
   check_other_vals_for_var(RDom,Var,Relns,CTA,CBV0,CBA0,CBV1,CBA1),
   improve_one(Vars2,Doms,Relns,CTA,CBV1,CBA1,FBV,FBA).

% check_other_vals_for_var(RDom,Var,Relns,CTA,CBV0,CBA0,CBV1,CBA1)
% checks the values RDom for variable Var
% Relns is the list of relations
% CTA is the current total assignment
% CBV0 is the previous best value
% CBA0 is the previous best assignment
% CBV1 is the final best value
% CBA1 is the final best assignment
check_other_vals_for_var([],_,_,_,CBV,CBA,CBV,CBA).
check_other_vals_for_var(Vals,Var,Relns,CTA,CBV0,CBA0,CBV1,CBA1) :-
   random_rem(Val,Vals,RVals),
   number_unsat_relns(Relns,[val(Var,Val)|CTA],Num),
   ( Num < CBV0
   -> check_other_vals_for_var(RVals,Var,Relns,CTA,Num,
                   val(Var,Val),CBV1,CBA1)
   ; check_other_vals_for_var(RVals,Var,Relns,CTA,CBV0,CBA0,CBV1,CBA1)
   ).
   

% domain_var_check(Var,Dom,Doms) is true if Dom is the domain of variable Var in Doms
domain_var_check(Var,Dom,[dom(Var1,Dom)|_]) :- Var==Var1,!.
domain_var_check(Var,Dom,[_|Doms]) :-
   domain_var_check(Var,Dom,Doms).

% val_in_asst(Var,Assignment) unifies Var with its value in Assignment
val_in_asst(Var,[val(Var1,Val1)|_]) :-
   Var==Var1,!,Var=Val1.
val_in_asst(Var,[_|Ass]) :-
   val_in_asst(Var,Ass).

% lookup(Var,Assignment,Val) unifies Var with its value in Assignment
lookup(Var,[val(Var1,Val1)|_],Val) :-
   Var==Var1,!,Val=Val1.
lookup(Var,[_|Ass],Val) :-
   lookup(Var,Ass,Val).

% rem_id(El,Lst,Rem) is true if Rem is the list remaining
% from removing the element of Lst that is identical to El.
rem_id(Var1,[Var|Vars],RVars) :-
   Var==Var1, !, RVars=Vars.
rem_id(Var1,[Var|Vars],[Var|RVars]) :-
    rem_id(Var1,Vars,RVars).

% getPrologVars(Doms,Vars) is true if Vars is the list of variables in Doms
getPrologVars([],[]).
getPrologVars([dom(X,_)|Ds],[X|Vs]) :-
   getPrologVars(Ds,Vs).

%%%% INTERFACE TO STANDARD CSP SETUP %%%%

% csp(Domains, Relations) means that each variable has
% an instantiation to one of the values in its Domain 
% such that all the Relations are satisfied.




% set_all(Asst) sets all of the variables in Asst to their values
set_all([]).
set_all([val(X,X)|R]) :-
   set_all(R).



% Computational Intelligence: a logical approach. 
% Prolog Code. 
% A crossword puzzle example of Figure 4.13.
% Copyright (c) 1998, Poole, Mackworth, Goebel and Oxford University Press.

crossword(A1,A2,A3,D1,D2,D3) :-     
    cross(A1,A2,A3,D1,D2,D3,
          [[a,d,d], [a,d,o], [a,g,e], [a,g,o], [a,i,d],
           [a,i,l], [a,i,m], [a,i,r], [a,n,d], [a,n,y],
           [a,p,e], [a,p,t], [a,r,c], [a,r,e], [a,r,k],
           [a,r,m], [a,r,t], [a,s,h], [a,s,k], [a,u,k],
           [a,w,e], [a,w,l], [a,y,e], [b,a,d], [b,a,g],
           [b,a,n], [b,a,t], [b,e,e], [b,o,a], [e,a,r],
           [e,e,l], [e,f,t], [f,a,r], [f,a,t], [f,i,t],
           [l,e,e], [o,a,f], [r,a,t], [t,a,r], [t,i,e]
          ]).
           

cross(A1,A2,A3,D1,D2,D3,Words) :-
    csp([dom(A1,Words),
         dom(A2,Words),
         dom(A3,Words),
         dom(D1,Words),
         dom(D2,Words),
         dom(D3,Words)],
        [rel([A1,D1],compatible(A1,1,D1,1)),
         rel([A1,D2],compatible(A1,2,D2,1)),
         rel([A1,D3],compatible(A1,3,D3,1)),
         rel([A2,D1],compatible(A2,1,D1,2)),
         rel([A2,D2],compatible(A2,2,D2,2)),
         rel([A2,D3],compatible(A2,3,D3,2)),
         rel([A3,D1],compatible(A3,1,D1,3)),
         rel([A3,D2],compatible(A3,2,D2,3)),
         rel([A3,D3],compatible(A3,3,D3,3))]).

compatible(Word1,I1,Word2,I2) :- 
    common_letter(Word1,I1,Word2,I2).

common_letter(Word1,I1,Word2,I2) :-
    letter(Word1,I1,Letter),
    letter(Word2,I2,Letter).

letter(Word,I,Letter) :- nth(I,Word,Letter).

% | ?- crossword(A,B,C,D,E,F).

% Computational Intelligence: a logical approach. 
% Prolog Code. 
% Random number generater (Example 2.9 page 485)
% Copyright (c) 1998, Poole, Mackworth, Goebel and Oxford University Press.

:- dynamic seed/1.

seed(447563523).

% rand(R) generates random real number R in the range [0,1)
rand(R) :- R is random(5000) / 5000.

  /*
   retract(seed(S)),
   N is (S * 314159262 + 453816693) mod 2147483647,
   assert(seed(N)),
   R is N / 2147483647.0 .
	*/
  
% ramdom(R,M) generates random integer R in the range 0..M-1
random(R,M) :-
    rand(RR),
    R is integer(M * RR).

% random_list(N,L) creates list L of length N of random numbers in range [0,1)
random_list(0,[]).
random_list(N,[R|L]) :-
    N>0,
    rand(R),
    N1 is N-1,
    random_list(N1,L).

% random_elt(E,L) selects a random element E of list L
random_elt(E,L) :-
   length(L,Depth),
   random(R,Depth),
   N1 is R+1,
   nth(N1,L,E).

% random_rem(E,L,R) selects a random element E of
% list L where list R is contains the other elements.
random_rem(E,L,R) :-
   length(L,Depth),
   random(Rand,Depth),
   N1 is Rand+1,
   nth(N1,L,E,R).


% nth(N,L,E,R) is true if E is the Nth element of
% list L, and R is the remianing elements. We start
% counting positions from 1.
nth(1,[E|R],E,R).
nth(N,[H|T],E,[H|R]) :-
   N>1,
   N1 is N-1,
   nth(N1,T,E,R).

nth(X,Y,Z):-nth1(X,Y,Z).


% :-['moo_csat.pl'].




% remove(E,L,R) is true if E is an element of L and R is the remaining
% elements after E is removed.
remove(E,[E|R],R).
remove(E,[A|L],[A|R]) :-
   remove(E,L,R).


% ==========================================
% BELIEF
% ==========================================

:-dynamic(complete_goal/1).

:-multifile(expireOptimizationsInContext/3).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

/*

This file impliments explaination theory over a compiled derivative of SUO-KIF

For Axiom Key See http://plato.stanford.edu/entries/logic-modal/

*/

% ==========================================
% Hit Variables
% ==========================================
/*deduceGoalOProve(Predicate,Logic,Depth,Table,L,Agent,Context,P ):-
	format('~q.\n',[deduceGoalOProve(Predicate,Logic,L,Agent,Context,Depth,Table)]),fail.
  */


deduceGoalOProve(Predicate,Logic,Depth,Table,subclass(X,Y),Agent,Context,Explaination):-X=='Entity',!,Y='Entity'.

deduceGoalOProve(Predicate,Logic,Depth,Table,Var,Agent,Context,Explaination):-getPrologVars(Var,[_,_,_|_],_,_),!,fail.
deduceGoalOProve(_,true,Depth,Table,equal(U,W),Agent,Context,P):-!,equal(U,W,P),!.  %defined in moo_equal.P
deduceGoalOProve(_,true,Depth,Table,not equal(U,W),Agent,Context,P):-!,not_equal(U,W,P),!. %defined in moo_equal.P
deduceGoalOProve(holds,Logic,Depth,Table,holds(holds,_,_),Agent,Context,Explaination):-!,fail.
deduceGoalOProve(Predicate,Logic,Depth,Table,Var,Agent,Context,Explaination):-
	once(writeDebug(deduceGoalOProve(Predicate,Logic,Var,Depth,Agent,Context,Table))),fail.


deduceGoalOProve(holds,Logic,Depth,Table,Var,Agent,Context,Explaination):-
	predicate_holds(Var,Predicate),	!,
	deduceGoalOProve(Predicate,Logic,Depth,Table,Var,Agent,Context,Explaination).
	
predicate_holds(holds(Predicate,_,_),Predicate).
predicate_holds(holds(Predicate,_,_,_),Predicate).
predicate_holds(holds(Predicate,_,_,_,_),Predicate).
predicate_holds(holds(Predicate,_),Predicate).
predicate_holds(G,Predicate):-functor(G,Predicate,_).


deduceGoalOProve(Predicate,Logic,Depth,Table,formula,Agent,Context,ExplainationB ):-!,fail.
%	writeDebug(red,formula),!,fail.

deduceGoalOProve(Predicate,var,Depth,Table,_,Agent,Context,B ):-!, true,fail.

deduceGoalOProve(_,Logic,Depth,Table,and(A,B),Agent,Context,ExplainationA * ExplainationB ):-!,
	deduceGoalOProve(holds,Logic,Depth,Table,A,Agent1,Context,ExplainationA ),
	deduceGoalOProve(holds,Logic,Depth,Table,B,Agent2,Context,ExplainationB ).

deduceGoalOProve(_,Logic,Depth,Table,gafs(Logic,A),Agent,Context,ExplainationA ):-!,
	writeDebug(red,defering_to_gafs(Logic,A)),
	deduceGoalOProve(holds,Logic,Depth,[defering_to_gafs(Logic,A)|Table],A,Agent1,Context,ExplainationA ).

deduceGoalOProve(_,Logic,Depth,Table,delay(Logic,A),Agent,Context,ExplainationA ):-!,
	%(mooCache(completed,template(Logic,Template),_)),
	writeDebug(red,delay_to_gafs(Logic,A)),!,
	deduceGoalOProve(holds,Logic,Depth,[delay_to_gafs(Logic,A)|Table],A,Agent1,Context,ExplainationA ).
 
% ==========================================
% Double Negation (Call)
% ==========================================
deduceGoalOProve(not,Logic,Depth,Table,not(not(NewRequest)),Agent,Context,ExplainationB ):-
	writeDebug(not_not),
	functor(NewRequest,Predicate,_), !,
	deduceGoalOProve(Predicate,Logic,Depth,Table,Var,Agent,Context,ExplainationB ).

% ============================================================
% Invert Not
% ============================================================
deduceGoalOProve(not,Logic,Depth,Table,not(NewRequest),Agent,Context,Explaination):-
	functor(NewRequest,Predicate,_),
	invert_logic(Logic,NewLogic),!,
	deduceGoalOProve(Predicate,NewLogic,Depth,Table,NewRequest,Agent,Context,Explaination).
	



invert_logic(true,false).
invert_logic(false,true).

	
% =============================================
% Choose between deduce_nonground_goal/deduce_ground_goal
% =============================================
deduceGoalOProve(Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-  %true,
	ground(Goal),!,
	deduce_ground_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P),!.

deduceGoalOProve(Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	term_to_atom(Goal,Template),!,
	deduce_nonground_goal(Template,Predicate,Logic,Depth,Table,Goal,Agent,Context,P).

	
% =============================================
% Prove a Nonground Goal
% =============================================
deduce_nonground_goal(Template,Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	mooCache(failed,nonground(Logic,Template,Goal),tried(Agent,Context)),!,
	writeDebug(fail_nonground_goal(Logic,Template,Goal)),!,
	fail.
	

deduce_nonground_goal(Template,Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	mooCache(completed,template(Logic,Template),tried(Agent,Context)),!,
	writeDebug(using_complete(Template)), !,
	mooCache(found,nonground(Logic,Template,Goal),P),
	writeDebug(in_db(Logic,Goal,Agent,Context,P)).

		
deduce_nonground_goal(Template,Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	retractall(mooCache(found,nonground(Logic,Template,_),_you )),fail.
	
defered_fact(Logic,Agent,Context,Goal):-
	fact_to_template(Goal,Template),
	defered(Logic,Template,Agent,Context,Goal).

defered(Logic,Template,Agent,Context,Goal):-
	not(not(mooCache(tabling,template(Logic,_,Goal),tried(Agent,Context)))).

deduce_nonground_goal(Template,Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	once(asserta(mooCache(tabling,template(Logic,Template,Goal),tried(Agent,Context)))),fail.

deduce_nonground_goal(Template,Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	prove_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P),
	record_complete(Logic,Template,Agent,Context,Goal,P).
	
record_complete(Logic,Template,Agent,Context,Goal,P):-mooCache(found,nonground(Logic,Template,Goal)),!.
record_complete(Logic,Template,Agent,Context,Goal,P):-asserta(mooCache(found,nonground(Logic,Template,Goal),P)).
	
deduce_nonground_goal(Template,Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	retractall(mooCache(tabling,template(Logic,Template,Goal),tried(Agent,Context))),
	asserta(mooCache(completed,template(Logic,Template),tried(Agent,Context))),!.

% =============================================
% Prove a Ground Goal
% =============================================
deduce_ground_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	mooCache(failed,ground(Logic,Goal),tried(Agent,Context)),!,
	writeDebug(fail_ground_goal(Logic,Goal)),!,
	fail.

deduce_ground_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	mooCache(found,ground(Logic,Goal),P),!,
	writeDebug(in_db(Logic,Goal,Agent,Context,P)),!.

deduce_ground_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	mooCache(found,nonground(Logic,Template,Goal),P),!,
	writeDebug(in_db(Logic,Goal,Agent,Context,P)),!.
	
deduce_ground_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	once(prove_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P)),
	asserta(mooCache(found,ground(Logic,Goal),P)),!.

% Persist Fail Only Truth
deduce_ground_goal(Predicate,true,Depth,Table,Goal,Agent,Context,P):-
	writeDebug(red,[tabled,failed,true,Goal,tried(Agent,Context)]),
	asserta(mooCache(failed,ground(true,Goal),tried(Agent,Context))),!,fail.



% ============================================================
% Write the Goal to debugger
% ============================================================
:-index(deduceGoalOProve(1,1,0,0,1,0,0,0)).


prove_goal(Predicate,Logic,Depth,Table,Var,Agent,Context,Explaination):-
	once(writeDebug(prove_goal(Predicate,Logic,Var,Depth,Agent,Context,Table))),fail.


prove_goal(Predicate,Logic,Depth,Table,holds(domain, [P, N,T]),Agent,Context,Explaination):-var(P),!,fail.

/*
prove_goal(Predicate,Logic,Depth,Table,holds(Predicate,[H|_]),Agent,Context,Explaination):-%true,
		     H=='Entity',!,fail.
prove_goal(Predicate,Logic,Depth,Table,holds(Predicate,[_,H|_]),Agent,Context,Explaination):-
		     H=='Entity',!,fail.
*/
%holds(part, ['Entity', 'SaudiArabia']))

%finite_goal('.',Logic,Depth,Table,[H|T],Agent,Context,Explaination):-!,
%	deduce_backchain(Logic,Depth,Table,[H|T],Agent,Context,Explaination).

% ============================================================
% Invert Not
% ============================================================
prove_goal(not,true,Depth,Table,not(NewRequest),Agent,Context,Explaination):-!,
	invert_logic(Logic,NewLogic),
	functor(NewRequest,Predicate,_),!,
	deduceGoalOProve(Predicate,NewLogic,Depth,Table,NewRequest,Agent,Context,Explaination).

% ==============================================   
% Time space Limits
% ==============================================   

prove_goal(Predicate,Logic,Depth,Table,Var,Agent,Context,Explaination):-
	(
	(getCputime(Now),cpuend(Then),Now>Then,writeDebug(timeout(Now>Then)))
	;
	(var(Var),once(writeDebug(vaR)))
	;catch((Depth<(-1),once(writeDebug(too_far))),_,true)
	),
	%asserta(noexistencials),
	!,fail.


%prove_goal(Predicate,Logic,Depth,Table,Var,Agent,Context,Explaination):-true,fail



	

% ==========================================
% Member BELIEF OPTIMIZATION
% ==========================================
/*
finite_goal(member,Logic,Depth,Table,member( X,Y),Agent,Context,P ):-!,
	prove_goal_member(Logic,Depth,Table,X,Y,Agent,Context,P ).

% ==========================================
% Attribute BELIEF OPTIMIZATION
% ==========================================

finite_goal(attribute,Logic,Depth,Table,attribute( X,Y),Agent,Context,P ):-!,
	prove_goal_attribute(Logic,Depth,Table,X,Y,Agent,Context,P ).
  */


% ==========================================
% Instance BELIEF OPTIMIZATION
% ==========================================

prove_goal(_,Logic,Depth,Table,instance( X,Y),Agent,Context,P ):-
	prove_goal_instance(Logic,Depth,Table,X,Y,Agent,Context,P ).
	

% ================================================
% True: Check Database for Goal
% ================================================
prove_goal(Predicate,true,Depth,Table,Goal,Agent,Context,Explaination):-
	deduceSurfaceGuarded(Predicate,true,Goal,Agent,Context,Explaination),
	writeDebug(green,found(Goal)).

% ================================================
% False: Check Database for Not Goal (Success)
% ================================================
prove_goal(Predicate,false,Depth,Table,Goal,Agent,Context,Explaination):-
	deduceSurfaceGuarded(Predicate,false,Goal,Agent,Context,Explaination),
	writeDebug(green,found(not(Goal))).
	     


   
prove_goal(Predicate,_,Depth,Table,Goal,Agent,Context,Explaination):-
	(disabled_backchain(Predicate),!,fail).

prove_goal(_,_,Depth,Table,holds(Predicate,_,_),Agent,Context,Explaination):-
	(disabled_backchain(Predicate),!,fail).

prove_goal(Predicate,_,Depth,Table,Goal,Agent,Context,Explaination):-
	atom(Predicate),
	disabled_backchain(Predicate),!,fail.
	
disabled_backchain(temporalPart).
disabled_backchain(part).
disabled_backchain(result).
disabled_backchain(equal).
disabled_backchain(copy).
disabled_backchain(manner).
disabled_backchain('piece'). 

disabled_backchain(A):-atom_codes(A,[115,117,98|_]). %sub*

prove_goal(temporalPart,true,Depth,Table,holds(temporalPart,Arg,Arg),Agent,Context,incode(holds(temporalPart,Arg,Arg),'sameness')).


%prove_goal(_,Logic,Depth,Table,subclass( 'Entity',Y),Agent,Context,P ):-!,fail.

prove_goal(holdsDuring,true,Depth,Table,holds(holdsDuring,TimePosition,Situation),Agent,Context, Explaination):- 
	once((writeDebug(subgoal(true,Situation,Agent,Context)),
        once(request_compile((Situation),NewRequest,Context,UVars,Given)),
	writeDebug(pink,subgoal(NewRequest,Agent,Context)), 
	functor(NewRequest,Predicate,_))),
        deduceGoalOProve(Predicate,true,Depth,Table,NewRequest,Agent,Context,Explaination).

% ==========================================
% Transition FINITE GOAL
% ==========================================

/*
prove_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	nobackchain_on(Predicate),!,
	writeDebug(red,nobackchain_on(PR)),fail.
*/

/*
prove_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
        not(not(memberchk(Goal,Table))),!,
	writeDebug(red,memberchk(Goal,Table)),fail.
*/
	
prove_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P):-
	instanciate_finate(Predicate,Logic,Depth,Table,Goal,Agent,Context),
	finite_goal(Predicate,Logic,Depth,Table,Goal,Agent,Context,P).
	%After chain completers term must be ground
	%ground(Goal).

instanciate_finate(Predicate,Logic,Depth,Table,VAR,Agent,Context):-ground(VAR),!.


%instanciate_finate(instance,Logic,Depth,Table,instance(E,C),Agent,Context):-!,
%	deduceSurface(instance,true,instance(E,C),Agent,Context,_).
instanciate_finate(Predicate,Logic,Depth,Table,instance(E,C),Agent,Context):-!,
	deduceInstanceTable(Context,C,'Class',_),not(C='Entity').
	%deduceSurface_dc(instance,true,instance([C,'Class']),Agent,Context,Explaination).

instanciate_finate(Predicate,Logic,Depth,Table,holds(holdsDuring,T,S),Agent,Context):-!,
	nonvar(T),nonvar(S).
		
	%%instance_all(Context,Predicate,Arg,SO). %,ground(Arg,SO).
instanciate_finate(Predicate,Logic,Depth,Table,holds(Predicate,Arg,SO),Agent,Context):-%true,
	instance_all(Context,Predicate,[Arg,SO]). %,ground(Arg,SO).

instanciate_finate(Predicate,Logic,Depth,Table,holds(Predicate,Arg,S,O),Agent,Context):-
	instance_all(Context,Predicate,[Arg,S,O]). %,ground(Arg,SO).

% ==========================================
% PROVE FINITE GOAL
% ==========================================

finite_goal(Predicate,Logic,Depth,Table,Var,Agent,Context,Explaination):-
	once(writeDebug(finite_goal(Predicate,Logic,Var,Depth,Agent,Context,Table))),fail.

/*
% ================================================
% True: Check Database for Not Goal (only ground)
% ================================================
finite_goal(Predicate,true,Depth,Table,Goal,Agent,Context,Explaination):-
		deduceSurfaceGuarded(Predicate,false,Goal,Agent,Context,P),!,writeDebug(countered(Goal)),!,fail.

% ================================================
% False: Check Database for Goal (if there is then fail)
% ================================================
finite_goal(Predicate,false,Depth,Table,Goal,Agent,Context,Explaination):-
		deduceSurfaceGuarded(Predicate,true,Goal,Agent,Context,P),!,writeDebug(encountered(Goal)),!,fail.
*/




% ==========================================
% "holdsDuring" BELIEF
% http://www.google.com/search?q=cache:hRA54mmgLc8:www-formal.stanford.edu/guha/guha-thesis.ps+holdsDuring+PRolog&hl=en
% ==========================================

/*
% Break a holdsDuring on Argument Type Violation
finite_goal(holdsDuring,Logic,Depth,Table,holdsDuring(TimePosition,Situation),Agent,Context, Explaination):-
	not(deduceGoalOProve(instance,true,Depth,Table,instance(TimePosition,'TimePosition'),Agent,Context, ITime)),!,fail.

% Break a holdsDuring on Free Situation
finite_goal(holdsDuring,Logic,Depth,Table,holdsDuring(TimePosition,false),Agent,Context, Explaination):- !,fail.

% holdsDuring(TimePosition,and(Situation1,Situation2)) => and(holdsDuring(TimePosition,Situation1),holdsDuring(TimePosition,Situation))
*/
/*
finite_goal(holdsDuring,Logic,Depth,Table,holdsDuring(TimePosition,and(Situation1,Situation2)),Agent,Context, Explaination):- 
	deduceGoalOProve(and,Logic,Depth,Table,
	and(holdsDuring(TimePosition,Situation1),holdsDuring(TimePosition,Situation)),
	Agent,Context, Explaination).
*/
    
% Situation => holdDuring(AnyTime,Situation) 
	      
/*
deduce_holdsDuring(Predicate,Logic,Depth,Table,TimePosition,Situation,Agent,Context,Explaination ):-
	writeDebug(deduce_holdsDuring(Predicate,Logic,holdsDuring,TimePosition,Situation)),fail.


deduce_holdsDuring(Predicate,Logic,Depth,Table,TimePosition,Situation,Agent,Context,ITime * Explaination * ConditionalExplaination ):-true,
	writeDebug(green,[holdsDuring,TimePosition,Logic,Situation]),
	client_rulebase(holdsDuring,Logic,holdsDuring(TimePosition,Situation), Agent,Context, Conditions, Explaination, F),
	writeDebug(green,[Conditions=>holdsDuring,TimePosition,Logic,Situation]),
	functor(Situation,NewPredicate),
	deduce_holdsDuring(NewPredicate,Logic,Depth,Table,TimePosition,Conditions,Agent,Context,ConditionalExplaination).
  */   
  

% ==========================================
% "entails" BELIEF
% ==========================================


finite_goal(entails,true,Depth,Table,entails(Pre,Goal),Agent,Context, Explaination):-!,
	((nonvar(Goal),functor(Goal,Predicate,_));nonvar(Pre)),
	writeDebug(green,entails(Pre,Goal)),!,
	deduce_entails(Predicate,true,Depth,Table,Pre,Goal,Agent,Context,Explaination ),
	writeDebug(green,succeed_deduces(true,Pre,Goal)).


deduce_entails(Predicate,Logic,Depth,Table,Pre,Goal,Agent,Context,Explaination ):-
	writeDebug(deduce_entails(Predicate,Logic,entails(Pre,Goal),Depth,Table,Agent,Context,Explaination )),fail.
    
deduce_entails(Predicate,true,Depth,Table,attribute(O,R),not(attribute(O,L)),Agent,Context,Explaination):-
	deduceGoalOProve(contraryProperty,true,Depth,Table,contraryProperty(L,R),Agent,Context,Explaination ).

deduce_entails(Predicate,true,Depth,Table,Pre,Goal,Agent,Context, Explaination ):-
	Pre==true,!,deduceGoalOProve(Predicate,true,Depth,Table,Goal,Agent,Context, Explaination).

deduce_entails(Predicate,true,Depth,Table,Pre,Goal,Agent,Context, Explaination ):-
	Pre==false,!,fail.  %Absurdy

deduce_entails(not,true,Depth,Table,Pre,not(Goal),Agent,Context, Explaination ):-!,nonvar(Goal),
	functor(Goal,Predicate,_),
	deduce_entails(Predicate,false,Depth,Table,Pre,Goal,Agent,Context, Explaination ).

deduce_entails(not,false,Depth,Table,Pre,not(Goal),Agent,Context, Explaination ):-!,nonvar(Goal),
	functor(Goal,Predicate,_),
	deduce_entails(Predicate,true,Depth,Table,Pre,Goal,Agent,Context, Explaination ).

deduce_entails(Predicate,Logic,Depth,Table,Reqs,Goal,Agent,Context, Explaination ):-
	client_entailmentbase(Logic,Depth,Table,Reqs,Goal,Agent,Context, Explaination ),
	not(memberchk(Reqs,Table)),
	writeDebug(silver,found_entails(Reqs,Logic,Goal)).
	
		
remove_conj(and(Pre,LeftOver),Pre,LeftOver).
remove_conj(and(LeftOver,Pre),Pre,LeftOver).
remove_conj(Pre,Pre,true).

              
/*
% ==========================================
% Range BELIEF	(Transitive closure)
% ==========================================       
finite_goal(Predicate,true,Depth,Table,range( Function, SubClass),Agent,Context,(Explaination1 * Explaination2 * Explaination3)):-
	nonvar(Function),
	deduceTransitiveClosure_PartialOrderingRelation(Context,subrelation,Function,Super,Explaination1),
	Function \= Super,
	deduceGoalOProve(Predicate,true,Depth,Table,range( Super, Class),Agent,Context,Explaination2),
	%write(Class),
	deduceTransitiveClosure_PartialOrderingRelation(Context,subclass,SubClass,Class,Explaination3).
	
% =====================================================
% RANGE TRAVELS UP TO SUPERRELATION (If no assertions Found)
% =====================================================
finite_goal(Predicate,true,Depth,Table,range( Function, Class),Agent,Context,Explaination1 * Explaination2):-
	var(Function),nonvar(Class),
	deduceGoalOProve(Predicate,true,Depth,Table,range( Super, Class),Agent,Context,Explaination2),
	deduceTransitiveClosure_PartialOrderingRelation(Context,subrelation,Function,Super,Explaination1).

*/
% ==========================================
% CONTENT BELIEF   
% ==========================================

/*
finite_goal(Predicate,true,Depth,Table, subsumesContentInstance( S1, S2),Agent,Context,Context:TN:('=>'( S1, S2))^Vars):-
		deduceSurfaceGuarded(Predicate,true,surface,(S1=>S2),Vars,Context,Context,TN,Maintainer,Accept).
finite_goal(Predicate,true,Depth,Table, equivalentContentInstance( S1, S2),Agent,Context,Context:TN:('<=>'( S1, S2))^Vars):-
		deduceSurfaceGuarded(Predicate,true,surface,(S1<=>S2),Vars,Context,Context,TN,Maintainer,Accept).

finite_goal(Predicate,true,Depth,Table,assertedFormula(Form),Agent,Context,assertedFormula(Form)):-in_cache(Form,Context,Context,Explaination).

finite_goal(Predicate,true,Depth,Table,assertedTermFormulas(Goal,Form),Agent,Context,assertedTermFormulas(Goal,Form)):-in_cache(Form,Context,Context,Explaination),once(getConstants(atomic,Form,Lists,_,_)),memberchk(Goal,Lists).


finite_goal(Predicate,Depth,Table,'<=>'(L,St),Agent,Context,Explaination):-!,
        writeDebug(goal('<=>'(L,St))),
	deduce_backchain(Depth,Table,'<=>'(L,St),Agent,Context,Explaination).

*/

% ===================================================
% Stop Crazy Existentials 
% ===================================================

/*
finite_goal(Predicate,Logic,Depth,Table,holds(Predicate,[Arg1,Arg2]),Agent,Context,Explaination):-
	((nonvar(Arg1),functor(Arg1,_,2));(nonvar(Arg2),functor(Arg2,_,2))),
	!,writeDebug(red,unwind(holds(Predicate,Arg1,Arg2))),!,fail.	       
*/
% ==========================================
% Special Predicate BELIEF
% ==========================================

/*
finite_goal(Predicate,Logic,Depth,Table,holds(Predicate,Arg1,Arg2),Agent,Context,Explaination):-
	fail,
	belief_math(Depth,['RelationExtendedToQuantities'|Table],holds(Predicate,Arg1,Arg2),Agent,Context,Explaination).
*/


% ===================================================
% No Backchain if Defered
% ===================================================
finite_goal(Predicate,Logic,Depth,Table,holds(Predicate,Arg1,Arg2),Agent,Context,Explaination):-
		 memberchk(defering_to_gafs(Logic,Predicate),Table),
		 writeDebug(red,backchain_defered(Predicate)),!,fail.

% ===================================================
% No Backchain Ever if uncommented
% ===================================================
%finite_goal(Predicate,Logic,Depth,Table,holds(Predicate,Arg1,Arg2),Agent,Context,Explaination):-!,fail.



/*
%Quick Negation check (finites are ground)  obnly for non constaints
finite_goal(Predicate,TF,Depth,Table,holds(Predicate,Arg1,Arg2),Agent,Context,Explaination ):-
	not(constraint_functor(Predicate)),
	backchain_goal(Predicate,false,Depth,Table,holds(Predicate,Arg1,Arg2),Agent,Context,Explaination),
	writeDebug(pink,refuted(holds(Predicate,Arg1,Arg2))),
	!,fail.

*/

% ===================================================
% Modus Ponens
% ===================================================
finite_goal(Predicate,true,Depth,Table,Fact,Agent,Context,Explaination * Explaination2 ):-
	writeDebug(calling_backchain(Fact,Table)),
	confirm_callable((Fact)),
	Depth2 is Depth -1,!,  %true,
	client_rulebase_spec(Predicate,true,Fact, Agent,Context, Conditions, Explaination, F,Type),
	confirm_rule(Predicate,true,Fact, Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds),
	deduceGoalOProve(holds,true,Depth2,[Fact|NewTable],NewConds,Agent,Context,Explaination2),
	confirm_ground(Conditions).
	

%constraint_functor(X):-not(atom(X)),!.
constraint_functor(instance).
constraint_functor(domain).
constraint_functor(valence).
constraint_functor(subclass).
constraint_functor(subrelation).
constraint_functor(disjoint).
constraint_functor(Sub):-atom_codes(Sub,[115,117,98|_]).


nobackchain_on(X):-not(atom(X)),!.
nobackchain_on(subrelation).

nobackchain_on(domain).
nobackchain_on(domainSubclass).
nobackchain_on(subclass).
nobackchain_on(Sub):-atom_codes(Sub,[115,117,98|_]).

% ============================================================
% Modus Tollens
% ============================================================
finite_goal(Predicate,false,Depth,Table,Fact,Agent,Context,Explaination * Explaination2 ):-
	writeDebug(calling_backchain(not(Fact),Table)),
	confirm_callable(not(Fact)),
	Depth2 is Depth -1,!,
	client_rulebase_spec(Predicate,false,Fact, Agent,Context, Conditions, Explaination, F,Type),
	confirm_rule(Predicate,false,Fact, Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds),
	deduceGoalOProve(holds,true,Depth2,[Fact|NewTable],NewConds,Agent,Context,Explaination2),
	confirm_ground(Conditions).

/*

confirm_callable(not(holds(Predicate,Arg1,Arg2))):-
	not(constraint_functor(Predicate)),
	writeDebug(red,no_constraint_functor(holds(Predicate,Arg1,Arg2))),!,
	fail.
*/	
/*
confirm_callable(not(holds(Predicate,Arg1,Arg2))):-
	not(('zzskFn'(_,_)=Arg1)),
	not(('zzskFn'(_,_)=Arg2)),
	not(('zzfn'(_,_)=Arg1)),
	not(('zzfn'(_,_)=Arg2)),
	writeDebug(red,no_zzt(holds(Predicate,Arg1,Arg2))),!,
	fail.
*/	
confirm_callable(_):-!.



confirm_rule(Predicate,TF,I, Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds):-
	memberchk(ident(equal),Type),
	writeDebug(red,no_ident(I,Conditions)),
	!,fail.


confirm_rule(Predicate,_,_, Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds):-
	memberchk(on(_),Type),!,fail.


% Succeed
confirm_rule(Predicate,false,holds(Predicate,Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type,Depth,Table,[defering_to_gafs(false,Predicate)|Table],Conditions):-
	writeDebug(purple,Type:(not(holds(Predicate,Arg1,Arg2)):modus_tollens:Conditions)),!.

% Succeed
confirm_rule(Predicate,true,holds(Predicate,Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type,Depth,Table,[defering_to_gafs(true,Predicate)|Table],Conditions):-
	writeDebug(blue,Type:(holds(Predicate,Arg1,Arg2):modus_ponens:Conditions,Explaination)),!.


confirm_rule(Predicate,L,G, Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds):-
	G=..[Predicate,Arg1,Arg2],!,
	confirm_rule(Predicate,L,holds(Predicate,Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds).
	

% Never gets here
confirm_rule(Predicate,false,holds(Predicate,Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds):-
	memberchk('zzskFn'(_),Type),
	writeDebug(red,no_skolems_on_modus_tollens(holds(Predicate,Arg1,Arg2),Conditions)),
	!,fail.

confirm_rule(Predicate,TF,holds(holds,Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds):-
	memberchk(ident(_),Type),
	writeDebug(red,no_ident(holds(Predicate,Arg1,Arg2),Conditions)),
	!,fail.

confirm_rule(Predicate,false,holds(Predicate,Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds):-
	memberchk('zzfn'(_),Type),
	writeDebug(red,no_functions_on_modus_tollens(holds(Predicate,Arg1,Arg2),Conditions)),
	!,fail.
	
confirm_rule(Predicate,TF,holds(Predicate,Arg1,Arg2), Agent,Context, and(equals(_,_),_), Explaination, F,Type,Depth,Table,NewTable,NewConds):-
	writeDebug(red,no_functions_on_modus_tollens(holds(Predicate,Arg1,Arg2),Conditions)),
	!,fail.
	

confirm_rule(Predicate,TF,holds(Predicate,Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds):-
	memberchk('2nd'(_),Type),
	memberchk('zzskFn'(_,_),[Arg1,Arg2]),
	writeDebug(red,Type:(TF:holds(Predicate,Arg1,Arg2),Conditions)),
	!,fail.
	

confirm_rule(Predicate,false,holds(Predicate,Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type,Depth,Table,[defering_to_gafs(false,Predicate)|Table],Conditions):-
	writeDebug(purple,Type:(not(holds(Predicate,Arg1,Arg2)):modus_tollens:Conditions)),!.

confirm_rule(Predicate,true,holds(Predicate,Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type,Depth,Table,[defering_to_gafs(true,Predicate)|Table],Conditions):-
	writeDebug(blue,Type:(holds(Predicate,Arg1,Arg2):modus_ponens:Conditions,Explaination)),!.


confirm_ground(subclass('Entity',B)):-!,fail.
confirm_ground(subclass(_,foobar)):-!,fail.
confirm_ground(subclass(A,B)):-!,
	deduceTransitiveClosure_PartialOrderingRelation(Context,subclass,A,B,Explaination1).
	
confirm_ground(and(A,B)):-!,
	confirm_ground(A),
	confirm_ground(B).
	
confirm_ground(Conditions):-ground(Conditions),
	writeDebug(green,confirm_ground(Conditions)).
confirm_ground(Conditions):-!,fail,
	writeDebug(pink,nonground(Conditions)),!,fail.



finite_goal(Predicate,Logic,Depth,Table,Call,Agent,Context,Explaination):-
	writeDebug(blue,completed_table(Logic,Call)),!,fail.


client_rulebase_spec(Predicate,Logic,Post, _Agent,Context, Pre, Explaination,Post ,Type):-
	ado_cache_rule(Predicate,Post, Pre, RuleDepth, Logic, Context, _AgentDisjoin, Explaination,Type).


% Backchain



%reorderAnteceedants(_,Best,Best):-ground(Best),!.
       
not_in(Goal,Table):-not(member(Goal,Table)).

exact_memberchk(X,[Y]):-X==Y,!.
exact_memberchk(X,[_|Y]):-exact_memberchk(X,Y),!.

in_rule(Goal,Goal).
in_rule(Goal,and(A,B)):-!,
	(in_rule(Goal,A);in_rule(Goal,B)),!.

not_in_rule(Goal,Rule):-not(in_rule(Goal,Rule)).



	
			%client_rulebase(Predicate,Logic,holds(Predicate,ArgS), Agent,Context, ConditionsO, ExplainationO, F,A).
/*
client_rulebase_spec(Predicate,Logic,holds(Predicate,ArgS), _Agent,Context, ConditionsO, ExplainationO, _,TypeO):-
	ArgS=Arg,SO,
	findall(O,
		((
			client_rulebase(Predicate,Logic,holds(Predicate,ArgS), Agent,Context, Conditions, Explaination, F,A),
			indiviguals(holds(Predicate,ArgS),Conditions,Explaination,A,O)		
		)),Rules),!,
	format('RULES: ~q~n',[Rules]),
	iterate_rulesout(Logic,holds(Predicate,ArgS),Rules,ConditionsO, ExplainationO).
  */

iterate_rulesout(Logic,Goal,[],ConditionsO, ExplainationO):-!,fail.
iterate_rulesout(Logic,Goal,Rules,ConditionsO, ExplainationO):-
	member(rule(Mark,ConditionsOM,ExplainationO),Rules),
	once(make_markings(Logic,Goal,Mark,ConditionsOM,Rules,ConditionsO)).

indiviguals(Goal,Conditions,(surf(Context,Mark) * HOW),_,rule(Mark,Conditions,(surf(Context,Mark) * HOW))).
/*
indiviguals(Goal,Conditions,(surf(_,Mark) * _),i(Mark,N,Each)):-
	indiviguals0(Conditions,ALL,N),!,
	member(Each,ALL).
*/

				 /*
indiviguals0(blah,_,0):-!,fail.
indiviguals0(and(and(and(and(A,B),C),D),E),[A,B,C,D,E],5).
indiviguals0(and(and(and(A,B),C),D),[A,B,C,D],4).
indiviguals0(and(and(A,B),C),[A,B,C],3).
indiviguals0(and(A,B),[A,B],2).
indiviguals0(Each,[Each],1).
				   */
	


:-index(make_markings(0,0,0,1,0,0)).

make_markings(Logic,Goal,Mark,V,Rules,V):-var(V),!.
make_markings(Logic,Goal,Mark,and(A,B),Rules,and(AA,BB)):-!,
	make_markings(Logic,Goal,Mark,A,Rules,AA),
	make_markings(Logic,Goal,Mark,B,Rules,BB),!.
make_markings(Logic,Goal,Mark,or(A,B),Rules,or(AA,BB)):-!,
	make_markings(Logic,Goal,Mark,A,Rules,AA),
	make_markings(Logic,Goal,Mark,B,Rules,BB),!.
make_markings(Logic,Goal,Mark,exists(A,B),Rules,exists(AA,BB)):-!,
	make_markings(Logic,Goal,Mark,A,Rules,AA),
	make_markings(Logic,Goal,Mark,B,Rules,BB),!.
make_markings(Logic,Goal,Mark,forall(A,B),Rules,forall(AA,BB)):-!,
	make_markings(Logic,Goal,Mark,A,Rules,AA),
	make_markings(Logic,Goal,Mark,B,Rules,BB),!.
make_markings(Logic,Goal,Mark,entails(A,B),Rules,entails(AA,BB)):-!,
	make_markings(Logic,Goal,Mark,A,Rules,AA),
	make_markings(Logic,Goal,Mark,B,Rules,BB),!.
make_markings(Logic,Goal,Mark,=>(A,B),Rules,=>(AA,BB)):-!,
	make_markings(Logic,Goal,Mark,A,Rules,AA),
	make_markings(Logic,Goal,Mark,B,Rules,BB),!.
make_markings(Logic,Goal,Mark,<=>(A,B),Rules,<=>(AA,BB)):-!,
	make_markings(Logic,Goal,Mark,A,Rules,AA),
	make_markings(Logic,Goal,Mark,B,Rules,BB),!.
make_markings(Logic,Goal,Mark,Ante,Rules,skip):-Goal==Ante,!.
make_markings(Logic,Goal,Mark,Ante,Rules,gafs(Logic,Ante)):-
	not(not(Goal=Ante)),!.
make_markings(Logic,Goal,Mark,Ante,Rules,delay(Logic,Ante)):-
	not(not(defered_fact(_,Agent,Context,Ante))),!.
make_markings(Logic,Goal,Mark,ConditionsO,Rules,ConditionsO):-!.


/*
iterate_rulesout(Logic,[partof(ConditionsO, ExplainationO)|Rest],partof(ConditionsO, ExplainationO)).
iterate_rulesout(Logic,[_|Rest],ConditionsO, ExplainationO):-iterate_rulesout(Logic,Rest,ConditionsO, ExplainationO).

	member(,Rules),not(ConditionsO=i(_)),
	member(partof(i(Each),ExplainationO),Rules),
	ispartof(Each,)
*/	
	




/*

% ===================================================
% ReflexiveRelation True
% ===================================================
prove_holds(true,Depth,Flags,Predicate,Arg1,[Arg2],Table,Agent,Context,P):-
	not_in('ReflexiveRelation',Table),
	memberchk('ReflexiveRelation',Flags),
	Arg1==Arg2.


% ===================================================
% ReflexiveRelation false
% ===================================================
prove_holds(false,Depth,Flags,Predicate,Arg1,[Arg2],Table,Agent,Context,P):-
	not_in('ReflexiveRelation',Table),
	memberchk('ReflexiveRelation',Flags),
	Arg1==Arg2,!,fail.


% ===================================================
% SymmetricRelation (True/False)
% ===================================================
prove_holds(true,Depth,Flags,Predicate,Arg1,[Arg2],Table,Agent,Context, P * Explaination):-
	not_in('SymmetricRelation',Table),
	memberchk('SymmetricRelation',Flags),
	deduceGoalOProve(Predicate,true,Depth,['SymmetricRelation'|Table],holds(InvPredicate,[Arg2,Arg1]),Agent,Context,Explaination).

	
% ===================================================
% PartialOrderingRelation
% ===================================================
prove_holds(true,Depth,Flags,Predicate,Sub,[Super],Table,Agent,Context,P * Explaination):-
	memberchk('PartialOrderingRelation',Flags),
	not(memberchk('TotalOrderingRelation',Flags)),
	deduceTransitiveClosure_PartialOrderingRelation(Context,Predicate,Sub,Super,Explaination),sendNote(Predicate).
	*/
/*
% ===================================================
% TotalOrderingRelation
% ===================================================
prove_holds(true,Depth,Flags,Predicate,Sub,[Super],Table,Agent,Context, P * Explaination):-
	memberchk('TotalOrderingRelation',Flags),
	deduceTransitiveClosure_TotalOrderingRelation(Context,Predicate,Sub,Super,Explaination),sendNote(Predicate).
*/	
% ===================================================
% TransitiveRelation
% ===================================================
				  /*
prove_holds(true,Depth,Flags,Predicate,Sub,[Super],Table,Agent,Context, P * Explaination):-
	memberchk('TransitiveRelation',Flags),
	not(memberchk('PartialOrderingRelation',Flags)),
	deduceTransitiveClosure_TransitiveRelation(Context,Predicate,Sub,Super,Explaination).	
				       */
% ===================================================
% RelationExtendedToQuantities 
% ===================================================

/*
prove_holds(false,Depth,Flags,Predicate,Sub,[Super],Table,Agent,Context, Explaination):-
	memberchk('RelationExtendedToQuantities',Flags),
        ((
	deduceGoalOProve(instance,true,2,[prove_holds,inverse(Predicate)|Table],instance(Sub,'Quantity'),Agent,Context,_)
	;
	deduceGoalOProve(instance,true,2,[prove_holds,inverse(Predicate)|Table],instance(Super, 'Quantity'),Agent,Context,_)
	)),
	
	((	belief_math(Depth,['RelationExtendedToQuantities'|Table],holds(Predicate,[Sub|Super]),Agent,Context,Explaination),!,fail)
	;
	Explaination=incode(not(Head),'RelationExtendedToQuantities')).
*/   
/*
% ===================================================
% IrreflexiveRelation False
% ===================================================
prove_holds(false,Depth,Flags,Predicate,Arg1,[Arg2],Table,Agent,Context,P * Explaination * bullet(not(holds(Predicate,[Arg1,Arg2])))):-
	not_in('IrreflexiveRelation',Table),
	memberchk('IrreflexiveRelation',Flags),
	Arg1==Arg2,
	Explaination=sfindi((instance(Predicate, 'IrreflexiveRelation')=>
		forall(Arg1, not holds(Predicate, Arg1, Arg2)))).
*/

/*
% ===================================================
% Inverse
% ===================================================
prove_holds(Logic,Depth,Flags,Predicate,Arg1,[Arg2],Table,Agent,Context,P * Explaination):-
	%(memberchk('BinaryRelation',Flags);memberchk('BinaryPredicate',Flags)),
	%not(memberchk('TransitiveRelation',Flags)),
	not_in(inverse(Predicate),Table),
	((
	deduceGoalOProve(inverse,true,2,[prove_holds,inverse(Predicate)|Table],inverse(InvPredicate,Predicate ),Agent,Context,P)
	;
	deduceGoalOProve(inverse,true,2,[prove_holds,inverse(Predicate)|Table],inverse(Predicate, InvPredicate),Agent,Context,P)
	)),
	deduceGoalOProve(InvPredicate,Logic,Depth,[inverse(Predicate),inverse(InvPredicate)|Table],holds(InvPredicate,[Arg2,Arg1]),Agent,Context,Explaination).
		       */

/*
% ===================================================
% True subrelation Child
% ===================================================
prove_holds(true,Depth,Flags,Predicate,Arg1,ArgS,Table,Agent,Context,P * Explaination):- 
	not_in(subrelation(Predicate),Table),
	deduceSurfaceGuarded(subrelation,true,subrelation(  Child ,Predicate  ),Agent,Context,P),
	Child \==  Predicate,
	writeDebug(Child \==  Predicate),
	deduceGoalOProve(Child,true,Depth,[subrelation(Predicate)|Table],holds(Child,Arg1,Arg2),Agent,Context,Explaination).

% ===================================================
% False subrelation Parent
% ===================================================
prove_holds(false,Depth,Flags,Predicate,Arg1,ArgS,Table,Agent,Context,P * Explaination):- 
	not_in(subrelation(Predicate),Table),
	deduceSurfaceGuarded(subrelation,true,subrelation(  Predicate, Child  ),Agent,Context,P),
	Child \==  Predicate,
	writeDebug(Child \==  Predicate),
	deduceGoalOProve(Child,false,Depth,[subrelation(Predicate)|Table],holds(Child,Arg1,Arg2),Agent,Context,Explaination).	
*/
/*
% ===================================================
% Temporal Belief
% ===================================================
prove_holds(Logic,Depth,Flags,Predicate,Arg1,[Arg2|ArgS],Table,Head,Agent,Context,P * Explaination):-
	fail, 
	not_in(domain,Table),
	(deduceGoalOProve(Predicate,true,2,[prove_holds|Table],domain( Predicate, 1 , 'Formula' ),Agent,Context,P),
	getNegationForm(Arg1,NNF),
	writeDebug(getNegationForm(Arg1,NNF)),
	Arg1 \= NNF,
	once((Proto_call=..[Predicate,NNF,Arg2|ArgS])),
	deduceGoalOProve(Predicate,Logic,Depth,Flags,[domain|Table],Proto_call,Agent,Context,Explaination))
	;
	(deduceGoalOProve(Predicate,true,2,[prove_holds|Table],domain( Predicate, 2 , 'Formula' ),Agent,Context,P),
	getNegationForm(Arg2,NNF),
	writeDebug(getNegationForm(Arg2,NNF)),
	Arg2 \= NNF,
	once((Proto_call=..[Predicate,Arg1,NNF|ArgS])),
	deduceGoalOProve(Predicate,Logic,Depth,Flags,[domain|Table],Proto_call,Agent,Context,Explaination)).
*/
 

/*

prove_goal_rtype(holdsDuring,true,Depth,Flags,Table,holdsDuring(TimePosition,Situation),Agent,Context, Explaination):-
	((nonvar(Situation),functor(Situation,Predicate,_));nonvar(TimePosition)), true,
	writeDebug(green,holdsDuring(TimePosition,Situation)),
	once(pos_neg(Situation,NewFact,Logic)),
	deduce_holdsDuring(Predicate,Logic,Depth,Flags,Table,TimePosition,NewFact,Agent,Context,Explaination ),
	writeDebug(green,succeed_holdsDuring(TimePosition,Situation)).


*/

% ==========================================================
% Not Subclass (Specialization)
% ==========================================================
/*
prove_goal_rtype(Predicate,false,Depth,Flags,Table,subclass(Sub,SubSuper),Agent,Context,Explaination * Explaination2):-
	Depth2 is Depth-1,
	deduceGoalOProve(Predicate,true,Depth2,[subclass(Sub,Super)|Table],disjoint(Sub,Super), Agent,Context,Explaination),
	deduceGoalOProve(Predicate,true,Depth2,[subclass(SubSub,Super),disjoint(Sub,Super)|Table],subclass(SubSuper,Super),Agent,Context,Explaination2).
*/  
% ==========================================================
% Is Disjoint
% ==========================================================
/*
prove_goal_rtype(Predicate,Logic,Depth,Flags,Table,disjoint(A,B),Agent,Context,Explaination):-
	deduce_disjoint(Logic,Depth,Flags,Table,A,B,Agent,Context,Explaination).
*/       
/*
deduce_disjoint(true,Depth,Flags,Table,A,B,Agent,Context,Explaination):-
	deduce_disjoint(true,Depth,Flags,Table,A,B,Agent,Context,Explaination):-
	prove_holds(true,Depth,Flags,Predicate,Arg1,[Arg2],Table,Head,Agent,Context,P * Explaination):-
 */       

% ==========================================================
% Single Value (Specialization)
% ==========================================================	

% TODO

% ====================================================	   
% make_all_subclasses(Agent,Context)
% ====================================================
/*

References:

Anderson, A. and Belnap, N., Entailment: The Logic of Relevance and Necessity, Princeton: Princeton University Press vol. 1 (1975), vol. 2 (1992) 
Barcan, R., "A Functional Calculus of First Order Based on Strict Implication," Journal of Symbolic Logic, 11 (1946): 1-16 
Bencivenga, E., "Free Logics," in Gabbay, D., and Guenthner, F. (eds.) Handbook of Philosophical Logic, Dordrecht: D. Reidel (1986): 3.6 
Bonevac, D., Deduction, Palo Alto, California: Mayfield Publishing Company (1987): Part II 
Boolos, G., The Logic of Provability, Cambridge, England: Cambridge University Press (1993) 
Bull, R. and Segerberg, Krister, "Basic Modal Logic," in Gabbay, D., and Guenthner, F. (eds.) Handbook of Philosophical Logic, Dordrecht: D. Reidel (1984): 2.1 
Carnap, R., Meaning and Necessity, Chicago: U. Chicago Press, 1947 
Chellas, Brian, Modal Logic: An Introduction, Cambridge, England: Cambridge University Press (1980) 
Cresswell, M. J., "Incompleteness and the Barcan formula", Journal of Philosophical Logic, 24 (1995): 379-403. 
Cresswell, M. J., "In Defence of the Barcan Formula," Logique et Analyse, 135-136 (1991): 271-282 
Fitting, M. and Mendelsohn, R., First Order Modal Logic, Dordrecht: Kluwer, (1998) 
Gabbay, D., Investigations in Modal and Tense Logics, Dordrecht: D. Reidel (1976) 
Gabbay, D., Temporal Logic: Mathematical Foundations and Computational Aspects, New York: Oxford University Press (1994) 
Garson, James, "Quantification in Modal Logic," in Gabbay, D., and Guenthner, F. (eds.) Handbook of Philosophical Logic, Dordrecht: D. Reidel (1984): 2.5 
Hintikka, J., Knowledge and Belief: An Introduction to the Logic of the Two Notions, Ithaca, N. Y.: Cornell University Press (1962) 
Hilpinen, R., Deontic Logic: Introductory and Systematic Readings, Dordrecht: D. Reidel (1971) 
Hughes, G. and Cresswell, M., An Introduction to Modal Logic, London: Methuen (1968) 
Hughes, G. and Cresswell, M., A Companion to Modal Logic, London: Methuen (1984) 
Hughes, G. and Cresswell, M., A New Introduction to Modal Logic, London: Routledge (1996) 
Kripke, Saul, "Semantical Considerations on Modal Logic," Acta Philosophica Fennica, 16, (1963): 83-94 
Konyndik, K., Introductory Modal Logic, Notre Dame: University of Notre Dame Press (1986) 
Kvart, I., A Theory of Counterfactuals, Indianapolis: Hackett Publishing Company (1986) 
Lemmon, E. and Scott, D., An Introduction to Modal Logic, Oxford: Blackwell (1977) 
Lewis, C.I. and Langford, C. H., Symbolic Logic, New York: Dover Publications, 1959 (1932) 
Lewis, D., Counterfactuals, Cambridge, Massachusetts: Harvard University Press (1973) 
Linsky, B. and Zalta, E., "In Defense of the Simplest Quantified Modal Logic," Philosophical Perspectives, 8, (Logic and Language), (1994): 431-458 
Prior, A. N., Time and Modality, Oxford: Clarendon Press (1957) 
Prior, A. N., Past, Present and Future, Oxford: Clarendon Press (1967) 
Quine, W. V. O., "Reference and Modality", in From a Logical Point of View, Cambridge, MA: Harvard University Press, 1953: 139-159 
Rescher, N, and Urquhart, A., Temporal Logic, New York: Springer Verlag (1971) 
Sahlqvist, H., "Completeness and Correspondence in First and Second Order Semantics for Modal Logic," in Kanger, S. (ed.) Proceedings of the Third Scandanavian Logic Symposium, Amsterdam: North Holland (1975): 110-143 
Van Benthem, J. F., The Logic of Time, Dordrecht: D. Reidel (1982) 
Zeman, J., Modal Logic, The Lewis-Modal Systems, Oxford: Oxford University Press (1973) 

*/


