% ===================================================================
% File 'e2c.pl'
% Purpose: Attempto Controlled English to CycL conversions from SWI-Prolog  
% This implementation is an incomplete proxy for CycNL and likely will not work as well
% Maintainer: Douglas Miles
% Contact: $Author: dmiles $@users.sourceforge.net ;
% Version: 'interface.pl' 1.0.0
% Revision:  $Revision: 1.3 $
% Revised At:   $Date: 2005/06/06 15:43:15 $
% from Bratko chapter 17 page 455. This comes from Pereira and Warren paper, AI journal, 1980 
% ===================================================================
/*
:-module(e2c,[
	e2c/1,
	e2c/2,
	testE2C/0]).
*/
% imports sentencePos/2,bposToPos/2
:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).
:-set_prolog_flag(double_quotes,string).

:-use_module(library('http/http_open')).
   

rosetta(String):-
      getWordTokens(String,Words),
      concat_atom(Words,' ',PSent),
      rosetta(PSent,Result),    
      roseSave('ParseMtForSourceFn'('StringInDocumentFn'('E2C-Parses-CCW',string(Words))),Result),
      writeCycL(Result).
   
roseSave(Mt,interp(B)):-!,roseSave(Mt,B),!.
roseSave(Mt,[]):-!.
roseSave(Mt,[A|B]):-!,roseSave(Mt,A),!,roseSave(Mt,B),!.
roseSave(Mt,prologsyn(A)):-!,roseSave(Mt,A),!.
roseSave(Mt,sem('NLSentence',A)):-!. %,roseSave(Mt,A),!.
roseSave(Mt,sem(Situation,Consts)):-semSave(Mt,sem(Situation,Consts),E).
roseSave(Mt,A):-catch((cycAssert(Mt:A)),_,true).
%roseSave(Mt,Result):-

semSave(Mt,[],[]).
semSave(Mt,[C|Consts],[C1|Consts1]):-semSave(Mt,C,C1),!,semSave(Mt,Consts,Consts1),!.
semSave(Mt,sem(Situation,Consts),E):-semSave(Mt,Consts,Consts1),!,
         gensymType(Situation,E),roseSave(Mt,isa(E,'Individual')),!,roseSave(Mt,isa(E,Situation)),!,semSave(Mt,parts(E,Consts1),EE).
semSave(Mt,parts(E,[]),E):-!.
semSave(Mt,parts(E,[C1|Consts1]),E):-!,semSave(Mt,parts(E,C1),_),semSave(Mt,parts(E,Consts1),EO).
semSave(Mt,parts(E,'NLPattern-Word'(Word,Type)),E):-!,roseSave(Mt,hypotheticDenotation(E,Type,Word)).
semSave(Mt,parts(E,Type),E):-roseSave(Mt,situationConstituents(E,Type)).
semSave(Mt,Parts,Parts).


gensymType(Situation,E):-atom(Situation),gensym(Situation,E).
gensymType(Situation,E):-gensym('TERM',E).
   

rosetta(PSent,Result):-
   sformat(URL,'"~w"',[PSent]),
   %www_form_encode(SURL,URL),
   socket:tcp_socket(Sock),
   socket:tcp_connect(Sock,'10.1.1.101':2002),
   sformat(Rquest,'/cdml?pos=~w',[URL]),
   socket:tcp_open_socket(Sock,In,Out),
   format(Out, 'GET ~w HTTP/1.0\r\nHost: ~w\r\nUser-Agent: ~w\r\nConnection: close\r\n', [Rquest, '10.1.1.101', 'SWI-Prolog (http://www.swi-prolog.org)']) ,
   flush_output(Out),
     %copy_stream_data(In,user_output).
     load_structure(In,POut,[space(remove)]),!,
     tcp_close_socket(Sock),
     parseRosetta(POut,Result),!.


