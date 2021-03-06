:- dynamic seed/1.

/*  VAX C-Prolog Benchmark Package  */
/*  Copyright 1985 by Tektronix, Inc., and Portland State University  */

qsort(N) :- 
    generate(0,N,L), !,
    quicksort(L,S).

generate(I,N,[]) :- I = N.
generate(I,N,L) :-
		  J is I+1,
		  random(1001,X),
		  generate(J,N,L1),
		  append([X],L1,L).

append([],L,L).
append([X|L1],L2,[X|L3]) :- append(L1,L2,L3).

seed(1301).

random(R,N) :- retract(seed(S)),
	   N is (S mod R)+1,
	   NewSeed is (125*S+1) mod 4096,
	   asserta(seed(NewSeed)), !.

quicksort([],[]).
quicksort([H|T],S) :- split(H,T,A,B),
	      quicksort(A,A1),
	      quicksort(B,B1),
	      append(A1,[H|B1],S).

split(H,[A|X],[A|Y],Z) :- A > H, split(H,X,Y,Z).
split(H,[A|X],Y,[A|Z]) :- A < H, split(H,X,Y,Z).
split(_,[],[],[]).
	     
