:-include('logicmoo_utils_header.pl').
% ===========================================================
% Logical Form Display
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(logicforms=logicforms,Options),!,parse_moo_lf(Options).

parse_moo_lf(Options):-memberchk(submit='Negated',Options),!,make,
        getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(Agent,opt_theory='LogicMOO',Context),
        getMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        getMooOption(Agent,user='Web',Agent),
        getMooOption(Agent,interp='kif',Interp),
        logOnFailure(getMooOption(Agent,tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        getNegationForm(NEWFORM,Vars,Output,Paths),
        writeObject('$spacer',Vars),
        writeObject(Output,Vars),
        writeObject('$spacer',Vars),
        writeObject('Paths '(Paths),Vars),!.

parse_moo_lf(Options):-memberchk(submit='Conjunctive',Options),!,make,
        getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(Agent,opt_theory='LogicMOO',Context),
        getMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        getMooOption(Agent,user='Web',Agent),
        getMooOption(Agent,interp='kif',Interp),
        logOnFailure(getMooOption(Agent,tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        getNegationForm(NEWFORM,Vars,NNF,Paths),
        getConjNF(NNF,Output),
        writeObject('$spacer',Vars),
        writeObject(Output,Vars).

parse_moo_lf(Options):-memberchk(submit='Disjunctive',Options),!,make,
        getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(Agent,opt_theory='LogicMOO',Context),
        getMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        getMooOption(Agent,user='Web',Agent),
        getMooOption(Agent,interp='kif',Interp),
        logOnFailure(getMooOption(Agent,tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        getNegationForm(NEWFORM,Vars,NNF,Paths),
        getDisjForm(NNF,Output),
        writeObject('$spacer',Vars),
        writeObject(Output,Vars).


parse_moo_lf(Options):-
        (memberchk(submit='Entailment',Options);memberchk(submit='Given',Options)),
        !,make,
        getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(Agent,opt_theory='LogicMOO',Context),
        getMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        getMooOption(Agent,user='Web',Agent),
        getMooOption(Agent,interp='kif',Interp),
        logOnFailure(getMooOption(Agent,tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        getAssertionClauses(Context,NEWFORM,Output,Vars,T),
        writeObject_conj(Output,Vars),
        writeObject('$spacer',Vars),
        writeObject('Paths '(T),Vars),!.

parse_moo_lf(Options):-
        memberchk(submit='True',Options),!,make,
        getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(Agent,opt_theory='LogicMOO',Context),
        getMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        getMooOption(Agent,user='Web',Agent),
        getMooOption(Agent,interp='kif',Interp),
        logOnFailure(getMooOption(Agent,tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        getAssertionClauses(Context,NEWFORM,Output,Vars,_),
        writeObject('$spacer',Vars),
        writeObject(Output,Vars).

parse_moo_lf(Options):-
        memberchk(submit='False',Options),!,make,
        getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(Agent,opt_theory='LogicMOO',Context),
        getMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        getMooOption(Agent,user='Web',Agent),
        getMooOption(Agent,interp='kif',Interp),
        logOnFailure(getMooOption(Agent,tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        getAssertionClauses(Context,not(NEWFORM),Output,Vars,_),
        writeObject('$spacer',Vars),
        writeObject(Output,Vars).


parse_moo_lf(Options):-
        memberchk(submit='Possible',Options),!,make,
        getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(Agent,opt_theory='LogicMOO',Context),
        getMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        getMooOption(Agent,user='Web',Agent),
        getMooOption(Agent,interp='kif',Interp),
        logOnFailure(getMooOption(Agent,tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        getAssertionClauses(Context,possible(NEWFORM),Output,Vars,_),
        writeObject('$spacer',Vars),
        writeObject_conj(Output,Vars).

parse_moo_lf(Options):-
        memberchk(submit='Impossible',Options),!,make,
        getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(Agent,opt_theory='LogicMOO',Context),
        getMooOption(Agent,sf=surf,Assertion),
        atom_codes(Assertion,Assertion_Chars),
        getMooOption(Agent,user='Web',Agent),
        getMooOption(Agent,interp='kif',Interp),
        logOnFailure(getMooOption(Agent,tn=_,TN)),
        logOnFailure(getCleanCharsWhitespaceProper(Assertion_Chars,Show)),!,
        logOnFailure(getSurfaceFromChars(Show,STERM,Vars)),!,
        logOnFailure(getMooTermFromSurface(STERM,NEWFORM)),!,
        getAssertionClauses(Context,not(possible(NEWFORM)),Output,Vars,_),
        writeObject('$spacer',Vars),
        writeObject_conj(Output,Vars).

parse_moo_lf(Options):-
        memberchk(submit='All Forms',Options),
        getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(Agent,opt_theory='LogicMOO',_Context),
        getMooOption(Agent,data=instance,TN),!,
        writeKnownFormsTN(Ctx,Context:TN).

parse_moo_lf(Options):-
        memberchk(submit='mooCache',Options),
        getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
        getMooOption(Agent,opt_theory='LogicMOO',_Context),
        getMooOption(Agent,data=instance,TN),!,
        listing_template(mooCache(_,_,_,_,_,_,TN,_,_)),
        listing_template(mooCache(PredR,Fact,Type,_,Context,(surf(Context,TN) ))),
        listing_template(mooCache(PredR,Fact,Pre,Type,_,Context,(surf(Context,TN) * via(_,Vars)))),
        !.

% ========================================================================
% Channel complie the CycKB
% ========================================================================


clean_base_cache:-
      retractall(saved('#$isa'(_,'#$Thing'),_,_,_)),
      retractall(saved('#$isa'(foo,_),_,_,_)).


saveFullImage:-
      clean_base_cache,
      tell('save.image'),
      format('
:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).
'),
     % buildGenlExtent,
      % Build KB
      ignore((saved(Data,Mt,A,C),
      convertCycKBToHL(saved(Data,Mt,A,C),New),
      rememberHLFact(New),fail)),
      told,%
	 tell('world.pl'),
      format('
:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).
:- ensure_loaded(logicmoo_hl_access).
'),
   makeHooks,
   format(':-~q.~n',[include('save.image')]),
   told,ensure_loaded('world.pl').



numberArgs(N,[]).
numberArgs(N,[1|T]):-N<5,NN is N+1,numberArgs(NN,T).
numberArgs(N,[0|T]):-numberArgs(N,T),!.


% buildDeclaredGenlExtent
% Mine the data
% buildDeclaredGenlExtent


% ========================================================================
% Hook up predicae hashes
% ========================================================================

makeHooks:-
   decontextualizedHLFact(hashOnPred(_,_,List)),
   rememberHLFact(callSig(List)),fail.
makeHooks:-decontextualizedHLFact(callSig(List)),
   makeHooksSig(List),fail.
makeHooks.

makeHooksSig(List):-
	    retractall(decontextualizedHLFact(( _:- _))),
	    retractall(decontextualizedHLFact((:- _))),
	    length(List,Arity),
	    Arity2 is Arity +2,
	    length(AList,Arity2),
	    concat_atom(['%backcall'|List],'',BackCallF),Back=..[BackCallF|AList],
	    concat_atom(['%gaf'|List],'',GafCallF),Gaf=..[GafCallF|AList],
	    concat_atom(['%rule'|List],'',RuleCallF),Rule=..[RuleCallF|AList],
	    concat_atom(['%univ'|List],'',UnivCallF),Univ=..[UnivCallF|AList],!,
	    rememberHLFact([(:-dynamic(Gaf)),(:-dynamic(Rule)),(:-dynamic(Univ)),(Back:-Gaf),(Back:-Univ),(Back:-Rule)]),!.

% ========================================================================
% GENLS Caching
% ========================================================================

buildDeclaredGenlExtent :-
      retractall(decontextualizedHLFact(isGenlsCache(_,_))),
      surfaceInference('#$genls'(X,'#$Thing')),
      addGenlExtent(X,'#$Thing'),fail.
buildDeclaredGenlExtent.

addGenlExtent(Parent,GParent):-
	 (surfaceInference('#$genls'(Child,Parent));decontextualizedHLFact(isGenlsCache(Child,Parent))),
	 not(contains_term(Child,Parent)),not(contains_term(Parent,Child)),
	 not(decontextualizedHLFact(isGenlsCache(Child,GParent))),
	 rememberHLFact(isGenlsCache(Child,GParent)),
	 addGenlExtent(Child,Parent).

% ========================================================================
% ISA Caching
% ========================================================================

buildDeclaredIsaExtent :-
      retractall(decontextualizedHLFact(isNearestIsaCache(_,_))),
      retractall(decontextualizedHLFact(isNearestIsaCacheNG(_,_))),
      surfaceInference('#$isa'(E,C)),
      addIsaExtent(E,C),fail.
buildDeclaredIsaExtent:-
      tell(isa),
      decontextualizedHLFact(isNearestIsaCache(E,C)),
      format('~q.~n',[isNearestIsaCache(E,C)]),
      fail.
buildDeclaredIsaExtent:-
      decontextualizedHLFact(isNearestIsaCacheNG(E,C)),
      format('~q.~n',[isNearestIsaCacheNG(E,C)]),
      fail.
buildDeclaredIsaExtent:-told.


trimIsaCache:-
	 isNearestIsaCache(Isa,Collection),
      isGenlsCache(Collection,Super),
      killableSuper(Super),
      retract(isNearestIsaCache(Isa,Super)),
      format('~q.~n',[isNearestIsaCache(Isa,Super)]),fail.


killableSuper('#$Thing'):-!,fail.
killableSuper(_).



      
addIsaExtent(E,New):-not(ground((E,New))),!,
      asserta(decontextualizedHLFact(isNearestIsaCacheNG(E,New))),!.
addIsaExtent(E,New):-
	 rememberHLFact(isNearestIsaCache(E,New)),
	 isGenlsCache(New,Before),rememberHLFact(isNearestIsaCache(E,Before)).

% ========================================================================
% Compiler output
% ========================================================================
:-dynamic(decontextualizedHLFact/1).
%:-index(decontextualizedHLFact/1).
rememberHLFact(X):-numbervars(X),rememberHLFactNumbered(X),!.
rememberHLFactNumbered([]):-!.
rememberHLFactNumbered([H|T]):-rememberHLFactNumbered(H),rememberHLFactNumbered(T),!.
rememberHLFactNumbered(argIsaTop(_, _, '#$Thing')):-!.
%rememberHLFactNumbered(X):-decontextualizedHLFact(X),!.
%rememberHLFactNumbered(X):-functor(X,F,_),not(atom_concat('#$',_,F)),assertz(decontextualizedHLFact(X)),fail.
rememberHLFactNumbered(asIs(X)):-!,rememberHLFactNumbered(X),!.
rememberHLFactNumbered(X):-format('~q.~n',[X]).
%      rememberHLFactNumberedW(X).

   
rememberHLFactNumberedF(X):-functor(X,F,A),rememberHLFactNumberedF(F,A).

rememberHLFactNumberedF((':-'),_).
rememberHLFactNumberedF(F,A):-
      decontextualizedHLFact(predMemo(F,A)) -> true ; asserta(decontextualizedHLFact(predMemo(F,A))).

% ====================================================================
% KB Saving (no proof) 
% ====================================================================

convertCycKBToHL(save(X,_,_,_),_):-not(compound(X)),!,fail.
convertCycKBToHL(saved('#$ist'(Mt,C),_,X,Y),O):-!,convertCycKBToHL(saved(C,Mt,X,Y),O),!.
convertCycKBToHL(save(X,_,_,_),_):-functor(X,F,_),not(atom_concat('#',_,F)),!,fail.


%Fix string args
convertCycKBToHL(saved(Full,Mt,A,C),O):- (memberchk('$stringSplit',C);memberchk('$nartFixed',C)),!,
      transoutString(Full,NoString),!,convertCycKBToHL(saved(NoString,Mt,A,[]),O),!.

transoutString(A,A):- not(compound(A)),!.
transoutString('$stringSplit'([A]),AA):-!,transoutString(A,AA).
transoutString('$nartFixed'(A),AA):-!,transoutString(A,AA).
transoutString('$stringSplit'(A),AA):-!,transoutString(A,AO),flatten(AO,AA),!.
transoutString('#$MeaningInSystemFn'(_, A),A).
 
transoutString([H|T],[HH|TT]):-transoutString(H,HH),transoutString(T,TT),!.
transoutString(Full,A):-Full=..List,transoutString(List,ListO),A=..ListO,!.

% ================================================================
% Some things are compiled outside 
% ================================================================
convertCycKBToHL(saved(Call,Mt,A,C),'%outsideComplied'(Call,Mt)):-isTermDoneOutsideCompiler(Call),!.

isTermDoneOutsideCompiler(Call):-isTermCompiledWithMOTEL(Call),!.

isTermCompiledWithMOTEL('#$implies'(_,O)):-!,isTermCompiledWithMOTEL(O),!.
isTermCompiledWithMOTEL(Call):-functor(Call,F,_),isPredCompiledWithMOTEL(F),!.

isPredCompiledWithMOTEL(A):-not(atom(A)),!,fail.
isPredCompiledWithMOTEL('#$isa').
isPredCompiledWithMOTEL('#$genls').
isPredCompiledWithMOTEL('#$genlPreds').
isPredCompiledWithMOTEL('#$disjointWith').
isPredCompiledWithMOTEL('#$oppositeAttributes').
isPredCompiledWithMOTEL('#$or').
isPredCompiledWithMOTEL('#$and').
isPredCompiledWithMOTEL('#$not').
isPredCompiledWithMOTEL(F):-atom_concat('#$nearest',_,F),!.
isPredCompiledWithMOTEL(F):-atom_concat('#$relation',_,F),!.
isPredCompiledWithMOTEL(F):-atom_concat('#$there',_,F),!.

% ================================================================
% or left decontexulaized   (comments and wordnet (and things with strings?))
% ================================================================
convertCycKBToHL(saved(Call,Mt,A,C),asIs(Call)):-isTermKeptAsIs(Call),!.

isTermKeptAsIs(Call):-functor(Call,F,_),isPredKeptAsIs(F),!.

isPredKeptAsIs(V):-not(atom(V)),!,fail.
isPredKeptAsIs('#$implies'):-!,fail.
isPredKeptAsIs('#$and'):-!,fail.
isPredKeptAsIs('#$or'):-!,fail.
isPredKeptAsIs('#$not'):-!,fail.
isPredKeptAsIs('#$comment').

isPredKeptAsIs(F):-atom_concat('#$wn',_,F),!.

isPredKeptAsIs(F):-saved('#$isa'(F,PredType),_,_,_),isDecontextualizedPredicateType(PredType),!.
isDecontextualizedPredicateType('#$NLSemanticPredicate').
isDecontextualizedPredicateType('#$MetaPredicate').
isDecontextualizedPredicateType('#$MetaRelation').
isDecontextualizedPredicateType('#$InferenceRelatedBookkeepingPredicate').


isPredKeptAsIs(F):-saved('#$argIsa'(F,_,Collection),_,_,_),isCollectionDecontextualized(Collection),!.
isPredKeptAsIs(F):-saved('#$arg1Isa'(F,Collection),_,_,_),isCollectionDecontextualized(Collection),!.
isPredKeptAsIs(F):-saved('#$arg2Isa'(F,Collection),_,_,_),isCollectionDecontextualized(Collection),!.
isPredKeptAsIs(F):-saved('#$arg3Isa'(F,Collection),_,_,_),isCollectionDecontextualized(Collection),!.
 
isCollectionDecontextualized('#$Predicate').
isCollectionDecontextualized('#$Relation').
isCollectionDecontextualized('#$Function').
isCollectionDecontextualized('#$LexicalTerm').
isCollectionDecontextualized('#$CharacterString').
isCollectionDecontextualized('#$Microtheory').
isCollectionDecontextualized('#$Collection').
isCollectionDecontextualized('#$SetOrCollection').
isCollectionDecontextualized('#$EvaluatableRelation').
isCollectionDecontextualized('#$ELNonAtomicTerm-Assertible').
isCollectionDecontextualized('#$CycLUnreifiedReifiableNonAtomicTerm').
isCollectionDecontextualized('#$SpeechPart').
isCollectionDecontextualized('#$LinguisticObjectType').
isCollectionDecontextualized('#$CycSystemString').





% ================================================================
% Implications
% ================================================================
convertCycKBToHL(saved('#$implies'(A,Gafish),Mt,X,Y),
      [(Save:-callProtected(AA,HVsStruct,HVExec,Mt,MtNext)),'#$implies'(A,Gafish,Mt)]):-
	 free_variables(Gafish,HVs),HVsStruct=..['$v'|HVs],
	 varConstraintsVars(HVs,'#$implies'(A,Gafish),HVExec,Rewrite),!,
	 rewriteRuleOnA(HVExec,MtNext,A,AA),!,
	 rewriteLiteral(Mt,'rule',Gafish,HVExec,Save),!.

% ================================================================
% Gafs
% ================================================================
convertCycKBToHL(saved(Gafish,Mt,X,Y),Save):-ground(Gafish),
	 rewriteLiteral(Mt,'gaf',Gafish,true,Save),!.

% ================================================================
% Universals
% ================================================================
convertCycKBToHL(saved(Gafish,Mt,X,Y),(Save:-gafVars(HVsStruct,HVExec))):-
	 free_variables(Gafish,HVs),HVsStruct=..[v|HVs],
	 varConstraintsVars(HVs,Gafish,HVExec,Rewrite),
	 rewriteLiteral(Mt,'univ',Rewrite,HVExec,Save),!.


% ================================================================
% Predicate wrappings
% ================================================================
predicateHash(List,Funct,Pred,Arity,HVExec,HashSecondOrder):-var(Pred),!,
	 varPredicateHash(List,Funct,Pred,Arity,HVExec,HashSecondOrder).

predicateHash(List,Funct,Pred,Arity,HVExec,HashSecondOrder):-
	 makePredArgTypeListCached(Pred,Arity,HashList),!,
	 makeHashAtom(Funct,HashList,HashSecondOrder),
	rememberHLFact(hashOnPred(Pred,HashSecondOrder,HashList)),!.


onlyAtoms([],[]).
onlyAtoms([Hash|ListI],['Funct'|ListO]):-not(atom(Hash)),!,onlyAtoms(ListI,ListO).
onlyAtoms([Hash|ListI],[Hash|ListO]):-onlyAtoms(ListI,ListO).
makeHashAtom(Funct,HashListI,HashSecondOrder):-
	 onlyAtoms(HashListI,HashListO),
	 concat_atom(['%',Funct|HashListO],'',HashSecondOrder),!.


varPredicateHash(List,Funct,Pred,Arity,HVExec,HashSecondOrder):-
      varPredicateHash2(List,HVExec,HashList),
	 makeHashAtom(Funct,HashList,HashSecondOrder),
	rememberHLFact(hashOnPred('?',HashSecondOrder,HashList)).


varPredicateHash2([],HVExec,[]).
varPredicateHash2([H|List],HVExec,[Hash|SecondOrder]):-
      varPredicateHash3(H,HVExec,Hash),
      varPredicateHash2(List,HVExec,SecondOrder).

varPredicateHash3(H,HVExec,Hash):-
      nondet_arg(_,HVExec,varArgIsas(Constraints,Var)),Var==H,!,
      isBestTypeOfListVarPred(Constraints,Hash),!.





isBestTypeOfListVarPred([],'#$Thing'):-!.
isBestTypeOfListVarPred(Constraints,Hash):-isBestTypeOfList(Constraints,Hash).

:-dynamic(predSecondOrderCached/3).

makePredArgTypeListCached(P,N,['Vars']):-var(P),!.
makePredArgTypeListCached(P,N,List):-predSecondOrderCached(P,N,List),!.
makePredArgTypeListCached(P,N,List):-makePredArgTypeList(P,N,List),
	    assert(predSecondOrderCached(P,N,List)).

makePredArgTypeList(Pred,N,['#$Thing']):-N<0,!.
makePredArgTypeList(Pred,1,[Type]):-isArgIsaTop(Pred,1,Type),!.
makePredArgTypeList(Pred,N,HashSecondOrder):-
      isArgIsaTop(Pred,N,Type),NN is N-1,
      makePredArgTypeList(Pred,NN,HashWraps),
      append(HashWraps,[Type],HashSecondOrder),!.

isArity(Pred,Arity):-saved('#$arity'(Pred,Arity),_,_,_),!.
isArity(Pred,2).

isArgIsaTop(Pred,N,Type):-not(atom(Pred)),writeSTDERR(isArgIsaTop(Pred,N,Type)),!,fail.
%isArgIsaTop(Pred,N,Type):-decontextualizedHLFact(argIsaTop(Pred,N,Type)).
%isArgIsaTop(Pred,N,Type):-isArgIsaTop2(Pred,N,Type),rememberHLFact(argIsaTop(Pred,N,Type)).
isArgIsaTop(Pred,N,Type):-findall(Type,isArgIsa(Pred,N,Type),Types),
	 isBestTypeOfList(Types,Type),!.

isBestTypeOfList(Types,Type):-sort(Types,Sorted),isBestTypeOfList2(Sorted,Type),!.
isBestTypeOfList(_,'#$Thing').
      
isBestTypeOfList2([Type],Type):-!.
isBestTypeOfList2(Types,Type):-delete(Types,'#$Thing',Sort),isMostConstrainingClass(Sort,Type),!.


% isMostConstrainingClass(Sort,Type).
isMostConstrainingClass([Class|Rest],Type):-
	 isGenlsCache(Sub,Class),member(Sub,Rest),
	 isMostConstrainingClass(Rest,Type).
isMostConstrainingClass([Class|_],Class).

:-dynamic(writenIsa/2).

rememberIsa('$stringSplit'(_),_):-!.
rememberIsa(E, '#$ComputationalObject'):-atom(E),!.
rememberIsa(E, '#$ComputationalObject'):-functor(E,F,_),isComputationalObjectFunctor(F),!.
rememberIsa(Gaf, '#$ComputationalObject'):-nondet_arg(_,Gaf,E),atom(E),atom_concat(':',_,E),!.

isComputationalObjectFunctor('#$isa').
isComputationalObjectFunctor(F):-isNoBackchain2(F),!.
rememberIsa(E,Type):-atom(E),atom_concat(':',_,E),!.
rememberIsa(E,Type):-rememberHLFact(hashIsa(E,Type)).

rewriteRuleOnA(HVExec,MtNext,X,'$formula'(X)):-var(X),!.
rewriteRuleOnA(HVExec,MtNext,Call,O):-is_list(Call),!,rewriteRuleOnAL(HVExec,MtNext,Call,O).
rewriteRuleOnA(HVExec,MtNext,Call,O):-compound(Call),!,Call=..List,(rewriteRuleOnAL(HVExec,MtNext,List,O);format(user_error,'~q.~n',[rewriteRuleOnAL(List,O)])),!.
rewriteRuleOnA(HVExec,MtNext,X,'$formulaAtomic'(X)).      

rewriteRuleOnAL(HVExec,MtNext,X,'$formula'(X,HVExec,MtNext)):-var(X),!.
rewriteRuleOnAL(HVExec,MtNext,[And|Lits],'$AND'(N,O)):- And=='#$and',!,rewriteRuleOnALS(HVExec,MtNext,Lits,O,N).
rewriteRuleOnAL(HVExec,MtNext,[Pred|List],Save):-
	 length(List,Arity),rewriteLiteral(MtNext,'backcall',Pred,List,Arity,HVExec,Save),!.
     
	 

rewriteRuleOnALS(HVExec,MtNext,[H],O,0):-rewriteRuleOnA(HVExec,MtNext,H,O).
rewriteRuleOnALS(HVExec,MtNext,[H|T],(O1,O2),N):-
	 rewriteRuleOnA(HVExec,MtNext,H,O1),
	 rewriteRuleOnALS(HVExec,MtNext,T,O2,Prevs),
	 N is Prevs + 1.


rewriteLiteral(Mt,HashName,[Pred|List],HVExec,SavedO):-!,
	 rewriteLiteral(Mt,HashName,Pred,List,Arity,HVExec,SavedO),!.
rewriteLiteral(Mt,HashName,Gafish,HVExec,SavedO):-
	 Gafish=..[Pred|List],length(List,Arity),
	 rewriteLiteral(Mt,HashName,Pred,List,Arity,HVExec,SavedO).
	 

rewriteLiteral(Mt,'rule',Pred,List,Arity,HVExec,asIsRule(Save)):-nonvar(Pred),isPredKeptAsIs(Pred),!,Save=..[Pred|List],!.
rewriteLiteral(Mt,HashName,Pred,List,Arity,HVExec,asIsCall(Save)):-nonvar(Pred),isPredKeptAsIs(Pred),!,Save=..[Pred|List],!.
rewriteLiteral(Mt,HashName,Pred,List,Arity,HVExec,outsideCall(Save,Mt)):-nonvar(Pred),isPredCompiledWithMOTEL(Pred),!,Save=..[Pred|List],!.
rewriteLiteral(Mt,HashName,Pred,List,Arity,HVExec,Save):-
	 predicateHash(List,HashName,Pred,Arity,HVExec,HashSecondOrder),
	 append([HashSecondOrder,Pred|List],[Mt],SavedO),Save=..SavedO,!.

% =========================================================
% Deduce var constraints
% =========================================================
varConstraints(Gafish,Cons,Rewrite):-free_variables(Gafish,Vs),
   varConstraintsVars(Vs,Gafish,Cons,Rewrite).
   
varConstraintsVars(Vs,Gafish,Cons,Gafish):-
      varAttributes(Gafish,Vars),
      collectVarConstraints(Vs,Vars,Cons),!.

collectVarConstraints([],Vars,true).
collectVarConstraints([H],VarCons,varArgIsas(Sort,H)):-
      findall(Arg - PredN ,(member(varIsa(Var,Arg,PredN),VarCons),H==Var),List),
      sortConstraints(List,Sort).

collectVarConstraints([H|T],VarCons,(varArgIsas(Sort,H),Cons)):-
      findall(Arg - PredN ,(member(varIsa(Var,Arg,PredN),VarCons),H==Var),List),
      sortConstraints(List,Sort),
      collectVarConstraints(T,VarCons,Cons).

sortConstraints(List,SortO):-sortConstraints2(List,Sort),sort(Sort,SortO),!.

sortConstraints2([],[]):-!.
sortConstraints2([Ignore|T],TT):-
	 ignoreConstraint(Ignore),!,
	 sortConstraints2(T,TT).
sortConstraints2([Before|T],O):-
	 changeConstraintCached(Before,After),!,
	 writeSTDERR(changeConstraint(Before,After)),
	 sortConstraints2(T,TT),
	 append(After,TT,O),!.


ignoreConstraint(1 - '#$isa').
ignoreConstraint(_ - '#$termOfUnit').
ignoreConstraint(1 - '#$thereExists').
ignoreConstraint(1 - '#$forAll').
ignoreConstraint(1-'#$TheSetOf').
ignoreConstraint(_-'#$different').   % [isa('$nartFixed'('#$GroupFn'('#$IntelligentAgent')))]
ignoreConstraint(_ - Pred):-compound(Pred),!,fail.
ignoreConstraint(_ - Pred):-atom_concat(':',_,Pred),!.
ignoreConstraint(1 - Pred):-surfaceInference('#$arg1Isa'(Pred,GT)),isUnboundCollection(GT).
ignoreConstraint(2 - Pred):-surfaceInference('#$arg2Isa'(Pred,GT)),isUnboundCollection(GT).
ignoreConstraint(3 - Pred):-surfaceInference('#$arg3Isa'(Pred,GT)),isUnboundCollection(GT).
ignoreConstraint(Arg - Pred):-surfaceInference('#$argIsa'(Pred,Arg,GT)),isUnboundCollection(GT).

isUnboundCollection('#$Thing').
isUnboundCollection('#$ELVariable').


:-dynamic(isChangeConstraintCached/2).

changeConstraintCached(X,Y):-isChangeConstraintCached(X,Y),!.
changeConstraintCached(X,Y):-changeConstraint(X,Y),asserta(isChangeConstraintCached(X,Y)),!.
changeConstraint(Arg - Pred,[(Collection)]):-surfaceInference('#$argIsa'(Pred,Arg,Collection)).
changeConstraint(1 - Pred,[(Collection)]):-surfaceInference('#$arg1Isa'(Pred,Collection)).
changeConstraint(2 - Pred,[(Collection)]):-surfaceInference('#$arg2Isa'(Pred,Collection)).
changeConstraint(3 - Pred,[(Collection)]):-surfaceInference('#$arg3Isa'(Pred,Collection)).
changeConstraint(Arg - Pred,[genl(Collection)]):-surfaceInference('#$argGenl'(Pred,Arg,Collection)).
changeConstraint(1 - Pred,[genl(Collection)]):-surfaceInference('#$arg1Genl'(Pred,Collection)).
changeConstraint(2 - Pred,[genl(Collection)]):-surfaceInference('#$arg2Genl'(Pred,Collection)).
changeConstraint(3 - Pred,[genl(Collection)]):-surfaceInference('#$arg3Genl'(Pred,Collection)).

changeConstraint(Before,[Before]).
% '#$afterAdding'('#$interArgIsa1-5', 'INTER-ARG-ISA-AFTER-ADDING') 



varAttributes(Var,[varIsa(Var,2,'#$implies')]):-var(Var),!.
varAttributes([P|Args],[]):-!.
varAttributes(Compound,[]):-ground(Compound),!.
varAttributes(Compound,O):-
      Compound=..[P|Args],
      varAttributes(P,1,Args,O).


varAttributes(P,_,[],[]).      
varAttributes('$stringCodes',_,_,[]).      
varAttributes('$stringSplit',_,_,[]).      

%varAttributes('#$isa',1,[E,C],[varIsa(Var,N,P)]).      
varAttributes(P,N,[H|T],[]):-ground([H|T]),!.
varAttributes(P,N,[H|T],[varIsa(H,N,P)|O]):-
   var(H),
   NN is N+1,!,
   varAttributes(P,NN,T,O).

varAttributes(P,N,[H|T],O):-
   varAttributes(H,O1),
   NN is N+1,!,
   varAttributes(P,NN,T,O2),
   append(O1,O2,O),!.

% =========================================================
% Memo var depths
% =========================================================

writeDepthVars([],true).
writeDepthVars([H],check_depth_of_var(H)).
writeDepthVars([H|T],(check_depth_of_var(H),O)):-writeDepthVars(T,O),!.




assertzRE(X):-assertz(X),writeSTDERR(assertzRE(X)).
assertaRE(X):-asserta(X),writeSTDERR(assertaRE(X)).
	  
/*
Rule Matrix Examples:


Example:

 (=>
          (instance ?OBJ Object) 
          (exists
             (?TIME1 ?TIME2) 
             (and
                (instance ?TIME1 TimePoint) 
                (instance ?TIME2 TimePoint) 
                (before ?TIME1 ?TIME2) 
                (forall
                   (?TIME) 
                   (=>
                      (and
                         (beforeOrEqual ?TIME1 ?TIME) 
                         (beforeOrEqual ?TIME ?TIME2) ) 
                      (time ?OBJ ?TIME) ) ) ) ) )
		      

  (=>
          (instance ?FUNCTION UnaryConstantFunctionQuantity) 
          (and
             (domain ?FUNCTION 1 ConstantQuantity) 
             (range ?FUNCTION ConstantQuantity) ) )
	     


 (=>
          (and
             (domain ?REL1 ?NUMBER ?CLASS1) 
             (domain ?REL2 ?NUMBER ?CLASS2) 
             (disjoint ?CLASS1 ?CLASS2) ) 
          (disjointRelation ?REL1 ?REL2) )


 (=>
          (and
             (subrelation ?PRED1 ?PRED2) 
             (domain ?PRED2 ?NUMBER ?CLASS2) 
             (domain ?PRED1 ?NUMBER ?CLASS1) ) 
          (subclass ?CLASS1 ?CLASS2) )

Items:
 
	Varible clasification:
	
	Term classes
	
	Term:  HV(n) SV(n) DV(n)
	
	a(H1,H2):-
		v(H1,v(S1)),v(H2,v(S2,v(DV,_))),
		b(H1,S1),
		c(H2,S1),
		d(S2,D1),
		e(D2).


	a(H1,H2):-
		b(H1,S1),
		c(H2,S1),
		d(S2,D1),
		e(D2).
		
	H1-1
	H2-1
	S1-2
	D1-3
	D2-Infinate
	
	   cost ordered
	(H1,H2),(S1),D1
	$2(	
	
	----------
	  |	  |
	H1-S1-H2
	      |
	     D1	   D2
	
	
	headvar plus a variables shared with another headvar

	headvar plus a variables shared with another headvar
	
	

	HeadSlots
	
	Universal in term with no head getPrologVars  

 
*/

% :-include('logicmoo_utils_header.pl').


isMooReadyForEdits:-isContextCompilerProcess(Context,Progress),!,write_ln(false).
isMooReadyForEdits:-write_ln(true).


isUncanonicalized(Context):-fail.	%TODO
isSourceNewerThenImage(Context):-fail.	%TODO
isKnowledgeBaseLoaded(Context,Ctx):-mooCache(_, _, _,_,Context, _, _, _),!.
isTheoryUntransfered(Context):-not(isKnowledgeBaseLoaded(Context,_)).
isTheoryLoading(Context,Status):-isMooProcess(ID,loadContextfromSource(Context,GlobalContext)),!.

isContextCompilerProcess(Context,LastPercent):-isMooProcess(ID,canonicalizeMooContextHTML(Context,GlobalContext)),!,
			flag('$last_written_percent',LastPercent,LastPercent).

isContextCurrentlyInUse(Context,unloaded):-nonvar(Context),isTheoryUntransfered(Context),!,fail.
isContextCurrentlyInUse(Context,canonicalizing(Progress)):-isContextCompilerProcess(Context,Progress),!.
isContextCurrentlyInUse(Context,loading(Status)):-isTheoryLoading(Context,Status),!.
isContextCurrentlyInUse(Context,_):-!,fail.

invokeContextCompilerProcess(Context):-!.
invokeContextCompilerProcess(Context):-not(isUncanonicalized(Context)),!.
invokeContextCompilerProcess(Context):-
	servantProcessCreate(canonicalizeMooContextHTML(Context,GlobalContext),
	canonicalizeMooContextHTML(Context,GlobalContext),ID,[detatched(true)]),!.

% =================================================
% Divide Fact and Rules	(Mine logic and create explaination node)
% =================================================
testrcan:-recanonicalizeMooContext('LogicMOO').

setOperationLock(Obj,Locker):-!.
setOperationUnlock(Obj,Locker):-!.



expireTN(Context,TN):-
	logOnFailureIgnore(retractallSpecial(mooCache(Literal,_,Context,TN))),  %Facts
	logOnFailureIgnore(retractallSpecial(mooCache(Literal,AnteLiteral,_,Context,TN))).

changeSurfaceStatusX(Context,TN,Before,After):-
	expireTN(Context,TN),!,
	changeSurfaceStatus(Context,TN,Before,After).

changeSurfaceStatus(Context,TN,Before,After):-
	retract(mooCache(Surface,CAN,Flags,Vars,Context,TN,Maintainer,Before)),
	assertzRE(mooCache(Surface,CAN,Flags,Vars,Context,TN,Maintainer,After)),fail.
changeSurfaceStatus(Context,TN,Before,After):-!.
		


canonicalizeMooContextHTML(Context):-!.
canonicalizeMooContextHTML(Context):-
	tell('errors.html'),
	saveMooCache,
	logOnFailure(canonicalizeMooContext(Context)),
	saveMooCache,
	told.
canonicalizeMooContextHTML(Context):-!.


:-dynamic(canonicalizerWarnings(Context,_,_)).

clearCanonicalizerWarnings(Context):-retractallSpecial(canonicalizerWarnings(Context,_,_)),!.

sendCanonicalizerWarning(Warning,Data,Surface,Rule,CLID,Flags,KRVars,Context,TN,Anontate,Matrix,
	[(canonicalizerWarning(Warning,Data):-true)]):-
       % writeq(Warning:Data),nl,
       \+ ( \+ (canonicalizerWarnings(Context,Warning,Data)))  -> ifInteractive(write(','));
	(assertzRE(canonicalizerWarnings(Context,Warning,Data)),
	ifInteractive(writeObject(nv([nl,Warning,nl,writeq(Data),nl,Surface,nl,nl]),KRVars))),!.



rct:-canonicalizeMooContextReal('LogicMOO').

canonicalizeMooContext(Context):-!.
canonicalizeMooContextReal(Context):-
	clearCanonicalizerWarnings(Context),
	writeDebug(starting(canonicalizeMooContext(Context))),
	flag(explaination_id,_,1),
	recanonicalizeTN(Context,TN),
	writeDebug(done(canonicalizeMooContext(Context))),!.


recanonicalizeTN(Context,TN):-
	(retractallSpecial(mooCache(Literal,_,Context,TN))),  %Facts
	(retractallSpecial(mooCache(Literal,AnteLiteral,_,Context,TN))),	 %Rules 
	fail.
	
recanonicalizeTN(Context,TN):-
	flag('$sofar',_,1),
	countAssertions(mooCache(_,_,_,_,Context,TN,_,_),Total),
	ifInteractive((say('\Compiling ~w surface clauses in ~w\n',[Total,Context]),writePercentAndTimeReset)),
	ignore((mooCache(Surface,CAN,Flags,Vars,Context,TN,Maintainer,Result),  
	once((
	%	numbervars((Surface,CAN,Flags,Vars,Context,TN,Maintainer,Result),'$VAR',0,_),!,
		canonicalizeClause(Surface,CAN,Flags,Vars,Context,TN,Maintainer,Result,AssertList),
		assertAll(AssertList),
	        ifInteractive((
			flag('$sofar',SoFar,SoFar+1),
			writePercentAndTime('\n ~1f% complete.  Estimated cpu "priority" seconds remaining: ~1f\n ',SoFar,Total,8)
			))
		)),fail)),
	ifInteractive((
		flag(explaination_id,ExplainationID,ExplainationID),
		countAssertions(mooCache(_,_,Context,TN),Facts),
		countAssertions(mooCache(_,_,_,Context,TN),Rules),
		say('\n100% complete.  Examined ~w internal lemma structures for ~w. Facts:~w  Rules:~w  \n',[ExplainationID,Context,Facts,Rules]))),!.
	
writePercentAndTimeReset:-
	((flag('$last_written_percent',_,1),getCputime(Now),!,flag('$cputime_percent_start',_,Now))).
	
writePercentAndTime(Format,SoFar,Total,Steps):-
        ((
		flag('$last_written_percent',LastPercent,LastPercent),
	        NewPercent is (SoFar/Total * 100),
		NextPercent is LastPercent + Steps, 
		writePercentAndTime(Format,SoFar,Total,NextPercent,NewPercent))).
		
writePercentAndTime(Format,SoFar,Total,NextPercent,NewPercent):- NextPercent > NewPercent,!,write(.).
writePercentAndTime(Format,SoFar,Total,NextPercent,NewPercent):-
				flag('$last_written_percent',_,NewPercent),
				getCputime(Now),flag('$cputime_percent_start',Start,Start),
				TimeLeft is (Total-SoFar)*2*((Now-Start)/SoFar),
				say(Format,[NewPercent,TimeLeft]),!.
				



ifInteractive(G):-ignore((thread_self(Id),(number(Id);(G,flush_output)),!)).

                                    
recanonicalizeTN(Context,TN):-!.
	
deduceLegalToCan(Surface,_):-
	once(getConstants(atomic,Surface,Consts,_,_)),
	member(Word,Consts),
	deduceDisabledSurfaceConst(Word),!,fail.

deduceLegalToCan(Surface,_):-!.
	
:-dynamic(deduceDisabledSurfaceConst/1).

%deduceDisabledSurfaceConst('graphPart'). deduceDisabledSurfaceConst('NormativeAttribute').

% In Moo DB we have 3 types of Vars
% var(Var)
% Var='$VAR'(_).
% Var=..['$existential',V|Body],var(V)

% isSlot(X):-var(Var).


% ================================================================
% Convert Forms to AssertionList
% ================================================================
canonicalizeClause(Surface,true,Flags,Vars,Context,TN,Maintainer,Result,[]):-!.
canonicalizeClause(Surface,not(true),Flags,Vars,Context,TN,Maintainer,Result,[]):-!.
canonicalizeClause(Surface,Var,Flags,Vars,Context,TN,Maintainer,Result,[]):-isSlot(Var),!.

% Context |= CAN1 & CAN2
canonicalizeClause(Surface,and(CAN1,CAN2),Flags,Vars,Context,TN,Maintainer,Result,AssertionList):-!,
	(canonicalizeClause(Surface,CAN1,Flags,Vars,Context,TN,Maintainer,Result,AssertList1)),
	(canonicalizeClause(Surface,CAN2,Flags,Vars,Context,TN,Maintainer,Result,AssertList2)),!,
	append(AssertList1,AssertList2,AssertionList).     

canonicalizeClause(Surface,CAN,Flags,Vars,Context,TN,Maintainer,Result,AssertList):-
	unnumbervars((Surface,CAN,Flags,Vars,Context,TN,Maintainer,Result),
			   (USurface,UCAN,UFlags,UVars,UContext,UCtx,UTN,UMaintainer,UResult)),
	canonicalizeRule(USurface,UCAN,UFlags,UVars,UContext,UCtx,UTN,UMaintainer,UResult,AssertList),!.

      
% =============================================================

ruleToPrologHeadBodyList(entails(Ante,Cons),Cons,Ante):-!.	
ruleToPrologHeadBodyList(Cons,Cons,true).      
	
% =============================================================
	
canonicalizeRule(Surface,Rule,Flags,KRVars,Context,TN,Maintainer,Result,Assertions):-!,
	ruleToPrologHeadBodyList(Rule,Cons,Ante),	
	conjunctsToList(Ante,NewAnteLA),!,
	%removeCompilables(Cons,NewAnteLA,NewAnteLA,NewAnteRM),!,
	removeCompilables(Cons,NewAnteLA,NewAnteLA,NewAnteR),!,
	canonicalizeLemme(Surface,Rule,Cons,Ante,NewAnteR,Flags,KRVars,Context,TN,Maintainer,Result,Assertions),!.


% =============================================================

deNeg(not Item,Item):-!.  deNeg(Item,Item).

% =============================================================


canonicalizeLemme(Surface,Rule,Cons,Ante,NewAnteR,Flags,KRVars,Context,TN,Maintainer,Result,[]):-
	member(Item,NewAnteR), deNeg(Item,NItem), nonvar(NItem),
	functor(NItem,F,_),(memberchkSpecial(F,['$taut',request])),
	ifInteractive(writeObject(implied(Rule),KRVars)),!.


% =============================================================

canonicalizeLemme(Surface,Rule,Cons,Ante,NewAnteR,Flags,KRVars,Context,TN,Maintainer,Result,Assertions):-
	getPrologVars(Cons,HeadSlots,_,_),
	adjustSkolemOrder(HeadSlots,NewAnteR,NewAnteL),!,
	nonWrappedCan(HardCoded),
	convertToHolds([holds|HardCoded],Cons,EntailedHead),!,
	convertBodyToHolds(NewAnteL,ConditionalBody),!,
	selectSign(EntailedHead,Sign),!,
	copy_term(EntailedHead,USeed),
	convertNegations(lit,USeed,ProtoType,_),
	numbervars(ProtoType,('$'),0,_),
	selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,EntailedHead,ConditionalBody,Matrix),!,
	flag(clause_id,CLID,CLID+1),
	setMooOption(Agent,putAttributeStructures,(Surface,Rule,CLID,Flags,KRVars,Context,Ctx,TN)),!,
	catch(putAttributeStructures(Context,Flags,Matrix,EmbededArgs),
		mooException(argDomains,Type,Details,DebugOnError),
		(set_prolog_flag(debug_on_error, DebugOnError),
		sendCanonicalizerWarning(Type,Details,Surface,Rule,CLID,Flags,KRVars,
								Context,TN,Ctx,Matrix,EmbededArgs))),!,
	convertNegations(lit,EmbededArgs,LiteralMatrix,_),
	createFunctionalClauses(':'(Rule,KRVars),LiteralMatrix,Clauses),
	mergeClauses(ProtoType,Clauses,Rule,CLID,Flags,KRVars,Context,TN,Assertions),!.


% =============================================================

adjustSkolemOrder(HeadSlots,[],[]):-!.
adjustSkolemOrder(HeadSlots,['$existential'(Var,Name,Form)|Rest],[VT|RestPlus]):-
	once((getPrologVars(Form,FormVars,_,_),set_partition(FormVars,HeadSlots,_,_,MustBeGround))),
	MustBeGround=[_|_], VT=..['$groundVars'|MustBeGround],!,
	adjustSkolemOrder(HeadSlots,Rest,RestS),
	append(RestS,['$existential'(Var,Name,Form)],RestPlus),!.
adjustSkolemOrder(HeadSlots,Other,Other):-!.

% =============================================================

mergeClauses(ProtoType,[],KR,CLID,Flags,KRVars,Context,TN,Assertions).
mergeClauses(ProtoType,[Matrix|Rest],KR,CLID,Flags,KRVars,Context,TN,Assertions):-!,
	getAssertMatrix(ProtoType,Matrix,KR,CLID,KRVars,Context,TN,HL),!,
	mergeClauses(ProtoType,Rest,KR,CLID,Flags,KRVars,Context,TN,SubAssertions),
	append(HL,SubAssertions,Assertions),!.

% =============================================================

getAssertMatrix(ProtoType,(C:-true),KR,CLID,KRVars,Context,TN,[mooCache(C,(ExplainationID,KRVars,KR),Context,TN)]):-
	flag(explaination_id,ExplainationID,ExplainationID+1).

getAssertMatrix(ProtoType,(C:-A),KR,CLID,KRVars,Context,TN,[mooCache(C,A,(ExplainationID,KRVars,KR),Context,TN)]):-
	flag(explaination_id,ExplainationID,ExplainationID+1).
getAssertMatrix(ProtoType,C,KR,CLID,KRVars,Context,TN,[mooCache(C,(ExplainationID,KRVars,KR),Context,TN)]):-
	flag(explaination_id,ExplainationID,ExplainationID+1).
		 
% =============================================================

createFunctionalClauses(Rule,[],[]).
createFunctionalClauses(Rule,[(C:-true)|More],[C|Clauses]):-!,
	createFunctionalClauses(Rule,More,Clauses).
createFunctionalClauses(Rule,[(C:-A)|More],Clauses):-!,
	createFunctionalClauses(Rule,(C:-A),SomeClauses),!,
	createFunctionalClauses(Rule,More,MoreClauses),
	append(SomeClauses,MoreClauses,Clauses),!.
createFunctionalClauses(Rule,[C|More],Clauses):-!,
	createFunctionalClauses(Rule,C,SomeClauses),!,
	createFunctionalClauses(Rule,More,MoreClauses),
	append(SomeClauses,MoreClauses,Clauses),!.

/*
createFunctionalClauses(Rule,C:-A,[C:-Body]):-
	getMostGeneralSubsumption(C,A,TF,UnsignedC,Variant),Variant=[_|_],!,
	%copy_term(Variant,Copy),numbervars(Copy,'$',0,_),
       %  nl,writeq(Rule),nl,writeq(Variant),nl,nl,
	Body=..[TF,A,Variant].
*/

%createFunctionalClauses(Rule,C:-A,[C:-Body]):-!,writeq(A),
	
createFunctionalClauses(Rule,C,[C]).


% =============================================================

getMostGeneralSubsumption((C), Term,'table_',C,VL):- !,
	findall(Variant,getMostGeneralSubsumptionI((C), Term,(Variant)),VL),!.

/*
getMostGeneralSubsumption(not C, Term,'f_t',C,Variant):-!,
	getMostGeneralSubsumptionI((C), (Term),Variant).

getMostGeneralSubsumption(C, Term,'t_f',C,Variant):-
	getMostGeneralSubsumptionI(not(C), Term,not(Variant)),!.
*/
getMostGeneralSubsumptionI(X, Term,_):-not(compound(Term)),!,fail.
%getMostGeneralSubsumptionI(X, Term,_):-functor(Term,F,_),atom_concat('$',_,F),!,fail.
getMostGeneralSubsumptionI(X, Term,Term) :-%functor(X,F,Arity),functor(Term,F,Arity),!,
				not(not((X=Term))),!.
getMostGeneralSubsumptionI(X, Term,Variant) :-
	arg(_, Term, Arg),
	getMostGeneralSubsumptionI(X, Arg,Variant).

	
% =============================================================

nonWrappedCan([holdsDuring,entails,'include-context',instance,request,'$existential',false,true,domain,equal,subclass,subrelation,disjointDecomposition]).

% =============================================================

removeCompilables(Cons,Rule,[],[]).
removeCompilables(Cons,Rule,[EQ|AnteLA],AnteL):-Cons==EQ,!,
	removeCompilables(Cons,Rule,AnteLA,AnteL),!.
removeCompilables(Cons,Rule,[New|AnteLA],[New|AnteL]):-!,
	removeCompilables(Cons,Rule,AnteLA,AnteL),!.

/*
removeCompilables(Cons,Rule,[instance(Var,Atom)|AnteLA],AnteL):-isSlot(Var),atom(Atom),!,
	removeCompilables(Cons,Rule,AnteLA,AnteL).
removeCompilables(Cons,Rule,[instance(Var,'$Class'(Atom,Fixed))|AnteLA],AnteL):-isSlot(Var),atom(Atom),!,
	removeCompilables(Cons,Rule,AnteLA,AnteL),!.
removeCompilables(Cons,Rule,[subclass(Var1,Var2)|AnteLA],AnteL):-unify_with_occurs_check(Var1,Var2),!,
	removeCompilables(Cons,Rule,AnteLA,AnteL),!.
removeCompilables(Cons,Rule,[domain(Pred,N,Class)|AnteLA],['$grDomain'(Pred,N,Class)|AnteL]):-
	removeCompilables(Cons,Rule,AnteLA,AnteL),!.
*/

% =============================================================

mapOnConj(Goal,LogAnte,ConditionalBody):-var(LogAnte),var(ConditionalBody),!,
	Goal=..[F|Args],append([F|Args],[LogAnte,ConditionalBody],CallL),
	Call=..CallL,!,call(Call).
mapOnConj(Goal,[LogAnte],[ConditionalBody]):-var(LogAnte),var(ConditionalBody),!,
	Goal=..[F|Args],append([F|Args],[LogAnte,ConditionalBody],CallL),
	Call=..CallL,!,call(Call).

mapOnConj(_,[],[]):-!.
mapOnConj(Goal,[Log|Ante],[Conditional|Body]):-!,
	mapOnConj(Goal,Log,Conditional),
	mapOnConj(Goal,Ante,Body).
mapOnConj(Goal,LogAnte,ConditionalBody):-nonvar(LogAnte),
	compound(LogAnte),LogAnte=..[Connective,L|IST],isBodyConnective(Connective),!,
	mapOnConj(Goal,[L|IST],ConditionalBodyList),
	ConditionalBody=..[Connective|ConditionalBodyList],!.
mapOnConj(Goal,LogAnte,ConditionalBody):-nonvar(ConditionalBody),
	compound(ConditionalBody),ConditionalBody=..[Connective,L|IST],isBodyConnective(Connective),!,
	mapOnConj(Goal,LogAnteL,[L|IST]),
	LogAnte=..[Connective|LogAnteL],!.
mapOnConj(Goal,LogAnte,ConditionalBody):-!,
	Goal=..[F|Args],append([F|Args],[LogAnte,ConditionalBody],CallL),
	Call=..CallL,
	call(Call).
	
% =============================================================

convertBodyToHolds(LogAnte,ConditionalBody):-!,
	((convertNegations(not,LogAnte,NottedLogAnte,_),
	nonWrappedCan(Nowrap),
	convertToHolds([holds|Nowrap],NottedLogAnte,ConditionalBody))).

convertToHolds(Flags,Mid,Term):-
	%
	(mapOnConj(convertToHoldsProp(Flags),Mid,Term)),!.
	
convertToHoldsProp(Flags,(Term),(Term)):-isSlot(Term),!.
convertToHoldsProp(Flags,not(Mid),not(Term)):-!,convertToHolds(Flags,Mid,Term).
convertToHoldsProp(Flags,Mid,'$eval'(Term)):-Mid=..[F|Args],atom(F),atom_concat(_,'Fn',F),!,Term=..[F|Args].
convertToHoldsProp(Flags,(Term),(Term)):-functor(Term,F,_),atom_concat('$',_,F),!.

convertToHoldsProp(Flags,Mid,(Term)):-Mid=..[holds,F|Args],atom(F),atom_concat(_,'Fn',F),!,Term=..[function,F|Args].
convertToHoldsProp([_|Flags],Mid,Term):-Mid	=..[F|Args],memberchkSpecial(F,Flags),!,Term=..[F|Args].
convertToHoldsProp(Flags,Mid,Mid):-not(not(memberchkSpecial(Mid,Flags))),!.
convertToHoldsProp([orig|_],Mid,Mid):-!.
convertToHoldsProp([Wrap|_],Mid,Term):-Mid=..[Wrap,F|Args],atom(F),!,Term=..[F|Args],!.
convertToHoldsProp([Wrap|_],Term,Term):-!.

% =============================================================

containsSkolems(Flags,EntailedHeadLiteral):-
	((getPrologVars(EntailedHeadLiteral,Vars,_,_),!,member(Each,Vars),member(replaceConsVar(Each,_),Flags),!)).

	
selectBestRuleMatrix(Sign,HeadSlots,Flags,_,EntailedHead,[],[ (EntailedHead:-guarded_(EntailedHead)) ]):-!.
selectBestRuleMatrix(Sign,HeadSlots,Flags,_,EntailedHead,[true],[EntailedHead]):-!.
selectBestRuleMatrix(Sign,HeadSlots,Flags,[],EntailedHead,Body,[ (EntailedHead:-novars_(Body)) ]):-!.
selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,EntailedHead,ConditionalBody,[( EntailedHead:-NewBodListGuarded) ]):-
	getPrologVars(EntailedHead:ConditionalBody,AllVars,_,_),
	getPrologVars(ConditionalBody,BodyVars,BodyVarsSingleMaybeHead,BodyVarsConnectedAndMaybeHead),
	set_partition(HeadSlots,BodyVars,PrivateHead,PrivateBody,AllSharedHB),!,
	set_partition(HeadSlots,BodyVarsConnectedAndMaybeHead,UsedMoreThenOnceHeads,BodyOnlyConected,SplitHeadVar),!,
	set_partition(HeadSlots,BodyVarsSingleMaybeHead,UsedOnceHeads,UniversalBody,HeadSlotsSingleInBody),!,
	reorderAnteListAddPropositionalMechanisms(Sign,ConditionalBody,PrivateHead,PrivateBody,UniversalBody,HeadSlotsSingleInBody,BodyOnlyConected,SplitHeadVar,NewBodListWrappedL),!,
	sort(NewBodListWrappedL,NewBodListWrappedS),
	list_to_comma(NewBodListWrappedS,BodListWrappedC),
	Inner =.. [Sign,BodListWrappedC,PrivateHead,PrivateBody],
	selectBestBodyMechanism(Inner,HeadSlots,BodyOnlyConected,UniversalBody,NewBodListGuarded),!.
	%VarInfClause =.. ['$v'],
	%NewBodListGuarded=..[Major,BodListWrappedC|VarINF].
	%prologPartitionList(NewBodListWrappedS,Item,sharedVars(Item,HeadSlots),Shared,Unshared),
	%list_to_comma(Shared,SharedT),
	%list_to_comma(Unshared,UnsharedT),
	%selectRule(Sign,ConjPosNegFA,DiscPosNegFA,NewBodListGuarded),!.%true.*/
	

% =============================================================
%			  uVar			  Hv				Bv		     CH
% =============================================================

intersectionMath(Start,Scale,L1,L2,L12,New):-set_partition(L1,L2,_,_,L12),!,
	length(L12,Length),!,New is (Length * Scale) + Start.

reorderAnteListAddPropositionalMechanisms(Sign,List,HeadSlots,BodyVars,UniversalBody,HeadSlotsSingleInBody,BodyOnlyConected,SplitHeadVar,Result):-
	reorderAnteListAddPropositionalMechanismEach(Sign,List,HeadSlots,BodyVars,UniversalBody,HeadSlotsSingleInBody,BodyOnlyConected,SplitHeadVar,KeyedResult),!,
	keysort(KeyedResult,SortedResult),%true,
	removeKeys(SortedResult,Result),!.
	
removeKeys([],[]).
removeKeys([K-I|KRest],[I|Rest]):-
	  removeKeys(KRest,Rest),!.
removeKeys([I|KRest],[I|Rest]):-
	  removeKeys(KRest,Rest),!.
	  

reorderAnteListAddPropositionalMechanismEach(Sign,[],HeadSlots,BodyVars,UniversalBody,HeadSlotsSingleInBody,BodyOnlyConected,SplitHeadVar,[]).
reorderAnteListAddPropositionalMechanismEach(Sign,[Conditional|Body],HeadSlots,BodyVars,UniversalBody,HeadSlotsSingleInBody,BodyOnlyConected,SplitHeadVar,[Cost-ConditionalClass|BodyList]):-
	getPrologVars(Conditional,CVars,_,_),
	intersectionMath(0,1000,UniversalBody,CVars,CUniversalBody,Cost1), %U Irealivant
	intersectionMath(Cost1,100,BodyOnlyConected,CVars,CBodyOnlyConected,Cost2), %X Transfer
        intersectionMath(Cost2,10,HeadSlotsSingleInBody,CVars,CHeadSlotsSingleInBody,Cost3), %H  Head
        intersectionMath(Cost3,1,SplitHeadVar,CVars,CSplitHeadVar,Cost), %S split head over cluases
	selectBestPropositionMechanism(Sign,HeadSlots,BodyVars,Conditional,ConditionalClass,CUniversalBody,CBodyOnlyConected,CHeadSlotsSingleInBody,CSplitHeadVar),
	reorderAnteListAddPropositionalMechanismEach(Sign,Body,HeadSlots,BodyVars,UniversalBody,HeadSlotsSingleInBody,BodyOnlyConected,SplitHeadVar,BodyList).	

%CUniversalBody,CBodyOnlyConected,CHeadSlotsSingleInBody,CSplitHeadVar
selectBestPropositionMechanism(Sign,HeadSlots,BodyVars,Conditional,ConditionalClass,
	CUniversalBody,CBodyOnlyConected,CHeadSlotsSingleInBody,CSplitHeadVar):-!,
	ConditionalClass=..[Sign,Conditional,CUniversalBody,CBodyOnlyConected,CHeadSlotsSingleInBody,CSplitHeadVar].
	
			/*

% Works but is unused now
selectBestPropositionMechanism(Sign,Conditional,CVars,HeadSlots,ConditionalClass,
			CUniversalBody,CBodyOnlyConected,CHeadSlots):-
	appendFunctInf(''(Conditional),'U',CUniversalBody,Conditional1),!,
	appendFunctInf(Conditional1,'B',CBodyOnlyConected,Conditional2),!,
	appendFunctInf(Conditional2,'H',CHeadSlots,ConditionalClassEF),!,
	removeEmptyFunct(Sign,ConditionalClassEF,ConditionalClass).
			  */
selectBestBodyMechanism(Seed,HeadSlots,BodyOnlyConected,UniversalBody,VarClause):-!,
	appendFunctInf(Seed,'Head',HeadSlots,Next1),	
	appendFunctInf(Next1,'Bodyc',BodyOnlyConected,Next2),
	appendFunctInf(Next2,'Univb',UniversalBody,VarClauseEF),!,	
	removeEmptyFunct(VarClauseEF,VarClause).


removeEmptyFunct(''(VarClause),VarClause):-!.
removeEmptyFunct(VarClause,VarClause):-!.

removeEmptyFunct(_,''(VarClause),VarClause):-!.
removeEmptyFunct(Sign,VarClause,VarClauseO):-
	VarClause=..[BF|Args],atom_concat(Sign,BF,New),
	VarClauseO=..[New|Args].
	
% =============================================================

appendFunctInf(In,_,[],In).
appendFunctInf(In,F,VarList,Out):-
	In=..[PF|Rest],
	atom_concat(PF,F,NF),
	append(Rest,[VarList],OArgs),
	Out=..[NF|OArgs],!.

appendFunct(In,_,[],In).
appendFunct(In,F,_,Out):-
	In=..[PF|Rest],
	atom_concat(PF,F,NF),
	Out=..[NF|Rest],!.

appendFunctV(In,_,[],In).
appendFunctV(In,F,VL,Out):-
	VC=..['$v'|VL],
	In=..[PF|Rest],
	atom_concat(PF,F,NF),
	append(Rest,[VC],NewL),!,
	Out=..[NF|NewL],!.

       
% =============================================================

wFrame:-
	prolog_current_frame(Frame),!,
	prolog_frame_attribute(Frame, goal,Value),!,
	writeq(Value),nl.
     
	
fc_cmp(not X,not(Y),'f-f'):-!,too_eq(X,Y),!. 
fc_cmp(not X, Y,'f-t'):-!,too_eq(X,Y),!.
fc_cmp( X,not Y,'t-f'):-!,too_eq(X,Y),!.
fc_cmp( X, Y,'t-t'):-too_eq(X,Y),!.
	
too_eq(X,Y):-compareVariant(X,Y,U,Dif),!,
	nonvar(U), (Dif < (1)).
			   

selectSign(not NHead,'f~'):-!.
selectSign(NHead ,'t~'):-!.


	
selectRule(Sign,[],[],Sign).

selectRule(Sign,[],DiscPosNeg,Body):-
	DiscPosNeg=..[F|PosNegL],
	concat_atom([Sign,F,'_'],RH),!,
	Body=..[RH|PosNegL].

selectRule(Sign,ConjPosNeg,[],Body):-
	ConjPosNeg=..[F|PosNegL],
	concat_atom([Sign,F,'_'],RH),
	Body=..[RH|PosNegL],!.

selectRule(Sign,ConjPosNegFA,DiscPosNegFA,Body):-
	ConjPosNegFA=..[C|OnjPosNegFAL],
	DiscPosNegFA=..[D|IsjPosNegFAL],
	concat_atom([Sign,C,D,'_'],RH),
	append(OnjPosNegFAL,IsjPosNegFAL,BArgs),!,
	Body=..[RH|BArgs],!.
	
				
selectConjPosNegFA(AllSharedHB,FindPos,FindNeg,ConjPosNegFA):-
	selectConjPosNeg(FindPos,FindNeg,ConjPosNeg),
	selectConjPosNegVars(AllSharedHB,ConjPosNeg,ConjPosNegFA),!.


selectConjPosNeg([],[],[]).
selectConjPosNeg([],FindNeg,'CNeg'(FindNegC)):-list_to_comma(FindNeg,FindNegC).
selectConjPosNeg(FindPos,[],'CPos'(FindPosC)):-list_to_comma(FindPos,FindPosC).
selectConjPosNeg(FindPos,FindNeg,'Con'(FindPosC,FindNegC)):-
	list_to_comma(FindNeg,FindNegC),list_to_comma(FindPos,FindPosC),!.

selectConjPosNegVars([],ConjPosNeg,ConjPosNeg).
selectConjPosNegVars(AllSharedHB,[],'CVars'(Shared)):-Shared=..['$v'|AllSharedHB].
selectConjPosNegVars(AllSharedHB,ConjPosNeg,ConjPosNegFA):-
	Shared=..['$v'|AllSharedHB],
	ConjPosNeg=..[F|Args],
	ConjPosNegFA=..[F,Shared|Args].


selectDiscPosNegFA(AllSharedHB,FindPos,FindNeg,DiscPosNegFA):-
	selectDiscPosNeg(FindPos,FindNeg,DiscPosNeg),
	selectDiscPosNegVars(AllSharedHB,DiscPosNeg,DiscPosNegFA),!.

selectDiscPosNeg([],[],[]).
selectDiscPosNeg([],FindNeg,'DNeg'(FindNegC)):-list_to_comma(FindNeg,FindNegC).
selectDiscPosNeg(FindPos,[],'DPos'(FindPosC)):-list_to_comma(FindPos,FindPosC).
selectDiscPosNeg(FindPos,FindNeg,'Dis'(FindPosC,FindNegC)):-list_to_comma(FindNeg,FindNegC),list_to_comma(FindPos,FindPosC).

selectDiscPosNegVars([],DiscPosNeg,DiscPosNeg).
selectDiscPosNegVars(AllSharedHB,[],'DVars'(Shared)):-Shared=..['$v'|AllSharedHB].
selectDiscPosNegVars(AllSharedHB,DiscPosNeg,DiscPosNegFA):-
	Shared=..['$v'|AllSharedHB],
	DiscPosNeg=..[F|Args],
	DiscPosNegFA=..[F,Shared|Args].

                                                         

/*
replaceConsVar(xB, '$existential'([63, 84, 73, 77, 69, 50], exists(xC, and(instance(xC, 'TimePoint'), and(instance([63, 84, 73, 77, 69, 50], 'TimePoint'), and(before(xC, [63, 84, 73, 77, 69, 50]), forall(D, =>(and(beforeOrEqual(xC, D), beforeOrEqual(D, [63, 84, 73, 77, 69, 50])), time(A, D))))))))), 
replaceConsVar(xC, '$existential'([63, 84, 73, 77, 69, 49], and(instance([63, 84, 73, 77, 69, 49], 'TimePoint'), and(instance(xB, 'TimePoint'), and(before([63, 84, 73, 77, 69, 49], xB), forall(D, =>(and(beforeOrEqual([63, 84, 73, 77, 69, 49], D), beforeOrEqual(D, xB)), time(A, D))))))))], 

entails(not(instance(xC, 'TimePoint')), not(instance(A, 'Object')))
   
Prolog:  impossible(instance(_, 'Object')) :- \+ searchable(instance(_, 'TimePoint'))


entails(not(instance(xB, 'TimePoint')), not(instance(A, 'Object'))) 

Prolog: impossible(instance(_, 'Object')) :- impossible(instance(_, 'TimePoint'))


entails(not(before(xC, xB)), not(instance(A, 'Object'))), 

Prolog: impossible(instance(_, 'Object')) :- impossible(before(_,_))
 


*/

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,EntailedHead,
		SharedPos,UnsharedPos,
		SharedNeg,UnsharedNeg,
	[(impossible(EntailedHead) :- impossible(Negs))],
		HeadSlots,BodyVars,VAllShare,SharedPosV,SharedNegV,
		PrivateHead,PrivateBody,UnsharedPos,UnsharedNeg):-!.

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,[],[not(Negs)],not(EntailedHead),
	[(impossible(EntailedHead) :- impossible(Negs))],
		HeadSlots,BodyVars,
		PrivateHead,PrivateBody,
		SharedPos,SharedNeg,[]/*No shared Vars*/):-!.
	
selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,EntailedHead,
		SharedPos,UnsharedPos,
		SharedNeg,UnsharedNeg,
		Matrix,
		HeadSlots,BodyVars,VAllShare,SharedPosV,SharedNegV,
		PrivateHead,PrivateBody,UnsharedPos,UnsharedNeg):-!.




/*

entails(not(domain(A, 1, 'ConstantQuantity')), not(instance(A, 'UnaryConstantFunctionQuantity'))),

Prolog: impossible(instance(A, 'UnaryConstantFunctionQuantity')):- impossible(domain(A, 1, 'ConstantQuantity'))


entails(not(range(A, 'ConstantQuantity')), not(instance(A, 'UnaryConstantFunctionQuantity'))), 

Prolog: neg(instance(A, 'UnaryConstantFunctionQuantity')):- neg(range(A, 'ConstantQuantity'))
 
*/
					       % no possitives
selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,[],[not(Negs)],not(EntailedHead),
	[(impossible(EntailedHead) :- impossible(ConditionalBody))],
		HeadSlots,BodyVars,
		[],[],	% No varables private on head or body /*All shared Vars*/
		SharedPos,SharedNeg,AllSharedHB):-!.

/*
replaceConsVar(xB, '$existential'([63, 84, 73, 77, 69, 50], exists(xC, and(instance(xC, 'TimePoint'), and(instance([63, 84, 73, 77, 69, 50], 'TimePoint'), and(before(xC, [63, 84, 73, 77, 69, 50]), forall(D, =>(and(beforeOrEqual(xC, D), beforeOrEqual(D, [63, 84, 73, 77, 69, 50])), time(A, D))))))))), 
replaceConsVar(xC, '$existential'([63, 84, 73, 77, 69, 49], and(instance([63, 84, 73, 77, 69, 49], 'TimePoint'), and(instance(xB, 'TimePoint'), and(before([63, 84, 73, 77, 69, 49], xB), forall(D, =>(and(beforeOrEqual([63, 84, 73, 77, 69, 49], D), beforeOrEqual(D, xB)), time(A, D))))))))], 

entails(and(not(time(A, D)), and(beforeOrEqual(D, xB), instance(A, 'Object'))), not(beforeOrEqual(xC, D))), 

Prolog: neg(beforeOrEqual(xC, D)) :- beforeOrEqual(D, xB),  instance(A, 'Object'),  not(time(A, D))


entails(and(not(time(A, D)), and(beforeOrEqual(xC, D), instance(A, 'Object'))), not(beforeOrEqual(D, xB))))))))), 

Prolog: neg(beforeOrEqual(D, xB)) :- beforeOrEqual(xC, D),  instance(A, 'Object'),  not(time(A, D))
*/


selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,Pos,Negs,EntailedHead,
	[(impossible(EntailedHead):- prove(ifThen([Item|Rest],Negs)))],
		HeadSlots,BodyVars,
		PrivateHead,PrivateBody,
		[Shared|PosV],SharedNeg,[S|Hared]  /*One Or More shared Vars*/):-!.

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,ConditionalBody,not(EntailedHead),
	[(impossible(EntailedHead):- prove(ifThen([Item|Rest],Negs)))],
	HeadSlots,BodyVars,PrivateHead,PrivateBody,[S|Hared]):-
	splitNegations(ConditionalBody,Pos,Negs), Pos=[_|_],   % At least one possitive
	prologEach(Negs,Item,sharedVars(Item,EntailedHead)),   % All negated body variables are shared with head
	prologAtLeastOne(Pos,Item,sharedVars(Item,EntailedHead)),!, % At least one positive item is shared
	subtractProp(Item,Pos,Rest),!. % we'll move that item first
	

/*


entails(and(not(subclass(E, D)), and(domain(A, C, E), domain(B, C, D))), not(subrelation(A, B)))

Prolog: neg(subrelation(A, B)):-  domain(A, C, E), domain(B, C, D), neg(subclass(E, D)).


entails(and(not(subclass(E, D)), and(subrelation(A, B), domain(A, C, E))), not(domain(B, C, D)))

Prolog: neg(domain(B, C, D)):- neg(subclass(E, D)), subrelation(A, B), domain(A, C, E)


entails(and(not(subclass(E, D)), and(subrelation(A, B), domain(B, C, D))), not(domain(A, C, E)))))),
 
Prolog: neg(domain(A, C, E)):- neg(subclass(E, D)), subrelation(A, B), domain(B, C, D)
 

entails(and(not(disjointRelation(A, D)), and(disjoint(C, E), domain(D, B, E))), not(domain(A, B, C))), 

Prolog: neg(domain(A, B, C)) :- disjoint(C, E), domain(D, B, E),  neg(disjointRelation(A, D))


and(entails(and(not(disjointRelation(A, D)), and(disjoint(C, E), domain(A, B, C))), not(domain(D, B, E))), 

Prolog: neg(domain(D, B, E)) :- disjoint(C, E), domain(A, B, C),  neg(disjointRelation(A, D))


entails(and(not(disjointRelation(A, D)), and(domain(A, B, C), domain(D, B, E))), not(disjoint(C, E)))))), 

Prolog: impossilbe(disjoint(C, E)) :- domain(A, B, C), domain(D, B, E), neg(disjointRelation(A, D)).

*/

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,Pos,Negs,EntailedHead,
		Matrix,
		HeadSlots,BodyVars,
		PrivateHead,PrivateBody,
		SharedPos,SharedNeg,AllSharedHB):-!.


selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails((Body),not(EntailedHeadLiteral)),
		[(impossible(EntailedHeadLiteral) :- ifThen(findall(Shared,Passed),Unshared))]):-
	notSkolemFlags(Flags),
	splitNegations(Body,Pos,[]),
	prologPartitionList(Pos,Item,sharedVars(Item,EntailedHeadLiteral),Passed,Unshared),
	sharedVars(Passed,EntailedHeadLiteral,Shared),!.

/*
replaceConsVar(xB, '$existential'([63, 84, 73, 77, 69, 50], exists(xC, and(instance(xC, 'TimePoint'), and(instance([63, 84, 73, 77, 69, 50], 'TimePoint'), and(before(xC, [63, 84, 73, 77, 69, 50]), forall(D, =>(and(beforeOrEqual(xC, D), beforeOrEqual(D, [63, 84, 73, 77, 69, 50])), time(A, D))))))))), 
replaceConsVar(xC, '$existential'([63, 84, 73, 77, 69, 49], and(instance([63, 84, 73, 77, 69, 49], 'TimePoint'), and(instance(xB, 'TimePoint'), and(before([63, 84, 73, 77, 69, 49], xB), forall(D, =>(and(beforeOrEqual([63, 84, 73, 77, 69, 49], D), beforeOrEqual(D, xB)), time(A, D))))))))], 

entails(instance(A, 'Object'), before(xC, xB))

Prolog: before(xC, xB) :- findall(A,instance(A, 'Object'))
	   searchable(before(_, _)):-searchable(instance(_, 'Object')).


entails(instance(A, 'Object'), instance(xC, 'TimePoint'))), 

Prolog: instance(xC, 'TimePoint') :- findall(A,instance(A, 'Object'))
	   searchable(instance(_,'TimePoint')):-searchable(instance(_, 'Object')).
	   

entails(instance(A, 'Object'), instance(xB, 'TimePoint'))), 

Prolog: instance(xB, 'TimePoint') :- findall(A,instance(A, 'Object'))
	   searchable(instance(_,'TimePoint')):-searchable(instance(_, 'Object')).
	   

*/

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails([true],EntailedHeadLiteral),
		[(EntailedHeadLiteral :- ( NewAnte,
		 searchable(EntailedHeadLiteral)))]):-
	containsSkolems(Flags,EntailedHeadLiteral),
	getPrologVars(EntailedHeadLiteral,ConVars,_,_),
	add_skolems_to_body(Flags,true,EntailedHeadLiteral,ConVars,NewAnte),
%	skolemizeVars(Flags,KRVars,EntailedHeadLiteral,SkolemizedHead),
	sharedVars(EntailedHeadLiteral,Thing,SharedVars).

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails(Thing,EntailedHeadLiteral),
		[(EntailedHeadLiteral :- ifThen(findall(SharedVars,Thing),NewAnte)),
		 (searchable(EntailedHeadLiteral):-searchable(Thing))]):-
	containsSkolems(Flags,EntailedHeadLiteral),
	getPrologVars(EntailedHeadLiteral,ConVars,_,_),
	add_skolems_to_body(Flags,true,EntailedHeadLiteral,ConVars,NewAnte),
%	skolemizeVars(Flags,KRVars,EntailedHeadLiteral,SkolemizedHead),
	sharedVars(EntailedHeadLiteral,Thing,SharedVars).
	

/*
replaceConsVar(xB, '$existential'([63, 84, 73, 77, 69, 50], exists(xC, and(instance(xC, 'TimePoint'), and(instance([63, 84, 73, 77, 69, 50], 'TimePoint'), and(before(xC, [63, 84, 73, 77, 69, 50]), forall(D, =>(and(beforeOrEqual(xC, D), beforeOrEqual(D, [63, 84, 73, 77, 69, 50])), time(A, D))))))))), 
replaceConsVar(xC, '$existential'([63, 84, 73, 77, 69, 49], and(instance([63, 84, 73, 77, 69, 49], 'TimePoint'), and(instance(xB, 'TimePoint'), and(before([63, 84, 73, 77, 69, 49], xB), forall(D, =>(and(beforeOrEqual([63, 84, 73, 77, 69, 49], D), beforeOrEqual(D, xB)), time(A, D))))))))], 

entails(instance(A, 'Object'), instance(xB, 'TimePoint')) 

Prolog:  searchable(instance(_, 'TimePoint')) :- searchable(instance(_, 'Object')).

*/

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails([true],not(Fact)),[impossible(EntailedHeadLiteral)]):-!.


selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails([true],(Fact)),[(NewCons)]):-!,
	skolemizeCons([],KRVars,Fact,Flags,NewCons).

			
selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails((ThingNotConnetedToHead),(EntailedHeadLiteral)),
		[(searchable(EntailedHeadLiteral) :- searchable(ThingNotConnetedToHead))]):-
	\+ (sharedVars(EntailedHeadLiteral,ThingNotConnetedToHead)),
	getPrologVars(ThingNotConnetedToHead,_,_,[]),!.
	

	
/*
replaceConsVar(xB, '$existential'([63, 84, 73, 77, 69, 50], exists(xC, and(instance(xC, 'TimePoint'), and(instance([63, 84, 73, 77, 69, 50], 'TimePoint'), and(before(xC, [63, 84, 73, 77, 69, 50]), forall(D, =>(and(beforeOrEqual(xC, D), beforeOrEqual(D, [63, 84, 73, 77, 69, 50])), time(A, D))))))))), 
replaceConsVar(xC, '$existential'([63, 84, 73, 77, 69, 49], and(instance([63, 84, 73, 77, 69, 49], 'TimePoint'), and(instance(xB, 'TimePoint'), and(before([63, 84, 73, 77, 69, 49], xB), forall(D, =>(and(beforeOrEqual([63, 84, 73, 77, 69, 49], D), beforeOrEqual(D, xB)), time(A, D))))))))], 

entails(and(not(time(A, D)), and(beforeOrEqual(xC, D), beforeOrEqual(D, xB))), not(instance(A, 'Object'))),

Near Prolog: neg(instance(A, 'Object')) :- beforeOrEqual(xC, D), beforeOrEqual(D, xB),  not(time(A, D)).

Prolog: impossible(instance(A, 'Object')) :- unionSpecial([D],searchable(beforeOrEqual(xC, D)), searchable(beforeOrEqual(D, xB))),  impossible(time(A, D)).

*/

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails((OnlyNegationsConnetedToHead),not(EntailedHeadLiteral)),
		[( impossible(EntailedHeadLiteral) :- (searchable(Pos),impossible(Negs)) )]):-
	splitNegations(OnlyNegationsConnetedToHead,Pos,Negs),
	sharedVars(Negs,EntailedHeadLiteral,_),
	\+ (sharedVars(Pos,EntailedHeadLiteral,_)),!.
	
