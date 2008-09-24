%:-prologAtInitalization(['mod/nani.sig']),prologAtInitalization(['mod/eliza.sig']),prologAtInitalization(bot),prologAtInitalization(bot2)
% ===================================================================
% COMPLIER OPTIONS
% ===================================================================

%module(mod_eliza).


:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).

:-dynamic(stdio/3).
:-dynamic(isRegistered/3).
:-multifile(isRegistered/3).
:-dynamic(chattingWith/2).
:-multifile(chattingWith/2).

:-dynamic(mem_c/1).
:-dynamic(isa_mem_c/1).
:-dynamic(isa_mem_c/3).

:-multifile(random_sent/1).
:-dynamic(random_sent/1).
:-multifile(e_rules/1).
:-dynamic(e_rules/1).
:-dynamic(default_channel/1).
:-dynamic(default_user/1).

:-multifile(server_environment/1).
:-dynamic(server_environment/1).

% ===================================================================
% Invokation  (knot,bot2).
% ===================================================================
bot2:-my_mooProcessCreate(killable,'Consultation Mode Test (KIFBOT!) OPN Server',consultation_thread(swiProlog,65530),Id,[]).
bot:-my_mooProcessCreate(killable,'Consultation Mode Test (KIFBOT!) Efnet Server',consultation_thread(swipl,65532),Id,[]).

my_mooProcessCreate(K,N,G,Id,A):-catch(mooProcessCreate(K,N,G,Id,A),_,fail),!.
my_mooProcessCreate(K,N,G,Id,A):-G,!.


% ===================================================================
% Conenctors
% ===================================================================
connect_chat(In,Out,Name,Port):-connect_chat(In,Out,'10.0.0.1',Port,Name,prologdev).

connect_chat(InStream,OutStream,Host,Port,User,Pass):-
		tcp_socket(SocketId),
		tcp_connect(SocketId,Host:Port),
		tcp_open_socket(SocketId, InStream, OutStream),
		format(OutStream,'~w\n',[User]),flush_output(OutStream),
		format(OutStream,'~w\n',[Pass]),flush_output(OutStream),
		retractall(stdio(User,_,_)),
		asserta((stdio(User,InStream,OutStream))),!.
		
get_chat(In,Out,User,CMD,Vars):-
        once(( catch(read_term(In,CMD,[variable_names(Vars)]),_,(CMD=true,Vars=[_])))).
	

	
consultation_thread(Name,Port):- 
	(connect_chat(In,Out,Name,Port)), 
	repeat,
		get_chat(In,Out,Name,CMD,Vars),
		retractall(stdio(Name,_,_)),
		asserta(stdio(Name,In,Out)),
		    catch((CMD),E,true),fail.
										       
 	
isRegistered(Channel,Botnick,Method):-isa_bot(Botnick),ignore(fail).

isa_bot("Kayumi").
isa_bot("Revenant").
isa_bot("trepan").
isa_bot("Tbot").
isa_bot("Xinfo").
isa_bot("Vallhalla").
isa_bot("Hibot").
isa_bot("Theist").
isa_bot("_Jibot").
isa_bot("eDTivrsky").
isa_bot("antiClauz").

/*
isa_bot(Botnick):-append([_|"itan"],_,Botnick).
isa_bot(Botnick):-append(_,"Bot",Botnick).
isa_bot(Botnick):-append(_,"bot",Botnick).
isa_bot(Botnick):-append(_,"ybo",Botnick).
isa_bot(Botnick):-append(_,"borg",Botnick).

*/


isRegistered("#ai",_,execute):-ignore(fail).
isRegistered("#pigface",_,execute):-ignore(fail).
isRegistered("#logicmoo",_,execute):-ignore(fail).
isRegistered("#kif",_,execute):-ignore(fail).
isRegistered("#rdfig",_,execute):-ignore(fail).
isRegistered("#enigma",_,execute):-ignore(fail).
isRegistered("#prolog",_,execute):-ignore(fail).
isRegistered("#ypn_ai",_,execute):-ignore(fail).
isRegistered(Channel,User,kifbot):-chattingWith(Channel,User).
isRegistered(Channel,User,doctor):-chattingWith(Channel,User).

register(User,Method):-ground((User,Method)),isRegisteredChk(Channel,User,Method),!.
register(User,Method):-assert(isRegistered(Channel,User,Method)).
register(Channel,User,Method):-ground((User,Method)),isRegisteredChk(Channel,User,Method),!.
register(Channel,User,Method):-
	assert(isRegistered(Channel,User,Method)).
unregister(User,Method):-retractall(isRegistered(Channel,User,Method)).
registered(Who):-
		isRegisteredChk(Channel,User,Method),
		say(isRegisteredChk(Channel,User,Method),['Channel'=Channel,'User'=User,'Method'=Method|_]),fail.
		

		       
from([_], _, _):-!.

from(Channel,Who,Method):-
	set_default_channel(Channel),
	set_default_user(Who),make,fail.

