%%%%%%%%%%% INLINE FILES


/*

  Sicstus Prolog Compatible Code


   this is a cheat for things we dont want to parse or worry about yet

   relax this will be replaced with the learning engine (completed but not in this code (sorry )).

   FYI: cmpd,cmpv will be seen throughout code perposely
        as it will be an extern call to a C module

*/
%%%:- module(personalize, [personalize/2]).



personalize(X,Y):-cmpd(X,Y,[personalize]).

cmpd([],[],_):-!.
cmpd([A|L],[X|R],Set):-cmpv([A],[X],Set),!,cmpd(L,R,Set).
cmpd([A|L],[A|R],Set):-!,cmpd(L,R,Set).

cmpv([X],[Y],[personalize]):-
	member(X=Y,[
	yourself=myself,
	were=was,
	was=were,
	myself=yourself,
	you=me,
	i=you,
	me=you,
	your=my,
	my=your,
	mine=yours,
	yours=mine,
	prolog=me,
	am=are,
	are=am,
	u=me
	]).
















%%%%%%%% I WILL NOT USE THIS BELOW :> :> !!!!!!!
%%%%%%THIS IS CHEATING 

%%%% USED FOR TESTING !!!!!!!!!

/*

  SWI-Prolog Compatible Code

     - Classic Eliza
     - http://prolog.vast1.com

*/



%%%:-module(eliza,[eliza/0,eliza/2,mut/3,rules/2,mem/2]).
:- dynamic(rules/2).
:- dynamic(mem/2).


eliza(X,Y):-mut(eliza,X,Y).


mut(DB,Words,Comment):-
	process_input(DB,Words,[],Input2),
        simplify(Input2,Input3),
        findkeywords(DB,Input3,KeyWords),
        sortkeywords(KeyWords,KeyWords2),
        makecomment(DB,KeyWords2,Input3,Com),
	flatten(Com,Comment),
 	!.

eliza :- rules_db(Rule), assertz(rules(eliza,Rule)), fail.
eliza.


clause(C) :- clause(C,true).
clause(C) :- clause(C,_).

/****************************************************************************/
% findnth(+List,+Number,-Element)
%    returns the Nth member of List in Element

findnth([E|_],1,E).
findnth([_|T],N,T1) :- V is N - 1,
                       findnth(T,V,T1).


/****************************************************************************/
% simplify(+List,-Result)
%   implements non-overlapping simplification
%   simplifies List into Result

simplify(List,Result) :- sr(List,Result,X,Y), !,
			 simplify(X,Y).
simplify([W|Words],[W|NewWords]) :- simplify(Words,NewWords).
simplify([],[]).


/****************************************************************************/
% match_ys(+MatchRule,+InputList)
%    matches the MatchRule with the InputList. If they match, the variables
%    in the MatchRule are instantiated to one of three things:
%       an empty list
%       a single word
%       a list of words

match_ys(A,C) :- match_aux1(A,C),!.
match_ys(A,C) :- match_aux2(A,C).

match_aux1(A,C) :-
	member([*|T],A),
	nonvar(T),
	member(Tm,T),
	nonvar(Tm),
	replace([*|T],A,Tm,B),
	match_aux2(B,C),
	!, last(L,T), L = Tm.

match_aux2([],[]).
match_aux2([Item|Items],[Word|Words]) :-
	match_aux3(Item,Items,Word,Words),!.
match_aux2([Item1,Item2|Items],[Word|Words]) :-
	var(Item1),
	nonvar(Item2),
	Item2 == Word,!,
	match_aux2([Item1,Item2|Items],[[],Word|Words]).
match_aux2([Item1,Item2|Items],[Word|Words]) :-
	var(Item1),
	var(Item2),!,
	match_aux2([Item1,Item2|Items],[[],Word|Words]).
match_aux2([[]],[]).

match_aux3(Word,Items,Word,Words) :-
	match_aux2(Items,Words), !.
match_aux3([Word|Seg],Items,Word,Words0) :-
	append(Seg,Words1,Words0),
	match_aux2(Items,Words1).


/****************************************************************************/
% makecomment(+KeyWordList,+InputList,-Comment)
%    returns ELIZA's Comment to the InputList based on the KeyWordList
%    takes care of special keywords 'your', and 'memory', which require
%       additional processing before a comment can be generated

