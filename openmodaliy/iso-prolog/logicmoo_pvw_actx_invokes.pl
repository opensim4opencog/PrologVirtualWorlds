end_of_file.
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


:-dynamic(instance_type_objname/2).
:-dynamic(learned/1).

gps(Thing,Property,Value).

object_template(thing(ObjName)):-instance_type_objname(thing,ObjName).
object_template(thing(ObjName)):-ground(ObjName),
        instance_type_objname(InstanceType,ObjName),
        ancestor(InstanceType,thing).

object_template(Pred):-
        catch2(Pred =.. [InstanceType,ObjName]),
        instance_type_objname(InstanceType,ObjName).

object_template(Pred):-
        catch2(Pred =.. [Type,ObjName]),
        instance_type_objname(InstanceType,ObjName),
        ancestor(InstanceType,Type).


instance_type_objname(Type,ObjName):-learned(instance_type_objname(Type,ObjName)).
instance_type_objname(Type,ObjName):-base_class_syntax(Type,ObjName).


base_class_syntax(vector,vector(1,1,1,1)).
base_class_syntax(vector,vector(_,_,_,_)).
base_class_syntax(vector,vector(1,1,1)).
base_class_syntax(vector,vector(_,_,_)).
base_class_syntax(ipropertylist,ipropertylist([])).
base_class_syntax(ipropertylist,ipropertylist(X)):-is_list(X).
base_class_syntax(ipropertymap,ipropertymap([])).
base_class_syntax(ipropertymap,ipropertymap(NameValuePairs)):-is_name_value_pairs(NameValuePairs).
base_class_syntax(array,array([])).
base_class_syntax(array,array(X)):-is_list(X).
base_class_syntax(string,string([])).
base_class_syntax(string,string(X)):-is_list(X).
base_class_syntax(string,string(X)):-atom(X).
%%base_class_syntax(string,X):-base_class_syntax(string,string(X)).



is_name_value_pairs([]):-!.
is_name_value_pairs([Property=Value|NameValuePairs]):-legal_pair(Property=Value),is_name_value_pairs(NameValuePairs).


legal_pair(_Property=_Value):-!.


ancestor(X,X).


ax_create(X):-actx_create_object(X,F),assert(foo(F)).

myguid('{2A24CF73-DFB2-4208-BEF3-2AD1214143E1}').
myt('{27EE3935-66AD-403E-BB2E-299AB01A3FFF}').
av('{19C15475-D70D-42E7-81EA-B732700CD4B4}').



obj_prop_get(X,Y,Z).

obj_prop_get(ithing('Admin'),Y,Z).


obj_prop_put(X,Y,Z).