set_default_channel(Channel):-retractall(default_channel(_)),assert(default_channel(Channel)).
set_default_user(User):-retractall(default_user(_)),assert(default_user(User)).

from(Channel,Who,Method):-once(catch(once(nani_event_from(Channel,Who,Method)),_,true)),fail.

from(Channel,Who,say(W)):-same_str(W,"goodbye"),!,              
		retractall(chattingWith(Channel,Who)).
		
from(Channel,Who,say(W)):-(same_str(W,"jllykifsh");same_str(W,"jllykifsh?")),!,
		asserta(chattingWith(Channel,Who)),
		say([hi,Who,'I will answer you until you say "goodbye"']),!.
		
from(Channel,Who,say(Atom)):- 
		isRegisteredChk(Channel,Who,execute),
		catch(atom_to_term(Atom,Term,Vars),_,fail),
		consts(atomic,Term,List,_,_),
		not(member(shell,List)),
		not(member(help,List)),
		not(member(rm,List)),
		(unsetMooOption(client=_)),
		(setMooOption(client=consultation)), 
		catch(once((Term,write_chat_vars(Vars))),E,(format(user_error,'~q\n',[E]),fail)).	

from(Channel,Who,say(String)):- 
		isRegisteredChk(Channel,Who,kifbot),       
		catch(kif_response(Channel,Who,String),_,fail).

from(Channel,Who,say(String)):- 
		catch(learn_response(Channel,Who,String),_,fail),fail.

from(Channel,Who,say(Codes)):-
	once((my_name_in_codes(Codes);isRegisteredChk(Channel,Who,doctor);rnd(0))),
	atom_codes(Atom,Codes),
	once(atom_to_input(Atom,WhatIs)),
	english_response(Channel,Who,WhatIs).

my_name_in_codes(Codes):-
	make_term_lowercase(Codes,Lower),!,
	my_name_in_lcase_codes(Lower).
	
my_name_in_lcase_codes(Codes):-
	append(_,[106, 108, 108|_],Codes).
my_name_in_lcase_codes(Codes):-
	append(_,[106,_,108, 108|_],Codes).

atom_to_input(Atom,Input3):-
	make_term_lowercase(Atom,AtomL),
	atom_codes(AtomL,String),
	e_clean_string(String,Cleanstring),
	extract_atomics(Cleanstring,Input),
	get_phasing(Input,[],Input2),	
	idioms(doctor,Input2,Input3),!.

codes_to_input(Codes,Input3):-
	make_term_lowercase(Codes,String),
	e_clean_string(String,Cleanstring),
	extract_atomics(Cleanstring,Input),
	get_phasing(Input,[],Input2),	
	idioms(doctor,Input2,Input3),!.
	

english_response(Channel,Who,List):-
		catch(once(dirrect_order(List)),_,fail).

english_response(Channel,Who,List):-
		catch(once(what_is_game(List)),_,fail),!.

english_response(Channel,Who,List):-
		catch(once(doctor(List)),_,fail),!.
		
dirrect_order([start,tomcat]).
%dirrect_order([start,tomcat]):-shell('/opt/tomcat/bin/startup.sh'),say([ok,done]).

				
rnd(N):-number(N),X is random(100),!,X<N.

write_chat_vars([]).
write_chat_vars([N=V|L]):-
	var(V),
	write_chat_vars(L).
write_chat_vars([V|L]):-
	sayq(V),
	write_chat_vars(L).
	
call_all(X):-X,fail.
call_all(X).


isRegisteredChk(ChannelI,WhoI,ActI):-
	isRegistered(Channel,Who,Act),
	same_str(ChannelI,Channel),
	same_str(WhoI,Who),
	same_str(ActI,Act).
	
same_str(X,X):-!.
same_str(S,Y):-to_codes(S,SS),to_codes(Y,YS),!,SS=YS.

to_codes(S,O):-string(S),!,string_to_atom(S,A),to_codes(A,O).
to_codes(A,O):-atom(A),!,atom_codes(A,O).
to_codes(C,C).


kif_response(Channel,Who,String):- 
	once(getSurfaceFromChars(String,STERM,Vars)),
	not( STERM=[comment,_]),
	once((getMooTermFromSurface(STERM,NEWFORM))),
	(unsetMooOption(client=_)),
	default_user(User),
	(setMooOption(client=consultation)),
	once(catch(invokeOperation_irc(Channel,Who,NEWFORM,User,Vars),E,say(E))),!.
	 
invokeOperation_irc(Channel,Who,X,User,Vars):-
        isRegisteredChk(Channel,Who,kifbot),!,
	once(invokeOperationProc(verbose,X,'ToplevelContext',TN,'Merge',User,Vars)),!.

invokeOperation_irc(Channel,Who,assert(X),User,Vars):-
      %  isRegisteredChk(Channel,Who,assert),
      !,
	invokeOperationProc(verbose,assert(X),'ToplevelContext',TN,'Merge',User,Vars),!.     
