%%apldriver.prolog


/*

%// Proper APL Syntax will involve this structure

APL Structure

		ReCognition
			
			How to Observe Event  
			
			Conditions that must be false for proper recognition

			Conditions that must be true for proper recognition
			
			Conditions that may be learned false when one observes it

			Conditions that may be learned true when one observes it

		ReCreation

			How to Recreate as Initiated by Self

			Conditions about the arguents that must be true to execute it

			Conditions about the arguents that must be false to execute it


		Goals

			Conditions that may become true when executed

			Conditions that may become false when executed

Intersions

		Conditions that are proprietary to other conditions for ReCognition and ReCreation that depend on 
			the OODB (VWorlds)


APL Truths

	Trueness

	Falseness
	
	Ambiguity
	
	Falacy


 APL has no "Weights" store in truth per say

	Instead it Uses Time or "Closes" to decide what is truer or lesser true
	
	Each data fact is inserted in the order received
	
	If the fact is already in the database its structure may be copied 
		(this may change since APL works from front to back of Que)

The Grammer of APL can be ported to G-Trans, Prolog, Lisp, AST, RST, Poplog, DCG are used in C++


modes (instanciated,coll,no_coll) , (known_fact,possible_fact)


alias => predicate = friendly name (modes)
-------------------------------------------------
i => instance_known = instance (instanciated,known_fact)
t => instance_possible = testable (instanciated,possible_fact)
k => collect_known = known (coll,known_fact)
c => collect_possible = coll all (coll,possible_fact)
f => test_known = factual (no_coll,known_fact)
q => test_possible = query (no_coll,possible_fact)
a => assert_known = assert fact (assert,known_fact)
m => assert_possible = memorable (assert,possible_fact)
p => safe_phrase = phrasing (all)

	

*/

%//------------------------------------------------------------------------------

:-dynamic(f/1).
:-multifile(f/1).

%//------------------------------------------------------------------------------

:-dynamic(fact/1).
:-multifile(fact/1).

%//------------------------------------------------------------------------------

:-dynamic(c/1).
:-multifile(c/1).

%//------------------------------------------------------------------------------

:-dynamic(p/2).
:-multifile(p/2).

%//------------------------------------------------------------------------------

:-dynamic(t/1).
:-multifile(t/1).

%//------------------------------------------------------------------------------

:-dynamic(fact/2).
:-multifile(fact/2).

%//------------------------------------------------------------------------------

:-dynamic(pp/2).
:-multifile(pp/2).

%//------------------------------------------------------------------------------

:-op(10,fx,~).
:-multifile('~'/1).

%//------------------------------------------------------------------------------

:- op(800,xfy,&).
:-multifile('&'/1).

%//------------------------------------------------------------------------------

:- op(900,yfx,:).
:-multifile(':'/1).

%//------------------------------------------------------------------------------



i(Datum):-instance_known(Datum).
t(Datum):-instance_possible(Datum).
k(Datum):-collect_known(Datum).
c(Datum):-if(Datum).
f(Datum):-test_known(Datum).
q(Datum):-test_possible(Datum).
a(Datum):-assert_known(Datum).
m(Datum):-assert_possible(Datum).


%//------------------------------------------------------------------------------

assert_new(X):-assert_new(fact(X)).


%//------------------------------------------------------------------------------

instance_known(Datum):-ground(Datum),test_known(Datum).

instance_possible(Datum):-ground(Datum),test_possible(Datum).

%//------------------------------------------------------------------------------

assert_known(end_of_file):-!.
assert_known(Datum):-clause(fact(Datum),true),!,writeDebug(assert_known,old(Datum)).
assert_known((Head:-Body)):-fact(Head,Body),!,writeDebug(assert_known,old(instance_possible(Head,Body))).
assert_known((Head:-Body)):-assert(fact(Head,Body)),!,writeDebug(assert_known,new(Head,while,Body)).
assert_known(Datum):-assert(fact(Datum)),writeDebug(assert_known,new(Datum)).

%//------------------------------------------------------------------------------

assert_possible(Datum):-assert_known(possible(Datum)).
assert_possible(pp(Implicit,Explicit)):-assert(pp(Implicit,Explicit)),!.

%//------------------------------------------------------------------------------

test_known(Datum):-catch(fact(Datum)),writeDebug(test_known,fact(Datum)).
test_known(Goal):-fact(Goal,Body),test_known(Body),ground(Goal).
test_known(Datum):-var(Datum),!,fail.
test_known(Datum):-writeDebug(test_known,nonfact(Datum)),fail.

%//------------------------------------------------------------------------------

test_possible(Datum):-test_known(Datum).

%//------------------------------------------------------------------------------

