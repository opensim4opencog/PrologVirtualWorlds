% generated: 17 November 1989
% option(s): SOURCE_TRANSFORM_1
%
%   tak
%
%   Evan Tick (from Lisp version by R. P. Gabriel)
%
%   (almost) Takeuchi function (recursive arithmetic)



go :-
	measure(tak100,true,Utime,Gtime,System),
        nl, write(bench(tak,100,Utime,Gtime,System)),write('.'),nl.

tak100 :- tak10, fail.
tak100 :- tak10, fail.
tak100 :- tak10, fail.
tak100 :- tak10, fail.
tak100 :- tak10, fail.
tak100 :- tak10, fail.
tak100 :- tak10, fail.
tak100 :- tak10, fail.
tak100 :- tak10, fail.
tak100 :- tak10, fail.
tak100.


tak10 :- tak, fail.
tak10 :- tak, fail.
tak10 :- tak, fail.
tak10 :- tak, fail.
tak10 :- tak, fail.
tak10 :- tak, fail.
tak10 :- tak, fail.
tak10 :- tak, fail.
tak10 :- tak, fail.
tak10 :- tak, fail.
tak10.


tak :- tak(18,12,6,_), ! .

tak(X,Y,Z,A):-
        X =< Y,
        Z = A.

tak(X,Y,Z,A):-
	X > Y,
        X1 is X - 1,
        tak(X1,Y,Z,A1),
        Y1 is Y - 1,
        tak(Y1,Z,X,A2),
        Z1 is Z - 1,
        tak(Z1,X,Y,A3),
        tak(A1,A2,A3,A).


