:-include('moo_header.pl').

prototype(TK,G,(TK,GN)):-copy_term(G,GN),numbervars(GN).

t_call(Label,TN,Goal):-((check_tabled_f(TN))),!,debug_w(proved_f(TN)),exit(Label,(!,fail)). % Deterministic Failure
t_call(Label,TN,Goal):-((check_tabled_t(TN))),!,debug_w(proved_t(TN)),exit(Label,true).    % Deterministic Exit
t_call(Label,TN,Goal):-((check_tabling_now(TN))),(((_,Goal)=TN)),!,debug_w(looped(TN)),exit(Label,(!,fail)). %force redo
t_call(Label,TN,Goal):-not((check_tabling_now(TN))),asserta(tabling(TN)),call(Goal),asserta(tabled_t(TN)),debug_w(proved(Goal)),fail(Label).
t_call(Label,TN,Goal):-!,debug_w(done_false(Goal)),asserta(tabled_f(TN)),exit(Label,true).

check_tabled_f((_,G)):-tabled_f((_,G)) .
check_tabled_t((TK,G)):-tabled_t((TK,G)) .
check_tabling_now((Ax:C,G)):-tabling((Ax:_,G)).

/*
and(X,Y):-X,Y. %clsid_gen(P),iff(P,X),iff(P,Y).
or(X,Y):-X;Y. %clsid_gen(P),iff(P,X);iff(P,Y).
*/

/* invokes the actuall goal*/


/*
  DEC-10 Prolog 4-port "Byrd Box" model

   See http://www.swi.psy.uva.nl/projects/SWI-Prolog/mailinglist/archive/0713.html for discussion with O'Keefe

   TN Represents the Top of the Explaination Tree
   Goal is a Prolog Goal

*/
:-dynamic(stop_request ).
:-dynamic(tabled_f/1 ).

/*

   iff(TK,Goal) :- prototype(TK,Goal,TN),
        (   (byrd_call(call_cut, TN , Goal,Cut),(Cut=yes ,!))
        ;   true
        ;   byrd_call(fail,TN, Goal)
        ),
        t_call(TN,Goal),
        (   byrd_call(exit,TN, Goal)
        ;   true
        ;   (byrd_call(redo, TN , Goal,Cut),(Cut=yes ,!,retractall(tabled(_)),retractall(tabled_f(_))))
        ).

t_call(TN,Goal):-not(not(check_tabled_f(TN))),!,debug_w(proved_f(TN)),fail. % Deterministic Failure
t_call(TN,Goal):-not(not(check_tabled_t(TN))),!,debug_w(proved_t(TN)).    % Deterministic Exit
t_call(TN,Goal):-not(not(check_tabling_now(TN))),debug_w(check_tabling_now(TN)),fail. %force redo
t_call(TN,Goal):-Goal,debug_w(proved(Goal)).
t_call(TN,Goal):-!,debug_w(non(Goal)),fail.
  */

%prototype(TK,G,(TK,G,GN)):-!,sformat(GN,'~p',[G]).

byrd_call(call_cut,TN, Goal,Cut) :-
       (((tabled_f(TN);
                check_tabling_now(TN)),Cut=yes);
                Cut=no,asserta(tabling(TN))),
       debug_w((call:TN:Goal:Cut)).

%check_tabling_now((TK,_,_)):-tabling((TK,_,_)).
%check_tabling_now((_,Goal,_)):-tabling((_,Goal,_)).
%check_tabling_now((_,_,Memo)):-tabling((_,_,Memo)).

byrd_call(exit,TN,  Goal) :-
        \+ (
       %TN=(TK,G,M),assert(tabling(TK,_,_)),
            %retract(tabling(TN)),
       asserta(tabled_t(TN)),debug_w((exit:TN:Goal))
        ),fail.

byrd_call(redo,TN, Goal,Cut) :-
         (((not(not(tabled_t(TN)));not(not(tabled_f(TN)))),Cut=yes);Cut=no,asserta(tabled_t(TN))),
       debug_w((redo:TN:cut(Cut):Goal)),fail.

byrd_call(fail,TN, Goal) :-
       debug_w((fail:TN:Goal)),
            retractall(tabling(TN)),
       asserta(tabled_f(TN))
         ,fail.

neg_equals(A,B):- not(A=B).


:-dynamic(tabling/1).

debug_w(('Completed' : 'Request' : goal :Goal)):-!. % each here
debug_w(X):-send_note(debug,true,'Step',X).
debug_w(_):-!.



% ===============================================================
% Adapted for LNProlog
% ===============================================================

% ===============================================================
% Richard O'Keefe's Iterative Deepener
% ===============================================================

id(Goal,DC) :-
    length(Bound, _),
    id(Goal, Bound, _,DC).

id(Goal, _, _,DC) :- DC <1,!,fail.

id(Goal, _, _,DC) :-
   fact(Goal,Context,Context,ETracking,ITracking).

id(Goal, Bound, Bound,DC) :-
   fact(Goal,Context,Context,ETracking,ITracking).

