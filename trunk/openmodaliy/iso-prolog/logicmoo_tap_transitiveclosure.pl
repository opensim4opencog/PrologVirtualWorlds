
/*
:-module(moo_transitiveclosure,
   [eraseTransitiveClosureCache/0,
   deduceTransitiveClosure_PartialOrderingRelation/5]).

:-use_module(moo_generation).
:-use_module(moo_globalisms).
*/

:-include('logicmoo_utils_header.pl').

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
	retractall(mooCache(Context,_Ctx,tc(UN,_,_))),!,
	retractall(mooCache(Context,_Ctx,tc(domainFn(_,UN),_),_)),!,
	retractall(mooCache(Context,_Ctx,tc(domainFn(UN,_),_),_)),!,
	retractall(mooCache(Context,_Ctx,tc(rangeFn(_,UN),_),_)),!,
	retractall(mooCache(Context,_Ctx,tc(rangeFn(UN,_),_),_)),!,
	writeDebug(eraseTransitiveClosureCache).


%eraseTransitiveClosureCache(Context,Var):-expireOptimizationsInContext(Context,Assertion),fail.

eraseTransitiveClosureCache(Context,Var):-var(Var),!.
eraseTransitiveClosureCache(Context,UN):-number(UN),!.
eraseTransitiveClosureCache(Context,[]):-!.
eraseTransitiveClosureCache(Context,[H|L]):-!,
	eraseTransitiveClosureCache(Context,H),
	eraseTransitiveClosureCache(Context,L),!.

eraseTransitiveClosureCache(Context,UN):-atom(UN),
	%writeDebug(eraseTransitiveClosureCache(Context,UN)),
	retractall(mooCache(Context,_Ctx,tc(UN,_,_))),!,
	retractall(mooCache(Context,_Ctx,tc(domainFn(_,UN),_),_)),!,
	retractall(mooCache(Context,_Ctx,tc(domainFn(UN,_),_),_)),!,
	retractall(mooCache(Context,_Ctx,tc(rangeFn(_,UN),_),_)),!,
	retractall(mooCache(Context,_Ctx,tc(rangeFn(UN,_),_),_)),!.
	

eraseTransitiveClosureCache(Context,UN):-
	getConstants(atomic,UN,Consts,_,_),
	logOnFailure(eraseTransitiveClosureCache(Context,Consts)).

writeDebug_tc(_).

storeInCacheIffNew(Context,S,C,P):-(mooCache(Context,tc(S,C),_)),!.
storeInCacheIffNew(Context,S,C,P):-asserta(mooCache(Context,tc(S,C),P)),!.

storeInCacheIffNew(Context,S,C,P):-!.

% =====================================================================================================
%% TC for PartialOrderingRelation
% =====================================================================================================
deduceTransitiveClosure_PartialOrderingRelation(Context,Predicate,S,C,
%        sfind(instance(Predicate,'PartialOrderingRelation')) *
     	sfind(subclass('PartialOrderingRelation','TransitiveRelation'))  *
	Explaination  
	):- deduceTransitiveClosure_Redir(Context,'PartialOrderingRelation',Predicate,S,C,Explaination). 
	
deduceTransitiveClosure_PartialOrderingRelation(Context,Predicate,S,S,
%	sfind(instance(Predicate,'PartialOrderingRelation')) * 
	sfind(subclass('PartialOrderingRelation','ReflexiveRelation')) * 
	sfindi('=>'(instance(Predicate,'ReflexiveRelation'),
	forall(S,holds(Predicate,S,S))))):- isAxiomInContext(
	sfind(instance(Predicate,'PartialOrderingRelation')) * 
	sfind(subclass('PartialOrderingRelation','ReflexiveRelation')) * 
	sfind('=>'(instance(Predicate,'ReflexiveRelation'),
	forall(S,holds(Predicate,S,S))))).

% =====================================================================================================
%% TC for TotalOrderingRelation
% =====================================================================================================
deduceTransitiveClosure_TotalOrderingRelation(Context,Predicate,S,C,
      %  sfind(instance(Predicate,'TotalOrderingRelation')) *
 	sfind(subclass('TotalOrderingRelation','PartialOrderingRelation')) 
	):- deduceTransitiveClosure_PartialOrderingRelation(Context,Predicate,S,C,Explaination). 

% =====================================================================================================
%% TC for TransitiveRelation
% =====================================================================================================
deduceTransitiveClosure_TransitiveRelation(Context,Predicate,S,C,Explaination):- 
	deduceTransitiveClosure_Redir(Context,'TransitiveRelation',Predicate,S,C,Explaination). 


