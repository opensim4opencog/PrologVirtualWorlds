:-dynamic(known_property_value(Property,Value)). % like known_property_value(size,big)
:-dynamic(known_proper_noun(ProperNoun,LogicalThing)). %like known_proper_noun('Superman','TheManOfSteel')
:-dynamic(known_noun(Word,PlurSing)). %like known_noun(apple,snglr)
:-dynamic(known_verb_to_predicate(Verb,Tense,SingPlur,LogicalPredicate)). %like known_verb_to_predicate(kissed,past,snglr,kiss)
:-dynamic(known_request_word(WhWhat,LogicalWhWhat)). %known_request_word(how,methodUsed)
:-dynamic(known_copula(IS_BE,PlurSing)).
:-dynamic(known_class(EnglishCol,Tense,LogicalClass)).
:-dynamic(known_determiner(Tense,LogicalQuant,Quantifier)).
:-dynamic(known_verb_frame(Verb,Tense,SingPlur,Frame_with_Subj_and_Obj,Subj,Obj)).

%:-module(moo_english,[]).

% :-include('moo_header.pl').


%:-use_module(library(lists)).
lfl(A):-any_to_words(A,X),phrase(sentence(P), X),writeq(P),nl.



/*

%---------------------------------------------------------------
% Test data
%---------------------------------------------------------------
expr(Z) --> term(X), "+", expr(Y), {Z is X + Y}.
expr(Z) --> term(X), "-", expr(Y), {Z is X - Y}.
expr(X) --> term(X).

term(Z) --> number(X), "*", term(Y), {Z is X * Y}.
term(Z) --> number(X), "/", term(Y), {Z is X / Y}.
term(Z) --> number(Z).

number(C) --> "+", number(C).
number(C) --> "-", number(X), {C is -X}.
number(X) --> [C], {"0"=<C, C=<"9", X is C - "0"}.


imp --> [vp].
decl --> [np, vp].
decl --> [np, vp, pp].
decl --> [pp, np, vp].
decl --> [set].


np --> [set].
np --> [set, pp].

set --> [object].
set --> [object, object].
set --> [object, object, object].
set --> [object, object, object, object].

object --> [pn].
object --> [spec, noun ].
object --> [noun].

spec --> [det].
spec --> [adj].
spec --> [adj_sat].
spec --> [num].
spec --> [spec, spec].

vp  --> [iv].
vp  --> [tv, np].
vp  --> [tv, np, pp].
vp  --> [dv, np, pp].
vp  --> [adv, vp].
pp  --> [p, np].
*/

%list_to_terms([],true):-!.
/*list_to_terms([T1,T2,T3,T4,T5,T6],((T1,T2,T3,T4,T5,T6)) ).
list_to_terms([T1,T2,T3,T4,T5], ((T1,T2,T3,T4,T5)) ).
list_to_terms([T1,T2,T3,T4], ((T1,T2,T3,T4)) ).
list_to_terms([T1,T2,T3], ((T1,T2,T3)) ).

list_to_terms([T1,T2|T3L],T):-nonvar(T),T=( (T1,T2),T3 ),!,list_to_terms(T3L,T3).
list_to_terms([T1|T2T3L],T):-nonvar(T),T=( T1,(T2,T3) ),!,list_to_terms(T2T3L,(T2,T3)).
list_to_terms([T1],T1):-!.
list_to_terms([T1|L],(T1,TL) ):-list_to_terms(L,TL).
 */

list_to_terms(List,(A,B)):-list_to_terms(A,AL),list_to_terms(B,BL),append(AL,BL,List).
list_to_terms(List,(A;B)):-list_to_terms(A,AL),list_to_terms(B,BL),append(AL,[or|BL],List).

:-multifile(s/3).
:-multifile(vp/4).
:-multifile(vg/5).
:-multifile(pp/3).

sent --> s(gap(0)).
sent --> np(gap(0)), s(gap(np)).
s(Gap) --> np(gap(0)), vp(Gap).
np(gap(0)) --> pn.
np(gap(0)) --> det, noun.
np(Gap) --> det, noun, pp(Gap).
np(gap(np)) --> [].
pp(Gap) --> p, np(Gap).
vp(gap(0)) --> iv.
vp(Gap) --> tv, np(Gap).
vp(Gap) --> datv, np(Gap), pp(gap(0)).
vp(Gap) --> datv, np(gap(0)), pp(Gap).

det --> [Det], {lex(Det,det)}.
p --> [P], {lex(P,p)}.
noun --> [N], {lex(N,noun)}.
verb --> [V], {lex(V,v)}.
pn --> [PN], {lex(PN,pn)}.
tv --> [TV], {lex(TV,tv)}.
datv --> [DV], {lex(DV,dv)}.
iv --> [IV],{lex(IV,iv)}.

