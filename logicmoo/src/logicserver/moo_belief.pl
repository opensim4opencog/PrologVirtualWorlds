% ==========================================
% BELIEF
% .+user nick #channel Ident 
% ==========================================

:-include('moo_header.pl').
:-dynamic(complete_goal/1).
:-multifile(expireOptimizationsInKB/3).
:-ensure_loaded(library(occurs)).
       


agentBelief(FmlInOpen,Literal,VarsRequested,Ctx,KB,User,PA, VarsRequested,found(PA),answer(true:PA)):-
	resetTableFlags, 
	flag('$UAnswers',_,0),
	writeDebug(green,'Stage 3 - Positive ':FmlInOpen ),  
	mooBeliefCallTopP(VarsRequested,Literal),
	flag('$UAnswers',UA,UA+1),
	flag('$UAnswers',PA,PA).

agentBelief(FmlInOpen,NLiteral,VarsRequested,Ctx,KB,User,PA, VarsRequested,found(NA),done(false:NA)):-%trace,
	flag('$UAnswers',PA,PA),PA<1,
	(NLiteral=..[TopFunctor|Args],(atom_concat('~',FN,TopFunctor);atom_concat('~',TopFunctor,FN))),!,Literal=..[FN|Args],
	writeDebug(red,'Stage 4 - Negative ':not(FmlInOpen)),
	mooBeliefCallTopN(VarsRequested,Literal),
	flag('$UAnswers',UA,UA+1),
	flag('$UAnswers',NA,NA).


				

% =========================================================
% Body Clause Connectives
% =========================================================
mooBeliefCallTopP(VarsRequested,Literal):-
	%call_with_depth_limit(
	mooBeliefLiteral(5,[VarsRequested^[]],Literal).
	%60000,N),
	%N \= depth_limit_exceeded,
	%writeq(N),nl.

mooBeliefCallTopN(VarsRequested,Literal):-
	call_with_depth_limit(mooBeliefLiteral(2,[VarsRequested^[]],Literal),60000,N),
	N \= depth_limit_exceeded,
	writeq(N),nl.
	


:-index(mooBeliefLiteral(0,1,1)).
	
% =========================================================
% Literals Are Called Here
% =========================================================

% ---------------------------------------------------------
% BuiltIns
% ---------------------------------------------------------

mooBeliefLiteral(Depth,PreviousVarSeek,'$fail'(_)):-!,fail.

mooBeliefLiteral(Depth,PreviousVarSeek,equal(CPos1,CPos2)):-!,
	unify_with_occurs_check(CPos1,CPos2).
mooBeliefLiteral(Depth,PreviousVarSeek,'~equal'(CPos1,CPos2)):-!,
	not(mooBeliefLiteral(Depth,PreviousVarSeek,equal(CPos1,CPos2))).

mooBeliefLiteral(Depth,PreviousVarSeek,'~instance'(O,'$Class'(Base))):-!,
				ground(O),O=..[T,A,ToB],not((member(Base,ToB));atom_concat('$',Base,T)),!.

mooBeliefLiteral(Depth,PreviousVarSeek,'$existential'(X,Y,Formula)):-!,ignore(unify_with_occurs_check(X,Y)),!.

mooBeliefLiteral(Depth,SubVarSeek,Goal):-functor(Goal,'~holds',_),!,fail.
				   
%mooBeliefLiteral(Depth,SubVarSeek,instance(O,'$Class'(C)),TopFunctor,FreeVs):-
%	ground(O),writeq(O),nl,arg(2,O,L),!,miclass(L,C).

miclass(L,C):-last(C,L),!. miclass(L,C):-member(C,L).
	
     	
% ---------------------------------------------------------
% Ensure only ONE-CALL however this means that a table 'should be prebuilt'
%    And is one First Call to Predicate Hash
% ---------------------------------------------------------

/*
TopFunctor(S) -> PrototypeRef(S) -> complete(Depth) 
				 -> Anwser(S)
				 -> Derived: sub(PrototypeRef(S))

A Derived PrototypeRef:
		  When we unnumbervars PrototypeRef
		  Unify it to the 'Rules' ProtoType Model since Every Rule has a descriptor to what it can prove (VarIOs)
*/


mooBeliefLiteral(Depth,SubVarSeek,Goal):-mooCache(Goal,_,KB,Ctx,TN). 

mooBeliefLiteral(Depth,SubVarSeek,subclass('$Class'(S),'$Class'(C))):-!,
	mooCache(A, B, deduceSingleSubclassPathList(S, 'Entity', List)),
	writeq(List),nl,
	member(C,List).

mooBeliefLiteral(Depth,SubVarSeek,domain('$Relation'(R,_),'$Quantity'(N,_),'$Class'(C))):-!,
	mooCache(KB, A, domain_vector(R, N, List,_)),
	nth1(N,List,C).
	
                       
mooBeliefLiteral(Depth,SubVarSeek,Goal):-	
	functor(Goal,TopFunctor,Arity),
	mooBeliefLiteral(TopFunctor,Arity,Depth,SubVarSeek,Goal).
	

mooBeliefLiteral(TopFunctor,Arity,Depth,SubVarSeek,Goal):-noBackchainFunctors(TopFunctor),!,
	noBackchainFunctorsCall(TopFunctor,Arity,Depth,SubVarSeek,Goal).
	
noBackchainFunctorsCall(TopFunctor,Arity,Depth,SubVarSeek,Goal):-!,fail. %mooCache(Goal,_,KB,Ctx,TN). 
%mooCache(Goal,Ante,ExplainationID:KRVars:KR,KB,Ctx,TN).	
	
% Blocked preds including all .*On (just backchaining)
noBackchainFunctors(disjointDecomposition). noBackchainFunctors(domain). 
noBackchainFunctors(subclass). 
noBackchainFunctors(copy). 
noBackchainFunctors(version). 
noBackchainFunctors(subrelation). 
noBackchainFunctors(function).
noBackchainFunctors(holds).
noBackchainFunctors(A):-atom(A),(atom_concat(_,'On',A);atom_concat('~',_,A)).



mooBeliefLiteral(TopFunctor,Arity,Depth,SubVarSeek,Goal):-
	free_variables(Goal,FreeVs),
	ensureProtoRef(TopFunctor,GoalPrototype,Goal,PrototypeRef,NumVars),
	mooBeliefCompleteAndRetunTable(PrototypeRef,Depth,SubVarSeek,
		TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype).

% =========================================================
% Find or Define the PrototypeRef for GoalPrototype
% =========================================================

ensureProtoRef(TopFunctor,GoalPrototype,Goal,PrototypeRef,NumVars):-
	% ----------------------------------------------------------------------------------------
	% Condition: GoalPrototype Supplied
	% ----------------------------------------------------------------------------------------
	(nonvar(GoalPrototype) ->
	% ----------------------------------------------------------------------------------------
	% Action: Confirm Access Structures 
	% ----------------------------------------------------------------------------------------
	recorded(TopFunctor,GoalPrototype:NumVars,PrototypeRef) ;
	% ----------------------------------------------------------------------------------------
	% Exception: GoalPrototype not Supplied
	% ExAction: Create GoalPrototype and Next Create PrototypeRef
	% ---------------------------------------------------------------------------------------- 
	(       copy_term(Goal,GoalPrototype),
		numbervars(GoalPrototype,'$VAR',1,NumVars),
		% ----------------------------------------------------------------------------------------
		% Condition: Ref Already Known
		% ----------------------------------------------------------------------------------------
		(recorded(TopFunctor,GoalPrototype:NumVars,PrototypeRef) -> 
		% ----------------------------------------------------------------------------------------
		% Action: Return Known
		% ---------------------------------------------------------------------------------------- 
		true; 
		% ---------------------------------------------------------------------------------------- 
		% Exception: GoalPrototype Not Identical To Previous Request (Ref Not Known)
		% ExAction: Create it, Find SubLinks and Find Parent Links
		% ---------------------------------------------------------------------------------------- 
		 % make a PrototypeRef For Goal
		 (      writeDebug(creating(GoalPrototype)),
			recorda(TopFunctor,GoalPrototype:NumVars,PrototypeRef),
			% Discover stable Model
			% trace,
			stableModel(TopFunctor,PrototypeRef,Goal,GoalPrototype))))),!.
	

% Model Mainted Already
stableModel(TopFunctor,PrototypeRef,Pattern,GoalPrototype):-
	recorded(PrototypeRef,complete(0)),!.



% Discover Family Rules
stableModel(TopFunctor,PrototypeRef,Pattern,GoalPrototype):-
	mooCache(Pattern,A,ExplainationID:KRVars:KR,KB,Ctx,TN),
	once(ensureProtoRef(TopFunctor,_,Pattern,RulePrototypeRef,_)),
	not(RulePrototypeRef=PrototypeRef),
	writeDebug(linking(Pattern,GoalPrototype)),
	(not(Pattern=GoalPrototype) -> 
		stableLink(PrototypeRef,RulePrototypeRef);
		stableLink(RulePrototypeRef,PrototypeRef)),
	fail.
	
% Discover Previously Built Tables?
% Set PrototypeRef Discovery Complete (Depth 0 = complete)
stableModel(TopFunctor,PrototypeRef,Pattern,GoalPrototype):-recorda(PrototypeRef,complete(0)),!.
			
stableLink(SuperPrototypeRef,SubPrototypeRef):-
	(recorded(SuperPrototypeRef,subRef(SubPrototypeRef));recorda(SuperPrototypeRef,subRef(SubPrototypeRef))),!,
	(recorded(SubPrototypeRef,superRef(SuperPrototypeRef));recorda(SubPrototypeRef,superRef(SuperPrototypeRef))),!	.