% =====================================================================================================
%% Generic Transitive Relation
% =====================================================================================================
deduceTransitiveClosure_Redir(Context,RType,Predicate,S,C, Explaination  *
	       sfindi( 
	   =>(instance(Predicate,'TransitiveRelation'),
	       forall(C,forall(D,forall(S,
	=>(and(holds(Predicate,S,D),holds(Predicate,D,C)),holds(Predicate,S,C)))))))
		):-
	    /*   isAxiomInContext( 
	   =>(instance(Predicate,'TransitiveRelation'),
	       forall(C,forall(D,forall(S,
	=>(and(holds(Predicate,S,D),holds(Predicate,D,C)),holds(Predicate,S,C))))))),!,	*/
		
		(deduceTransitiveClosure_O_TransitiveRelation(Context,Predicate,S,C,Explaination)).

deduceTransitiveClosure_O_TransitiveRelation(Context,Predicate,S,C,Explaination):-nonvar(C),!,
	deduceTransitiveClosure_rl_TransitiveRelation(Context,Predicate,S,C,Explaination).
deduceTransitiveClosure_O_TransitiveRelation(Context,Predicate,S,C,Explaination):-nonvar(S),!,
	deduceTransitiveClosure_lr_TransitiveRelation(Context,Predicate,S,C,Explaination).
deduceTransitiveClosure_O_TransitiveRelation(Context,Predicate,S,C,Explaination):-!,
	deduceTransitiveClosure_open_TransitiveRelation(Context,Predicate,S,C,Explaination).

% =====================================================================================================
%% Generic Transitive Open 
% =====================================================================================================

deduceTransitiveClosure_open_TransitiveRelation(Context,Predicate,S,C,Explaination):-
	%KeyTerm 'myUniv'  [holdsFn,Predicate],!,
	deduceTransitiveClosure_open_t_TransitiveRelation(Context,Predicate,Predicate,S,C,Explaination).

deduceTransitiveClosure_open_t_TransitiveRelation(Context,KeyTerm,Predicate,S,C,_):-
	once(table_make_deduceTransitiveClosure_open_0_TransitiveRelation(Context,KeyTerm,Predicate)),fail.
deduceTransitiveClosure_open_t_TransitiveRelation(Context,KeyTerm,Predicate,S,C,P):-
	DataTerm 'myUniv'  [Predicate,S,C],
	mooCache(Context,_Ctx,tc(Predicate,DataTerm),P).

table_make_deduceTransitiveClosure_open_0_TransitiveRelation(Context,KeyTerm,Predicate):-
		mooCache(Context,_Ctx,tc(KeyTerm,complete,table)),!.
table_make_deduceTransitiveClosure_open_0_TransitiveRelation(Context,KeyTerm,Predicate):-
		once((
		mooCache(Context,_Ctx,tc(KeyTerm,incomplete,table));assert(mooCache(Context,_Ctx,tc(KeyTerm,incomplete,table))))),fail.
table_make_deduceTransitiveClosure_open_0_TransitiveRelation(Context,KeyTerm,Predicate):-
		(DataTerm 'myUniv'  [Predicate,S,C]),
		make_deduceTransitiveClosure_open_0_TransitiveRelation(Context,KeyTerm,Predicate,S,C,P),
		once(storeInCacheIffNew(Context,KeyTerm,DataTerm,P)),fail.

table_make_deduceTransitiveClosure_open_0_TransitiveRelation(Context,KeyTerm,Predicate):-
		retractall(mooCache(Context,_Ctx,tc(KeyTerm,incomplete,table))),
		assert(mooCache(Context,_Ctx,tc(KeyTerm,complete,table))).
		


make_deduceTransitiveClosure_open_0_TransitiveRelation(Context,KeyTerm,Predicate,S,C,P):-
	(Call  'myUniv'   [Predicate,S,C]),
	getFactForTransitiveClosure(Context,Call,P).

make_deduceTransitiveClosure_open_0_TransitiveRelation(Context,KeyTerm,Predicate,S,C,(P *Explaination )):-
	(Call  'myUniv'   [Predicate,S,M]),
	getFactForTransitiveClosure(Context,Call,P),
	deduceTransitiveClosure_O_TransitiveRelation(Context,Predicate,M,C,Explaination).

% =====================================================================================================
%% Generic Transitive Left to Right 
% =====================================================================================================

deduceTransitiveClosure_lr_TransitiveRelation(Context,Predicate,S,C,Explaination):-
	KeyTerm 'myUniv'  [rangeFn,Predicate,S],!,
	deduceTransitiveClosure_l_r_TransitiveRelation(Context,KeyTerm,Predicate,S,C,Explaination).


deduceTransitiveClosure_l_r_TransitiveRelation(Context,KeyTerm,Predicate,S,C,_):-
	once(table_deduceTransitiveClosure_l_r_0_TransitiveRelation(Context,KeyTerm,Predicate,S)),fail.
	
deduceTransitiveClosure_l_r_TransitiveRelation(Context,KeyTerm,Predicate,S,C,P):-
	%writeDebug(silver,have_tabel(KeyTerm)),
	DataTerm 'myUniv'  [Predicate,S,C],!,
	mooCache(Context,_Ctx,tc(KeyTerm,DataTerm),P).
	

