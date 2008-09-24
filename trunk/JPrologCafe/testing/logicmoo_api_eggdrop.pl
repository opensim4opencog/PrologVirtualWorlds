%<?
:-include('logicmoo_utils_header.pl'). %<?
% ===================================================================
% Conenctors
% ===================================================================
eggdropConnect(In,Out,BotNick,Port):-eggdropConnect(In,Out,'127.0.0.1',Port,BotNick,logicmoo).

:-use_module(library(socket)).
eggdropConnect:- eggdropConnect(In,Out,'swipl',11444).

eggdropConnect(InStream,OutStream,Host,Port,Agent,Pass):-
       tcp_socket(SocketId),
       tcp_connect(SocketId,Host:Port),
       tcp_open_socket(SocketId, InStream, OutStream),
       mformat(OutStream,'~w\n',[Agent]),flush_output(OutStream),
       mformat(OutStream,'~w\n',[Pass]),flush_output(OutStream),
       retractall(stdio(Agent,_,_)),
       asserta((stdio(Agent,InStream,OutStream))),!.
		
consultation_thread(BotNick,Port):- 
      eggdropConnect(In,Out,BotNick,Port),
      to_egg('.echo off\n'),
      to_egg('.console ~w ""\n',[BotNick]),
      to_egg('eot.\n',[]),
      repeat,
      catch(read_term(In,CMD,[variable_names(Vars)]),_,fail),      
      catch(CMD,E,to_user_error(E)),
      fail.

get2react([L|IST1]):- convert_to_strings(IST1,IST2), CALL =.. [L|IST2],/*channel_say("#logicmoo",CALL),*/ catch(CALL,E,to_user_error(E)).
get2react([L|IST1]).

:-dynamic(default_channel/1).
:-dynamic(default_user/1).

convert_to_strings([],[]):-!.
convert_to_strings([IS|T1],[I|ST2]):-convert_to_string(IS,I),!,convert_to_strings(T1,ST2).

term_to_string(I,IS):- catch(string_to_atom(IS,I),_,(term_to_atom(I,A),string_to_atom(IS,A))),!.

convert_to_string(I,ISO):-
                term_to_string(I,IS),!,
		string_to_list(IS,LIST),!,
		list_replace(LIST,92,[92,92],LISTM),
		list_replace(LISTM,34,[92,34],LISTO),
		string_to_atom(ISO,LISTO),!.

list_replace(List,Char,Replace,NewList):-
	append(Left,[Char|Right],List),
	append(Left,Replace,NewLeft),
	list_replace(Right,Char,Replace,NewRight),
	append(NewLeft,NewRight,NewList),!.
list_replace(List,_Char,_Replace,List):-!.

nick("bear_afk", "n=bear@pool-72-78-164-30.phlapa.east.verizon.net", "*", "#swhack", "bear").
pubm(Agent, HOSTAMSK,TYPE,Channel,MESSAGE):- from(Channel,Agent,say(MESSAGE)).
part(USER, HOSTAMSK,TYPE,DEST,MESSAGE):- !.
join(USER, HOSTAMSK,TYPE,DEST):- !.
rejoin(USER, HOSTAMSK,TYPE,DEST):- !.
mode("", "irc.freenode.net", "*", "#swhack", "+v", "dahut").
split("CrashChaos", "n=crash@s079.wh-sproll.uni-ulm.de", "*", "#prolog").
topc("*", "*", "*", "#logicmoo", "").
chof("dmiles", "12").
chon("dmiles", "12").
notc("NickServ","NickServ@services.","*","Password accepted - you are now recognized","jllykifsh").

%same_str(X,Y):-term_to_string(X,XS),term_to_string(Y,YS),toLowercase(XS,XL),toLowercase(YS,YL),!,XL=YL.

to_user_error(CMD):-format(user_error,'~q~n',[CMD]),flush_output(user_error).

from(X,Y,_,Z):-from_irc(X,Y,Z),!.
from(X,Y,Z):-from_irc(X,Y,Z),!.


channel_say(console,Data):-!,write(Data),nl.