makecomment(DB,[[your,2]|T],InputList,Comment) :-
	assertz(mem(DB,InputList)),
	clause(rules(DB,[[your,2],Reassembly]),_),
	mc_aux(DB,[[your,2]|T],Reassembly,InputList,Comment),!.
makecomment(DB,[[memory,0]|T],_,Comment) :-
	retract(mem(DB,I2)),
	retractall(mem(DB,I2)),
	clause(rules(DB,[[memory,0],Reassembly]),_),
	mc_aux(DB,[[memory,0]|T],Reassembly,I2,Comment),!.
makecomment(DB,[[memory,0]|T],InputList,Comment) :-
	\+ retract(mem(DB,_)),!,
	makecomment(DB,T,InputList,Comment).
makecomment(DB,[Keyword|T],InputList,Comment) :-
	clause(rules(DB,[Keyword,Reassembly]),_),
	mc_aux(DB,[Keyword|T],Reassembly,InputList,Comment),!.
makecomment(DB,[_|T],InputList,Comment) :-
	makecomment(DB,T,InputList,Comment),!.


mc_aux(DB,KeyWordList,[[DRuleNum,MatchRule,N|T]|_],InputList,Comment) :-
	match_ys(MatchRule,InputList),
	mc_aux2(DB,KeyWordList,DRuleNum,N,T,InputList,Comment),!.
mc_aux(DB,KeyWordList,[_|T],InputList,Comment) :-
	mc_aux(DB,KeyWordList,T,InputList,Comment).
mc_aux(_,_,[],_,_) :- !,fail.

mc_aux2(DB,KeyWordList,DRuleNum,N,T,InputList,Comment) :-
	length(T,TLen),
	N < TLen, !,
	NewN is N + 1,
	findnth(T,NewN,Mn),
	mc_aux3(DB,KeyWordList,DRuleNum,N,NewN,Mn,InputList,Comment).
mc_aux2(DB,KeyWordList,DRuleNum,N,T,InputList,Comment) :-
	member(Mn,T),
	mc_aux3(DB,KeyWordList,DRuleNum,N,0,Mn,InputList,Comment).


mc_aux3(DB,[Keyword|T],DRuleNum,N,NewN,[equal,MnT],InputList,Comment) :-
	!,
	updaterule(DB,Keyword,DRuleNum,N,NewN),
	makecomment(DB,[MnT|T],InputList,Comment).
mc_aux3(DB,[Keyword|T],DRuleNum,N,NewN,[newkey],InputList,Comment) :-
	!,
	updaterule(DB,Keyword,DRuleNum,N,NewN),
	makecomment(DB,T,InputList,Comment).
mc_aux3(DB,[Keyword|_],DRuleNum,N,NewN,Mn,_,Mn) :-
	updaterule(DB,Keyword,DRuleNum,N,NewN).


/****************************************************************************/
% process_input(+Input_List,+[],?Output)
%     returns part of input after a comma, or
%             part of input before a period

process_input(_,[],L,L).
process_input(DB,['.'|_],L,L) :- findkeywords(DB,L,K), length(K,Kl), Kl >= 3,!.
process_input(DB,['.'|T],_,L) :- !, process_input(DB,T,[],L).
process_input(DB,[','|_],L,L) :- findkeywords(DB,L,K), length(K,Kl), Kl >= 3,!.
process_input(DB,[','|T],_,L) :- !, process_input(DB,T,[],L).
process_input(DB,[H|T],S,L) :- append(S,[H],S2), process_input(DB,T,S2,L).


/****************************************************************************/
% findkeywords(+InputList,?KeyWordList)
%    returns a list with the keywords in the input list
%    if no keywords are found returns a list with keywords 'memory' and 'none'

findkeywords(eliza,[],[[memory,0],[none,0]]).
findkeywords(DB,[H|T],[[H,I]|T1]) :- clause(rules(DB,[[H,I]|_]),_), !, findkeywords(DB,T,T1).
findkeywords(DB,[_|T],T1) :- findkeywords(DB,T,T1).


/****************************************************************************/
% sortkeywords(+KeyWordList,?SortedList)
%    returns a list with the keywords sorted according to their importance
%    this routine implements a simple bubble sort, customized for this
%    application/hyp


