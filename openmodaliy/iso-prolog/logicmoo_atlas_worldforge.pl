% ===================================================================
% File 'opencyc_wf.pl'
% Purpose: Lower-level connection based utilities for interfacing to WorldForge from SWI-Prolog
% Maintainer: Douglas Miles
% Contact: $Author: dmiles $@users.sourceforge.net ;
% Version: 'opencyc_wf.pl' 1.0.0
% Revision:  $Revision: 1.1 $
% Revised At:   $Date: 2002-12-03 08:34:15 $
% ===================================================================

/*
:-module(worldforge_atlas,
      [
      wf_connect/1,
      wf_connect/2,
      chose_or_create_character/1,
      wf/1,
      wf/2,
      readPackedTokChars/1,
      flush_read_to_error/0,
      read_to/2,
      writeWF/1,
      writeWF/2,
      writeWF/3,
      charsToProlog/2,
      tokensToProlog/2,
      tokenize_packed/2,
      getLocalSerial/1,
      bachToPackedTok/2,
      packedTokToChars/2,
      blockOnSerial/2,
      bach/1,
      bachToPackedChars/2,
      reactPackedTimes/1,
      best_member/3,
      reactPackedTok/1,
      reactPackedTokChars/1]).
*/
%:- if_swi module_transparent(reactPackedHook/1).
:-dynamic(reactPackedHook/1).
:-multifile(reactPackedHook/1).

:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).


:-dynamic(wf_socket_data/3).
:-dynamic(mooInfo/2).

% ======================================================================
% Conenct to WorldForge server
% ======================================================================


wf_connect(Server):-
   wf_connect(Server,6767).

wf_connect(Server,Port):-
   tcp_socket(SocketId),
   tcp_connect(SocketId,Server:Port),
   retractall(wf_socket_data(_,_,_)),
   tcp_open_socket(SocketId,InStream,OutStream),
   asserta(wf_socket_data(SocketId,InStream,OutStream)),
   writeWF('ATLAS OpenCyc-SemanticWeb\n'),
   writeWF('ICAN Packed\n\n'),!,
   sleep(1),!,
   flush_read_to_error,!.

% ======================================================================
% After logging in Choose or Create an Actor
% ======================================================================

chose_or_create_character(Character):-
	 reactPackedTimes(4),
	 not(mooInfo(player_login, Player)),
	 throw(wf_error('Must Login First')).

chose_or_create_character(Character):-
	not(mooInfo(characters, Actor)),!,
	format(user_error,'% No Characters not found creating ~w.~n',[Character]),
	mooInfo(player_login, PlayerLogin),
	wf(createChar(PlayerLogin,Character)),!.

chose_or_create_character(Character):-
	reactPackedTimes(4),
	mooInfo(characters, _),!,
	findall(Characters,mooInfo(characters, Characters),List),
	format(user_error,'% Characters found: ~q~n',[List]),
	(best_member(Character,List,Name) ->
	    (wf(takeCharacter(Name)),!,format(user_error,'% Assuming: ~q~n',[Name]));
	    (mooInfo(player_login, PlayerLogin),format(user_error,'% Creating: ~q~n',[Character]),wf(createChar(PlayerLogin,Character)))),!.

best_member(Character,List,Character):-member(Character,List),!.
best_member(Character,List,Char):-
      atom_codes(Character,CCCharacter),
      best_member2(CCCharacter,List,Char),!.
best_member2(Character,[H|List],H):-atom_codes(H,HC),subset(Character,HC),!.
best_member2(Character,[_|List],H):-!,best_member2(Character,List,H).

% ======================================================================
% Enact a Highlevel command in WF Server
% ======================================================================
wf(CMD):-wf(CMD,_).
   
wf(login(Agent,Passwd),Result):-
	   getLocalSerial(SerialNo),
      asserta(mooInfo(player_login,Agent)),!,
      bach({serialno:int(SerialNo),arg:{password:Passwd,username:Agent,id:Agent,objtype="obj",parents:["player"]},id=string(""),parents:["login"],objtype:"op"}),!,
      wf(look(Agent)).

wf(createAccount(Agent,Passwd),Result):-
	getLocalSerial(SerialNo),
      bach({from:Agent,serialno:int(SerialNo),id:account,parents:["create"], arg:{id:Character, name:Character, height:1.8}, objtype:"op"}).

wf(login(Agent,Passwd,Character),Result):-
      wf(login(Agent,Passwd),Result),
      chose_or_create_character(Character),
      getLocalSerial(SerialNo),
      bach({from:Agent,serialno:int(SerialNo),parents:["set"], arg:{id:Character, name:Character, height:1.8}, objtype:"op"}).



wf(createChar(PlayerLogin,CharName),Result):-
      wf(create(PlayerLogin,CharName,farmer,[description="A simple swineherd",objtype="obj",sex="male",id=string("")]),_).

wf(takeCharacter(Actor),Result):-
      repeat,
      reactPackedTokChars, 
      mooInfo(player_login, Player),	   
      % look at char from player
      getLocalSerial(SerialNo),
      bach({serialno:int(SerialNo),from:string(Player),arg:{id: string(Actor)},id:'',parents: ["look"],objtype: "op"}),
      asserta(mooInfo(active_characters, Actor)),!,
      % look AS char
      getLocalSerial(SerialNo2),
      bach({serialno:int(SerialNo2),from:string(Actor),id:'',parents: ["look"],objtype: "op"}),!.



wf(look(Target),Result):-
	 getLocalSerial(SerialNo),
	 mooInfo(active_characters, Actor),
      bach({serialno:int(SerialNo),from:string(Actor),arg:{id: Target},id:'',parents: ["look"],objtype: "op"}),!.

	  
wf(deleteInActivesChars,_):-
      mooInfo(active_characters, _),!,
      findall(Actor,(mooInfo(characters, Actor),not(mooInfo(active_characters, Actor))),InActives),!,
      wf(deleteAllChars(InActives),_).

wf(deleteAllChars([]),_).
wf(deleteAllChars([H|T]),_):-
      wf(deleteChar(H),_),
      wf(deleteAllChars(T),_).

wf(deleteChar(Actor),_):-!,
	 mooInfo(active_characters, Agent),
	 wf(set(Agent,Actor,status,float(0.000)),_),
	 retractall(mooInfo(_, Actor)),
	 wf(delete(Actor,Actor),_).

wf(delete(Agent,Target),_):-!,
	 getLocalSerial(SerialNo),
	 bach({serialno:int(SerialNo),from:string(Agent),arg:{id:Target},parents: ["delete"],objtype: "op"}),!.

wf(set(Agent,Target,Name,Value),_):-!,
	 getLocalSerial(SerialNo),
	 bach({serialno:int(SerialNo),from:string(Agent),arg:{id: Target, Name: Value},parents: ["set"],objtype: "op"}),!.

wf(get(Target),_):-!,
       %%  mooInfo(active_characters, Agent),
	 getLocalSerial(SerialNo),
	 bach({serialno:SerialNo,arg:{id:Target},parents: ["get"],objtype: "op"}),!.

wf(move(Agent,Target,Loc,PosList),_):-!,
	 getLocalSerial(SerialNo),
	 bach({serialno:int(SerialNo),from:Agent,arg:{id: Target,loc:Loc, pos: PosList},parents: ["move"],objtype: "op"}),!.
      
wf(talk(Agent,Said),_):-
	 getLocalSerial(SerialNo),
	 bach({serialno:int(SerialNo),from:Agent,arg:{say:string(Said)},parents: ["talk"],objtype: "op"}),!.

wf(say(Said),_):- !,
	 mooInfo(active_characters, Actor),
	 getLocalSerial(SerialNo), !,
	 bach({serialno:int(SerialNo),from:Actor,arg:{say:string(Said)},parents: ["talk"],objtype: "op"}),!.

wf(bach(CMD,Args),_):-
	 getLocalSerial(SerialNo),
	 bach({serialno:int(SerialNo),from:Agent,arg:{in_map(Args)},parents: [string(CMD)],objtype: "op"}),!.

