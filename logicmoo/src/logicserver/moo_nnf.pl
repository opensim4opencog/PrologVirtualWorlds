:-include('moo_header.pl').

/******************************************************************************

Note: this file is used as sanity reference 

none of the predicates are used




Purpose: 1. Compute normal forms for first-order formula.
         2. Skolemize first-order formula.

Version: 0.1

Language: SWI Prolog

Filename: nfs

Date: 1999.7.15

Maintainer: Anthony Aaby

Usage: see nfs_help

Copyright (C) 1999 Anthony A. Aaby

This program is free software; you can redistribute it  and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place   not    Suite 330, Boston, MA  02111-1307, USA.

To reach the author send email to aabyan@wwc.edu
or Anthony Aaby
   Computer Science Department
   Walla Walla College
   College Place, WA 99324

FORMULA SYNTAX
        not A =  not    A
        A/\B  = A  and  B
        A\/B  = A or B
        A=>B  = A  =>   B
        A<=>B = A  <=>   B
     forall(X,A) = forall(X,A)
  exists(X,A) = exists(X,A)
          []F = known F
          <>F = consistent F
          0 F = next F
   until(A,B) = until(A,B)
******************************************************************************/

%:- module(nfs,[nnf/2,cnf/2,dnf/2,pnf/2,cf/2,nfs_help/0,e/0]).

:- op(400,fy,known),  % Necessity, Always
   op(400,fy,consistent),  % Possibly, Eventually
   op(400,fy,next),  % Next time
   op(400,fy,  not   ),    % negation
   op(500,xfy, and ),   % conjunction
   op(600,xfy,or),   % disjunction
   op(650,xfy, =>  ),  % implication
   op(700,xfy, <=>  ). % equivalence

%%% Negation Normal Form

% Usage: nnf(+Fml, ?NNF)

nnf(Fml,NNF) :- nnf(Fml,[],NNF,_).

%  not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not     not   
%  nnf(+Fml,+FreeV,-NNF,-Paths)
%
% Fml,NNF:    See above.
% FreeV:      List of free variables in Fml.
% Paths:      Number of disjunctive paths in Fml.

nnf(known F,FreeV,BOX,Paths) :- !,
	nnf(F,FreeV,NNF,Paths), cnf(NNF,CNF), boxRule(known CNF, BOX).

nnf(consistent F,FreeV,DIA,Paths) :- !,
	nnf(F,FreeV,NNF,Paths), dnf(NNF,DNF), diaRule(consistent DNF, DIA).

nnf(next F,FreeV,CIR,Paths) :- !,
	nnf(F,FreeV,NNF,Paths), cirRule(next NNF, CIR).

nnf(until(A,B),FreeV,NNF,Paths) :- !,
	nnf(A,FreeV,NNF1,Paths1),
	nnf(B,FreeV,NNF2,Paths2),
	Paths is Paths1 + Paths2,
	NNF = until(NNF1, NNF2).

nnf(forall(X,F),FreeV,forall(X,NNF),Paths) :- !,
	nnf(F,[X|FreeV],NNF,Paths).

nnf(exists(X,Fml),FreeV,NNF,Paths) :- !,
	skolemize(Fml,X,FreeV,FmlSk),
	nnf(FmlSk,FreeV,NNF,Paths).

nnf(A  and  B,FreeV,NNF,Paths) :- !,
	nnf(A,FreeV,NNF1,Paths1),
	nnf(B,FreeV,NNF2,Paths2),
	Paths is Paths1 * Paths2,
	(Paths1 > Paths2  -> NNF = (NNF2  and  NNF1);
		            NNF = (NNF1  and  NNF2)).

nnf(A or B,FreeV,NNF,Paths) :- !,
	nnf(A,FreeV,NNF1,Paths1),
	nnf(B,FreeV,NNF2,Paths2),
	Paths is Paths1 + Paths2,
	(Paths1 > Paths2  -> NNF = (NNF2 or NNF1);
		            NNF = (NNF1 or NNF2)).

nnf(Fml,FreeV,NNF,Paths) :- 
	(Fml =  not   (  not   A)      -> Fml1 = A;
	 Fml =  not   (known F)   -> Fml1 = consistent (  not   F);
	 Fml =  not   (consistent F)   -> Fml1 = known (  not   F);
	 Fml =  not   (next F)   -> Fml1 = next (  not   F);
	 Fml =  not  until(A,B) -> (nnf(  not   A,FreeV,NNA,_), nnf(  not   B,FreeV,NNB,_),
                             Fml1 = forall(NNB)or until(NNB,NNA  and  NNB));
	 Fml =  not  forall(X,F)  -> Fml1 = exists(X,-F);
	 Fml =  not  exists(X,F)   -> Fml1 = forall(X,-F);
	 Fml =  not   (A or B)   -> Fml1 =  not  A  and   not  B;
	 Fml =  not   (A  and  B)   -> Fml1 =  not  A or  not  B;
	 Fml = (A  =>   B)   -> Fml1 =  not  A or B;
	 Fml =  not   (A  =>   B)  -> Fml1 = A  and   not  B;
	 Fml = (A  <=>   B)  -> Fml1 = (A  and  B) or (  not   A  and   not  B);
	 Fml =  not   (A  <=>   B) -> Fml1 = (A  and   not  B) or (  not   A  and  B)),!,
	nnf(Fml1,FreeV,NNF,Paths).