% =========================================================
% Complete And Retun Table
% =========================================================


mooBeliefCompleteAndRetunTable(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-
	logOnFailure(mooBeliefCompleteTableLevel(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype)),!,
	mooBeliefReturnTable(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype).


% =========================================================
% Return Tables of More Specific Clauses *FIRST*
% Then Table of PrototypeRef
% =========================================================
mooBeliefReturnTable(PrototypeRef,0,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-!,fail.

% ----------------------------------------------------------------------------------------
% Showall In PrototypeRef 
% ----------------------------------------------------------------------------------------
mooBeliefReturnTable(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-
	recorded(PrototypeRef,Goal),
	writeDebug(blue,Goal).

% ----------------------------------------------------------------------------------------
% Iterate And Table More Specific Clauses
% ----------------------------------------------------------------------------------------
mooBeliefReturnTable(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-
	recorded(PrototypeRef,subRef(SubPrototypeRef)),
	recorded(SubPrototypeRef,Goal),
	writeDebug(blue,Goal).
	


% =========================================================
% Record Tables
% =========================================================
mooBeliefRecordBindings(PrototypeRef,DepthToExplore,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-
	(not(recorded(PrototypeRef,Goal)) ->
		(recorda(PrototypeRef,Goal),writeDebug(green,Goal));
		writeDebug(pink,Goal)).

/*
% ----------------------------------------------------------------------------------------
% Iterate And *NOT* Table More General Tables
% ----------------------------------------------------------------------------------------
mooBeliefReturnTable(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-
	recorded(PrototypeRef,superRef(SuperPrototypeRef)),
	writeDebug(superOf(Goal)),
	mooBeliefReturnTable(SuperPrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype).
*/
	
% =========================================================
% Complete Tables To A Depth
% =========================================================

% ----------------------------------------------------------------------------------------
% Level '0' is always considered complete (Exit)
% ----------------------------------------------------------------------------------------
mooBeliefCompleteTableLevel(PrototypeRef,(0),SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-!.
	
% ----------------------------------------------------------------------------------------
% Highest Level Completed >= Level Requested  (Exit)
% ----------------------------------------------------------------------------------------
mooBeliefCompleteTableLevel(PrototypeRef,DepthToExplore,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-
	% ----------------------------------------------------------------------------------------
	% Condition: Depth is complete therefore All lowers depths *should be*
	% ----------------------------------------------------------------------------------------
	recorded(PrototypeRef,complete(CompletedTableDepth)),
	CompletedTableDepth >= DepthToExplore,
	% ----------------------------------------------------------------------------------------
	% Action: Exit Deterministically
	% ----------------------------------------------------------------------------------------
	!.

% ----------------------------------------------------------------------------------------
% Condition: Depth is not complete therefore All lowers must be complete first before we can proceed
% ----------------------------------------------------------------------------------------
mooBeliefCompleteTableLevel(PrototypeRef,DepthToExplore,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-
	% ----------------------------------------------------------------------------------------
	% Find The greatest completed Depth
	% ----------------------------------------------------------------------------------------
	recorded(PrototypeRef,complete(CompletedTableDepth)),
	% ----------------------------------------------------------------------------------------
	% Get Next Depth
	% ----------------------------------------------------------------------------------------
	NextDepth is CompletedTableDepth + 1,
	% ----------------------------------------------------------------------------------------
	% Build Table for this Next Depth
	% ----------------------------------------------------------------------------------------
	mooBeliefBuildTableLevel(PrototypeRef,NextDepth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype),!,
	% ----------------------------------------------------------------------------------------
	% Try Again to Table to 'DepthToExplore'
	% ----------------------------------------------------------------------------------------
	mooBeliefCompleteTableLevel(PrototypeRef,DepthToExplore,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype).

% =========================================================
% Build Tables At A Specific Depth
% =========================================================


mooBeliefBuildTableLevel(PrototypeRef,ThisNextDepth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-
	% ----------------------------------------------------------------------------------------
	% trace to debugger
	% ----------------------------------------------------------------------------------------
	writeDebug(mooBeliefBuildTableLevel(ThisNextDepth,Goal,GoalPrototype,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity)),
	% ----------------------------------------------------------------------------------------
	% Not special predicate blocked for backchaining
	% ----------------------------------------------------------------------------------------
	not(noBackchainFunctors(TopFunctor)),  % could be moved higher up for speed increase?
	% ----------------------------------------------------------------------------------------
	% Not Free Holds Arg1 ( % could enum plug in Relations if it was varibiable? )
	% ----------------------------------------------------------------------------------------
	(TopFunctor='holds' -> (arg(1,Goal,V),ground(V)) ; true), % SHOULD STRIP HOLDS?!?
	% ----------------------------------------------------------------------------------------
	findall(SubPrototypeGoal,
	((
	% ----------------------------------------------------------------------------------------
	% Action: Search for More specific SubPrototypeRef links *first*
	% ----------------------------------------------------------------------------------------
	recorded(PrototypeRef,subRef(SubPrototypeRef)),
	recorded(TopFunctor,SubPrototypeGoal:SubNumVars,SubPrototypeRef),
	writeDebug(subTabling(ThisNextDepth:SubPrototypeGoal)),
	unnumbervars(SubPrototypeGoal,SubGoal),
	% ----------------------------------------------------------------------------------------
	% Action: Bring SubPrototypeRef up to Date (for ThisNextDepth)
	% ----------------------------------------------------------------------------------------
	mooBeliefCompleteTableLevel(SubPrototypeRef,ThisNextDepth,SubVarSeek_,TopFunctor,SubNumVars,_,Arity,SubGoal,SubPrototypeGoal)
	% ----------------------------------------------------------------------------------------
	% Action: Force Next
	% ----------------------------------------------------------------------------------------
	 )),SolvedSubPrototypes),
	 writeDebug(SolvedSubPrototypes),
	% ----------------------------------------------------------------------------------------
	% Finally BELIEF!
	% ----------------------------------------------------------------------------------------
	% Not Too many Free Variables Before rule test
	% ----------------------------------------------------------------------------------------
	% NumVars<6, % could be moved higher up for speed increase? 
	% ----------------------------------------------------------------------
	% Look in Rulebase:
	%      Should *only* 
	%       	less general protoypes,
	%		more general prototypes,
	%		or exact protoypes 
	%		be used?
	% ----------------------------------------------------------------------
	% Lower Depth for calling anteceedants
	% ----------------------------------------------------------------------
	NextLowerDepth is ThisNextDepth -1,
	% ----------------------------------------------------------------------
	mooCache(Goal,Ante,ExplainationID:KRVars:KR,KB,Ctx,TN), 
	% ----------------------------------------------------------------------
	% Not already Figured from sub prototypes
	% ----------------------------------------------------------------------
	not(memberchk(Goal,SolvedSubPrototypes)),
	% ----------------------------------------------------------------------
	% Find survied Heads vars 
	% ----------------------------------------------------------------------
	free_variables(Goal,NewFreeVs),
	% ----------------------------------------------------------------------
	% See if variables are too deeply used 
	% ----------------------------------------------------------------------
	deepenCheck(4,FreeVs,Lits), % Note: NextLowerDepth is used but may not be appropriate
	% ----------------------------------------------------------------------
	% If they survived the deepen check, Deepen them. ( 'Lits' now holds their aliases. )
	% ----------------------------------------------------------------------
	deepenEach(Lits),
	% ----------------------------------------------------------------------
	% Invoke Rule that upon deterministic (good or bad) the 'Lits' should have been undeepened.
	% ----------------------------------------------------------------------
	mooBeliefLiteralTryLitVars(PrototypeRef,NextLowerDepth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
		Ante,ExplainationID:KRVars:KR,KB,Ctx,TN,Lits),
	% ----------------------------------------------------------------------
	% Fail forces more solutions to be found
	% ----------------------------------------------------------------------
	fail.
	 
mooBeliefBuildTableLevel(PrototypeRef,ThisNextDepth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype):-
	% ----------------------------------------------------------------------
	% Give Deterministic Exit to build
	% ----------------------------------------------------------------------
	recorda(PrototypeRef,complete(ThisNextDepth)),!.


% Deterministically Find 'Body Belief Mechanism' and call it non deterministicly
mooBeliefLiteralTryLitVars(PrototypeRef,DepthToExplore,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	Ante,ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-
	mooBeliefLiteralTryBodyMechanism(PrototypeRef,DepthToExplore,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,Ante,ExplainationID:KRVars:KR,KB,Ctx,TN,Lits).     
	
% fullfills obligation above: " 'Lits' should have been undeepened"
mooBeliefLiteralTryLitVars(PrototypeRef,DepthToExplore,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	_Ante,ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-undeepenEach(Lits),!,fail.

% =========================================================
% Body Belief Mechanisms
% =========================================================
:-index(mooBeliefLiteralTryBodyMechanism(0,0,0,0,0,0,0,0,1,0,0,0,0,0)).
	
% ----------------------------------------------------------------------
% DEBUGGING HACK BLOCK ALL ~HEADS  (Negated Goal Literals)
% ----------------------------------------------------------------------
mooBeliefLiteralTryBodyMechanism(PrototypeRef,Depth,PreviousVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	Ante,ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-
	% Write Ante to debugger
	writeDebug(Ante),
	% No negs
	functor(Ante,AF,_),
	atom_concat('f~',_,AF),!,fail.


% ----------------------------------------------------------------------
% Condition: The body is constructed exclusvely from Head Varibles (HVs)
% Action: Find all unique sets of HVs
% Exception: Except when Head is for a Negated Literal
% ExAction: Definite failure (hack for now)
% ----------------------------------------------------------------------
mooBeliefLiteralTryBodyMechanism(PrototypeRef,Depth,PreviousVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	't~Head'(BodyGoal,HeadV,BodyV,HV),ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-
	buildVarSeek(HV^[],PreviousVarSeek,BodyVarSeek,BodyGoal,NewBodyGoal),!,
	mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,BodyGoal).

% ----------------------------------------------------------------------
% Condition: The body is not related to head in any way
% Action: Find one UB set that exists for the Head
% Exception: Except when Head is for a Negated Literal
% ExAction: Definite failure (hack for now)
% ----------------------------------------------------------------------
mooBeliefLiteralTryBodyMechanism(PrototypeRef,Depth,PreviousVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	't~Univb'(BodyGoal,HeadV,BodyV,UB),ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-!,
	buildVarSeek([]^UB,PreviousVarSeek,BodyVarSeek,BodyGoal,NewBodyGoal),!,
	mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,BodyGoal),!.

	
% ----------------------------------------------------------------------
% Condition: Head Vars (HVs), but all body exclusive (UBs) are disjoint from each other
% Action: Find only one ground UB set that exists for each unique HV set
% Exception: Except when Head is for a Negated Literal
% ExAction: Definite failure (hack for now)
% ----------------------------------------------------------------------

mooBeliefLiteralTryBodyMechanism(PrototypeRef,Depth,PreviousVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	't~HeadUnivb'(BodyGoal,HeadV,BodyV,HV,UB),ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-
	arg(1,BodyGoal,G),%arg(1,G,P),
	arg(1,G,[_|_]),!,fail.

mooBeliefLiteralTryBodyMechanism(PrototypeRef,Depth,PreviousVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	't~HeadUnivb'(BodyGoal,HeadV,BodyV,HV,[UB]),ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-!,
	writeDebug(red,bagof(UB,BodyGoal,HV)),!,
	bagof(UB,
		mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,
			TopFunctor,NumVars,FreeVs,Arity,Goal,
			GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,BodyGoal),Ignored).

mooBeliefLiteralTryBodyMechanism(PrototypeRef,Depth,PreviousVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	't~HeadUnivb'(BodyGoal,HeadV,BodyV,HV,UB),ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-!,
	buildVarSeek(HV^UB,PreviousVarSeek,BodyVarSeek,BodyGoal,NewBodyGoal),!,
	mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,BodyGoal).


% ----------------------------------------------------------------------
% Condition: Head Vars (HVs), and all body connected (BCs) no disjoint Body variables
% Action: Find each unique HV set with one ground BC set
% Exception: Except when Head is for a Negated Literal
% ExAction: Definite failure (hack for now)
% ----------------------------------------------------------------------
mooBeliefLiteralTryBodyMechanism(PrototypeRef,Depth,PreviousVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	't~HeadBodyc'(BodyGoal,HeadV,BodyV,HV,BC),ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-!,
	buildVarSeek(HV^BC,PreviousVarSeek,BodyVarSeek,BodyGoal,NewBodyGoal),!,
	mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,BodyGoal).

% ----------------------------------------------------------------------
% Condition: Head Vars (HVs), and all body connected (BCs) and some disjoint Body variables (UBs)
% Action: Find each unique HV set with one ground BC set and ignore UBs
% Exception: Except when Head is for a Negated Literal
% ExAction: Definite failure (hack for now)
% ----------------------------------------------------------------------
mooBeliefLiteralTryBodyMechanism(PrototypeRef,Depth,PreviousVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	't~HeadBodycUnivb'(BodyGoal,HeadV,BodyV,HV,BC,UB),ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-!,
	buildVarSeek(HV^BC^UB,PreviousVarSeek,BodyVarSeek,BodyGoal,NewBodyGoal),!,
	mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,BodyGoal).
	
% Sanity Check
mooBeliefLiteralTryBodyMechanism(PrototypeRef,Depth,PreviousVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,
	Unk,ExplainationID:KRVars:KR,KB,Ctx,TN,Lits):-
	writeDebug(unknownMechanism:mooBeliefLiteralTryBodyMechanism(Unk)),!,fail.


% =========================================================
% Call Body Conjunctions
% =========================================================
:-index(mooBeliefCallBodyJunctions(0,0,0,0,0,0,0,0,0,0,0,0,0,0,1)).

% First/Then
mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,
	(CPos1,CPos2)):-!,
	mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,CPos1),
	mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,CPos2).

% disjoint eigther/or
mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,
	or(CPos1,CPos2)):-!,
	(mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,CPos1),!);
	(mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,CPos2),!).

% clean of further junctions
mooBeliefCallBodyJunctions(HeadV,BodyV,PrototypeRef,Depth,BodyVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,ExplainationID,TN,
	CPos):-%trace,
	mooBeliefPropositionMechanism(HeadV,BodyV,PrototypeRef,Depth,ClauseVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,CPos).


% =========================================================
% Propostional Control Mechanisms
% =========================================================
:-index(mooBeliefPropositionMechanism(0,0,0,0,0,0,0,0,0,0,0,0,0,1)).

/* built as
	
	ConditionalClass=..[Sign,Conditional,HeadV,BodyVars,
		CUniversalBody,CBodyOnlyConected,CHeadVSingleInBody,CSplitHeadVar].
 */


% Only Universal Body Vars in this Proposition
mooBeliefPropositionMechanism(HeadV,BodyV,PrototypeRef,Depth,ClauseVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,
	't~'(LiteralGoal,CUniversalBody,[],[],[])):-!,
	once(mooBeliefCallProposition(PrototypeRef,Depth,ClauseVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,LiteralGoal)).


% Only Body Vars in this Proposition
mooBeliefPropositionMechanism(HeadV,BodyV,PrototypeRef,Depth,ClauseVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,
	't~'(LiteralGoal,CUniversalBody,CBodyOnlyConected,CHeadVSingleInBody,CSplitHeadVar)):-!,
	selectMech(CUniversalBody,CBodyOnlyConected,CHeadVSingleInBody,CSplitHeadVar,HeadV,BodyV,LiteralGoal,ClauseVarSeek,NewLiteralGoal),!,
	mooBeliefCallProposition(PrototypeRef,Depth,ClauseVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,NewLiteralGoal).

selectMech(  []  ,  []  ,  []  ,  []  ,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,'$once'(LiteralGoal)).
	
selectMech(  []  ,  []  ,  []  ,[CSplitHeadVar],
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,if_then('$oneGround'([CSplitHeadVar|HeadV]),LiteralGoal)).
	
selectMech(  []  ,  []  ,  []  ,CSplitHeadVar,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,if_then('$oneGround'(CSplitHeadVar),LiteralGoal)).


selectMech(  []  ,  []  ,CHeadVSingleInBody,  []  ,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
	
selectMech(  []  ,  []  ,CHeadVSingleInBody,CSplitHeadVar,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(  []  ,CBodyOnlyConected,  []  ,  []  ,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(  []  ,CBodyOnlyConected,  []  ,CSplitHeadVar,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(  []  ,CBodyOnlyConected,CHeadVSingleInBody,  []  ,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(  []  ,CBodyOnlyConected,CHeadVSingleInBody,CSplitHeadVar,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(CUniversalBody,  []  ,  []  ,  []  ,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(CUniversalBody,  []  ,  []  ,CSplitHeadVar,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(CUniversalBody,  []  ,CHeadVSingleInBody,  []  ,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(CUniversalBody,  []  ,CHeadVSingleInBody,CSplitHeadVar,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(CUniversalBody,CBodyOnlyConected,  []  ,  []  ,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(CUniversalBody,CBodyOnlyConected,  []  ,CSplitHeadVar,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(CUniversalBody,CBodyOnlyConected,CHeadVSingleInBody,  []  ,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).
selectMech(CUniversalBody,CBodyOnlyConected,CHeadVSingleInBody,CSplitHeadVar,
	HeadV,BodyV,LiteralGoal,ClauseVarSeek,LiteralGoal).


% Only Body Vars in this Proposition
mooBeliefPropositionMechanism(HeadV,BodyV,PrototypeRef,Depth,ClauseVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,
	't~'(LiteralGoal,CUniversalBody,CBodyOnlyConected,[],[])):-!,
	oneGround(HeadV),
	mooBeliefCallProposition(PrototypeRef,Depth,ClauseVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,LiteralGoal).

mooBeliefPropositionMechanism(HeadV,BodyV,PrototypeRef,Depth,ClauseVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Lits,
	Ante):-	trace,
	writeDebug(unkown:HeadV:BodyV:Ante),fail.

oneGround([]).
oneGround([_]).
oneGround([A,B]):-!,ground(A);ground(B).
oneGround([A,_,B]):-ground(A);ground(B).
oneGround([_,A,_|B]):-!,ground(A);ground(B).


% =========================================================
% Call An Anteceedant
% 	Loops back into starting a new table 
% =========================================================

mooBeliefCallProposition(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,
	'$oneGround'(Vars)):-!,oneGround(Vars),!.

mooBeliefCallProposition(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,
	'$once'(Goal)):-
	mooBeliefCallProposition(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,
	Goal),!.

mooBeliefCallProposition(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,
	'$findAll'(Vs,Goal)):-
	findall(Vs,mooBeliefCallProposition(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,
	Goal),VsU),sort(VsU,VsS),!,member(Vs,VsS).
	
mooBeliefCallProposition(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,
	if_then(Condition,LiteralGoal)):-
	mooBeliefCallProposition(PrototypeRef,Depth,SubVarSeek,
		TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,Condition),!,
	mooBeliefCallProposition(PrototypeRef,Depth,SubVarSeek,
		TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,LiteralGoal).

mooBeliefCallProposition(PrototypeRef,Depth,SubVarSeek,TopFunctor,NumVars,FreeVs,Arity,Goal,GoalPrototype,KB,Ctx,NewLiteralGoal):-
	mooBeliefLiteral(Depth,SubVarSeek,NewLiteralGoal).

% =========================================================
% Var Seeking Utility Can manipulate A Goal in such a way that only productive vars ar sought after
% It also rebuilds the seekage afterwards
% =========================================================
buildVarSeek(_,PreviousVarSeek,PreviousVarSeek,Goal,Goal):-!.
%trace,!.

willFindSeek(_,PreviousVarSeek,PreviousVarSeek,Goal,Goal):-!.

% =========================================================
% Variable iterative deepening Utility (globally shared between all tables)
% =========================================================
deepenCheck(Max,[],[]):-!.
deepenCheck(Max,[V|Vs],[VN|VNs]):-
	deepenCheckV(Max,V,VN),!,
	deepenCheck(Max,Vs,VNs).
	
deepenCheckV(Max,V,Vn):-
	term_to_atom(V,Vn),
	flag(Vn,D,D), D < Max.
		
deepenEach([]):-!.
deepenEach([Vn|Vs]):-flag(Vn,D,D+1),deepenEach(Vs),!.

undeepenEach([]):-!.
undeepenEach([Vn|Vs]):-flag(Vn,D,D-1),undeepenEach(Vs),!.









/*

:-index(mooBeliefLiteral(0,0,1,1,1,0,0,0,1)).

mooBeliefLiteral(Depth,PreviousVarSeek,Term,Major,'t~',Rest,Body,GoalTemplate,More):-!, 
	Depth>0,
	free_variables(PreviousVarSeek,UVarSeek),
	free_variables(More,FreeVarsBody),
	set_partition(UVarSeek,FreeVarsBody,Collected,NewInBody,Carried),
	writeq(('<font color=green>':Depth,carriedN:Carried,newInBodyN:NewInBody,collectedN:Collected:'</font>')),nl,!,
	deepenCheck(4,Carried,LitCarried),!,
	deepenEach(LitCarried),!,
	mooBeliefCallVD(LitCarried,Depth,[FreeVarsBody|UVarSeek],Goal,TopFunctor,Arity,Two,Rest,Body,NewInBody,Carried).


mooBeliefCallVD(LitCarried,Depth,PreviousVarSeek,Goal,TopFunctor,Arity,Two,Rest,Body,NewInBody,Carried):-
	mooBeliefCallV(Depth,PreviousVarSeek,Goal,TopFunctor,Arity,Two,Rest,Body,NewInBody,Carried).
mooBeliefCallVD(LitCarried,Depth,PreviousVarSeek,Goal,TopFunctor,Arity,Two,Rest,Body,NewInBody,Carried):-
	undeepenEach(LitCarried),!.
	
% Some Carried and new in body (fail)
mooBeliefCallV(Depth,PreviousVarSeek,Goal,TopFunctor,Arity,Two,Rest,Body,[_|_],[_|_]):-fail,
	Depth2 is Depth-2,
	mooBeliefLiteral(Depth2,
		PreviousVarSeek,Body).

% None Carried no new in body (Expansion)
mooBeliefCallV(Depth,PreviousVarSeek,Goal,TopFunctor,Arity,Two,Rest,Body,[],[]):-!,
	Depth2 is Depth+1,
	mooBeliefLiteral(Depth2,
		PreviousVarSeek,Body).

% Some Carried no new in body (Shorten Depth)
mooBeliefCallV(Depth,PreviousVarSeek,Goal,TopFunctor,Arity,Two,Rest,Body,[],[_|_]):-
	Depth2 is Depth-1,
	mooBeliefLiteral(Depth2,
		PreviousVarSeek,Body).

% None Carried new in body (Shorten Depth)
mooBeliefCallV(Depth,PreviousVarSeek,Goal,TopFunctor,Arity,Two,Rest,Body,[_|_],[]):-!,
	Depth2 is Depth-1,
	mooBeliefLiteral(Depth2,
		PreviousVarSeek,Body).


mooBeliefLiteral(Depth,PreviousVarSeek,Term,Major,'f~',Rest,Body,GoalTemplate,More):-!,fail,
	Depth>0,
	free_variables(PF,UVarSeek),free_variables(Body,FreeVarsBody),
	set_partition(UVarSeek,FreeVarsBody,Collected,NewInBody,Carried),
	writeq(('<font color=red>':Depth,carriedN:Carried,newInBodyN:NewInBody,collectedN:Collected:'</font>')),nl,!,
	deepenCheckW(3,Carried,LitCarried),!,
	deepenEach(LitCarried),!,
	mooBeliefCallVD(LitCarried,Depth,[FreeVarsBody,PF|OVarSeek],Goal,TopFunctor,Arity,Two,Rest,Body,Collected,NewInBody,Carried).

mooBeliefLiteral(Depth,_,'$grInstance'(Term),'$grInstance',_,Rest,Body,GoalTemplate,More):-!,
		mooBeliefLiteral(Depth,PreviousVarSeek,instance(Term,_)).

mooBeliefLiteral(Depth,_,'$grDomain'(P,N,Goal),'$grDomain',_,Rest,Body,GoalTemplate,More):-!,
		mooCache(domain(P,N,Goal),_,KB,Ctx,TN).


mooBeliefLiteral(Depth,PreviousVarSeek,Goal,'Z_Uvar_',Two,Rest,Body,SingleUVar,[]):-!,
	once((mooBeliefLiteral(Depth,PreviousVarSeek,Body),tground(SingleUVar))).
	
mooBeliefLiteral(Depth,PreviousVarSeek,Goal,'Z_Uvar_bodyConnectedVar_',Two,Rest,Body,SingleUVar,[BodyConnectedVar]):-!,
	once((mooBeliefLiteral(Depth,PreviousVarSeek,Body),tground(SingleUVar))).

mooBeliefLiteral(Depth,PreviousVarSeek,Goal,'Z_Uvar_singleHeadVar_',Two,Rest,Body,SingleUVar,[SingleHeadVar]):-!,
	once((mooBeliefLiteral(Depth,PreviousVarSeek,Body),tground(SingleUVar))).

mooBeliefLiteral(Depth,PreviousVarSeek,Goal,'Z_Uvar_amulitHeadVar_',Two,Rest,Body,SingleUVar,[MulitHeadVar]):-!,
	findall(MulitHeadVar:SingleUVar,mooBeliefLiteral(Depth,PreviousVarSeek,Body),Ans),
	sort(Ans,AnsS),!,
	member(MulitHeadVar:SingleUVar,AnsS).

tground(X):-ground(X).


	
mooBeliefLiteral(Depth,_,Goal,Major,Two,Rest,Body,GoalTemplate,More):-writeq(Goal),nl,fail. 

mooBeliefLiteral(Depth,PreviousVarSeek,Goal,bodyConnectedVar_,Two,Rest,Body,MulitHeadVar,More):-!,
	mooBeliefLiteral(Depth,PreviousVarSeek,Body).
  
mooBeliefLiteral(Depth,PreviousVarSeek,Goal,amulitHeadVar_singleHeadVar_bodyConnectedVar_,Two,Rest,Body,MulitHeadVar,[S,BCV]):-!,
	findall(MulitHeadVar:S,mooBeliefLiteral(Depth,PreviousVarSeek,Body),Ans),
	sort(Ans,AnsS),!,
	member(MulitHeadVar:S,AnsS).

mooBeliefLiteral(Depth,PreviousVarSeek,Goal,amulitHeadVar_singleHeadVar_,Two,Rest,Body,MulitHeadVar,[S]):-!,
	findall(MulitHeadVar:S,mooBeliefLiteral(Depth,PreviousVarSeek,Body),Ans),
	sort(Ans,AnsS),!,
	member(MulitHeadVar:S,AnsS).

mooBeliefLiteral(Depth,PreviousVarSeek,Goal,amulitHeadVar_,Two,Rest,Body,MulitHeadVar,More):-!,
	findall(MulitHeadVar,mooBeliefLiteral(Depth,PreviousVarSeek,Body),Ans),
	sort(Ans,AnsS),!,
	member(MulitHeadVar,AnsS).
	
mooBeliefLiteral(Depth,PreviousVarSeek,Goal,amulitHeadVar_bodyConnectedVar_,Two,Rest,Body,MulitHeadVar,More):-!,
	findall(MulitHeadVar,mooBeliefLiteral(Depth,PreviousVarSeek,Body),Ans),
	sort(Ans,AnsS),!,
	member(MulitHeadVar,AnsS).

mooBeliefLiteral(Depth,PreviousVarSeek,Goal,singleHeadVar_,Two,Rest,Body,MulitHeadVar,More):-!,
	findall(MulitHeadVar,mooBeliefLiteral(Depth,PreviousVarSeek,Body),Ans),
	sort(Ans,AnsS),!,
	member(MulitHeadVar,AnsS).

mooBeliefLiteral(Depth,PreviousVarSeek,Goal,singleHeadVar_bodyConnectedVar_,Two,Rest,Body,MulitHeadVar,More):-!,
	findall(MulitHeadVar,mooBeliefLiteral(Depth,PreviousVarSeek,Body),Ans),
	sort(Ans,AnsS),!,
	member(MulitHeadVar,AnsS).	


mooBeliefLiteral(Depth,_,'$unifyCheck'(Term1,Term2,_),'$unifyCheck',_,Rest,Body,GoalTemplate,More):-!,
		Term1=Term2.
		
mooBeliefLiteral(Depth,_,Term,_,'$g',Rest,Body,GoalTemplate,More):-!,ground(Term).

mooBeliefLiteral(Depth,PreviousVarSeek,Goal,Major,Two,Rest,Body,GoalTemplate,More):-mooCache(Goal,_,KB,Ctx,TN). % ExplainationID:KRVars:KR
		   */     										

si(TopFunctor,X):-
	prolog_frame_attribute(TopFunctor,X,Y),writeq(Y),nl,
	!.

si(X):-
	prolog_current_frame(TopFunctor), R=R,
	prolog_frame_attribute(TopFunctor,X,Y),writeq(Y),nl,
	!.


	
writeColor(Color,Thing,Vars):-
	ignore(writeObject(colourize(Color,Thing),Vars)),!.
writeColor(Color,Thing):-
	ignore(writeObject(colourize(Color,Thing),_)),!.


		    /*

mooBeliefLiteral(Depth,PreviousVarSeek,('t~UnivHead'(Body, HeadTemplate, UniV,HeadVars))):- !,
	once((mooBeliefLiteral(Depth,(HeadTemplate,PreviousVarSeek),Body),ground(Univ))),!.

mooBeliefLiteral(Depth,PreviousVarSeek,('t~UnivDouble'(Body, HeadTemplate, UniV,HeadVars))):- !,
	once((mooBeliefLiteral(Depth,(HeadTemplate,PreviousVarSeek),Body),ground(Univ))),!.

mooBeliefLiteral(Depth,PreviousVarSeek,('t~HeadBody'(Body, HeadTemplate,HeadVars,BodyV))):- !,
	findall(HeadVars,(mooBeliefLiteral(Depth,(HeadTemplate,PreviousVarSeek),Body),ground(BodyV)),Ans),!,
	sort(Ans,AnsS),!,
	member(HeadVars,AnsS).

mooBeliefLiteral(Depth,PreviousVarSeek,('t~HeadBodyDouble'(Body, HeadTemplate,HeadVars,BodyV,HeadVarsDouble))):- !,
	findall(HeadVars^HeadVarsDouble,(mooBeliefLiteral(Depth,(HeadTemplate,PreviousVarSeek),Body),ground(BodyV)),Ans),!,
	sort(Ans,AnsS),!,
	member((HeadVars^HeadVarsDouble),AnsS).

mooBeliefLiteral(Depth,PreviousVarSeek,('t~UnivHeadBody'(Body, HeadTemplate,UniV,HeadVars,BodyV,HeadVarsDouble))):- !,
	findall(HeadVars^HeadVarsDouble,(mooBeliefLiteral(Depth,(HeadTemplate,PreviousVarSeek),Body),ground((UniV,BodyV))),Ans),!,
	sort(Ans,AnsS),!,
	member((HeadVars^HeadVarsDouble),AnsS).

mooBeliefLiteral(Depth,PreviousVarSeek,('t~Headtest'(Body,HeadTemplate,HeadVars))):- !,ground(HeadVars),
	once((mooBeliefLiteral(Depth,(HeadTemplate,PreviousVarSeek),Body))),!.
			       */




/*
t_CPosDVars_('$v'(A),
	 '$existential'(A, 
	 [63, 79, 82, 71, 65, 78, 73, 83, 77],
	  instance([63, 79, 82, 71, 65, 78, 73, 83, 77], 'Organism')and part(B, [63, 79, 82, 71, 65, 78, 73, 83, 77])),
	  '$v'(B))

*/
	
	
%	'$v'(_G273), 
%	'$existential'(_G273, [63, 80, 85, 66], instance([63, 80, 85, 66], 'Publication')and agent([63, 80, 85, 66], _G313)and patient([63, 80, 85, 66], _G328)), 
		
%        '$v'(_G313, _G328),  % Discontected getPrologVars
%	holds('$Relation'(publishes, ['Predicate', 'BinaryPredicate']), '$Object'(_G358, ['Agent'|_G362]), '$Class'(_G364, _G365)))







write_int(Body,RealLiteral):-
	unwrapPatterns(RealLiteral,URealLiteral),writeq(URealLiteral),nl.


mcf1 :- mcf(instance(X,Y)).
mcf2 :- mcf(subclass(X,Y)).
 
mcf(URealLiteral):-mcf(URealLiteral,6).


wrapPatternArgs(URealLiteral,RealLiteral):-!,
	mapOnConj(unwrapPatternsProc,RealLiteral,URealLiteral).
	

mcf(URealLiteral,Bm):-
	resetTableFlags,%trace,		 
	wrapPatternArgs(URealLiteral,RealLiteral),!,
	getCputime(B),
     %   findall(URealLiteral,(deduceCanGoal(KB,Ctx,RealLiteral,Bm),write(URealLiteral),nl),Total),
        findall(URealLiteral,(deduceDepthBoundGoal(RealLiteral,KB,Ctx,[],Bm,O),write(URealLiteral:O),nl),Total),
	getCputime(E),
	sort(Total,Sorted),
	length(Total,N),
	length(Sorted,SortedN),
	write(Sorted),nl,
	T is E-B,
	write(found:N:unique:SortedN:T:seconds),nl.


% ===================================
% Export to prolog file
% ===================================

exportToProlog(KB):-
	tell(KB),fail.
	
exportToProlog(KB):-
	format(':-include(''moo_header.pl'').\n\n'),
	mooCache(Goal,ExplainationID:KRVars:KR,KB,Ctx,TN),
	format('~q.\n\n',[Goal:-accessed(ExplainationID,KRVars,KR,TN)]),
	fail.
exportToProlog(KB):-
	mooCache(Goal,A,ExplainationID:KRVars:KR,KB,Ctx,TN),
	format('~q :-\n\t\t\t~q.\n\n',[Goal,(mooBeliefLiteral(Depth,PreviousVarSeek,A),accessed(ExplainationID,KRVars,KR,TN))]),
	fail.
	
exportToProlog(KB):-
	told,[KB].


accessed(ExplainationID,KRVars,KR,TN).


:-index(deduceInCurrentModel(0,0,0,1,0)).

deduceInCurrentModel(KB,Ctx,P,[],P):-!.
deduceInCurrentModel(KB,Ctx,P,prove(_),_):-!,fail.
deduceInCurrentModel(KB,Ctx,P,ifThen(_,_),_):-!,fail.
deduceInCurrentModel(KB,Ctx,P,impossible(_),_):-!,fail.
deduceInCurrentModel(KB,Ctx,P,and(_,_),_):-!,fail.
deduceInCurrentModel(KB,Ctx,P,not(_),_):-!,fail.
deduceInCurrentModel(KB,Ctx,P,TopFunctor,P):-functor(TopFunctor,function,_),!,fail. %%ground(TopFunctor).
deduceInCurrentModel(KB,Ctx,P,'$existential'(v(_,V,_),Y,TopFunctor),[TopFunctor|P]):-!,ignore(unify_with_occurs_check(TopFunctor,V)),!.
deduceInCurrentModel(KB,Ctx,P,(A,Literal),O):-!,deduceInCurrentModel(KB,Ctx,P,A,M),deduceInCurrentModel(KB,Ctx,M,Literal,O).
deduceInCurrentModel(KB,Ctx,P,Literal,[TN|P]):-
	isDeducedOrKnown(Literal,TN).

isNotAvailable(P,Literal):-fail.
isDeducedOrKnownEitherWay(L,P):-isDeducedOrKnown(L,P).


deduceDepthBoundGoal([],KB,Ctx,P,Bm,P):-!.
deduceDepthBoundGoal([Body|Second],KB,Ctx,P,Bm,O):-!, 
       % not(usedAtLeastEver(TN)),
	deduceDepthBoundGoal(Body,KB,Ctx,P,Bm,M),
	Bn is Bm -1,
	deduceDepthBoundGoal(Second,KB,Ctx,M,Bn,O).

%deduceDepthBoundGoal(Literal,KB,Ctx,P,Bm,P):-writeDebug(deduceDepthBoundGoal(Literal,KB,Ctx,P,Bm)),fail.
deduceDepthBoundGoal(Literal,KB,Ctx,P,Bm,P):-Bm <1,!,deduceInCurrentModel(KB,_Ctx,P,Literal,O).

deduceDepthBoundGoal(and(Body,Second),KB,Ctx,P,Bm,O):-!,
       % not(usedAtLeastEver(TN)),
	deduceInCurrentModel(KB,Ctx,[TN:KRVars|P],Body,M),
	Bn is Bm -1,
	deduceDepthBoundGoal(Second,KB,Ctx,M,Bn,O).

deduceDepthBoundGoal(not(Body),KB,Ctx,P,Bm,O):-!,
	deduceDepthBoundGoal(impossible(Body),KB,Ctx,P,Bm,O).

deduceDepthBoundGoal(Literal,KB,Ctx,P,Bm,O):-
	(deduceInCurrentModel(KB,_Ctx,P,Literal,O);
			(mooCache(Literal,Body,TN:KRVars:_,KB,_Ctx,_),
			((length(KRVars,Goal),(Goal<Bm))),
			not((memberchk(TN:KRVars,P),numbervars(Literal,'$VAR',0,_),isDeducedOrKnownEitherWay(Literal,_))),
			writeDebug(deduceUsageOfRule(Body,Bm,TN,KRVars,P,Literal)),
			deduceUsageOfRule(KB,Ctx,TN,KRVars,P,Literal,Body,Bm,O))).
			% Was Asserted
			
/*
deduceUsageOfRule(KB,Ctx,TN,KRVars,P,Literal,Body,0,O):-!,
	deduceInCurrentModel(KB,Ctx,P,Body,O).
*/

deduceUsageOfRule(KB,Ctx,TN,KRVars,P,Literal,unoptimized(Body),D,O):-!,fail.

deduceUsageOfRule(KB,Ctx,TN,KRVars,P,Literal,[],Bm,P).

deduceUsageOfRule(KB,Ctx,TN,KRVars,P,Literal,ifThen(findall(Vars,Firstly),Second),Bm,[O|M]):-!,
       % not(usedAtLeastEver(TN)),
	Bn is Bm -1,
	findall(Vars,
		(deduceDepthBoundGoal(Firstly,KB,Ctx,[TN:KRVars|P],Bn,M),stableGround(Vars)),List),
	sort(List,ListS),
	writeDebug(green,getPrologVars(Vars)),
	member(Vars,ListS),
	deduceDepthBoundGoal(Second,KB,Ctx,P,Bn,O),
	putDeducedNew(Literal,O,[M,O]).
	

deduceUsageOfRule(KB,Ctx,TN,KRVars,P,Literal,'$existential'(v(_,V,_),Y,TopFunctor),Bm,[TN:TopFunctor|P]):-!,ignore(unify_with_occurs_check(TopFunctor,V)),!.

deduceUsageOfRule(KB,Ctx,TN,KRVars,P,Literal,ifThen(Firstly,Second),Bm,[O|M]):-!,
       % not(usedAtLeastEver(TN)),
	Bn is Bm -1,
	deduceDepthBoundGoal(Firstly,KB,Ctx,[TN:KRVars|P],Bn,M),
	stableGround(Firstly),
	deduceDepthBoundGoal(Second,KB,Ctx,P,Bn,O),
	putDeducedNew(Literal,O,[M,O]).


deduceUsageOfRule(KB,Ctx,TN,KRVars,P,Literal,prove(Body),D,O):-!,
	 not(memberchk(TN:KRVars,P)),!,
	D2 is D -1,
	deduceDepthBoundGoal(Body,KB,Ctx,[TN:KRVars|P],D2,O),
	putDeducedNew(Literal,O).

/*
	deduceInCurrentModel(KB,Ctx,[r(TN,Body,KRVars)|P],Body,O),!,
	putDeducedNew(Literal,O).
  */

deduceUsageOfRule(KB,Ctx,TN,KRVars,P,Literal,Body,D,O):-ground(Literal),!,
       % not(usedAtLeastEver(TN)),
	deduceInCurrentModel(KB,Ctx,[r(TN,Body,KRVars)|P],Body,O),!,
	putDeducedNew(Literal,O).
	

deduceUsageOfRule(KB,Ctx,TN,KRVars,P,Literal,Body,D,O):-
	D2 is D -1,
	deduceDepthBoundGoal(Body,KB,Ctx,[TN:KRVars|P],D2,O),
	putDeducedNew(Literal,O).


isNotAvailableBackChain(_,_):-fail.

isDeducedOrKnown(Lit,Explaination):-
	(mooCache(Lit,_, KB, Ctx, Explaination);recorded(Lit,Lit:Explaination)).
putDeduced(Lit,Explaination):-(isDeducedOrKnown(Lit,_);recorda(Lit,Lit:Explaination)),!,writeDebug(green,learned(Lit)).
putDeducedNew(Lit,Explaination):-recorda(Lit,Lit:Explaination).


memberchk_cnj(Lit,(Lit,_)).	
memberchk_cnj(Lit,(Lit)).	
memberchk_cnj(Lit,(_,A)):-memberchk_cnj(Lit,A).


/*
mooCache(holds(v('Abstract', modalProperty, ['Relation', 'Predicate', 'BinaryPredicate'|A]), v('Physical', B, ['Object', 'ContentBearingObject', 'LinguisticExpression', 'Phrase', 'Clause', 'Sentence', 'Formula'|Goal]), v('Abstract', D, ['Attribute', 'NormativeProperty'|E])), prove([holds(v('Abstract', modalProperty, ['Relation', 'Predicate', 'BinaryPredicate'|TopFunctor]), v('Physical', G, ['Object', 'ContentBearingObject', 'LinguisticExpression', 'Phrase', 'Clause', 'Sentence', 'Formula'|H]), v('Abstract', D, ['Attribute', 'NormativeProperty'|E])), entails(v('Physical', G, ['Object', 'ContentBearingObject', 'LinguisticExpression', 'Phrase', 'Clause', 'Sentence', 'Formula'|H]), v('Physical', B, ['Object', 'ContentBearingObject', 'LinguisticExpression', 'Phrase', 'Clause', 'Sentence', 'Formula'|Goal]))]), 
	110343:['FORMULA1'=G, 'PROP'=D, 'FORMULA2'=B]:entails(and(modalProperty(G, D), entails(G, B)), modalProperty(B, D))

	, 'PrologMOO', 'QUALITIES', 7453).
*/


:-index(deduceGoal(0,0,1,1,0,1,1)).

deduceGoal(KB,Ctx,ExplainationIn,MaxVars,(H,T),ExplainationOut):-
	deduceGoal(KB,Ctx,ExplainationIn,MaxVars,H,ExplainationMid),
	deduceGoal(KB,Ctx,ExplainationMid,MaxVars,T,ExplainationOut).

/*
deduceGoal(KB,Ctx,ExplainationIn,MaxVars,not NLiteral,Refs):-!,% trace,
	NLiteral=..[TopFunctor|Args],(atom_concat('~',FN,TopFunctor);atom_concat('~',TopFunctor,FN)),!,
		not(Literal=..[FN|Args],!,deduceGoal(KB,Ctx,ExplainationIn,MaxVars,Literal,Refs)),!.
*/

deduceGoal(KB,Ctx,ExplainationIn,MaxVars,\+ Literal,ExplainationIn):-!,not(deduceGoal(KB,Ctx,[neg|ExplainationIn],MaxVars,Literal,_)),!.



	
deduceGoal(KB,Ctx,ExplainationIn,MaxVars,'$existential'(v(_,TopFunctor,_),A,TopFunctor),_):-!,ignore(V=TopFunctor). %,numbervars(TopFunctor,'skolem',0,_),!.

/*
deduceGoal(KB,Ctx,ExplainationIn,MaxVars,Literal,ExplainationOut):-
		Literal=..[TopFunctor|Args],deduceGoalF(KB,Ctx,ExplainationIn,MaxVars,TopFunctor,Literal,Args,ExplainationOut).

deduceGoalF(KB,Ctx,ExplainationIn,MaxVars,holds,Literal,[v(_,TopFunctor,_)|Args],Ref):-
	atom(TopFunctor),!,NewGoal=..[TopFunctor|Args],deduceGoalDB(KB,Ctx,ExplainationIn,MaxVars,NewGoal,Ref).
deduceGoalF(KB,Ctx,ExplainationIn,MaxVars,holds,Literal,[TopFunctor|Args],Ref):-!,
	atom(TopFunctor),NewGoal=..[TopFunctor|Args],deduceGoalDB(KB,Ctx,ExplainationIn,MaxVars,NewGoal,Ref).
deduceGoalF(KB,Ctx,ExplainationIn,MaxVars,TopFunctor,Literal,Args,Ref):-
	NewGoal=..[holds,TopFunctor|Args],deduceGoalDB(KB,Ctx,ExplainationIn,MaxVars,NewGoal,Ref).
deduceGoalF(KB,Ctx,ExplainationIn,MaxVars,TopFunctor,Literal,Args,Ref):-
	NewGoal=..[holds,v(_,TopFunctor,_)|Args],deduceGoalDB(KB,Ctx,ExplainationIn,MaxVars,NewGoal,Ref).


mooRulebase(RealLiteral,Body,TN:KRVars):-
	mooCache(RealLiteral,guard(RFVH,FVH,Body,CLID,KRVars,RuleVars,UnivLiteral,BodyUniv,
	BodySelfConnected,Shared,PrivLiteral,FakeLiteral),Key,KB,Ctx,TN).


finishgaf(Literal,Bm):-
        mooCache(Lit,_, KB, Ctx, TN),
        memberchk_cnj(Lit,Body),
        mooRulebase(Literal,Body,RN),    %%  trace,
	deduceCanGoal(KB,Ctx,Literal,Body,0).
	

deduceCanGoal(KB,Ctx,'$existential'(v(_,V,_),Y,TopFunctor)):-!,ignore(unify_with_occurs_check(TopFunctor,V)),!.
deduceCanGoal(KB,Ctx,(A,Literal)):-!,
	deduceCanGoal(KB,Ctx,A),deduceCanGoal(KB,Ctx,Literal).
deduceCanGoal(KB,Ctx,Literal):-mooCache(Literal,_, KB, Ctx, TN).

deduceCanGoal(KB,Ctx,Literal,Bm):-
	deduceCanGoal(KB,Ctx,Literal);
	( Bm>0, 
        mooRulebase(Literal,Body,RN,Goal),
	Goal<Bm,     %trace,
	Bn is Bm -1,
	deduceCanGoal(KB,Ctx,Literal,Body,Bn)).
	
deduceCanGoal(KB,Ctx,Literal,Body,Bn):-ground(Literal),!,deduceCanGoal(KB,Ctx,Body,Bn),!,not(mooCache(Literal,_, KB, Ctx, _)).
deduceCanGoal(KB,Ctx,Literal,Body,0):-!,deduceCanGoal(KB,Ctx,Body).
deduceCanGoal(KB,Ctx,Literal,Body,N):-deduceCanGoal(KB,Ctx,Body,N).

memberchk_cnj(Lit,(Lit,_)).	
memberchk_cnj(Lit,(Lit)).	
memberchk_cnj(Lit,(_,A)):-memberchk_cnj(Lit,A).

mooRulebase(RealLiteral,Body,TN,N):-
		mooRulebase(RealLiteral,Body,TN:KRVars),length(KRVars,N).

mooRulebase(RealLiteral,Body,TN:KRVars):-
	mooCache(RealLiteral,guard(RFVH,FVH,Body,CLID,KRVars,RuleVars,UnivLiteral,BodyUniv,
	BodySelfConnected,Shared,PrivLiteral,FakeLiteral),Key,KB,Ctx,TN).
	
								    */


	%,ground(RealLiteral))).
	%deduceCanGoal(KB,Ctx,Body,Bn).

/*
mcf(URealLiteral,Bm):- Bn is Bm +1,
	unwrapPatterns(RealLiteral,URealLiteral),
	%mooCache(Lit,_, KB, Ctx, TN),
*/
/*
	memberchk_cnj(Lit,Body),
	%not((ground(RealLiteral),deduceCanGoal(KB,Ctx,Body))),
	deduceCanGoal(KB,Ctx,Body),
	write_int(Body,RealLiteral),fail.
%	memberchk_cnj(Lit,Body),(ground(MG) -> (write(RealLiteral),nl,fail) ; ).
*/
	






	


createLocalDistrict(Econ):-
	resetTableFlags,
	mooCache(RealLiteral,guard(RFVH,FVH,Body,CLID,KRVars,RuleVars,UnivLiteral,
		BodyUniv,BodySelfConnected,Shared,PrivLiteral,FakeLiteral),Key,KB,Ctx,TN),
	createMerchant(Econ,TN,FakeLiteral,RealLiteral,Body,RuleVars),fail.
createLocalDistrict(Econ):-!.



solveLiteral(Lit):-
%	createFreeEconomy(Econ),
%	resetTableFlags,
	Econ=1,
	createBuyer(Econ,Buyer,Lit), trace,
	setBuyerDemand(Econ,Buyer,Lit),
	giveCash(Econ,Buyer,200),!,
	researchShopping(Econ,Buyer,[Cost-Best|TodoList]),!,
	startBuyingAllYouCan(Econ,BestTodoList,Have),
	returnLits(Buyer,Have,Lit).
	
returnLits(Buyer,Have,Have):-retract(sup(Have)).

startBuyingAllYouCan(Econ,BestTodoList,Have).
	
	
researchShopping(Econ,Buyer,BestTodoList):-
	findall(Result,(
		recorded(Buyer,wants(Lit)),
		recorded(Econ,advertises(Merchant,Lit)),
		not(recorded(Merchant,wants(Lit))),
		getEstimate(Econ,Buyer,Lit,Merchant,Result)
		),Merchants),
		keysort(Merchants,BestTodoList). % Puts cheapest first
		
getEstimate(Econ,Buyer,PrimaryProduct,Merchant,(Markup-Merchant)):-
	recorded(Merchant,recipee(Needs,PrimaryProduct),_),
	researchShoppingForMerchant(Econ,Merchant,Needs,Lit,Cost),
	supply(Lit),
	Markup is Cost +1,!.
	
researchShoppingForMerchant(Econ,Merchant,(A,B),Lit,Cost):-!,
	researchShoppingForMerchant(Econ,Merchant,(A),Lit,Cost1),!,
	researchShoppingForMerchant(Econ,Merchant,(B),Lit,Cost2),
	supply(Lit),
	Cost is Cost1+Cost2,!.
	
researchShoppingForMerchant(Econ,Merchant,(B),Lit,0):-
	recorded(Merchant,inventory(B),_),!.

researchShoppingForMerchant(Econ,Merchant,(B),Lit,Cost):-
	comodity(Merchant,Econ,Lit,Cost),
	findall(N,(comodity(Merchant,Econ,Lit,_),recorda(Merchant,inventory(Lit)),supply((Lit))),List),List=[_|_],!.
		

supply(X):-asserta(sup(X)).

		

%comodity(Buyer,Econ,'$existential'(v(_,V,_),_,TopFunctor),2):-ignore(V=TopFunctor),!.
comodity(Buyer,Econ,Lit,1):-
	mooCache(Lit,_, KB, Ctx, TN),
	once(flag(TN,X,X+1)),X<6.


	
researchShoppingForMerchant(Econ,Merchant,Lit,Lit,Resale):-
	recorda(Merchant,wants(Lit),Ref),
	findall(Result,(
		recorded(Econ,advertises(Other,Lit)),
		%not((recorded(Other,wants(Lit)))),
		not((recorded(Other,wants(_)))),
		not((recorded(Econ,consumes(Other,Lit)))),
		getEstimate(Econ,Merchant,Lit,Other,Result)
		),Merchants),
		erase(Ref),!,keysort(Merchants,[Cost-Best|TodoList]),!,
		Resale is Cost+1.

		
getMerchant(Econ,Merchant,WishList,Forsale,Cash,Recipee):-
	recorded(Econ,merchant(Name,Catagory),Merchant),
	not(recorded(Econ,outlawed(Catagory),_)),
	not(recorded(Econ,busted(Name),_)),
	getMerchantInfo(Econ,Merchant,WishList,Forsale,Cash,Recipee).
	
getMerchantInfo(Econ,Merchant,WishList,Forsale,Cash,recipee(Needs,PrimaryProduct)):-
	flag(Merchant,Cash,Cash), 
	findall(Wants,recorded(Econ,consumes(Merchant,Wants)),WishList),
	findall(Sells,recorded(Econ,advertises(Merchant,Sells)),Forsale),!,
	recorded(Merchant,recipee(Needs,PrimaryProduct),_).
	
	

%shoppingSpree(Buyer,Lit,Econ):-
startBuying(Econ,Buyer,Lit,BestTodoList):-
	getFirstErrand(Buyer,Goal,Errand),
	getCash(Econ,Buyer,Cash),
	cheapest(Econ,Errand,Cost),
	Cost < Cash -> 
			% Can afford
			(buyErrand(Buyer,Errand,Cost),!,
			sellUneededExtra(Buyer,Econ),!,
			calculateMostGoodsForCash(Cash,Lit,BestTodoList),!,
			startBuying(Econ,Buyer,Lit,BestTodoList)
			);
			% Can't afford
			(putOnWishlist(BestTodoList,Buyer,Errand,NewBestTodoList),!,
			startBuying(Econ,Buyer,Lit,NewBestTodoList)).
	


createMerchant(Econ,Name,Catagory,PrimaryProduct,Needs,Coupons):-
	recorda(Econ,merchant(Name,Catagory),Merchant),!,
	recorda(Econ,advertises(Merchant,PrimaryProduct)),!,
	createConsumption(Econ,Merchant,Needs),
	flag(Merchant,_,1), % One Dollar
	createRecipe(Econ,Merchant,Needs,PrimaryProduct),
	recorded(Econ,coupons(Coupons)),!.

createConsumption(Econ,Merchant,(A,B)):-!,
	createConsumption(Econ,Merchant,A),	
	createConsumption(Econ,Merchant,B).
createConsumption(Econ,Merchant,A):-!,
	recorda(Econ,consumes(Merchant,A)),!.

createRecipe(Econ,Merchant,Needs,PrimaryProduct):-
	recorda(Merchant,recipee(Needs,PrimaryProduct),_),!.

getDeadbeatBuyer(Econ,Buyer,WishList,Forsale):-
	recorded(Econ,buyer(Lit),Buyer),
	flag(Buyer,X,X),X<1,
	getTrade(Buyer,WishList,Forsale).
	



createFreeEconomy(Econ):-Econ is random(50),!.

createBuyer(Econ,Buyer,Lit):-
	recorda(Econ,buyer(Lit),Buyer),!.	
	
setBuyerDemand(Econ,Buyer,Lit):-
	recorda(Buyer,wants(Lit)).

setBuyerSurplus(Econ,Buyer,Lit):-
	recorda(Buyer,sells(Lit)).

getFreeStuff(Econ,FreeStuff):-
	recorded(Econ,free(FreeStuff)),
	not(recorded(Econ,outlawed(FreeStuff),_)).

	


	
giveCash(Econ,Buyer,Cash):-
	flag(Buyer,X,X+Cash),!.

takeCash(Econ,Buyer,Cash):-
	flag(Buyer,X,X-Cash),!.
	

inventory(Stack,Literal):-member(gaf(Literal),Stack).


getConsumersOfLiteral(Stack,Literal,WhoList):-findall(Who,member(req(Literal,Who),Stack),WhoList).

supplyLiteralToWhoListFree(Stack,Literal,WhoList):-findall(Who,member(req(Literal,Who),Stack),WhoList).

getSuppliersMenu(Stack,Literal,SuppliersMenu):-
		findall(Who,member(req(Literal,Who),Stack),WhoList).


		       
/*
deduceGoal(KB,Ctx,[consumer(TN,Goal,)|ExplainationIn],MaxVars,g(RealLiteral,ImportantVars),[sm(Literal,TN)|ExplainationIn]):-
		mooCache(RealLiteral,_, KB, Ctx, TN),
		(not(ground(ImportantVars)) -> ! ; true ). %,not(member(TN,Refs)).
  */
  
% Ignored Stuff  i(ignored), 
% Stable Table for st(instance($0,'Class')). as sm(instance('Attribute','Class')).
% Delay calls to d(instance(X,'Class')). 




deduceGoal(KB,Ctx,[Consumer|ExplainationIn],MaxVars,g(RealLiteral,ImportantVars),ExplainationOut):-MaxVars>0,
	ignore(tn(bogus,PVars,RealLiteral) = Consumer),
	ignore(MaxVars=PVars),NMaxVars is MaxVars - 1,
	ensureKey(RealLiteral,CallLiteral,Depth,HashKey),	
	mooCache(RealLiteral,guard(RFVH,FVH,Body,CLID,KRVars,RuleVars,UnivLiteral,BodyUniv,BodySelfConnected,Shared,PrivLiteral,FakeLiteral),Key,KB,Ctx,TN),
	copy_term(Body,BodyInstanceCopy),numbervars(BodyInstanceCopy,'$',0,NBodyVarsLeft),
	compare(CmpPrevious,PVars,NBodyVarsLeft),
	writeq(mooCacheCall(RealLiteral,CallLiteral,CmpPrevious,MaxVars,PVars,NBodyVarsLeft)),nl,
	mooCacheCall(CmpPrevious,[Consumer|ExplainationIn],MaxVars,PVars,NBodyVarsLeft,
		RealLiteral,CallLiteral,BodyInstanceCopy,RFVH,FVH,Body,CLID,KRVars,RuleVars,
		UnivLiteral,BodyUniv,BodySelfConnected,Shared,PrivLiteral,FakeLiteral,Key,KB,Ctx,TN,ExplainationOut).



ensureKey(Literal,Key,Depth,HashKey):-
	copy_term(Literal,Proto),
	unwrapPatterns(Proto,Key),
	numbervars(Key,'$',0,Depth),
	hash_term(Key,HashKey),!.

unwrapPatterns(Term,OTerm):-
	mapOnConj(unwrapPatternsProc,Term,OTerm).
	

unwrapPatternsProc(Term,OTerm):-nonvar(Term),!,
 	Term=..[TopFunctor|Args],
	getArgsd(Args,New),!,
 	OTerm=..[TopFunctor|New].

unwrapPatternsProc(Term,OTerm):-
 	OTerm=..[TopFunctor|New],
	getArgsdr(Args,New),
 	Term=..[TopFunctor|Args],!.

getArgsd([],[]).
getArgsd([v(_,v,_)|T],['$'|TT]):-!,getArgsd(T,TT).
getArgsd([v(_,A,_)|T],[A|TT]):-!,getArgsd(T,TT).
getArgsd(['$'|T],['$'|TT]):- !,getArgsd(T,TT).
getArgsd([H|T],[H|TT]):- !,getArgsd(T,TT).

getArgsdr([],[]).   getArgsdr([v(_,V,_)|T],[V|TT]):-!,getArgsdr(T,TT).


% When a GAF is not found and we need to backchain:

% First we examine the backhain 'Goal' Literal .. exmaples are:

% instance(v(_,X,_),v(_,'Class',_)) And which Bindings they to actually know

% for example if they are just asking for arg 2 of instance  then we want to cut on first solution
% in this case the prototype is g(instance(v(_,X,_),v(_,'Class',_),['Class'])

% if they want X then we must find all uniue binding of X
% so the prototype of the head is  g(instance(v(_,X,_),v(_,'Class',_),[X])

% if they want X and Y in g(instance(v(_,X,_),v(_,Y,_),[X,Y]).

% we first make a session prototype:  g(instance(v(_,X,_),v(_,Y,_),[X,Y]) -> g(instance(v(_,$VAR,_),v(_,$VAR,_),[$VAR(1),$VAR(2)])

% Tries to give table (Must have been at least prevoius calls as well (A B Goal))
deduceGoalCall(>,[A,B,Goal|ExplainationIn],MaxVars,PVars,NBodyVarsLeft,
	RealLiteral,CallLiteral,BodyInstanceCopy,RFVH,FVH,Body,CLID,KRVars,RuleVars,
	UnivLiteral,BodyUniv,BodySelfConnected,Shared,PrivLiteral,FakeLiteral,Key,KB,Ctx,TN,[did(RealLiteral),A,B,Goal|ExplainationIn]):-
	writeq(tryingToGiveOneAnswer(CallLiteral)),recorded(TN,RealLiteral),!,
	writeq(did(RealLiteral)),nl,!.   % Give Table only if have it

% Tries to give stack but likely will fail.
/*
mooCacheCall(>,Previous,MaxVars,PVars,NBodyVarsLeft,
	RealLiteral,CallLiteral,BodyInstanceCopy,RFVH,FVH,Body,CLID,KRVars,RuleVars,
	UnivLiteral,BodyUniv,BodySelfConnected,Shared,PrivLiteral,FakeLiteral,Key,KB,Ctx,TN,[preproved(CallLiteral)|ExplainationOut):-!,
	member(RealLiteral,Previous),member(preproved(CallLiteral)).
*/

mooCacheCall(_,Previous,MaxVars,PVars,NBodyVarsLeft,
	RealLiteral,CallLiteral,BodyInstanceCopy,RFVH,FVH,Body,CLID,KRVars,RuleVars,
	UnivLiteral,BodyUniv,BodySelfConnected,Shared,PrivLiteral,FakeLiteral,Key,KB,Ctx,TN,[gave(RealLiteral)|Previous]):-
	recorded(TN,CallLiteral,Ref),!, % Known call table
	writeq(tryingToGiveAllAnswers(CallLiteral)),
	recorded(TN,RealLiteral),   % Give Full Table
	writeq(gave(RealLiteral)).
	
mooCacheCall(_,Previous,MaxVars,PVars,NBodyVarsLeft,
	RealLiteral,CallLiteral,BodyInstanceCopy,RFVH,FVH,Body,CLID,KRVars,RuleVars,
	UnivLiteral,BodyUniv,BodySelfConnected,Shared,PrivLiteral,FakeLiteral,Key,KB,Ctx,TN,ExplainationOut):-
	recorda(TN,CallLiteral,Ref),!, % Table the call
	writeq((slide:TN:NVars: (<):PVars:(<):MaxVars:PTN)),nl,
	findall(Shared,((
	     %   call_with_depth_limit(
				deduceGoal(KB,_Ctx,[tn(TN,NVars,Body),PIN|ExplainationIn],NMaxVars,Body,ExitP) %,ground(Shared)   %
	      %  ,90,DL),ground(Shared)
			)),Sols),
			list_to_set(Sols,SolsS),
			length(SolsS,N),!,
	N>0,
	%trace,
	write(TN:returned:N),
	member(Shared,SolsS),
	recorda(TN,RealLiteral:Body).



/*
		%,write(Explaination).
	

		%ground(UnivLiteral), % Makes sure this is valid
		%not(Functor=not(_)),
	erase(Ref),%trace,

	%copy_term(RFVH,Session),
	%numbervars(Session,'$',0,_),
	
%	=(RFVH,FVH), %unifies list of 'real' prolog variables
%        not(recorded(TN,RuleVars,Ref)),
 %       copy_term(RuleVars,Session),
	%numbervars(FVH,'$VAR',0,_),
  %      recorda(TN,Session,Ref),


deduceGoal(KB,Ctx,ExplainationIn,MaxVars,Literal,Ref):-Literal=..[TopFunctor|Args],!,
	not(member(Literal,Ref)),
	deduceGoalF(KB,Ctx,ExplainationIn,MaxVars,TopFunctor,Literal,Args,[Literal|Ref]).
	
deduceGoalF(KB,Ctx,ExplainationIn,MaxVars,holds,Orig,Args,Ref):-!,fail.
deduceGoalF(KB,Ctx,ExplainationIn,MaxVars,TopFunctor,Orig,Args,Ref):-
	Literal=..[holds,TopFunctor|Args],
	deduceGoal(KB,Ctx,ExplainationIn,MaxVars,Literal,Ref).

*/




igi:-igi('Relation').


igi(Class):-
	ig(X,instance(v(_,X,_),v('Abstract', Class,_))).

ig(X):-ig(X,X).

ig(Y,X):-
	resetTableFlags,
       findall(Y,(deduceGoalD(KB,Ctx,[tn(0,55,request)],50,X,Out),write(Y),nl),L),
       nl,nl,writeq(L),nl,
       length(L,N),
       sort(L,LS),
       length(LS,LN),
       write(num:N:u:LN),nl.
       

epc1:-
	call_with_depth_limit(epc(X=Y,instance(X,Y)),30,_).
	

epc(PreviousVarSeek,Goal):-
	resetTableFlags,
       findall(PreviousVarSeek,(mooBeliefLiteral(5,PreviousVarSeek,Goal),writeColor(green,Goal)),L),
       length(L,N),
       sort(L,LS),
       length(LS,LN),
       write(num:N:u:LN),nl.
	
	
isAtLeastOne([_|_]).

