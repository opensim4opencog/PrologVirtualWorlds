% =================================================================================
% Utilities Common to OpenCyc/LogicMoo Projects
% =================================================================================

% ===================================================================
% File 'lm_utils.pl' 
% Maintainer: Douglas Miles 
% Contact: dmiles@users.sourceforge.net; 
% Version: 'lm_utils.pl' 1.0.0 
% ===================================================================


:-multifile(expireOptimizationsInContext/3).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).
            
% ===================================================================
% This File is the bootstrap for the Moo Infence engine one first calls "[belief_module]"
% So first is loads the proper files and then starts up the system
% There are no predicates defined in this file (it just uses other files' predicates)
% ===================================================================
% ===================================================================
% EXPORTS
% ===================================================================
isNonVar(Denotation):-not(isSlot(Denotation)).

% ===============================================================================================
% ===============================================================================================

isSlot(Denotation):-((isVarProlog(Denotation);isVarObject(Denotation))),!.

isSlot(Denotation,Denotation):- isVarProlog(Denotation),!.
isSlot(Denotation,PrologVar):- isVarObject(Denotation,PrologVar),!.

% ===============================================================================================
% ===============================================================================================

isHiddenSlot(Term):-fail.

% ===============================================================================================
% ===============================================================================================

isVarProlog(A):-((var(A);A='$VAR'(_))).

% ===============================================================================================
% ===============================================================================================

isVarObject(Denotation):-((
		  isObject(Denotation,BaseType),
		  arg(1,Denotation,Value),!,isSlot(Value))).

isVarObject(Denotation,Value):-((
		  isObject(Denotation,BaseType),
		  arg(1,Denotation,Value),!,isSlot(Value))).

% ===============================================================================================
% ===============================================================================================
	
isObject(Denotation,BaseType):-
	(((atom(BaseType) ->
		  (atom_concat('$',BaseType,F),functor(Denotation,F,2));
		  (functor(Denotation,F,2),atom_concat('$',BaseType,F))
		 ),!)).

% ===============================================================================================
% ===============================================================================================

isQualifiableAsClass(Atom):-atom(Atom),!.
isQualifiableAsClass('$Class'(Atom,_)):-atom(Atom),!.

isQualifiableAs(Denotation,BaseType,Value):-
		  isObject(Denotation,BaseType),
		  arg(1,Denotation,Value).

% ===============================================================================================
% ===============================================================================================

isQualifiedAs(Denotation,_,_):-not(compound(Denotation)),!,fail.
isQualifiedAs(Denotation,BaseType,Value):-
		  isQualifiedAs(Denotation,BaseType,Value,SubType).
isQualifiedAs(Denotation,BaseType,Value,SubType):-
		  isObject(Denotation,BaseType),
		  arg(1,Denotation,Value),
		  arg(2,Denotation,List),
		  lastImproperMember(BaseType,SubType,List).

% ===============================================================================================
% ===============================================================================================

lastImproperMember(Default,Default,List):-isVarProlog(List),!.
lastImproperMember(Default,Default,[]):-!.
lastImproperMember(Default,SubType,List):-proper_list(List),last(SubType,List).
lastImproperMember(Default,SubType,[SubType|End]):-isVarProlog(End),!.
lastImproperMember(Default,SubType,[_|Rest]):-
	lastImproperMember(Default,SubType,Rest),!.
	
% ===============================================================================================
% ===============================================================================================

isQualifiedAndKnownAs(Denotation,BaseType,Value):-
		  isQualifiedAs(Denotation,BaseType,Value),!,
		  not(isVarProlog(Value)).

% ===============================================================================================
% ===============================================================================================

isQualifiedAndVarAs(Denotation,BaseType,Value):-
		  isQualifiedAs(Denotation,BaseType,Value),!,
		  isVarProlog(Value).

% ===============================================================================================
% ===============================================================================================

isQualifiedAndVarAndUnifiable(Denotation,BaseType,NValue):-
		  isQualifiedAs(Denotation,BaseType,Value),!,
		  (isVarProlog(Value);
		  (\+ \+ NValue=Value)),!.

% ===============================================================================================
% ===============================================================================================

isBodyConnective(Funct):-atom_concat(_,'_',Funct),!.
isBodyConnective(Funct):-atom_concat('t~',_,Funct),!.
isBodyConnective(Funct):-atom_concat('f~',_,Funct),!.
isBodyConnective(Funct):-member(Funct,[and,or,until,',',';',':-',unless,xor,holdsDuring]). % Other Propositional Wrappers

isEntityref(Var,Var):-isSlot(Var),!.
isEntityref(Term,A):-Term=..[F,A,B],!,atom_concat('$',_,F),!.


% ===============================================================================================
% ===============================================================================================

isLiteralTerm(A):-isLiteralTerm_util(A),!.
isLiteralTerm(not(A)):-isLiteralTerm_util(A),!.

isLiteralTerm_util(A):-var(A),!.
isLiteralTerm_util('$VAR'(_)):-!.
isLiteralTerm_util(string(_)):-!.
isLiteralTerm_util(A):-not(compound(A)),!.
isLiteralTerm_util(A):-string(A).

% ===============================================================================================
% ===============================================================================================

isEntitySlot(Term):-isSlot(Term),!.
isEntitySlot(Term):-not(compound(Term)),!.
isEntitySlot(Term):-isEntityFunction(Term,FnT,ArgsT),!.

% ===============================================================================================
% ===============================================================================================

isEntityFunction(Term,FnT,ArgsT):-isSlot(Term),!,fail.
isEntityFunction(Term,FnT,ArgsT):-atomic(Term),!,fail.
isEntityFunction(Term,FnT,ArgsT):-Term=..[FnT|ArgsT],hlPredicateAttribute(FnT,'Function'),!.



% ===================================================================
% getPrologVars/4. 
% ===================================================================

getPrologVars(Term, Vars, Singletons, Multiples) :-
    ((getPrologVars(Term, VarList, []),
    close_list(VarList),
    keysort(VarList, KeyList),
    split_key_list(KeyList, Vars, Singletons, Multiples))).

getPrologVars(Term,  [Term - x|V], V) :-isVarProlog(Term),!.
getPrologVars(Term, V, V) :-not(compound(Term)),!.
getPrologVars(Term,  V0, V) :-
	isQualifiedAs(Term,Type,PrologVar),
	Type \= existential,!,
	(isVarProlog(PrologVar) -> V0=[PrologVar - x|V]; V0=V),!.
	
getPrologVars(Term, V0, V) :- 
	 functor(Term, F, N),
	 getPrologVars(1, N, Term, V0, V).
getPrologVars(I, N, Term, V0, V) :-
    (  (I > N) -> V=V0
    ;   arg(I, Term, Arg),
	getPrologVars(Arg, V0, V1),
	J is I + 1,
	getPrologVars(J, N, Term, V1, V)
    ).

% ===================================================================
% getPrologVars/4. 
% ===================================================================

getAllPrologVars(Term, Vars, Singletons, Multiples) :-
    ((getAllPrologVars(Term, VarList, []),
    close_list(VarList),
    keysort(VarList, KeyList),
    split_key_list(KeyList, Vars, Singletons, Multiples))).

getAllPrologVars(Term,  [Term - x|V], V) :-isVarProlog(Term),!.
getAllPrologVars(Term, V, V) :-not(compound(Term)),!.
getAllPrologVars(Term, V0, V) :- 
	 functor(Term, F, N),
	 getAllPrologVars(1, N, Term, V0, V).
getAllPrologVars(I, N, Term, V0, V) :-
    (  (I > N) -> V=V0
    ;   arg(I, Term, Arg),
	getAllPrologVars(Arg, V0, V1),
	J is I + 1,
	getAllPrologVars(J, N, Term, V1, V)
    ).

% ===================================================================
% getSlots/4. Returns no Existential Body Vars
% ===================================================================

getSlots(Term, Vars, Singletons, Multiples) :-
    ((getSlots(Term, VarList, []),
    close_list(VarList),
    keysort(VarList, KeyList),
    split_key_list(KeyList, Vars, Singletons, Multiples))).

getSlots(Term,  [Term - x|V], V) :-isVarProlog(Term),!.
getSlots(Term, V, V) :-not(compound(Term)),!.
getSlots(Term, V, V) :-isHiddenSlot(Term),!.
getSlots(Term,  VO, V) :-
	isQualifiedAs(Term,existential,EVar),!,
	getSlots(EVar,  VO, V).
getSlots(Term,  V0, V) :-
	isQualifiedAs(Term,Type,PrologVar),!,
	(isVarProlog(PrologVar) -> V0=[Term - x|V]; V0=V),!.
getSlots(Term, V0, V) :- 
	 functor(Term, F, N),
	 getSlots(1, N, Term, V0, V).
getSlots(I, N, Term, V0, V) :-
    (  (I > N) -> V=V0
    ;   arg(I, Term, Arg),
	getSlots(Arg, V0, V1),
	J is I + 1,
	getSlots(J, N, Term, V1, V)
    ).


% ===================================================================
% ===================================================================

rnd(N):-number(N),X is random(100),!,X<N.

% ===================================================================
% ===================================================================
sameString(X,Y):-same_str(X,Y).

same_str(X,X):-!.
same_str(S,Y):-to_codes(S,SS),to_codes(Y,YS),!,SS=YS.

close_str(S,Y):-to_codes(S,SS),to_codes(Y,YS),!,toLowercase(SS,LSS),toLowercase(YS,LYS),!,LSS=LYS.

% ===================================================================
% ===================================================================

to_codes(A,O):-atom(A),!,atom_codes(A,O).
to_codes(S,O):-string(S),!,string_to_atom(S,A),to_codes(A,O).
to_codes(C,C).

% ===================================================================
% getConstants/4. 
% ===================================================================

getConstants(Types,Term, Vars, Singletons, Multiples) :-
    ((getConstants(Types,Term, VarList, []),
    close_list(VarList),
    keysort(VarList, KeyList),
    split_key_list(KeyList, Vars, Singletons, Multiples))).

getConstants(Types,Term, [Term - x|V], V) :- getConstants(Types,Term),!.
getConstants(Types,Term, V, V) :- var(Term),!.
getConstants(Types,Term,  FOUND, V) :-
            Term=..[L,I|ST],
            getConstants(Types,L, VL, []),
            consts_l(Types,[I|ST], FLIST),
            append(V,FLIST,UND),
            append(VL,UND,FOUND),!.

getConstants(Types,Term, V, V) :- !.
    
consts_l(Types,[],[]).
consts_l(Types,[L|IST], FLIST):-
         getConstants(Types,L, FOUND,[]), 
         consts_l(Types,IST, FOUNDMore), !,
         append(FOUND,FOUNDMore,FLIST).

    
getConstants(_,('.')):-!,fail.
getConstants(_,'[]'):-!,fail.
getConstants(_,'$VAR'(_)):-!,fail.
getConstants(atomic,A):-atomic(A).
getConstants(atom,A):-atom(A).
getConstants(skolems,'zzskFn'(_)).
getConstants(funct,'AssignmentFn'(_,_)).


% ===============================================================================================
% ===============================================================================================

retractallLogged(T):-
	format(':-retractall(~q).~n',[T]),
	retractall(T).

assertLogged(T):-
	format(':-assert(~q).~n',[T]),
	assert(T).



asserta_if_new(A):-A,!.
asserta_if_new(A):-asserta(A),!.


pvar_gen('$VAR'(N)):-idGen(N),!.


tn_link(Clause,ETracking,Context,ETracking):-!.

tn_link(Clause,ETracking,Context,theorya(Context,Num)):-atomic(ETracking),atom_codes(ETracking,[84,45|Rest]),!,number_codes(Num,Rest).
tn_link(Clause,ETracking,Context,theoryb(Context,ETracking)):-!.


ignore(X,Y):-ignore((X,Y)).


%   select(?Element, ?List, ?List2)
%   is true when the result of removing an occurrence of Element in List
%   is List2.
     /*
select(Element, [Element|Tail], Tail).
select(Element, [Head|Tail1], [Head|Tail2]) :-
        select(Element, Tail1, Tail2).
       */
	      

% ===================================================================
% has_singleton_vars(-Term).
% Checks for singleton variables
%
% Example:
%| ?- has_singleton_vars(a(X)).
% yes
%
%| ?- has_singleton_vars(a(X,X)).
% no
% ===================================================================


var_merge(L1,L2,O):-
	close_ll(L1,L11),
	close_ll(L2,L22),
	append(L1,L2,L4),
	catch(sort(L4,O),_,L4=O),!.
var_merge(L1,L2,L2):-!.

% ===============================================================================================
% ===============================================================================================

close_ll([],[]):-!.
close_ll(L1,[]):-isSlot(L1),!.
close_ll([H|T],[H|TT]):-close_ll(T,TT),!.


has_singleton_vars(Clause):-getPrologVars(Clause,_,[_|_],_).
has_singleton_vars(Clause):-getPrologVars(Clause,[_|_],_,[]).

% ===================================================================
% any_to_string(+Term,?String).
% converts any term to a list of prolog chars
%
% Example:
% | ?- any_to_string(X,Y).
% X = _h83
% Y = [95,104,56,51]
% | ?- any_to_string("X",Y).
% Y = [91,56,56,93]
% | ?- any_to_string("t(r)",Y).
% Y = [91,49,49,54,44,52,48,44,49,49,52,44,52,49,93]
% ===================================================================

any_to_string(Term,String) :- !,fmt_write_string(Atom,"%S",args(Term)),atom_codes(Atom,String).

% ===================================================================
% conv(?List,?Term) 
% conv_det(+List,-Term)
% term_to_list(?Term,?List)
% term_to_atomlist(?Term,?AtomList)

% Based on Conjuctions
% term_to_list/2 is the inverse of  conv(?List,?Term) and conv_det(+List,-Term)  

% term_to_atomlist/2 is a more agressive form of term_to_list
% Examples:
/*
| ?- conv(X,Y).
X = []
Y = true

| ?- conv([a,b],Y).
Y = (a  ','  b)

| ?- conv(Out,(a :- b)).
Out = [(a :- b)]


*/
% ===================================================================

conv([],true):-!.
conv([X|T],(X,R)) :- T \== [],!,conv(T,R).
conv([X],(X)).

% ===============================================================================================
% ===============================================================================================

conv_det([],true):-!.
conv_det(V1,_V2):-var(V1),!,fail.
conv_det([X|T],(X,R)) :- T \== [],!,conv_det(T,R).
conv_det([X],(X)).

% ===============================================================================================
% ===============================================================================================

term_to_list(Var,[]):-var(Var).
term_to_list(','(A,B,C,D,E,F),List):-!,term_to_list(((A,B),C),AL),term_to_list(((D,E),F),BL),append(AL,BL,List).
term_to_list(','(A,B,C,D,E),List):-!,term_to_list(((A,B),C),AL),term_to_list((D,E),BL),append(AL,BL,List).
term_to_list(','(A,B,C,D),List):-!,term_to_list(((A,B),C),AL),term_to_list((D),BL),append(AL,BL,List).
term_to_list(','(A,B,E),List):-!,term_to_list(((A,B)),AL),term_to_list((E),BL),append(AL,BL,List).
term_to_list((A,B),List):-!,term_to_list(A,AL),term_to_list(B,BL),append(AL,BL,List).
term_to_list(true,[]):-!.
term_to_list(List,List):-is_list(List).
term_to_list(Term,[Term]):-compound(Term).
term_to_list(Term,[Term]):-atomic(Term).

% ===============================================================================================
% ===============================================================================================

term_to_atomlist([Var],[Var]) :- ( var(Var) ; atomic(Var) ),!.
term_to_atomlist([TERM],[H|T]):-!,
         TERM=..[H|Args],
         term_to_atomlist(Args,T).
term_to_atomlist([H|T],List):-!,
         term_to_atomlist([H],HList),
         term_to_atomlist(T,TList),
         append(HList,TList,List).
term_to_atomlist(Term,AtomList):-!,
         conv(TermList,Term),
         term_to_atomlist(TermList,AtomList). 


% ===================================================================
% getSharedVariables(Term1,Term2).
% tests to see if Varables are shared between two Terms
% ===================================================================

getSharedVariables(Left,Right):-copy_term((Left,Right),(VLP,VRP)),numbervars((VLP,VRP)),
            term_to_atomlist(VLP,VLPP),term_to_atomlist(VRP,VRPP),
             member('$VAR'(N),VLPP),member('$VAR'(N),VRPP).
                                          
                                          
% ===================================================================
% delete_once/3.
/*
delete_once(+List1, ?Elem, ?List2)
Delete forall members of List1 that simultaneously equal with Elem and equal the result with List2. 
*/

% ===================================================================

delete_once([],X,X):-!.
delete_once(_,[],[]):-!.
delete_once([DeleteThisItem],[DeleteThisItem|ListOfMore],ListOfMore):-!.
delete_once([DeleteThisItem],[Skip|ListOfMore],[Skip|ListAfterDelete]):- !,
         delete_once([DeleteThisItem],ListOfMore,ListAfterDelete).
delete_once([DeleteThisItem|DeleteThese],SourceList,ResultList):-!,
         delete_once([DeleteThisItem],SourceList,DeleteThisItemResult),
         delete_once(DeleteThese,DeleteThisItemResult,ResultList).


% ===================================================================
% clean_true(+DirtyTerm,-CleanTerm).
% removes resundant true atoms from terms
% ===================================================================

clean_true(X,Y):-once((term_to_list(X,L))),once((delete(L,true,L2))),once((conv_det(L2,Y))).

% ===================================================================
% optional_bound(+Term1,+Term2).
% Prepairs an option binding and alway succeeds
% ===================================================================
optional_bound(Left,Right):-ignore(Left=Right).



% ===================================================================
% PREDICATE remove(void,voidlist,voidlist).
% ===================================================================
remove(X,[X|New],New):- !.
remove(X,[A|Old],[A|New]):-
	remove(X,Old,New).   /*  X!=A !! */

% ===================================================================
% Global Variable Manipuitalion for Moo
% ===================================================================

%global_set(Name,Value):-flag(Name,_,Value).

global_increment(Name):-flag(Name,N,N+1).

%global_get(Name,Value):- flag(Name,Value,Value).

copy_term(Term,TermCopy,CopyOfVars):-copy_term(Term,TermCopy),
               getPrologVars(TermCopy,CopyOfVars,_,_).


% ===================================================================
%    moo_B_consult/1
% ===================================================================

moo_B_consult(FileName):-
      [FileName],!.

moo_B_consult(FileName):-real_prolog_file_name(FileName,AbsoluteFile),
      [AbsoluteFile].

% ===============================================================================================
% ===============================================================================================

assert_prolog(X,_) :- (var(X)),!.
assert_prolog([H|T],Vars) :-list_to_term([H|T],Term),!,assert_prolog(Term,Vars).
assert_prolog(PTERM_NATIVE,Vars):-PTERM_NATIVE=..[C,X,Y],'surface-instance'(C,'Junctive',_),!,
         assert_prolog(X,Vars),
         assert_prolog(Y,Vars).
assert_prolog(X,_Vars) :- predicate_property(X,built_in),!. 
assert_prolog(X,_Vars) :- ground(X),retract(X),fail.
assert_prolog(X,_Vars) :- /* not(exists_in_database(X)),!, */ assert(X). %, writeIfOption(modification,(X),Vars).
assert_prolog(_X,_Vars) :- !. %,not(exists_in_database(X)), assert_prolog(X). %writeIfOption(disp_modification,assert_prolog(X),Vars)

assert_prolog(Context_atom,WFF,Vars):-
         add_context(Context_atom,WFF,WFFAC),
         assert_prolog(WFFAC,Vars).

% ===============================================================================================
% ===============================================================================================

do_to_conjuncts(Var,G):- (var(Var);var(G)),!.
do_to_conjuncts((A,B),G):- !,
         ignore(once(do_to_conjuncts(A,G))),
         ignore(once(do_to_conjuncts(B,G))).
do_to_conjuncts(A,G):- !,
         ignore((C=..[G,A],once(C))).  

do_to_conjuncts(Var,Var2,G):- (var(Var);var(Var2   );var(G)),!.

do_to_conjuncts(V,(A,B),G):- !, %numbervars((A,B)),
         ignore(once(do_to_conjuncts(V,A,G))),
         ignore(once(do_to_conjuncts(V,B,G))).

do_to_conjuncts(V,A,G):- !,
         ignore((C=..[G,V,A],once(C))).  
                        

% ===============================================================================================
% ===============================================================================================

mretractall(TERM):-!, functor(TERM,F,A),predicate_property(TERM,Z),!,abolish(F/A),predicate_property(TERM,Q),!,dynamic(F/A),predicate_property(TERM,P),!. 


% ===============================================================================================
% ===============================================================================================


exists_in_database((Y:-_)):-predicate_property(Y,built_in).
exists_in_database((X)):-predicate_property(X,built_in).
exists_in_database((Y:-X)):-!,not(predicate_property(Y,built_in)),clause(Y,X).
exists_in_database((Y)):-!,not(predicate_property(Y,built_in)),clause(Y,true).



% ===============================================================================================
% ===============================================================================================

clause_id_gen(CLID):-idGen(CLID).

% ===============================================================================================
% not assertzble objects
% ===============================================================================================

non_assertzble(Var):-var(Var),!.
non_assertzble([]):-!.
non_assertzble(end_of_file):-!.
non_assertzble('end-of-file'):-!.
non_assertzble(fail):-!.
non_assertzble(false):-!.
non_assertzble(true):-!.
non_assertzble(comment(_C)):-!.
non_assertzble(browser_only(_C)):-!.
non_assertzble(List):-is_list(List),length(List,X),X<8,!. 

% ===============================================================================================
% ===============================================================================================

once_ignore(X):-once(ignore(X)).


/*
% SWI Builtin
union([X|L1],L2,L3) :-
	identical_member(X,L2),
	!,
	union(L1,L2,L3).
union([X|L1],L2,[X|L3]) :-
	union(L1,L2,L3).
union([],L,L).
*/

assert_if_new(X):-catch(X,_,fail),!.
assert_if_new(X):-assertz(X).


% ===================================================================
% split_key_list/4. (Adapted from Richard O'Keefe). 
% ===================================================================


split_key_list([], [], [], []).
split_key_list([V-_,W-_|Vs], Vars, Singletons, Multiples) :- W == V, !,
    Vars = [V|Vars1],
    Multiples = [V|Multiples1],
    split_key_list(Vs, V, Vs1),
    split_key_list(Vs1, Vars1, Singletons, Multiples1).
split_key_list([V-_|Vs], [V|Vars], [V|Singletons], Multiples) :-
    split_key_list(Vs, Vars, Singletons, Multiples).
split_key_list([W - _|Vs], V, Vs1) :- W == V, !,
    split_key_list(Vs, V, Vs1).
split_key_list(Vs1, _, Vs1).

% ===============================================================================================
% ===============================================================================================

close_list([]):-!.
close_list([_]):-!.
close_list([_,_]):-!.
close_list([_,_|CLOSE]):-!,close_list(CLOSE).
close_list([_,_,_|CLOSE]):-!,close_list(CLOSE).
close_list([_,_,_,_,_|CLOSE]):-!,close_list(CLOSE).
close_list([_,_,_,_,_,_,_|CLOSE]):-!,close_list(CLOSE).

% ===============================================================================================
% ===============================================================================================

unbind_numbers('$VAR'(P),'$VAR'(P)):-!.
unbind_numbers('AssignmentFn'(P,Q),'AssignmentFn'(P,Q)):-!.
unbind_numbers(P,_):-number(P),!.
unbind_numbers(P,P):-var(P),!.
unbind_numbers(P,Q):- !,P =.. [F|ARGS],!,
                   unbind_numbers_l(ARGS,UARGS),
                    Q=..[F|UARGS],!.

unbind_numbers_l([],[]).
unbind_numbers_l([A|RGS],[U|ARGS]):-!,
         unbind_numbers(A,U),
         unbind_numbers_l(RGS,ARGS),!.

% ===============================================================================================
% ===============================================================================================

consult_as_dynamic(FilenameLocal):- 
         open(FilenameLocal,'read',R),   
         repeat,
         read(R,TERM),
         ((TERM = end_of_file -> (true,!) ; 
               ((
                 assert(TERM),fail
                 ))
         )),!,
         close(R).


% ===================================================================
% Destructive Freezing/Melting
% ===================================================================

call_frozen(Goal):-
	copy_term(Goal,Copy),
	crossref_vars(Goal,Copy,CopyKey),
	call(Copy),
	setarg_vars(Copy,CopyKey,Goal).
	

freeze_vars(Fml,Frozen,MeltKey):-
	copy_term(Fml,Frozen),
	crossref_vars(Fml,Frozen,MeltKey),
	numbervars(Frozen),!.
	
melt_vars(Frozen,[]=[],Frozen):- !.
melt_vars(Frozen,[OV|OL]=[FV|VL],Thawed):-
	subst(Frozen,FV,OV,Thawing),
	melt_vars(Thawing,OL=VL,Thawed),!.

% ===============================================================================================
% ===============================================================================================

setarg_vars(Frozen,[]=[],Frozen):- !.
setarg_vars(Frozen,OL=NL,Thawed):-
	OT=..[getPrologVars|OL],
	NT=..[getPrologVars|NL],
	setarg_vars(1,Frozen,OT,OL,NT,NL,Thawed).
	
setarg_vars(_,Thawed,OT,OL,_,[],Thawed):-!.
setarg_vars(N,Frozen,OT,[OH|OL],NT,[NH|NL],Thawed):-
	setarg(N,OT,NH),
	NN is N +1,
	setarg_vars(NN,Frozen,OT,OL,NT,NL,Thawed),!.
	
% ===============================================================================================
% ===============================================================================================

crossref_vars(Fml,Frozen,FmlVars = FrozenVars):-
	free_variables(Fml,FmlVars),
	free_variables(Frozen,FrozenVars),!.


% ===================================================================
% Substitution based on =
% ===================================================================

% Usage: repl(+Fml,+X,+Sk,?FmlSk)

replc(Fml,X,Sk,FmlSk):-
	(repl(Fml,X,Sk,FmlSk)),!.
       

repl(  Var, VarS,SUB,SUB ) :- Var=VarS,!.
repl(        P, X,Sk,        P1 ) :- functor(P,_,N),repl1( X, Sk, P, N, P1 ).

repl1( _,  _, P, 0, P  ).

repl1( X, Sk, P, N, P1 ) :- N > 0, P =.. [F|Args], repl2( X, Sk, Args, ArgS ),
            repl2( X, Sk, [F], [FS] ),
            P1 =.. [FS|ArgS].

repl2( _,  _, [], [] ).
repl2( X, Sk, [A|As], [Sk|AS] ) :- X = A, !, repl2( X, Sk, As, AS).
repl2( X, Sk, [A|As], [A|AS]  ) :- var(A), !, repl2( X, Sk, As, AS).
repl2( X, Sk, [A|As], [Ap|AS] ) :- repl( A,X,Sk,Ap ),repl2( X, Sk, As, AS).
repl2( X, Sk, L, L ).

weak_repl(  Var, VarS,SUB,SUB ) :- Var=VarS,!.
weak_repl(        P, X,Sk,        P1 ) :- functor(P,_,N),weak_repl1( X, Sk, P, N, P1 ).

weak_repl1( _,  _, P, 0, P  ).

weak_repl1( X, Sk, P, N, P1 ) :- N > 0, P =.. [F|Args], weak_repl2( X, Sk, Args, ArgS ),
            weak_repl2( X, Sk, [F], [FS] ),
            P1 =.. [FS|ArgS].

weak_repl2( _,  _, [], [] ).
weak_repl2( X, Sk, [A|As], [Sk|AS] ) :- X = A, !, weak_repl2( X, Sk, As, AS).
weak_repl2( X, Sk, [A|As], [A|AS]  ) :- var(A), !, weak_repl2( X, Sk, As, AS).
weak_repl2( X, Sk, [A|As], [Ap|AS] ) :- weak_repl( A,X,Sk,Ap ),weak_repl2( X, Sk, As, AS).
weak_repl2( X, Sk, L, L ).


% ===============================================================================================
% Shared with XSB
% ===============================================================================================

file_newer(F1,F2):- time_file(F1,T1),time_file(F2,T2),!,T1>T2.


% ===============================================================================================
% ===============================================================================================

%reconsult(F):-consult(F).

% ===============================================================================================
% ===============================================================================================

set_global_compiler_options(_Ignore).
load_dyn(X):-[X].
assert_new(X):-retractall(X),assert(X).

% ===============================================================================================
% ===============================================================================================

file_open(FileName,r,IOPort):- open(FileName,read,IOPort,[type(binary)]).
file_open(FileName,w,IOPort):- open(FileName,write,IOPort,[type(binary)]).
file_close(IOPort):-close(IOPort).

% ===============================================================================================
% ===============================================================================================

fmt_write_string(String,Format,Args):- catch(Args=..[_|FARGS],_,FARGS=[]),
      string_to_atom(Format,FMTSTR),
      fmtString(String,FMTSTR,FARGS).

fmt_write(Format,Args):- catch(Args=..[_|FARGS],_,FARGS=[]),
      string_to_atom(Format,FMTSTR),
      say(FMTSTR,FARGS).

fmt_write(OUTPUT,Format,Args):- catch(Args=..[_|FARGS],_,FARGS=[]),
      string_to_atom(Format,FMTSTR),
      say(OUTPUT,FMTSTR,FARGS).

real_file_name(X,X).

% ===============================================================================================
% ===============================================================================================

setPrologFlag(X,Y):-catch(current_prolog_flag(X,Y),_,fail).
setPrologFlag(_,' ').

% ===============================================================================================
% ===============================================================================================

real_kif_file_name(_FileName,AbsoluteFile):-
      once(add_file_extension(".kif",_FileName,SeeThisFile)),
      once(add_file_user_lib_directory(SeeThisFile,AbsoluteFile)).
                                                         
real_prolog_file_name(_FileName,AbsoluteFile):-
      once(add_file_extension(".P",_FileName,SeeThisFile)),
      once(add_file_user_lib_directory(SeeThisFile,AbsoluteFile)).
                                                         

add_file_user_lib_directory(_LocalFile,AbsoluteFile):- 
         name(_LocalFile,FileNameString),
         once((('LOGIC_ENGINE_RT'(RTD),!,name(RTD,RTDString));((writeIfOption(cb_error,'MOO_XSB_RT Not Set in Environment',_),RTDString=[])))),
         once(append(RTDString,[47|FileNameString],LEPAth)),
         name(AbsoluteFile,LEPAth).

add_file_extension(EXT,FileName,LocalFile):-atom_codes(Extension,EXT),
            safe_file_name_extension(FileName, Extension, LocalFile).

safe_file_name_extension(SourceFile,Ext,SurfaceFile):-
            actual_file_name(SourceFile,File),
            file_name_extension(Base,_,File), 
            file_name_extension(Base,Ext,SurfaceFile). 


file_get0(IOPort,end_of_file):-  at_end_of_stream(IOPort),!.
file_get0(IOPort,Char):- get_code(IOPort,Char),ignore((Char<0)).
 

file_getbuf(IOPort,1,Char,1):-get_char(IOPort,Char).

% ===============================================================================================
% ===============================================================================================

max(X,Y,Max) :-
	X >= Y ->
		Max = X;
	%true ->
		Max = Y.

% ===============================================================================================
% ===============================================================================================

min(X,Y,Min) :-
	X =< Y ->
		Min = X;
	%true ->
		Min = Y.

% ===============================================================================================
% ===============================================================================================


isCharCodelist([]).  isCharCodelist([A|T]):-integer(A),A>9,A<128,isCharCodelist(T).

% ===============================================================================================
% ===============================================================================================

nop.
nop(_).

% ===============================================================================================
% ===============================================================================================

conjoin(A,B,C) :-
	A == true ->
		C = B;
	B == true ->
		C = A;
	A == false ->
		C = false;
	B == false ->
		C = false;
	%true ->
		C = (A , B).

% ===============================================================================================
% ===============================================================================================

disjoin(A,B,C) :-
	A == true ->
		C = true;
	B == true ->
		C = true;
	A == false ->
		C = B;
	B == false ->
		C = A;
	%true ->
		C = (A ; B).

% ===============================================================================================
% ===============================================================================================


delete_ident(V,_,V):-isSlot(V),!.
delete_ident([],_,[]):-!.
delete_ident([Item|L1],ItemT,L2):-Item==ItemT, !,delete_ident(L1,ItemT,L2).
delete_ident([A|L1],ItemT,[A|L2]):-!,delete_ident(L1,ItemT,L2).

delete_idents(V,_,V):-isSlot(V),!.
delete_idents([],_,[]):-!.
delete_idents([Item|L1],ItemT,L2):-identical_membership(Item,ItemT),!,delete_idents(L1,ItemT,L2).
delete_idents([A|L1],ItemT,[A|L2]):-!,delete_idents(L1,ItemT,L2).

% ===============================================================================================
% ===============================================================================================

nth_identical_member(X,[],_):-!,fail.
nth_identical_member(X,[XX|_],1):-X==XX,!.
nth_identical_member(X,[_|XXs],NN):- nth_identical_member(X,XXs,N),NN is N + 1.

identical_membership(X,[]):-!,fail.
identical_membership(X,[XX|_]):-X==XX,!.
identical_membership(X,[_|XXs]):- identical_membership(X,XXs).

% ===============================================================================================
% ===============================================================================================
	             	 	
fdelete([],T,[]):-!.

fdelete([Replace|Rest],[H|T],Out):-
	functor(Replace,F,_),memberchk(F,[H|T]),!,
       fdelete(Rest,[H|T],Out),!.

fdelete([Replace|Rest],[H|T],[Replace|Out]):-!,
       fdelete(Rest,[H|T],Out),!.

fdelete([Replace|Rest],F,Out):-
	functor(Replace,F,_),!,%F=FF,
       fdelete(Rest,F,Out),!.

fdelete([Replace|Rest],F,[Replace|Out]):-
       fdelete(Rest,F,Out),!.

%:-ensure_loaded(opencyc_chatterbot_data).

/*****************************************************************************/
% echar_type(+Char,?Type)
%    Char is an ASCII code.
%    Type is whitespace, punctuation, numeric, alphabetic, or special.

echar_type(46,period) :- !.
echar_type(35,alphanumeric) :- !.
echar_type(58,alphanumeric) :- !.
echar_type(47,alphanumeric) :- !.
echar_type(X,alphanumeric) :- X >= 65, X =< 90, !.
echar_type(X,alphanumeric) :- X >= 97, X =< 123, !.
echar_type(X,alphanumeric) :- X >= 48, X =< 57, !.
echar_type(X,whitespace) :- X =< 32, !.
echar_type(X,punctuation) :- X >= 33, X =< 47, !.
echar_type(X,punctuation) :- X >= 58, X =< 64, !.
echar_type(X,punctuation) :- X >= 91, X =< 96, !.
echar_type(X,punctuation) :- X >= 123, X =< 126, !.
echar_type(_,special).

% ===============================================================================================
% ===============================================================================================

numbervars(X):-
   numbervars(X,'$VAR',0,_).

% ===============================================================================================
% ===============================================================================================

toUppercase([],[]).
toUppercase(I,O):-integer(I),!,to_upper(I,O).
toUppercase([A|L],[AO|LO]):-
   toUppercase(A,AO),
   toUppercase(L,LO).
toUppercase(MiXed,UPPER):-atom(MiXed),!,
   atom_codes(MiXed,Codes),
   toUppercase(Codes,UCodes),
   atom_codes(UPPER,UCodes).
toUppercase(A,A).

% ===============================================================================================
% ===============================================================================================

all_upper_atom(Atom):-
   atom_codes(Atom,Codes),
   all_upper_codes(Codes).

all_upper_codes([]).
all_upper_codes([H|T]):-
      upper_code(H),all_upper_codes(T).

upper_code(H):-H < 91.


% ===============================================================================================
% ===============================================================================================
assertOnLoad(X):-catch((functor(X,F,A),functor(N,F,A),!,call(N)),_,fail),!.
assertOnLoad(X):-assert(X).

toLowercase(V,V):-var(V).
toLowercase([],[]).
toLowercase([In|B],[LA|D]):-!,
	toLowercase(In,LA),!,
	toLowercase(B,D),!.

toLowercase(In,Out):-integer(In),to_lower(In,Out).

toLowercase(In,Out):-
	compound(In),!,In=..List,
	toLowercase(List,ToLo),!,
	Out=..ToLo,!.

toLowercase(In,LA):-
	atom(In),
	atom_codes(In,Codes),
	lcase_code_str(Codes,LCodes),!,
	atom_codes(LA,LCodes),!.

toLowercase(In,In).

make_term_lowercase_util(B,BL):-atom(B),toLowercase(B,BL),!.

lcase_code_str([],[]).
lcase_code_str([Code|Codes],[L|LCodes]):-
	 to_lower(Code,L),
	 lcase_code_str(Codes,LCodes).
	 

/*****************************************************************************/
% get_first_word(+String,-Rest,-Word) (final version)
%  Extracts the first Word from String; Rest is rest of String.
%  A word is a series of contiguous letters, or a series
%  of contiguous digits, or a single special character.
%  Assumes String does not begin with whitespace.

get_first_word([C|Chars],Rest,[C|RestOfWord]) :-
	echar_type(C,Type),
	get_first_word_aux(Type,Chars,Rest,RestOfWord).

get_first_word_aux(special,Rest,Rest,[]) :- !.
   % if Char is special, don''t read more chars.

get_first_word_aux(Type,[C|Chars],Rest,[C|RestOfWord]) :-
	echar_type(C,Type), !,
	get_first_word_aux(Type,Chars,Rest,RestOfWord).

get_first_word_aux(_,Rest,Rest,[]).   % if previous clause did not succeed.


/*****************************************************************************/
% ltrim_codes(+X,?Y)
%   Removes whitespace characters from the
%   beginning of string X, giving string Y.

ltrim_codes([C|Chars],Result) :-
	echar_type(C,whitespace), !,
	ltrim_codes(Chars,Result).

ltrim_codes(X,X).   % if previous clause did not succeed.


/*****************************************************************************/
% e_digit_value(?D,?V)
%  Where D is the ASCII code of a digit,
%  V is the corresponding number.

e_digit_value(48,0).
e_digit_value(49,1).
e_digit_value(50,2).
e_digit_value(51,3).
e_digit_value(52,4).
e_digit_value(53,5).
e_digit_value(54,6).
e_digit_value(55,7).
e_digit_value(56,8).
e_digit_value(57,9).


/*****************************************************************************/
% e_string_to_number(+S,-N)
%  Converts string S to the number that it
%  represents, e.g., "234" to 234.
%  Fails if S does not represent a nonnegative integer.

e_string_to_number(S,N) :-
	e_string_to_number_aux(S,0,N).

e_string_to_number_aux([D|Digits],ValueSoFar,Result) :-
	e_digit_value(D,V),
	NewValueSoFar is 10*ValueSoFar + V,
	e_string_to_number_aux(Digits,NewValueSoFar,Result).

e_string_to_number_aux([],Result,Result).


/*****************************************************************************/
% extract_atomics(+String,-ListOfAtomics) (second version)
%  Breaks String up into ListOfAtomics
%  e.g., " abc def  123 " into [abc,def,123].

extract_atomics(String,ListOfAtomics) :-getCycLTokens(String,ListOfAtomics).

/*****************************************************************************/
% e_clean_string(+String,-Cleanstring)
%  removes all punctuation characters from String and return Cleanstring

e_clean_string(X,X):-!.

e_clean_string([C|Chars],L) :- 
	echar_type(C,punctuation),
	e_clean_string(Chars,L), !.
e_clean_string([C|Chars],[C|L]) :-
	e_clean_string(Chars,L), !.
e_clean_string([C|[]],[]) :-
	echar_type(C,punctuation), !.
e_clean_string([C|[]],[C]).


/****************************************************************************/
% e_findnth(+List,+Number,-Element)
%    returns the Nth member of List in Element

e_findnth([E|_],1,E).
e_findnth([_|T],N,T1) :- V is N - 1,
                       e_findnth(T,V,T1).

	      
/****************************************************************************/
% e_match(+MatchRule,+InputList)
%    e_matches the MatchRule with the InputList. If they e_match, the variables
%    in the MatchRule are instantiated to one of three things:
%       an empty list
%       a single word
%       a list of words

e_match([A|_],[C|_]):-atom(A),atom(C),A\==C,!,fail.
e_match(A,C) :-free_variables(A,AV),!,
      e_match(A,AV,C).
 
e_match(A,AV,C):- e_match_aux1(A,C),not(member([],AV)),!.
e_match(A,AV,C):- e_match_aux2(A,C),not(member([],AV)).

e_match_aux1(A,C) :-
	member([*|T],A),
	nonvar(T),
	member(Tm,T),
	nonvar(Tm),
	e_replace([*|T],A,Tm,B),
	e_match_aux2(B,C),
	!, last(L,T), L = Tm.

e_match_aux2([],[]).
e_match_aux2([Item|Items],[Word|Words]) :-
	e_match_aux3(Item,Items,Word,Words),!.
e_match_aux2([Item1,Item2|Items],[Word|Words]) :-
	var(Item1),
	nonvar(Item2),
	Item2 == Word,!,
	e_match_aux2([Item1,Item2|Items],[[],Word|Words]).
e_match_aux2([Item1,Item2|Items],[Word|Words]) :-
	var(Item1),
	var(Item2),!,
	e_match_aux2([Item1,Item2|Items],[[],Word|Words]).
e_match_aux2([[]],[]).

e_match_aux3(Word,Items,Word,Words) :-
	e_match_aux2(Items,Words), !.
e_match_aux3([Word|Seg],Items,Word,Words0) :-
	append(Seg,Words1,Words0),
	e_match_aux2(Items,Words1).


/****************************************************************************/
% e_replace(+Element1,+List1,+Element2,-List2)
%    e_replaces all instances of Element1 in List1 with Element2 and returns
%       the new list as List2
%    does not e_replace variables in List1 with Element1

e_replace(_,[],_,[]).
e_replace(X,[H|T],A,[A|T2]) :- nonvar(H), H = X, !, e_replace(X,T,A,T2).
e_replace(X,[H|T],A,[H|T2]) :- e_replace(X,T,A,T2).
    

randomPick([],[]):-!.
randomPick(List,Pick):-length(List,N),X is random(N),nth0(X,List,Pick).

% ===============================================================================================
% ===============================================================================================

clean_out_atom(A,B):-
      atom_codes(A,M),
      clean_codes(M,MC),             
      atom_codes(B,MC),!.

% ===============================================================================================
% ===============================================================================================

clean_codes([],[]).
clean_codes([C|X],XX):-
      C<32,!,clean_codes(X,XX).
clean_codes([C|X],[C|XX]):-
      clean_codes(X,XX).

% ===============================================================================================
% ===============================================================================================

dumpList([]).
dumpList([Out|More]):-!,
   format(user_error,'~n-> ~q~n',[Out]),
   dumpList(More).

% ===============================================================================================
% ===============================================================================================

listAsString([],[]).
listAsString([A|List],[AA|ListS]):-
   asString(A,AA),
   listAsString(List,ListS).

asString(A,A):-isVarProlog(A),!.
asString(A,string([A])).

% ===============================================================================================
% ===============================================================================================

get_to_last_atom([],[],[]).
get_to_last_atom([A|M],[A|LA],Rest):-
      atomic(A),
      get_to_last_atom(M,LA,Rest).
get_to_last_atom(M,[],M).

% ===============================================================================================
% ===============================================================================================

% ===========================================================
% Channel be executed in Channel
% ===========================================================

download(URL):-file_base_name(URL,File),!,download(URL,File).
download(URL,File):-say([downloading,URL,File]),!,sformat(Shell,'wget -O ~w  ~w ',[File,URL]),string_to_atom(Shell,Cmd),shell(Cmd),!,
	say([done,downloading,File]).

% ===============================================================================================
% ===============================================================================================

use(URL):- file_base_name(URL,File),!,((download(URL,File),load_file(File))).

% ===============================================================================================
% ===============================================================================================

load_file(File):-file_name_extension(Base, rpm,File),!, sformat(Shell,'rpm --force -i ~w  ',[File]),string_to_atom(Shell,Cmd),shell(Cmd),!,say([done,installing,File]).
load_file(File):-file_name_extension(Base, 'P',File),!,ensure_loaded(File),say([done,compiling,File]).
load_file(File):-file_name_extension(Base, 'p',File),!,ensure_loaded(File),say([done,compiling,File]).
load_file(File):-file_name_extension(Base, 'pro',File),!,ensure_loaded(File),say([done,compiling,File]).
load_file(File):-file_name_extension(Base, 'swi',File),!,ensure_loaded(File),say([done,compiling,File]).
load_file(File):-file_name_extension(Base, 'PRO',File),!,ensure_loaded(File),say([done,compiling,File]).
load_file(File):-file_name_extension(Base, daml,File),file_name_extension(Base, cycl,KIFFile),!, sformat(Shell,'cat ~w |python ./rdfx2kif.py ~w > ~w.cycl  ',[File,Base,Base]),string_to_atom(Shell,Cmd),shell(Cmd),!,agent_load_kif_surface(KIFFile,Context,Agent,AX),say([loaded,AX,'axioms.  Now canonicalizing...']).
load_file(File):-file_name_extension(File, cycl,KIFFile),!,sformat(Shell,'cat ~w |python ./rdfx2kif.py ~w > ~w.cycl  ',[File,File,File]),string_to_atom(Shell,Cmd),shell(Cmd),!,agent_load_kif_surface(KIFFile,Context,Agent,AX),say([loaded,AX,'axioms.  Now canonicalizing...']).

perl(CMD):-sformat(Shell,'perl -c "~w"  ',[CMD]),string_to_atom(Shell,Cmd),shell(Cmd).

load_file(File):-file_name_extension(Base, E,File),!,say([E,'Extension is not yet regisitered']).


%surf(Num):-number(Num),mooCache(PredR,_,F,Vars,_,_,Num,_,_),say(F,Vars),fail.
%surf(Num):-say([assertion,Num,not,found]).

		
/*
doc(Const):-mooCache(PredR,_,documentation(Const,F),Vars,_,_,Num,_,_),!,my_toMarkup(cycl,F,Vars,O),say(O),!.
doc(Const):-say('Not Found').
  */

load_file_into_module(File):-load_file_into_module(File,user).

load_file_into_module(File,PublicName):-expand_file_name(File,[Name]),!,
	open(Name,'read',Stream),!,
	repeat,
		once(handle_stream(Stream,PublicName)),
		at_end_of_stream(Stream),!,close(Stream).
	
handle_stream(Stream,PublicName):-	
		catch(read_term(Stream,X,[syntax_errors(true),module(PublicName),term_positon('$stream_position'(CharIndex,LineNum,Line,Pos))]),E,say(E)),
		catch(process_read(PublicName,X),E,say(E)),flush_output(user_error),!.
	
% ===============================================================================================
% ===============================================================================================

process_read(PublicName,':-'(include(File))):-!. %,load_file_into_module(File,PublicName).
process_read(PublicName,':-'(X)):-!,PublicName:X.
process_read(PublicName,(X)):-!,assert(PublicName:X).


% ===============================================================================================
% ===============================================================================================

	
call_all(X):-X,fail.
call_all(X).

% ===============================================================================================
% ===============================================================================================

functor_member(Property,[]):-!,fail.
functor_member(Property,Specification):-
	functor(Property,S,_),!,
	member(Super,Specification),
	functor(Super,S,_).




atomSplit(Atom,Words):-var(Atom),!,
   concat_atom(Words,' ',Atom).
   
atomSplit(Atom,Words):-
      atom_codes(Atom,Codes),!,
      getCycLTokens(Codes,Words),!.

% ===================================================================
% Substitution based on ==
% ===================================================================

% Usage: subst(+Fml,+X,+Sk,?FmlSk)

subst(A,B,C,D):-notrace(nd_subst(A,B,C,D)),!.

nd_subst(  Var, VarS,SUB,SUB ) :- Var==VarS,!.
nd_subst(        P, X,Sk,        P1 ) :- functor(P,_,N),nd_subst1( X, Sk, P, N, P1 ).

nd_subst1( _,  _, P, 0, P  ).

nd_subst1( X, Sk, P, N, P1 ) :- N > 0, P =.. [F|Args], nd_subst2( X, Sk, Args, ArgS ),
            nd_subst2( X, Sk, [F], [FS] ),
            P1 =.. [FS|ArgS].

nd_subst2( _,  _, [], [] ).
nd_subst2( X, Sk, [A|As], [Sk|AS] ) :- X == A, !, nd_subst2( X, Sk, As, AS).
nd_subst2( X, Sk, [A|As], [A|AS]  ) :- var(A), !, nd_subst2( X, Sk, As, AS).
nd_subst2( X, Sk, [A|As], [Ap|AS] ) :- nd_subst( A,X,Sk,Ap ),nd_subst2( X, Sk, As, AS),!.
nd_subst2( X, Sk, L, L ).

weak_nd_subst(  Var, VarS,SUB,SUB ) :- Var=VarS,!.
weak_nd_subst(        P, X,Sk,        P1 ) :- functor(P,_,N),weak_nd_subst1( X, Sk, P, N, P1 ).

weak_nd_subst1( _,  _, P, 0, P  ).

weak_nd_subst1( X, Sk, P, N, P1 ) :- N > 0, P =.. [F|Args], weak_nd_subst2( X, Sk, Args, ArgS ),
            weak_nd_subst2( X, Sk, [F], [FS] ),
            P1 =.. [FS|ArgS].

weak_nd_subst2( _,  _, [], [] ).
weak_nd_subst2( X, Sk, [A|As], [Sk|AS] ) :- X = A, !, weak_nd_subst2( X, Sk, As, AS).
weak_nd_subst2( X, Sk, [A|As], [A|AS]  ) :- var(A), !, weak_nd_subst2( X, Sk, As, AS).
weak_nd_subst2( X, Sk, [A|As], [Ap|AS] ) :- weak_nd_subst( A,X,Sk,Ap ),weak_nd_subst2( X, Sk, As, AS).
weak_nd_subst2( X, Sk, L, L ).



