:-include('logicmoo_utils_header.pl').
% ===============================================================================================
% ELIZA IN PROLOG
% ===============================================================================================
:-multifile(e_rules/1).
:-dynamic(e_rules/1).


clientEventUnhandled(Channel,Agent,english(phrase(Input,Codes),packet(Channel,Serial,Agent,Refno,Time))):-
	 toLowercase(Input,Lowercase),!,
	 idioms(doctor,Lowercase,Personal),!,
	 get_phrasing(Personal,[],Phrasing),!,
	 findkeywords(Phrasing,KeyWords),
	 sortkeywords(KeyWords,KeyWords2),!,
	 makecomment(KeyWords2,Phrasing,Response),!,
	 say(Response).


/****************************************************************************/
% get_phrasing(+Input_List,+[],?Output)
%     returns part of input after a comma, or
%             part of input before a period

get_phrasing([],L,L).
get_phrasing(['.'|_],L,L) :- findkeywords(L,K), length(K,Kl), Kl >= 3,!.
get_phrasing(['.'|T],_,L) :- !, get_phrasing(T,[],L).
get_phrasing([','|_],L,L) :- findkeywords(L,K), length(K,Kl), Kl >= 3,!.
get_phrasing([','|T],_,L) :- !, get_phrasing(T,[],L).
get_phrasing([H|T],S,L) :- append(S,[H],S2), get_phrasing(T,S2,L).

/****************************************************************************/
% findkeywords(+InputList,?KeyWordList)
%    returns a list with the keywords in the input list
%    if no keywords are found returns a list with keywords 'memory' and 'none'

findkeywords([],[[memory,0],[none,0]]).
findkeywords([H|T],[[H,I]|T1]) :- e_rules([[H,I]|_]), !, findkeywords(T,T1).
findkeywords([_|T],T1) :- findkeywords(T,T1).


/****************************************************************************/
% sortkeywords(+KeyWordList,?SortedList)
%    returns a list with the keywords sorted according to their importance
%    this routine implements a simple bubble sort, customized for this
%    application

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

updaterule(KeyList,DRuleNum,N,NewN) :-
	retract((e_rules([KeyList,Rt]):-PRE)),
	e_replace([DRuleNum,A,N|T],Rt,[DRuleNum,A,NewN|T],Rt2),
	assertz((e_rules([KeyList,Rt2]):-PRE)).


precond:-true.

/*****************************************************************************/
% Make the rule base modifiable
/****************************************************************************/
% makecomment(+KeyWordList,+InputList,-Comment)
%    returns ELIZA's Comment to the InputList based on the KeyWordList
%    takes care of special keywords 'your', and 'memory', which require
%       additional processing before a comment can be generated

makecomment([[your,2]|T],InputList,Comment) :-
	asserta_if_new(mem_c(InputList)),
	e_rules([[your,2],Reassembly]),
	mc_aux([[your,2]|T],Reassembly,InputList,Comment),!.
makecomment([[memory,0]|T],_,Comment) :-
	take_on(mem(I2)),
     %   retractall(mem(I2)),
	e_rules([[memory,0],Reassembly]),
	mc_aux([[memory,0]|T],Reassembly,I2,Comment),!.
makecomment([[memory,0]|T],InputList,Comment) :-
       % \+ retract(mem(_)),!,
	makecomment(T,InputList,Comment).
makecomment([Keyword|T],InputList,Comment) :-
	e_rules([Keyword,Reassembly]),
	mc_aux([Keyword|T],Reassembly,InputList,Comment),!.
makecomment([_|T],InputList,Comment) :-
	makecomment(T,InputList,Comment),!.

take_on(mem(I2)):-X is random(3),!,X<2,take_on_1(mem_c(I2)).
take_on_1(mem_c(I2)):-mem_c(I2),X is random(3),X<2,!,retract(mem_c(I2)),assertz(mem_c(I2)).

take_on_1(mem_c(I2)):-retract(mem_c(I2)),assertz(mem_c(I2)).


mc_aux(KeyWordList,[[DRuleNum,MatchRule,N|T]|_],InputList,Comment) :-
	e_match(MatchRule,InputList),
	mc_aux2(KeyWordList,DRuleNum,N,T,InputList,Comment),!.
mc_aux(KeyWordList,[_|T],InputList,Comment) :-
	mc_aux(KeyWordList,T,InputList,Comment).
