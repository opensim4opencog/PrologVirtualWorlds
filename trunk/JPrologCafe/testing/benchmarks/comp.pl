go :-
  measure(runcomp(50000),true,Utime,Gtime,System),
  nl, write(bench(comp,20,Utime,Gtime,System)),write('.'),nl.

runcomp(0) :- !.
runcomp(N) :-
  N1 is N - 1,
  doComp,
  runcomp(N1).

doComp :- 
  test(t1 > t2).

test(A) :-
  run(A, B), run2(A, B).
  

run((A :- B), (C :- D)) :-
  run(A, C),
  run(B, D).
run(In, Out) :-
  not(doperator_goal(In, _)),
  dchangeIllegalTerms(In, Out).
run(In, Out) :-
  doperator_goal(In, Tmp),
  dchangeIllegalTerms(Tmp, Out).

run2((A :- B), (C :- D)) :-
  run2(A, C),
  run2(B, D).
run2(Out, In) :-
  dchangeIllegalTerms(Out, In),
  not(doperator_goal(_, In)).
run2(Out, In) :-
  dchangeIllegalTerms(Tmp, In),
  doperator_goal(Out, Tmp).


dchangeIllegalTerms(A, A) :- var(A), !.
dchangeIllegalTerms(varnr, tvarnr____term) :- !.
dchangeIllegalTerms(cut, tcut____term) :- !.
dchangeIllegalTerms(continuation, tcontinuation____term) :- !.
dchangeIllegalTerms(A, Z) :-
   not(var(A)),
   name(t, [T]),
   name(A, [T|R]),
   name(Z, [T,T|R]), !.
dchangeIllegalTerms(A, Z) :-
   not(var(Z)),
   name(Z, [T,T|R]),
   name(t, [T]),
   name(A, [T|R]),
   !.
dchangeIllegalTerms(A, A).

doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < (Y, Z, R, T, G, A, X) , smallerthan(X,Y)).
doperator_goal(X < Y , smallerthan(X,Y)).
doperator_goal(X < Y , smallerthan(X,Y)).
doperator_goal(X < Y , smallerthan(X,Y)).
doperator_goal(X < Y , smallerthan(X,Y)).
doperator_goal(X < Y , smallerthan(X,Y)).
doperator_goal(X < Y , smallerthan(X,Y)).
doperator_goal(X < Y , smallerthan(X,Y)).
doperator_goal(X < Y , smallerthan(X,Y)).
doperator_goal(X < Y , smallerthan(X,Y)).
doperator_goal(X < Y , smallerthan(X,Y)).
doperator_goal(T > F, a(T, F)) :- not(doperator_goal(F, _)), not(doperator_goal(T, _)).
