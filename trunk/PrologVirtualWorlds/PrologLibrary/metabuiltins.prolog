:-dynamic( ax_worldwalker /1).

write_debug(What):-write_debug(unknown,What).
 
write_debug(A,B):-ax_worldwalker(X),once((!,catch2((
        sformat(This,'debug ~w:~w',[A,B]),actx_invoke_object(X,[say],[This],[])
        )))).

 
write_debug(A,B):-!,catch(write((A:B)),_,true),nl.

%write_debug(A,B):-catch(post((A:B)),_,true).

/*
write_debug(make_scripts,Scripted:Proposed_Method) :- !, write_debug1(Scripted), write(Proposed_Method), write('.'), nl.
write_debug(make_scripts,Scripted) :- !,write(Scripted), write('.'), nl.
write_debug(Who,What):-ground(What),writeq(Who=What),nl,ttyflush.
write_debug(_,_).
*/

write_debug1(Scripted:Proposed_Method) :- !, write_debug1(Scripted), write(Proposed_Method), write(';'), nl.
write_debug1(Scripted) :- write(Scripted), write(';'), nl.

%//------------------------------------------------------------------------------

/****************************************************************************/
% replace(+Element1,+List1,+Element2,-List2)
%    replaces all instances of Element1 in List1 with Element2 and returns
%       the new list as List2
%    does not replace variables in List1 with Element1

replace(_,[],_,[]).
replace(X,[H|T],A,[A|T2]) :- nonvar(H), H = X, !, replace(X,T,A,T2).
replace(X,[H|T],A,[H|T2]) :- replace(X,T,A,T2).

%//------------------------------------------------------------------------------

replaceall(_,_,[],[]):-!.
replaceall(X,INS,[X|TXT],OUT):-!,append([13,10|INS],TXT,OUT).
replaceall(X,INS,[A|TXT],[A|OUT]):-!,replaceall(X,INS,TXT,OUT).

%//------------------------------------------------------------------------------

aformat(Atom,Format,List):-
	sformat(String,Format,List),
	string_to_atom(String,Atom).

%//------------------------------------------------------------------------------

assertn(X):-catch((X),_,fail),!.
assertn(X):-catch(asserta((X)),_,true),!.
assertn(_X):-!.

%//------------------------------------------------------------------------------

try(Process):-ignore(catch(Process)).

%//------------------------------------------------------------------------------

try2(Process):-catch(Process,_,true).

%//------------------------------------------------------------------------------

catch(Process):-catch(Process,_Error,(write_debug(Process,_Error),fail)).

%//------------------------------------------------------------------------------

catch2(Process):-catch(Process,_,fail).

%//------------------------------------------------------------------------------

post(X):-term_to_atom(X,R),assertz(result(R)).

:-post(just_loaded).

%//------------------------------------------------------------------------------

real_atom([]):-!,fail.
real_atom(X):-atomic(X).

%//------------------------------------------------------------------------------

list_term_to_atom([],[]):-!.
list_term_to_atom([TH|TL],[AH|AL]):-!,term_to_atom(TH,AH),list_term_to_atom(TL,AL).

                              




