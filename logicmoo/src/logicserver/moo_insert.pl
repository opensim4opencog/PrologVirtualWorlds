:-include('moo_header.pl').

/*

When this enter in thru this interface various things may happen:

1)   The surface is stored as-is (noncanonicalize)
2)   The surface is checked for truth (untrusted/trusted)
3)   The surface is canonicalized (canonicalize)
4)   The canonicalized form is complied (compile)
5)   The compiled form reaches active memory									 
									 
Prototype: tell(+Assertion[,Ctx][,Tracking]).

Assertion: is any writeFmt in KIF/STANDARD/Prolog
Ctx: Prolog atom defining the context the assertion or command is ran in
Tracking: Prolog Term provided by external source for tracking purposes


Usage: a component needs to modify the known world

See examples above

*/
% Entry point for Compiling assertions and retractions 

% =====================================================================================
%  agentInsert(Insert_chars,Ctx,TN,Context,User)
% =====================================================================================
agentInsert(Insert_chars,Cxt):-!,
         agentInsert(Insert_chars,Ctx,TN,Context,'Maintainer').
         
agentInsert(Insert_chars,Ctx,TN):- 
         agentInsert(Insert_chars,Ctx,TN,Context,'Maintainer').

agentInsert(Insert_chars,Ctx,TN,Context,User):-!,   
            once(tell_retract_parse_chars(Insert_chars,Pterm,Vars)),
            invokeInsert([untrusted,canonicalize],surface,Pterm,Ctx,TN,Context,Vars,User).


dte:- agentInsert("(isa Joe Human)",'GlobalContext').

% ===================================================================
% Normalizing Surfaces
% ===================================================================

toFSCK(Form,_,_,AssertionO,SourceCtxO,SourceTNO):-
			Form=..[asserted,SourceCtx,Assertion],!,
			toFSCK(Assertion,SourceCtx,SourceTN,AssertionO,SourceCtxO,SourceTNO).
toFSCK(Form,_,_,AssertionO,SourceCtxO,SourceTNO):-
			Form=..[pnf,Assertion,SourceCtx,SourceTN],!,
			atom_codes(SourceTN,[84,45|TNCODES]),catch(number_codes(SourceTNM,TNCODES),_,SourceTNM=TN),
			toFSCK(Assertion,SourceCtx,SourceTNM,AssertionO,SourceCtxO,SourceTNO).
toFSCK(Form,_,_,AssertionO,SourceCtxO,SourceTNO):-
			Form=..[ist,SourceCtx,Assertion],!,
			toFSCK(Assertion,SourceCtx,SourceTN,AssertionO,SourceCtxO,SourceTNO).
toFSCK('BACKWARD'(Formula),Ctx,TN,Formula,Ctx,TN):-!.
toFSCK('FORWARD'(Formula),Ctx,TN,Formula,Ctx,TN):-!.
toFSCK('sharedNotes'(_,Formula),Ctx,TN,Formula,Ctx,TN):-!.
toFSCK('clause-form'(Formula),Ctx,TN,Formula,Ctx,TN):-!.
toFSCK('request'(Formula),Ctx,TN,'request'(Formula),Ctx,TN):-!.
toFSCK((Formula),Ctx,TN,(Formula),Ctx,TN):-!.


% ===================================================================
% MOO TELL/RETRACT
% ===================================================================

:-index(invokeInsert(1,1,1,0,0,0,0,0)).

% ======================================================
% Ignored Invokations
% ======================================================
invokeInsert(Driver,Any,surf,Ctx,TN,Context_Name,Vars,Maintainer):-!.
invokeInsert(Driver,Form,formula,Ctx,TN,Context_Name,Vars,Maintainer):-!.        
invokeInsert(Driver,Any,comment(_),Ctx,TN,Context_Name,Vars,Maintainer):-!.
invokeInsert(Driver,Any,file_comment(_),Ctx,TN,Context_Name,Vars,Maintainer):-!.
invokeInsert(Driver,Any,end_of_file,Ctx,TN,Context_Name,Vars,Maintainer):-!.
invokeInsert(Driver,Any,true,Ctx,TN,Context_Name,Vars,Maintainer):-!.
invokeInsert(Driver,Any,(true:-true),Ctx,TN,Context_Name,Vars,Maintainer):-!.
invokeInsert(Driver,Any,Form,Ctx,TN,Context_Name,Vars,Maintainer):-var(Ctx),
	sendNote(user,'Assert mech','Variable in Context',Form),!,true.
invokeInsert(Driver,Any,Form,Ctx,TN,Context_Name,Vars,Maintainer):-var(Context_Name),
	sendNote(user,'Assert mech','Variable in Context',Form),!,true.