wrap(A):-any_to_words(A,X),phrase(sentence(P),X),loglang(P).


lex(kim,np).
lex(sandy,np).
lex(lee,np).
lex(baby,noun).
lex(man,noun).
lex(woman,noun).
lex(telescope,noun).
lex(park,noun).
lex(a,det).
lex(the,det).
lex(to,p).
lex(with,p).
lex(in,p).
lex(died,iv).
lex(ate,tv).
lex(ate,tv).
lex(saw,tv).
lex(gave,tv).
lex(gave,dv).
lex(handed,dv).
lex(Word,noun):-w_n([Word],_).
lex(Word,adj):-w_a([Word],_).
lex(Word,adj_sat):-w_as([Word],_).
lex(Word,adv):-w_av([Word],_).
lex(Word,VT):-member(VT,[iv,tv,dv]),w_v([Word],_).
% Students visited the town. 

s(at(NP,VP))  -->  np(Num, NP), vp(Num, VP). 

%Noun Phrase 
% this student, a beer, the town 
np(Num, np(Det, N))  -->  detp(Num, Det), ng(Num,_, N). 
% students, beers, towns(plural countable) 
np(pl, np( N))  -->  ng(pl,c, N). 
% linguistics, money, meat (singular non-countable) 
np(sg, np( N))  -->  ng(sg,nc, N). 
% a cold beer 
% the students of Tartu 
np(Num, np(Det, N, PP))  -->  detp(Num, Det), ng(Num,_, N), pp(PP). 
% he, they 
np(Num, np(PersPron))  -->  pers_pron(Num, PersPron). 
% (he saw) her reading 
np(Num, np(Det, N, V))  -->  detp(Num, Det), noun(Num, _, N), verb(_, _, part1, V). 

% Noun Group 
% student, beer, town 
ng(Num,C, ng(N))  -->  noun(Num, C, N). 
% linguistics student, beer town 
ng(Num,C, ng(N1, N2))  -->  noun(_, _, N1), noun(Num, C, N2). 

%Determiner Phrase 
% a, the, this 
detp(Num, detp(Det))  -->  det(Num,Det). 
% my, his 
detp(Num, detp(Pron))  -->  poss_pron(Num,Pron). 
% one, two, four 
detp(Num, detp(NDet)) -->  num_det(Num,NDet). 
% those four 
detp(Num, detp(Det, NDet))  --> det(Num, Det), num_det(Num, NDet). 
% all the 
detg(Num, detg(QDet, Det))  -->  quant_det(QDet), det(Num, Det). 

% Verb Phrase 
% goes, were, is sleeping 
vp(Num, vp(V))  --> vg(Num, _, V). 
% give a book 
vp(Num, vp(V, NP)) --> vg(Num, _, V), np(_, NP). 
% give the book to the teacher 
p(Num, vp(V, NP, PP))  -->  vg(Num, _, V), np(_, NP), pp(PP). 
% give her a book for birthday 
vp(Num, vp(V, NP1, NP2, PP)) -->  vg(Num, _, V), np(_, NP1), np(_, NP2), pp(PP). 
% give her a book for birthday on Monday 
vp(Num, vp(V, NP, PP1, PP2))  --> vg(Num, _, V), np(_, NP), pp(PP1), pp(PP2). 

%Verb Group 
% visits, bought, go 
vg(Num, _, vg(V))  --> verb(Num, _, fin, V). 

% Auxiliary Phrase 
% is visiting, were bought 
vg(Num, _, vg(Auxgr, V)) --> auxgr(Num, Pers, _, _, NextForm, Auxgr), verb( Num, Pers, NextForm, V). 
% will be visited 
auxgr(Num, Pers, Form, Preced, EndForm, Str) --> aux(Num, Pers, Form, EndForm, Preced, Aux), { Str =.. [auxgr, Aux] }. 
% could have been visited 
auxgr(Num, Pers, Form, Preced, EndForm, Str) --> aux(Num, Pers, Form, NextForm, Preced, Aux), auxgr(_, _, NextForm, NextPreced, EndForm, Auxgr), { Preced < NextPreced, Str =.. [auxgr, Aux, Auxgr] }. 

% Predicative Phrase 
% (he) is a student 
vg(Num, _, vg(V, COP_NP)) -->  verb(Num, _, cop, V), cop_np(COP_NP). 
% (he) is ready to go 
vg(Num, _, vg(V, PRP))  -->  verb(Num, _, cop, V), prp(PRP). 
% (he) is student 
cop_np(np(N)) --> noun(sg, _,N). 
% (he) is an interesting teacher 
cop_np(cop_np(NP))  -->  np(_, NP). 
% (beer) is cold 
prp(A)  --> adj(A). 
% (he) is interesting 
prp(A) --> adjv(A). 