invokeOperation_irc(Channel,Who,query(X),User,Vars):-
	%isRegisteredChk(Channel,Who,query),
	!,
	invokeOperationProc(verbose,query(X),'ToplevelContext',TN,'Merge',User,Vars),!.     
invokeOperation_irc(Channel,Who,queryyn(X),User,Vars):-
	%isRegisteredChk(Channel,Who,query),
	!,
	invokeOperationProc(verbose,queryyn(X),'ToplevelContext',TN,'Merge',User,Vars),!.     
    
get_default_channel(Dest):-default_channel(Dest),!.
get_default_channel(console).


say(fixed([N|L])):-!,my_toMarkUp(chat,nv([N|L]),Vars,O),!,sayn(O).
say(fixed(N)):-!,my_toMarkUp(chat,N,Vars,O),!,sayn(O).
say(N):-fix_say(N,O),!,say(fixed(O)).


fix_say([],[]).
fix_say([N|L],[O|LL]):-!,fix_say(N,O),!,fix_say(L,LL).
	
fix_say(N,N).


say(N,Vars):-
	fix_say(N,F),
	my_toMarkUp(chat,F,Vars,O),!,sayn(O).


sayq(Data):-default_channel(Dest),
	sformat(Data2,'~q',[Data]),
	say_to(Dest,Data2).  
sayn(Data):-  
	get_default_channel(Dest),
	say_to(Dest,Data).  

say_to(console,Data):-!,write(Data),nl.
say_to([35|Dest],Data):-!,
	once(stdio(User,InStream,OutStream)),
	once(channel_say(OutStream,[35|Dest],Data)),
	format(OutStream,'eot.\n',[Dest,Data]),
	flush_output(OutStream),
	flush_output(user_error),!.
say_to('#'(Dest),Data):-atom(Dest),!,atom_codes(Dest,Codes),say_to([35|Codes],Data).
say_to(Dest,Data):-atom(Dest),!,atom_codes(Dest,Codes),say_to(Codes,Data).
say_to(Dest,Data):-!,
	once(stdio(User,InStream,OutStream)),
	ignore(catch(format(OutStream,'.msg ~s ~w\n',[Dest,Data]),_,fail)),
	format(OutStream,'eot.\n',[Dest,Data]),
	flush_output(OutStream),
	flush_output(user_error),!.	

say_with_lines(Output):-say(Output).
	
channel_say(OutStream,Channel,Data):-
	concat_atom(List,'\n',Data),
	channel_say_list(OutStream,Channel,List).

channel_say_list(OutStream,Channel,[]).
channel_say_list(OutStream,Channel,[N|L]):-
	ignore(catch(format(OutStream,'\n.say ~s ~w\n',[Channel,N]),_,fail)),
	flush_chan,
	channel_say_list(OutStream,Channel,L),!.

my_toMarkUp(A,B,C,D):-catch(toMarkUp(A,B,C,D),_,fail),!.
my_toMarkUp(X,_,_,Y):-is_list(X,' ',Y),!.
my_toMarkUp(X,_,_,X).


flush_chan:-once(stdio(_,_,O)),flush_output(O).

to_egg(X):-to_egg(X,[]),!.
to_egg(X,Y):-once(stdio(User,InStream,OutStream)),once((sformat(S,X,Y),format(OutStream,'~s\n',[S]),!,flush_output(OutStream))).

eot:-!.	


english_to_kif(ENG):-ado_tuple(L,P,A,B),
	make_term_lowercase_util(P,PL),
	make_term_lowercase_util(A,AL),
	make_term_lowercase_util(B,BL),
	paraphrase([L,AL,PL,BL],E),
	E=ENG.
	
ado_tuple(Logic,P,A,B):-
	mooCache(Surface,CLF,Flags,Vars,Context,TN,Author,TMResult),
	Surface=..[P,A,B],
	ground((P,A,B)).


	
paraphrase([L,A,P,B],[A,P,B]).
make_term_lowercase_util(B,BL):-atom(B),make_term_lowercase(B,BL),!.
	

% ===========================================================
% To be executed in Channel
% ===========================================================

download(URL):-file_base_name(URL,File),!,download(URL,File).
download(URL,File):-say([downloading,URL,File]),!,sformat(Shell,'wget -O ~w  ~w ',[File,URL]),string_to_atom(Shell,Cmd),shell(Cmd),!,
	say([done,downloading,File]).

use(URL):- file_base_name(URL,File),!,((download(URL,File),load_file(File))).

in_thread(X):-mooProcessCreate(killable,chat(X),X,Id,[]).

