:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% Execute Login command
% =================================================================================.
% =================================================================================
clientEvent(Channel,Agent,english(phrase([login,Username,Password],That),Packet)):-!,
      clientEvent(Channel,Agent,english(phrase([logon,Username,Password],That),Packet)).

clientEvent(Channel,Agent,english(phrase([logon,Username,Password],_),Packet)):-
	 login_command(account,Channel,Agent,Serialno,Username,Id,Password,login(account,Username,Password)).

% Login Something 
clientEvent(Channel,Agent,message(login(object(PlayerType, List),OPTokens), Packet)):-!,
	 atlasMemberValue(password,Password,List),
	 (atlasMemberValue(username,Username,List);atlasMemberValue(name,Username,List);atlasMemberValue(id,Username,List)),!,
	 login_command(PlayerType,Channel,Agent,Packet,Username,Username,Password,List).

% Player not found
login_command(PlayerType,Channel,Agent,packet(Channel, Refno, Agent, Serialno, Time),Username,Id,Password,List):-
      not(isUserNameValid(Username)),
      writeSTDERR('Player not found ~w ~n',[Username]),
      sendError(Channel,Agent,Serialno,'Login is invalid',map(List)),!.
% Player found  (check password)
/*
login_command(PlayerType,Channel,Agent,Serialno,Username,Id,Password):-
      writeSTDERR('Player found'),
	 not(wfQuery(valueOf(Id,password,Password)),
*/
% Player found  (passed password check)
login_command(PlayerType,Channel,Agent,packet(Channel, Refno, Agent, Serialno, Time),Username,Id,Password,List):-
          clientEvent(Channel,Agent,get(thing(Id),packet(Channel, Refno, Agent, Serialno, Time))),
	 playerCharInGameNow(Id,Serialno,Channel,Agent,Id).


	