sortkeywords(X,Y) :- sort_aux(X,A,1), !, sortkeywords(A,Y).
sortkeywords(X,Y) :- sort_aux(X,Y,_).

sort_aux([],[],0).
sort_aux([X],[X],0).
sort_aux([[A,X],[B,Y]|T],[[B,Y],[A,X]|T],1) :- X < Y.
sort_aux([X,Y|T],[X|T2],S) :- sort_aux([Y|T],T2,S).


/****************************************************************************/
% updaterule(+KeyList,+DRuleNum,+N,+NewN)
%    updates a rule by changing the number of the reassembly rule associated
%       with a decomposition rule. The main rule to modify is indicated by
%       KeyList. The decomposition rule within the main rule is indicated by
%       DRuleNum. N is the previous reassembly rule used. NewN is the new
%       one used. N is updated to NewN so that next time a different reassembly
%       (actually the next in sequence) in used.

updaterule(DB,KeyList,DRuleNum,N,NewN) :-
	retract(rules(DB,[KeyList,Rt])),
	replace([DRuleNum,A,N|T],Rt,[DRuleNum,A,NewN|T],Rt2),
	assertz(rules(DB,[KeyList,Rt2])).

mem(eliza,[my,name,is,prolog]).
mem(eliza,[nephrael,is,an,eleet,haXor]).



rules(hyp,[[X,100],[
        [1,[A,X,B],0,
		[A,Y,B] ]]]):-
                        s(XX,XXX,X,_,_,_),
                        wn(hyp,XX,XXX,YY,YYY),
                        s(YY,YYY,Y,_,_,_).



/*****************************************************************************/
% The rule base
%    The format of the rules is:
%
% rules([[keyword, importance of keyword],[
%         [pattern #, [the pattern], last response used,
%             [response 1],
%             [response 2],
%             ...
%             [response n]]]]).

rules_db([[sorry,0],[
	[1,[_],0,
		[please,do,not,apologize,.],
		[apologies,are,not,necessary,.],
		[what,feelings,do,you,have,when,you,apologize,?],
		['I',have,told,you,that,apologies,are,not,required,.]]]]).

rules_db([[remember,5],[
	[1,[_,you,remember,Y],0,
		[do,you,often,think,of,Y,?],
		[does,thinking,of,Y,bring,anything,else,to,mind,?],
		[what,else,do,you,remember,?],
		[why,do,you,remember,Y,just,now,?],
		[what,in,the,present,situation,reminds,you,of,Y,?],
		[what,is,the,connection,between,me,and,Y,?]],
	[2,[_,do,i,remember,Y],0,
		[did,you,think,'I',would,forget,Y,?],
		[why,do,you,think,i,should,recall,Y,now,?],
		[what,about,Y,?],
		[equal,[what,0]],
		[you,mentioned,Y,.]],
	[3,[_],0,
		[newkey]]]]).

rules_db([[if,3],[
	[1,[_,if,Y],0,
		[do,you,think,it,is,likely,that,Y,?],
		[do,you,wish,that,Y,?],
		[what,do,you,think,about,Y,?],
		[really,',',if,Y,?]]]]).

rules_db([[dreamt,4],[
	[1,[_,you,dreamt,Y],0,
		[really,',',Y,?],
		[have,you,ever,fantasied,Y,while,you,were,awake,?],
		[have,you,dreamt,Y,before,?],
                [equal,[dream,3]],
		[newkey]]]]).

rules_db([[dream,3],[
	[1,[_],0,
		[what,does,that,dream,suggest,to,you,?],
		[do,you,dream,often,?],
		[what,persons,appear,in,your,dreams,?],
		[do,you,believe,that,dreaming,has,something,to,do,with,your,problem,?],
		[newkey]]]]).


rules_db([[none,0],[
	[1,[_],0,
		[meta(unknown)]
                
                ]]]).


rules_db([[perhaps,0],[
	[1,[_],0,
		[you,do,not,seem,quite,certain,.],
		[why,the,uncertain,tone,?],
		[can,you,not,be,more,positive,?],
		[you,are,not,sure,?],
		[do,you,not,know,?]]]]).

rules_db([[name,15],[
	[1,[_],0,
		['I',am,not,interested,in,names,.],
		['I',have,told,you,before,',','I',do,not,care,about,names,.],
		[please,continue,.]]]]).