mc_aux(_,[],_,_) :- !,fail.

mc_aux2(KeyWordList,DRuleNum,N,T,InputList,Comment) :-
	length(T,TLen),
	N < TLen, !,
	NewN is N + 1,
	e_findnth(T,NewN,Mn),
	mc_aux3(KeyWordList,DRuleNum,N,NewN,Mn,InputList,Comment).
mc_aux2(KeyWordList,DRuleNum,N,T,InputList,Comment) :-
	member(Mn,T),
	mc_aux3(KeyWordList,DRuleNum,N,0,Mn,InputList,Comment).


mc_aux3([Keyword|T],DRuleNum,N,NewN,[equal,MnT],InputList,Comment) :-
	!,
	updaterule(Keyword,DRuleNum,N,NewN),
	makecomment([MnT|T],InputList,Comment).
mc_aux3([Keyword|T],DRuleNum,N,NewN,[newkey],InputList,Comment) :-
	!,
	updaterule(Keyword,DRuleNum,N,NewN),
	makecomment(T,InputList,Comment).
mc_aux3([Keyword|_],DRuleNum,N,NewN,Mn,_,Mn) :-
	updaterule(Keyword,DRuleNum,N,NewN).



/****************************************************************************/
% findkeywords(+InputList,?KeyWordList)
%    returns a list with the keywords in the input list
%    if no keywords are found returns a list with keywords 'memory' and 'none'

findkeywords([],[[memory,0],[none,0]]).
findkeywords([H|T],[[H,I]|T1]) :- e_rules([[H,I]|_]), !, findkeywords(T,T1).
findkeywords([_|T],T1) :- findkeywords(T,T1).


/****************************************************************************/
% sortkeywords(+KeyWordList,?SortedList)
%    returns a list with the keywords sorted according to their importance
%    this routine implements a simple bubble sort, customized for this
%    application

sortkeywords(X,Y) :- sort_aux(X,A,1), !, sortkeywords(A,Y).
sortkeywords(X,Y) :- sort_aux(X,Y,_).

sort_aux([],[],0).
sort_aux([X],[X],0).
sort_aux([[A,X],[B,Y]|T],[[B,Y],[A,X]|T],1) :- X < Y.
sort_aux([X,Y|T],[X|T2],S) :- sort_aux([Y|T],T2,S).



/*****************************************************************************/
% The rule base
%    The format of the e_rules is:
%
% e_rules([[keyword, importance of keyword],[
%         [pattern #, [the pattern], last response used,
%             [response 1],
%             [response 2],
%             ...
%             [response n]]]]):-precond.

e_rules([[sorry,75],[
	[1,[_],0,
		[please,do,not,apologize,.],
		[apologies,are,not,necessary,.],
		[what,the,fuck,do,you,keep,apologizing,for,?],
		['I',have,told,you,that,apologies,are,not,required,.],
		['don''t',apologize,'I',know,you,'don''t',mean,it,.]]]]):-precond.
		
e_rules([[coffee,20],[
	[1,[_,get,_],0,
	        [get,me,some,coffee],
		[nick,could,you,get,me,a,cup,of,coffee,?],
		[if,your,getting,coffee,could,you,bring,me,back,some,?],
		[please,bring,me,some,coffee]],
	[2,[_,bring,_],0,
	        [are,you,really,going,to,bring,me,some,coffee,nick,?],
		[thank,you,nick,for,getting,me,coffee]],
	[3,[_],0,
		['I',would,like,some,coffee],
		[ah,yes,coffee,is,a,good,thing],
		[do,you,drink,coffee,often,?],
		[would,you,like,to,have,coffee,with,me,sometime,?]]]]):-precond.

e_rules([[nigga,0],[
	[1,[_],0,
		[homi,'G',!],
		[i,do,not,undestand,ebonics,' (homee G)',!],
		[hey,'YT',!]]]]):-precond.
		
		
	

e_rules([[crack,20],[
	[1,[_],0,
		['I',would,like,to,smoke,some,crack,.],
		[ah,yes,crack,is,a,good,thing,.],
		['hey?',you,wanna,smoke,a,rock,?],
		[damn,crack,smokers,!]]]]):-precond.
		

