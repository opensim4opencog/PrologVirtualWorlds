:-include('moo_header.pl').

% =====================================================================================================
%% TC Database
% =====================================================================================================


:-op(600,xfx,'myUniv').


myUniv(domainFn(A,B),[domainFn,A,B]):-!.
myUniv(rangeFn(A,B),[rangeFn,A,B]):-!.
%myUniv(holds(P,A,B),[P,A,B]):-!.
myUniv(Term,[P|AB]):-!,Term=..[P|AB].
myUniv(holds(P,AB),[P|AB]):-!.


eraseTransitiveClosureCache:-
	retractall(mooCache(KB,_Ctx,tc(UN,_,_))),!,
	retractall(mooCache(KB,_Ctx,tc(domainFn(_,UN),_),_)),!,
	retractall(mooCache(KB,_Ctx,tc(domainFn(UN,_),_),_)),!,
	retractall(mooCache(KB,_Ctx,tc(rangeFn(_,UN),_),_)),!,
	retractall(mooCache(KB,_Ctx,tc(rangeFn(UN,_),_),_)),!,
	writeDebug(eraseTransitiveClosureCache).


%eraseTransitiveClosureCache(KB,Ctx,Var):-expireOptimizationsInKB(KB,Ctx,Assertion),fail.

eraseTransitiveClosureCache(KB,Ctx,Var):-var(Var),!.
eraseTransitiveClosureCache(KB,Ctx,UN):-number(UN),!.
eraseTransitiveClosureCache(KB,Ctx,[]):-!.
eraseTransitiveClosureCache(KB,Ctx,[H|L]):-!,
	eraseTransitiveClosureCache(KB,Ctx,H),
	eraseTransitiveClosureCache(KB,Ctx,L),!.

eraseTransitiveClosureCache(KB,Ctx,UN):-atom(UN),
	%writeDebug(eraseTransitiveClosureCache(KB,Ctx,UN)),
	retractall(mooCache(KB,_Ctx,tc(UN,_,_))),!,
	retractall(mooCache(KB,_Ctx,tc(domainFn(_,UN),_),_)),!,
	retractall(mooCache(KB,_Ctx,tc(domainFn(UN,_),_),_)),!,
	retractall(mooCache(KB,_Ctx,tc(rangeFn(_,UN),_),_)),!,
	retractall(mooCache(KB,_Ctx,tc(rangeFn(UN,_),_),_)),!.
	

eraseTransitiveClosureCache(KB,Ctx,UN):-
	getConstants(atomic,UN,Consts,_,_),
	logOnFailure(eraseTransitiveClosureCache(KB,Ctx,Consts)).

writeDebug_tc(_).

storeInCacheIffNew(KB,Ctx,S,C,P):-(mooCache(KB,Ctx,tc(S,C),_)),!.
storeInCacheIffNew(KB,Ctx,S,C,P):-asserta(mooCache(KB,Ctx,tc(S,C),P)),!.

storeInCacheIffNew(KB,Ctx,S,C,P):-!.

% =====================================================================================================
%% TC for PartialOrderingRelation
% =====================================================================================================
deduceTransitiveClosure_PartialOrderingRelation(KB,Ctx,Predicate,S,C,
%        sfind(instance(Predicate,'PartialOrderingRelation')) *
     	sfind(subclass('PartialOrderingRelation','TransitiveRelation'))  *
	Explaination  
	):- deduceTransitiveClosure_Redir(KB,Ctx,'PartialOrderingRelation',Predicate,S,C,Explaination). 
	
deduceTransitiveClosure_PartialOrderingRelation(KB,Ctx,Predicate,S,S,
%	sfind(instance(Predicate,'PartialOrderingRelation')) * 
	sfind(subclass('PartialOrderingRelation','ReflexiveRelation')) * 
	sfindi('=>'(instance(Predicate,'ReflexiveRelation'),
	forall(S,holds(Predicate,S,S))))):- isAxiomInKB(
	sfind(instance(Predicate,'PartialOrderingRelation')) * 
	sfind(subclass('PartialOrderingRelation','ReflexiveRelation')) * 
	sfind('=>'(instance(Predicate,'ReflexiveRelation'),
	forall(S,holds(Predicate,S,S))))).

