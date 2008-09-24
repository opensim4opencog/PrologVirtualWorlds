% Where does the zebra live?
% Puzzle solution written by Claude Sammut.

go :- go(200).

go(N) :-
	measure(zebra(N), dummyzebra(N), Utime,Gtime,System),
        nl, write(bench(zebra,N,Utime,Gtime,System)),write('.'),nl.

zebra(_) :- zebra1, fail.
zebra(0) :- !.
zebra(N) :-
    M is N - 1, zebra(M).

dummyzebra(_) :- dummyzebra, fail.
dummyzebra(0) :- !.
dummyzebra(N) :-
	    M is N - 1, dummyzebra(M).

dummyzebra.

zebra1 :- zebra, ! .

zebra :-
	houses(Houses),
	mymember(house(red, english, _, _, _), Houses),
	mymember(house(_, spanish, dog, _, _), Houses),
	mymember(house(green, _, _, coffee, _), Houses),
	mymember(house(_, ukrainian, _, tea, _), Houses),
	right_of(house(green,_,_,_,_), house(ivory,_,_,_,_), Houses),
	mymember(house(_, _, snails, _, winstons), Houses),
	mymember(house(yellow, _, _, _, kools), Houses),
	Houses = [_, _, house(_, _, _, milk, _), _,_],
	Houses = [house(_, norwegian, _, _, _)|_],
	next_to(house(_,_,_,_,chesterfields), house(_,_,fox,_,_), Houses),
	next_to(house(_,_,_,_,kools), house(_,_,horse,_,_), Houses),
	mymember(house(_, _, _, orange_juice, lucky_strikes), Houses),
	mymember(house(_, japanese, _, _, parliaments), Houses),
	next_to(house(_,norwegian,_,_,_), house(blue,_,_,_,_), Houses),
	mymember(house(_, _, zebra, _, _), Houses),
	mymember(house(_, _, _, water, _), Houses),
	true. % print_houses(Houses).

houses([
	house(_, _, _, _, _),
	house(_, _, _, _, _),
	house(_, _, _, _, _),
	house(_, _, _, _, _),
	house(_, _, _, _, _)
]).

right_of(A, B, [B, A | _]).
right_of(A, B, [_ | Y]) :- right_of(A, B, Y).

next_to(A, B, [A, B | _]).
next_to(A, B, [B, A | _]).
next_to(A, B, [_ | Y]) :- next_to(A, B, Y).

mymember(X, [X|_]).
mymember(X, [_|Y]) :- mymember(X, Y).

print_houses([A|B]) :- !,
	write(A), nl,
	print_houses(B).
print_houses([]).