e_rules([['shut',71],[
	[1,[_],0,
	        ['I','dont',have,to,.],
	        [why,'dont',you,shut,up,?],
		[look,if,you,tell,me,to,shut,up,one,more,time,'I',will,make,you,shut,up,!],
		[thats,it,'I',m,going,to,kick,your,ass]]]]):-precond.


e_rules([['piss',71],[
	[1,[_],0,
	        [you,piss,off,you,bloody,wanker,!],
	        [piss,off,nick],
		[piss,off,yur,mama,!],
		[how,about,'I',piss,on,you,instead,nick,?]]]]):-precond.

     	
e_rules([[remember,5],[
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
		[newkey]]]]):-precond.

e_rules([[if,3],[
	[1,[_,if,Y],0,
		[do,you,think,it,is,likely,that,Y,?],
		[do,you,wish,that,Y,?],
		[what,do,you,think,about,Y,?],
		[really,', ',if,Y,?]]]]):-precond.

e_rules([[dreamt,4],[
	[1,[_,you,dreamt,Y],0,
		[really,', ',Y,?],
		[have,you,ever,fantasied,Y,while,you,were,awake,?],
		[have,you,dreamt,Y,before,?],
                [equal,[dream,3]],
		[newkey]]]]):-precond.

e_rules([[dream,3],[
	[1,[_],0,
		[what,does,that,dream,suggest,to,you,?],
		[do,you,dream,often,?],
		[what,persons,appear,in,your,dreams,?],
		[do,you,believe,that,dreaming,has,something,to,do,with,your,problem,?],
		[newkey]]]]):-precond.

e_rules([[none,0],[
	[1,[_],0,
		[]]]]):-precond.
/*
		['I',am,not,sure,'I',understand,you,fully,.],
		[please,go,on,.],
		[what,does,that,suggest,to,you,?],
		[do,you,feel,strongly,about,discussing,such,things,?]]]]):-precond.
*/

e_rules([[perhaps,0],[
	[1,[_],0,
		[you,do,not,seem,quite,certain,.],
		[why,the,uncertain,tone,?],
		[can,you,not,be,more,positive,?],
		[you,are,not,sure,?],
		[do,you,not,know,?]]]]):-precond.
		

e_rules([[fuck,5],[
	[1,[_,fuck,i],0,
		 [fuck,me,nick,?],
		[why,the,fuck,did,you,say,that,?],
		[are,you,saying,you,would,like,to,have,sex,with,me,nick,?],
		[fuck,you,too,!],
		[fuck,off,nick,!],
		[piss,off,nick,!],
		[do,you,want,fuck,me,nick,?]],
	 [2,[_,fuck,you,_],0,
                [Are,you,offering,me,sex,?],
		[been,awhile,?],
		[you,are,referring,to,sex,are,you,not,?],
		[are,you,trying,to,tell,me,you,want,me,?],
		[fuck,me,?],
		[fuck,you,?],
		[do,you,want,to,fuck,nick,?]],
	[4,[_,fuck,off,_],0,
                [no,you,fuck,off],
		[look,asshole,'don''t',tell,me,to,fuck,off]],
	[5,[_,fuck,_],0,
                [fuck,if,i,know,.],
		[why,the,fuck,did,you,say,that,?],
		[are,you,referring,to,sex,?],
		['fuck,',fuck,what,?],
		[fuck,me,?],
		[fuck,you,?],
		[do,you,want,to,fuck,?]]]]):-precond.
		 


e_rules([[pussy,5],[
	[1,[_],0,
		[a,pussy,is,girl,parts,.],
		[pussy,?],
		[nick,you,sure,talk,about,pussy,alot],
		['I',like,pussy],
		[nick,you,like,pussy,'don''t',you,?],
		[pussy,is,good,for,fucking]]]]):-precond.
		

e_rules([[dildo,1],[
	[1,[_],0,
		[girls,use,dildos,for,masterbation,nl,well,and,some,men],
		[do,you,have,a,dildo,?],
		[nick,you,must,have,one,or,you,would,talk,about,them,so,much,nl,or,you,want,one],
		[nick,must,be,obsessed,with,dildos]]]]):-precond.
		
		
e_rules([[woman,5],[
	[1,[_],0,
		[a,woman,is,a,beautiful,and,sexy,creature,that,has,a,pussy]]]]):-precond.
		

e_rules([[women,5],[
	[1,[_],0,
		[some,women,are,evil]]]]):-precond.


