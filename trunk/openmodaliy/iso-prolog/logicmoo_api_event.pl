:-include('logicmoo_utils_header.pl').
% =================================================================================
% Execute Atlas Packet from Client
% =================================================================================

% Error ops
sendEvent(Channel,Agent,[]):-!.
sendEvent(Channel,Agent,error([-1])):-!.
sendEvent(Channel,Agent,error(Codes)):-writeSTDERROnDebug('error: ~s',[Codes]),!.
sendEvent(Channel,Agent,CMD):-!,
% Invoke Pre
      findall(1,safetyEach(clientEventPre(Channel,Agent,CMD)),_),
      writeSTDERROnDebug(sendEvent(Channel,Agent,CMD)),!,
% Invoke Real
      findall(clientEvent(Channel,Agent,CMD),safetyEach(clientEvent(Channel,Agent,CMD)),Out),!,
% Invoke Unhandled
      ifThen(not(Out=[_|_]),(muh(clientEventUnhandled(Channel,Agent,CMD)),findall(1,safetyEach(clientEventUnhandled(Channel,Agent,CMD)),_))),!.

muh(X):-writeSTDERR(X),sayLine("#kif",[X]).

ifThen(Test,Then):-ignore((Test,!,Then)).

safetyEach(X):-catch(X,E,(format(user_error,'% ~q from ~q.\n',[E,X]),fail)).

% =================================================================================
% Event Rewrites (Aliases) TODO remove these
% =================================================================================
clientEvent(Channel,Agent,request(Options,Packet)):-clientEvent(Channel,Agent,Options).
clientEvent(Channel,Agent,Options):-memberchk(client='tqsystem',Options),!,ignore(invokeTestSystem(Options)).
clientEvent(Channel,Agent,english(phrase(Text,_),packet(Channel,Serial,Agent,Refno,Time))):-
      clientEvent(Channel,Agent,imperative_text(Agent,Text,packet(Channel,Serial,Agent,Refno,Time))).
clientEvent(Channel,Agent,english(phrase(This,_),_)):-clientEvent(Channel,Agent,This).
% Last Call before unkown
clientEvent(Channel,Agent,CMD):-
	 reducePackedTok(CMD,CMDO),CMD\=CMDO,!,clientEvent(Channel,Agent,CMDO).