wf(create(Agent,Name,Superclass,AttributeNameValueList),_):-
	 getLocalSerial(SerialNo),
	 bach({serialno:int(SerialNo),from:Agent,arg:{name:Name,parents: [string(Superclass)],in_map(AttributeNameValueList)},parents: ["create"],objtype: "op"}),!.
			


wf(Cmd,Result):-!.

% ======================================================================
% Write to WorldForge server
% ======================================================================

writeWF(Format):-writeWF(Format,[]).
writeWF(Format,Args):-
      wf_socket_data(SocketId,InStream,OutStream),
      writeWF(OutStream,Format,Args).
writeWF(OutStream,Format,Args):-
      format(OutStream,Format,Args),
      flush_output(OutStream),!.
   

% ======================================================================
% Read PackedChars from WorldForge server
% ======================================================================

readPackedTokChars(Data):-	 
	 wf_socket_data(SocketId,InStream,OutStream),
	 read_to(InStream,Data),!.

read_to(InStream,Data):-flag(backet_depth,_,0),
	    flush_read_to(InStream,Data),!.

flush_read_to(InStream,[C|Codes]):-
   get0(InStream,C),reactRead([C],X),!, 
      (X < 2 -> Codes = [] ; flush_read_to(InStream,Codes)).
   
reactRead("[",99):-flag(backet_depth,X,X+1),!.
reactRead("]",X):-flag(backet_depth,X,X-1),!.
reactRead([-1],0).
reactRead(_,99).

% ======================================================================
% Tranlate PackedTok from WorldForge server
% ======================================================================

charsToProlog(Chars,PackedTok):-
	 tokenize_packed(Chars,Tokens),!,
	 writeq(Tokens),nl,
	 tokensToProlog(Tokens,PackedTok),!.
   
tokensToProlog(Tokens,PackedTok):-
   phrase(list_item(PackedTok),Tokens),!.

tokensToProlog(Tokens,PackedTok):-
   phrase(list_items(PackedTok),Tokens),!.

map_items([H|T]) -->  map_item(H),map_items(T).
map_items([])-->[].
list_items([H|T]) -->  list_item(H),list_items(T).
list_items([])-->[].

descapeString(Value,String):-atom(Value),!,atom_codes(Value,Codes),descapeCodes(Codes,Desc),!,atom_codes(String,Desc),!.
descapeString(Value,String):-is_list(Value),!,descapeCodes(Value,Desc),!,atom_codes(String,Desc),!.
descapeString(String,String).


descapeCodes([],[]).
descapeCodes([43,L,R|Codes],[New|Desc]):-lrn(L,R,New),descapeCodes(Codes,Desc),!.
descapeCodes([C|Codes],[C|Desc]):-descapeCodes(Codes,Desc),!.

lrn(L,R,New):-ascii_hex(L,LV),ascii_hex(R,RV),!,New is LV*16+RV.
ascii_hex(R,RV):-R<58,RV is R-48.
ascii_hex(R,RV):-R<103,RV is R-97 +10.

map_item(Name:(String)) --> ['$',Name,=,Value],{notSpecial(Name,Value),descapeString(Value,String),!}.
map_item(Name:('')) --> ['$',Name,=].
map_item(Name:int(Int)) --> ['@',Name,=,Value],{notSpecial(Name,Value),atom_to_term(Value,Int,_)}.
map_item(Name:int(0)) --> ['@',Name,=].
map_item(Name:float(Float)) --> ['#',Name,=,Value],{notSpecial(Name,Value),atom_to_term(Value,Float,_)}.
map_item(Name:float(0.0)) --> ['#',Name,=].
map_item(Name:list([])) -->['(',Name,=,')'].
map_item(Name:list(Value)) -->['(',Name,=],list_items(Value),[')'],{notSpecial(Name)}.
map_item(Name:map([])) --> [Name,=,'[',']'].
map_item(Name:map(Value)) --> [Name,=,'['],map_items(Value),[']'],{notSpecial(Name)}.


list_item(map(Value)) --> ['['],map_items(Value),[']'].
list_item((String)) -->['$',Value],{notSpecial(Value),descapeString(Value,String),!}.
list_item(int(Int)) -->['@',Value],{notSpecial(Value),atom_to_term(Value,Int,_)}.
list_item(float(Float)) -->['#',Value],{notSpecial(Value),atom_to_term(Value,Float,_)}.
list_item(list(Value)) -->['('],list_items(Value),[')'].


notSpecial(H,H2):-notSpecial(H2),notSpecial(H),!.
notSpecial(X):-not(member(X,['=','@','#','$','(',')','[',']'])),!.

% TOKENIZATION                          
tokenize_packed([],[]).
tokenize_packed([32|T],O):-tokenize_packed(T,O),!.

tokenize_packed([C|List],[A|Out]):-
      member(C,"=@#$()[]"),!,atom_codes(A,[C]),
      tokenize_packed(List,Out),!.

tokenize_packed(List,[Token|More]):-
   get_token(List,Token,MoreChars),
   tokenize_packed(MoreChars,More),!.

get_token(List,Token,MoreChars):-
   get_tokens(List,[],Token,MoreChars),!.

get_tokens([C|List],Stack,Token,[C|List]):-
   member(C,"=@#$()[]"),
   reverse(Stack,TokenChars),
   tokenize_lit(TokenChars,Token),!.

get_tokens([C|Chars],Stack,Token,MoreChars):-
   get_tokens(Chars,[C|Stack],Token,MoreChars),!.

tokenize_lit(TokenChars,Token):-atom_codes(Token,TokenChars).

% ======================================================================
% Local Serial for WorldForge server
% ======================================================================

getLocalSerial(X):-flag(getLocalSerial,X,X+1).

% ======================================================================
% Create player on WorldForge server
% ======================================================================

:-getLocalSerial(ID).
:-getLocalSerial(ID).
:-getLocalSerial(ID).


% ======================================================================
% Convert Bach to Packed Chars
% ======================================================================

bachToPackedChars(Bach,PackedChars):-
      debugOnFailure(bachToPackedTok(Bach,PackedTok)),!,
      debugOnFailure(packedTokToChars(PackedTok,PackedChars)).
      
% ======================================================================
% Convert Bach to Packed Tokens
% ======================================================================

bachToPackedTok([],[]):-!.
bachToPackedTok(arg:V,O):-bachToPackedTok(args:V,O),!.
bachToPackedTok(N=V,O):-!,bachToPackedTok(N:V,O),!.

bachToPackedTok(N:String,N:string(String)):-local_is_string(String),!.
bachToPackedTok(N:Atom,N:string(String)):-atom(Atom),string_to_atom(String,Atom),!.
bachToPackedTok(N:V,N:VO):-!,bachToPackedTok(V,VO),!.
bachToPackedTok(B,P):-bachToPackedTokD(B,P),!.
bachToPackedTok('{}',map([])):-!.
bachToPackedTok('[]',list([])):-!.
bachToPackedTok('{}'(Map),map(Out)):-!,comma_to_list(Map,List),!,bachToPackedTokS(List,Out),!.
bachToPackedTok([H|T],list(Out)):-!,bachToPackedTokS([H|T],Out),!.
bachToPackedTok(list(List),list(Out)):-!,bachToPackedTokS(List,Out),!.
bachToPackedTok(map(List),map(Out)):-!,bachToPackedTokS(List,Out),!.
bachToPackedTok(Rest,Rest):-compound(Rest),!.
bachToPackedTok(Rest,string(Rest)):-!.

bachToPackedTokD(Var,int(0)):-var(Var),!,writeSTDERROnDebug(bachToPackedTokD),!.
bachToPackedTokD(int(Int),int(Int)):-!.
bachToPackedTokD(float(Int),float(Int)):-!.
bachToPackedTokD(string(Int),string(Int)):-!.
bachToPackedTokD(list(List),list(Out)):-!,bachToPackedTokS(List,Out),!.
bachToPackedTokD(map(List),map(Out)):-!,bachToPackedTokS(List,Out),!.