rules_db([[xfremd,75],[
	[1,[_],0,
		['I',am,sorry,',','I',only,speak,'English',.]]]]).

rules_db([[computer,50],[
	[1,[_],0,
		[do,computers,worry,you,?],
		[why,do,you,mention,computers,?],
		[what,do,you,think,machines,have,to,do,with,your,problem,?],
		[do,you,not,think,computers,can,help,people,?],
		[what,about,machines,worries,you,?],
		[what,do,you,think,about,machines,?]]]]).

rules_db([[are,0],[
	[1,[_,are,you,Y],0,
		[do,you,believe,you,are,Y,?],
		[would,you,want,to,be,Y,?],
		[you,wish,'I',would,tell,you,you,are,Y,?],
                [what,would,it,mean,if,you,were,Y,?],
		[equal,[what,0]]],
	[2,[_,are,i,Y],0,
		[why,are,you,interested,in,whether,'I',am,Y,or,not,?],
		[would,you,prefer,if,'I',were,not,Y,?],
		[perhaps,'I',am,Y,in,your,fantasies,.],
                [do,you,sometimes,think,'I',am,Y,?],
		[equal,[what,0]]],
	[3,[_,are,Y],0,
		[did,you,think,they,might,not,be,Y,?],
		[would,you,like,it,if,they,were,not,Y,?],
		[what,if,they,were,not,Y,?],
		[possibly,they,are,Y,.]],
	[4,[_],0,
		[why,do,you,say,am,?],
		['I',do,not,understand,that,.]]]]).

rules_db([[foobar,10],[
        [1,[X,foobar,Y],0,
                [X,my_foobar,Y],
                [really,X,',',my,Y,?]]]]).


rules_db([[my,0],[
	[1,[_,my,Y],0,
		[why,are,you,concerned,over,my,Y,?],
		[what,about,your,own,Y,?],
		[are,you,worried,about,someone,elses,Y,?],
		[really,',',my,Y,?]]]]).

rules_db([[was,2],[
	[1,[_,was,you,Y],0,
		[what,if,you,were,Y,?],
		[do,you,think,you,were,Y,?],
		[were,you,Y,?],
		[what,would,it,mean,if,you,were,Y,?],
                [what,does,Y,suggest,to,you,?],
		[equal,[what,0]]],
	[2,[_,you,was,Y],0,
		[were,you,really,?],
		[why,do,you,tell,me,you,were,Y,just,now,?],
		[perhaps,'I',already,knew,you,were,Y,.]],
	[3,[_,was,i,Y],0,
		[would,you,like,to,believe,'I',was,Y,?],
		[what,suggests,that,'I',was,Y,?],
		[what,do,you,think,?],
		[perhaps,'I',was,Y,.],
		[what,if,'I',had,been,Y,?]],
	[4,[_],0,
		[newkey]]]]).

rules_db([[im,0],[
        [1,[_,im,_],0,
		[equal,[i,0]]]]]).

rules_db([[youre,0],[
        [1,[_,youre,_],0,
		[equal,[you,0]]]]]).

