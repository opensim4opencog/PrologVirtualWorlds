:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% ATLAS Client Chooses CODEC (Mode)
% =================================================================================
% =================================================================================
negotateType(Channel,Agent,'TELNETDX',ClientSocket, In, Out,'TELNETDX'):-
writeFmtFlushed(Out,'
Welcome to the LogicMoo Agency ATLAS/RKF/TIA
type:  
logon <username> <password>
create account <username> <password>
 or
chat
',[]).


negotateType(Channel,Agent,'Atlas',ClientSocket, In, Out,Transport):-
   writeFmtFlushed(Out,'ATLAS logicmoo.titan\n',[]),
   read_line_with_nl(In, Codes, []),
   writeSTDERROnDebug('client: ~s~n.',[Codes]),
   getICanList(In,Offers),!,
   writeSTDERROnDebug('client can: ~w~n.',[Offers]),
   chooseCan(Offers,Transport),
   writeSTDERROnDebug('server will: ~w~n.',[Transport]),
   writeFmtFlushed(Out,'IWILL ~w\n\n',[Transport]).

getICanList(In,Offers):-
   getICanList(In,[],Offers).
getICanList(In,PrevousOffers,FinalOffers):-
	 readUntil(10,In, Codes),writeSTDERROnDebug('client codes: ~w~n.',[Codes]),
	 (blankCodes(Codes) -> FinalOffers=PrevousOffers ; 
	 (convertToOffer(Codes,Offer),
	 getICanList(In,[Offer|PrevousOffers],FinalOffers))),!.

chooseCan(Offers,Choice):-member(Choice,['Packed','Bach','XML']),memberchk(Choice,Offers),!.
%chooseCan(Offers,Choice):-member(Choice,['Bach','Packed','XML']),memberchk(Choice,Offers),!.
chooseCan([Choice],Choice).
chooseCan(_,'Packed').

blankCodes([_,10]).
blankCodes([13]).
blankCodes([]).

convertToOffer([Code|New],Offer):-char_type(Code,space),!,convertToOffer(New,Offer).
convertToOffer(Codes,Offer):-append(New,[Code],Codes),char_type(Code,space),!,convertToOffer(New,Offer).
convertToOffer(Codes,Offer):- append("ICAN",New,Codes),!,convertToOffer(New,Offer).
convertToOffer(Codes,Offer):- atom_codes(Offer,Codes).

modeType(english,'TELNETDX').
modeType(prolog,'PROLOG').
modeType(soap,'SOAP').
modeType(X,X).

clientEvent(Channel,Agent,english(phrase([mode,Type],_),Packet)):-
	 modeType(Type,INNER),
	 retract(wantsEvents(Channel,Agent,In,Out,Before)),
	 assert(wantsEvents(Channel,Agent,In,Out,INNER)).

% ===========================================================
% PROLOGD for OpenCyc SERVICE
% ===========================================================
negotateType(Channel,Agent,'LOGICMOO',ClientSocket, In, Out,Transport):-!,
      peek_char(In,Char),char_to_protocol(Char,Transport).

negotateType(Channel,Agent,'UOX',ClientSocket, In, Out,'UOX'):-!,
      get0(In,A),get0(In,B),get0(In,C),get0(In,D),
      uoWriteServerlist(Out).

negotateType(Channel,Agent,Transport,ClientSocket, In, Out,Transport).


char_to_protocol('(','CycApiRequest').
char_to_protocol('G','HTTPD').
char_to_protocol('P','HTTPD').
char_to_protocol('<','SOAP').
char_to_protocol('+','JAVA').
char_to_protocol(_,'PROLOG').