parsedNot('linkcount').
parsedNot('wordset').
parsedNot('rawwords').
parsedNot('hypolinks').
parsedNot('trigrams').
parsedNot('cyc-hypolinks').
parsedNot('cyc-query').
parsedNot('origsent').
parsedNot('wordcount').
parsedNot('parsecount').
parsedNot('semparse').
parsedNot('synparse').
parsedNot('totalparses').
parsedNot('link').
parsedNot('xdata').
parseRosetta('\n',[]).
parseRosetta('\n\n',[]).
parseRosetta([],[]).
parseRosetta(element(E,_,_),[]):-parsedNot(E).
parseRosetta(element(prologsem,_,[Data]),[(Term)]):-atom_to_term(Data,TermI,_),parseRosetta(TermI,Term).
parseRosetta(element(prologsyn,_,[Data]),[prologsyn(Term)]):-atom_to_term(Data,TermI,_),parseRosetta(TermI,Term).
parseRosetta(element(prologlinks,_,[Data]),[(Term)]):-atom_to_term(Data,TermI,_),parseRosetta(TermI,Term).
parseRosetta(element(linkgram,[],POut),Result):-parseRosetta(POut,Result).
parseRosetta(element(interpertation,[],POut),[interp(Result)]):-parseRosetta(POut,Result).

parseRosetta([POut|More],R):-!,parseRosetta(POut,R1),parseRosetta(More,R2),flatten([R1],RR1),flatten([R2],RR2),append(RR1,RR2,R).
parseRosetta(links(POut),Out):-!,parseRosetta((POut),Out).
parseRosetta(l(rw,A,B),[]):-!.
parseRosetta(l(P,A,B),'linkParserLink-IntermedPredicate'(PP,AA,BB)):-!,nodeTrans(P,PP),parseRosetta(A,AA),parseRosetta(B,BB).
parseRosetta(w(W,T),'NLPattern-Word'(WW,TTU)):-!,nodeTrans(T,TT),wordForString(W,WW,TTT),mostSpec(TTT,TT,TTU),!.
parseRosetta(w(W),'NLPattern-Word'(WW,TT)):-!,wordForString(W,WW,TT),!.
parseRosetta(node(P,Data),'sem'(PP,(POut))):-nodeTrans(P,PP),!,parseRosetta(Data,POut).
parseRosetta(POut,POut):-nl,writeq(POut),nl.

mostSpec(TTT,'NLWordForm',TTT).
mostSpec(TTT,TT,TT).

nodeTrans(v,'Verb').
nodeTrans(a,'Adjective').
nodeTrans(n,'Noun').
nodeTrans(p,'Pronoun').
nodeTrans('-','NLWordForm').
nodeTrans(np,'NounPhrase').
nodeTrans(v,'Verb').
nodeTrans(vp,'VerbPhrase').
nodeTrans(state,'TemporallyExistingThing').
nodeTrans(property,'Role').
nodeTrans(event,'Situation').
nodeTrans(action,'Action').
nodeTrans(thing,'SpatialThing').
nodeTrans(person,'Person').
nodeTrans(location,'SubcollectionOfWithRelationFromTypeFn'('EnduringThing-Localized',toLocation,'Translocation')).
nodeTrans(destination,'Location-Underspecified').


nodeTrans(ss,'TheSentenceSubject').
nodeTrans(pp,'PrepositionalPhrase').

nodeTrans(s,'NLSentence').
nodeTrans(whnp,'WHPronoun').

nodeTrans(P,string([P])).


end_of_file.

/sbin/iptables --flush
/sbin/iptables -t nat --flush

# Unlimited traffic on the loopback interface
/sbin/iptables -A INPUT -i lo -j ACCEPT
/sbin/iptables -A OUTPUT -o lo -j ACCEPT
# Unlimited traffic on the eth0 interface
/sbin/iptables -A INPUT -i eth0 -j ACCEPT
/sbin/iptables -A OUTPUT -o eth0 -j ACCEPT

# Set the default policy to drop
/sbin/iptables --policy INPUT ACCEPT
/sbin/iptables --policy OUTPUT ACCEPT
/sbin/iptables --policy FORWARD ACCEPT
/sbin/iptables -t nat --policy PREROUTING ACCEPT
/sbin/iptables -t nat --policy OUTPUT ACCEPT