/*
replaceConsVar(xB, '$existential'([63, 84, 73, 77, 69, 50], exists(xC, and(instance(xC, 'TimePoint'), and(instance([63, 84, 73, 77, 69, 50], 'TimePoint'), and(before(xC, [63, 84, 73, 77, 69, 50]), forall(D, =>(and(beforeOrEqual(xC, D), beforeOrEqual(D, [63, 84, 73, 77, 69, 50])), time(A, D))))))))), 
replaceConsVar(xC, '$existential'([63, 84, 73, 77, 69, 49], and(instance([63, 84, 73, 77, 69, 49], 'TimePoint'), and(instance(xB, 'TimePoint'), and(before([63, 84, 73, 77, 69, 49], xB), forall(D, =>(and(beforeOrEqual([63, 84, 73, 77, 69, 49], D), beforeOrEqual(D, xB)), time(A, D))))))))], 

entails(and(beforeOrEqual(xC, D), and(beforeOrEqual(D, xB), instance(A, 'Object'))), time(A, D)), 

Prolog: time(A, D) :- prove(instance(A, 'Object'),beforeOrEqual(xC, D),beforeOrEqual(D, xB)).

*/


selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails((Body),(EntailedHeadLiteral)),
		[ (EntailedHeadLiteral:- prove(Body) )]):-
	splitNegations(Body,Pos,[]),
	not(containsSkolems(Flags,EntailedHeadLiteral:Body)),
	prologEach(Pos,Item,sharedVars(Item,EntailedHeadLiteral)),!.
	
