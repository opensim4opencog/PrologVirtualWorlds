:-dynamic(ax_worldwalker/1).


invokelist([]):-!.
invokelist([Method|MethodList]):-invokemethod(Method),invokelist(MethodList).

getpropstr(A,B,C):-invoke(getpropstr(A,B),C).
setpropstr(A,B,C):-invoke(setpropstr(A,B,C)).

object_id(Object,ID):-invoke(getpropstr(Object,id),ID).
pltopl(Pl1,Pl2):-invoke(pltopl(Pl1),Pl2).

getevent(Y):-!,ax_avatar([getevent],[],Out),raw_to_formed_event(Out,Y).
%% eventlist([]):-getevent(X)


invokemethod(Method):-invokemethod(Method,Out),trueness(Out).

invokemethod(Method,Out):-invoke(Method,Out).
invokemethod(Method,Out):-invoke(invoke_eval(Method),Out).

invoke(getpropstr(activex_object(O),Y,Out)):- catch2(actx_invoke_object(activex_object(O),[Y,propget],[],Out) ),!.
invoke(getpropstr(activex_object(O),Y,Out)):- catch2(actx_invoke_object(activex_object(O),['Property'],[Y],Out) ),!.
invoke(getpropstr(X,Y,Out)):- catch2(ax_avatar([getpropstr],[X,Y],Out)),not(Out=[]),!.
invoke(getpropstr(X,Y,Out)):- ax_worldwalker(A),catch(actx_invoke_object(A,['getObjByName'],[X],TempThing)),catch(actx_invoke_object(TempThing,[Y,propget],[],Out) ),!.

invoke(Term):- invoke(Term,Out),!,trueness(Out).
invoke(Term,Out):-!,once(invoke2(Term,Out)).

invoke2(Term,X):- Term=..[Verb,Arg],term_to_atom(Arg,Atom),!,ax_avatar_funct(Verb,[Atom],X).
invoke2(Term,X):- Term=..[Verb|Args],!,ax_avatar_funct(Verb,Args,X),!.

ax_avatar_funct(Prop,Args,Value):-ax_avatar([Prop,'func'],Args,Value),!.

ax_avatar_get(Prop,Value):-ax_avatar([Prop,'propget'],[],Value).
ax_avatar_get(Prop,Args,Value):-ax_avatar([Prop,'propget'],Args,Value).
ax_avatar_put(Prop,Value):-ax_avatar([Prop,'propput'],[Value],[]).

%ax_avatar(Function,Args,Out):-real_atom(Function),real_atom(Args),ax_worldwalker(AX),actx_invoke_object(AX,[Function],[Args],SOut),string_to_atom(SOut,AOut),atom_term(AOut,Out),!.
%ax_avatar(Function,Args,Out):-real_atom(Function),ax_worldwalker(AX),actx_invoke_object(AX,[Function],Args,SOut),string_to_atom(SOut,AOut),atom_term(AOut,Out),!.
%ax_avatar(Function,Args,Out):-real_atom(Args),ax_worldwalker(AX),actx_invoke_object(AX,Function,[Args],SOut),string_to_atom(SOut,AOut),atom_term(AOut,Out),!.
ax_avatar(Function,Args,Out):-once((ax_worldwalker(AX),!,actx_invoke_object(AX,Function,Args,SOut))),ax_return(SOut,Out).
ax_return(SOut,Out):-catch((string_to_atom(SOut,AOut),atom_to_term(AOut,Out,_)),_,fail),!.
ax_return(SOut,Out):-catch(string_to_atom(SOut,Out),_,fail),!.
ax_return([],[]):-!.
ax_return(Out,Out):-!.

get_actx_items:-
	actx_create_object('PrologVirtualWorlds.AvatarMorphism',AX),assert(ax_worldwalker(AX)).
	%actx_invoke_object(AX,[iproxyAvatar,propget],[],AA),assert(ax_avatar(AA)).

:- ignore((not(ax_worldwalker(_)),get_actx_items)).

trueness(true).
trueness(user_type(_)).
trueness(_):-!,fail.


/*

Finding an Object
~~~~~~~~~~~~~~~~~~~

obj_name(NameofObj):-obj_like([name=NameofObj|_])


obj_ref(ReferenceID):-obj_like([RefID=ReferenceID|_])


obj_like([membership=memberValue|List])





propput([obj_like(_),member,member,member],Value).

propget([obj_like(_),member,member,member],Value).

propput([obj_like(_),member,thiscall],[args1,arg2]).

propget([obj_like(_),member,thiscall([args1,arg2])],ReturnValue)


*/