load_file(File):-file_name_extension(Base, rpm,File),!, sformat(Shell,'rpm --force -i ~w  ',[File]),string_to_atom(Shell,Cmd),shell(Cmd),!,say([done,installing,File]).
load_file(File):-file_name_extension(Base, 'P',File),!,ensure_loaded(File),say([done,compiling,File]).
load_file(File):-file_name_extension(Base, 'p',File),!,ensure_loaded(File),say([done,compiling,File]).
load_file(File):-file_name_extension(Base, 'pro',File),!,ensure_loaded(File),say([done,compiling,File]).
load_file(File):-file_name_extension(Base, 'swi',File),!,ensure_loaded(File),say([done,compiling,File]).
load_file(File):-file_name_extension(Base, 'PRO',File),!,ensure_loaded(File),say([done,compiling,File]).
load_file(File):-file_name_extension(Base, daml,File),file_name_extension(Base, kif,KIFFile),!, sformat(Shell,'cat ~w |python ./rdfx2kif.py ~w > ~w.kif  ',[File,Base,Base]),string_to_atom(Shell,Cmd),shell(Cmd),!,agent_load_kif_surface(KIFFile,Context,User,AX),say([loaded,AX,'axioms.  Now canonicalizing...']).
load_file(File):-file_name_extension(File, kif,KIFFile),!,sformat(Shell,'cat ~w |python ./rdfx2kif.py ~w > ~w.kif  ',[File,File,File]),string_to_atom(Shell,Cmd),shell(Cmd),!,agent_load_kif_surface(KIFFile,Context,User,AX),say([loaded,AX,'axioms.  Now canonicalizing...']).

perl(CMD):-sformat(Shell,'perl -c "~w"  ',[CMD]),string_to_atom(Shell,Cmd),shell(Cmd).

load_file(File):-file_name_extension(Base, E,File),!,say([E,'Extension is not yet regisitered']).


bot(ps):-!,mooThreadCreate_data(Perms,Name,Goal,Id,Options),
	prolog_current_thread(Id,Status),
	sayq((Id:Name:Goal:(Status):(Perms):Options)),fail.
bot(killable(Id)):-nonvar(Id),mooThreadCreate_data(killable,Name,Goal,Id,Options),
	prolog_thread_signal(Id,prolog_thread_exit(killed)).

		
yn(P):-P,!,sayn(yes).
yn(P):-sayn(no).

timeit(X):-getCputime(S),yn(X),getCputime(E),Time is E-S,say(Time).
mtry(G):-catch(G,E,say(E)).

listpr(P/A):-
	functor(H,P,A),!,
	listpr(H),!.	
listpr(H):-
	clause(H,Y),
		(Y=true -> sayq(H);sayq((H:-Y))),fail.
listpr(H).		

pp(P/A):-
	functor(H,P,A),!,
	pp(H),!.	
pp(H):-findall(Y,predicate_property(H,Y),L),L=[_|_],sayq(L),!.

pp(H):-say(nv(['i don''t know about',H])),!.


%surf(Num):-number(Num),mooCache(PredR,_,F,Vars,_,_,Num,_,_),say(F,Vars),fail.
%surf(Num):-say([assertion,Num,not,found]).

			
theorysize:-predicate_property(mooCache(_,_,_,_,_,_,_,_,_),number_of_clauses(X)),say(X).

/*
doc(Const):-mooCache(PredR,_,documentation(Const,F),Vars,_,_,Num,_,_),!,my_toMarkUp(kif,F,Vars,O),sayn(O),!.
doc(Const):-sayn('Not Found').
  */

load_file_into_module(File):-load_file_into_module(File,user).

load_file_into_module(File,PublicName):-expand_file_name(File,[Name]),!,
	open(Name,'read',Stream),!,
	repeat,
		once(handle_stream(Stream,PublicName)),
		at_end_of_stream(Stream),!,close(Stream).
	
handle_stream(Stream,PublicName):-	
		catch(read_term(Stream,X,[syntax_errors(true),module(PublicName),term_positon('$stream_position'(CharIndex,LineNum,Line,Pos))]),E,say(E)),
		catch(process_read(PublicName,X),E,say(E)),flush_output(user_error),!.
	
process_read(PublicName,':-'(include(File))):-!. %,load_file_into_module(File,PublicName).
process_read(PublicName,':-'(X)):-!,PublicName:X.
process_read(PublicName,(X)):-!,assert(PublicName:X).



learn_response(Channel,Who,Codes):-
	atom_codes(Atom,Codes),
	once(atom_to_input(Atom,WhatIs)),
	remember_what_is(WhatIs).


% ===========================================================
%    main routine of what_is_game
% ===========================================================
	
ww(WH):-member(WH,[who,what,where,when,why,how]).

%is_be([*,is,will,are,was,were,isnt,wasnt,werent,been,likes,feels,thinks,do,does,can,being,has,hasnt,be,am,M],M):-!.
is_be([*,is,will,are,was,were,isnt,could,be,am,M],M):-!.


what_is_game(WhatIs):-
	remember_what_is(WhatIs).
what_is_game(In):-
	once(what_is_query(In)),!.
	
	
	

% Tell
remember_what_is([im|X]):-remember(i,am,X),!,bumem,fail.
% Tell
remember_what_is([youre|X]):-remember(you,is,X),!,bumem,fail.


% Nani hook
remember_what_is(X):-catch(once(do_irc(X)),_,fail),!.