% Prepositional Phrase 
% to the mountains 
pp(pp(P, NP))  -->  prep(P), np(_, NP).  

% Adjective Phrase 
% ready to go 
adjv(adjv(A,InfMark,V)) -->  adj(A), inf_mark(InfMark), verb(_, _, base, V). 

adj --> [A], {w_a([A],_);w_as([A],_)}.

% imparitive(P) --> verb_phrase(X, P1).
:-op(100,fx,+).

s(s(NP,VP))-->np(NP,VP).
np(np(Det,Noun))-->det(Det),noun(Noun).
vp(vp(Verb,NP))-->verb(Verb),np(NP).
det(det(each))-->[each].
det(det(a))-->[a].
noun(noun(man))-->[man].
noun(noun(woman))-->[woman].
verb(verb(loves))-->[loves].

sentence(P) --> noun_phrase(X, P1, P), verb_phrase(X, P1).


noun_phrase(X, P2, P) -->
	determiner(X, P2, P1, P),
	adjective(X, P3, P2, P1), 
	noun(X, P4), 
	rel_clause(X, P4, P3).

noun_phrase(X, P1, P) -->
        adjective(X, P2, P1, P), 
	noun(X, P3), 
	rel_clause(X, P3, P2).

noun_phrase(X, P1, P) -->
        determiner(X, P2, P1, P), noun(X, P3), rel_clause(X, P3, P2).
noun_phrase(X, P, P) --> name(X).

verb_phrase(X, P) --> trans_verb(X, Y, P1), noun_phrase(Y, P1, P).
verb_phrase(X, P) --> intrans_verb(X, P).

rel_clause(X, P1, P1&P2) --> [that], verb_phrase(X, P2).
rel_clause(_, P, P) --> [].

adjective(X,P1, P2, ATTRIB:(P1=>P2))--> [ADJ], 
	{att(ATTRIB,ADJ,X)}.

att(attrib(blue,X),blue,X):-!.
att(attrib(ADJ,X),ADJ,X):-w_a([ADJ],_).


determiner(X, P1, P2, all_known(X):(P1=>P2) ) --> [Y], 
	{ member(Y,[all,every])}.

determiner(X, P1, P2, do_exists(X):(P1&P2) ) --> [a].
determiner(X, P1, P2, do_exists(X):(P1&P2) ) --> [an].
determiner(X, P1, P2, do_exists(X):(P1&P2) ) --> [the].

determiner(X, P1, P2, some_of(X):(P1&P2) ) --> [some].
determiner(X, P1, P2, some_of(X):(P1&P2) ) --> [many].
determiner(X, P1, P2, some_of(X):(P1&P2) ) --> [most].
determiner(X, P1, P2, some_of(X):(P1&P2) ) --> [].

noun(X, man(X) ) --> [man].
noun(X, woman(X) ) --> [woman].
noun(X, Test ) --> [Y], {Test =.. [Y,X], w_n([Y],_)}.

name(john) --> [john].

trans_verb(X, Y, loves(X,Y) ) --> [loves].
trans_verb(X, Y, TV) --> [V], {TV=..[V,X,Y],w_v([V],_)}.
intrans_verb(X, lives(X) ) --> [lives].
intrans_verb(X, ITV ) --> [V], {ITV=..[V,X],w_v([V],_)}.



to_surface(CHARS,Formula):-
         sentenceToList(CHARS,S),
         sentence(G,S,[]),!,tologic(G,F), 
         fixExistentials(F,SKIF),!,
         getMooTermFromSurface(SKIF,Formula).


known_verb_to_predicate(LogicalPredicate,present,SingPlur,LogicalPredicate):-nonvar(LogicalPredicate),quick_search('Q'(LogicalPredicate,_,_)).
known_class(LogicalClass,sngular,LogicalClass):-nonvar(LogicalClass),quick_search('instance'(LogicalClass,'Class')).

quick_search(Goal):-!,fail.

/*
quick_search(Goal):-
%         add_explaination_recording_args(Goal,ExplainationIn,ExplainationO,Goal2),
         add_complete_search_args(Goal,2,DepthOut,Goal3),
         add_explaination_recording_args(Goal3,ExplainationIn,ExplainationO,Goal4),
        !, Goal4,!.
*/

% ==========================================
% BELIEF
% ==========================================

:-dynamic(complete_goal/1).

:-multifile(expireOptimizationsInContext/3).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).


%end_of_file.