/*
	skolemizeVars(Flags,KRVars,Body,SkolemizedBody),
		
		skolemizeVars(Flags,KRVars,EntailedHeadLiteral,SkolemizedEntailedHeadLiteral),!.
  */



/*

entails(instance(A, 'UnaryConstantFunctionQuantity'), domain(A, 1, 'ConstantQuantity'))), 

Prolog: domain(A, 1, 'ConstantQuantity'):- instance(A, 'UnaryConstantFunctionQuantity').



entails(instance(A, 'UnaryConstantFunctionQuantity'), range(A, 'ConstantQuantity'))))

Prolog: range(A, 'ConstantQuantity') :- instance(A, 'UnaryConstantFunctionQuantity')



entails(and(subrelation(A, B), and(domain(A, C, E), domain(B, C, D))), subclass(E, D)) 

Prolog: subclass(E, D):- domain(A, C, E), domain(B, C, D), subrelation(A, B).

*/

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails((Body),(EntailedHeadLiteral)),
		[((EntailedHeadLiteral) :- prove(Body))]):-
	sharedVars(EntailedHeadLiteral,Body,ListOfShared),
	getPrologVars(EntailedHeadLiteral,EntailedHeadLiteralAll,_,_),
	getPrologVars(Body,BodyAll,_,_),
	allUnion(EntailedHeadLiteralAll,BodyAll),!.


