/* CHANGELOG by M.Banbara
  - print_times/4 --> print_times/5
  - report_csv/2 is added.
*/


cuttest(N):-statistics(runtime,[T1|_]),
        cutit11(N), statistics(runtime,[T2|_]),
        compens_loop(N), statistics(runtime,[T3|_]),
        print_times(cuttest(N),T1,T2,T3,N,300).

compens_loop(0).
compens_loop(X) :- Y is X - 1, compens_loop(Y).

/* cutit11(N)    */
cutit11(0).
cutit11(N):- cutt1([100,100,100,100,100,100,100,100,100,100,
                 100,100,100,100,100,100,100,100,100,100,
                 100,100,100,100,100,100,100,100,100,100,
                 100,100,100,100,100,100,100,100,100,100,
                 100,100,100,100,100,100,100,100,100,100,
                 100,100,100,100,100,100,100,100,100,100,
                 100,100,100,100,100,100,100,100,100,100,
                 100,100,100,100,100,100,100,100,100,100,
                 100,100,100,100,100,100,100,100,100,100,
                 100,100,100,100,100,100,100,100,100,100]),
           M is N-1, cutit11(M).

cutt1([]).
cutt1([X|L]):-X=100, !, cutt1(L).
cutt1([X|L]):-X > 100, cutt1(L).

%print_times(T1,T2,T3,X,I) :-        /* prints the results */
%        TT1 is T2 - T1,
%        TT2 is T3 - T2,
%        TT is TT1 - TT2,
%        write('T overall loop:   '),write(TT1), nl,
%        write('T compens loop:   '),write(TT2), nl,
%        write('T net:            '),write(TT),nl,
%        write('KLips:            '),
%        Li is I * X,
%        Lips is Li / TT,
%        KLips is Lips / 1000,
%        write(KLips),nl,nl.
print_times(Name,T1,T2,T3,X,I) :-        /* prints the results */
        TT1 is T2 - T1,
        TT2 is T3 - T2,
        TT is TT1 - TT2,
        write('# Name:             '),write(Name), nl,
        write('# T overall loop:   '),write(TT1), write(' msec.'),nl,
        write('# T compens loop:   '),write(TT2), write(' msec.'),nl,
        write('# T net:            '),write(TT),write(' msec.'),nl,
        write('# KLips:            '),
        Li is I * X,
        Lips is Li / TT,
        KLips is Lips / 1000,
        write(KLips),nl,
	report_csv(['###CSV###',Name,TT1,TT2,TT,KLips], ','),
	nl.

report_csv([], _) :- !.
report_csv([X], _) :- !, write(X), nl.
report_csv([X|Xs], Delim) :- write(X), write(Delim), report_csv(Xs, Delim).