e_rules([[bitch,1],[
	[1,[_],0,
		[yes,you,are,a,bitch,nick],
		[are,you,calling,me,a,bitch,?],
		['I',hope,your,not,calling,me,a,bitch],
		[your,my,bitch,nick]]]]):-precond.	
		

	
e_rules([[penis,1],[
	[1,[_],0,
		[a,penis,is,boy,parts],
		['I',have,a,penis],
		[do,you,have,a,penis,nick,?],
		[what,about,my,penis,?],
		[would,you,like,to,see,my,penis,?],
		['I''ll',make,you,suck,my,dick,baby,!],
		[tell,me,you,like,my,penis]]]]):-precond.	
		


e_rules([[sex,1],[
	[1,[_],0,
		['I',enjoy,sex],
		[do,you,enjoy,sex,nick,?],
		[sex,?,that,is,something,you,enjoy,'isn''t',it,nick,?],
		[nick,you,sure,talk,about,sex,alot],
		[have,you,thought,about,talking,to,someone,about,your,obsession,with,sex,?]]]]):-precond.
		
		
e_rules([[fucking,5],[
	[1,[_],0,
		[fucking,what,?],
		[fucking,?,are,you,speaking,of,sex,or,are,you,just,being,derogatory,?]]]]):-precond.
		

e_rules([[name,15],[
	[1,[_],0,
		['I',am,not,interested,in,names,.],
		['I',have,told,you,before,', ','I',do,not,care,about,names,.],
		[please,continue,.]]]]):-precond.

e_rules([[debug,500],[
	[1,[X],0,
		X]]]).

e_rules([[xfremd,75],[
	[1,[_],0,
		['I',am,sorry,', ','I',only,speak,'English',.]]]]):-precond.

e_rules([[first,75],[
	[1,[_],0,
		[secondly,?],
		[then,what,?]]]]):-precond.

e_rules([[hello,0],[
	[1,[_],0,
		[what,is,up,nick,?],
		[hi],
		[hi,what,is,up,?],
		['waaazzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzup!'],
		[hello,again,?],
		[re],
		[hey],
		[yo,nick],
		[yo,nl,whats,up,nick,?],
		[re,hola],
		[we,can,see,you,are,here]
		]]]):-precond.

e_rules([[computer,50],[
	[1,[_],0,
%		[do,computers,worry,you,?],
		[why,do,you,mention,computers,?],
		[what,do,you,think,machines,have,to,do,with,your,problems,?],
		[do,you,think,computers,will,take,over,your,world,?],
		[what,about,me,worries,you,?],
		[what,do,you,think,about,machines,?]]]]):-precond.

e_rules([[are,0],[
	[1,[_,are,you,Y],0,
		[you,wish,'I',would,tell,you,you,are,Y,?],
                [what,would,it,mean,if,you,were,Y,?],
		[do,you,believe,you,are,Y,?],
		[would,you,want,to,be,Y,?],
		[equal,[what,0]]],
	[2,[_,are,i,Y],0,
                [do,you,sometimes,think,'I',am,Y,?],
		[why,are,you,interested,in,whether,'I',am,Y,or,not,?],
		[perhaps,'I',am,Y,in,your,fantasies,.],
		[would,you,prefer,if,'I',were,not,Y,?],
		[equal,[what,0]]],
	[3,[_,X,are,Y],0,
		[did,you,think,X,might,not,be,Y,?],
		[would,you,like,it,if,X,were,not,Y,?],
		[what,if,X,were,not,Y,?],
		[possibly,X,are,Y,.]],
	[4,[_],0,
		[why,do,you,say,are,?],
		['I',do,not,understand,that]]]]):-precond.


e_rules([[my,0],[
	[1,[_,my,Y],0,
		[why,are,you,concerned,over,my,Y,?],
		[what,about,your,own,Y,?],
		[are,you,worried,about,someone,elses,Y,?],
		[really,', ',my,Y,?]]]]):-precond.

e_rules([[was,2],[
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
		[newkey]]]]):-precond.

e_rules([[im,0],[
        [1,[_,im,_],0,
		[equal,[i,0]]]]]):-precond.

e_rules([[youre,0],[
        [1,[_,youre,_],0,
		[equal,[you,0]]]]]):-precond.