% Tell
remember_what_is(WhatIs):-
	is_be(ISBE,M),
	once(e_match([X,ISBE,Y],WhatIs)),
	not(ww(X)),!,
	remember(X,M,Y),!,bumem,fail.
	


what_is_query([i,save]):-bumem,fail.


bumem:-tell('mod/mod_eliza.data'),listing(isa_mem_c),listing(mem_c),told.
	
what_is_query([i,learned]):-
	retract(isa_mem_c(Mem)),!,
	assertz(isa_mem_c(Mem)),!,
	say_english(Mem),!.

what_is_query(In):-
	once((get_default_user_atom(W),subst(In,you,W,WhatIs))),
	once((is_be(ISBE,M),e_match([WH,ISBE,Y],WhatIs))),
	ww(WH),
	find_explain(Y).

what_is_query(In):-
	once((get_default_user_atom(W),subst(In,you,W,WhatIs))),
	once((is_be(ISBE,M),e_match([WH,ISBE,_,Y],WhatIs))),
	ww(WH),
	find_explain(Y).


find_explain(Y):-
	to_list(Y,YY),!,
	append(YY,_,Mem),!,
	isa_mem(Mem),
	say_english(Mem),!.
	
remember(IX,IM,IY):-
	get_default_user_atom(Who),
	subst((IX,IM,IY),you,Who,(X,M,Y)),
	to_list(X,XX),
	to_list(Y,YY),
	once(append(XX,[M|YY],Mem)),!,
	asserta_if_new(isa_mem_c(Mem)),!.
	
isa_mem(Mem):-
	retract(isa_mem_c(Mem)),!,
	assertz(isa_mem_c(Mem)).


to_list([],_):-!,fail.
to_list([H|T],[H|T]):-!.
to_list(im,[i,am]):-!.
to_list(youre,[you,are]):-!.
to_list(H,[H]):-!.


isa_mem([X,is,a,sublass,of,D]):-english_to_kif([X,subclass,D]),rnd(50).
isa_mem([X,is,opposite,of,D]):-english_to_kif([X,disjoint,D]),rnd(50).
isa_mem([X,is,opposite,of,D]):-english_to_kif([X,contraryproperty,D]),rnd(50).
isa_mem([X,is,an,instance,of,D]):-english_to_kif([X,instance,D]).
isa_mem([X,is,D]):-english_to_kif([X,documentation,D]).
isa_mem([X,is,a,subclass,of,D]):-english_to_kif([X,subclass,D]).

mem(X):-isa_mem([youre|X]).
mem(X):-isa_mem([you,are|X]).

get_default_user_atom(WhatIs):-
	default_user(Nick),
	atom_codes(Atom,Nick),
	atom_to_input(Atom,[WhatIs]),!.

% ===========================================================
%    main routine of ELIZA
% ===========================================================
	
doctor(Input3):-
	once(findkeywords(Input3,KeyWords)),
	sortkeywords(KeyWords,KeyWords2),!,
	makecomment(KeyWords2,Input3,Comment),!,
	say_english(Comment).


say_english(Comment):- 
	flatten(Comment,Comment2),
	clean_generation(Comment2,O),!,
	say_eli2(O).

say_eli2([nop]):-!. 
say_eli2([randomsentence]):-!, say_randomsentence.
say_eli2(T):-!,say(nv(T)),!.
say_randomsentence:-!.


choose_randomsentence(X):-
	repeat,
		retract(random_sent(Y)),
		assertz(random_sent(Y)),
		4 is random(10),
		Y=X.
		

random_sent([blah,blah]).			

clean_generation([],[]).
clean_generation([B,'?'|More],[Cat|More2]):-!,expand_alias(B,A),atom_concat(A,'?',Cat),clean_generation(More,More2).
clean_generation([B,','|More],[Cat|More2]):-!,expand_alias(B,A),atom_concat(A,',',Cat),clean_generation(More,More2).
clean_generation([B,'!'|More],[Cat|More2]):-!,expand_alias(B,A),atom_concat(A,'!',Cat),clean_generation(More,More2).
clean_generation(['.'],[]).
clean_generation([B|L],[A|LL]):-
	expand_alias(B,A),!,
	clean_generation(L,LL),!.

expand_alias(nick,A):-!,default_user(B),!,atom_codes(A,B),!.
expand_alias(mychan,A):-!,default_channel(B),!,atom_codes(A,B),!.
expand_alias(B,B):-!.

/****************************************************************************/
% idioms(+Dictionay,+List,-Result)
%   implements non-overlapping idioms
%   idiomizes List into Result

idioms(Dict,List,Result) :- 
	once(use_dict(Dict,List,Result,X,Y)), !,
	idioms(Dict,X,Y),!.
	
idioms(Dict,[W|Words],[W|NewWords]):- 
	idioms(Dict,Words,NewWords),!.
	
idioms(Dict,[],[]):-!.

use_dict(doctor,[Ignore|X],Y,X,Y):-
	dictionary(_,ignore_word,Ignore).

use_dict(doctor,In,New,X,Y):-
	use_dict(contractions,In,New,X,Y).
		