% ======================================================
% Conjunctive Invokations
% ======================================================
invokeInsert(Driver,Type,(Form,Ula),Ctx,TN,Context,Vars,Maintainer):-!,
		logOnFailure(invokeInsert(Driver,Type,Form,Ctx,TN,Context,Vars,Maintainer)),!,
		logOnFailure(invokeInsert(Driver,Type,Ula,Ctx,TN,Context,Vars,Maintainer)),!.

invokeInsert(Driver,Type,and(Form,Ula),Ctx,TN,Context,Vars,Maintainer):-!,
		logOnFailure(invokeInsert(Driver,Type,Form,Ctx,TN,Context,Vars,Maintainer)),!,
		logOnFailure(invokeInsert(Driver,Type,Ula,Ctx,TN,Context,Vars,Maintainer)),!.


% ======================================================
% Choose KIF or ACE (For now always choosing kif)
% ======================================================
invokeInsert(Driver,chars,CHARS,Ctx,TN,Context_Name,Vars,Maintainer):-!,
         invokeInsert(Driver,kif,CHARS,Ctx,TN,Context_Name,Vars,Maintainer),!.       


% ======================================================
% Make Surface Forms from KIF chars
% ======================================================
invokeInsert(Driver,kif,CHARS,Ctx,ETN,Context_Name,_,Maintainer):-!,
         once(getSurfaceFromChars(CHARS,STERM,Vars)),                      
         getMooTermFromSurface(STERM,Formula),!,   
         invokeInsert(Driver,surface,Formula,Ctx,ETN,Context_Name,Vars,Maintainer),!.       

% ======================================================
% Make Surface Forms from ACE chars
% ======================================================
invokeInsert(Driver,ace,CHARS,Ctx,TN,Context_Name,Vars,Maintainer):-
         ace_to_surface(CHARS,Formula),
         getMooTermFromSurface(STERM,Formula),!,
         invokeInsert(Driver,surface,Formula,Ctx,TN,Context_Name,Vars,Maintainer),!.       

% ======================================================
% Actually Assert the surface (trusted/untrusted)
% ======================================================
invokeInsert(Driver,surface,FormulaIn,Ctx,TN,SContext,Vars,Maintainer):-!,         
	logOnFailure(toFSCK(FormulaIn,Ctx,TN,Formula,SCtx,STN)),!,
	invokeInsert(Driver,fsck,Formula,SCtx,STN,SContext,Vars,Maintainer),!.         

% ======================================================
% Check for Simple Surface
% ======================================================
/*
invokeInsert(Driver,fsck,Surface,Ctx,TN,Context,Vars,Maintainer):-
	        getConstants(atomic,Surface,C,_,_),
		once(disabledKeywords(C)),!,
		idGen(TN), % create tracking now if needed
		destroyTN(Context,TN,_),
		ignore((
			logOnFailure(assertaClean(mooCache(Surface,entails(true,true),[],Vars,Context,TN,Maintainer,not_used)))
		)),!.
*/
     
disabledKeywords(L):-
	member(T,L),
	disabledKeyword(T),!.
	
disabledKeyword('AbstractionFn').	
disabledKeyword('GeneralizedUnionFn').	
disabledKeyword('GeneralizedIntersectionFn').	
disabledKeyword('UnionFn').	
disabledKeyword('ExtensionFn').	
disabledKeyword('IntersectionFn'). 
disabledKeyword('ComplementFn'). 
disabledKeyword('RangeFn'). 
disabledKeyword('DomainFn'). 
%disabledKeyword(A):-atom(A),atom_concat(_,'Fn',A),!.	
disabledKeyword(A):-atom(A),atom_concat(_,'On',A),!.	

/*
disabledKeyword(exists).	
*/

tam(Surface,Vars):-
	flag(indent,_,0),
	TN = test,
	Context = 'PrologMOO',
	Maintainer = 'Maintainer',
	Ctx = 'Context',
%	writeObject(Surface,Vars),
%	once(writeObject('$spacer',Vars)),
	logOnFailure(getAssertionClauses(Context,Surface,CAN,Vars,Flags)),
	once(writeObject('$spacer',Vars)),
	once(writeObject(ff(Flags),Vars)),
	once(writeObject('$spacer',Vars)),
	flag(clause_id,_,0),
	Result = 'on',
	tam(Surface,CAN,Flags,Vars,Context,TN,Maintainer,Result),
	flag(clause_id,CLID,CLID),
	format('<hr>Clauses: ~w',[CLID]).

	

tam(Surface,and(CAN1,CAN2),Flags,Vars,Context,TN,Maintainer,Result):-!,
	tam(Surface,CAN1,Flags,Vars,Context,TN,Maintainer,Result),
	tam(Surface,CAN2,Flags,Vars,Context,TN,Maintainer,Result).
tam(Surface,entails(true,CAN2),Flags,Vars,Context,TN,Maintainer,Result):-!,
	tam(Surface,CAN2,Flags,Vars,Context,TN,Maintainer,Result).
