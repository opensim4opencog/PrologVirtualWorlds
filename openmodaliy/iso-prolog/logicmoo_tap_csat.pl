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
/*
csp(Doms,Relns) :-
   ac(Doms,Relns).
*/

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


   % number_unsat_relns(Relns,Asst,N) means N is the number of unsatisfied
% relations of Relns using assignment Asst
number_unsat_relns([],_,0).
number_unsat_relns([rel([X,Y],R)|Rels],Asst,N) :-
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
   random_csp(Doms,Relns,20,Ans),      % 20 is arbitrary setting
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

% gsat(Doms,Relns,N,M,Ass) is true if Ass is an
% assignment of a value for each variable that satisfies
% Relns using GSAT. 
% N is the maxumum number of restarts.
% Each restart does M steps of hill climbing.
gsat(Doms,Relns,_,M,SAss) :-
   random_assign(Doms,Asst),
%   writeln(['Given Doms = ',Doms]),
   writeln(['   Random Assignment: ',Asst]),
   it_improve(Asst,Doms,Relns,M,SAss).
gsat(Doms,Relns,N,M,SAss) :-
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
   domain(Var,Dom,Doms),
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
   

% domain(Var,Dom,Doms) is true if Dom is the domain of variable Var in Doms
domain(Var,Dom,[dom(Var1,Dom)|_]) :- Var==Var1,!.
domain(Var,Dom,[_|Doms]) :-
   domain(Var,Dom,Doms).

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

csp(Doms,Relns) :-
   gsat(Doms,Relns,20,20,Ans),      % 20 20 is arbitrary setting
   set_all(Ans).



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
   length(L,N),
   random(R,N),
   N1 is R+1,
   nth(N1,L,E).

% random_rem(E,L,R) selects a random element E of
% list L where list R is contains the other elements.
random_rem(E,L,R) :-
   length(L,N),
   random(Rand,N),
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