% =====================================================================================================
%% TC for TotalOrderingRelation
% =====================================================================================================
deduceTransitiveClosure_TotalOrderingRelation(KB,Ctx,Predicate,S,C,
      %  sfind(instance(Predicate,'TotalOrderingRelation')) *
 	sfind(subclass('TotalOrderingRelation','PartialOrderingRelation')) 
	):- deduceTransitiveClosure_PartialOrderingRelation(KB,Ctx,Predicate,S,C,Explaination). 

% =====================================================================================================
%% TC for TransitiveRelation
% =====================================================================================================
deduceTransitiveClosure_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination):- 
	deduceTransitiveClosure_Redir(KB,Ctx,'TransitiveRelation',Predicate,S,C,Explaination). 


% =====================================================================================================
%% Generic Transitive Relation
% =====================================================================================================
deduceTransitiveClosure_Redir(KB,Ctx,RType,Predicate,S,C, Explaination  *
	       sfindi( 
	   =>(instance(Predicate,'TransitiveRelation'),
	       forall(C,forall(D,forall(S,
	=>(and(holds(Predicate,S,D),holds(Predicate,D,C)),holds(Predicate,S,C)))))))
		):-
	    /*   isAxiomInKB( 
	   =>(instance(Predicate,'TransitiveRelation'),
	       forall(C,forall(D,forall(S,
	=>(and(holds(Predicate,S,D),holds(Predicate,D,C)),holds(Predicate,S,C))))))),!,	*/
		
		(deduceTransitiveClosure_O_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination)).

deduceTransitiveClosure_O_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination):-nonvar(C),!,
	deduceTransitiveClosure_rl_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination).
deduceTransitiveClosure_O_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination):-nonvar(S),!,
	deduceTransitiveClosure_lr_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination).
deduceTransitiveClosure_O_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination):-!,
	deduceTransitiveClosure_open_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination).

% =====================================================================================================
%% Generic Transitive Open 
% =====================================================================================================

deduceTransitiveClosure_open_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination):-
	%KeyTerm 'myUniv'  [holdsFn,Predicate],!,
	deduceTransitiveClosure_open_t_TransitiveRelation(KB,Ctx,Predicate,Predicate,S,C,Explaination).

deduceTransitiveClosure_open_t_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,_):-
	once(table_make_deduceTransitiveClosure_open_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate)),fail.
deduceTransitiveClosure_open_t_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,P):-
	DataTerm 'myUniv'  [Predicate,S,C],
	mooCache(KB,_Ctx,tc(Predicate,DataTerm),P).

table_make_deduceTransitiveClosure_open_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate):-
		mooCache(KB,_Ctx,tc(KeyTerm,complete,table)),!.
table_make_deduceTransitiveClosure_open_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate):-
		once((
		mooCache(KB,_Ctx,tc(KeyTerm,incomplete,table));assert(mooCache(KB,_Ctx,tc(KeyTerm,incomplete,table))))),fail.
table_make_deduceTransitiveClosure_open_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate):-
		(DataTerm 'myUniv'  [Predicate,S,C]),
		make_deduceTransitiveClosure_open_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,P),
		once(storeInCacheIffNew(KB,Ctx,KeyTerm,DataTerm,P)),fail.

table_make_deduceTransitiveClosure_open_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate):-
		retractall(mooCache(KB,_Ctx,tc(KeyTerm,incomplete,table))),
		assert(mooCache(KB,_Ctx,tc(KeyTerm,complete,table))).
		


make_deduceTransitiveClosure_open_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,P):-
	(Call  'myUniv'   [Predicate,S,C]),
	getFactForTransitiveClosure(KB,Ctx,Call,P).

make_deduceTransitiveClosure_open_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,(P *Explaination )):-
	(Call  'myUniv'   [Predicate,S,M]),
	getFactForTransitiveClosure(KB,Ctx,Call,P),
	deduceTransitiveClosure_O_TransitiveRelation(KB,Ctx,Predicate,M,C,Explaination).

% =====================================================================================================
%% Generic Transitive Left to Right 
% =====================================================================================================

deduceTransitiveClosure_lr_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination):-
	KeyTerm 'myUniv'  [rangeFn,Predicate,S],!,
	deduceTransitiveClosure_l_r_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,Explaination).