table_deduceTransitiveClosure_l_r_0_TransitiveRelation(Context,KeyTerm,Predicate,S):-
		mooCache(Context,_Ctx,tc(KeyTerm,complete,table)),!.

table_deduceTransitiveClosure_l_r_0_TransitiveRelation(Context,KeyTerm,Predicate,S):-
		once((
		mooCache(Context,_Ctx,tc(KeyTerm,incomplete,table));assert(mooCache(Context,_Ctx,tc(KeyTerm,incomplete,table))))),fail.

table_deduceTransitiveClosure_l_r_0_TransitiveRelation(Context,KeyTerm,Predicate,S):-
		(DataTerm 'myUniv'  [Predicate,S,C]),
		deduceTransitiveClosure_l_r_0_TransitiveRelation(Context,KeyTerm,Predicate,S,C,P),
		once(storeInCacheIffNew(Context,KeyTerm,DataTerm,P)),fail.
		
table_deduceTransitiveClosure_l_r_0_TransitiveRelation(Context,KeyTerm,Predicate,S):-
		retractall(mooCache(Context,_Ctx,tc(KeyTerm,incomplete,table))),
		assert(mooCache(Context,_Ctx,tc(KeyTerm,complete,table))),!.


deduceTransitiveClosure_l_r_0_TransitiveRelation(Context,KeyTerm,Predicate,S,C,P):-
	(Call  'myUniv'   [Predicate,S,C]),
	getFactForTransitiveClosure(Context,Call,P).

deduceTransitiveClosure_l_r_0_TransitiveRelation(Context,KeyTerm,Predicate,S,C,(P *Explaination )):-
	(Call  'myUniv'   [Predicate,S,M]),
	getFactForTransitiveClosure(Context,Call,P),
	deduceTransitiveClosure_O_TransitiveRelation(Context,Predicate,M,C,Explaination).

% =====================================================================================================
%% Generic Transitive  Right  to Left
% =====================================================================================================

deduceTransitiveClosure_rl_TransitiveRelation(Context,Predicate,S,C,Explaination):-
	KeyTerm 'myUniv'  [domainFn,Predicate,C],!,
	deduceTransitiveClosure_r_l_TransitiveRelation(Context,KeyTerm,Predicate,S,C,Explaination).

deduceTransitiveClosure_r_l_TransitiveRelation(Context,KeyTerm,Predicate,S,C,_):-
		table_deduceTransitiveClosure_r_l_0_TransitiveRelation(Context,KeyTerm,Predicate,C),fail.
deduceTransitiveClosure_r_l_TransitiveRelation(Context,KeyTerm,Predicate,S,C,P):-
		%writeDebug(have_tabel(KeyTerm)),
		DataTerm 'myUniv'  [Predicate,S,C],!,mooCache(Context,_Ctx,tc(KeyTerm,DataTerm),P).

table_deduceTransitiveClosure_r_l_0_TransitiveRelation(Context,KeyTerm,Predicate,C):-
		mooCache(Context,_Ctx,tc(KeyTerm,complete,table)),!.

table_deduceTransitiveClosure_r_l_0_TransitiveRelation(Context,KeyTerm,Predicate,C):-
		once((
		mooCache(Context,_Ctx,tc(KeyTerm,incomplete,table));assert(mooCache(Context,_Ctx,tc(KeyTerm,incomplete,table))))),fail.

table_deduceTransitiveClosure_r_l_0_TransitiveRelation(Context,KeyTerm,Predicate,C):-
		(DataTerm 'myUniv'  [Predicate,S,C]),
		deduceTransitiveClosure_r_l_0_TransitiveRelation(Context,KeyTerm,Predicate,S,C,P),
		once(storeInCacheIffNew(Context,KeyTerm,DataTerm,P)),fail.

table_deduceTransitiveClosure_r_l_0_TransitiveRelation(Context,KeyTerm,Predicate,C):-
		retractall(mooCache(Context,_Ctx,tc(KeyTerm,incomplete,table))),assert(mooCache(Context,_Ctx,tc(KeyTerm,complete,table))),!.


deduceTransitiveClosure_r_l_0_TransitiveRelation(Context,KeyTerm,Predicate,S,C,P):-
	(Call  'myUniv'   [Predicate,S,C]),
	getFactForTransitiveClosure(Context,Call,P).

deduceTransitiveClosure_r_l_0_TransitiveRelation(Context,KeyTerm,Predicate,S,C,(P2 * P)):-
	(Call  'myUniv'   [Predicate,M,C]),
	getFactForTransitiveClosure(Context,Call,P),
	deduceTransitiveClosure_O_TransitiveRelation(Context,Predicate,S,M,P2).

isAxiomInContext(_):-!. %TODO



:-eraseTransitiveClosureCache.