rules_db([[you,0],[
	[1,[_,you,cant,Y],0,
		[how,do,you,know,you,can,not,Y,?],
		[have,you,tried,?],
		[perhaps,you,could,Y,now,?],
		[do,you,really,want,to,be,able,to,Y,?]],
	[2,[_,you,dont,Y],0,
		[do,you,not,really,Y,?],
		[why,do,you,not,Y,?],
		[do,you,wish,to,be,able,to,Y,?],
		[does,that,trouble,you,?]],
	[3,[_,you,feel,Y],0,
		[tell,me,more,about,such,feelings,.],
		[do,you,often,feel,Y,?],
		[do,you,enjoy,feeling,Y,?],
		[of,what,does,feeling,Y,remind,you,?]],
        [4,[_,you,was,_],0,
		[equal,[was,2]]],
	[5,[_,you,Y,i,_],0,
		[perhaps,in,your,fantasy,we,Y,each,other,?],
		[do,you,wish,to,Y,me,?],
		[you,seem,to,need,to,Y,me,.],
		[do,you,Y,anyone,else,?]],
	[6,[_,you,[*,want,need,_],Y],0,
		[what,would,it,mean,to,you,if,you,got,Y,?],
		[why,do,you,want,Y,?],
		[suppose,you,got,Y,soon,?],
		[what,if,you,never,got,Y,?],
		[what,would,getting,Y,mean,to,you,?],
		[what,does,wanting,Y,have,to,do,with,this,discussion,?]],
	[7,[_,you,[*,feel,think,believe,wish,_],you,Y],0,
		[do,you,really,think,so,?],
		[but,you,are,not,sure,you,Y,?],
		[do,you,really,doubt,you,Y,?]],
        [8,[_,you,_,[*,feel,think,believe,wish,_],_,i,_],0,
		[equal,[you,0]]],
	[9,[_,youre,_,[*,sad,unhappy,depressed,sick,M],_],0,
		['I',am,sorry,to,hear,you,are,M,.],
		[do,you,think,coming,here,will,help,you,not,to,be,M,?],
		['I',am,sure,it,is,not,pleasant,to,be,M,.],
		[can,you,explain,what,made,you,M,?]],
	[10,[_,youre,_,[*,happy,elated,glad,better,M],_],0,
		[how,have,'I',helped,you,to,be,M,?],
		[has,your,treatment,made,you,M,?],
		[what,makes,you,M,just,now,?],
		[can,you,explain,why,you,are,suddenly,M,?]],
	[11,[_,youre,Y],0,
		[is,it,because,you,are,Y,that,you,came,to,me,?],
		[how,long,have,you,been,Y,?],
		[do,you,believe,it,normal,to,be,Y,?],
		[do,you,enjoy,being,Y,?]],
	[12,[X],0,
		[you,say,X],
		[can,you,elaborate,on,that,?],
		[do,you,say,X,for,some,special,reason,?],
		[that,is,quite,interesting,.]]]]).

rules_db([[i,0],[
        [1,[_,i,remind,you,of,_],0,
		[equal,[dit,10]]],
	[2,[_,im,Y],0,
		[what,makes,you,think,'I',am,Y,?],
		[does,it,please,you,to,believe,'I',am,Y,?],
		[do,you,sometimes,wish,you,were,Y,?],
		[perhaps,you,would,like,to,be,Y,?]],
	[3,[_,i,Y,you],0,
		[why,do,you,think,'I',Y,you,?],
		[you,like,to,think,'I',Y,you,',',do,you,not,?],
		[what,makes,you,think,'I',Y,you,?],
		[really,',','I',Y,you,.],
		[do,you,wish,to,believe,'I',Y,you,?],
		[suppose,'I',did,Y,you,',',what,would,that,mean,to,you,?],
		[does,someone,else,believe,'I',Y,you,?]],
	[4,[_,i,Y],0,
		[we,were,discussing,you,',',not,me,.],
		[oh,',',i,Y,.],
		[you,are,not,really,talking,about,me,',',are,you,?],
		[what,are,your,feelings,now,?]]]]).

rules_db([[no,0],[
	[1,[_],0,
		[are,you,saying,no,just,to,be,negative,?],
		[you,are,being,a,bit,negative,.],
		[why,not,?],
		[why,no,?]]]]).

rules_db([[your,2],[
	[1,[_,your,_,[*,mother,father,brother,sister,children,wife,M],Z],0,
		[tell,me,more,about,your,family,.],
		[who,else,in,your,family,Z,?],
		[your,M,?],
		[what,else,comes,to,mind,when,you,think,of,your,M,?]],
	[2,[_,your,Y],0,
		[your,Y,?],
		[why,do,you,say,your,Y,?],
		[does,that,suggest,anything,else,which,belongs,to,you,?],
		[is,it,important,to,you,that,your,Y,?]]]]).

rules_db([[memory,0],[
	[1,[_,your,Y],0,
		[lets,discuss,further,why,your,Y,.],
		[earlier,you,said,your,Y,.],
		[but,your,Y,?],
		[does,that,have,anything,to,do,with,the,fact,that,your,Y,?]]]]).
		
rules_db([[can,0],[
	[1,[_,can,i,Y],0,
		[you,believe,'I',can,Y,',',do,you,not,?],
		[equal,[what,0]],
		[you,want,me,to,be,able,to,Y,?],
		[perhaps,you,would,like,to,be,able,to,Y,yourself,?]],
	[2,[_,can,you,Y],0,
		[whether,or,not,you,can,Y,depends,on,you,more,than,on,me,.],
		[do,you,want,to,be,able,to,Y,?],
                [perhaps,you,do,not,want,to,Y,.],
		[equal,[what,0]]]]]).