tam(Surface,entails(false,CAN2),Flags,Vars,Context,TN,Maintainer,Result):-!,
	tam(Surface,absurd(CAN2),Flags,Vars,Context,TN,Maintainer,Result).
tam(Surface,true,Flags,Vars,Context,TN,Maintainer,Result):-!.
	

tam(Surface,entails(OAnte,OConsq),Flags,Vars,Context,TN,Maintainer,Result):-!, 
	flag(indent,_,0),
	once(writeObject('$spacer',Vars)),
	once(writeObject_conj(entails(OAnte,OConsq),Vars)),
%	once(writeObject('$spacer',Vars)),
%	once(writeObject(ff(Flags),Vars)),
	once(writeObject('$spacer',Vars)),
	flag(clause_id,CLID,CLID+1),
	logOnFailure(once(putAttributeStructures(Surface,Rule,Context,Flags,entails(OAnte,OConsq),entails(Ante,Consq)))),!,
		convertListNotNeg([Consq],[NConsq]),
		conjunctsToList(Ante,List),%trace,
		reorderAnteceedants([NConsq|List],List,All),!,
		convertListNotNeg(All,AnteListS),!,
		unnumbervars(
				(NConsq,AnteListS,Vars,Context,surf(Context,TN,CLID,Vars)),
				(UNConsq,UAnteListS,UVars,UContext,UCtx,UExplaination)),
		numbervars((UNConsq,UAnteListS,UVars,UContext,UCtx,UExplaination),'$VAR',0,_),% trace,
	length(AnteListS,Cost),
		format('~q.~n',['2'(UNConsq,UAnteListS,Cost,UVars,UContext,UCtx,UExplaination)]),!.
		


tam(Surface,surface,Flags,Vars,Context,TN,Maintainer,Result):-!,
	tam(Surface,Surface,Flags,Vars,Context,TN,Maintainer,Result).
     
% Simple Fact
tam(Surface,OConsq,Flags,Vars,Context,TN,Maintainer,Result):-!,
	flag(indent,_,0),
	once(writeObject('$spacer',Vars)),
	once(writeObject_conj(OConsq,Vars)),
%	once(writeObject('$spacer',Vars)),
%	once(writeObject(ff(Flags),Vars)),
	once(writeObject('$spacer',Vars)),
	flag(clause_id,CLID,CLID+1),
	logOnFailure(once(putAttributeStructures(Surface,Rule,Context,Flags,OConsq,Consq))),!,
	convertListNotNeg([Consq],[NConsq]),
	unnumbervars((NConsq,Ctx,surf(Context,TN,CLID,Vars)),(UConsq,UCtx,UExplaination)),
	numbervars((UConsq,UCtx,UExplaination),'$VAR',0,_),!,
	format('~q.~n',['1'(UConsq,Context,UCtx,UExplaination)]).
	
	

% ======================================================
% Do truth checks and assets surface and NNF
% ======================================================
invokeInsert(Driver,fsck,Surface,Ctx,TN,Context,Vars,Maintainer):-!,
		idGen(TN), % create tracking now if needed
	        destroyTN(Context,TN,_),
		ignore((
		% This may fail and will give an error message
			% Vars come back Numbered
			((
			  logOnFailure(getAssertionClauses(Context,Surface,CAN,Vars,Flags)) ->
				((
						% This may fail but it will give a message
						%getTruthCheckResults(tell,Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,Result),
						%Result = on, 
						% TODO assert the kr terms and delete them from list
						(save_can_to_file(Context,Handle) ->
							format(Handle,'~q.\n',['mooCache'(Surface,CAN,Flags,Vars,Context,TN,Maintainer,on)]);
                                                        assert(mooCache(Surface,CAN,Flags,Vars,Context,TN,Maintainer,Result))),!,
						ignore((
							memberchk(compile,Driver),
							recanonicalizeTN(Context,TN)))
					));
				% This will save errors
						(save_can_to_file(Context,Handle) ->
							format(Handle,'~q.\n',['error1'(Surface,CAN,Flags,Vars,Context,TN,Maintainer,on)]);
                                                        assert(mooCache(Surface,CAN,Flags,Vars,Context,TN,Maintainer,on))),!
			  )),!
				
		)),!.

	
% =================================================
% Assert clean and new (Prolog) 												 
% =================================================
assertaClean(X):-unnumbervars(X,Y),!,asserta(Y).

assertzClean(X):-unnumbervars(X,Y),!, assert(Y).


assertAll([]):-!.
assertAll(end_of_file).
assertAll([H|T]):-!,system_dependant:prolog_notrace((assertAll(H),assertAll(T))),!.
assertAll((H,T)):-!,notace((assertAll(H),assertAll(T))),!.	
assertAll(':-'(T)):-!,call(T).
assertAll(InContext):-isClaused(InContext),!.
assertAll(T):-catch(asserta(T),E,format('~n% prolog warning ~w ~n',[E])),!. 