bachToPackedTokD(Str,string(Str)):-local_is_string(Str),!.
bachToPackedTokD(Int,int(Int)):-integer(Int),!.
bachToPackedTokD(Num,float(Num)):-number(Num),!.
bachToPackedTokD(Atom,string(String)):-atom(Atom),string_to_atom(String,Atom),!.


      
bachToPackedTokS([],[]).
bachToPackedTokS([H|T],[HH|TT]):-bachToPackedTok(H,HH),bachToPackedTokS(T,TT),!.



comma_to_list((in_map(A),B),All):-comma_to_list(B,BB),!,append(A,BB,All).
comma_to_list((A,B),[A|BB]):-comma_to_list(B,BB),!. 
comma_to_list(in_map(A),A).
comma_to_list(A,[A]).

local_is_string([A,B|_]):-integer(A),A>12,A<129,integer(B),B>12,B<129.
local_is_string(X):-string(X).

% ======================================================================
% Generate Packed Chars from Packed Tokens
% ======================================================================

packedTokToChars(Var,'Var'):-var(Var),!.
packedTokToChars(N=V,Out):-!,packedTokToChars(N:V,Out),!.

packedTokToChars(map(List),Out):-!,packedTokToCharsS(List,Map),!,sformat(Out,'[~w]',[Map]).
packedTokToChars(list(List),Out):-!,packedTokToCharsS(List,Map),!,sformat(Out,'(~w)',[Map]).
packedTokToChars(string(S),Out):-!,packetString(S,SO),sformat(Out,'$~w',[SO]).
packedTokToChars(int(S),Out):-!,sformat(Out,'@~w',[S]).
packedTokToChars(float(S),Out):-!,sformat(Out,'#~w',[S]).
packedTokToChars(real(S),Out):-!,sformat(Out,'#~w',[S]).
packedTokToChars(Name:map(List),Out):-!,packedTokToCharsS(List,Map),!,sformat(Out,'(~w=[~w])',[Name,Map]).
packedTokToChars(Name:list(List),Out):-!,packedTokToCharsS(List,Map),!,sformat(Out,'(~w=~w)',[Name,Map]).
packedTokToChars(Name:string(S),Out):-!,packetString(S,SO),sformat(Out,'$~w=~w',[Name,SO]).
packedTokToChars(Name:int(S),Out):-!,sformat(Out,'@~w=~w',[Name,S]).
packedTokToChars(Name:real(S),Out):-!,sformat(Out,'#~w=~w',[Name,S]).
packedTokToChars(Name:float(S),Out):-float(S),!,sformat(Out,'#~w=~f',[Name,S]).
packedTokToChars(Name:float(S),Out):-!,sformat(Out,'#~w=~w',[Name,S]).
packedTokToChars(Name:S,Out):-atom(S),!,sformat(Out,'$~w=~w',[Name,S]).
%packedTokToChars(S,Out):-atom(S),!,sformat(Out,'$~w',[S]).
%packedTokToChars(S,Out):-!,sformat(Out,'$~w',[S]).
%packedTokToChars(S,Out):-bachToPackedTok(S,M),!,packedTokToChars(M,Out).



packedTokToCharsS([],'').
packedTokToCharsS([H],O):-packedTokToChars(H,O),!.
packedTokToCharsS([H|T],O):-
      packedTokToChars(H,HO),
      packedTokToCharsS(T,TO),!,
      sformat(O,'~w~w',[HO,TO]).

packetString(In,Out):-
      packetString1(In,Mid),
      packetString2(Mid,Out),!.
   
packetString1([],''):-!.
packetString1('',''):-!.
packetString1([H|T],SO):-sformat(SO,'~s',[[H|T]]),!.
packetString1(S,SO):-sformat(SO,'~w',S),!.

packetString2('','').
packetString2(Mid,Out):-string(Mid),!,string_to_atom(Mid,A),atom_codes(A,Codes),
	 packetString3(Codes,Fixed),atom_codes(Out,Fixed).
packetString2(Mid,Out):-atom(Mid),!,atom_codes(Mid,Codes),
	 packetString3(Codes,Fixed),atom_codes(Out,Fixed).
packetString3([],[]):-!.
packetString3([H|T],Out):-member(H,"+[]()@#$=\n\r"),
      hexCharsFor(H,Hex),
      packetString3(T,TT),append([43|Hex],TT,Out).
packetString3([H|T],[H|TT]):-!,
      packetString3(T,TT).

hexCharsFor(H,Out):-sformat(String,'~16r',[H]),atom_codes(String,Hex),(Hex=[A] -> Out=[48,A] ; Out=Hex),!.
	 
% ======================================================================
% Enact a Bach command on WF Server
% ======================================================================
bach(Bach):-
   bachToPackedChars(Bach,Chars),
   format(user_error,'~q~n.',[bach(Chars)]),
   writeWF(Chars).


% ======================================================================
% Blocks until Serial is recieved
% ======================================================================

blockOnSerial(ID2,nop).

% ======================================================================
% Read but do not react
% ======================================================================

flush_read_to_error:-
   readPackedTokChars(Data),!,
   ((Data= []) ; format(user_error,'~s~n~n',[Data])),!.

% ======================================================================
% Reacts to WF Server event
% ======================================================================

reactPackedTimes(0):-!.
reactPackedTimes(N):-
      reactPackedTokChars,NN is N-1,!,
      reactPackedTimes(NN).


reactPackedTokChars:-
   once(readPackedTokChars(Data)),
   once(reactPackedTokChars(Data)).

reactPackedTokChars([]):-!.

reactPackedTokChars([91|Data]):-!,
     tokenize_packed([91|Data],Tokens),!,
     ( tokensToProlog(Tokens,PackedTok) ->
     reactPackedTok(PackedTok) ;
     format(user_error,'Error ~s~n~n',[[91|Data]])),!.

reactPackedTokChars(Data):-
      format(user_error,'~s.~n~n',[Data]).

reactPackedTok([]).

reactPackedTok([H|T]):-
   reactPackedTok(H),
   reactPackedTok(T),!.



reactPackedTok(map(PackedTok)):-
      member(time_string:'',PackedTok),
      member(objtype:ObjType,PackedTok),
      member(parents:Parents,PackedTok),
      member(args:Args,PackedTok),!,
      member(to:Channel,PackedTok),!,
      member(from:Agent,PackedTok),!,
      member(serialno:Serialno,PackedTok),!,
      member(refno:Refno,PackedTok),!,
      member(seconds:Seconds,PackedTok),!,
      member(future_seconds:Future_seconds,PackedTok),!,
      atom_to_term(Seconds,Secs,_),
      atom_to_term(Future_seconds,FSecs,_),
      Time is Secs + FSecs, !,
      local_removeData([objtype,parents,args,to,from,serialno,refno,future_seconds,seconds,time_string],PackedTok,PackedTokO),!,
      reactPackedTok(msg(ObjType,Parents,Args,packet(Channel,Agent,Serialno,Refno,Time,PackedTokO))).



reactPackedTok(map([Packet|PackedTok])):-
      member(objtype:ObjType,PackedTok),
      member(parents:Parents,PackedTok),
      member(id:Id,PackedTok),!,
      local_removeData([objtype,parents,id],PackedTok,PackedTokO),!,
      reactPackedTok(msg(ObjType,Parents,[id:Id|PackedTokO],Packet)).



reactPackedTok(msg(op,list([X]),list([map(L)]),Packet)):-
    MSG=..[X,L,Packet],
   reactPackedTok(MSG).

reactPackedTok(msg(Type,list([X]),L,Packet)):-
   reactPackedTok(data(Type,X,L,Packet)).
   
reactPackedTok(sight(List,Packet)):-reactPackedTok(map([Packet|List])).			   


reactPackedTok(sound(List,Packet)):-reactPackedTok(map([Packet|List])).


reactPackedTok(info([],Packet)):-!.
reactPackedTok(info([Info|List],Packet)):-!,
      reactPackedTok(info(Info,Packet)),
      reactPackedTok(info(List,Packet)).