%fixup Channel
channel_say('#'(Dest),Data):-atom(Dest),!,atom_codes(Dest,Codes),channel_say([35|Codes],Data).
channel_say(Dest,Data):-atom(Dest),atom_codes(Dest,[34|Codes]),unquoteAtom(Dest,UQuote),!,channel_say(UQuote,Data).
channel_say(Dest,Data):-atom(Dest),!,atom_codes(Dest,Codes),channel_say(Codes,Data).  
%fixup Message
channel_say(Channel,[L|Data]):-is_list(Data),atomic(L),not(number(L)), %[hi,there,bob]
	catch(concat_atom([L|Data],' ',List),_,fail),!,
	channel_say(Channel,List).

channel_say(Channel,[L|Data]):-is_list(Data),atomic(L),number(L), %"hi there bob"
	convert_to_string([L|Data],DataO),!,
	channel_say(Channel,DataO).

%outputs
channel_say(Channel,Data):-atom(Data),!,
	concat_atom(List,'\n',Data),
	channel_say_list(Channel,List).

channel_say(Channel,DataI):- 
        not(is_list(DataI)),
        compound(DataI),
        term_to_atom(DataI,DataA),
        convert_to_string(DataA,DataL),
   channel_say_list(Channel,[DataL]).

channel_say(Channel,DataI):- 
        not(is_string(DataI)),
	convert_to_string(DataI,DataL),!,
   channel_say_list(Channel,[DataL]).

/*
channel_say(Dest,Data):-!,
   once(stdio(Agent,InStream,OutStream)),
	ignore(catch(format(OutStream,'.msg ~s ~w\n',[Dest,Data]),_,fail)),
	mformat(OutStream,'eot.\n',[Dest,Data]),
	flush_output(OutStream),
	flush_output(user_error),!.	
*/

channel_say_list(Channel,[]).
channel_say_list(Channel,[N|L]):-in_html,!,
   ignore(catch(format('~w',[N]),_,fail)),
	channel_say_list(Channel,L),!.
channel_say_list(Channel,[N|L]):-
	once(stdio(Agent,InStream,OutStream)),
   ignore(catch(format(OutStream,'\n.msg ~s ~w\n',[Channel,N]),_,fail)),
	%%ignore(catch(format(OutStream,'\n.tcl putserv "PRIVMSG ~s :~w " ;  return "noerror ."\n',[Channel,N]),_,fail)),	
   ignore(catch(format(OutStream,'\n eot.\n',[]),_,fail)),
   flush_output(OutStream),
   ignore(catch(format(OutStream,'\n.msg jllykifsh ~w\n',[N]),_,fail)),
   flush_output(OutStream),
        channel_say_list(Channel,L),!.

to_egg(X):-to_egg('~w',[X]),!.
to_egg(X,Y):-once(stdio(Agent,InStream,OutStream)),once((sformat(S,X,Y),mformat(OutStream,'~s\n',[S]),!,flush_output(OutStream))).

msgm(_,_,_,From,Said):-from_irc(From,From,say(Said)).

% to_egg('.raw').
eot:-!.	

getRegistered(Channel,Agent,kifbot):-chattingWith(Channel,Agent).
getRegistered("#ai",_,execute):-ignore(fail).
getRegistered("#pigface",_,execute):-ignore(fail).
getRegistered("#logicmoo",_,execute):-ignore(fail).
getRegistered("#kif",_,execute):-ignore(fail).
getRegistered("#rdfig",_,execute):-ignore(fail).
getRegistered("#prolog",_,execute):-ignore(fail).

%%:-dynamic(isRegisteredChk/3).

from_irc(Codes,_,_):-my_name_used(Codes),!.
from_irc(_,Codes,_):-my_name_used(Codes),!.
from_irc("logicmoo",Y,Z):-!.
from_irc(Y,"logicmoo",Z):-!.		       
from_irc([_], _, _):-!.
from_irc(Channel,Agent,Method):-
	set_default_channel(Channel),
	set_default_user(Agent),
        to_user_error(from_irc(Channel,Agent,Method)),
	ircEvent(Channel,Agent,Method).

my_name_used(Codes):-convert_to_string(Codes,IS),
      atom_codes(IS,CODES),toLowercase(CODES,LCodes),
	append(_,[106,108, 108|_],LCodes).