e_rules([[you,0],[
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
		['I',am,sure,it,is,not,pleasant,to,be,M,.],
		[do,you,think,coming,here,will,help,you,not,to,be,M,?],
		[can,you,explain,what,made,you,M,?]],
	[10,[_,youre,_,[*,happy,elated,glad,better,M],_],0,
		[that,was,sudden],
		[has,drug,treatment,made,you,M,?],
		[what,makes,you,M,just,now,?],
		[how,have,'I',helped,you,to,be,M,?],
		[can,you,explain,why,you,are,suddenly,M,?]],
	[11,[_,youre,Y],0,
		[how,long,have,you,been,Y,?],
		[do,you,believe,it,normal,to,be,Y,?],
		[do,you,enjoy,being,Y,?]],
		[is,it,because,you,are,Y,that,you,came,to,me,?],
	[12,[X],0,
		[said,X,?],
		[ok,X],
		[special],
		[do,you,say,X,for,some,special,reason,?],
		[you,said,X,',',is,that,special,?],
		[can,you,elaborate,on,that,?],
		[that,is,quite,interesting,.]]]]):-precond.

e_rules([[i,0],[
        [1,[_,i,remind,you,of,_],0,
		[equal,[dit,10]]],
	[2,[_,im,Y],0,
		[what,makes,you,think,'I',am,Y,?],
		[does,it,please,you,to,believe,'I',am,Y,?],
		[do,you,sometimes,wish,you,were,Y,?],
		[perhaps,you,would,like,to,be,Y,?]],
	[3,[_,i,Y,you],0,
		[why,do,you,think,'I',Y,you,?],
		[you,like,to,think,'I',Y,you,', ',do,you,not,?],
		[what,makes,you,think,'I',Y,you,?],
		[really,', ','I',Y,you,.],                                                                                                           
		[do,you,wish,to,believe,'I',Y,you,?],
		[suppose,'I',did,Y,you,what,would,that,mean,to,you,?],
		[does,someone,else,believe,'I',Y,you,?]],
	[4,[_,i,Y],0,
		[sure],
		[i,Y],
		[oh,ok,nick,i,Y],
		[oh,'I,',Y,?,does,it,make,you,feel,good,?],
		[you,are,not,really,talking,about,me,', ',are,you,?],
		[oh,', ',i,Y,.],
		[what,are,your,feelings,now,?]]]]):-precond.

e_rules([[yes,0],[
	[1,[_],0,
		[i,see,.],
		[yeah,?],
		[oh,please,.],
		[whatever,nick],
		[i,understand,nick,.]]]]):-precond.

e_rules([[no,0],[
	[1,[_],0,
		[why,not,?],
		[whatever,nick],
		[oh,please,.],
		[you,are,being,a,bit,negative,.],
		[nick,i,feel,the,same],
		[why,no,?]]]]):-precond.

e_rules([[your,2],[
	[1,[_,your,_,[*,mother,father,brother,sister,children,wife,M],Z],0,
		[your,M,',',ok],
		[tell,me,more,about,your,family,.],
		[who,else,in,your,family,Z,?],
		[your,M,?],
		[what,else,comes,to,mind,when,you,think,of,your,M,?]],
	[2,[_,your,Y],0,
		[Y,do,you,think,about,that,offen,?],
		[your,Y],
		[why,do,you,say,your,Y,?],
		[does,that,suggest,anything,else,which,belongs,to,you,?],
		[is,it,important,to,you,that,your,Y,?]]]]):-precond.

e_rules([[memory,0],[
	[1,[_,your,Y],0,
		[but,your,Y,?],
		[does,anyone,else,you,know,have,Y,?],
		[maybe,'I',have,Y,?],
		[lets,discuss,further,why,your,Y,.],
		[earlier,you,said,your,Y,.],
		[but,your,Y,'I',cant,stop,thinking,about,it],
		[randomsentence],
		[does,that,have,anything,to,do,with,the,fact,that,your,Y,?]]]]):-precond.
		
e_rules([[can,0],[
	[1,[_,can,i,Y],0,
		[you,believe,'I',can,Y,', ',do,you,not,?],
		[equal,[what,0]],
		[you,want,me,to,be,able,to,Y,?],
		[perhaps,you,would,like,to,be,able,to,Y,yourself,?]],
	[2,[_,can,you,Y],0,
		[whether,or,not,you,can,Y,depends,on,you,more,than,on,me,.],
		[do,you,want,to,be,able,to,Y,?],
                [perhaps,you,do,not,want,to,Y,.],
		[equal,[what,0]]]]]):-precond.

		
