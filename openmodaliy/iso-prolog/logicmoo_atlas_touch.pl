:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% Execute Touch command
% =================================================================================
% =================================================================================
% Authorial KR for this OP
opHowto(['touch'],[sendOpTokensPerceptionFromAs("feel"),sendOpTokensPerceptionFromTo("feel")]).

% touch Something 
clientEvent(Channel,Agent, message(touch(thing(Id),OPTokens), packet(Channel, Refno, Agent, Serialno, Time))):-!,
   	getObjectRefInfo(Agent,Map),
   	getObjectMap(Agent,Toucher),
	 writeMessageOp(Channel,Agent,Id,Agent,Refno,['feel'],Map),!, %feeler
	 writeMessageOp(Channel,Agent,Agent,Id,Refno,['feel'],Toucher),!. %touchee


%[args:list([map([id:bigguy1])]), parents:list([touch]), objtype:op]

% ==================================================
% Turning Things On/Off
% ==================================================

clientEvent(Channel,Agent,[turn,Thing,OnOff]):-switch(Thing,OnOff).

switch(Thing,Status):-
	retractall(switch_status(Thing,_)),	% remove the old value
	asserta_if_new(has_switch(Thing)),  % add the new status
	assert(switch_status(Thing,Status)).  % add the new status


:-dynamic(has_switch/1).
:-dynamic(switch_status/2).

has_switch('washing machine'). % the washing machine has a switch
switch_status('washing machine',off). % the washing machine is off