/*

entails(and(disjoint(C, E), and(domain(A, B, C), domain(D, B, E))), disjointRelation(A, D)), 

Prolog: disjointRelation(A, D):- domain(A, B, C), domain(D, B, E), disjoint(C, E).

*/

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails((Body),(EntailedHeadLiteral)),
		[((EntailedHeadLiteral) :- ifThen(Passed,Failed))]):-
	splitNegations(Body,Pos,[]),
	prologPartitionList(Pos,Item,sharedVars(Item,EntailedHeadLiteral),Passed,Failed),!.



selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails(Body,not(Head)),[(impossible(Head) :- once(prove(NewAnte)))]):-
	getPrologVars(Head,ConVars,_,_),!,
	add_skolems_to_body(Flags,Body,Head,ConVars,NewAnte),!.

selectBestRuleMatrix(Sign,HeadSlots,Flags,KRVars,entails(Body,Head),[(Head:-unoptimized(NewAnte))]):-
	getPrologVars(Head,ConVars,_,_),!,
	add_skolems_to_body(Flags,Body,Head,ConVars,NewAnte),!.


notSkolemFlags(Flags):-not(member(replaceConsVar(_,_),Flags)).

splitNegations(List,Pos,Neg):-
	((prologPartitionList(List,Item,isNegation(Item),NegT,PosT),!,NegT=Neg,PosT=Pos)).
	
sharedVars(Item1,Item2):-
	((sharedVars(Item1,Item2,SharedVars,Item1Only,Item2Only),!,SharedVars=[_|_])).
	
sharedVars(Item1,Item2,SharedVars):-
	((sharedVars(Item1,Item2,SharedVars,Item1Only,Item2Only),!)).
	