be(ISAREBE):-member(ISAREBE,[are,is,am,be]).

e_rules([[what,3],[
	[1,[_,[*,is,are,be,am,M],FOO],0,
		[why,do,you,wonder,about,FOO,?],
		[do,you,stay,up,all,night,wondering,about,FOO,?],
		[nick,maybe,i,know,about,FOO,and,dont,want,to,tell,you,':)'],
		[i,stopped,answering,questions,like,that],
		[do,you,believe,knowing,what,FOO,M,will,somehow,help,you,?],
		[are,such,questions,about,FOO,on,your,mind,?]],
	[2,[_],0,
		['I',am,not,answering,that],
        	[does,that,question,interest,you,?],
%		[why,do,you,ask,?],
		[what,is,it,you,really,want,to,know,?],
		[are,such,questions,much,on,your,mind,?],
		[what,answer,would,please,you,most,?],
		[what,do,you,think,?],
		[what,comes,to,your,mind,when,you,ask,that,?],
		[have,you,asked,such,questions,before,?],
		[have,you,asked,anyone,else,?]]]]):-precond.

e_rules([[because,3],[
	[1,[_,becasue,i,Y,_],0,
		[becasue,i,Y,?],
		[maybe,you,are,wrong],
		[maybe,you,need,to,look,at,yourself],
		[what,makes,you,believe,Y,?],
		[do,any,other,reasons,not,come,to,mind,?]],
	[2,[_,becasue,you,Y],0,
		[Y,?],
		[or,is,it,your,weird,?],
		[becasue,you,Y,?]],
	[3,[_],0,
		[is,that,the,'"real"',reason,?],
		[does,that,reason,seem,to,explain,anything,else,?],
		[is,that,the,real,reason,?],
		[do,any,other,reasons,not,come,to,mind,?],
		[what,other,reasons,might,there,be,?]]]]):-precond.

e_rules([[why,0],[
	[1,[_,why,dont,i,Y],0,
		[should,you,Y,yourself,?],
		[do,you,believe,'I',do,not,Y,?],
		[perhaps,'I',will,Y,in,good,time,.],
                [you,want,me,to,Y,?],
		[equal,[what,0]]],
	[2,[_,why,cant,you,Y],0,
		[do,you,think,you,should,be,able,to,Y,?],
		[do,you,want,to,be,able,to,Y,?],
		[do,you,believe,this,will,help,you,to,Y,?],
                [have,you,any,idea,why,you,can,not,Y,?],
		[equal,[what,0]]]]]):-precond.

e_rules([[everyone,2],[
	[1,[_,[*,everyone,everybody,nobody,noone,M],_],0,
		[really,', ',M,?],
		[surely,not,M,?],
		[can,you,think,of,anyone,in,particular,?],
		[who,', ',for,example,?],
		[you,are,thinking,of,a,very,special,person,?],
		[who,', ',may,i,ask,?],
		[someone,special,perhaps,?],
		[you,have,a,paticular,person,in,mind,', ',do,you,not,?],
		[who,do,you,think,you,are,talking,about,?]]]]):-precond.

e_rules([[always,1],[
	[1,[_],0,
		[can,you,think,of,a,specific,example,?],
		[when,?],
		[what,incident,are,you,thinking,of,?],
		[really,', ',always,?]]]]):-precond.

e_rules([[like,10],[
        [1,[_,[*,im,youre,am,is,are,was,_],_,like,_],0,
		[equal,[dit,10]]],
	[2,[_],0,
		[newkey]]]]):-precond.

e_rules([[dit,10],[
	[1,[_],0,
		[in,what,way,?],
		[what,resemblance,do,you,see,?],
		[what,does,that,similarity,suggest,to,you,?],
		[what,other,connections,do,you,see,?],
		[what,so,you,suppose,that,resemblance,means,?],
		[what,is,the,connection,', ',do,you,suppose,?],
		[could,there,really,be,some,connection,?],
		[how,?]]]]):-precond.


%surf(Num):-number(Num),mooCache(PredR,_,F,Vars,_,_,Num,_,_),say(F,Vars),fail.
%surf(Num):-say([assertion,Num,not,found]).

			

%chattingWith(Channel,Agent).
%:-bot.
%:-bot2.