use_dict(doctor,In,New,X,Y):-
	use_dict(slang,In,New,X,Y).
	
use_dict(doctor,In,New,X,Y):-
	use_dict(swap_person,In,New,X,Y).

use_dict(doctor,In,New,X,Y):-
	use_dict(invert,In,New,X,Y).
	
use_dict(doctor,In,New,X,Y):-
	use_dict(mud,In,New,X,Y).
	
use_dict(D,In,New,X,Y):-
	dictionary(D,This,Replace),
	append(This,X,In),!,
	append(Replace,Y,New).

use_dict(D,[B|X],[A|Y],X,Y):-
	dictionary(D,B,A).


use_dict(invert,[not,A|X],[B|Y],X,Y):-sar(inverse(A,B)),!.
use_dict(invert,[not,A|X],[B|Y],X,Y):-sar(inverse(B,A)),!.
use_dict(invert,[not,A|X],[B|Y],X,Y):-sar(contraryProperty(A,B)),!.
use_dict(invert,[not,A|X],[B|Y],X,Y):-sar(contraryProperty(B,A)),!.

use_dict(invert,[A|X],[not,B|Y],X,Y):-sar(inverse(A,B)).
use_dict(invert,[A|X],[not,B|Y],X,Y):-sar(contraryProperty(A,B)).

sar(PFact):-fail.


:-dynamic(dictionary/3).

dictionary(contractions,[do,not],[dont]).
dictionary(contractions,[can,not],[cant]).
dictionary(contractions,[cannot],[cant]).
dictionary(contractions,[will,not],[wont]).

dictionary(mud,X,Y):-catch(mud_dictionary(X,Y),_,fail).

%dictionary(doctor,[shut,up],['shut up']).
%dictionary(doctor,[piss,off],['piss off']).

dictionary(doctor,dreamed,dreamt).
dictionary(doctor,fantasy,dreamt).
dictionary(doctor,fantasies,dreamt).
dictionary(doctor,dreams,dream).
dictionary(doctor,how,how).
dictionary(doctor,what,what).
dictionary(doctor,when,when).
dictionary(doctor,alike,dit).
dictionary(doctor,different,dit).
dictionary(doctor,same,dit).
dictionary(doctor,opposite,dit).
dictionary(doctor,certainly,yes).
dictionary(doctor,ok,ok).
dictionary(doctor,yep,yes).
dictionary(doctor,yup,yes).
dictionary(doctor,yeah,yes).
dictionary(doctor,maybe,perhaps).
dictionary(doctor,deutsch,xfremd).
dictionary(doctor,francais,xfremd).
dictionary(doctor,espanol,xfremd).
dictionary(doctor,machine,computer).
dictionary(doctor,machines,computer).
dictionary(doctor,computers,computer).
dictionary(doctor,whatever,insult).
dictionary(doctor,everybody,everyone).
dictionary(doctor,nobody,everyone).
dictionary(doctor,would,will).

dictionary(swap_person,your,my).
dictionary(swap_person,yours,mine).
dictionary(swap_person,there,here).
dictionary(swap_person,here,there).
dictionary(swap_person,were,was).
dictionary(swap_person,me,you).
dictionary(swap_person,[u,are],[im]).
dictionary(swap_person,[you,are],[im]).
dictionary(swap_person,[i,am],[youre]).
dictionary(swap_person,myself,yourself).
dictionary(swap_person,yourself,myself).
dictionary(swap_person,['ill'],[you,will]).
dictionary(swap_person,'i',you).
dictionary(swap_person,'im',youre).
dictionary(swap_person,'youre',im).
dictionary(swap_person,am,are).
dictionary(swap_person,would,will).
dictionary(swap_person,i,you).
dictionary(swap_person,you,i).
dictionary(swap_person,u,i).
dictionary(swap_person,my,your).
dictionary(swap_person,mine,yours).
dictionary(swap_person,us,you).
dictionary(swap_person,we,you).
dictionary(swap_person,them,they).
dictionary(swap_person,belong,belonging).

dictionary(swap_person,he,i).
dictionary(swap_person,[hes],[i,am]).
dictionary(swap_person,['Hes'],[i,am]).
dictionary(swap_person,'He',i).
dictionary(swap_person,['ive'],[you,have]).

dictionary(swap_person,['he',is],[i,am]).
dictionary(swap_person,his,my).
dictionary(swap_person,him,me).
dictionary(irc,ignore_word,prolog).
dictionary(irc,ignore_word,'jllykifsh').
dictionary(irc,ignore_word,'jlly').
dictionary(irc,ignore_word,'jellyfish').
dictionary(irc,ignore_word,'jll').

dictionary(slang,r,are).
dictionary(slang,[wanna],[want,to]).

dictionary(slang,sup,hello).
dictionary(slang,hijm,him).