sharedVars(Item1,Item2,SharedVars,Item1Only,Item2Only):-
	((getPrologVars(Item1,V1s,_,_),getPrologVars(Item2,V2s,_,_),!,
	set_partition(V1s,V2s,Item1Only,Item2Only,SharedVars),!)).

isNegation(Item):-((isSlot(Item);(Item=not(V),nonvar(V));(functor(Item,F,A),atom_concat(~,_,F)))).
	


	
getGoalProfile(Name,Time):-flag(Name,Time,Time).
profileGoalTime(Name,Goal):-
	getCputime(Start),
	call(Goal),
	getCputime(End),
	flag(Name,Time,Time),
	Average is (Time+(End-Start))/2,
	flag(Name,_,Average).	

normalizeProp(Flags,In,Out,Cons):-
	ifThenElse(
		(member(holds,Flags);member(orig,Flags)),
		(convertNegations(not,In,Notted,_),
		convertToHolds(Flags,Notted,Held)),
		Held=In),
	convertNegations(Flags,Held,Out,Cons).

convertRuleBodyWFS(Flags,Context,RuleHead,[],!).

convertRuleBodyWFS(Flags,Context,RuleHead,[Cons],Rewritten):-
	normalizeProp(Flags,Cons,Rewritten,Bare).

convertRuleBodyWFS(Flags,Context,RuleHead,[H|T],(RH,RT)):-!,
	convertRuleBodyWFS(Flags,Context,RuleHead,H,RH),
	convertRuleBodyWFS(Flags,Context,RuleHead,T,RT),!.

convertRuleBodyWFS(Flags,Context,RuleHead,(H,T),(RH,RT)):-!,
	convertRuleBodyWFS(Flags,Context,RuleHead,H,RH),
	convertRuleBodyWFS(Flags,Context,RuleHead,T,RT),!.

convertRuleBodyWFS(Flags,Context,RuleHead,and(H,T),(RH,RT)):-!,
	convertRuleBodyWFS(Flags,Context,RuleHead,H,RH),
	convertRuleBodyWFS(Flags,Context,RuleHead,T,RT),!.

convertRuleBodyWFS(Flags,Context,RuleHead,or(H,T),(RH,RT)):-!,
	convertRuleBodyWFS(Flags,Context,RuleHead,H,RH),
	convertRuleBodyWFS(Flags,Context,RuleHead,T,RT),!.

convertRuleBodyWFS(Flags,Context,RuleHead,Cons,(Rewritten)):-
	normalizeProp(Flags,Cons,Rewritten,Bare).



getPredicateKey((LogConsq),(Key)):-
		convertNegations((not),LogConsq,NC,_),!,
		getPredicateKey2((NC),(Key)).
getPredicateKey2(not(LogConsq),not(Key)):-nonvar(LogConsq),!,getPredicateKey2((LogConsq),(Key)).
getPredicateKey2((LogConsq),(Key)):-functor(LogConsq,Key,_),!.


deduceType(_,_):-fail.

convertListNotNeg(Var,holds('TruthFn',Var,'True')):-isSlot(Var),!.
convertListNotNeg(not(Var),holds('TruthFn',Var,'False')):-isSlot(Var),!.
convertListNotNeg([],[]):-!.
convertListNotNeg([not(X)|Y],[ (~ X)|YY]):-deduceType(F,negationByFailure),!,convertListNotNeg(Y,YY).
convertListNotNeg([not(X)|Y],[ (XX)|YY]):- not(isSlot(X)),!,
		X=..[F|Args],!, atom_concat('~',F,FN),
		XX=..[FN|Args],!,convertListNotNeg(Y,YY).
convertListNotNeg([(X)|Y],[(X)|YY]):-!,convertListNotNeg(Y,YY).

reorderAnteceedants([NConsq|List],List,All):-
	reorderAnteceedants(NConsq,List,List,All),!.

reorderAnteceedants(Head,Body,[],[]):-!.
reorderAnteceedants(Head,Body,[AProto],[AProto]):-!.
reorderAnteceedants(Head,Body,AProto,Ordered):-
	predsort(reorderAnteceedants(Head,Body),AProto,Ordered).

reorderAnteceedants(Head,Body,Result,T1,T2):-
	compareVariant(T1,T2,M,C1,C2),
	compare(Result,C2,C1),
	not(Result = (=)),!.
reorderAnteceedants(Head,Body,Result,T1,T2):-
	compareVariant(T1,Head,M,C1,H1),
	compareVariant(T2,Head,M,C2,H2),
	compare(Result,C2,C1),
	not(Result = (=)),!.
reorderAnteceedants(Head,Body,<,T1,T2).


set_partition([],Set2,[],Set2,[]):-!.
set_partition(Set1,[],Set1,[],[]):-!.
set_partition([H|S1],Set2,O1,O2,[H|Intersection]):-
	memberchkSpecial(H,Set2,NewSet2),!,
	set_partition(S1,NewSet2,O1,O2,Intersection).
%set_partition(Set2,[H|S1],O2,O1,[H|Intersection]):-
%	memberchkSpecial(H,Set2,NewSet2),
%	set_partition(S1,NewSet2,O1,O2,Intersection).
set_partition([H|S1],Set2,[H|O1],O2,Intersection):-
	set_partition(S1,Set2,O1,O2,Intersection).

memberchkSpecial(H,[HH|List],ListO):- 
	'=='(HH,H)
		-> ListO=List ; 
		(memberchkSpecial(H,List,ListM),ListO =  [HH|ListM]).

	
convertFactHeadWFS(Flags,Context,Cons,Rewritten):-
	normalizeProp(Flags,Cons,Rewritten,Bare).
	

convertNegations(Var,In,Out,Bare):-var(Var),!,convertNegations(keep,In,Out,Bare).	
convertNegations(Var,In,In,In):-isSlot(In),!.
convertNegations(Var,not(In),not(In),true(In)):-isSlot(In),!.

convertNegations(Flags,In,Out,Bare):-member(neg(Neg),Flags),!,convertNegations(Neg,In,Out,Bare).
convertNegations([Neg|_],In,Out,Bare):-atom(Neg),!,convertNegations(Neg,In,Out,Bare).
convertNegations(keep,PTerm,PTerm,Cons):-!,convertNegations(not,PTerm,_,Cons).

convertNegations(Neg,[Log|Ante],[Conditional|Body],[Bare|BBody]):-!,
	convertNegations(Neg,Log,Conditional,BLog),
	convertNegations(Neg,Ante,Body,BBody).

convertNegations(Neg,LogAnte,ConditionalBody,Connective):-
	compound(LogAnte),LogAnte=..[Connective|List],isBodyConnective(Connective),!,
	convertNegations(Neg,List,ConditionalBodyList,_),!,
	ConditionalBody=..[Connective|ConditionalBodyList],!.

convertNegations(Other,PTerm,PTerm,Cons):-
	PTerm=..[Other,Cons],!.
	
convertNegations(Neg,\+(In),Out,Bare):-!,convertNegations(Neg,not(In),Out,Bare).
convertNegations(Neg,'~'(In),Out,Bare):-!,convertNegations(Neg,not(In),Out,Bare).
convertNegations(Neg,In,Out,Bare):-
	In=..[NegF|Args],atom_concat('~',Pos,NegF),
	Mid=..[Pos|Args],!,convertNegations(Neg,not(Mid),Out,Bare).

convertNegations(lit,not(PTerm),Lit,PTerm):-
	PTerm=..[Pos|Args],
	atom_concat('~',Pos,F),!,
	Lit=..[F|Args].
convertNegations(Neg,not(Cons), Term,Cons):-
	Term=..[Neg,Cons].	
convertNegations(_,Cons,Cons,Cons).


/*
selectBestBodyMechanism(''(Seed),[],[],[],[],'Novars'(Seed)):-!.
selectBestBodyMechanism(''(Seed),UniversalBody,[],[],[],T):-!,T=..['Zall',Seed|UniversalBody].
selectBestBodyMechanism(''(Seed),[],HeadSlotsSingleInBody,[],[],T):-!,T=..['Headall',Seed|HeadSlotsSingleInBody].
selectBestBodyMechanism(''(Seed),[],[],BodyOnlyConected,[],T):-!,T=..['Bodyall',Seed|BodyOnlyConected].
selectBestBodyMechanism(''(Seed),[],[],[],SplitHeadVar,T):-!,T=..['HeadAll',Seed|SplitHeadVar].
selectBestBodyMechanism(Seed,UniversalBody,HeadSlotsSingleInBody,BodyOnlyConected,SplitHeadVar,VarClause):-!,
	appendFunctInf(Seed,'Univb',UniversalBody,Next1),	
	appendFunctInf(Next1,'Headb',HeadSlotsSingleInBody,Next2),	
	appendFunctInf(Next2,'Bodyc',BodyOnlyConected,Next3),
	appendFunctInf(Next3,'Doubleb',SplitHeadVar,VarClauseEF),!,
	removeEmptyFunct(VarClauseEF,VarClause).
*/


/* 
selectBestPropositionMechanism(Sign,Conditional,CVars,HeadSlots,ConditionalClass,
	CUniversalBody,CHeadSlotsSingleInBody,CBodyOnlyConected,CSplitHeadVar):-%true,
	appendFunct(''(Conditional),'Z_Uvar_',CUniversalBody,Conditional1),!,
	appendFunct(Conditional1,'amulitHeadVar_',CSplitHeadVar,Conditional2),
	appendFunct(Conditional2,'singleHeadVar_',CHeadSlotsSingleInBody,Conditional3),
	appendFunct(Conditional3,'bodyConnectedVar_',CBodyOnlyConected,ConditionalClass),!.
*/



/*
This File only exports one Predicate called: putAttributeStructures/4 and the rest of the predicates are helper to putAttributeStructures/2
                                                                                                                                                                
It does two things : Adds arg and predicate type checking .


ProtoType: putAttributeStructures(Surface,KRVars,Rule,Context,TN,CLID, ExplainationID,Anontate,Flags,IN_UnconstrainedFormua,OUT_UnconstrainedFormua)

1) if a variable is found in the head of a clause it confirms that it is contrained by a argument type checking

2) if a varaible is found in the predicate position (Head or Body) it rewrites the predicate to use functsymbol (list of Hiloged predicates) 

                                                                                  
Examples:
                                                                                  
   | ?- putAttributeStructures(Surface,KRVars,Rule,Context,TN,CLID, ExplainationID,Anontate,Flags,('instance'(R,agent) :-'instance'(R,human)),S).
   R = _h99
   S = ('instance'(_h99,agent) :- 'instance'(_h99,human))
   
   | ?- putAttributeStructures(Surface,KRVars,Rule,Context,TN,CLID, ExplainationID,Anontate,Flags,('instance'(R,agent):-performs(R,task1)),S).
   R = _h95
   S = ('instance'(_h95,agent) :- 'domain'(performs,1,_h95)  ','  performs(_h95,task1))
   
   | ?- putAttributeStructures(Surface,KRVars,Rule,Context,TN,CLID, ExplainationID,Anontate,Flags,('instance'(R,agent):-performs(R,Act)),S).
   R = _h95
   Act = _h141
   S = ('instance'(_h95,agent) :- 'domain'(performs,1,_h95)  ','  'domain'(performs,2,_h141)  ','  performs(_h95,_h141))

*/

% ===============================================
% putAttributeStructures: Has Two Phases
% ===============================================
	
putAttributeStructures(Context,Anontate,Flags,Formula,SlotStructure):-
		putPropositionAttributes(Context,Anontate,Flags,'=>',1,Formula,SlotStructure),!.



localCanonicalizerNotice(Type,Details):-
       getMooOption(Agent,putAttributeStructures,(Surface,Rule,CLID,Flags,KRVars,Context:Ctx,TN)),
       % writeq(Warning:Data),nl,
       ( \+ ( \+ (canonicalizerWarnings(Context,Type,Details)))  -> ifInteractive(write(','));
	(assertzRE(canonicalizerWarnings(Context,Type,Details)),
	ifInteractive(writeObject(nv([nl,Details,nl,Type,nl,Surface,nl,nl]),KRVars)))),!.


localCanonicalizerWarning(Type,Details):-
	localCanonicalizerNotice(Type,Details).

localCanonicalizerError(Type,Details):-
	throwMoo(argDomains,error:Type,Details).


putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,Var,Slot):-isVarProlog(Var),!,
	getDomainsForVar(Caller,ArgN,Var,Flags,Domains),
	getPutBestConstraintAttribute(Caller,ArgN,Context,Anontate,Var,Domains,Slot),!.

putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,ArgIn,ArgIn):-isSlot(ArgIn),!.

/*
putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,'$existential'(ArgIn,Lit,Formula),SlotStructure):-!,
	getDomainsForVar(Caller,ArgN,ArgIn,Flags,[Caller:ArgN|Domains]),
	getPutBestConstraintAttribute(Caller,ArgN,Context,Anontate,'$existential'(Lit,Formula),Domains,SlotStructure),!.
*/
	

getDomainsForVar(Caller,ArgN,ArgIn,Flags,Domains):-!,member(domainV(Var,Domains),Flags),Var==ArgIn,!.
getDomainsForVar(Caller,ArgN,ArgIn,Flags,[]):-!,write(getDomainsForVar(Caller,ArgN,ArgIn,Flags,[])),true.

putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,[],[]):-!.

putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,[ArgIn|B],[AA|BB]):-!,
	putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,ArgIn,AA),
	putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,B,BB),!.

putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,string(ArgIn),SlotStructure):-!,nonvar(ArgIn),!,
	 putStringAttribute(ArgIn,Caller,ArgN,SlotStructure),!.

putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-atom(ArgIn),!,
	putAtomAttributeProc(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure),!.

putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-number(ArgIn),!,
	deduceClassesForDomainVList([Caller:ArgN],Classes),!,
	sort(Classes,ClassesM), % removes duplicates
	delete(ClassesM,'Entity',ClassesMM), % Entity too general
	getListOfConstraintStructuresFromListOfClasses(ArgIn,ClassesMM,LCM),
	mostConstrainingAttribute(LCM,TightestFit),
	applyNumericFit(Caller,ArgN,TightestFit,ArgIn,SlotStructure),!.
	
		applyNumericFit(Caller,ArgN,TightestFit,ArgIn,SlotStructure):-TightestFit=[_,_,_,_|_],!,
			applyAttributeStructureToEntity(Caller,ArgN,ArgIn,TightestFit,SlotStructure),!.
			
		applyNumericFit(Caller,ArgN,_,ArgIn,SlotStructure):-!,
			applyAttributeStructureToEntity(Caller,ArgN,ArgIn,['Abstract','Quantity','Number'],SlotStructure).

putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,Term,OTerm):-
	Term=..[Funct|Args],
	putPropositionAttributesFunctor(Context,Anontate,Flags,Caller,ArgN,Funct,Args,OTerm),!.


% Connectives We deal with each argument	
putPropositionAttributesFunctor(Context,Anontate,Flags,Caller,ArgN,Funct,Args,OTerm):-
	(memberchkSpecial(Funct,[and,or,not,possible,searchable,impossible,absurd,proves,prove,xor,
	'<=>','=>',entails,(:-),',',';',ifThen]);
	atom_concat(_,'_',Funct)),!,
	putPropositionAttributes(Context,Anontate,Flags,and,1,Args,ArgsO),
	OTerm=..[Funct|ArgsO],!.

% Connectives We Skip all but the first argument
putPropositionAttributesFunctor(Context,Anontate,Flags,Caller,ArgN,Funct,[ArgIn|B],OTerm):-
	(atom_concat('t~',_,Funct);atom_concat('f~',_,Funct)),!,
	putPropositionAttributes(Context,Anontate,Flags,Caller,ArgN,ArgIn,AA),
	OTerm=..[Funct,AA|B],!.

% Functors That we dont touch Any Arguments
putPropositionAttributesFunctor(Context,Anontate,Flags,Caller,ArgN,Funct,Args,OTerm):-
	(atom_concat('$',_,Funct);memberchkSpecial(Funct,['v','$existential','include-context'])),!,
	OTerm=..[Funct|Args],!.

% Functors That Are Variable Arity That we cannot hash the arg length
putPropositionAttributesFunctor(Context,Anontate,Flags,Caller,ArgN,Funct,Args,OTerm):-
	memberchkSpecial(Funct,[disjointDecomposition,exhaustiveDecomposition,partition]),!,
	putPropositionAttributes(Context,Anontate,Flags,Funct,1,Args,ArgsO),!,
	OTerm=..[Funct,ArgsO],!.

% Functors That Are Variable Arity That we *CAN* hash the arg length
putPropositionAttributesFunctor(Context,Anontate,Flags,Caller,ArgN,Funct,[Arg1|Args],OTerm):-
	memberchkSpecial(Funct,[holds,function,'AssignmentFn']),!,
	putPropositionAttributesArguments(Context,Anontate,Flags,Funct,1,[Arg1],[SlotedF]),!,
	putPropositionAttributesArguments(Context,Anontate,Flags,Arg1,1,Args,ArgsO),!,  
	OTerm=..[Funct,SlotedF|ArgsO],!.

