-include('moo_header.pl').


/* 
Purpose of this file:

Before a Context is canonicalized we must be able to make simple belief on

(instance AnyTerm ?What)
(domain AnyTerm ?N ?What)
(range AnyTerm ?What)
(subclass AnyTerm ?What)
(valence AnyTerm ?What)

(instance ?What Class)
(instance ?What Relation)
(instance ?What Function)
(instance ?What Predicate)
(instance ?What Attribute)

And get back the 'Simplest' answer first


Also we should be able to in some situations:

(domainSubclass AnyTerm ?N ?What)
(rangeSubclass AnyTerm ?What)

*/

% :-include('moo_header.pl').
% ================================================
% Superfulous Entry
% ================================================




deduceSurfaceGuarded(Fact,Context,Explaination):-	  
	ground(Fact),!,
	deduceSurface_full(Fact,Context,Explaination).
	
deduceSurfaceGuarded(Fact,Context,Explaination):-	  
	not(ground(Fact)),!,
	deduceSurface(Fact,Context,Explaination).
	
getFactForTransitiveClosure(Context,Call,Explaination):-
	deduceSurface_easy(Call,Context,Explaination).

%:-index(deduceSurface_gaf(Fact,Context,Explaination)).
	
% ========================================================
% deduceSurface => deduceSurface_full
% ========================================================

% Non defined presently	except this redirection
deduceSurface_full(Fact,Context,Explaination):-
	deduceSurface(Fact,Context,Explaination).

/*
deduceSurface_full(instance,false,Context,holds(instance,E,C),P1 * P2 * crossref(instance,disjoint)):-
	     deduceSurface(instance(E,M),Context,P1),
	     deduceSurface_full(disjoint(M,C),Context,P2).

deduceSurface_full_reflexive(disjoint(M,C),Context,P2):-
		deduceSurface_full(disjoint(C,M),Context,P2).

deduceSurface_full_reflexive(disjoint(M,C),Context,P2):-
		deduceSurface_full(disjoint(M,C),Context,P2).

deduceSurface_full(PredR,false,Context,holds(instance,E,C),P1 * P2):-
	     deduceSurface(instance(E,M),P1),
	     deduceSurface_full(subclass,false,Context,holds(subclass,M,C),P2),M\=C.
*/	     

% ========================================================
% Easy Forwardchains
% ========================================================

deduceSurface_easy(Fact,Context,Explaination):-
	deduceSurface_gaf(Fact,Context,Explaination).
	


/*
deduceSurface_easy(instance(X,Class),Context,Explainations):-
	atom(Class),deduceSurface_dc_instance1(X,Class,Context,Explainations).
*/
deduceSurface_gaf_sym(Fact,Context,P1):-deduceSurface_gaf(Fact,Context,I).
deduceSurface_gaf_sym(Fact,Context,P1):-compound(Fact),
	Fact=..[F,A,B],
	Term=..[F,B,A],
	deduceSurface_gaf(Term,Context,I).

deduceSurface_gaf(Fact,Context,surf(Context,_Ctx,TN,0)):-
	mooCache(Fact,_,Flags,[],Context,_Ctx,TN,Maintainer,TMResult).
	

% ========================================================
% subclass/partition True
% ========================================================

deduceSurface_easy(subclass(A,B),Context,Explaination * incode(holds(subclass,A,B),crossref(Composition,subclass))):-
	member(Composition,[disjointDecomposition,exhaustiveDecomposition,partition]),
	deduceSurface_gaf(G,Context,Explaination),
	G=..[Composition,B|Children],
%	not(B='Entity'),
	member(A,Children),
%	not(A='Entity'),
	A\=B.
% ====================================================
% make_disjoint_classes(Context,Ctx)
% ===================================================