% i hadnt see your quote at first when i posted the wikipedia quote.  i had only seen the part about being up to the compiler to pick the best method if it had to chose between a const and non-const method 
% say(X) :- default_channel(C),channel_say(C,X).
% tcs :- channel_say("#logicmoo",[hi,dmiles_afk,'I will answer you in','"#logicmoo','until you say "goodbye"']).

ircEvent(Channel,Agent,say(W)):-same_str(W,"goodbye"),!,retractall(chattingWith(Channel,Agent)).
ircEvent(Channel,Agent,say(W)):-(same_str(W,"jllykifsh");same_str(W,"jllykifsh?")),!,
		retractall(chattingWith(Channel,Agent)),!,
   asserta(chattingWith(Channel,Agent)),!,
   channel_say(Channel,[hi,Agent,'I will answer you in',Channel,'until you say "goodbye"']).

ircEvent(Channel,Agent,say(Codes)):-
	 once((my_name_used(Codes);isRegisteredChk(Channel,Agent,kifbot))),
	 getWordTokens(Codes,IInput),
         removeStr("jllykifsh",IInput,Input),
	 unsetMooOption(Agent,client=_),setMooOption(Agent,client=consultation),!, 
         doEnglishH(Channel,Agent,Input,Codes).

ircEvent(Channel,Agent,say(Codes)):-
         chattingWith(Channel,Agent),!,
	 getWordTokens(Codes,IInput),!,
         removeStr("jllykifsh",IInput,Input),
	 unsetMooOption(Agent,client=_),setMooOption(Agent,client=consultation),!, 
         doEnglishH(Channel,Agent,Input,Codes).


ircEvent(Channel,Agent,Method):-sendEvent(Channel,Agent,Method).
ircEvent(Channel,Agent,Method):-!.

doEnglishH(Channel,Agent,Input,Codes):-
      thread_create(doEnglishH2(Channel,Agent,Input,Codes),TN,[]),
      thread_create(doEnglishH3(Channel,Agent,Input,Codes),TN2,[]).

doEnglishH2(Channel,Agent,Input,Codes):-
      catch((string_to_atom(Codes,Atom),atom_to_term(Atom,Term,Vars)),_,fail),
      callable(Term),
      to_user_error( callable(Term)),
      catch((Term,fail),E,once(say(E))).
doEnglishH2(Channel,Agent,Input,Codes):- english2Kif(Input,KIF),!,channel_say_list(Channel,[KIF]),!.

doEnglishH3(Channel,Agent,[lookup,WNWord],Codes):-
         wordToWNPOS(CycWord,WNWord,POS),!,
         channel_say_list(Channel,[wordToWNPOS(CycWord,WNWord,POS)]).

doEnglishH3(Channel,Agent,Input,Codes):-
	 idGen(Serial),idGen(Refno),get_time(Time),
	 sendEvent(Channel,Agent,english(phrase(Input,Codes),packet(Channel,Serial,Agent,Refno,Time))).
   

removeStr(Jllykifsh,[],[]):-!.
removeStr(Jllykifsh,[I,Punct|Input],Input):-same_str(I,Jllykifsh),is_punct_str(Punct),!.
removeStr(Jllykifsh,[I|Input],Input):-same_str(I,Jllykifsh),!.
removeStr(Jllykifsh,[I|Input],[I|Onput]):-removeStr(Jllykifsh,Input,Onput).

is_punct_str(Punct):-same_str(",",Punct).
is_punct_str(Punct):-same_str(".",Punct).
is_punct_str(Punct):-same_str(":",Punct).

%set_default_channel(Channel):-retractall(default_channel(_Channel)),asserta(default_channel(Channel)).
%set_default_user(Channel):-retractall(default_user(_Channel)),asserta(default_user(Channel)).
ctcp("nsh", "n=nsh@wikipedia/nsh", "*", "#swhack", "ACTION", "sees start of topic").

%from(Channel,Agent,Method):-once(catch(once(nani_event_from(Channel,Agent,Method)),_,true)),fail.
%from(Channel,Agent,say(String)):- catch(learn_response(Channel,Agent,String),_,fail),fail.

eggdrop:-servantProcessCreate(killable,'Consultation Mode Test (KIFBOT!) OPN Server',consultation_thread(swipl,11444),Id,[]).
%%eggdrop:-consultation_thread(swipl,11444).

mformat(A,B,C):-format(A,B,C).
:-dynamic( end_of_file).