putPropositionAttributesFunctor(Context,Anontate,Flags,Caller,ArgN,Funct,Args,TermO):-
	putPropositionAttributesArguments(Context,Anontate,Flags,Funct,1,Args,ArgsO),!,
	TermO=..[Funct|ArgsO].


putPropositionAttributesArguments(Context,Anontate,Flags,Caller,N,[],[]):-!.
putPropositionAttributesArguments(Context,Anontate,Flags,Caller,N,[Arg|ArgS],[ArgO|ArgsO]):-
	     putPropositionAttributes(Context,Anontate,Flags,Caller,N,Arg,ArgO),NN is N+1,
	     putPropositionAttributesArguments(Context,Anontate,Flags,Caller,NN,ArgS,ArgsO),!.

	
% ===============================================
% Slot Atom
% ===============================================

%:-dynamic(putAtomAttributeProc/6).

putAtomAttribute(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-
	var(ArgIn),write(putAtomAttribute(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure)),true,fail.

% Quoted Atom
putAtomAttribute(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-atom_concat('"',_,ArgIn),!,
	putStringAttribute((ArgIn),Caller,ArgN,SlotStructure),!.

/*
putAtomAttribute(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-atom(ArgIn),	
	mooCache(Context,Anontate,putAtomAttributeProc(ArgIn,SlotStructure)),!.

*/
putAtomAttribute(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-
	putAtomAttributeProc(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure),!.
%	assertaRE(mooCache(Context,Anontate,putAtomAttributeProc(ArgIn,SlotStructure))),!.

       
% Class name
putAtomAttributeProc(Context,Anontate,Flags,Caller,ArgN,ArgIn,'$Class'(ArgIn)):-
	getClassesListFromContext(Rs,Ctx,Context),identical_member(ArgIn,Rs),!.
	
/*
	putClassEntityAttributes(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure),!.
	
% Classes use special wrapper
putClassEntityAttributes(Context,Anontate,Flags,Caller,ArgN,ArgIn,ArgIn):-!.

	getConstraintStructureForClass(ArgIn,'Class',Slot),!,
	%deduceSingleSubclassPathList(ArgIn,ArgIn,'Entity',SlotStructure)),!,
	%reverse(ReversedSlots,Slot),
	applyAttributeStructureToEntity(Caller,ArgN,ArgIn,['Abstract','Class'|_],SlotStructure),!.
*/
  
% Any Name
putAtomAttributeProc(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-
	deduceClassFromEntity(Context,Anontate,ArgIn,Class),nonvar(Class),!,
	getConstraintStructureForClass(ArgIn,Class,Slot),
	applyAttributeStructureToEntity(Caller,ArgN,ArgIn,Slot,SlotStructure),!.
					 
% Function Name
putAtomAttributeProc(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-
	getFunctionListFromContext(Rs,Ctx,Context),memberchkSpecial(ArgIn,Rs),!,
	deduceClassFromEntity(Context,Anontate,ArgIn,Class),nonvar(Class),!,
	getConstraintStructureForClass(ArgIn,Class,Slot),
	applyAttributeStructureToEntity(Caller,ArgN,ArgIn,Slot,SlotStructure),!.

% Predicate Name
putAtomAttributeProc(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-
	getPredicatesListFromContext(Rs,Ctx,Context),memberchkSpecial(ArgIn,Rs),!,
	applyAttributeStructureToEntity(Caller,ArgN,ArgIn,['Abstract', 'Relation', 'Predicate'],SlotStructure).

% Attribute Name
putAtomAttributeProc(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-
	getAttributeNamelistFromContext(Rs,Ctx,Context),memberchkSpecial(ArgIn,Rs),!,
	applyAttributeStructureToEntity(Caller,ArgN,ArgIn,[ 'Abstract', 'Attribute'],SlotStructure).

% Assume By Previous Holder Predicate

putAtomAttributeProc(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotStructure):-
	ground(ArgIn),
	mooCache(Context,Anontate,classFromWrap(ArgIn,Class)),!,
	getConstraintStructureForClass(ArgIn,Class,Slot),
	applyAttributeStructureToEntity(Caller,ArgN,ArgIn,Slot,SlotStructure),!.


% Assume By Holder Predicate
putAtomAttributeProc(Context,Anontate,Flags,Caller,ArgN,ArgIn,SlotedA):-
	ground(ArgIn),
	once(getPutBestConstraintAttribute(Caller,ArgN,Context,Anontate,ArgIn,[Caller:ArgN],SlotedA)),
	getClassFromWrap(SlotedA,Assumed),!,
	localCanonicalizerWarning('Assumed Term Agent Domain ','=>'(domain(Caller,ArgN,Assumed),instance(ArgIn,Assumed))),!,
	(ground(classFromWrap(ArgIn,Assumed)) ->assertaRE(mooCache(Context,Anontate,classFromWrap(ArgIn,Assumed))) ;true).

% Assume New Physical
putAtomAttributeProc(Context,Anontate,Flags,Caller,ArgN,ArgIn,'$Object'(ArgIn,_)):-nonvar(ArgIn),!,
	localCanonicalizerWarning('Assumed Term (No domain available) ',
		and(domain(Caller,ArgN,'?UNKNOWN'),instance(ArgIn,'Object'))),!.

	
putStringAttribute(Lit,Caller,ArgN,SlotStructure):-
	applyAttributeStructureToEntity(Caller,ArgN,string(Lit),['Physical','Object','ContentBearingObject'],SlotStructure),!.


deduceClassFromEntity(Context,Anontate,Entity,Class):-
%	deduceTransitiveClosure_PartialOrderingRelation(Context,Anontate,Agent3,subclass,M,Class,SCExplaination),
	deduceSurface_gaf(instance(Entity,Class),_UAgent,Context,Explaination),not(Class='InheritableRelation'),!.
	
getPutBestConstraintAttribute(Caller,ArgN,Context,Anontate,Var,Domains,SlotStructure):-
	deduceClassesForDomainVList(Domains,Classes),!,
	sort(Classes,ClassesM), % removes duplicates
	delete(ClassesM,'Entity',ClassesMM), % Entity too general
	getListOfConstraintStructuresFromListOfClasses(Var,ClassesMM,LCM),
	mostConstrainingAttribute(LCM,TightestFit),
	applyAttributeStructureToEntity(Caller,ArgN,Var,TightestFit,SlotStructure),!.

% getClassFromWrap(Slot,Class).

getClassFromWrap(ArgIn,'Entity'):-isPrologVar(ArgIN),!.
getClassFromWrap('$Class'(ArgIn),'Class'):-!.
getClassFromWrap(Slot,Class):-
	functor(Slot,F,A),atom_concat('$',_,F),
	arg(2,Slot,ClassesTerm),!,
	getClassBottemFromTerm(ClassesTerm,Class).
	
getClassBottemFromTerm(ClassesTerm,ClassO):-
	ClassesTerm=..[Class,Var], 
	(isPrologVar(Var) -> 
		ClassO = Class ;
		getClassBottemFromTerm(Var,ClassO)),!.
	
mostConstrainingAttribute([],[]):-!. % none
mostConstrainingAttribute([ArgIn],ArgIn):-!. %only one
mostConstrainingAttribute(List,Longest):-lengthSort(List,[Longest|_]),!.

% Put the longest list first
lengthSort(ListOfLists,LongestFirst):-	
	predsort(lengthSort,ListOfLists,LongestFirst),!.
lengthSort(Result,List1,List2):-
	length(List1,L1),
	length(List2,L2),
	compare(Result,L2,L1).

% ===============================================    
% Build Class Path
% ===============================================

getListOfConstraintStructuresFromListOfClasses(Orig,[],[]):-!.
getListOfConstraintStructuresFromListOfClasses(Orig,[Class|More],[Choice|Classes]):-
	getConstraintStructureForClass(Orig,Class,Choice),!,
	getListOfConstraintStructuresFromListOfClasses(Orig,More,Classes),!.


getConstraintStructureForClass(Orig,V,['Entity']):-var(V),!.

getConstraintStructureForClass(Orig,'UnionFn'(Class, _),Slot):-
	getConstraintStructureForClass(Orig,Class,Slot),!.

getConstraintStructureForClass(Orig,Class,Slot):-
	deduceSingleSubclassPathList(Orig,Class,'Entity',ReversedSlots),!,
	reverse(ReversedSlots,Slot).

getConstraintStructureForClass(Orig,Class,[]):-
	localCanonicalizerWarning('Problem: Class is not connected to Knowledgebase ontology',(Class)),!.
	

deduceSingleSubclassPathList(Entity,Origin,Destination,Path):-not(ground((Origin,Destination))),true,fail.

deduceSingleSubclassPathList(InheritableRelation, 'InheritableRelation', _, ['Abstract','Relation']):-!.

deduceSingleSubclassPathList(Entity,Origin,Destination,Path):-
		mooCache(Context,Anontate,deduceSingleSubclassPathList(Origin,Destination,Path)),!.

deduceSingleSubclassPathList(Entity,Origin,Destination,Path):-
	deduceSingleSubclassPathListProc(Entity,Origin,Destination,Path),
	assertaRE(mooCache(Context,Anontate,deduceSingleSubclassPathList(Origin,Destination,Path))).

deduceSingleSubclassPathListProc(Entity,Origin,Origin,[]):-!.
deduceSingleSubclassPathListProc(Entity,Origin,Destination,[Origin|O]):-
	deduceDirectSubclass(Origin,ClassMid),
	deduceSingleSubclassPathList(Entity,ClassMid,Destination,O).

deduceSingleSubclassPathListProc(Entity,Origin,ClassFinal,[]):-
	localCanonicalizerError('Problem with knowledgebase: Could not deduce ',(subclass(Origin,'?CLASS'))),!.
	
deduceDirectSubclass(Origin,ClassMid):-
	deduceSurface_easy(subclass(Origin,ClassMid),_,Ctx,_P),
	nonvar(Origin),not((ClassMid='InheritableRelation')),!.
	
% ===============================================
% Slot Entity
% ===============================================

argVar(Arg):-catch(arg(1,Arg,$),_,true).

% Wraps Entity
applyAttributeStructureToEntity(Caller,ArgN,ArgV,[V|L],ArgV):-var(V),!,
	localCanonicalizerError('Could not retrieve domains for',domain(Caller,ArgN,ArgV)).

applyAttributeStructureToEntity(Caller,ArgN,ArgV,[],ArgV):-!,
	localCanonicalizerNotice('Specialize for rule',domain(Caller,ArgN,'Entity')),!.
	
applyAttributeStructureToEntity(Caller,ArgN,ArgV,['Entity'],ArgV):-!,
	localCanonicalizerNotice('Specialize for rule',domain(Caller,ArgN,'Entity')),!.

applyAttributeStructureToEntity(Caller,ArgN,ArgV,['Abstract','Class'|_],'$Class'(ArgV)):-!.

applyAttributeStructureToEntity(Caller,ArgN,ArgV,[Class],'$IdentityFn'(ArgV,Slot)):-!,
	Slot=..[Class,_],localCanonicalizerNotice('Specialized for rule',domain(Caller,ArgN,Class)),!.
	
applyAttributeStructureToEntity(Caller,ArgN,ArgV,[Base,Class|Remainder],Slot):-!,
	atom_concat('$',Class,DClass),
	Slot=..[DClass,ArgV,Classes],
	makeClassNodeF(Remainder,Classes),!.

applyAttributeStructureToEntity(Caller,ArgN,ArgV,Class,ArgV):-!,
	localCanonicalizerError('Unknown Structure',domain(Caller,ArgN,Class)),!.

makeClassNodeF(V,V):-isSlot(V),!.
makeClassNodeF([],_):-!.
makeClassNodeF([Super|L],Class):-
	Class=..[Super,SubClass],
	makeClassNodeF(L,SubClass),!.

% ===============================================
% Convert [Pred1:N1,Pred2:N2,...] list to Domain List
% ===============================================

deduceClassesForDomainVList([],[]):-!.
deduceClassesForDomainVList([Pred:N|PredN],[Class|More]):-
	deduceClassForDomainV(Pred:N,Class),!,
	deduceClassesForDomainVList(PredN,More).

deduceClassForDomainV(Var:N,'Entity'):-var(Var),!.
deduceClassForDomainV('$instanceof':Class,Class):-!.

deduceClassForDomainV(Pred:N,Class):-deduceSurface(domain(Pred,N,Class),_,_,_),!.
deduceClassForDomainV(Pred:range,Class):-deduceSurface(range(Pred,Class),_,_,_),!.

deduceClassForDomainV(Pred:N,Class):-
		deduceSurfaceDomainVector(Arity,Pred,VectS,Agent,Context,Anontate,_),
		nth1(N,VectS,Class),!.

deduceClassForDomainV(Pred:N,Class):-deduceSurface(range(Pred,Class),_,_,_),!.
		%sendNote(currentUser,'Canonicalizer','Warning While Compiling Knowledgebase (using range) ',domain(Pred,N,Class)),!.

deduceClassForDomainV(Pred:range,Class):-deduceSurface_gaf(domain(Pred,4,Class),_,_,_),!.
deduceClassForDomainV(Pred:range,Class):-deduceSurface_gaf(domain(Pred,3,Class),_,_,_),!.
deduceClassForDomainV(Pred:range,Class):-deduceSurface_gaf(domain(Pred,2,Class),_,_,_),!.

deduceClassForDomainV(Pred:range,'Class'):-deduceSurface(rangeSubclass(Pred,Class),_,_,_),!.

deduceClassForDomainV(Pred:N,'Entity'):-isEntityref(Pred,_),number(N),!.
 		
deduceClassForDomainV(Pred:N,'Entity'):-!,
	localCanonicalizerWarning('Warning: No Predicate Domain ',domain(Pred,N,'Entity')),!.




%end_of_file.


import_ado_cache:-import_ado_cache('LogicMOO').
import_ado_cache(ContextName):-
	makeKnowledgeBase(ContextName,ContextName),
	showKnowledgeBase,
	getCurrentKnowledgeBase(Context),
	format('~nImporting ~w.~n~n',[Context]),!,
	before_ado_cache(ContextName,Context),
	import_ado_cache(ContextName,Context),
	after_ado_cache(ContextName,Context).


import_ado_cache_all_on:-
	   setMooOption(Agent,traceOutput,yes),
	   setMooOption(Agent,prover,decider),
	   setMooOption(Agent,translationMode,flatRelational),
	   setMooOption(Agent,decider,on).
	
import_ado_cache(ContextName,Context):-
	mooCache(_, surface, Axiom, Vars, ContextName, Context, Tracking, Agent, Status),%true,
	ado_cache_moo_assertKIF(Axiom,Tracking,Context,Context,ContextName),
	fail.
	
import_ado_cache(ContextName,Context):-
	format('~n% Done importing ~w.~n~n',[ContextName]),!.
	

before_ado_cache(ContextName,Context):-
	retractallSpecial(shown_template(_)),
	tell(footheory).

after_ado_cache(ContextName,Context):-
	listing(shown_template),
	told,
	predicate_property(shown_template(_), number_of_clauses(N)),
	format('~q clauses ~n',[N]).

ado_cache_moo_assertKIF(Axiom,Tracking,Context,Context,ContextName):-
	moo_assertKIF(Axiom,Tracking,Context,Context,ContextName),!.  % Puts it into MooHL % TODO

	
:-dynamic(shown_template/1).

show_template(Surface,Axiom,Result):-shown_template(Result),!.

show_template(Surface,Axiom,Result):-
	format('~nSurf: ~q~nResult:  ~q~n ~q~n~n',[Surface,Axiom,Result]),
	assertzRE(shown_template(Result)),!.       

make_template(Var,Var):-isSlot(Var),!.
make_template(Term,OTerm):-compound(Term),
	Term=..[P|List],
	hlPredicateAttribute(logicalConnective,P),!,
	maplist(make_template,List,OList),
	OTerm=..[P|OList],!.
make_template(Term,OTerm):-compound(Term),
	Term=..List,!,
	maplist(make_template,List,OList),
	OTerm=..OList,!.
make_template(P,P):-member(P,[holds,instance]),!.
make_template(P,Q):-predicate_table(P,Q),!.
make_template(P,Q):-
	flag(pred_num,X,X+1),
	atom_concat('p',X,Q),
	assertaRE(predicate_table(P,Q)),!.
	
write_cl_of2(Tracking,Axiom):-
	translate(Axiom,Clauses),
	format('~n% ~w  ~q~n',[Tracking,Axiom]),
	list_clauses_out(Clauses).

list_clauses_out([]).
list_clauses_out([H|T]):-
	list_clause_out(H),
	list_clauses_out(T).

list_clause_out(true).
list_clause_out(cl([H],[])):-format('fact(~q).~n',[H]).
list_clause_out(cl([],H)):-format('disj(~q).~n',[H]).
list_clause_out(H):-format('~q.~n',[H]).

cmp_is_a_fuction(X):-atom(X),atom_concat(_,'Fn',X).


moo_assertKIF(Axiom):-				   
	getCurrentKnowledgeBase(Context),
	moo_assertKIF(Axiom,Context).

moo_assertKIF(Axiom,Context):-
	idGen(Tracking),
	moo_assertKIF(Axiom,Tracking,[],ContextName).	

moo_assertKIF(Axiom,Context,Context):-
	idGen(Tracking),
	moo_assertKIF(Axiom,Tracking,[Context],ContextName).	


moo_assertKIF(Axiom,Tracking,Context,ContextName):-
	moo_assertKIF(Axiom,Tracking,Context,ContextName,ContextName).

% ===============================================================	
% moo_assertKIF(Axiom,Tracking,Context,ContextObj,ContextName)
% ===============================================================	

moo_assertKIF(Axiom,Tracking,Context,Context,ContextName):-
	preprocessAxiom(Axiom,AxiomO),!,
	moo_assertKIF2(AxiomO,Tracking,Context,Context,ContextName).


preprocessAxiom(Axiom,Axiom).  % TODO
'VariableArityRelation'(disjointDecomposition).
'VariableArityRelation'(exhaustiveDecomposition).
'VariableArityRelation'(partition).

 
% Uses Specialization

moo_assertKIF2(Axiom,Tracking,Context,Context,ContextName):-
	moo_assertKIF_implicit(Axiom),!.
moo_assertKIF2(Axiom,Tracking,Context,Context,ContextName):-
	moo_assertKIF_fast(Axiom,Tracking,Context,Context,ContextName),!.
moo_assertKIF2(Axiom,Tracking,Context,Context,ContextName):-
	moo_assertKIF_slow(Axiom,Tracking,Context,Context,ContextName),!.

moo_assertKIF2(Axiom,Tracking,Context,Context,ContextName):-
        numbervars(Surface,'$VAR',1,_),!,
	getAssertionClauses(Context,Axiom,Clauses,_,Flags),!,
	write_surf_axioms(Axiom,Clauses).

write_surf_axioms(Surface,and(A,B)):-!,
	write_surf_axioms(Surface,A),!,
	write_surf_axioms(Surface,B).
	
write_surf_axioms(Surface,CL):-!,
	flag(pred_num,_,0),
	retractallSpecial(predicate_table(_,_)),!,
	unnumbervars(Surface:CL,OSurface:OCL),	!,
	numbervars(OSurface:OCL,'$VAR',0,_),
	make_template(OCL,Template),!,
	show_template(OSurface,OCL,Template),!.



%        format('Uncompiled: ~q~n',[C]),
 %       ignore((C=[],write(Axiom),nl,nl)),

% ===============================================================	
% Fast/Slow Specializations
% ===============================================================      
moo_assertKIF_fast(KIF,Tracking,Context,Context,ContextName):-
	moo_assertKIF_mapping(KIF,Tracking,Context,Context,Mapped),
	invokeMapping(Mapped),!.

moo_assertKIF_mapping(KIF,Tracking,Context,Context,Mapped):-
	mapping_nt(KIF,Context,Context,Mapped).

mapping_nt(KIF,Context,Context,Mapped):-
	mapping_nt(KIF,Mapped).
	
invokeMapping(undefined).
invokeMapping([A|B]):-!,invokeMapping(A),invokeMapping(B).
invokeMapping((A,B)):-!,invokeMapping(A),invokeMapping(B).
invokeMapping((A;B)):-!,invokeMapping(A);invokeMapping(B).
invokeMapping('+'(Axiom)):-!,moo_assertKIF(Axiom).
invokeMapping(Slowmapped):-Slowmapped.

moo_assertKIF_slow(KIF,Tracking,Context,Context,ContextName):-
	moo_assertKIF_slowmapping(KIF,Tracking,Context,Context,Slowmapped),
	invokeMapping(Slowmapped),!.

moo_assertKIF_slowmapping(KIF,Tracking,Context,Context,Slowmapped):-
	slowmapping_nt(KIF,Context,Context,Slowmapped).

slowmapping_nt(KIF,Context,Context,Slowmapped):-
	slowmapping_nt(KIF,Slowmapped).


% ===============================================================	
% Mapped Specializations
% ===============================================================      

mapping_nt(KIF,Context,Context,Mapped):-
	mapping_nt(KIF,Mapped). % TODO 

% instance/2 
mapping_nt(instance(CName1,CName2),assert_ind(CName1,CName2)).

% subclass/2
mapping_nt(subclass(A, B),defprimconcept(A,[supers([B])])).

% disjoint/2
mapping_nt(disjoint(CName1,CName2),defprimconcept(CName2,not(CName1))).    

% subrelation/2	   %TODO unclear
mapping_nt(subrelation(A, B),defprimconcept(A,B)).

% range/2	   % TODO unclear
mapping_nt(range(A, B),(defprimrole(A,B),defprimrole(B))).

% rangeSubclass/2	   % TODO unclear
mapping_nt(range(A, B),defprimrole(A,B)).

% ===============================================================	
% Slowmapped Specializations
% ===============================================================      

slowmapping_nt(KIF,Context,Context,Slowmapped):-
	slowmapping_nt(KIF,Slowmapped). % TODO 

% inverse/2 for Functions
slowmapping_nt(inverse(CName1,CName2),defrole(CName1,inverse(CName2))):-cmp_is_a_fuction(CName1).

% inverse/2 for ELSE (Predicates)
slowmapping_nt(inverse(CName1,CName2),defprimconcept(CName1,inverse(CName2))).

% Not instance/2 
slowmapping_nt(not(instance(CName1,CName2)),assert_ind(CName1,not(CName2))).

% Catches mapping_info Functions
slowmapping_nt(Term,assert_ind(A,B,R)):-compound(Term),functor(Term,R,N),cmp_is_a_fuction(R),
	mapping_info_relation(R,N),
	Term=..[R,A,B].

% Catches mapping_info Predicates
slowmapping_nt(Term,assert_ind(A,B,R)):-compound(Term),functor(Term,R,N),
	mapping_info_relation(R,N),
	Term=..[R,A,B].

% Catches new Functions %TODO
slowmapping_nt(Term,assert_ind(A,B,R)):-compound(Term),functor(Term,R,N),cmp_is_a_fuction(R),
	assertzRE(mapping_info_relation(R,N)),
	Term=..[R,A,B].

% Catches new Relations %TODO
slowmapping_nt(Term,assert_ind(A,B,R)):-compound(Term),functor(Term,R,N),
	assertzRE(mapping_info_relation(R,N)),
	Term=..[R,A,B].

:-dynamic(mapping_info_relation/2).

mapping_info_relation(representsInLanguage,2).
mapping_info_relation(expressedInLanguage,2).
mapping_info_relation(equivalentContentInstance,2).
mapping_info_relation(subsumesContentClass,2).
mapping_info_relation(equivalentContentClass,2).


mapping_info_relation(documentation,2).
mapping_info_relation(age,2).
mapping_info_relation(publishes,2).
mapping_info_relation(authors,2).
mapping_info_relation(internalRelatedConcept,2).
mapping_info_relation(relatedInternalConcept,2).
mapping_info_relation(synonymousExternalConcept,2).
mapping_info_relation(subsumedExternalConcept,2).
mapping_info_relation(subsumingExternalConcept,2).
mapping_info_relation(subsumesContentInstance,2).
mapping_info_relation(starts,2).
mapping_info_relation(meetsTemporally,2).
mapping_info_relation(finishes,2).
mapping_info_relation(before,2).
mapping_info_relation(identityElement,2).
mapping_info_relation(trichotomizingOn,2).
mapping_info_relation(holdsDuring,2).
mapping_info_relation(deathTime,2).
mapping_info_relation(birthTime,2).
mapping_info_relation(agent,2).
mapping_info_relation(attribute,2).
mapping_info_relation(part,2).
mapping_info_relation(surface,2).
mapping_info_relation(patient,2).
mapping_info_relation(superficialPart,2).
mapping_info_relation(connected,2).
mapping_info_relation(orientation,2).
mapping_info_relation(between,2).
mapping_info_relation(containsInformation,2).
mapping_info_relation(employs,2).
mapping_info_relation(occupiesPosition,2).
mapping_info_relation(member,2).
mapping_info_relation(familyRelation,2).
mapping_info_relation(connectsEngineeringComponents,2).
mapping_info_relation(connectedEngineeringComponents,2).
mapping_info_relation(engineeringSubcomponent,2).
mapping_info_relation(hasPurpose,2).
mapping_info_relation(version,2).
mapping_info_relation(component,2).
mapping_info_relation(date,2).
mapping_info_relation(greaterThan,2).
mapping_info_relation(inhabits,2).
mapping_info_relation(effector,2).
mapping_info_relation(mother,2).
mapping_info_relation(father,2).
mapping_info_relation(siblings,2).
mapping_info_relation(parent,2).
mapping_info_relation(wants,2).
mapping_info_relation(inScopeOfInterest,2).
mapping_info_relation(time,2).
mapping_info_relation(result,2).
mapping_info_relation(instrument,2).
mapping_info_relation(origin,2).
mapping_info_relation(destination,2).
mapping_info_relation(length,2).
mapping_info_relation(path,2).
mapping_info_relation(subProcess,2).
mapping_info_relation(located,2).
mapping_info_relation(during,2).
mapping_info_relation(overlapsSpatially,2).
mapping_info_relation(meetsSpatially,2).
mapping_info_relation(properPart,2).
mapping_info_relation(fills,2).
mapping_info_relation(properlyFills,2).
mapping_info_relation(completelyFills,2).
mapping_info_relation(partiallyFills,2).
mapping_info_relation(hole,2).
mapping_info_relation(interiorPart,2).
mapping_info_relation(overlapsPartially,2).
mapping_info_relation(partlyLocated,2).
mapping_info_relation(connects,2).
mapping_info_relation(duration,2).
mapping_info_relation(lessThan,2).
mapping_info_relation(lessThanOrEqualTo,2).
mapping_info_relation(resource,2).
mapping_info_relation(cooccur,2).
mapping_info_relation(earlier,2).
mapping_info_relation(overlapsTemporally,2).
mapping_info_relation(temporallyBetweenOrEqual,2).
mapping_info_relation(beforeEq,2).
mapping_info_relation(temporallyBetween,2).
mapping_info_relation(temporalPart,2).
mapping_info_relation(frequency,2).
mapping_info_relation(monetaryValue,2).
mapping_info_relation(measure,2).			
mapping_info_relation(larger,2).
mapping_info_relation(pathLength,2).
mapping_info_relation(graphPart,2).
mapping_info_relation(greaterThanOrEqualTo,2).
mapping_info_relation(arcWeight,2).
mapping_info_relation(subGraph,2).
mapping_info_relation(links,2).
mapping_info_relation(element,2).
mapping_info_relation(subsetSpecial,2).
mapping_info_relation(uses,2).
mapping_info_relation(subPlan,2).
mapping_info_relation(realization,2).
mapping_info_relation(represents,2).
mapping_info_relation(possesses,2).
mapping_info_relation(crosses,2).
mapping_info_relation(penetrates,2).
mapping_info_relation(traverses,2).
mapping_info_relation(exactlyLocated,2).
mapping_info_relation(holdsObligation,2).
mapping_info_relation(confersObligation,2).
mapping_info_relation(capability,2).
mapping_info_relation(holdsRight,2).
mapping_info_relation(confersRight,2).
mapping_info_relation(hasSkill,2).
mapping_info_relation(exploits,2).
mapping_info_relation(entails,2).
mapping_info_relation(copy,2).
mapping_info_relation(distributes,2).
mapping_info_relation(reflexiveOn,2).
mapping_info_relation(equivalenceRelationOn,2).
mapping_info_relation(totalOrderingOn,2).
mapping_info_relation(partialOrderingOn,2).
mapping_info_relation(irreflexiveOn,2).
mapping_info_relation(closedOn,2).
mapping_info_relation(true,2).
mapping_info_relation(knows,2).
mapping_info_relation(believes,2).
mapping_info_relation(needs,2).
mapping_info_relation(manner,2).
mapping_info_relation(piece,2).

% ===============================================================	
% Undefined Specializations
% =============================================================== 

% successorAttributeClosure/2
mapping_nt(successorAttributeClosure(A, B),undefined).

% disjointDecomposition/N
mapping_nt(disjointDecomposition(Class,List),undefined).
% exhaustiveDecomposition/N
mapping_nt(exhaustiveDecomposition(Class,List),undefined).
% partition/N
mapping_nt(partition(Class,List),undefined).




% contraryProperty/2
mapping_nt(contraryProperty(A, B),undefined).

% subAttribute/2
mapping_nt(subAttribute(A, B),undefined).

% successorAttribute/2
mapping_nt(successorAttribute(A, B),undefined).

% domain/3
mapping_nt(domain(A, B, C),undefined).

% 'include-context'/1
mapping_nt('include-context'(A),undefined).

% domainSubclass/3
mapping_nt(domainSubclass(A, B, C),undefined).

% singleValued/2
mapping_nt(singleValued(A, B),undefined).

% range/2
mapping_nt(range(A, B),undefined).

% starts/2
mapping_nt(starts(A, B),undefined).

% equal/2
mapping_nt(equal(A, B),undefined).

% meetsTemporally/2
mapping_nt(meetsTemporally(A, B),undefined).

% finishes/2
mapping_nt(finishes(A, B),undefined).

% before/2
mapping_nt(before(A, B),undefined).

% internalRelatedConcept/2
mapping_nt(internalRelatedConcept(A, B),undefined).

% identityElement/2
mapping_nt(identityElement(A, B),undefined).

% trichotomizingOn/2
mapping_nt(trichotomizingOn(A, B),undefined).

% holdsDuring/2
mapping_nt(holdsDuring(A, B),undefined).

% deathTime/2
mapping_nt(deathTime(A, B),undefined).

% birthTime/2
mapping_nt(birthTime(A, B),undefined).

% agent/2
mapping_nt(agent(A, B),undefined).

% attribute/2
mapping_nt(attribute(A, B),undefined).

% member/2
mapping_nt(member(A, B),undefined).

% version/2
mapping_nt(version(A, B),undefined).

% greaterThan/2
mapping_nt(greaterThan(A, B),undefined).

% destination/2
mapping_nt(destination(A, B),undefined).

% length/2
mapping_nt(length(A, B),undefined).

% path/2
mapping_nt(path(A, B),undefined).

% subProcess/2
mapping_nt(subProcess(A, B),undefined).

% lessThan/2
mapping_nt(lessThan(A, B),undefined).

% lessThanOrEqualTo/2
mapping_nt(lessThanOrEqualTo(A, B),undefined).

% larger/2
mapping_nt(larger(A, B),undefined).

% greaterThanOrEqualTo/2
mapping_nt(greaterThanOrEqualTo(A, B),undefined).

% links/2
mapping_nt(links(A, B),undefined).

% element/2
mapping_nt(element(A, B),undefined).

% subsetSpecial/2
mapping_nt(subsetSpecial(A, B),undefined).

% subPlan/2
mapping_nt(subPlan(A, B),undefined).

% entails/2
mapping_nt(entails(A, B),undefined).

% copy/2
mapping_nt(copy(A, B),undefined).

% distributes/2
mapping_nt(distributes(A, B),undefined).

% reflexiveOn/2
mapping_nt(reflexiveOn(A, B),undefined).

% equivalenceRelationOn/2
mapping_nt(equivalenceRelationOn(A, B),undefined).

% totalOrderingOn/2
mapping_nt(totalOrderingOn(A, B),undefined).

% partialOrderingOn/2
mapping_nt(partialOrderingOn(A, B),undefined).

% irreflexiveOn/2
mapping_nt(irreflexiveOn(A, B),undefined).

% closedOn/2
mapping_nt(closedOn(A, B),undefined).

% valence/2
mapping_nt(valence(A, B),undefined).

% true/2
mapping_nt(true(A, B),undefined).

% ===============================================================	
% Implied Axioms
% ===============================================================
moo_assertKIF_implicit(Axiom):-fail.


% ===============================================================	
% Asking Axioms
% ===============================================================
moo_askKIF(Axiom).
moo_askKIF(Axiom,Context).
moo_askKIF(Axiom,Context,Context).


cmp_print(X):-atom(X),!,write(X).
cmp_print(X):-print(X).
cmp_write(X):-write(X).
cmp_writeq(X):-writeq(X).
cmp_nl:-nl.
cmp_format(X):-format(X).
cmp_format(X,Y):-format(X,Y).
cmp_format(S,X,Y):-format(S,X,Y).