isClaused(InContext):-
	system_dependant:prolog_notrace(not(not((numbervars(InContext,'$VAR',0,_),!,isClausedG(InContext))))),!.
	
isClausedG(mooCache(C,A,ExplainationID:KRVars:KR,Context,TN)):-
	clause(mooCache(C,A,_:_:_,Context,_,_),true,OldID),
	clause(mooCache(OC,OA,_,_,_,_),true,OldID),
	numbervars(OC:OA,'$VAR',0,_),
	C:A==OC:OA,!,ifInteractive(write(',')),!.

isClausedG(mooCache(C,ExplainationID:KRVars:KR,Context,TN)):-
	clause(mooCache(C,_:_:_,Context,_,_),true,OldID),
	clause(mooCache(OC,_,_,_,_),true,OldID),
	numbervars(OC,'$VAR',0,_),
	C==OC,!,ifInteractive(write(',')),!.

isClausedG(mooCache(_:_,C,Context,_,_)):-
	clause(mooCache(_:_,C,Context,_,_),true,OldID),
	clause(mooCache(_:_,OC,Context,_,_),true,OldID),
	numbervars(OC,'$VAR',0,_),
	C==OC,!,ifInteractive(write(',')),!.

isClausedG(InContext:-B):-isClausedG(InContext,B),!.
isClausedG(InContext):-isClausedG(InContext,true),!.

isClausedG(C,A):-
	clause(C,A,OldID),
	clause(OC,OA,OldID),
	numbervars(OC:OA,'$VAR',0,_),
	C:A==OC:OA,!.
	
		
countAssertions(C:-A,N):-countAssertions(C,A,N).
countAssertions(C,N):-countAssertions(C,true,N).	

countAssertions(C,A,N):-
       % numbervars(C:A,'$VAR',0,_),
	flag(clauses_count,_,0),
	clause(C,A,ID),
	flag(clauses_count,X,X+1),fail.
countAssertions(C,A,N):-flag(clauses_count,N,N),!.


% ===================================================================
% MOO TM (Surface)
% ===================================================================
	 
% Assertion Time Errors

% Contradiction: The assertion contradicts other assertion(s) in the knowledge base. ; RAP note: this should be followed by a explaination as per the XML element definition for "explaination" 
% (done/implied) Syntax error: Illegal character in assertion 
% (done) Syntax error: Unmatched parentheses in assertion 
% (done)  Syntax error: Missing parentheses in assertion 
% (done/implied) Syntax error: Unspecified 
% Argument type violation ; RAP note: this should be followed by a explaination of the type violation as per the XML element definition for "explaination" 
% Out of memory error 
% (in Java) Broken socket: The connection between the web-based GUI and the belief engine is broken 
% (done to test)  Redundant assertion: ; RAP note: this should be followed by a explaination of the type violation as per the XML element definition for "explaination" 
% (done to test)  Undefined constant: Do you wish to add the constants to the Context? ; RAP note: this should be followed by a list of constants and a prompt to the user 

/* 
% Context/Ctx Must Be loaded
getTruthCheckResults(Action,Driver,surface,Formula,Ctx,TN,SContext,Vars,Maintainer,notice(' You need to load the Context. Would you like to do so now?',not(isKnowledgeBaseLoaded(SContext,Ctx)))):-
		not(isKnowledgeBaseLoaded(SContext,Ctx)),!.
*/

% ===================================================================
% MOO Consitancy checking
% ===================================================================

% This next line disables truth checks on everything 
getTruthCheckResults(AskInsert,Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,on):-!.

% Normalize the Forms and make appropiate checks
getTruthCheckResults(tell,Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,Result):-
	unnumbervars((Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer),(USurface,UCAN,UFlags,UCtx,UTN,UContext,UVars,UMaintainer)),
	numbervars((USurface,UCAN,UFlags,UCtx,UTN,UContext,UVars,UMaintainer),'$VAR',0,_),
	getTruthCheckResultsInsert(Driver,USurface,UCAN,UFlags,UCtx,UTN,UContext,UVars,UMaintainer,Result),!.

getTruthCheckResults(Ask,Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,Result):- !,
	unnumbervars((Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer),(USurface,UCAN,UFlags,UCtx,UTN,UContext,UVars,UMaintainer)),
	numbervars((USurface,UCAN,UFlags,UCtx,UTN,UContext,UVars,UMaintainer),'$VAR',0,_),
	getTruthCheckResults_ask(Driver,USurface,UCAN,UFlags,UCtx,UTN,UContext,UVars,UMaintainer,Result),!.

% ===================================================================
% MOO Consitancy For Assert/Insert
% ===================================================================

% This next line disables truth checks on Insert
getTruthCheckResultsInsert(Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,on):-!.

% Allow Trusted Driver
getTruthCheckResultsInsert(Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,on):-memberchk(trusted,Driver),!.