nnf(Lit,_,Lit,1).

boxRule(known (A and B), (BA)  and  (BB)) :- !, boxRule(known A,BA), boxRule(known B,BB).
boxRule(BOX, BOX).
diaRule(consistent (A or B), (DA) or (DB)) :- !, diaRule(consistent A,DA), diaRule(consistent B,DB).
diaRule(DIA, DIA).
cirRule(next (A or B), (DA) or (DB)) :- !, cirRule(next A,DA), cirRule(next B,DB).
cirRule(next (A  and  B), (DA)  and  (DB)) :- !, cirRule(next A,DA), cirRule(next B,DB).
cirRule(CIR, CIR).

%%%  Conjunctive Normal Form (CNF)  not     not    assumes Fml in NNF

% Usage: cnf( +NNF, ?CNF )

cnf(P  and  Q, P1  and  Q1):- !, cnf(P, P1), cnf(Q, Q1).
cnf(P or Q,     CNF):- !, cnf(P, P1), cnf(Q, Q1), cnf1(P1 or Q1, CNF).
cnf(CNF,       CNF).

cnf1((P  and  Q) or R, P1  and  Q1):- !, cnf1(P or R, P1), cnf1(Q or R, Q1).
cnf1(P or (Q  and  R), P1  and  Q1):- !, cnf1(P or Q, P1), cnf1(P or R, Q1).
cnf1(CNF,             CNF).

%%%  Disjunctive Normal Form (DNF)  not     not    assumes Fml in NNF

% Usage: dnf( +NNF, ?DNF )

dnf(P or Q, P1 or Q1):- !, dnf(P, P1), dnf(Q, Q1).
dnf(P  and  Q,     DNF):- !, dnf(P, P1), dnf(Q, Q1), dnf1(P1  and  Q1, DNF).
dnf(DNF,       DNF).

dnf1(P  and  (Q or R), P1 or Q1):- !, dnf1(P  and  Q, P1), dnf1(P  and  R, Q1).
dnf1((P or Q)  and  R, P1 or Q1):- !, dnf1(P  and  R, P1), dnf1(Q  and  R, Q1).
dnf1(DNF,             DNF).

%%%  Prenex Normal Form (PNF)

% Usage: pnf( +Fml, ?PNF )  not     not    assumes Fml in NNF

pnf(F,PNF) :- pnf(F,[],PNF).

% pnf(+Fml, +Vars, ?PNF)

pnf(     forall(X,F),Vs, forall(X,PNF)) :- !, pnf(F,[X|Vs], PNF).
pnf(      exists(X,F),Vs,  exists(X,PNF)) :- !, pnf(F,[X|Vs], PNF).

pnf(  exists(X,A)  and  B,Vs,  exists(Y,PNF)) :- !, copy_term((X,A,Vs),(Y,Ay,Vs)),
                                        pnf(Ay  and  B,[Y|Vs], PNF).
pnf(  exists(X,A) or B,Vs,  exists(Y,PNF)) :- !, copy_term((X,A,Vs),(Y,Ay,Vs)),
                                        pnf(Ay or B,[Y|Vs], PNF).
pnf( forall(X,A)  and  B,Vs, forall(Y,PNF)) :- !, copy_term((X,A,Vs),(Y,Ay,Vs)),
                                        pnf(Ay  and  B,[Y|Vs], PNF).
pnf( forall(X,A) or B,Vs, forall(Y,PNF)) :- !, copy_term((X,A,Vs),(Y,Ay,Vs)),
                                        pnf(Ay or B,[Y|Vs], PNF).

pnf( A  and   exists(X,B),Vs,  exists(Y,PNF)) :- !, copy_term((X,B,Vs),(Y,By,Vs)),
                                        pnf(A  and  By,[Y|Vs], PNF).
pnf( A or  exists(X,B),Vs,  exists(Y,PNF)) :- !, copy_term((X,B,Vs),(Y,By,Vs)),
                                        pnf(A or By,[Y|Vs], PNF).
pnf( A  and  forall(X,B),Vs, forall(Y,PNF)) :- !, copy_term((X,B,Vs),(Y,By,Vs)),
                                        pnf(A  and  By,[Y|Vs], PNF).
pnf( A or forall(X,B),Vs, forall(Y,PNF)) :- !, copy_term((X,B,Vs),(Y,By,Vs)),
                                        pnf(A or By,[Y|Vs], PNF).

