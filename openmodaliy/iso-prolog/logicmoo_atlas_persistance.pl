:-include('logicmoo_utils_header.pl').
% =================================================================================
% =================================================================================
% Blackboard Interface
% =================================================================================
% =================================================================================

:-dynamic(have/2).

wfAssert([H|T]):-G=..[H|T],!,wfAssert(G).
wfAssert(Data):- catch(Data,_,fail),!,writeSTDERROnDebug((Data)).
wfAssert(Data):-notifyChanges(Data),fail.
wfAssert(valueOf(A,B,C)):-!,asserta((valueOf(A,B,C))),!,writeSTDERROnDebug(wfAssert(valueOf(A,B,C))).
wfAssert(Data):-Data=..[B,A,C],!,wfAssert(valueOf(A,B,C)).

wfQuery([H|T]):-G=..[H|T],!,wfQuery(G).
wfQuery(valueOf(X,P,Y)):-!,valueOfQ(X,P,Y).
wfQuery(Data):- catch(Data,_,fail).
wfQuery(G):-G=..[P,X,Y],valueOfQ(X,P,Y).


saved(A,B,C,D):-var(A),!,saved_uc(A,B,C,D).
saved(A,B,C,D):-saved_pc(A,B,C,D).

saved_uc(Call,'#$MasonMt',_,[X,P,Y,'#$MasonMt']):-valueOf(X,P,Y),Call=..[P,X,Y].
saved_uc(Call,Mt,Vars,[P,Mt|Consts]):- '#$arity'(P,N,_,_),atom(P),length(List,N),Call=..[P|List],append(List,[Mt,v(VConsts,Vars)],CList),
      Prolog=..[P|CList],catch(Prolog,_,fail),VConsts=[].%,VConsts=..[_|Consts].

saved_pc(Call,'#$MasonMt',_,[X,P,Y,'#$MasonMt']):-Call=..[P,X,Y],valueOf(X,P,Y).
saved_pc(Call,Mt,Vars,[P,Mt|Consts]):- Call=..[P|List],append(List,[Mt,v(VConsts,Vars)],CList),
      Prolog=..[P|CList],catch(Prolog,_,fail),VConsts=[].%,VConsts=..[_|Consts].


getBrowseAtoms(X):-valueOf(X,id,Y).

clientEvent(Channel,Agent,english(phrase([make],That),Packet)):-!,make.



:-at_halt(endWorld).
endWorld:-logoff(ALL),fail.
endWorld:-!.
endWorld:-saveWorld.

saveWorld:- tell('world.persist'),listing(valueOf),listing(typeOf),told.