% Surface is Redundant and On?
getTruthCheckResultsInsert(Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,notice(' Redundant assertion <pre>~s</pre> original author was ~w.\n',[MaintainerForm,PMaintainer])):-
	mooCache(_,surface,Surface,_,Context,PTN,PMaintainer,_),!,flag(indent,_,0),flag(explaination_linenumber,_,0),
	isMooOption(client=E),
	toMarkUp(E,surf(SContext,TN),OldVars,MaintainerForm),!.
	
% Surface Contants must forall be declared
getTruthCheckResultsInsert(Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,Result):-
	isMooOption(opt_deduce_domains=off),       
		once(getConstants(atomic,Surface,UsedConstants,_,_)),	
		getTruthCheckResults_constants(Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,Result),!.
		
getTruthCheckResults_constants(Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,
	notice('Undefined constant: ~w\nTry asserting the as "(instance -Word- -Something-)"',[UnDefinedList])):-
		logOnFailure(checkAllConstantsHaveTypes(Formula,Constants,UnDefinedList)),
		UnDefinedList=[_|_],!. %TODO

% Surface Contants must forall be declared
getTruthCheckResultsInsert(Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,Result):-
	isMooOption(opt_deduce_domains=off),       
		once(getConstants(atomic,Surface,UsedConstants,_,_)),	
		getTruthCheckResults_constants(Driver,Surface,CAN,Flags,Ctx,TN,Context,Vars,Maintainer,Result),!.


% Each predicate has adequate nth-domains
/* TDODO
getTruthCheckResults(Action,Driver,surface,Formula,Ctx,TN,SContext,Vars,Maintainer,notice(' Relation missing Domian constaints',R)):-
		Formula=..[Relation|ARGS],
		relation_missing_nth_domains_l([Relation|ARGS],R),!.
*/		
relation_missing_nth_domains(Formula,domain(R,Missing,_)):-
		Formula=..[R|ARGS],
		length(ARGS,L),
		is_nth_domain_missing(Relation,L,Missing),!.
relation_missing_nth_domains(Formula,nv(MissingList)):-
		Formula=..[_|ARGS],
		relation_missing_nth_domains_l(ARGS,MissingList).

relation_missing_nth_domains_l([],[]):-!.
relation_missing_nth_domains_l([A|RGS],[Missing|List]):-
		relation_missing_nth_domains(A,Missing),
		relation_missing_nth_domains_l(RGS,List).

% Nth Domains

getTruthCheckResults(Action,Driver,surface,Formula,Ctx,TN,SContext,Vars,Maintainer,notice('Nth-domain violations ~w.\n',[BadList])):-	 
       Formula =.. [V|Ector],
       logOnFailure(once(nth_domain_check_surface_expression(V,1,Ector,BadList))),memberchk(and(_,_),BadList).

% Relation on Head
getTruthCheckResults(Action,Driver,surface,Formula,Ctx,TN,SContext,Vars,Maintainer,notice('Clause heads must be relations. "~w"\n',['instance'(V,AC)])):-
       Formula =.. [V|Ector],not(arg_meets_class_contraint(V,VS,'Relation')),!.
       	

getTruthCheckResults(Action,Driver,surface,Formula,Ctx,TN,SContext,Vars,Maintainer,accept('Passed Checks')):-!.


nth_domain_check_surface_expression(V,N,[],[]):-!.
nth_domain_check_surface_expression(V,N,[E|Ctor],[B|AdList]):-
		nth_domain_each_arg(V,N,E,B),
		NN is N + 1,
		nth_domain_check_surface_expression(V,NN,Ctor,AdList).
				
nth_domain_each_arg(V,N,E,null):-isSlot(E),!.

nth_domain_each_arg(P,N,E,(and('instance'(E,EC),'domain'(P,N,PC)))):-
	is_nth_domain_of(P,N,PC),
	not(arg_meets_class_contraint(E,EC,PC)).

nth_domain_each_arg(P,N,E,(and('instance'(E,EC),'domainSubclass'(P,N,PC)))):-
	is_nth_domain_of(P,N,PC),
	not(arg_meets_class_contraint(E,EC,PC)).

nth_domain_each_arg(V,N,E,null):-!.

	
% (not A:-u(A, holds('Class-Class', 'subclass', B, C), 'Formula'), not holds('Entity-Class', 'instance', D, C), holds('Entity-Class', 'instance', D, B))

        
checkAllConstantsHaveTypes(Formula,[],[]):-!.

checkAllConstantsHaveTypes('instance'(C,H),[TC|List],UnDefinedList):-C==TC,!, 
		checkAllConstantsHaveTypes('instance'(C,H),List,UnDefinedList).

checkAllConstantsHaveTypes('subclass'(C,H),[TTC|List],UnDefinedList):-C==TC,!,
		checkAllConstantsHaveTypes('subclass'(C,H),List,UnDefinedList).