reactPackedTok(info(Type:Value,Packet)):-!,
      reactPackedTok(info(Type,Value,Packet)).
reactPackedTok(info(Type,list(Value),Packet)):-!,
      retractall(mooInfo(Type,_)),
      reactPackedTok(info(Type,Value,Packet)).

reactPackedTok(info(Type,[],Packet)):-!.
reactPackedTok(info(Type,[H|T],Packet)):-!,
      reactPackedTok(info(Type,H,Packet)),
      reactPackedTok(info(Type,T,Packet)).

reactPackedTok(info(Type,H,Packet)):-
   reactPackedTok(asserta(mooInfo(Type,H))).
         
reactPackedTok(asserta(mooInfo(Type,H))):-
   format(user_error,'~q~n',[asserta(mooInfo(Type,H))]),
   asserta(mooInfo(Type,H)).



reactPackedTok(set([id:Thing, Var:Value], Packet)):-reactPackedTok(set(Thing,Var=Value,Packet)).
reactPackedTok(appearance([id:Thing, loc:Value], Packet)):-reactPackedTok(appearance(Thing,Value,Packet)).
reactPackedTok(disappearance([id:Thing, loc:Value], Packet)):-reactPackedTok(disappearance(Thing,Value,Packet)).
reactPackedTok(talk([say:Value], packet(Channel,Agent,Refno,Serialno,Seconds,PackedTokO))):-
      reactPackedTok(talk(Agent,Value,packet(Channel,Agent,Refno,Serialno,Seconds,PackedTokO))).

reactPackedTok(talk([say:Value], packet(Channel,Agent,Refno,Serialno,Seconds,PackedTokO))):-
      reactPackedTok(talk(Agent,Value,packet(Channel,Agent,Refno,Serialno,Seconds,PackedTokO))).

reactPackedTok(set(Agent, status=Num,_)).

reactPackedTok(X):-
      reactPackedHook(X),!.
   
reactPackedTok(X):-
   format(user_error,'% reactPackedTok(~q).~n',[X]).

% ======================================================================
% Remove cached attribs
% ======================================================================

local_removeData(Attribs,[],[]).
local_removeData(Attribs,[In:_|InS],Out):-
	   member(In,Attribs),!,local_removeData(Attribs,InS,Out).
local_removeData(Attribs,[In|InS],[In|Out]):-
	 local_removeData(Attribs,InS,Out).


end_of_file.

create account dmiles ttt
create settler nephreal
look


end_of_file.


