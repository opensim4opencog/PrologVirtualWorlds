:-include('logicmoo_utils_header.pl'). %<?
% =================================================================================
% Writeback to clients.. initial form is assumed Bach       and gets translated to the clients negatioted protocall
% =================================================================================
sayIfFeedback(Channel,Agent,Comment):- say(Comment).
:-dynamic(wantsEvents/5).
% say/1

say(Format):- atom(Format),default_channel(Channel),sayLine(Channel,Format).
say(Format):- default_channel(Channel),say(Channel,Format).

% say/2
say(Format,[A|RGS]):-atom(Format),catch(sformat(String,Format,[A|RGS]),_,fail),!,say(String).

say(Channel,Agent,Event):-wantsEvents(Channel,Agent,In,Out,'TELNETDX'),!,
   toEnglish(Channel,Agent,Event,English),format(Out,'~w~n> ',[English]),flush_output(Out).
say(Channel,Agent,Bach):-wantsEvents(Channel,Agent,In,Out,'Packed'),!,
   catch(bachToPackedChars(Bach,Chars),_,fail),
   wantsEvents(Channel,Agent,In,Out,Transport),!,
   format(user_error,'~n~q ~s~n~n',[Channel,Agent,Chars]),flush_output(user_error),
   catch((format(Out,'~s',[Chars]),flush_output(Out)),_,killClient(CC)).

toEnglish(Channel,Agent,map(Event),English):-member(parents:list([OP]),Event),
      (sameString(OP,"info");sameString(OP,"sight");sameString(OP,"sound")),
      member(args:RealE,Event),!,toEnglish(Channel,Agent,RealE,English).     
toEnglish(Channel,Agent,Event,English):-
      flag(indent,_,0),
      toMarkupFormula(bach,Event,[],English).
toEnglish(Channel,Agent,English,English).


say(Channel,Format):-expandFormat(Format,GO),ignore(rememberSaidIt(GO)),!,sayLine(Channel,GO),!.

%expandFormat(Format,GO):-computeAnswer(5,Format,G,_),flatten([G],GO),!.
expandFormat(Format,Format).
%expandFormat(Format,GO):-toMarkup(5,Format,G,_),flatten([G],GO),!.

% say/3
say(Channel,Format,Args):-is_file_descriptor(Channel),!,!,catch(format(Channel,Format,Args),_,true).
say(Channel,Format,[A|RGS]):-atom(Format),catch(sformat(String,Format,[A|RGS]),_,fail),!,sayLine(Channel,String).
%say(Channel,Format,Args):-!,sformat(String,Format,Args),sayLine(Channel,String).

is_file_descriptor(user_error).
is_file_descriptor(user_output).
is_file_descriptor(user_input).
is_file_descriptor('$stream'(_)).
is_file_descriptor('$socket'(_)).

sayLine(Channel,[]):-!,sayLine(Channel,'\n').
sayLine(Channel,String):-is_string(String),!,string_to_atom(String,Atom),sayLine(Channel,Atom).
sayLine(Channel,Format):-is_string(Channel),!,string_to_atom(Channel,Atom),sayLine(Atom,Format).
sayLine(Channel,[T|O]):-not(integer(T)),toCycApiExpression_l([T|O],[],Format),!,sayLine(Channel,Format).
sayLine(Channel,Format):-is_file_descriptor(Channel),!,write(Channel,Format).
sayLine(Channel,Format):-!,%toMarkup(chat,Format,_,Out),
      term_to_string(Format,String),string_to_atom(String,AFormat),
      channel_say(Channel,AFormat),!.
sayLine(Channel,Format):-
	 catch((
	 tcp_socket(Socket),
         tcp_connect(Socket,'localhost':4447),
         tcp_open_socket(Socket,InStream,OutStream),
	 writeSTDERR('~q~n',[sayLine(Channel,Format)]),
	   sleep(0.2),
	   format(OutStream,'~w:~w~n~n',[Channel,Format]),
	   flush_output(OutStream),
	   sleep(0.2),
	   tcp_close_socket(Socket)
	   ),
	 E,writeSTDERR(E)).

:-dynamic(default_channel/1).
default_channel(user_output).
:-dynamic(default_user/1).
set_default_channel(Channel):-retractall(default_channel(_)),assert(default_channel(Channel)).
set_default_user(Agent):-retractall(default_user(_)),assert(default_user(Agent)).

:-dynamic(saved_note/4).
%sendNote(Agent,Channel,Subj,Data):-!.%say(sendNote(Agent,Channel,Subj,Data)).
% ================================================================
%   Transform Signals to Objects
% ================================================================

% ===================================================================
% writeObject(-Prolog)
%
% Replaces writeq in some cases
% ===================================================================
writeObject(Term):-!,writeObject(Term,Vars).
writeObject(Term,Vars):-toMarkup(html,Term,Vars,Chars),write(Chars).

% ================================================================
%   Serialize Objects to XML
% ================================================================
writeObject(quiet,Term,Vars):-!.

writeObject(Verbose,Term,Vars):-writeObject(Term,Vars).

		
writeObject(OBJ,Vars):- isMooOption(client=html),!,
		((toMarkup(html,OBJ,Vars,Chars),say(Chars))),!.
		
writeObject(OBJ,Vars):- isMooOption(client=atomata),!,
		((toMarkup(cycl,OBJ,Vars,Chars),say(Chars))),!.

writeObject(OBJ,Vars):- isMooOption(client=console),!,
		((toMarkup(cycl,OBJ,Vars,Chars),say(Chars))),!.

writeObject(OBJ,Vars):- !,
		((toMarkup(cycl,OBJ,Vars,Chars),say(Chars))),!.


writeObject_conj(A,Vars):-isSlot(A),!,
	writeObject(A,Vars).

writeObject_conj(and(A,true),Vars):-!,
	writeObject_conj(A,Vars).

writeObject_conj(and(true,A),Vars):-!,
	writeObject_conj(A,Vars).

writeObject_conj(and(A,B),Vars):-!,
	writeObject_conj(A,Vars),
	writeObject_conj('\n\n Also \n\n ',Vars),
	writeObject_conj(B,Vars).

writeObject_conj(Output,Vars):-
	%write(Output),nl.
	writeObject(Output,Vars).