checkAllConstantsHaveTypes('subAttribute'(C,H),[TC|List],UnDefinedList):-C==TC,!,
		checkAllConstantsHaveTypes('subAttribute'(C,H),List,UnDefinedList).

checkAllConstantsHaveTypes('subcontext'(C,H),[TC|List],UnDefinedList):-C==TC,!,
		checkAllConstantsHaveTypes('subcontext'(C,H),List,UnDefinedList).

checkAllConstantsHaveTypes('subrelation'(C,H),[TC|List],UnDefinedList):-C==TC,!,
		checkAllConstantsHaveTypes('subrelation'(C,H),List,UnDefinedList).
		
checkAllConstantsHaveTypes(Formula,[C|List],UnDefinedList):-
		member(C,['$VAR',exists,forall,'instance','subclass',and,=>,or,<=>,not,not,'Entity','Context','KnowledgeBase']),!,
		checkAllConstantsHaveTypes(Formula,List,UnDefinedList).

checkAllConstantsHaveTypes(Formula,[C|List],UnDefinedList):-number(C),!, % Numbers
		checkAllConstantsHaveTypes(Formula,List,UnDefinedList).

checkAllConstantsHaveTypes(Formula,[C|List],UnDefinedList):-catch(atom_codes(C,[34|Codes]),_,fail),!, %Strings
		checkAllConstantsHaveTypes(Formula,List,UnDefinedList).

checkAllConstantsHaveTypes(Formula,[C|List],UnDefinedList):-
		in_cache('instance'(C,_),SContext,SCtx,O),!,
		checkAllConstantsHaveTypes(Formula,List,UnDefinedList).

checkAllConstantsHaveTypes(Formula,[C|List],UnDefinedList):-
		in_cache('subclass'(C,_),SContext,SCtx,O),!,
		checkAllConstantsHaveTypes(Formula,List,UnDefinedList).

checkAllConstantsHaveTypes(Formula,[C|List],UnDefinedList):-
		in_cache('subrelation'(C,_),SContext,SCtx,O),!,
		checkAllConstantsHaveTypes(Formula,List,UnDefinedList).

checkAllConstantsHaveTypes(Formula,[C|List],UnDefinedList):-
		in_cache('subAttribute'(C,_),SContext,SCtx,O),!,
		checkAllConstantsHaveTypes(Formula,List,UnDefinedList).

checkAllConstantsHaveTypes(Formula,[C|List],[C|UnDefinedList]):-!,
		checkAllConstantsHaveTypes(Formula,List,UnDefinedList).
		



% TODO - NTH-DOMAIN CHECK


% TODO - CONTRADICTION CHECK

% Surface Is accepted becasue none of the above cauth a rejection
getTruthCheckResults(Action,Driver,Form,Formula,Ctx,TN,SContext,Vars,Maintainer,accept('Accepted')):-!.	
	
	% ask_pclause_proc(inconsistent(CL),SCtx,SContext,QM,Vars,Result,Explaination),!,
        %  ( (Result > 0) -> ( sendNote('<A Color="red">(Retraction for Hypothetcal)</A>'),assert(tq_skipped),nl,nl,sendNote(user,truthConsistency,'Retract: '(not(CL)),['truthConsistency Says Veto this Assertion ',Explaination]),nl,nl); true),
	 
% ===================================================================
% MOO TM (Canonical Form)
% ===================================================================
	 
% TN Must be Bound
getTruthCheckResults_can(Driver,wfs,CAN,Ctx,TN,Context,Vars,Maintainer,notice(' Prolog Code Error: You Sent a Tracking Number?',TN)):-isSlot(TN),!.

% Surface Contants must forall be declared
getTruthCheckResults_can(Driver,wfs,CAN,Ctx,TN,Context,Vars,Maintainer,notice('Currently Unused Constant in Belief',Const)):-	 
		getConstants(atomic,CAN,UsedConstants,_,_),
		'not-implemented'(Const),member(Const,UsedConstants),!.


% Normalize the Wfs to check Redudant
getTruthCheckResults_can(Driver,wfs,CAN,Ctx,TN,Context,Vars,Maintainer,R):-
	catch(unnumbervars(CAN,RFormula),_,fail),numbervars(RFormula,'$VAR',0,_),
	getTruthCheckResults_can_redundant(CAN,Action,Driver,wfs,RFormula,Ctx,TN,Context,Vars,Maintainer,R),!.

% Wfs is Redundant and On?
getTruthCheckResults_can_redundant(CAN,Action,Driver,wfs,RFormula,Ctx,TN,Context,Vars,Maintainer,notice(' Redundant assertion',author(PMaintainer,STN))):-
	mooCache(PredR,Form,RFormula,Rvars,Context,STN,PMaintainer,on),!.
	