if(Datum):-fact(Datum).
if(Conj):-nonvar(Conj),Conj=(Datum,Datum2),(if(Datum),if(Datum2)).
if(Disj):-nonvar(Disj),Disj=(Datum;Datum2),(if(Datum);if(Datum2)).
if(Datum):-fact(Datum,Requirements),if(Requirements).
if(p(Datum,Datum2)):-p(Datum,Datum2).

if(if(Datum)):-nonvar(Datum),fact(Datum).
if(if(Datum)):-nonvar(Datum),fact(Datum,Requirements),if(Requirements).

%//------------------------------------------------------------------------------

collect_known(Goal):-fact(Goal),ground(Goal).
collect_known(Goal):-fact(Goal,true),ground(Goal).
collect_known(Goal):-fact(Goal,Body),catch(collect_known(Body),_,fail),ground(Goal),writeDebug(t,proven(Goal)).
collect_known(Datum):-!,writeDebug(t,failed(Datum)),fail.


%collect_known(p(Datum,Datum2)):-catch(p(Datum,Datum2),_,fail).
%collect_known((Datum,Datum2)):-(collect_known(Datum),ground(Datum)),(collect_known(Datum2),ground(Datum2)).
%collect_known((Datum;Datum2)):-(collect_known(Datum),ground(Datum));(collect_known(Datum2),ground(Datum2)).

%//------------------------------------------------------------------------------

collect_possible(Datum):-collect_known(Datum).
collect_possible(Datum):-nonvar(Datum),catch(Datum,_,fail),writeDebug(c,proved(Datum)).
collect_possible(Datum):-nonvar(Datum),!,writeDebug(c,failed(Datum)),fail.
collect_possible(_):-writeDebug(c,unbound),!,fail.

%//------------------------------------------------------------------------------

:- dynamic(de_brack/0).

all_instance :- retractall( de_brack ),assert( de_brack ).

non_instance :- retractall( de_brack ).

:- all_instance.

%//------------------------------------------------------------------------------

:-dynamic(in_p/0),dynamic(pp/2).

p(Datum,Datum2):-var(Datum),var(Datum2),!,fail.

p(Datum,Datum2):-dcg_macro(Datum),catch(phrase(Datum,[Datum2])).

p(Datum,Pred):-atom(Datum),catch(Pred =..[Datum,Arg]),if(Pred),ground(Arg).

p(Datum,Pred):-dcg_macro(Datum),nonvar(Pred),functor(Pred,_,1),catch(Pred =..[_,Arg],Test=..[Datum,Arg]),if(Test).

p(Implicit,Explicit):-pp(Implicit,Explicit).

%p(Datum,Pred):-ground((Datum,Pred)),Pred =..[Datum,_].

%//------------------------------------------------------------------------------

sphrase(Datum,Datum2):-
	catch(
		(dcg_macro(Datum),catch(phrase(Datum,Datum2)),!),
		error(existence_error(procedure, _/_), _),
		(ground(Datum),[Datum]=Datum2,writeDebug(c,faking(Datum)))
	).

~(neq(Datum,Datum)).

~(Datum):-catch(not(Datum),_,fail).

%//------------------------------------------------------------------------------

neq(Datum,Datum2):-Datum\=@=Datum2.
neq(List):-sort(List,Set),length(List,N),length(Set,N),fail.
neq(_).

%//------------------------------------------------------------------------------
%% 'C:/msr/output/data.prolog'

apl_load_datafile(DataFile):-see(DataFile),repeat,read(Datum),assert_known(Datum),Datum=end_of_file,seen.

%//------------------------------------------------------------------------------
%% 'C:/msr/output/espg.prolog'

apl_load_macrofile(MacroFile):-retractall(dcg_macro(_)),see(MacroFile),repeat,read(Datum),assert_apl_rule(Datum),Datum=end_of_file,seen.

%//------------------------------------------------------------------------------

assert_apl_rule(end_of_file):-!.
assert_apl_rule((Macro-->CLAUSE_WHEN)):-!,
		retractall(dcg_macro(Macro)),
		assert(dcg_macro(Macro)),
		expand_term((Macro-->CLAUSE_WHEN),Prolog),
		retractall(Prolog),
		assert(Prolog),
		assert_dcg_macro_tree((Macro-->CLAUSE_WHEN)).

assert_apl_rule(Prolog):-!,assert(Prolog).

%//------------------------------------------------------------------------------

assert_dcg_macro_tree((Macro-->PROGRAM)):-!,
	retractall(dcg_tree(Macro,PROGRAM)),
	assert(dcg_tree(Macro,PROGRAM)).

%//------------------------------------------------------------------------------