rules_db([[what,0],[
	[1,[_],0,
		[why,do,you,ask,?],
		[does,that,question,interest,you,?],
		[what,is,it,you,really,want,to,know,?],
		[are,such,questions,much,on,your,mind,?],
		[what,answer,would,please,you,most,?],
		[what,do,you,think,?],
		[what,comes,to,your,mind,when,you,ask,that,?],
		[have,you,asked,such,questions,before,?],
		[have,you,asked,anyone,else,?]]]]).

rules_db([[because,0],[
	[1,[_],0,
		[is,that,the,real,reason,?],
		[do,any,other,reasons,not,come,to,mind,?],
		[does,that,reason,seem,to,explain,anything,else,?],
		[what,other,reasons,might,there,be,?]]]]).

rules_db([[why,0],[
	[1,[_,why,dont,i,Y],0,
		[do,you,believe,'I',do,not,Y,?],
		[perhaps,'I',will,Y,in,good,time,.],
		[should,you,Y,yourself,?],
                [you,want,me,to,Y,?],
		[equal,[what,0]]],
	[2,[_,why,cant,you,Y],0,
		[do,you,think,you,should,be,able,to,Y,?],
		[do,you,want,to,be,able,to,Y,?],
		[do,you,believe,this,will,help,you,to,Y,?],
                [have,you,any,idea,why,you,can,not,Y,?],
		[equal,[what,0]]]]]).

rules_db([[everyone,2],[
	[1,[_,[*,everyone,everybody,nobody,noone,M],_],0,
		[really,',',M,?],
		[surely,not,M,?],
		[can,you,think,of,anyone,in,particular,?],
		[who,',',for,example,?],
		[you,are,thinking,of,a,very,special,person,?],
		[who,',',may,i,ask,?],
		[someone,special,perhaps,?],
		[you,have,a,paticular,person,in,mind,',',do,you,not,?],
		[who,do,you,think,you,are,talking,about,?]]]]).

rules_db([[always,1],[
	[1,[_],0,
		[can,you,think,of,a,specific,example,?],
		[when,?],
		[what,incident,are,you,thinking,of,?],
		[really,',',always,?]]]]).

rules_db([[like,10],[
        [1,[_,[*,im,youre,am,is,are,was,_],_,like,_],0,
		[equal,[dit,10]]],
	[2,[_],0,
		[newkey]]]]).

rules_db([[dit,10],[
	[1,[_],0,
		[in,what,way,?],
		[what,resemblance,do,you,see,?],
		[what,does,that,similarity,suggest,to,you,?],
		[what,other,connections,do,you,see,?],
		[what,so,you,suppose,that,resemblance,means,?],
		[what,is,the,connection,',',do,you,suppose,?],
		[could,there,really,be,some,connection,?],
		[how,?]]]]).

sr([asshole|X],[slippery,oriface|Y],X,Y).
sr([mouth|X],[orifice|Y],X,Y).
sr([crapple|X],[jeff|Y],X,Y).
sr([douglas|X],[nephrael|Y],X,Y).
sr([yesh|X],[yes|Y],X,Y).

sr([do,not|X],[dont|Y],X,Y).
sr([can,not|X],[cant|Y],X,Y).
sr([cannot|X],[cant|Y],X,Y).
sr([will,not|X],[wont|Y],X,Y).
sr([dreamed|X],[dreamt|Y],X,Y).
sr([dreams|X],[dream|Y],X,Y).
sr([how|X],[what|Y],X,Y).
sr([when|X],[what|Y],X,Y).
sr([alike|X],[dit|Y],X,Y).
sr([same|X],[dit|Y],X,Y).
sr([certainly|X],[yes|Y],X,Y).
sr([maybe|X],[perhaps|Y],X,Y).
sr([deutsch|X],[xfremd|Y],X,Y).
sr([francais|X],[xfremd|Y],X,Y).
sr([espanol|X],[xfremd|Y],X,Y).
sr([machine|X],[computer|Y],X,Y).
sr([machines|X],[computer|Y],X,Y).
sr([computers|X],[computer|Y],X,Y).

:-eliza.