id(Goal, [_|Bound0], Bound,DC) :-
    rule(Goal , Cost, Body, Context, Ctx, TN, CLID),
    id_body(Body, Bound0, Bound,DC).

id_body([], Bound, Bound,DC).
id_body([Goal|Goals], Bound0, Bound,DC) :-
   DC2 is DC -1,
    id(Goal, Bound0, Bound1,DC2),
    id_body(Goals, Bound1, Bound,DC2).





iff(_,true):-!.
iff(_,neg(X)):-!, not (X).
iff(TK,(GoalA,GoalB)):-!,
         iff(TK,GoalA),iff(TK,GoalB).
iff(TK,Goal):-prototype(TK,Goal,Proto),clsid_gen(Label),block(Label,t_call(Label,Proto,Goal),Ret),Ret.



abolish_all_tables:-retractall(tabled_f(_)),retractall(tabled_t(_)),retractall(tabling(_)).
show_all_tables:-listing(tabled_f(_)),listing(tabled_t(_)),listing(tabling(_)).

/*
 breadth-first evaluation

    bounded depth first iterative deepening search interpreter
     (ie fair search strategy)
     won't work with side effects but other sys preds ok

     returns forall solutions to Goal eventually
     avoids duplicating solutions where Prolog wouldn't
*/

/*
     meta interpreter with depth bound (fail if bound gets to 0)
     Doesn't return success unless depth is within Delta of bound
     (to stop multiple answers being returned on successive
     iterations with increasing depth)

     Should really have goal in first argument for indexing

     (NU-Prolog will index on third arg due to when declaration).

     ?- fair_solve(_, _, G) when G.

     bounded depth first iterative deepening search interpreter
     (ie fair search strategy)
     won't work with side effects but other sys preds ok

     returns forall solutions to Goal eventually
     avoids duplicating solutions where Prolog wouldn't
*/
fair_solve(Goal) :-
    gen_depth(D, Delta),
    fair_solve(D, Delta, Goal).

%fair_solve(G):-id(G,4).

/*
     meta interpreter with depth
     bound (fail if bound gets to 0)
     Doesn't return success unless depth is within Delta of bound
     (to stop multiple answers being returned on successive
     iterations with increasing depth)

     Should really have goal in first argument for indexing

     (NU-Prolog will index on third arg due to when declaration).

     ?- fair_solve(_, _, G) when G.
*/
fair_solve(D, Delta, true) :-
    !,
    D < Delta.

fair_solve(D, Delta, (A, B)) :-
    !,
    fair_solve(D, Delta, A),
    fair_solve(D, Delta, B).

/*fair_solve(D, Delta, (A ; B)) :-
    !,
    (   fair_solve(D, Delta, A)
    ;
        fair_solve(D, Delta, B)
    ).
    */
/*  should do more system preds explicitly  */

fair_solve(D, Delta, A) :-
    fact(A,Context,Context,ETracking,ITracking), fail,
    !. %,
    %call(A).

fair_solve(D, Delta, A) :-
    D > 0,
    D1 is D - 1,
    rule(A, Depth, B,Context,TN, CLID),
    fair_solve(D1, Delta, B).


