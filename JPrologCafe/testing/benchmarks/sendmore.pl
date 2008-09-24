
% Cryptoaddition:
% Find the unique answer to:
%  SEND
% +MORE
% -----
% MONEY
% where each letter is a distinct digit.

go :- go(10).

go(N) :-
	 measure(send(N), dummysend(N), Utime,Gtime,System),
	 nl, write(bench(send,N,Utime,Gtime,System)),write('.'),nl.

send(_) :- send, fail.
send(N) :-
	N > 0, !, write(N), nl,
	    M is N - 1, send(M).
send(N).

dummysend(_) :- dummysend, fail.
dummysend(N) :-
	N > 0, !,
        M is N - 1, dummysend(M).
dummysend(N).

dummysend.

send :-
	digit(D), digit(E), not(D==E),
	sumdigit(0, D, E, Y, C1),
	digit(N), not(N==Y), not(N==E), not(N==D),
	digit(R), not(R==N), not(R==Y), not(R==E), not(R==D),
	sumdigit(C1,N, R, E, C2),
	digit(O), not(O==R), not(O==N), not(O==Y), not(O==E), not(O==D),
	sumdigit(C2,E, O, N, C3),
	leftdigit(S), not(S==O), not(S==R), not(S==N), not(S==Y), not(S==E), not(S==D),
	leftdigit(M), not(M==S), not(M==O), not(M==R), not(M==N), not(M==Y), not(M==E), not(M==D),
	sumdigit(C3,S, M, O, M),
	fail.
send.

sumHelper(X, X, 0) :-
  X < 10.
sumHelper(X, S, 1) :-
  S is X - 10.

sumdigit(C, A, B, S, D) :-
	X is C + A + B,
	sumHelper(X, S, D).

digit(0).
digit(1).
digit(2).
digit(3).
digit(4).
digit(5).
digit(6).
digit(7).
digit(8).
digit(9).

leftdigit(1).
leftdigit(2).
leftdigit(3).
leftdigit(4).
leftdigit(5).
leftdigit(6).
leftdigit(7).
leftdigit(8).
leftdigit(9).