C: [@serialno=1(args=[$password=pass$id=testJava$username=testJava$objtype=obj$name=testJava(parents=$player)])$id=(parents=$login)$objtype=op]
S: [@serialnoo=3715@refno=1$from=$to=#seconds=0#future_seconds=0$time_string=(args=[$message=Login is invalid][(args=[$id=testJava$name=testJava$objtype=obj(parents=$player)$password=pass$username=testJava])$from=#future_seconds=0$id=$name=$objtype=op(parents=$login)@refno=0#seconds=0@serialno=1$time_string=$to=])(parents=$error)$id=$objtype=op$name=]

C: [@serialno=2(args=[$password=pass$id=testJava$username=testJava$objtype=obj$name=testJava(parents=$player)])$id=(parents=$create)$objtype=op]
S: [@serialnoo=3717@refno=2$from=$to=#seconds=0#future_seconds=0$time_string=(args=[(character_types=$farmer$orc$settler)(characters=)$id=testJava$name=testJava$objtype=object(parents=$player)$password=pass$username=testJava])(parents=$info)$id=$objtype=op$name=]

C: [@serialno=3$from=testJava(args=[$description=A simple swineherd$id=$objtype=obj$name=nephrael(parents=$orc)$sex=male])$id=(parents=$create)$objtype=op]
		    %[@serialno=14720078@refno=0$from=$to=guest#seconds=0#future_seconds=0$time_string=(args=[$id=lobby$name=lobby$objtype=object(parents=$room)(people=$guest$admin)(rooms=)])(parents=$sight)$id=sight$objtype=op$name=]
		    % [@serialno=14720079@refno=0$from=$to=guest#seconds=0#future_seconds=0$time_string=(args=[(characters=$TestCharacter2_26941)$id=guest$objtype=object(parents=$player)$password=guest])(parents=$sight)$id=sight$objtype=op$name=]',[]).
S: [@serialnoo=3722@refno=3$from=$to=#seconds=0#future_seconds=0$time_string=(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)$description=A simple swineherd$id=nephrael_67$loc=world_0#mass=60$mode=birth$name=nephrael$objtype=obj(parents=$orc)(pos=#0#0#0)$sex=male#stamp=0#status=1$type=orc])(parents=$info)$id=$objtype=op$name=]
S: [@serialnoo=3723@refno=3721$from=nephrael_67$to=nephrael_67#seconds=75425.6#future_seconds=0$time_string=(args=)(parents=$setup)$id=setup$objtype=op$name=$sub_to=mind]
S: [@serialnoo=3728@refno=0$from=nephrael_67$to=nephrael_67#seconds=75425.6#future_seconds=0$time_string=(args=[(args=[$id=coin_68$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=75425.6@serialno=0$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3729@refno=3727$from=coin_68$to=nephrael_67#seconds=75425.6#future_seconds=0$time_string=(args=[(args=[$id=coin_68$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=coin_68#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=0@serialno=0$time_string=$to=coin_68])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3732@refno=0$from=nephrael_67$to=nephrael_67#seconds=75425.6#future_seconds=0$time_string=(args=[(args=[$id=coin_69$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=75425.6@serialno=0$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3733@refno=3731$from=coin_69$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_69$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=coin_69#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=0@serialno=0$time_string=$to=coin_69])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3736@refno=0$from=nephrael_67$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_70$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=75425.6@serialno=0$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3737@refno=3735$from=coin_70$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_70$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=coin_70#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=0@serialno=0$time_string=$to=coin_70])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3740@refno=0$from=nephrael_67$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_71$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=75425.6@serialno=0$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3741@refno=3739$from=coin_71$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_71$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=coin_71#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=0@serialno=0$time_string=$to=coin_71])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3744@refno=0$from=nephrael_67$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_72$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=75425.6@serialno=0$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3745@refno=3743$from=coin_72$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_72$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=coin_72#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=0@serialno=0$time_string=$to=coin_72])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3748@refno=0$from=nephrael_67$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_73$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=75425.6@serialno=0$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3749@refno=3747$from=coin_73$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_73$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=coin_73#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=0@serialno=0$time_string=$to=coin_73])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3752@refno=0$from=nephrael_67$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_74$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=75425.6@serialno=0$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3753@refno=3751$from=coin_74$to=nephrael_67#seconds=75425.7#future_seconds=0$time_string=(args=[(args=[$id=coin_74$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=coin_74#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=0@serialno=0$time_string=$to=coin_74])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3756@refno=0$from=nephrael_67$to=nephrael_67#seconds=75425.8#future_seconds=0$time_string=(args=[(args=[$id=coin_75$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=75425.6@serialno=0$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3758@refno=0$from=nephrael_67$to=nephrael_67#seconds=75425.8#future_seconds=0$time_string=(args=[(args=[$id=coin_76$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=75425.6@serialno=0$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3760@refno=0$from=nephrael_67$to=nephrael_67#seconds=75425.8#future_seconds=0$time_string=(args=[(args=[$id=coin_77$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=75425.6@serialno=0$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3761@refno=3755$from=coin_75$to=nephrael_67#seconds=75425.8#future_seconds=0$time_string=(args=[(args=[$id=coin_75$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=coin_75#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=0@serialno=0$time_string=$to=coin_75])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3763@refno=3724$from=world_0$to=nephrael_67#seconds=75425.8#future_seconds=0$time_string=(args=[(contains=$giwo_45$Thomas__Achtner_1$gfire1_12$munin_23$Giwo_34$nephrael_56$coin_61$nephrael_67)$id=world_0$objtype=object(parents=$world)])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3764@refno=3725$from=nephrael_67$to=nephrael_67#seconds=75425.8#future_seconds=0$time_string=(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)(contains=$coin_68$coin_69$coin_70$coin_71$coin_72$coin_73$coin_74$coin_75$coin_76$coin_77)$description=A simple swineherd$id=nephrael_67$loc=world_0#mass=60$mode=birth$name=nephrael$objtype=obj(parents=$orc)(pos=#0#0#0)$sex=male#stamp=0#status=1$type=orc])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3767@refno=3757$from=coin_76$to=nephrael_67#seconds=75426.1#future_seconds=0$time_string=(args=[(args=[$id=coin_76$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=coin_76#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=0@serialno=0$time_string=$to=coin_76])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3769@refno=3759$from=coin_77$to=nephrael_67#seconds=75426.1#future_seconds=0$time_string=(args=[(args=[$id=coin_77$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])$from=coin_77#future_seconds=0$id=$name=$objtype=op(parents=$create)@refno=0#seconds=0@serialno=0$time_string=$to=coin_77])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3777@refno=3765$from=nephrael_67$to=nephrael_67#seconds=75426.1#future_seconds=0$time_string=(args=[(args=[$id=nephrael_67#status=0.999])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3726#seconds=75425.8@serialno=3765$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]

C: [@serialno=4$from=nephrael_67$id=(parents=$look)$objtype=op]
S: [@serialnoo=3778@refno=4$from=world_0$to=nephrael_67#seconds=75426.2#future_seconds=0$time_string=(args=[(contains=$giwo_45$Thomas__Achtner_1$gfire1_12$munin_23$Giwo_34$nephrael_56$coin_61$nephrael_67)$id=world_0$objtype=object(parents=$world)])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=5$from=nephrael_67(args=[$id=coin_68])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3779@refno=5$from=coin_68$to=nephrael_67#seconds=75431#future_seconds=0$time_string=(args=[$id=coin_68$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=6$from=nephrael_67(args=[$id=coin_69])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3780@refno=6$from=coin_69$to=nephrael_67#seconds=75431#future_seconds=0$time_string=(args=[$id=coin_69$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=7$from=nephrael_67(args=[$id=coin_70])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3781@refno=7$from=coin_70$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[$id=coin_70$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=8$from=nephrael_67(args=[$id=coin_71])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3782@refno=8$from=coin_71$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[$id=coin_71$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=9$from=nephrael_67(args=[$id=coin_72])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3783@refno=9$from=coin_72$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[$id=coin_72$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=10$from=nephrael_67(args=[$id=coin_73])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3784@refno=10$from=coin_73$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[$id=coin_73$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=11$from=nephrael_67(args=[$id=coin_74])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3785@refno=11$from=coin_74$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[$id=coin_74$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=12$from=nephrael_67(args=[$id=coin_75])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3786@refno=12$from=coin_75$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[$id=coin_75$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=13$from=nephrael_67(args=[$id=coin_76])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3787@refno=13$from=coin_76$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[$id=coin_76$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=14$from=nephrael_67(args=[$id=coin_77])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3788@refno=14$from=coin_77$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[$id=coin_77$loc=nephrael_67#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=15$from=nephrael_67(args=[$id=giwo_45])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3789@refno=15$from=giwo_45$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)(contains=$coin_46$coin_47$coin_48$coin_49$coin_50$coin_51$coin_52$coin_53$coin_54$coin_55)$description=a friendly guy$id=giwo_45$loc=world_0#mass=60$mode=birth$name=giwo$objtype=object(parents=$farmer)(pos=#0#0#0)$sex=male#stamp=61#stamp_contains=0#status=0.939$type=farmer])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=16$from=nephrael_67(args=[$id=Thomas__Achtner_1])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3790@refno=16$from=Thomas__Achtner_1$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)(contains=$coin_2$coin_3$coin_4$coin_5$coin_6$coin_7$coin_8$coin_9$coin_10$coin_11)$description=a friendly guy$id=Thomas__Achtner_1$loc=world_0#mass=60$mode=birth$name=Thomas  Achtner$objtype=object(parents=$farmer)(pos=#0#0#0)$sex=male#stamp=206#stamp_contains=0#status=0.794$type=farmer])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=17$from=nephrael_67(args=[$id=gfire1_12])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3791@refno=17$from=gfire1_12$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)(contains=$coin_13$coin_14$coin_15$coin_16$coin_17$coin_18$coin_19$coin_20$coin_21$coin_22)$description=$id=gfire1_12$loc=world_0#mass=60$mode=birth$name=gfire1$objtype=obj(orientation=#0#-0#0.834468#0.551056)(parents=$farmer)(pos=#-6.70635#-7.84921#0)$sex=male#stamp=241#status=0.798$type=farmer(velocity=#0#0#0)])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=18$from=nephrael_67(args=[$id=munin_23])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3792@refno=18$from=munin_23$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)(contains=$coin_33$coin_24$coin_32$coin_25$coin_26$coin_27$coin_28$coin_29$coin_30$coin_31)$description=munin$id=munin_23$loc=world_0#mass=60$mode=birth$name=munin$objtype=object(parents=$farmer)(pos=#0#0#0)$sex=male#stamp=172#stamp_contains=0#status=0.828$type=farmer])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=19$from=nephrael_67(args=[$id=Giwo_34])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3793@refno=19$from=Giwo_34$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)(contains=$coin_43$coin_42$coin_35$coin_36$coin_37$coin_44$coin_38$coin_39$coin_40$coin_41)$description=a friendly guy$id=Giwo_34$loc=world_0#mass=60$mode=birth$name=Giwo$objtype=object(parents=$farmer)(pos=#0#0#0)$sex=male#stamp=62#stamp_contains=0#status=0.938$type=farmer])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=20$from=nephrael_67(args=[$id=nephrael_56])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3794@refno=20$from=nephrael_56$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)(contains=$coin_57$coin_58$coin_59$coin_60$coin_62$coin_63$coin_64$coin_65$coin_66)$description=A simple swineherd$id=nephrael_56$loc=world_0#mass=60$mode=birth$name=nephrael$objtype=obj(orientation=#0#0#-0.70211#0.712069)(parents=$farmer)(pos=#-6.93095#2.09762#0)$sex=female#stamp=72#status=0.983$type=farmer(velocity=#0#0#0)])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=21$from=nephrael_67(args=[$id=coin_61])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3795@refno=21$from=coin_61$to=nephrael_67#seconds=75431.1#future_seconds=0$time_string=(args=[$id=coin_61$loc=world_0#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#-7.02857#3.02857#0)#stamp=1#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=22$from=nephrael_67(args=[$id=coin_46])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3796@refno=22$from=coin_46$to=nephrael_67#seconds=75432.1#future_seconds=0$time_string=(args=[$id=coin_46$loc=giwo_45#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=23$from=nephrael_67(args=[$id=coin_47])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3797@refno=23$from=coin_47$to=nephrael_67#seconds=75432.1#future_seconds=0$time_string=(args=[$id=coin_47$loc=giwo_45#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=24$from=nephrael_67(args=[$id=coin_48])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3798@refno=24$from=coin_48$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_48$loc=giwo_45#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=25$from=nephrael_67(args=[$id=coin_49])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3799@refno=25$from=coin_49$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_49$loc=giwo_45#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=26$from=nephrael_67(args=[$id=coin_50])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3800@refno=26$from=coin_50$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_50$loc=giwo_45#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=27$from=nephrael_67(args=[$id=coin_51])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3801@refno=27$from=coin_51$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_51$loc=giwo_45#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=28$from=nephrael_67(args=[$id=coin_52])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3802@refno=28$from=coin_52$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_52$loc=giwo_45#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=29$from=nephrael_67(args=[$id=coin_53])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3803@refno=29$from=coin_53$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_53$loc=giwo_45#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=30$from=nephrael_67(args=[$id=coin_54])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3804@refno=30$from=coin_54$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_54$loc=giwo_45#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=31$from=nephrael_67(args=[$id=coin_55])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3805@refno=31$from=coin_55$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_55$loc=giwo_45#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=32$from=nephrael_67(args=[$id=coin_2])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3806@refno=32$from=coin_2$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_2$loc=Thomas__Achtner_1#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=33$from=nephrael_67(args=[$id=coin_3])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3807@refno=33$from=coin_3$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_3$loc=Thomas__Achtner_1#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=34$from=nephrael_67(args=[$id=coin_4])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3808@refno=34$from=coin_4$to=nephrael_67#seconds=75432.2#future_seconds=0$time_string=(args=[$id=coin_4$loc=Thomas__Achtner_1#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=35$from=nephrael_67(args=[$id=coin_5])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3809@refno=35$from=coin_5$to=nephrael_67#seconds=75432.3#future_seconds=0$time_string=(args=[$id=coin_5$loc=Thomas__Achtner_1#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=36$from=nephrael_67(args=[$id=coin_6])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3810@refno=36$from=coin_6$to=nephrael_67#seconds=75432.3#future_seconds=0$time_string=(args=[$id=coin_6$loc=Thomas__Achtner_1#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=37$from=nephrael_67(args=[$id=coin_7])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3811@refno=37$from=coin_7$to=nephrael_67#seconds=75432.3#future_seconds=0$time_string=(args=[$id=coin_7$loc=Thomas__Achtner_1#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=38$from=nephrael_67(args=[$id=coin_8])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3812@refno=38$from=coin_8$to=nephrael_67#seconds=75432.3#future_seconds=0$time_string=(args=[$id=coin_8$loc=Thomas__Achtner_1#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=39$from=nephrael_67(args=[$id=coin_9])$id=(parents=$look)$objtype=op]
S: [@serialnoo=3813@refno=39$from=coin_9$to=nephrael_67#seconds=75432.3#future_seconds=0$time_string=(args=[$id=coin_9$loc=Thomas__Achtner_1#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
C: [@serialno=40$from=nephrael_67(args=[$id=coin_10])$id=(parents=$look)$objtype=op]
C: [@serialno=41$from=nephrael_67(args=[$id=coin_11])$id=(parents=$look)$objtype=op]
C: [@serialno=42$from=nephrael_67(args=[$id=coin_13])$id=(parents=$look)$objtype=op]
C: [@serialno=43$from=nephrael_67(args=[$id=coin_14])$id=(parents=$look)$objtype=op]
C: [@serialno=44$from=nephrael_67(args=[$id=coin_15])$id=(parents=$look)$objtype=op]
C: [@serialno=45$from=nephrael_67(args=[$id=coin_16])$id=(parents=$look)$objtype=op]
C: [@serialno=46$from=nephrael_67(args=[$id=coin_17])$id=(parents=$look)$objtype=op]
C: [@serialno=47$from=nephrael_67(args=[$id=coin_18])$id=(parents=$look)$objtype=op]
C: [@serialno=48$from=nephrael_67(args=[$id=coin_19])$id=(parents=$look)$objtype=op]
C: [@serialno=49$from=nephrael_67(args=[$id=coin_20])$id=(parents=$look)$objtype=op]
C: [@serialno=50$from=nephrael_67(args=[$id=coin_21])$id=(parents=$look)$objtype=op]
C: [@serialno=51$from=nephrael_67(args=[$id=coin_22])$id=(parents=$look)$objtype=op]
C: [@serialno=52$from=nephrael_67(args=[$id=coin_33])$id=(parents=$look)$objtype=op]
C: [@serialno=53$from=nephrael_67(args=[$id=coin_24])$id=(parents=$look)$objtype=op]
C: [@serialno=54$from=nephrael_67(args=[$id=coin_32])$id=(parents=$look)$objtype=op]
C: [@serialno=55$from=nephrael_67(args=[$id=coin_25])$id=(parents=$look)$objtype=op]
C: [@serialno=56$from=nephrael_67(args=[$id=coin_26])$id=(parents=$look)$objtype=op]
C: [@serialno=57$from=nephrael_67(args=[$id=coin_27])$id=(parents=$look)$objtype=op]
C: [@serialno=58$from=nephrael_67(args=[$id=coin_28])$id=(parents=$look)$objtype=op]
C: [@serialno=59$from=nephrael_67(args=[$id=coin_29])$id=(parents=$look)$objtype=op]
C: [@serialno=60$from=nephrael_67(args=[$id=coin_30])$id=(parents=$look)$objtype=op]
C: [@serialno=61$from=nephrael_67(args=[$id=coin_31])$id=(parents=$look)$objtype=op]
C: [@serialno=62$from=nephrael_67(args=[$id=coin_43])$id=(parents=$look)$objtype=op]
C: [@serialno=63$from=nephrael_67(args=[$id=coin_42])$id=(parents=$look)$objtype=op]
C: [@serialno=64$from=nephrael_67(args=[$id=coin_35])$id=(parents=$look)$objtype=op]
C: [@serialno=65$from=nephrael_67(args=[$id=coin_36])$id=(parents=$look)$objtype=op]
C: [@serialno=66$from=nephrael_67(args=[$id=coin_37])$id=(parents=$look)$objtype=op]
C: [@serialno=67$from=nephrael_67(args=[$id=coin_44])$id=(parents=$look)$objtype=op]
C: [@serialno=68$from=nephrael_67(args=[$id=coin_38])$id=(parents=$look)$objtype=op]
C: [@serialno=69$from=nephrael_67(args=[$id=coin_39])$id=(parents=$look)$objtype=op]
C: [@serialno=70$from=nephrael_67(args=[$id=coin_40])$id=(parents=$look)$objtype=op]
C: [@serialno=71$from=nephrael_67(args=[$id=coin_41])$id=(parents=$look)$objtype=op]
C: [@serialno=72$from=nephrael_67(args=[$id=coin_57])$id=(parents=$look)$objtype=op]
C: [@serialno=73$from=nephrael_67(args=[$id=coin_58])$id=(parents=$look)$objtype=op]
C: [@serialno=74$from=nephrael_67(args=[$id=coin_59])$id=(parents=$look)$objtype=op]
C: [@serialno=75$from=nephrael_67(args=[$id=coin_60])$id=(parents=$look)$objtype=op]
C: [@serialno=76$from=nephrael_67(args=[$id=coin_62])$id=(parents=$look)$objtype=op]
C: [@serialno=77$from=nephrael_67(args=[$id=coin_63])$id=(parents=$look)$objtype=op]
C: [@serialno=78$from=nephrael_67(args=[$id=coin_64])$id=(parents=$look)$objtype=op]
C: [@serialno=79$from=nephrael_67(args=[$id=coin_65])$id=(parents=$look)$objtype=op]
C: [@serialno=80$from=nephrael_67(args=[$id=coin_66])$id=(parents=$look)$objtype=op]
C: [@serialno=81$from=nephrael_67(args=[$id=nephrael_67(pos=#-1.4904762506484985#-1.1761904954910278#0.0)$loc=world_0])$id=nephrael_67(parents=$move)$objtype=op]
C: [@serialno=82$from=nephrael_67(args=[$say= waaah$style=shout])$id=(parents=$talk)$objtype=op]
C: [@serialno=83$from=nephrael_67(args=[$say= shhh$style=whisper])$id=(parents=$talk)$objtype=op]
C: [@serialno=85$from=nephrael_67(args=[$id=coin_72(pos=#-1.4904799461364746#-1.1761900186538696#0.0)$loc=world_0])$id=coin_72(parents=$move)$objtype=op]
C: [@serialno=86$from=nephrael_67(args=[$id=nephrael_67(pos=#-2.2134921550750732#-0.8420634865760803#0.0)$loc=world_0])$id=nephrael_67(parents=$move)$objtype=op]
C: [@serialno=87$from=nephrael_67(args=[$id=coin_61])$id=coin_61(parents=$touch)$objtype=op]
C: [@serialno=88$from=nephrael_67(args=[$id=nephrael_67(pos=#-6.943650722503662#1.9992064237594604#0.0)$loc=world_0])$id=nephrael_67(parents=$move)$objtype=op]
C: [@serialno=89$from=nephrael_67(args=[$description=jumps$id=emote$loc=world_0])$id=(parents=$imaginary)$objtype=op]
C: [@serialno=90$from=nephrael_67(args=[$say=asd])$id=(parents=$talk)$objtype=op]
C: [@serialno=91$from=nephrael_67(args=[(velocity=#0.0#0.0#0.0)$id=nephrael_67$loc=world_0])$id=nephrael_67(parents=$move)$objtype=op]
C: [@serialno=92$from=testJava$id=testJava(parents=$logout)$objtype=op] 
S: [@serialnoo=3814@refno=40$from=coin_10$to=nephrael_67#seconds=75432.3#future_seconds=0$time_string=(args=[$id=coin_10$loc=Thomas__Achtner_1#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3815@refno=41$from=coin_11$to=nephrael_67#seconds=75432.3#future_seconds=0$time_string=(args=[$id=coin_11$loc=Thomas__Achtner_1#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3816@refno=42$from=coin_13$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_13$loc=gfire1_12#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3817@refno=43$from=coin_14$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_14$loc=gfire1_12#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3818@refno=44$from=coin_15$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_15$loc=gfire1_12#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3819@refno=45$from=coin_16$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_16$loc=gfire1_12#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3820@refno=46$from=coin_17$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_17$loc=gfire1_12#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3821@refno=47$from=coin_18$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_18$loc=gfire1_12#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3822@refno=48$from=coin_19$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_19$loc=gfire1_12#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3823@refno=49$from=coin_20$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_20$loc=gfire1_12#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3824@refno=50$from=coin_21$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_21$loc=gfire1_12#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3825@refno=51$from=coin_22$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_22$loc=gfire1_12#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3826@refno=52$from=coin_33$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_33$loc=munin_23#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3827@refno=53$from=coin_24$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_24$loc=munin_23#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3828@refno=54$from=coin_32$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_32$loc=munin_23#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3829@refno=55$from=coin_25$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_25$loc=munin_23#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3830@refno=56$from=coin_26$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_26$loc=munin_23#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3831@refno=57$from=coin_27$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_27$loc=munin_23#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3832@refno=58$from=coin_28$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_28$loc=munin_23#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3833@refno=59$from=coin_29$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_29$loc=munin_23#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3834@refno=60$from=coin_30$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_30$loc=munin_23#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3835@refno=61$from=coin_31$to=nephrael_67#seconds=75432.4#future_seconds=0$time_string=(args=[$id=coin_31$loc=munin_23#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3836@refno=62$from=coin_43$to=nephrael_67#seconds=75432.5#future_seconds=0$time_string=(args=[$id=coin_43$loc=Giwo_34#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3837@refno=63$from=coin_42$to=nephrael_67#seconds=75432.5#future_seconds=0$time_string=(args=[$id=coin_42$loc=Giwo_34#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3838@refno=64$from=coin_35$to=nephrael_67#seconds=75432.5#future_seconds=0$time_string=(args=[$id=coin_35$loc=Giwo_34#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3839@refno=65$from=coin_36$to=nephrael_67#seconds=75432.5#future_seconds=0$time_string=(args=[$id=coin_36$loc=Giwo_34#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3840@refno=66$from=coin_37$to=nephrael_67#seconds=75432.5#future_seconds=0$time_string=(args=[$id=coin_37$loc=Giwo_34#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3841@refno=67$from=coin_44$to=nephrael_67#seconds=75432.5#future_seconds=0$time_string=(args=[$id=coin_44$loc=Giwo_34#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3842@refno=68$from=coin_38$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_38$loc=Giwo_34#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3843@refno=69$from=coin_39$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_39$loc=Giwo_34#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3844@refno=70$from=coin_40$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_40$loc=Giwo_34#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3845@refno=71$from=coin_41$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_41$loc=Giwo_34#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3846@refno=72$from=coin_57$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_57$loc=nephrael_56#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3847@refno=73$from=coin_58$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_58$loc=nephrael_56#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3848@refno=74$from=coin_59$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_59$loc=nephrael_56#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3849@refno=75$from=coin_60$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_60$loc=nephrael_56#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3850@refno=76$from=coin_62$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_62$loc=nephrael_56#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3851@refno=77$from=coin_63$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_63$loc=nephrael_56#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3852@refno=78$from=coin_64$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_64$loc=nephrael_56#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3853@refno=79$from=coin_65$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_65$loc=nephrael_56#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3854@refno=80$from=coin_66$to=nephrael_67#seconds=75432.6#future_seconds=0$time_string=(args=[$id=coin_66$loc=nephrael_56#mass=0.01$material=gold$name=coin$objtype=object(parents=$coin)(pos=#0#0#0)#stamp=0#status=1$type=coin])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3857@refno=3855$from=Giwo_34$to=nephrael_67#seconds=75449.7#future_seconds=0$time_string=(args=[(args=[$id=Giwo_34#status=0.937])$from=Giwo_34#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3701#seconds=75449.7@serialno=3855$time_string=$to=Giwo_34])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3860@refno=3858$from=giwo_45$to=nephrael_67#seconds=75456.7#future_seconds=0$time_string=(args=[(args=[$id=giwo_45#status=0.938])$from=giwo_45#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3704#seconds=75456.7@serialno=3858$time_string=$to=giwo_45])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3862@refno=81$from=nephrael_67$to=nephrael_67#seconds=75461.2#future_seconds=0$time_string=(args=[(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)$id=nephrael_67$loc=world_0$mode=running(orientation=#0#-0#-0.944725#0.327863)(pos=#0#0#0)(velocity=#-3.92506#-3.09741#0)])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$move)@refno=0#seconds=75461.2@serialno=81$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3864@refno=3863$from=nephrael_67$to=nephrael_67#seconds=75461.6#future_seconds=0$time_string=(args=[(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)$id=nephrael_67$loc=world_0$mode=standing(orientation=#0#-0#-0.944725#0.327863)(pos=#-1.49048#-1.17619#0)(velocity=#0#0#0)])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$move)@refno=3861#seconds=75461.6@serialno=3863$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3867@refno=3865$from=munin_23$to=nephrael_67#seconds=75473.4#future_seconds=0$time_string=(args=[(args=[$id=munin_23#status=0.827])$from=munin_23#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3707#seconds=75473.4@serialno=3865$time_string=$to=munin_23])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3870@refno=3868$from=Thomas__Achtner_1$to=nephrael_67#seconds=75476.6#future_seconds=0$time_string=(args=[(args=[$id=Thomas__Achtner_1#status=0.793])$from=Thomas__Achtner_1#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3710#seconds=75476.6@serialno=3868$time_string=$to=Thomas__Achtner_1])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3873@refno=3871$from=nephrael_56$to=nephrael_67#seconds=75486#future_seconds=0$time_string=(args=[(args=[$id=nephrael_56#status=0.982])$from=nephrael_56#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3713#seconds=75486@serialno=3871$time_string=$to=nephrael_56])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3874@refno=82$from=nephrael_67$to=nephrael_67#seconds=75496.5#future_seconds=0$time_string=(args=[(args=[$say= waaah$style=shout])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$talk)@refno=0#seconds=75496.5@serialno=82$time_string=$to=nephrael_67])(parents=$sound)$id=$objtype=op$name=]
S: [@serialnoo=3877@refno=3875$from=gfire1_12$to=nephrael_67#seconds=75503#future_seconds=0$time_string=(args=[(args=[$id=gfire1_12#status=0.797])$from=gfire1_12#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3719#seconds=75503@serialno=3875$time_string=$to=gfire1_12])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3878@refno=83$from=nephrael_67$to=nephrael_67#seconds=75504.4#future_seconds=0$time_string=(args=[(args=[$say= shhh$style=whisper])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$talk)@refno=0#seconds=75504.4@serialno=83$time_string=$to=nephrael_67])(parents=$sound)$id=$objtype=op$name=]
S: [@serialnoo=3881@refno=3879$from=nephrael_67$to=nephrael_67#seconds=75515.9#future_seconds=0$time_string=(args=[(args=[$id=nephrael_67#status=0.998])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3766#seconds=75515.9@serialno=3879$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3882@refno=84$from=nephrael_67$to=nephrael_67#seconds=75527.4#future_seconds=0$time_string=(args=[(args=[$say=blahbhal])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$talk)@refno=0#seconds=75527.4@serialno=84$time_string=$to=nephrael_67])(parents=$sound)$id=$objtype=op$name=]
S: [@serialnoo=3883@refno=85$from=coin_72$to=nephrael_67#seconds=75533.2#future_seconds=0$time_string=(args=[(args=[$id=coin_72$loc=world_0(pos=#-1.49048#-1.17619#0)])$from=nephrael_67#future_seconds=0$id=coin_72$name=$objtype=op(parents=$move)@refno=0#seconds=75533.2@serialno=85$time_string=$to=coin_72])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3886@refno=3884$from=Giwo_34$to=nephrael_67#seconds=75539.9#future_seconds=0$time_string=(args=[(args=[$id=Giwo_34#status=0.936])$from=Giwo_34#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3856#seconds=75539.9@serialno=3884$time_string=$to=Giwo_34])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3889@refno=3887$from=giwo_45$to=nephrael_67#seconds=75546.8#future_seconds=0$time_string=(args=[(args=[$id=giwo_45#status=0.937])$from=giwo_45#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3859#seconds=75546.8@serialno=3887$time_string=$to=giwo_45])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3892@refno=3890$from=munin_23$to=nephrael_67#seconds=75563.4#future_seconds=0$time_string=(args=[(args=[$id=munin_23#status=0.826])$from=munin_23#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3866#seconds=75563.4@serialno=3890$time_string=$to=munin_23])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3895@refno=3893$from=Thomas__Achtner_1$to=nephrael_67#seconds=75566.6#future_seconds=0$time_string=(args=[(args=[$id=Thomas__Achtner_1#status=0.792])$from=Thomas__Achtner_1#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3869#seconds=75566.6@serialno=3893$time_string=$to=Thomas__Achtner_1])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3897@refno=86$from=nephrael_67$to=nephrael_67#seconds=75568.5#future_seconds=0$time_string=(args=[(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)$id=nephrael_67$loc=world_0$mode=running(orientation=#0#-0#0.976667#0.214761)(pos=#-1.49048#-1.17619#0)(velocity=#-4.53878#2.0975#0)])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$move)@refno=0#seconds=75568.5@serialno=86$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3899@refno=3898$from=nephrael_67$to=nephrael_67#seconds=75568.7#future_seconds=0$time_string=(args=[(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)$id=nephrael_67$loc=world_0$mode=standing(orientation=#0#-0#0.976667#0.214761)(pos=#-2.21349#-0.842063#0)(velocity=#0#0#0)])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$move)@refno=3896#seconds=75568.7@serialno=3898$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3902@refno=3900$from=nephrael_56$to=nephrael_67#seconds=75576.1#future_seconds=0$time_string=(args=[(args=[$id=nephrael_56#status=0.981])$from=nephrael_56#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3872#seconds=75576.1@serialno=3900$time_string=$to=nephrael_56])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3904@refno=3903$from=nephrael_67$to=nephrael_67#seconds=75579.7#future_seconds=0$time_string=(args=[(args=[$action=touch$id=nephrael_67])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$action)@refno=0#seconds=75579.7@serialno=3903$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3906@refno=88$from=nephrael_67$to=nephrael_67#seconds=75584.5#future_seconds=0$time_string=(args=[(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)$id=nephrael_67$loc=world_0$mode=running(orientation=#0#-0#0.963649#0.267171)(pos=#-2.21349#-0.842063#0)(velocity=#-4.28619#2.57459#0)])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$move)@refno=0#seconds=75584.5@serialno=88$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3909@refno=3908$from=nephrael_67$to=nephrael_67#seconds=75585.5#future_seconds=0$time_string=(args=[(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)$id=nephrael_67$loc=world_0$mode=running(orientation=#0#-0#0.963649#0.267171)(pos=#-6.43095#1.69124#0)(velocity=#0#5#0)])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$move)@refno=3905#seconds=75585.5@serialno=3908$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3911@refno=3910$from=nephrael_67$to=nephrael_67#seconds=75585.7#future_seconds=0$time_string=(args=[(args=[(bbox=#-0.25#-0.25#0#0.25#0.25#2)$id=nephrael_67$loc=world_0$mode=standing(orientation=#0#-0#0.963649#0.267171)(pos=#-6.94365#1.99921#0)(velocity=#0#0#0)])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$move)@refno=3907#seconds=75585.7@serialno=3910$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3914@refno=3912$from=gfire1_12$to=nephrael_67#seconds=75593.1#future_seconds=0$time_string=(args=[(args=[$id=gfire1_12#status=0.796])$from=gfire1_12#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3876#seconds=75593.1@serialno=3912$time_string=$to=gfire1_12])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3915@refno=89$from=nephrael_67$to=nephrael_67#seconds=75604.8#future_seconds=0$time_string=(args=[(args=[$description=jumps$id=emote$loc=world_0])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$imaginary)@refno=0#seconds=75604.8@serialno=89$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3918@refno=3916$from=nephrael_67$to=nephrael_67#seconds=75605.9#future_seconds=0$time_string=(args=[(args=[$id=nephrael_67#status=0.997])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3880#seconds=75605.9@serialno=3916$time_string=$to=nephrael_67])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3921@refno=3919$from=Giwo_34$to=nephrael_67#seconds=75629.9#future_seconds=0$time_string=(args=[(args=[$id=Giwo_34#status=0.935])$from=Giwo_34#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3885#seconds=75629.9@serialno=3919$time_string=$to=Giwo_34])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3924@refno=3922$from=giwo_45$to=nephrael_67#seconds=75636.8#future_seconds=0$time_string=(args=[(args=[$id=giwo_45#status=0.936])$from=giwo_45#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3888#seconds=75636.8@serialno=3922$time_string=$to=giwo_45])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3927@refno=3925$from=munin_23$to=nephrael_67#seconds=75653.5#future_seconds=0$time_string=(args=[(args=[$id=munin_23#status=0.825])$from=munin_23#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3891#seconds=75653.5@serialno=3925$time_string=$to=munin_23])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3930@refno=3928$from=Thomas__Achtner_1$to=nephrael_67#seconds=75656.6#future_seconds=0$time_string=(args=[(args=[$id=Thomas__Achtner_1#status=0.791])$from=Thomas__Achtner_1#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3894#seconds=75656.6@serialno=3928$time_string=$to=Thomas__Achtner_1])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3933@refno=3931$from=nephrael_56$to=nephrael_67#seconds=75666.2#future_seconds=0$time_string=(args=[(args=[$id=nephrael_56#status=0.98])$from=nephrael_56#future_seconds=0$id=$name=$objtype=op(parents=$set)@refno=3901#seconds=75666.2@serialno=3931$time_string=$to=nephrael_56])(parents=$sight)$id=$objtype=op$name=]
S: [@serialnoo=3934@refno=90$from=nephrael_67$to=nephrael_67#seconds=75671.3#future_seconds=0$time_string=(args=[(args=[$say=asd])$from=nephrael_67#future_seconds=0$id=$name=$objtype=op(parents=$talk)@refno=0#seconds=75671.3@serialno=90$time_string=$to=nephrael_67])(parents=$sound)$id=$objtype=op$name=]
S: [@serialnoo=3935@refno=92$from=testJava$to=testJava#seconds=0#future_seconds=0$time_string=(args=[(args=)$from=testJava#future_seconds=0$id=testJava$name=$objtype=op(parents=$logout)@refno=0#seconds=0@serialno=92$time_string=$to=])(parents=$info)$id=$objtype=op$name=] 



      
