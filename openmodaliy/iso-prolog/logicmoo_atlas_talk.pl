:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% Execute Talk command
% =================================================================================
% =================================================================================
:- style_check(-singleton).
:- style_check(-discontiguous).
:- style_check(-atom).
:- style_check(-string).
% Authorial KR for this OP
%opHowto(['talk'],[sendOpTokensPerceptionFromAs("sound")]).
clientEvent(Channel,Agent, message(talk(concept(Set),OPTokens), packet(Channel, Refno, Agent, Serialno, Time))):-!,
      sendPerception(Agent,Refno,Channel,Agent,"sound",map(Set)).

clientEvent(Channel,Agent,english(phrase([say|List],_),packet(Channel, Refno, Agent, Serialno, Time))):-
      sendPerception(Agent,Refno,Channel,Agent,"sound",map([say:string(List)])).