/*
     generate increasing depth bounds
     and difference between them
*/
%gen_depth(X, Y):- X is rnd(
gen_depth(3, 7).

%gen_depth(D, Delta) :-
 %   gen_depth(3, 7, D, Delta).

    /*  initial depth, big delta  */


%gen_depth(D, Delta, D, Delta).

%gen_depth(D0, _, D, Delta) :- catch((
 %   R is (D0) / 2 ,
  %  Delta1 is integer( R ) + 1,
    /*  anything > 0 would do  */
  %  D1 is D0 + Delta1,
    /*  D1 is next depth  */
   % gen_depth(D1, Delta1, D, Delta))  ,_,((D1=2,Delta=6))).






save_db_state(F):-
      tell(F),
      listing(rule),
      listing(fact),
      listing(resolve_skolem),
      told.

wr(KR,pnf(KR,Ctx,TN)):-rule(Goal, Depth, Body,Context,TN, CLID),
            KP= (=>(Body,Goal)),
            prolog_to_krlog(KP,KR).




% ==================================================================
% 'Lambda', from Clocksin, "Clause and Effect"
% Case Study: Higher-Order Functional Programming
% Examples:
% | ?- eval(sum@[3,4],X).
% X = 7
% yes
%
%| ?- eval(lessThan@[div@[2,4],div@[3,4]],X).
% X = true
% yes
%
%| ?- eval(mult@[sum@[2,4],sum@[3,4]],X).
%X = 42
% yes
% ===================================================================
/*
fact('instance-of'(A,C),Context,(TN1:TN2), (CN1:CN2)):-
         fact('instance-of'(A,B),Context,TN1, CN1),
         fact('subclass-of'(B,C),Context,TN2, CN2).
*/
%fact('same'(A,A),All_Contexts,All_Ctxs,'Kernel', 'Skolemization').



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% $Id: moo_tap.pl,v 1.4 2002-03-14 12:46:24 dmiles Exp $
% Sicstus Prolog
% Copyright (C) 1993: Bernhard Beckert and Joachim Posegga
%                     Universit"at Karlsruhe
%                     Email: {beckert|posegga}@ira.uka.de
%
% Purpose:            \LeanTaP: tableau-based theorem prover for NNF.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%:- module(leantap,[lt_prove/2,prove_uv/2]).

%:-     use_module(library(lists),[append/3]).
%:-     use_module(unify,[unify/2]).

%%%%%%%%%% BEGIN OF TOPLEVEL PREDICATES

% -----------------------------------------------------------------
% lt_prove(+Fml,?VarLim)
% prove_uv(+Fml,?VarLim)
%
% succeeds if there is a closed tableau for Fml with not more
% than VarLim free variables on each branch.
% prove_uv uses universal variables, lt_prove does not.
%
% Iterative deepening is used when VarLim is unbound.
% Examples:
%
% | ?- lt_prove((p(a) , -p(f(f(a))) , forall(X,(-p(X) ; p(f(X))))), 1).
% no
% | ?- lt_prove((p(a) , -p(f(f(a))) , forall(X,(-p(X) ; p(f(X))))), 2).
% yes
%

lt_solve(Fml):-
   lt_prove(Fml,VarLim).

lt_prove(Fml,VarLim) :- nonvar(VarLim),!,lt_prove(Fml,[],[],[],VarLim).
lt_prove(Fml,Result) :-
        iterate(VarLim,1,lt_prove(Fml,[],[],[],VarLim),Result).

prove_uv(Fml,VarLim) :- nonvar(VarLim),!,lt_prove(Fml,[],[],[],[],[],VarLim).

prove_uv(Fml,Result) :-
        iterate(VarLim,1,lt_prove(Fml,[],[],[],[],[],VarLim),Result).


iterate(Current,Current,Goal,Current) :-
        Goal.

iterate(VarLim,Current,Goal,Result) :-
        Current1 is Current + 1,
        iterate(VarLim,Current1,Goal,Result).

%%%%%%%%%% END OF TOPLEVEL PREDICATES

% -----------------------------------------------------------------
% lt_prove(+Fml,+UnExp,+Lits,+FreeV,+VarLim)
%
% succeeds if there is a closed tableau for Fml with not more
% than VarLim free variables on each branch.
%  Fml: inconsistent formula in skolemized negation normal form.
%       syntax: negation: '-', disj: ';', conj: ','
%       quantifiers: 'forall(X,<Formula>)', where 'X' is a prolog variable.
%
%  UnExp:    list of formula not yet expanded
%  Lits:     list of literals on the current branch
%  FreeV:    list of free variables on the current branch
%  VarLim:   max. number of free variables on each branch
%            (controlls when backtracking starts and alternate
%            substitutions for closing branches are considered)
%

lt_prove(Goal,UnExp,Lits,FreeV,VarLim) :- fact(Goal,Context,TN, CLID).

lt_prove((A,B),UnExp,Lits,FreeV,VarLim) :- !,
        lt_prove(A,[B|UnExp],Lits,FreeV,VarLim).

lt_prove((A,B),UnExp,Lits,FreeV,VarLim) :- !,
        lt_prove(A,[B|UnExp],Lits,FreeV,VarLim).

lt_prove((A;B),UnExp,Lits,FreeV,VarLim) :- !,
        lt_prove(A,UnExp,Lits,FreeV,VarLim),
        lt_prove(B,UnExp,Lits,FreeV,VarLim).

lt_prove(forall(X,Fml),UnExp,Lits,FreeV,VarLim) :- !,
        \+ length(FreeV,VarLim),
        copy_term((X,Fml,FreeV),(X1,Fml1,FreeV)),
        append(UnExp,[forall(X,Fml)],UnExp1),
        lt_prove(Fml1,UnExp1,Lits,[X1|FreeV],VarLim).

lt_prove(Lit,_,[L|Lits],_,_) :-
        (Lit =   not  Neg;  not  Lit = Neg) ->
        (unify(Neg,L); lt_prove(Lit,[],Lits,_,_)).

lt_prove(Lit,[Next|UnExp],Lits,FreeV,VarLim) :-
        lt_prove(Next,UnExp,[Lit|Lits],FreeV,VarLim).

% -----------------------------------------------------------------
% lt_prove(+Fml,+UnExp,+Lits,+DisV,+FreeV,+UnivV,+VarLim)
%
% same as lt_prove/5 above, but uses universal variables.
% additional parameters:
% DisV:   list of  not -universal variables on branch
% UnivV:  list of universal variables on branch

lt_prove((A,B),UnExp,Lits,DisV,FreeV,UnivV,VarLim) :- !,
        lt_prove(A,[(UnivV:B)|UnExp],Lits,DisV,FreeV,UnivV,VarLim).

lt_prove((A;B),UnExp,Lits,DisV,FreeV,UnivV,VarLim) :- !,
        copy_term((Lits,DisV),(Lits1,DisV)),
        lt_prove(A,UnExp,Lits,(DisV+UnivV),FreeV,[],VarLim),
        lt_prove(B,UnExp,Lits1,(DisV+UnivV),FreeV,[],VarLim).

lt_prove(forall(X,Fml),UnExp,Lits,DisV,FreeV,UnivV,VarLim) :- !,
        \+ length(FreeV,VarLim),
        copy_term((X,Fml,FreeV),(X1,Fml1,FreeV)),
        append(UnExp,[(UnivV:forall(X,Fml))],UnExp1),
        lt_prove(Fml1,UnExp1,Lits,DisV,[X1|FreeV],[X1|UnivV],VarLim).

lt_prove(Lit,_,[L|Lits],_,_,_,_) :-
        (Lit = -Neg; -Lit = Neg ) ->
        (unify(Neg,L); lt_prove(Lit,[],Lits,_,_,_,_)).

lt_prove(Lit,[(UnivV:Next)|UnExp],Lits,DisV,FreeV,_,VarLim) :-
        lt_prove(Next,UnExp,[Lit|Lits],DisV,FreeV,UnivV,VarLim).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% $Id: moo_tap.pl,v 1.4 2002-03-14 12:46:24 dmiles Exp $
% Sicstus Prolog
% Copyright (C) 1993: Bernhard Beckert and Joachim Posegga
%                     Universit"at Karlsruhe
%                     Email: {beckert|posegga}@ira.uka.de
%
% Purpose:            Testdata for \LeanTaP
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%:- module(leantest,[provefml/1,incprovefml/1,uv_provefml/1,uv_incprovefml/1]).
:- op(400,fy,-), op(500,xfy,and), op(600,xfy,v),op(650,xfy, => ), op(700,xfy, <=> ).

%:-     use_module(leantap,[lt_prove/2,prove_uv/2]).
%:-     use_module(nnf,[nnf/2]).

%:-     use_module(library(lists),[member/2]).


% ------------------------------------------------------------
%
% provefml(Name)
% calls fml(Name,Limit,Fml) and tries to lt_prove Fml with VarLim = Limit.
%
% use 'provefml(_),fail.' to lt_prove forall formulae in the database


provefml(Name) :-
        fml(Name,Limit,F),
        write(Name),
        nnf(F,NNF),
   system_dependant:prolog_statistics(cputime,_),
        (lt_prove(NNF,Limit)
           -> (system_dependant:prolog_statistics(cputime,Time),
               format(' proved in ~w msec, VarLim = ~w ~n',[Time,Limit]))
           ; (system_dependant:prolog_statistics(cputime,Time),
              format(' no explaination after ~w msec. ~n',[Time]))).

% ------------------------------------------------------------
%
% incprovefml(Name)
% same as provefml/1, but VarLimit is determined by iterative deepening
%
% 'incprovefml(_),fail.' runs over forall problems in the database

incprovefml(Name) :-
        fml(Name,_,F),
        write(Name),
        nnf(F,NNF),
        system_dependant:prolog_statistics(cputime,_),
        (lt_prove(NNF,Limit)
           -> (system_dependant:prolog_statistics(cputime,Time),
             format('  proved in ~w msec, found VarLim = ~w ~n',[Time,Limit]))
           ; (system_dependant:prolog_statistics(cputime,Time),
              format(' no explaination after ~w msec. ~n',[Time]))).

% ------------------------------------------------------------
%
% uv_provefml(Name)
% calls fml(Name,Limit,Fml) and tries to lt_prove Fml with VarLim = Limit
% and universal variables
%
% use 'uv_provefml(_),fail.' to lt_prove forall formulae in the database


uv_provefml(Name) :-
        fml(Name,Limit,F),
        write(Name),
        nnf(F,NNF),
        system_dependant:prolog_statistics(cputime,_),
        (prove_uv(NNF,Limit)
           -> (system_dependant:prolog_statistics(cputime,Time),
               format(' proved in ~w msec, VarLim = ~w ~n',[Time,Limit]))
           ; (system_dependant:prolog_statistics(cputime,Time),
              format(' no explaination after ~w msec. ~n',[Time]))).

% ------------------------------------------------------------
%
% uv_incprovefml(Name)
% same as uv_provefml/1, but VarLimit is determined by iterative deepening
%
% 'uv_incprovefml(_),fail.' runs over forall problems in the database

uv_incprovefml(Name) :-
        fml(Name,_,F),
        write(Name),
        nnf(F,NNF),
        system_dependant:prolog_statistics(cputime,_),
        (prove_uv(NNF,Limit)
           -> (system_dependant:prolog_statistics(cputime,Time),
             format('  proved in ~w msec, found VarLim = ~w ~n',[Time,Limit]))
           ; (system_dependant:prolog_statistics(cputime,Time),
              format(' no explaination after ~w msec. ~n',[Time]))).


%%%%%%%%%% DATABASE OF PELLETIER:

%  not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not   not
% fml(?Name,?VarLim,?Fml)
%
% stores formula Fml under Name with VarLim.
%
% This database consists of a subset of Pelletier's formulae


fml(pel1,0, not ((p  =>  q)  <=>  ( not  q  =>   not  p))).

fml(pel2,0, not ( not ( not  p)  <=>  p)).

fml(pel3,0, not ( not  (p  =>  q)  =>  (q  =>  p))).

fml(pel4,0, not (( not  p  =>  q)  <=>  ( not  q  =>  p))).

fml(pel5,0, not (((p or q)  =>  (p or r))  =>  (p or (q  =>  r)))).

fml(pel6,0, not  (p or  not  p)).

fml(pel7,0, not  (p or  not ( not ( not  p)))).

fml(pel8,0, not (((p  =>  q)  =>  p)  =>  p)).

fml(pel9,0, not ((((p or q) and ( not  p or q)) and (p or  not  q))  =>   not ( not  p or  not  q))).

fml(pel10,0, not  (p  <=>  q) and
  (q  =>  r) and
  (r  =>  (p and q)) and
  (p  =>  (q or r))).


fml(pel11,0, not  (p  <=>  p)).


fml(pel12,0, not (((p  <=>  q)  <=>  r)  <=>  (p  <=>  (q  <=>  r)))).


fml(pel13,0, not ((p or (q and r))  <=>  ((p or q) and (p or r)))).


fml(pel14,0, not ((p  <=>  q)  <=>  ((q or  not  p) and ( not  q or p)))).


fml(pel15,0, not ((p  =>  q)  <=>  ( not  p or q))).


fml(pel16,0, not ((p  =>  q) or (q  =>  p))).


fml(pel17,0, not (((p and (q  =>  r))  =>  s)  <=>  ((( not  p or q) or s) and (( not  p or  not  r) or s)))).


fml(pel18,2, not  exists(Y, forall(X, (f(Y)  =>  f(X))))).


fml(pel19,2, not  exists(X, forall(Y, forall(Z, ((p(Y)  =>  q(Z))  =>  (p(X)  =>  q(X))))))).


fml(pel20,6, not  (forall(X, forall(Y, exists(Z, forall(W, ((p(X) and q(Y))  =>  (r(Z) and s(W)))))))  =>  exists(X1, exists(Y1, ((p(X1) and q(Y1))  =>  exists(Z1, (r(Z1)))))))).


fml(pel21,2, not  exists(X, (p  <=>  f(X))) and
  exists(X1, (p  =>  f(X1))) and
  exists(X2, (f(X2)  =>  p))).


fml(pel22,2, not  (forall(X, (p  <=>  f(X)))  =>  (p  <=>  forall(X1, (f(X1)))))).


fml(pel23,1, not  (forall(X, (p or f(X)))  <=>  (p or forall(X1, (f(X1)))))).


fml(pel24,6, not  exists(X, (p(X) and r(X))) and
   not  exists(X1, (s(X1) and q(X1))) and
  forall(X4, (p(X4)  =>  (q(X4) or r(X4)))) and
  ( not  exists(X2, (p(X2)))  =>  exists(Y, (q(Y)))) and
  forall(X3, ((q(X3) or r(X3))  =>  s(X3)))).


fml(pel25,3, not  exists(X, (q(X) and p(X))) and
  exists(X1, (p(X1))) and
  forall(X2, (f(X2)  =>  ( not  g(X2) and r(X2)))) and
  forall(X3, (p(X3)  =>  (g(X3) and f(X3)))) and
  (forall(X4, (p(X4)  =>  q(X4))) or exists(Z, (p(Z) and r(Z))))).


fml(pel26,3, not  (forall(X, (p(X)  =>  r(X)))  <=>  forall(Y, (q(Y)  =>  s(Y)))) and
  (exists(X1, (p(X1)))  <=>  exists(Y2, (q(Y2)))) and
  forall(X2, forall(Y3, ((p(X2) and q(Y3))  =>  (r(X2)  <=>  s(Y3)))))).


fml(pel27,4, not  forall(X, (j(X)  =>   not  i(X))) and
  exists(X1, (f(X1) and  not  g(X1))) and
  forall(X2, (f(X2)  =>  h(X2))) and
  forall(X4, ((j(X4) and i(X4))  =>  f(X4))) and
  (exists(X5, (h(X5) and  not  g(X5)))  =>  forall(X6, (i(X6)  =>   not  h(X6))))).


fml(pel28,3, not  forall(X, ((p(X) and f(X))  =>  g(X))) and
  forall(X1, (p(X1)  =>  forall(Z, (q(Z))))) and
  (forall(X2, (q(X2) or r(X2)))  =>  exists(X21, (q(X21) and s(X21)))) and
  (exists(X3, (s(X3)))  =>  forall(X31, (f(X31)  =>  g(X31))))).


fml(pel29,2, not ((forall(X, (f(X)  =>  h(X))) and forall(U, (g(U)  =>  j(U))))  <=>  forall(W, forall(Y, ((f(W) and g(Y))  =>  (h(W) and j(Y)))))) and
  (exists(X1, (f(X1))) and exists(Y1, (g(Y1))))).


fml(pel30,2, not  forall(X, (i(X))) and
  forall(X1, ((f(X1) or g(X1))  =>   not  h(X1))) and
  forall(X2, ((g(X2)  =>   not  i(X2))  =>  (f(X2) and h(X2))))).


fml(pel31,3, not  exists(X, (i(X) and j(X))) and
   not  exists(X1, (f(X1) and (g(X1) or h(X1)))) and
  exists(X2, (i(X2) and f(X2))) and
  forall(X3, ( not  h(X3)  =>  j(X3)))).


fml(pel32,3, not  forall(X, ((f(X) and k(X))  =>  j(X))) and
  forall(X1, ((f(X1) and (g(X1) or h(X1)))  =>  i(X1))) and
  forall(X2, ((i(X2) and h(X2))  =>  j(X2))) and
  forall(X3, (k(X3)  =>  h(X3)))).


fml(pel33,1, not  (forall(X, ((p(a) and (p(X)  =>  p(b)))  =>  p(c)))  <=>  forall(X1, (( not  p(a) or (p(X1) or p(c))) and ( not  p(a) or ( not  p(b) or p(c))))))).


fml(pel34,5, not ((exists(X, forall(Y, (p(X)  <=>  p(Y))))  <=>  (exists(U, (q(U)))  <=>  forall(W, (q(W)))))  <=>  (exists(X1, forall(Y1, (q(X1)  <=>  q(Y1))))  <=>  (exists(U1, (p(U1)))  <=>  forall(W1, (p(W1))))))).


fml(pel35,4, not  exists(X, exists(Y, (p(X,Y)  =>  forall(Z, forall(W, (p(Z,W)))))))).


fml(pel36,6, not  forall(X, exists(Y, (h(X,Y)))) and
  forall(X1, exists(Y2, (f(X1,Y2)))) and
  forall(X2, exists(Y1, (g(X2,Y1)))) and
  forall(X3, forall(Y3, ((f(X3,Y3) or g(X3,Y3))  =>  forall(Z3, ((f(Y3,Z3) or g(Y3,Z3))  =>  h(X3,Z3))))))).


fml(pel37,7, not  forall(X, exists(Y, (r(X,Y)))) and
  forall(Z, exists(W, forall(X1, exists(Y1, (p(X1,Z)  =>  ((p(Y1,W) and p(Y1,Z)) and (p(Y1,W)  =>  exists(U, (q(U,W)))))))))) and
  forall(X2, forall(Z2, ( not  p(X2,Z2)  =>  exists(Y2, (q(Y2,Z2)))))) and
  (exists(X3, exists(Y3, (q(X3,Y3))))  =>  forall(Z3, (r(Z3,Z3))))).


fml(pel38,4, not  (forall(X, ((p(a) and (p(X)  =>  exists(Y, (p(Y) and r(X,Y)))))  =>  exists(Z, exists(W, ((p(Z) and r(X,W)) and r(W,Z))))))  <=>  forall(X1, (((( not  p(a)) or p(X1)) or exists(Z1, exists(W1, ((p(Z1) and r(X1,W1)) and r(W1,Z1))))) and ((( not  p(a)) or ( not  exists(Y1, (p(Y1) and r(X1,Y1))))) or exists(Z2, exists(W2, ((p(Z2) and r(X1,W2)) and r(W2,Z2))))))))).


fml(pel39,1,exists(X, forall(Y, (f(Y,X)  <=>   not  f(Y,Y))))).


fml(pel40,3, not  (exists(Y, forall(X, (f(X,Y)  <=>  f(X,X))))  =>   not  forall(X1, exists(Y1, forall(Z, (f(Z,Y1)  <=>   not  f(Z,X1))))))).


fml(pel41,3,exists(Z, forall(X, (f(X,Z)))) and
  forall(Z1, exists(Y, forall(X1, (f(X1,Y)  <=>  (f(X1,Z1) and  not  f(X1,X1))))))).


fml(pel42,3,exists(Y, forall(X, (f(X,Y)  <=>   not  exists(Z, (f(X,Z) and f(Z,X))))))).


fml(pel43,5, not  forall(X,forall(Y, ((q(X,Y)  =>  q(Y,X)) and (q(Y,X)  =>  q(X,Y))))) and
  forall(X1,forall(Y1,(q(X1,Y1)  =>  forall(Z, ((f(Z,X1)  =>  f(Z,Y1)) and (f(Z,Y1)  =>  f(Z,X1))))))) and
  forall(X2,forall(Y2,(forall(Z2, ((f(Z2,X2)  =>  f(Z2,Y2)) and (f(Z2,Y2)  =>  f(Z2,X2))))  =>  q(X2,Y2))))).


fml(pel44,3, not  exists(X, (j(X) and  not  f(X))) and
  forall(X1, (f(X1)  =>  (exists(Y, (g(Y) and h(X1,Y))) and exists(Y1, (g(Y1) and  not  h(X1,Y1)))))) and
  exists(X2, (j(X2) and forall(Y2, (g(Y2)  =>  h(X2,Y2)))))).


fml(pel45,5, not  exists(X, (f(X) and  not  exists(Y, (g(Y) and h(X,Y))))) and
  forall(X1, ((f(X1) and forall(Y, ((g(Y) and h(X1,Y))  =>  j(X1,Y))))  =>  forall(Y1, ((g(Y1) and h(X1,Y1)) and k(Y1))))) and
   not  exists(Y2, (l(Y2) and k(Y2))) and
  exists(X2, ((f(X2) and forall(Y3, (h(X2,Y3)  =>  l(Y3)))) and forall(Y11, ((g(Y11) and h(X2,Y11))  =>  j(X2,Y11)))))).


fml(pel46,5, not  forall(X, (f(X)  =>  g(X))) and
  forall(X1,forall(Y,(f(X1) and ((f(Y) and h(Y,X1))  =>  g(Y))  =>  g(X1)))) and
  (exists(X2, (f(X2) and  not  g(X2)))  =>  exists(X21, ((f(X21) and  not  g(X21)) and forall(Y2, ((f(Y2) and  not  g(Y2))  =>  j(X21,Y2)))))) and
  forall(X3,forall(Y3,((f(X3) and f(Y3) and h(X3,Y3))  =>   not  j(Y3,X3))))).

fml(pel47,44,(( not  (exists(X1,exists(Y1,(animal(X1) and animal(Y1) and exists(Z1,(isagrain(Z1) and likestoeat(Y1,Z1)) and likestoeat(X1,Y1)))))))

 and forall(X2,(wolf(X2)         =>  animal(X2))) and exists(X3,wolf(X3))
 and forall(X4,(fox(X4)          =>  animal(X4))) and exists(X5,fox(X5))
 and forall(X6,(bird(X6)         =>  animal(X6))) and exists(X7,bird(X7))
 and forall(X8,(caterpillar(X8)  =>  animal(X8))) and exists(X9,caterpillar(X9))
 and forall(X10,(snail(X10)      =>  animal(X10))) and exists(X11,snail(X11))

 and exists(X12,isagrain(X12)) and forall(X13,(isagrain(X13)  =>  isaplant(X13)))

 and forall(X14,(animal(X14)  =>  (forall(Y2,isaplant(Y2)  =>  likestoeat(X14,Y2)) v
                      forall(Y3,((animal(Y3) and smaller(Y3,X14) and exists(Z1,(isaplant(Z1) and likestoeat(Y3,Z1))))  =>
                              likestoeat(X14,Y3))))))

 and forall(X15,forall(Y4,((bird(Y4) and (snail(X15) or caterpillar(X15)))  =>  smaller(X15,Y4))))
 and forall(X16,forall(Y5,((bird(X16) and fox(Y5))  =>  smaller(X16,Y5))))
 and forall(X17,forall(Y6,((fox(X17)  and wolf(Y6))  =>  smaller(X17,Y6))))
 and forall(X18,forall(Y7,((wolf(X18) and (fox(Y7) or isagrain(Y7)))  =>   not  likestoeat(X18,Y7))))
 and forall(X19,forall(Y8,((bird(X19) and caterpillar(Y8))  =>  likestoeat(X19,Y8))))
 and forall(X20,forall(Y9,((bird(X20) and snail(Y9))  =>   not  likestoeat(X20,Y9))))
 and forall(X21,((caterpillar(X21) or snail(X21))  =>  exists(Y10,(isaplant(Y10) and likestoeat(X21,Y10)))))
)).



% The following are some of the problems from Overbeek's CADEnon 11 competition.

fml(overbeek1,19,
    (( not  p(a,b,c)) and
    p(b,a,c) and
    forall(X,p(X,X,e)) and
    forall(X,forall(Y,p(X,Y,f(X,Y)))) and
    forall(X,p(X,g(X),e)) and
    forall(X,p(g(X),X,e)) and
    forall(X,p(X,e,X)) and
    forall(X,p(e,X,X)) and
    forall(X,forall(Y,forall(Z,forall(U,forall(V,forall(W,(
         (p(X,Y,U) and p(Y,Z,V) and p(U,Z,W))  =>  p(X,V,W)))))))) and
    forall(X,forall(Y,forall(Z,forall(U,forall(V,forall(W,(
         (p(X,Y,U) and p(Y,Z,V) and p(X,V,W))  =>  p(U,Z,W)))))))) and
    forall(X,forall(Y,forall(U,forall(V,
         (p(X,Y,U) and p(X,Y,V))  =>  eq(U,V))))) and
    forall(X,eq(X,X)) and
    forall(X,forall(Y,(eq(X,Y)  =>  eq(Y,X)))) and
    forall(X,forall(Y,forall(Z,((eq(X,Y) and eq(Y,Z))  =>  eq(X,Z))))) and
    forall(X,forall(Y,forall(U,forall(V,((eq(U,V) and p(U,X,Y))  =>  p(V,X,Y)))))) and
    forall(X,forall(Y,forall(U,forall(V,((eq(U,V) and p(X,U,Y))  =>  p(X,V,Y)))))) and
    forall(X,forall(Y,forall(U,forall(V,((eq(U,V) and p(X,Y,U))  =>  p(X,Y,V)))))) and
    forall(X,forall(U,forall(V,(eq(U,V)  =>  eq(f(X,U),f(X,V)))))) and
    forall(X,forall(U,forall(V,(eq(U,V)  =>  eq(f(U,X),f(V,X)))))) and
    forall(U,forall(V,(eq(U,V)  =>  eq(g(U),g(V))))))
).

fml(overbeek2,10,(
    p(a,b,c) and
    p(c,g(a),d) and
    p(d,g(b),h) and
    p(h,b,j) and
    p(j,g(h),k) and
    not  p(k,g(b),e) and
    forall(X,forall(Y,(p(X,X,Y)  =>  p(X,Y,e)))) and
    forall(X,forall(Y,(p(X,X,Y)  =>  p(Y,X,e)))) and
    forall(X,forall(Y,p(X,Y,f(X,Y)))) and
    forall(X,p(X,g(X),e)) and
    forall(X,p(g(X),X,e)) and
    forall(X,p(X,e,X)) and
    forall(X,p(e,X,X)) and
    forall(X,forall(Y,forall(Z,forall(U,forall(V,forall(W,(
         (p(X,Y,U) and p(Y,Z,V) and p(U,Z,W))  =>  p(X,V,W)))))))) and
    forall(X,forall(Y,forall(Z,forall(U,forall(V,forall(W,(
         (p(X,Y,U) and p(Y,Z,V) and p(X,V,W))  =>  p(U,Z,W)))))))) and
    forall(X,forall(Y,forall(U,forall(V,
         (p(X,Y,U) and p(X,Y,V))  =>  eq(U,V))))) and
    forall(X,eq(X,X)) and
    forall(X,forall(Y,(eq(X,Y)  =>  eq(Y,X)))) and
    forall(X,forall(Y,forall(Z,((eq(X,Y) and eq(Y,Z))  =>  eq(X,Z))))) and
    forall(X,forall(Y,forall(U,forall(V,((eq(U,V) and p(U,X,Y))  =>  p(V,X,Y)))))) and
    forall(X,forall(Y,forall(U,forall(V,((eq(U,V) and p(X,U,Y))  =>  p(X,V,Y)))))) and
    forall(X,forall(Y,forall(U,forall(V,((eq(U,V) and p(X,Y,U))  =>  p(X,Y,V)))))) and
    forall(X,forall(U,forall(V,(eq(U,V)  =>  eq(f(X,U),f(X,V)))))) and
    forall(X,forall(U,forall(V,(eq(U,V)  =>  eq(f(U,X),f(V,X)))))) and
    forall(U,forall(V,(eq(U,V)  =>  eq(g(U),g(V))))))
).

fml(overbeek4,14,(
    forall(X,forall(Y,forall(Z,p(e(X,e(e(Y,e(Z,X)),e(Z,Y))))))) and
     not  p(e(e(e(a,e(b,c)),c),e(b,a))) and
    forall(X,forall(Y,((p(X) and p(e(X,Y)))  =>  p(Y))))
)).




member_q(X, PROVING):-member(X,PROVING) .
member_q((X,_), PROVING):-member(X,PROVING).
member_q((_,Y), PROVING):-member(X,PROVING).


%l_solve(G):-call_with_depth_limit(l_solve(G,[]),2,X),not(X=depth_limit_exceeded),fail.
%l_solve(G):-call_with_depth_limit(l_solve(G,[]),5,X),not(X=depth_limit_exceeded),fail.
%l_solve(G):-call_with_depth_limit(l_solve(G,[]),10,X),not(X=depth_limit_exceeded),fail.
%l_solve(G):-call_with_depth_limit(l_solve(G,[]),13,X),not(X=depth_limit_exceeded),fail.
%l_solve(G):-call_with_depth_limit(l_solve(G,[]),20,X),not(X=depth_limit_exceeded),fail.

l_solve(G):-l_solve(G,[],100).
%l_solve(G):-call_with_depth_limit(l_solve(G,[]),20,X).


%l_solve(X, PROVING,Depth) :-Depth < 1 ,!,fail. %write_ln(loop(X)),!,fail.

%l_solve(X, PROVING,Depth) :-gen_occurs(X,Proving),!,fail. %write_ln(loop(X)),!,fail.
l_solve(X, PROVING,Depth) :-
     member_q(X, PROVING), %write_ln(loop(X)),
    !,
    fail.

l_solve(X  =>  Y, PROVING) :-
    !,
    assert(suppose(X)),
    l_solve(Y, [Y | PROVING]),
    retractall(suppose(X)).

l_solve(X, _PROVING,Depth) :-
    fact(X).

l_solve( (A , B), PROVING,Depth) :-
   Depth2 is Depth - 1,
   Depth3 is Depth - 1,
    l_solve(A, PROVING,Depth2),
    l_solve(B, PROVING,Depth3).

l_solve(X, PROVING,Depth) :-
   Depth2 is Depth - 1  ,
    rule(G  =>  X),
    l_solve(G, [X | PROVING],Depth2).


rule(G  =>  X) :- rule(X, Depth, G,Context,TN, CLID).
%fact(G  =>  X) :- rule(X, Depth, G,Context,TN, CLID).
fact(X) :- fact(X,Context,TN, CLID).




