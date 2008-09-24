go :-
	L = [1,2,3,4,5,6,7,8,9,10],
	measure(q10_10(L),true,Utime,Gtime,System),
        nl, write(bench(queens10,10,Utime,Gtime,System)),write('.'),nl.


q10_10(L) :- q(L,_), fail.
q10_10(L) :- q(L,_), fail.
q10_10(L) :- q(L,_), fail.
q10_10(L) :- q(L,_), fail.
q10_10(L) :- q(L,_), fail.
q10_10(L) :- q(L,_), fail.
q10_10(L) :- q(L,_), fail.
q10_10(L) :- q(L,_), fail.
q10_10(L) :- q(L,_), fail.
q10_10(L) :- q(L,_), fail.
q10_10(_).


q(L,C):-
	perm(L,P), 
	pair(L,P,C),
	safe([],C), ! .

perm([],[]).
perm(Xs,[Z|Zs]):-
	sel(Z,Xs,Ys), 
	perm(Ys,Zs).

sel(X,[X|Xs],Xs).    
sel(X,[Y|Ys],[Y|Zs]):-
	sel(X,Ys,Zs).

pair([],[],[]).
pair([X|Y],[U|V],[p(X,U)|W]):-
	pair(Y,V,W).

safe(_X,[]).
safe(X,[Q|R]):-
	test(X,Q),
	safe([Q|X],R).

test([],_X).
test([R|S],Q):-
	test(S,Q),
	nd(R,Q).

nd(p(C1,R1),p(C2,R2)):-
	C  is C1-C2, 
	R  is R1-R2, not(C==R),
	NR is R2-R1, not(C==NR).