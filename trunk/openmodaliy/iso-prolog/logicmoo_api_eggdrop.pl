:-include('logicmoo_utils_header.pl').
% ===================================================================
% Conenctors
% ===================================================================
eggdropConnect(In,Out,BotNick,Port):-eggdropConnect(In,Out,'127.0.0.1',Port,BotNick,logicmoo).

eggdropConnect(InStream,OutStream,Host,Port,Agent,Pass):-
       tcp_socket(SocketId),
       tcp_connect(SocketId,Host:Port),
       tcp_open_socket(SocketId, InStream, OutStream),
       format(OutStream,'~w\n',[Agent]),flush_output(OutStream),
       format(OutStream,'~w\n',[Pass]),flush_output(OutStream),
       retractall(stdio(Agent,_,_)),
       asserta((stdio(Agent,InStream,OutStream))),!.
		
consultation_thread(BotNick,Port):- 
      eggdropConnect(In,Out,BotNick,Port),
      to_egg('.echo off\n'),
      to_egg('.console ~w ""\n',[BotNick]),
      to_egg('eot.\n',[]),
      repeat,
      catch(read_term(In,CMD,[variable_names(Vars)]),_,fail),
      catch(CMD,_,true),
      fail.

from(X,Y,_,Z):-from_irc(X,Y,Z),!.
from(X,Y,Z):-from_irc(X,Y,Z),!.


eggdrop_say_to(console,Data):-!,write(Data),nl.
eggdrop_say_to([35|Dest],Data):-!,
	once(stdio(Agent,InStream,OutStream)),
	once(channel_say(OutStream,[35|Dest],Data)),
	format(OutStream,'eot.\n',[Dest,Data]),
	flush_output(OutStream),
	flush_output(user_error),!.
eggdrop_say_to('#'(Dest),Data):-atom(Dest),!,atom_codes(Dest,Codes),eggdrop_say_to([35|Codes],Data).
eggdrop_say_to(Dest,Data):-atom(Dest),!,atom_codes(Dest,Codes),eggdrop_say_to(Codes,Data).
eggdrop_say_to(Dest,Data):-!,
	once(stdio(Agent,InStream,OutStream)),
	ignore(catch(format(OutStream,'.msg ~s ~w\n',[Dest,Data]),_,fail)),
	format(OutStream,'eot.\n',[Dest,Data]),
	flush_output(OutStream),
	flush_output(user_error),!.	
  
channel_say(OutStream,Channel,Data):-atom(Data),!,
	concat_atom(List,'\n',Data),
	channel_say_list(OutStream,Channel,List).

channel_say(OutStream,Channel,Data):-
	channel_say_list(OutStream,Channel,[Data]).

channel_say_list(OutStream,Channel,[]).
channel_say_list(OutStream,Channel,[N|L]):-
	ignore(catch(format(OutStream,'\n.msg ~s ~w\n',[Channel,N]),_,fail)),
	flush_output(OutStream),
	channel_say_list(OutStream,Channel,L),!.

to_egg(X):-to_egg('~w',[X]),!.
to_egg(X,Y):-once(stdio(Agent,InStream,OutStream)),once((sformat(S,X,Y),format(OutStream,'~s\n',[S]),!,flush_output(OutStream))).

eot:-!.	

getRegistered(Channel,Agent,kifbot):-chattingWith(Channel,Agent).
getRegistered("#ai",_,execute):-ignore(fail).
getRegistered("#pigface",_,execute):-ignore(fail).
getRegistered("#logicmoo",_,execute):-ignore(fail).
getRegistered("#kif",_,execute):-ignore(fail).
getRegistered("#rdfig",_,execute):-ignore(fail).
getRegistered("#prolog",_,execute):-ignore(fail).

from_irc("logicmoo",Y,Z):-!.
from_irc(Y,"logicmoo",Z):-!.		       
from_irc([_], _, _):-!.
from_irc(Channel,Agent,Method):-
	set_default_channel(Channel),
	set_default_user(Agent),
      %  writeSTDERR('~q',[from_irc(Channel,Agent,Method)]),
	ircEvent(Channel,Agent,Method).


my_name_in_codes(Codes):-
	toLowercase(Codes,Lower),!,
	my_name_in_lcase_codes(Lower).
	
my_name_in_lcase_codes(Codes):-
	append(_,[106, 108, 108|_],Codes).
my_name_in_lcase_codes(Codes):-
	append(_,[106,_,108, 108|_],Codes).


ircEvent(Channel,Agent,say(W)):-same_str(W,"goodbye"),!,retractall(chattingWith(Channel,Agent)).
ircEvent(Channel,Agent,say(W)):-(same_str(W,"jllykifsh");same_str(W,"jllykifsh?")),!,
		retractall(chattingWith(Channel,Agent)),!,
		asserta(chattingWith(Channel,Agent)),!,
		say([hi,Agent,'I will answer you in',Channel,'until you say "goodbye"']).
ircEvent(Channel,Agent,say(Codes)):-!,
	 once((my_name_in_codes(Codes);isRegisteredChk(Channel,Agent,kifbot))),
	 getCycLTokens(Codes,Input),!,
	 unsetMooOption(Agent,client=_),setMooOption(Agent,client=consultation),!, 
	 idGen(Serial),idGen(Refno),get_time(Time),
	 sendEvent(Channel,Agent,english(phrase(Input,Codes),packet(Channel,Serial,Agent,Refno,Time))).
ircEvent(Channel,Agent,Method):-sendEvent(Channel,Agent,Method).

%from(Channel,Agent,Method):-once(catch(once(nani_event_from(Channel,Agent,Method)),_,true)),fail.
%from(Channel,Agent,say(String)):- catch(learn_response(Channel,Agent,String),_,fail),fail.

:-servantProcessCreate(killable,'Consultation Mode Test (KIFBOT!) OPN Server',consultation_thread(logicmoo,65532),Id,[]).