deduceTransitiveClosure_l_r_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,_):-
	once(table_deduceTransitiveClosure_l_r_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S)),fail.
	
deduceTransitiveClosure_l_r_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,P):-
	%writeDebug(silver,have_tabel(KeyTerm)),
	DataTerm 'myUniv'  [Predicate,S,C],!,
	mooCache(KB,_Ctx,tc(KeyTerm,DataTerm),P).
	

table_deduceTransitiveClosure_l_r_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S):-
		mooCache(KB,_Ctx,tc(KeyTerm,complete,table)),!.

table_deduceTransitiveClosure_l_r_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S):-
		once((
		mooCache(KB,_Ctx,tc(KeyTerm,incomplete,table));assert(mooCache(KB,_Ctx,tc(KeyTerm,incomplete,table))))),fail.

table_deduceTransitiveClosure_l_r_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S):-
		(DataTerm 'myUniv'  [Predicate,S,C]),
		deduceTransitiveClosure_l_r_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,P),
		once(storeInCacheIffNew(KB,Ctx,KeyTerm,DataTerm,P)),fail.
		
table_deduceTransitiveClosure_l_r_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S):-
		retractall(mooCache(KB,_Ctx,tc(KeyTerm,incomplete,table))),
		assert(mooCache(KB,_Ctx,tc(KeyTerm,complete,table))),!.


deduceTransitiveClosure_l_r_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,P):-
	(Call  'myUniv'   [Predicate,S,C]),
	getFactForTransitiveClosure(KB,Ctx,Call,P).

deduceTransitiveClosure_l_r_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,(P *Explaination )):-
	(Call  'myUniv'   [Predicate,S,M]),
	getFactForTransitiveClosure(KB,Ctx,Call,P),
	deduceTransitiveClosure_O_TransitiveRelation(KB,Ctx,Predicate,M,C,Explaination).

% =====================================================================================================
%% Generic Transitive  Right  to Left
% =====================================================================================================

deduceTransitiveClosure_rl_TransitiveRelation(KB,Ctx,Predicate,S,C,Explaination):-
	KeyTerm 'myUniv'  [domainFn,Predicate,C],!,
	deduceTransitiveClosure_r_l_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,Explaination).

deduceTransitiveClosure_r_l_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,_):-
		table_deduceTransitiveClosure_r_l_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,C),fail.
deduceTransitiveClosure_r_l_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,P):-
		%writeDebug(have_tabel(KeyTerm)),
		DataTerm 'myUniv'  [Predicate,S,C],!,mooCache(KB,_Ctx,tc(KeyTerm,DataTerm),P).

table_deduceTransitiveClosure_r_l_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,C):-
		mooCache(KB,_Ctx,tc(KeyTerm,complete,table)),!.

table_deduceTransitiveClosure_r_l_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,C):-
		once((
		mooCache(KB,_Ctx,tc(KeyTerm,incomplete,table));assert(mooCache(KB,_Ctx,tc(KeyTerm,incomplete,table))))),fail.

table_deduceTransitiveClosure_r_l_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,C):-
		(DataTerm 'myUniv'  [Predicate,S,C]),
		deduceTransitiveClosure_r_l_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,P),
		once(storeInCacheIffNew(KB,Ctx,KeyTerm,DataTerm,P)),fail.

table_deduceTransitiveClosure_r_l_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,C):-
		retractall(mooCache(KB,_Ctx,tc(KeyTerm,incomplete,table))),assert(mooCache(KB,_Ctx,tc(KeyTerm,complete,table))),!.


deduceTransitiveClosure_r_l_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,P):-
	(Call  'myUniv'   [Predicate,S,C]),
	getFactForTransitiveClosure(KB,Ctx,Call,P).

deduceTransitiveClosure_r_l_0_TransitiveRelation(KB,Ctx,KeyTerm,Predicate,S,C,(P2 * P)):-
	(Call  'myUniv'   [Predicate,M,C]),
	getFactForTransitiveClosure(KB,Ctx,Call,P),
	deduceTransitiveClosure_O_TransitiveRelation(KB,Ctx,Predicate,S,M,P2).

isAxiomInKB(_):-!. %TODO



:-eraseTransitiveClosureCache.