pnf(        A  and  B,Vs,       PNF ) :- pnf(A,Vs,Ap), pnf(B,Vs,Bp), 
                                     (A\=Ap; B\=Bp), pnf(Ap  and  Bp,Vs,PNF).
pnf(        A or B,Vs,       PNF ) :- pnf(A,Vs,Ap), pnf(B,Vs,Bp), 
                                     (A\=Ap; B\=Bp), pnf(Ap or Bp,Vs,PNF).

pnf(          PNF, _,       PNF ).

%%%  Clausal Form (CF)  not     not    assumes Fml in PNF  and
%                                 each quantified variable is unique

% cf(+Fml, ?Cs)
% Cs is a list of the form: [cl(Head,Body), ...]
% Head  and Body are lists.

cf(PNF, Cla):- removeQ(PNF,[], UnQ), cnf(UnQ,CNF), clausify(CNF,Cla,[]).

% removes quantifiers
removeQ( forall(X,F),Vars, RQ) :- removeQ(F,[X|Vars], RQ).
removeQ(  exists(X,F),Vars, RQ) :- skolemize(F,X,Vars,Fsk),
                               removeQ(Fsk,Vars, RQ).
removeQ( F,_,F ).

clausify( (P  and  Q), C1, C2 ) :- !, clausify( P, C1, C3 ), clausify( Q, C3, C2 ).
clausify( P, [cl(A,B)|Cs], Cs ) :- inclause( P, A, [], B, [] ), !.
clausify( _, C, C ).

inclause( (P or Q), A, A1, B, B1 ) :- !, inclause( P, A2, A1, B2, B1 ),
                                        inclause( Q, A,  A2, B,  B2 ).

inclause(  not  P, A,  A, B1, B ) :- !, notin( P, A ), putin( P, B, B1 ).
inclause( P,  A1, A, B,  B ) :- !, notin( P, B ), putin( P, A, A1 ).

notin(X,[Y|_]) :- X==Y, !, fail.
notin(X,[_|Y]) :- !,notin(X,Y).
notin(_,[]).

putin(X,[],   [X]   ) :- !.
putin(X,[Y|L],[Y|L] ) :- X == Y,!.
putin(X,[Y|L],[Y|L1]) :- putin(X,L,L1).

%%%  Skolemizing   not     not    Two methods

% Usage: skolemize(+Fml,+X,+FreeV,?FmlSk)
% Replaces existentially quantified variable with the formula
% VARIABLES MUST BE PROLOG VARIABLES
% exists(X,p(X))  not    -> p(p(exists))

skolemize(Fml,X,FreeV,FmlSk):-
	copy_term((X,Fml,FreeV),(Fml,Fml1,FreeV)),
	copy_term((X,Fml1,FreeV),(exists,FmlSk,FreeV)).

% Usage: skolem( +Fml, +X, +FreeV, ?FmlSk )
% Replaces existentially quantified variable with a unique function
% fN(Vars) N=1,...
% VARIABLES MAYBE EITHER PROLOG VARIABLES OR TERMS

skolem( F, X, FreeV, FmlSk) :- gensym( f, Fun ), Sk =..[Fun|FreeV],
                                subst( F, X, Sk, FmlSk ).

%%% Substitution

/*
% Usage: subst(+Fml,+X,+Sk,?FmlSk)

subst( forall(Y,P), X,Sk, forall(Y,P1) ) :- !, subst( P,X,Sk,P1 ).
subst(  exists(Y,P), X,Sk,  exists(Y,P1) ) :- !, subst( P,X,Sk,P1 ).
subst(    P  and  Q, X,Sk,   P1  and  Q1 ) :- !, subst( P,X,Sk,P1 ),
                                         subst( Q,X,Sk,Q1 ).
subst(    P or Q, X,Sk,   P1 or Q1 ) :- !, subst( P,X,Sk,P1 ),
                                         subst( Q,X,Sk,Q1 ).
subst(        P, X,Sk,        P1 ) :- functor(P,_,N),
                                      subst1( X, Sk, P, N, P1 ).

subst1( _,  _, P, 0, P  ).
subst1( X, Sk, P, N, P1 ) :- N > 0, P =..[F|Args], subst2( X, Sk, Args, ArgS ),
                             P1 =..[F|ArgS].

subst2( _,  _, [], [] ).
subst2( X, Sk, [A|As], [Sk|AS] ) :- X == A, !, subst2( X, Sk, As, AS).
subst2( X, Sk, [A|As], [A|AS]  ) :- var(A), !, subst2( X, Sk, As, AS).
subst2( X, Sk, [A|As], [Ap|AS] ) :- subst( A,X,Sk,Ap ),
                                    subst2( X, Sk, As, AS).
                                    
*/                                    

/********************************  The End ***********************************/



