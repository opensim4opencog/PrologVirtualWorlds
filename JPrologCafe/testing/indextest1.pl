run :-
  buildterm(A, 5000, a), invoke(B, A).

buildterm(_, 0, _) :- fail.
buildterm('Hidden Message', 1, _).
buildterm(A, N, F) :-
  NN is N - 1,
  buildterm(NA, NN, F),
  A =.. [F|[NA]].

invoke(B, A) :- var(A), fail.
invoke(B, b(A)) :- invoke(B, A).
invoke(B, a(A)) :- invoke(B, A).
invoke(A, A) :- atomic(A).