dictionary(slang,yur,your).
dictionary(slang,[the,bot],[me]).
dictionary(slang,person,wanker).
dictionary(slang,bith,bitch).
dictionary(slang,yuor,your).
dictionary(slang,wazzup,hello).
dictionary(slang,wazup,hello).
dictionary(slang,greetings,[hello]).
dictionary(slang,[whats,up],[hello]).
dictionary(slang,[what,is,up],[hello]).
dictionary(slang,[whats],[what,is]).
dictionary(slang,mom,mother).
dictionary(slang,mum,mother).
dictionary(slang,mommy,mother).
dictionary(slang,momma,mother).
dictionary(slang,mamma,mother).
dictionary(slang,dad,father).
dictionary(slang,daddy,father).

dictionary(slang,waht,what).
dictionary(slang,wut,what).
dictionary(slang,[wtf],[what,the,fuck]).
dictionary(slang,teh,the).
dictionary(slang,k,ok).
dictionary(slang,re,hello).
dictionary(slang,hi,hello).
dictionary(slang,tard,retard).
dictionary(slang,neph,nephrael).
dictionary(slang,cause,because).
dictionary(slang,casue,because).
dictionary(slang,cuase,because).
dictionary(slang,cuz,because).
dictionary(slang,luv,love).
dictionary(slang,lub,love).
dictionary(slang,lubbin,lubbing).
dictionary(slang,becasue,because).
dictionary(slang,mebbe,maybe).
%dictionary(slang,X,Y):-isa_mem(X,is,Y),rnd(25).


/*****************************************************************************/
% echar_type(+Char,?Type)
%    Char is an ASCII code.
%    Type is whitespace, punctuation, numeric, alphabetic, or special.

echar_type(46,period) :- !.
echar_type(35,alphanumeric) :- !.
echar_type(58,alphanumeric) :- !.
echar_type(47,alphanumeric) :- !.
echar_type(X,alphanumeric) :- X >= 65, X =< 90, !.
echar_type(X,alphanumeric) :- X >= 97, X =< 123, !.
echar_type(X,alphanumeric) :- X >= 48, X =< 57, !.
echar_type(X,whitespace) :- X =< 32, !.
echar_type(X,punctuation) :- X >= 33, X =< 47, !.
echar_type(X,punctuation) :- X >= 58, X =< 64, !.
echar_type(X,punctuation) :- X >= 91, X =< 96, !.
echar_type(X,punctuation) :- X >= 123, X =< 126, !.
echar_type(_,special).


make_term_lowercase(V,V):-var(V).
make_term_lowercase([],[]).
make_term_lowercase([In|B],[LA|D]):-!,
	make_term_lowercase(In,LA),!,
	make_term_lowercase(B,D),!.

make_term_lowercase(In,Out):-integer(In),lcase_code(In,Out).

make_term_lowercase(In,Out):-
	compound(In),!,
	In=..List,
	make_term_lowercase(List,ToLo),!,
	Out=..ToLo,!.

make_term_lowercase(In,LA):-
	atom(In),
	atom_codes(In,Codes),
	lcase_code_str(Codes,LCodes),!,
	atom_codes(LA,LCodes),!.

make_term_lowercase(In,In).

lcase_code_str([],[]).
lcase_code_str([Code|Codes],[L|LCodes]):-
	 lcase_code(Code,L),
	lcase_code_str(Codes,LCodes).
	 

lcase_code(X,Y) :- 
	X >= 65,
	X =< 90,
	Y is X + 32, !.

lcase_code(X,X).
                   

/*****************************************************************************/
% get_first_word(+String,-Rest,-Word) (final version)
%  Extracts the first Word from String; Rest is rest of String.
%  A word is a series of contiguous letters, or a series
%  of contiguous digits, or a single special character.
%  Assumes String does not begin with whitespace.

get_first_word([C|Chars],Rest,[C|RestOfWord]) :-
	echar_type(C,Type),
	get_first_word_aux(Type,Chars,Rest,RestOfWord).

get_first_word_aux(special,Rest,Rest,[]) :- !.
   % if Char is special, don''t read more chars.

get_first_word_aux(Type,[C|Chars],Rest,[C|RestOfWord]) :-
	echar_type(C,Type), !,
	get_first_word_aux(Type,Chars,Rest,RestOfWord).

get_first_word_aux(_,Rest,Rest,[]).   % if previous clause did not succeed.


/*****************************************************************************/
% ltrim_codes(+X,?Y)
%   Removes whitespace characters from the
%   beginning of string X, giving string Y.

ltrim_codes([C|Chars],Result) :-
	echar_type(C,whitespace), !,
	ltrim_codes(Chars,Result).

ltrim_codes(X,X).   % if previous clause did not succeed.


/*****************************************************************************/
% e_digit_value(?D,?V)
%  Where D is the ASCII code of a digit,
%  V is the corresponding number.

e_digit_value(48,0).
e_digit_value(49,1).
e_digit_value(50,2).
e_digit_value(51,3).
e_digit_value(52,4).
e_digit_value(53,5).
e_digit_value(54,6).
e_digit_value(55,7).
e_digit_value(56,8).
e_digit_value(57,9).