% Wfs is Redundant and Disabled?
getTruthCheckResults_can_redundant(CAN,Action,Driver,wfs,RFormula,Ctx,TN,Context,Vars,Maintainer,notice(' Redundant assertion (and Disabled)',author(PMaintainer,OFF,STN))):-
	mooCache(PredR,Form,RFormula,Rvars,Context,STN,PMaintainer,rejected),!.
	 
% =============================================================================
% HEAD BODY CHECK FOR PROLOG
% =============================================================================

% Clause is ground (Fine)
getTruthCheckResults_can(Driver,Form,CAN,Ctx,TN,Context,Vars,Maintainer,accept('Ground Fact')):-
		getPrologVars(CAN,[],_,_),!.

% No Singles (Fine)
getTruthCheckResults_can(Driver,Form,CAN,Ctx,TN,Context,Vars,Maintainer,accept('Complete')):-
		getPrologVars(CAN,_,[],_),!.

% Head is ground (Fine)
getTruthCheckResults_can(Driver,Form,entails(ANT,CAN) ,Ctx,TN,Context,Vars,Maintainer,warn(Warning)):-
		once(getPrologVars(CAN,[],_,_)),
		once(getPrologVars(CAN,ANT,_,_)),
		O=' ', %toMarkUp(kif,ANT,Vars,O),
		fmtString(S,'Creates ground fact from ante variables ~w (<font color=green>Warning</font>)',[O]),
		string_to_atom(S,Warning),!.
		

% Detect 3 Singletons in Head or Gaf (<font color=red>rejected</font>)
getTruthCheckResults_can(Driver,Form,entails(true,CAN),Ctx,TN,Context,Vars,Maintainer,notice(Warning,'rejected')):-
		once(getPrologVars(CAN,_,[A,B,C|D],_)),
		O=' ', %toMarkUp(kif,[A,B,C|D],Vars,O),
		fmtString(S,'3 or more Universal Variables In Head on Canonicalization ~w (<font color=red>rejected</font>)',[O]),
		string_to_atom(S,Warning),!.

% Detect 2 Singletons in Head or Gaf (<font color=green>warning</font>)
getTruthCheckResults_can(Driver,Form,entails(true,CAN),Ctx,TN,Context,Vars,Maintainer,warn(Warning)):-not(memberchk(test_question,Driver)),
		once(getPrologVars(CAN,_,[A,B|D],_)),
		O=' ', %toMarkUp(kif,[A,B|D],Vars,O),
		fmtString(S,'2 Universal Variables In Head on Canonicalization ~w (<font color=red>rejected</font>)',[O]),
		string_to_atom(S,Warning),!.

% Detect 3 Singletons in Clause (<font color=red>rejected</font>)
getTruthCheckResults_can(Driver,Form,CAN,Ctx,TN,Context,Vars,Maintainer,notice(Warning,'rejected')):-
		once(getPrologVars(CAN,_,[A,B,C|D],_)),
		O=' ', %toMarkUp(kif,[A,B,C|D],Vars,O),
		fmtString(S,'3 or more Universal Variables In Clause on Canonicalization ~w (<font color=red>rejected</font>)',[O]),
		string_to_atom(S,Warning),!.

% Detect No Overlap  (<font color=red>rejected</font>)
getTruthCheckResults_can(Driver,Form,entails(B,CAN),Ctx,TN,Context,Vars,Maintainer,notice('No connection between Head and Body Variables (<font color=red>rejected</font>)','rejected')):-
		once(getPrologVars(CAN,Avars,_,_)),
		once(getPrologVars(B,[BV|BVars],_,_)),
		intersection(Avars,[BV|BVars],[]),!.

% Detect 1 Singleton in Head or Gaf (<font color=green>warning</font>)
getTruthCheckResults_can(Driver,Form,entails(true,CAN),Ctx,TN,Context,Vars,Maintainer,warn(Warning)):-not(memberchk(test_question,Driver)),
		once(getPrologVars(CAN,_,[A|B],_)),
		O=' ', %toMarkUp(kif,[A|B],Vars,O),
		fmtString(S,'Universal Variables In Head on Canonicalization ~w (<font color=red>warning</font>)',[O]),
		string_to_atom(S,Warning),!.
		
% Detect 1 Singleton in Head or Gaf (<font color=green>warning</font>)
getTruthCheckResults_can(Driver,Form,CAN,Ctx,TN,Context,Vars,Maintainer,warn(Warning)):- not(memberchk(test_question,Driver)),
		once(getPrologVars(CAN,_,[A|B],_)),
		O=' ', %toMarkUp(kif,[A|B],Vars,O),
		fmtString(S,'Universal Variables In Canonicalization ~w (<font color=red>warning</font>)',[O]),
		string_to_atom(S,Warning),!.

getTruthCheckResults_can(Driver,Form,CAN,Ctx,TN,Context,Vars,Maintainer,accept('Accepted')):-!.


% TODO Turn on/off