deduceSurface_easy(disjoint(A,B),Context,  Explaination * incode(holds(disjoint,A,B),Composition) ):-%true,
	member(Composition,[disjointDecomposition,exhaustiveDecomposition,partition]),
	deduceSurface_gaf(G,Context,Explaination),
	G=..[Composition,C|Children],
	%`not(C='Entity'),
	member(A,Children),
	member(B,Children),
	A\=B.


% ================================================
% Normal Forwardchains
% ================================================

deduceSurface(valence(Predicate,N),Context,Explaination):-!,
	deduceValence(Context,Predicate,N,Explaination),!.

deduceSurface(Fact,Context,Explaination):-
	deduceSurface_easy(Fact,Context,Explaination).

deduceSurface(instance(X,Class),Context,incode(instance(X,Class),'Found in Class Constants')):-
	Class='Class',!,
	getClassesListFromContext(Rs,Context,Ctx),
	member(X,Rs).		

deduceSurface(instance(X,Class),Context,incode(instance(X,Class),'Found in Predicate Constants')):-
	Class='Predicate',!,
	getPredicatesListFromContext(Rs,Context,Ctx),
	member(X,Rs).	
	
deduceSurface(instance(X,Class),Context,incode(instance(X,Class),'Found in Attribute Constants')):-
	Class='Attribute',!,
	getAttributeNamelistFromContext(Rs,Context,Ctx),
	member(X,Rs).	

deduceSurface(instance(X,Class),Context,incode(instance(X,Class),'Found in Relation Constants')):-
	Class='Relation',!,
	getRelationsListFromContext(Rs,Context,Ctx),
	member(X,Rs).		

deduceSurface(instance(X,Class),Context,incode(instance(X,Class),'Found in Function Constants')):-
	Class='Function',!,
	getFunctionListFromContext(Rs,Context,Ctx),
	member(X,Rs).		

deduceSurface(instance(E,C),Context,P1 * P2 *  Explaination):-
	deduceSurface_easy(instance(E,M),Context,P1),
	not((M=='Entity')),
	deduceSurface_full(subclass(M,C),Context,P2),
	(M\==C),
	Explaination=sfindi((holds(subclass,M, C)=>forall(E,( holds(instance,E, M)=>holds(instance,E, C))))).
	     	
% ========================================================
% subrelations
% ========================================================

deduceSurface(subrelation(S,C),Context,Explaination):-!,
	deduceTransitiveClosure_PartialOrderingRelation(Context,subrelation,S,C,Explaination),not((S=C)).
						 
% ========================================================
% instance true
% ========================================================

deduceSurface(subclass(S,C),Context,Explaination):-!,
	deduceTransitiveClosure_PartialOrderingRelation(Context,subclass,S,C,Explaination),not((S=C)).

deduceSurface(instance(R,C),Context, (P1*P2*P3)):-
	deduceSurface_easy(subrelation(R,P),Context,P1),R\=P,
	deduceSurface_easy(instance(C,'InheritableRelation'),Context, P2), 
	deduceSurface(instance(P,C),Context, P3).


:-retractall(mooCache(Context,_Ctx,completed_table(_))).


deduceSurface(disjoint(A,B), P1 * P2 * P3 ):-
	(nonvar(A);nonvar(B)),
	deduceSurface(holds(disjoint,SuperOfB,SuperOfA),P1),
	deduceTransitiveClosure_PartialOrderingRelation(Context,subclass,A,SuperOfA,P2),
	deduceTransitiveClosure_PartialOrderingRelation(Context,subclass,B,SuperOfB,P3),
	((A\=SuperOfA);(B\=SuperOfB)).
	%TODO((A=AA,B=BB);(B=AA,A=BB)).


deduceSurface(domain(disjointDecomposition,N,'Class'),Context, incode(domain(disjointDecomposition,N,'Class'))):-integer(N),!.
deduceSurface(domain(exhaustiveDecomposition,N,'Class'),Context, incode(domain(exhaustiveDecomposition,N,'Class'))):-integer(N),!.
deduceSurface(domain('AssignmentFn',1,'Function'),Context, incode(domain('AssignmentFn',1,'Function'))):-!.
deduceSurface(domain('AssignmentFn',N,'Entity'),Context, incode(domain(AssignmentFn,N,'Entity'))):-integer(N),N>1,!.
deduceSurface(domain('holds',1,'Relation'),Context, incode(domain('holds',N,'Relation'))):-!.
deduceSurface(domain('holds',N,'Entity'),Context, incode(domain('holds',N,'Entity'))):-integer(N),N>1,!.


deduceSurface(domain(R,N,S),Context,(P1 * P2 * Explaination)):-
	deduceSurface_full(subrelation(R,P),Context,P1),
	R\=P,
	deduceSurface_full(domain(P,N,S),Context, P2),
	Explaination=(holds(subrelation,R, P)and domain(P, N, S)and domain(R, N, S)=>holds(subclass,S, S)).

deduceSurface(domain(R,N,S),Context, P1 * Explaination):-
	nonvar(N),
	deduceSurface(valence(R,N),Context,P1),
	deduceSurface(range(R,S),Context, Explaination),!.

deduceSurface(domain(R,N,S),Context,P1 * Explaination):-
	nonvar(N),
	deduceSurface_full(subrelation(R,PredR),Context,P1),R\=P,
	deduceSurface(valence(P,N),Context,_),
	deduceSurface(range(P,S),Context, Explaination),!.
	

deduceSurface(domainSubclass(R,N,S),Context,(P1 * P2 * Explaination)):-
	deduceSurface_full(subrelation(R,P),Context,P1),
	R\=P,
	deduceSurface_full(domainSubclass(P,N,S),Context, P2),
	Explaination=(holds(subrelation,R, P)and domainSubclass(P, N, S)and domainSubclass(R, N, S)=>holds(subclass,S, S)).

deduceSurface(domainSubclass(R,N,S),Context, P1 * Explaination):-
	nonvar(N),
	deduceSurface(valence(R,N),Context,P1),
	deduceSurface(rangeSubclass(R,S),Context, Explaination),!.

deduceSurface(domainSubclass(R,N,S),Context,P1 * Explaination):-
	nonvar(N),
	deduceSurface_full(subrelation(R,PredR),Context,P1),R\=P,
	deduceSurface(valence(P,N),Context,_),
	deduceSurface(rangeSubclass(P,S),Context, Explaination),!.

deduceSurface(domain(R,N,'Class'),Context, Explaination):-
	nonvar(N),nonvar(R),
	deduceSurface(domainSubclass(R,N,S),Context, Explaination),!.

deduceSurface(range(R,'Class'),Context, Explaination):-
	nonvar(N),nonvar(R),
	deduceSurface(rangeSubclass(R,N,S),Context, Explaination),!.


% ==========================================================
% Instance Deduction (Specialization) via Domains
% ==========================================================
%domain(A, 2, B)=>forall(C, forall(D, holds(A, D, C)=>holds(instance,C, B)))

expireOptimizationsInContext(Context,Assertion):-
	%writeDebug(yellow,'erasing instancell'),
	retractall((mooCache(Context,_Ctx,deduceInstanceTable(Class,Set)))).

	
instance_all(Context,Predicate,Args):-
	deduceSurfaceDomainVector(N,Predicate,VectS,Context,Explainations),
	deducePossibleInstancesFromClasslist(Context,VectS,Args).

deducePossibleInstancesFromClasslist(Context,[],[]).
deducePossibleInstancesFromClasslist(Context,[Class|Classes],[Arg|ArgS]):-
	deduceInstanceTable(Context,Arg,Class,_),
	deducePossibleInstancesFromClasslist(Context,Classes,ArgS).
	


deduceInstanceTable(Context,Arg,'Entity',_):-
	writeDebug(red,deduceInstanceTable(Context,Arg,'Entity')),!,fail.

deduceInstanceTable(Context,Arg,'Class',incode(instance(Arg,'Class'),'Found in Class Constants')):-
	getClassesListFromContext(Rs,Context,Ctx),!,
	member(Arg,Rs).

deduceInstanceTable(Context,Arg,'Relation',incode(instance(Arg,'Relation'),'Found in Relation Constants')):-
	getRelationsListFromContext(Rs,Context,Ctx),!,
	member(Arg,Rs).

deduceInstanceTable(Context,Arg,'Predicate',incode(instance(Arg,'Predicate'),'Found in Predicate Constants')):-
	getPredicatesListFromContext(Rs,Context,Ctx),!,
	member(Arg,Rs).

deduceInstanceTable(Context,Arg,'Function',incode(instance(Arg,'Function'),'Found in Function Constants')):-
	getFunctionListFromContext(Rs,Context,Ctx),!,
	member(Arg,Rs).
	
deduceInstanceTable(Context,Arg,'Attribute',incode(instance(Arg,'Attribute'),'Found in Attribute Constants')):-
	getAttributeNamelistFromContext(Rs,Context,Ctx),!,
	member(Arg,Rs).
	

deduceInstanceTable(Context,Arg,Class,incode(instance(Arg,Class),'Found in Defined Constants')):-
	mooCache(Context,_Ctx,deduceInstanceTable(Class,Set)),
	%writeDebug(silver,extent(holds(instance,Class,Set))),
	!,
	member(Arg,Set).
	
deduceInstanceTable(Context,Arg,Class,incode(instance(Arg,Class),'Found in Defined Cached Constants')):-
	atom(Class),
	findall(A,
			deduceSurface(instance(Arg,Class),Context,Explaination),
		List),sort(List,Set),!,
	asserta(mooCache(Context,_Ctx,deduceInstanceTable(Class,Set))),
	writeDebug(green,made_extent(Context,deduceInstanceTable(Class,Set))),!,
	member(Arg,Set).
     


expireOptimizationsInContext(Context,Assertion):-
		writeDebug(yellow,'erasing deduceInstanceTable'),
		retractall((mooCache(Context,_Ctx,deduceInstanceTable(Class,Set)))).
	


getRelationsListFromContext:-retractall(mooCache(Context,_Ctx,relation_list(Rs))),!,getRelationsListFromContext(Rs,Context,Ctx),write(Rs).


getRelationsListFromContext(Rs,Context,Ctx):-
	mooCache(Context,_Ctx,relation_list(Rs)),!.
getRelationsListFromContext(Rs,Context,Ctx):-
	findall(X,
		(
			(
				(
					(
						deduceTransitiveClosure_PartialOrderingRelation(Context,subclass,M,'Relation',SCExplaination),
						deduceSurface_gaf(instance(X,M),Context,Explaination)
					);
					(
					deduceSurface_gaf(GAF,Context,_Explaination),
					((
					  GAF=subrelation(X,_);GAF=subrelation(_,X);
					  GAF=inverse(X,_);GAF=inverse(_,X);
					  GAF=range(X,_);GAF=domain(X,1,_);GAF=domainSubclass(X,_,_);GAF=rangeSubclass(X,_,_)
					  
					  ))
				)
			),
			atom(X)
		)),Relations),
	sort(Relations,Rs),
	asserta(mooCache(Context,_Ctx,relation_list(Rs))),!.

getAttributeNamelistFromContext(Rs,Context,Ctx):-
	mooCache(Context,_Ctx,attribute_list(Rs)),!.
getAttributeNamelistFromContext(Rs,Context,Ctx):-
	findall(X,
		(
			(
				(
					(
						deduceTransitiveClosure_PartialOrderingRelation(Context,subclass,M,'Attribute',SCExplaination),
						deduceSurface_gaf(instance(X,M),Context,Explaination)
					);
					(
					deduceSurface_gaf(GAF,Context,_Explaination),
					((GAF=subAttribute(X,_);
						GAF=subAttribute(_,X);
							GAF=successorAttribute(X,_);
								GAF=successorAttribute(_,X);
									GAF=contraryProperty(X,_);
									GAF=contraryProperty(_,X);
									GAF=attribute(X,_)))
				)                                   
			),
			atom(X)
		)),Attributes),
	sort(Attributes,Rs),
	asserta(mooCache(Context,_Ctx,attribute_list(Rs))),!.


getPredicatesListFromContext(Rs,Context,Ctx):-
	mooCache(Context,_Ctx,predicatesListFromContext(Rs)),!.
getPredicatesListFromContext(Rs,Context,Ctx):-
	getRelationsListFromContext(RR,Context,Ctx),!,
	findall(X,
		((
		member(X,RR),
		not(atom_concat(_,'Fn',X))
		)),Relations),
	sort(Relations,Rs),
	asserta(mooCache(Context,_Ctx,predicatesListFromContext(Rs))),!.

getFunctionListFromContext(Rs,Context,Ctx):-
	mooCache(Context,_Ctx,functionListFromContext(Rs)),!.
getFunctionListFromContext(Rs,Context,Ctx):-
	getRelationsListFromContext(RR,Context,Ctx),!,
	findall(X,
		((
		member(X,RR),
		atom_concat(_,'Fn',X)
		)),Relations),
	sort(Relations,Rs),
	asserta(mooCache(Context,_Ctx,functionListFromContext(Rs))),!.

getClassesListFromContext(Rs,Context,Ctx):-
	mooCache(Context,_Ctx,classListFromContext(Rs)),!.
getClassesListFromContext(Rs,Context,Ctx):-
	findall(X,
		((
					deduceSurface_gaf(GAF,Context,_Explaination),
					((GAF=subclass(_,X);GAF=subclass(X,_);GAF=disjoint(_,X);GAF=disjoint(X,_);GAF=instance(_,X);GAF=range(_,X);
						deduceSubclassBySurfacePartition(GAF,Classes)
					)),nonvar(X))),Classes),
	sort(Classes,Rs),
	asserta(mooCache(Context,_Ctx,classListFromContext(Rs))),!.
			
deduceSubclassBySurfacePartition(GAF,A):-
	GAF=..[Composition|Children],
	member(Composition,[disjointDecomposition,exhaustiveDecomposition,partition]),
	member(A,Children).
/*
deduceSurface_dc_instance(X,Class,Context,Explainations  * SCExplaination):-%var(X),var(Class),!,
	atom(Class),
	deduceSurface_dc_instance1(X,M,Context,Explainations),
        deduceTransitiveClosure_PartialOrderingRelation(Context,subclass,M,Class,SCExplaination).
*/
       /*
deduceSurface_dc_instance1(X,Class,Context,argOf(Predicate,N,X)):-%var(X),var(Class),!,
	deduceSurfaceDomainVector(_Arity,Predicate,VectS,Context,Explainations),
	not(atom_concat(_,'Fn',Predicate)),
	nth1(N,VectS,Class),
	deduceSurfaceEntityInRelationArg(N,Predicate,X,Context,ExplainationO).
	 */

% ==================================================
% deduceSurfaceEntityInRelationArg(N,Predicate,VectS,Context,ExplainationO)
%
% (un)precacheSurfaceEntityInRelationArg(Context,Ctx)
% ==================================================

deduceSurfaceEntityInRelationArg(N,Predicate,X,Context,ExplainationO):-
	precacheSurfaceEntityInRelationArg(Context,Ctx),!,
	mooCache(Context,_Ctx,arg_vector(Predicate,N,X,ExplainationO)).
	
precacheSurfaceEntityInRelationArg(Context,Ctx):-mooCache(Context,_Ctx,done(precacheSurfaceEntityInRelationArg)),!.

/*
precacheSurfaceEntityInRelationArg(Context,Ctx):-
	mooCache(Fact, surface, Context,  Explaination),
	precacheSurfaceEntityInRelationArg_util(Fact,Classification,Context,Explaination),fail.

precacheSurfaceEntityInRelationArg(Context,Ctx):-
	mooCache(Predicate,HLFact,HLConds,_,  Context,  Explaination),
	precacheSurfaceEntityInRelationArg_util(entails(HLFact,HLConds),Classification,Context,Explaination),fail.
*/
	
precacheSurfaceEntityInRelationArg(Context,Ctx):-assert(mooCache(Context,_Ctx,done(precacheSurfaceEntityInRelationArg))),!.
		
expireSurfaceEntityInRelationArg(Context,Ctx):-
	 retractall(mooCache(Context,_Ctx,arg_vector(Predicate,N,VectS,_))),
	  retractall(mooCache(Context,_Ctx,done(precacheSurfaceEntityInRelationArg))).

% ==================================================
% Memorize Constants
%		precacheSurfaceEntityInRelationArg_util(entails(HLFact,HLConds),Classification,Context,Explaination),
%		precacheSurfaceEntityInRelationArg_util(HLFact,Classification,Context,Explaination),
% ==================================================

precacheSurfaceEntityInRelationArg_util(A,Classification,Context,Explaination):-(isSlot(A);string(A);atom(A);number(A)),!.

precacheSurfaceEntityInRelationArg_util([H|T],Classification,Context,Explaination):-!,
	precacheSurfaceEntityInRelationArg_util(H,Classification,Context,Explaination),!,
	precacheSurfaceEntityInRelationArg_util(T,Classification,Context,Explaination).
precacheSurfaceEntityInRelationArg_util(Formula,Classification,Context,Explaination):-
		Formula=..[holds,P|Args],!,
		precacheSurfaceEntityInRelationArg_util_holdsN(P,1,Args,Classification,Context,Explaination).
precacheSurfaceEntityInRelationArg_util(skolem(_,_),Classification,Context,Explaination):-!.
precacheSurfaceEntityInRelationArg_util(Formula,Classification,Context,Explaination):-
		Formula=..[_|Args],!,
		precacheSurfaceEntityInRelationArg_util(Args,Classification,Context,Explaination).
	
precacheSurfaceEntityInRelationArg_nosaveRelation(precacheSurfaceEntityInRelationArg_nosaveRelation). %dummy 

precacheSurfaceEntityInRelationArg_util_holdsN(P,N,_,Classification,Context,Explaination):-precacheSurfaceEntityInRelationArg_nosaveRelation(P),!.
precacheSurfaceEntityInRelationArg_util_holdsN(P,N,[],Classification,Context,Explaination).
precacheSurfaceEntityInRelationArg_util_holdsN(P,N,[A|RGS],Classification,Context,Explaination):-
	precacheSurfaceEntityInRelationArg_argN(PredR,P,N,A,Classification,Context,Explaination),
	NN is N+1,
	precacheSurfaceEntityInRelationArg_util_holdsN(P,NN,RGS,Classification,Context,Explaination).
	

precacheSurfaceEntityInRelationArg_argN(PredR,P,N,A,Classification,Context,Explaination):-non_memerable(A),!.
precacheSurfaceEntityInRelationArg_argN(PredR,Predicate,N,VectS,Classification,Context,Explaination):-
	mooCache(Context,_Ctx,arg_vector(Predicate,N,VectS,_)),!.
precacheSurfaceEntityInRelationArg_argN(PredR,Predicate,N,VectS,Classification,Context,Explaination):-
	asserta(mooCache(Context,_Ctx,arg_vector(Predicate,N,VectS,Explaination))),!.

non_memerable(A):-isSlot(A).
%non_memerable(A):-number(A).
%non_memerable(A):-string(A).
%non_memerable(A):-not(atom(A)),!,fail.
%non_memerable(A):-atom_codes(A,[34|_]).

	

%reif(Var):-var(Var),!,fail.
%reif('Entity'):-!,fail.
reif('Formula'):-!,fail.
%reif(N):-number(N),!,fail.
reif(_):-!.

%extentable('Entity'):-!,fail.
%extentable('Entity'):-!,fail.
%extentable('Formula'):-!,fail.
%extentable('SymbolicString'):-!,fail.
%extentable(Var):-var(Var),!,fail.
%extentable(_):-!.



expireOptimizationsInContext(Context,Assertion):-
		writeDebug(silver,'destroy_valence_vectors/expireDomainsListForRelation(Context,Ctx)/expireSurfaceEntityInRelationArg(Context,Ctx)'),
		retractall(mooCache(Context,_Ctx,functionListFromContext(Rs))),
		retractall(mooCache(Context,_Ctx,relation_list(Rs))),
		retractall(mooCache(Context,_Ctx,predicatesListFromContext(Rs))),
		retractall(mooCache(Context,_Ctx,classListFromContext(Rs))),
		retractall(mooCache(Context,_Ctx,attribute_list(Rs))),
		expireSurfaceEntityInRelationArg(Context,Ctx),
		expireDomainsListForRelation(Context,Ctx),
		precacheSurfaceEntityInRelationArg(Context,Ctx).
		



% Get Domain (Will return all predicates and arity with explaination of how it was derived)

deduceSurfaceDomainVector(N,Predicate,VectS,Context,ExplainationO):-
	buildDomainsListForRelation(Context,Ctx),!,
	mooCache(Context,_Ctx,domain_vector(Predicate,N,VectS,ExplainationO)).
	
buildDomainsListForRelation(Context,Ctx):-mooCache(Context,_Ctx,done(buildDomainsListForRelation)),!.
buildDomainsListForRelation(Context,Ctx):- 
	once(getRelationsListFromContext(Rs,Context,Ctx)),
	member(Predicate,Rs),  %true, 	
	once(deduceValence(Context,Predicate,N,_Explaination)),
	once(deduceHoldsNDomVect(N,Predicate,VectS,Context,Ctx)),
	%writeDebug(green,domain_vector(Predicate,N,VectS)),
	asserta_if_new(mooCache(Context,_Ctx,domain_vector(Predicate,N,VectS,cached))),fail.
buildDomainsListForRelation(Context,Ctx):-assert(mooCache(Context,_Ctx,done(buildDomainsListForRelation))),!.
		
expireDomainsListForRelation(Context,Ctx):-
	 retractall(mooCache(Context,_Ctx,domain_vector(Predicate,N,VectS,cached))),
	  retractall(mooCache(Context,_Ctx,done(buildDomainsListForRelation))).


% assert cache/2 
deduceSurface_domain(Predicate,N,Class,Context,Ctx):-
	atom(Predicate),
	deduceSurface(domain(Predicate,N,Class),Context,Explaination2).

:-index(deduceHoldsNDomVect(1,0,1,0,0)).


deduceHoldsNDomVect(2,Predicate,[Class1,Class2],Context,Ctx):-!,
	deduceSurface_domain(Predicate,1,Class1,Context,Ctx),!,
	deduceSurface_domain(Predicate,2,Class2,Context,Ctx),!.

deduceHoldsNDomVect(3,Predicate,[Class1,Class2,Class3],Context,Ctx):-!,
	deduceSurface_domain(Predicate,1,Class1,Context,Ctx),!,
	deduceSurface_domain(Predicate,2,Class2,Context,Ctx),!,
	deduceSurface_domain(Predicate,3,Class3,Context,Ctx),!.

deduceHoldsNDomVect(1,Predicate,[Class1],Context,Ctx):-!,
	deduceSurface_domain(Predicate,1,Class1,Context,Ctx),!.

deduceHoldsNDomVect(4,Predicate,[Class1,Class2,Class3,Class4],Context,Ctx):-!,
	deduceSurface_domain(Predicate,1,Class1,Context,Ctx),!,
	deduceSurface_domain(Predicate,2,Class2,Context,Ctx),!,
	deduceSurface_domain(Predicate,3,Class3,Context,Ctx),!,
	deduceSurface_domain(Predicate,4,Class4,Context,Ctx),!.

deduceHoldsNDomVect(5,Predicate,[Class1,Class2,Class3,Class4,Class5],Context,Ctx):-!,
	deduceSurface_domain(Predicate,1,Class1,Context,Ctx),!,
	deduceSurface_domain(Predicate,2,Class2,Context,Ctx),!,
	deduceSurface_domain(Predicate,3,Class3,Context,Ctx),!,
	deduceSurface_domain(Predicate,4,Class4,Context,Ctx),!,
	deduceSurface_domain(Predicate,5,Class5,Context,Ctx),!.

 

isPropositional(A):-isSlot(A),!.
isPropositional(not(A)):-!,isPropositional(A).
isPropositional(forall(_,A)):-!,isPropositional(A).
isPropositional(exists(_,A)):-!,isPropositional(A).
isPropositional(poss(A)):-!,isPropositional(A).
isPropositional(nec(A)):-!,isPropositional(A).
isPropositional(<=>(A,B)):-!,isPropositional(A),isPropositional(B).
isPropositional(=>(A,B)):-!,isPropositional(A),isPropositional(B).
isPropositional(or(A,B)):-!,isPropositional(A),isPropositional(B).
isPropositional(and(A,B)):-!,isPropositional(A),isPropositional(B).
isPropositional(A):-A=..[P|_],is_instance_of(P,C),is_subclass_of(C,'Predicate').



% ==========================================================
% True/False Instance (Specialization)  based on Asserted Rule
% ==========================================================

prove_goal_instance(Logic,Depth,Table,Entity,EClass,Agent,Context,P ):-!,
	Depth2 is Depth -1,!,
	writeDebug('?'(Logic,Entity,EClass)),   
	(((proveInstance(Logic,Depth2,Table,Entity,EClass,Agent,Context,P),
	nonvar(Entity),nonvar(EClass), 
	writeDebug('+i'(Logic,Entity,EClass))));(!,writeDebug('-i'(Logic,Entity,EClass)),!,fail)).


% ==========================================================
% Instance Deduction (Specializations based only on Structure of Fact)
% ==========================================================


%proveInstance(true,Depth,Table,X,Class,Agent,Context,incode(instance(X,Classes),'Cached')):-
%	atom(X),r_flags(X,Classes),!,member(Class,Classes).

%proveInstance(Logic,Depth,Table,X,Class,Agent,Context,Explaination):-
%	deduceSurfaceGuarded(instance,Logic,instance(X,Class),Agent,Context,Explaination).

proveInstance(Logic,Depth,Table,X,Class,Agent,Context,Explaination):-var(X),!,fail.

proveInstance(Logic,Depth,Table,X,Class,Agent,Context,Explaination):-
	memberchk(defering_to_gafs(Logic,_),Table),!,fail.
	
proveInstance(Logic,Depth,Table,X,Class,Agent,Context, Explaination2 * Explaination):- %true,
	compound(X),X=..[P|List],!,
	proveInstance_compound(Logic,Depth,Table,P,List,X,Class,Agent,Context,Explaination).
	
proveInstance(false,Depth,Table,'Formula',Type,Agent,Context,incode(not 'Formula')).

proveInstance(Logic,Depth,Table,Number,Type,Agent,Context,Explaination):-
	number(Number),!,
	proveInstance_number(Logic,Depth,Table,Number,Type,Agent,Context,Explaination).

/*
proveInstance(Logic,Depth,Table,Arg1,Arg2,Agent,Context,Explaination * Explaination2):-  !,
	not(member('some_instance'(Arg1),Table)),
	client_rulebase_spec(instance,Logic,instance(Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type),
	atom(Arg2),
	confirm_rule(instance,Logic,instance(Arg1,Arg2), Agent,Context, Conditions, Explaination, F,Type,Depth,Table,NewTable,NewConds),
	deduceGoal(holds,true,Depth,['Atom_instance'(Arg1),instance(Arg1,Arg2)|NewTable],NewConds,Agent,Context,Explaination2),
	confirm_ground(Conditions).
*/	

proveInstance_number(true,Depth,Table,Number,'RealNumber',Agent,Context,incode(instance(Number,'RealNumber'))).
proveInstance_number(Logic,Depth,Table,Number,Integer,Agent,Context,Explaination):-
	integer(Number),!,
	proveInstance_integer(Logic,Depth,Table,Number,Integer,Agent,Context,Explaination).

proveInstance_number(false,Depth,Table,Number,'Integer',Agent,Context,incode(not(instance(Number,'Integer')))).
	
proveInstance_integer(true,Depth,Table,Number,'Integer',Agent,Context,incode(instance(Number,'Integer'))).
proveInstance_integer(true,Depth,Table,Number,'PositiveInteger',Agent,Context,incode(instance(Number,'PositiveInteger'))):-Number>0.
proveInstance_integer(true,Depth,Table,Number,'NegativeInteger',Agent,Context,incode(instance(Number,'NegativeInteger'))):-Number<0.
proveInstance_integer(false,Depth,Table,Number,'PositiveInteger',Agent,Context,incode(instance(Number,'PositiveInteger'))):-Number<0.
proveInstance_integer(false,Depth,Table,Number,'NegativeInteger',Agent,Context,incode(instance(Number,'NegativeInteger'))):-Number>0.

proveInstance_compound(Logic,Depth,Table,'zzskFn',List,X,C,Agent,Context,Explaination):-!,
	proveInstance_skolem(Logic,Depth,Table,'zzskFn',List,X,C,Agent,Context,Explaination).


% Formula was a skolem
proveInstance_skolem(true,Depth,Table,'zzskFn',List,X,C,Agent,Context,Explaination * Explaination2):- C=='Entiry',!,
	ground(X).
/*

proveInstance_skolem(Logic,Depth,Table,'zzskFn',List,X,C,Agent,Context,Explaination * Explaination2):-
	not_in(instance(X,C),Table),
	client_rulebase(instance,true_'$existential's, instance(X,C),Agent,Context,ConditionsE,Explaination,Type),
	ground(ConditionsE),
	not(proveInstance_skolem_dis(ConditionsE)),
	writeDebug(green,'<='('E_instance'(X,C),ConditionsE)),
	deduce_backchain(true,Depth,[instance(X,C)|Table],ConditionsE,Agent,Context,Explaination2),!.
*/		

proveInstance_skolem_dis(not(A)):-!,
	proveInstance_skolem_dis(A).
proveInstance_skolem_dis(instance(_,C)):-not(atom(C)).
proveInstance_skolem_dis(ConditionsE):-functor(ConditionsE,F,N),proveInstance_skolem_dis(F,N).
proveInstance_skolem_dis(and,N).
proveInstance_skolem_dis(_,N):-N>4.
proveInstance_skolem_dis(ON,N):-atom_concat(_,'On',ON).

 
% Formula is a Function with Range
proveInstance_compound(true,Depth,Table,P,List,X,Class,Agent,Context,Explaination * Explaination2 * incode(instance(X,Class),'Structural Instances')):-
	deduceSurface(instance,true,Context,instance(P,'Function'),Explaination),
	deduceSurfaceGuarded(P,true,range( P ,Class),Agent,Context,Explaination2).

% Formula is a Function of not Range
proveInstance_compound(false,Depth,Table,P,List,X,Class,Agent,Context,Explaination * incode(not(range(P,Class))) * incode(instance(X,Class),'Structural Instances')):-
	deduceSurface(instance,true,Context,instance(P,'Function'),Explaination),
	not(deduceSurfaceGuarded(Predicate,true,Table,range(P,Class),Agent,Context,Explaination2)).

% Formual is a Proposition if it is headed be a Relation
proveInstance_compound(true,Depth,Table,P,List,X,'Proposition',Agent,Context,Explaination * incode(instance(X,'Proposition'),'Structural Instances') ):-
	deduceSurface(instance,true,Context,instance(P,'Relation'),Explaination).

% Formula is a Not Proposition if it is headed be a Relation
proveInstance_compound(false,Depth,Table,P,List,X,'Proposition',Agent,Context,(incode(not(instance(X,'Proposition'),'Structural Instances')* Explaination))):-
	deduceSurface(instance,true,Context,instance(P,'Relation'),Explaination).

% Formula is Formula
proveInstance_compound(true,Depth,Table,P,List,X,'Formula',Agent,Context,incode(instance(X,'Formula'),'Structural Instances')).

% Bachain on Formula
/*
proveInstance_compound(true,Depth,Table,P,List,X,C,Agent,Context,Explaination):-
		ground((X,C)),
		client_rulebase(instance,true, instance(X,C),Agent,Context,ConditionsE,_), requistionable(ConditionsE,Type),
		Depth2 is Depth-1,
		deduce_backchain(true,Depth2,[instance(X,C)|Table],ConditionsE,Agent,Context,Explaination). %TODO only one Antecedant Possble
*/



% =====================================================
% index/dynamic All exported predicates
% =====================================================

:-dynamic('PrologMOO_valence2'/6).
:-dynamic(forwardchain_PrologMOO_valence2/6).
:-dynamic(backchain_PrologMOO_valence2/6).

deduceValence(Context,not,1,inlinecode(valence(not,1))):-!.

deduceValence(Context,X,2,inlinecode(valence(X,2))):-
	deduceSurfaceGuarded(instance(X,'SententialOperator'),Context,Explaination),!.

deduceValence(Context,R,N,Explaination):-
	deduceValence_util(Context,R,N,Explaination),!.

% Valence #2
deduceValence(Context,X,N,Explaination * Explaination3 * inlinecode(valence,N)):-
	deduceTransitiveClosure_PartialOrderingRelation(Context,subrelation,X,Super,Explaination3),
	X\=Super,
	deduceValence_util(Context,Super,N,Explaination),!.

deduceValence(Context,X,N,Explaination * Explaination3 * inlinecode(valence,N)):-
	deduceSurface_gaf_sym(disjointRelation(X,Super),Context,Explaination3),
	X\=Super,
	deduceValence_util(Context,Super,N,Explaination),!.
	


	

% ==================================================================================
% Entry Points for singleValued   valence/2
% ==================================================================================
% Valence #1
deduceNegValence(Context,X,YY,Explaination * inlinecode(valence,1)):-
	isValenceInt(YY),
	deduceValence_util(Context,X,Y,Explaination),!,Y=YY.
	
% Valence #10
deduceNegValence(Context,X,YY,Explaination * inlinecode(valence,10)):-
	deduceValence_util(Context,X,Y,Explaination),isValenceInt(YY),Y\=YY,!.

	
% Valence Asserted
deduceValence_util(Context,X,Y,Explaination):-
	mooCache(valence(X,Y), _,_,_,Context,_,_,Explaination),!.
					    
% Valence #6
deduceValence_util(Context,X,2,Explaination * inlinecode(valence,6)):-
	deduceSurfaceGuarded(instance(X,'BinaryPredicate'),Context,Explaination),!.

% Valence #7
deduceValence_util(Context,X,3,Explaination * inlinecode(valence,7)):-
	deduceSurfaceGuarded(instance(X,'TernaryPredicate'),Context,Explaination),!.

% Valence #8
deduceValence_util(Context,X,4,Explaination * inlinecode(valence,8)):-
	deduceSurfaceGuarded(instance(X,'QuaternaryPredicate'),Context,Explaination),!.
	
% Valence #3
deduceValence_util(Context,X,2,Explaination * inlinecode(valence,3)):-
	deduceSurfaceGuarded(instance(X,'UnaryFunction'),Context,Explaination),!.

% Valence #4
deduceValence_util(Context,X,3,Explaination * inlinecode(valence,4)):-
	deduceSurfaceGuarded(instance(X,'BinaryFunction'),Context,Explaination),!.

% Valence #5
deduceValence_util(Context,X,4,Explaination * inlinecode(valence,5)):-
	deduceSurfaceGuarded(instance(X,'TernaryFunction'),Context,Explaination),!.


	


% Utility Predicates

isValenceInt(1).
isValenceInt(2).
isValenceInt(3).
isValenceInt(4).
isValenceInt(5).

	
% ================================================================================================================
% Rules for valence/2
% ================================================================================================================


/* 


Rule # 1 for valence   
  
Forms:
 1. "(=> 
         (instance ?REL VariableArityRelation) 
         (not 
            (exists (?INT) 
               (valence ?REL ?INT))))"
1a. (entails 
         (instance ?REL VariableArityRelation) 
         (not 
            (valence ?REL ?INT)))


Flags: r1([2nd(holds)])

*/

explaination_line(incode('PrologMOO_valence2', 1, ['REL'=A, 'INT'=B|C]), via(entails(holds(instance, A, 'VariableArityRelation'), not holds(valence, A, B)), ['REL'=A, 'INT'=B|C])*surf('PrologMOO', 1628)).

/*
'PrologMOO_~valence'( A, B, 'BASE ONTOLOGY', E, E*D*incode('PrologMOO_valence2', 1, ['REL'=A, 'INT'=B|C])) :- 
	forwardchain_instance(true, holds(instance, A, 'VariableArityRelation'), 'BASE ONTOLOGY', D)
*/


/* 


Rule # 2 for valence   
  
Forms:
 2. "(=> 
         (instance ?REL QuintaryPredicate) 
         (valence ?REL 5))"
2a. (entails 
         (instance ?REL QuintaryPredicate) 
         (valence ?REL 5))


Flags: r1([2nd(holds)])

*/

explaination_line(incode('PrologMOO_valence2', 2, ['REL'=A|B]), via(entails(holds(instance, A, 'QuintaryPredicate'), holds(valence, A, 5)), ['REL'=A|B])*surf('PrologMOO', 1622)).

/*
deduceValence_util(Context, A, 5, 'BASE ONTOLOGY', D, D*C*incode('PrologMOO_valence2', 2, ['REL'=A|B])) :- 
	forwardchain_instance(true, holds(instance, A, 'QuintaryPredicate'), 'BASE ONTOLOGY', C)
*/


/* 


Rule # 3 for valence   
  
Forms:
 3. "(=> 
         (instance ?REL QuaternaryPredicate) 
         (valence ?REL 4))"
3a. (entails 
         (instance ?REL QuaternaryPredicate) 
         (valence ?REL 4))


Flags: r1([2nd(holds)])

*/

explaination_line(incode('PrologMOO_valence2', 3, ['REL'=A|B]), via(entails(holds(instance, A, 'QuaternaryPredicate'), holds(valence, A, 4)), ['REL'=A|B])*surf('PrologMOO', 1614)).

/*
deduceValence_util(Context, A, 4, 'BASE ONTOLOGY', D, D*C*incode('PrologMOO_valence2', 3, ['REL'=A|B])) :- 
	forwardchain_instance(true, holds(instance, A, 'QuaternaryPredicate'), 'BASE ONTOLOGY', C)
*/


/* 


Rule # 4 for valence   
  
Forms:
 4. "(=> 
         (instance ?REL TernaryPredicate) 
         (valence ?REL 3))"
4a. (entails 
         (instance ?REL TernaryPredicate) 
         (valence ?REL 3))


Flags: r1([2nd(holds)])

*/

explaination_line(incode('PrologMOO_valence2', 4, ['REL'=A|B]), via(entails(holds(instance, A, 'TernaryPredicate'), holds(valence, A, 3)), ['REL'=A|B])*surf('PrologMOO', 1606)).

/*
deduceValence_util(Context, A, 3, 'BASE ONTOLOGY', D, D*C*incode('PrologMOO_valence2', 4, ['REL'=A|B])) :- 
	forwardchain_instance(true, holds(instance, A, 'TernaryPredicate'), 'BASE ONTOLOGY', C)
*/


/* 


Rule # 5 for valence   
  
Forms:
 5. "(=> 
         (instance ?REL BinaryPredicate) 
         (valence ?REL 2))"
5a. (entails 
         (instance ?REL BinaryPredicate) 
         (valence ?REL 2))


Flags: r1([2nd(holds)])

*/

explaination_line(incode('PrologMOO_valence2', 5, ['REL'=A|B]), via(entails(holds(instance, A, 'BinaryPredicate'), holds(valence, A, 2)), ['REL'=A|B])*surf('PrologMOO', 1598)).

/*
deduceValence_util(Context, A, 2, 'BASE ONTOLOGY', D, D*C*incode('PrologMOO_valence2', 5, ['REL'=A|B])) :- 
	forwardchain_instance(true, holds(instance, A, 'BinaryPredicate'), 'BASE ONTOLOGY', C)
*/


/* 


Rule # 6 for valence   
  
Forms:
 6. "(=> 
         (instance ?FUNCTION TernaryFunction) 
         (valence ?FUNCTION 3))"
6a. (entails 
         (instance ?FUNCTION TernaryFunction) 
         (valence ?FUNCTION 3))


Flags: r1([2nd(holds)])

*/

explaination_line(incode('PrologMOO_valence2', 6, ['FUNCTION'=A|B]), via(entails(holds(instance, A, 'TernaryFunction'), holds(valence, A, 3)), ['FUNCTION'=A|B])*surf('PrologMOO', 1580)).

/*
deduceValence_util(Context, A, 3, 'BASE ONTOLOGY', D, D*C*incode('PrologMOO_valence2', 6, ['FUNCTION'=A|B])) :- 
	forwardchain_instance(true, holds(instance, A, 'TernaryFunction'), 'BASE ONTOLOGY', C)
*/


/* 


Rule # 7 for valence   
  
Forms:
 7. "(=> 
         (instance ?FUNCTION BinaryFunction) 
         (valence ?FUNCTION 2))"
7a. (entails 
         (instance ?FUNCTION BinaryFunction) 
         (valence ?FUNCTION 2))


Flags: r1([2nd(holds)])

*/

explaination_line(incode('PrologMOO_valence2', 7, ['FUNCTION'=A|B]), via(entails(holds(instance, A, 'BinaryFunction'), holds(valence, A, 2)), ['FUNCTION'=A|B])*surf('PrologMOO', 1558)).

/*
deduceValence_util(Context, A, 2, 'BASE ONTOLOGY', D, D*C*incode('PrologMOO_valence2', 7, ['FUNCTION'=A|B])) :- 
	forwardchain_instance(true, holds(instance, A, 'BinaryFunction'), 'BASE ONTOLOGY', C)
*/


/* 


Rule # 8 for valence   
  
Forms:
 8. "(=> 
         (instance ?FUNCTION UnaryFunction) 
         (valence ?FUNCTION 1))"
8a. (entails 
         (instance ?FUNCTION UnaryFunction) 
         (valence ?FUNCTION 1))


Flags: r1([2nd(holds)])

*/

explaination_line(incode('PrologMOO_valence2', 8, ['FUNCTION'=A|B]), via(entails(holds(instance, A, 'UnaryFunction'), holds(valence, A, 1)), ['FUNCTION'=A|B])*surf('PrologMOO', 1536)).

/*
deduceValence_util(Context, A, 1, 'BASE ONTOLOGY', D, D*C*incode('PrologMOO_valence2', 8, ['FUNCTION'=A|B])) :- 
	forwardchain_instance(true, holds(instance, A, 'UnaryFunction'), 'BASE ONTOLOGY', C)
*/


/* 


Rule # 9 for valence   
  
Forms:
 9. "(=> 
         (subrelation ?PRED1 ?PRED2) 
         (exists (?NUMBER) 
            (and 
               (valence ?PRED1 ?NUMBER) 
               (valence ?PRED2 ?NUMBER))))"
9a. (entails 
         (subrelation ?PRED1 ?PRED2) 
         (valence ?PRED2 
            (Valence8SkFn ?PRED2 ?PRED1)))


Flags: r1([zzskFn(Valence8SkFn), 2nd(holds)])

*/

explaination_line(incode('PrologMOO_valence2', 9, ['PRED1'=A, 'PRED2'=B, 'NUMBER'=C|D]), via(entails(holds(subrelation, A, B), holds(valence, B, zzskFn('Valence8SkFn', [B, A]))), ['PRED1'=A, 'PRED2'=B, 'NUMBER'=C|D])*surf('PrologMOO', 183)).

/*
deduceValence_util(Context, B, zzskFn('Valence8SkFn', [B, A]), 'STRUCTURAL ONTOLOGY', E, F*E*incode('PrologMOO_valence2', 9, ['PRED1'=A, 'PRED2'=B, 'NUMBER'=C|D])) :- 
	forwardchain(true, holds(subrelation, A, B), 'STRUCTURAL ONTOLOGY', E)
*/


/* 


Rule # 10 for valence   
  
Forms:
 10. "(=> 
         (subrelation ?PRED1 ?PRED2) 
         (exists (?NUMBER) 
            (and 
               (valence ?PRED1 ?NUMBER) 
               (valence ?PRED2 ?NUMBER))))"
10a. (entails 
         (subrelation ?PRED1 ?PRED2) 
         (valence ?PRED1 
            (Valence8SkFn ?PRED2 ?PRED1)))


Flags: r1([zzskFn(Valence8SkFn), 2nd(holds)])

*/

explaination_line(incode('PrologMOO_valence2', 10, ['PRED1'=A, 'PRED2'=B, 'NUMBER'=C|D]), via(entails(holds(subrelation, A, B), holds(valence, A, zzskFn('Valence8SkFn', [B, A]))), ['PRED1'=A, 'PRED2'=B, 'NUMBER'=C|D])*surf('PrologMOO', 183)).

/*
deduceValence_util(Context, A, zzskFn('Valence8SkFn', [B, A]), 'STRUCTURAL ONTOLOGY', E, F*E*incode('PrologMOO_valence2', 10, ['PRED1'=A, 'PRED2'=B, 'NUMBER'=C|D])) :- 
	forwardchain(true, holds(subrelation, A, B), 'STRUCTURAL ONTOLOGY', E)
*/