/*****************************************************************************/
% e_string_to_number(+S,-N)
%  Converts string S to the number that it
%  represents, e.g., "234" to 234.
%  Fails if S does not represent a nonnegative integer.

e_string_to_number(S,N) :-
	e_string_to_number_aux(S,0,N).

e_string_to_number_aux([D|Digits],ValueSoFar,Result) :-
	e_digit_value(D,V),
	NewValueSoFar is 10*ValueSoFar + V,
	e_string_to_number_aux(Digits,NewValueSoFar,Result).

e_string_to_number_aux([],Result,Result).


/*****************************************************************************/
% string_to_atomic(+String,-Atomic)
%  Converts String into the atom or number of
%  which it is the written representation.

string_to_atomic([C|Chars],Number) :-
	e_string_to_number([C|Chars],Number), !.

string_to_atomic(String,Atom) :- name(Atom,String).
  % assuming previous clause failed.


/*****************************************************************************/
% extract_atomics(+String,-ListOfAtomics) (second version)
%  Breaks String up into ListOfAtomics
%  e.g., " abc def  123 " into [abc,def,123].

extract_atomics(String,ListOfAtomics) :-
	ltrim_codes(String,NewString),
	extract_atomics_aux(NewString,ListOfAtomics).

extract_atomics_aux([C|Chars],[A|Atomics]) :-
	get_first_word([C|Chars],Rest,Word),
	string_to_atomic(Word,A),       % <- this is the only change
	extract_atomics(Rest,Atomics).

extract_atomics_aux([],[]).


/*****************************************************************************/
% e_clean_string(+String,-Cleanstring)
%  removes all punctuation characters from String and return Cleanstring

e_clean_string([C|Chars],L) :- 
	echar_type(C,punctuation),
	e_clean_string(Chars,L), !.
e_clean_string([C|Chars],[C|L]) :-
	e_clean_string(Chars,L), !.
e_clean_string([C|[]],[]) :-
	echar_type(C,punctuation), !.
e_clean_string([C|[]],[C]).


/****************************************************************************/
% e_findnth(+List,+Number,-Element)
%    returns the Nth member of List in Element

e_findnth([E|_],1,E).
e_findnth([_|T],N,T1) :- V is N - 1,
                       e_findnth(T,V,T1).


/****************************************************************************/
% e_match(+MatchRule,+InputList)
%    e_matches the MatchRule with the InputList. If they e_match, the variables
%    in the MatchRule are instantiated to one of three things:
%       an empty list
%       a single word
%       a list of words

e_match(A,C) :- e_match_aux1(A,C),!.
e_match(A,C) :- e_match_aux2(A,C).

e_match_aux1(A,C) :-
	member([*|T],A),
	nonvar(T),
	member(Tm,T),
	nonvar(Tm),
	e_replace([*|T],A,Tm,B),
	e_match_aux2(B,C),
	!, last(L,T), L = Tm.

e_match_aux2([],[]).
e_match_aux2([Item|Items],[Word|Words]) :-
	e_match_aux3(Item,Items,Word,Words),!.
e_match_aux2([Item1,Item2|Items],[Word|Words]) :-
	var(Item1),
	nonvar(Item2),
	Item2 == Word,!,
	e_match_aux2([Item1,Item2|Items],[[],Word|Words]).
e_match_aux2([Item1,Item2|Items],[Word|Words]) :-
	var(Item1),
	var(Item2),!,
	e_match_aux2([Item1,Item2|Items],[[],Word|Words]).
e_match_aux2([[]],[]).

e_match_aux3(Word,Items,Word,Words) :-
	e_match_aux2(Items,Words), !.
e_match_aux3([Word|Seg],Items,Word,Words0) :-
	append(Seg,Words1,Words0),
	e_match_aux2(Items,Words1).

/****************************************************************************/
% e_replace(+Element1,+List1,+Element2,-List2)
%    e_replaces all instances of Element1 in List1 with Element2 and returns
%       the new list as List2
%    does not e_replace variables in List1 with Element1

e_replace(_,[],_,[]).
e_replace(X,[H|T],A,[A|T2]) :- nonvar(H), H = X, !, e_replace(X,T,A,T2).
e_replace(X,[H|T],A,[H|T2]) :- e_replace(X,T,A,T2).


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
% get_phasing(+Input_List,+[],?Output)
%     returns part of input after a comma, or
%             part of input before a period

get_phasing([],L,L).
get_phasing(['.'|_],L,L) :- findkeywords(L,K), length(K,Kl), Kl >= 3,!.
get_phasing(['.'|T],_,L) :- !, get_phasing(T,[],L).
get_phasing([', '|_],L,L) :- findkeywords(L,K), length(K,Kl), Kl >= 3,!.
get_phasing([', '|T],_,L) :- !, get_phasing(T,[],L).
get_phasing([H|T],S,L) :- append(S,[H],S2), get_phasing(T,S2,L).


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
		[nop]]]]):-precond.
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



:-['mod/mod_eliza.data'].


%chattingWith(Channel,User).
%:-bot.
%:-bot2.