% =========================================
% Limits for Compiler
% =========================================
%clean_clauses((CAN,B),BB):-found_in(CAN,B),!,clean_clauses(B,BB).
clean_clauses((CAN,B),BB):-unused_clause(CAN),!,clean_clauses(B,BB).
clean_clauses((CAN,B),(CAN,BB)):-!,clean_clauses(CAN,CAN),clean_clauses(B,BB).
clean_clauses((equal(X,Y) :- CAN),(same(X,Y) :- CAN)):-!.
clean_clauses((CAN , B),BB):-unused_clause(CAN),!,clean_clauses(B,BB).
clean_clauses((CAN , B),(CAN , BB)):-!,clean_clauses(CAN,CAN),clean_clauses(B,BB).
clean_clauses(CAN,true):-unused_clause(CAN).
clean_clauses(CAN,CAN).

unused_clause(end_of_file).
unused_clause((not(equal(_,_)) :- _ )).
unused_clause((not(same(_,_)) :- _ )).
unused_clause((_ :- not(equal(_,_))  )).
unused_clause(true).
unused_clause(nop).
unused_clause(surf).
%unused_clause((equal(_,_):-_)).
%unused_clause((not(equal(_,_)):-_)).
unused_clause((not('domain-check'(_,_,_)):-_)).
found_in(CAN,B):-CAN==B,!.
found_in(CAN,(B , BB)):- !,
      found_in(CAN,B),
      found_in(CAN,BB).



/*
moo_assert_can_rule_phase2(Consq,ProtoConsq,AnteListS,AnteProto,Vars,Context,Explaination):-
		contridictory([+Consq|AnteListS]),!,
		moo_assert_can_rule_phase3(contridictory,Consq,ProtoConsq,AnteProto,AnteListS,Vars,Context,Explaination).

moo_assert_can_rule_phase2(Consq,ProtoConsq,AnteProto,AnteListS,Vars,Context,Explaination):-
		contridictory([-Consq|AnteListS]),!,
		moo_assert_can_rule_phase3(contridictory,Consq,ProtoConsq,AnteProto,AnteListS,Vars,Context,Explaination).

contridictory(List):-
		findall(Mem,member(+Mem,List),Pos),
		findall(Mem,member(-Mem,List),Negs),!,
		intersection(Pos,Negs,[_|_]).

moo_assert_can_rule_phase2(Consq,ProtoConsq,AnteListS,AnteProto,Vars,Context,Explaination):-
		mooCache(Consq,PBefore,AnteListS,PAfter,PCost,Context,PCtx,PExplaination),
		intersection(PAnteListS,AnteListS,[_|_]),
		is_semantic_duplication(Consq,ProtoConsq,AnteProto,AnteListS,Vars,Context,Explaination,PAnteListS,PBefore,PAfter,PCost,Context,PCtx,PExplaination),!.
		
% Logically entails the same things
is_semantic_duplication(Consq,ProtoConsq,AnteProto,AnteListS,Vars,Context,Explaination,PAnteListS,PBefore,PAfter,PCost,Context,PCtx,PExplaination):-
		subset(PAnteListS,AnteListS),!,
		moo_assert_can_rule_phase3(slower(Consq,PExplaination),ProtoConsq,AnteProto,AnteListS,Vars,Context,Explaination),!.

is_semantic_duplication(Consq,ProtoConsq,AnteProto,AnteListS,Vars,Context,Explaination,PAnteListS,PBefore,PAfter,PCost,Context,PCtx,PExplaination):-
		subset(AnteListS,PAnteListS),!,
		moo_assert_can_rule_phase3(faster(Consq,PExplaination),ProtoConsq,AnteProto,AnteListS,Vars,Context,Explaination),!.

*/

% =====================================================================================
%  retract(Retract_chars,Ctx,TN,Context,User)
% =====================================================================================

bp_retract(Retraction_Chars,Context,TN,L,User):-
      (((idGen(TN),TrackingAtom=(TN)) ; TrackingAtom=TN)),!,
      setMooOption(OptionList),
      write_response_begin,!, %%trace,
      tell_retract_parse_chars(Retraction_Chars,Formula,Vars),
      %%ignore(once(retract(Retraction_Chars,_Ctx,TrackingAtom,Context,User))),
      retract_odbc(Formula,Ctx,TrackingAtom,Context,Vars,User),
      write_response_end.


     /*
retract(Retract_chars,Cxt):-!,
         retract(Retract_chars,Ctx,TN,Context,'Maintainer').
         
retract(Retract_chars,Ctx,TN):- 
         retract(Retract_chars,Ctx,TN,Context,'Maintainer').

retract(Retract_chars,Ctx,TN,Context,User):-!,   
            once(tell_retract_parse_chars(Retract_chars,Pterm,Vars)),
            moo_invoke(retract,forall,surface,Pterm,Ctx,TN,Context,Vars,User).
       */



