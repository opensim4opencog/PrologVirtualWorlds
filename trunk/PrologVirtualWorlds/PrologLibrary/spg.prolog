% <?
:-dynamic(f/1).
:-multifile(f/1).
:-dynamic(c/1).
:-multifile(c/1).
:-dynamic(p/2).
:-multifile(p/2).
:-dynamic(t/1).
:-multifile(t/1).

/*

f/1   fact that is in the system database and my be realivant and has no side effects (test that can bind variable)
t/1   test for an f.. and is a little fuzzy.. must  (test that cannot bind variables)
c/1   call ( is an exectution that may or may not bind variables )
p/2   phrase/term expansion


*/

write_debug(Who,What):-writeq(Who=What),nl,ttyflush.

:-[data].

c(X):-nonvar(X),catch(t(X),_,fail),!.
c(X):-nonvar(X),catch(X,_,fail),write_debug(c,proved(X)).
c(X):-nonvar(X),!,write_debug(c,failed(X)),fail.
c(_):-write_debug(c,unbound),fail.

script --> scriptlet.
script --> scriptlet, script.

scriptlet --> prolog ; action ; state ; change ; test.

/* define prolog */

prolog --> [call(X)], {nonvar(X),c(X)}.

/* define action */
action --> conceptual_primitive.

conceptual_primitive --> phys_conceptual_primitive ; mental_conceptual_primitive.

phys_conceptual_primitive --> ptrans ; atrans ; propel ; move ; grasp ; injest ; expel ; speak ; attend .
phys_act --> phys_conceptual_primitive.

mental_conceptual_primitive --> mtrans ; mbuild ; forget .

ptrans --> ptrans(_Actor,_Obj1,_Loc).
atrans --> atrans(_Actor,_Obj,_Loc).
propel --> propel(_Actor,_Obj,_Loc,_Force).
move --> move(_Actor,_Bpart).
grasp -->  grasp(_Actor,_Bpart,_Obj). 
injest --> injest(_Actor,_Obj). 
expel --> expel(_Actor,_Obj,_Loc). 
speak --> speak(_Actor,_Sounds). 
attend --> attend(_Actor,_SenseOrgan,_Loc). 
mtrans --> mtrans(_Actor,_Info,_Actor2). 
forget --> forget(_Actor,_Info).
mbuild --> mbuild(_Actor,_Info).


ptrans(Actor,Obj,Loc) --> [ptrans(Actor,Obj1,Loc)],{
        p(actor,Actor),
	p(pobj,Obj),
	p(loc,Loc),
	c(neq(Obj,Loc)),
	c(not(p(bpart_obj,Obj))),
	c(not(t(at(Obj1,Loc))))
	}.

atrans(Actor,Obj,Loc) --> [atrans(Actor,Obj,Loc)], {
	p(actor,Actor),
	p(aobj,Obj),
	p(loc,Loc)
	}.

propel(Actor,Obj,Loc,Force) --> [propel(Actor,Obj,Loc,Force)],
        {p(actor,Actor),p(pobj,Obj),p(loc,Loc),p(force,Force),neq(Obj,Loc)}.

move(Actor,Bpart) --> [move(Actor,Bpart)],
        {p(actor,Actor),p(bpart,Bpart),t(has_part(Actor,Bpart))}.

grasp(Actor,Bpart,Obj) --> [grasp(Actor,Bpart,Obj)],
        {p(actor,Actor),t(has_part(Actor,Bpart)),p(grasper,Bpart),p(pobj,Obj),neq(Bpart,Obj)}.

injest(Actor,Obj) --> [injest(Actor,Obj)],
        {p(actor,Actor),p(pobj,Obj),neq(Actor,Obj),not(p(bpart,Obj))}.

expel(Actor,Obj,Loc) --> [expel(Actor,Obj,Loc)],
        {p(actor,Actor),p(intakable,Obj),p(loc,Loc),neq(Actor,Obj),neq(Obj,Loc)}.

speak(Actor,Sounds) --> [speak(Actor,Sounds)],
        {p(actor,Actor),p(sounds,Sounds)}.

attend(Actor,SenseOrgan,Loc) --> [attend(Actor,SenseOrgan,Loc)],
        {p(actor,Actor),p(sense_organ,SenseOrgan),p(loc,Loc)}.

mtrans(Actor,Info,Actor2) --> [mtrans(Actor,Info,Actor2)],
        {p(actor,Actor),p(mtransable,Info),p(actor,Actor2),neq(Actor,Actor2)}.

forget(Actor,Info) --> [forget(Actor,Info)],
        {p(actor,Actor),p(mtransable,Info)}.

mbuild(Actor,Info) --> [mbuild(Actor,Info)],
        {p(actor,Actor),p(mtransable,Info)}.

% X --> [X],{c(X)}.

obj --> [obj(X)] , {c(obj(X))}.
obj --> pobj ; aobj.

pobj --> actor.
pobj --> bpart_obj.
pobj --> [pobj(X)], {c(pobj(X))}.
bpart_obj --> [bpart_obj(G,A)],{p(bpart,G),p(actor,A)}.
aobj --> [aobj(X)], {c(aobj(X))}.

loc --> [loc(X)], {c(loc(X))}.
loc --> obj_loc.
obj_loc --> [loc(Obj)],{p(pobj,Obj),not(p(bpart,Obj))}.

bpart --> sense_organ.
bpart --> limbus.
bpart --> grasper.
bpart --> organs.
sense_organ --> [sense_organ(X)], {c(sense_organ(X))}.
grasper --> [grasper(X)] , {c(grasper(X))}.
limbus --> [limbus(X)], {c(limbus(X))}.
organs --> [organs(X)], {c(organs(X))}.


actor --> [actor(X)],{c(actor(X))}.
force --> [force(X)],{c(force(X))}.

% information --> tellable.
mtransable --> goal.
% mtransable --> [X], {nonvar(X),p(script,X)}.
mtransable --> phys_conceptual_primitive.
mtransable --> fact.

goal --> goal(_).
goal(Goal) --> [goal(Goal)],{p(fact,Goal)}.

 
sounds --> [sound(Noise)],{c(sound(Noise))}.

/* define state */

state --> at ; not ; do ; has ; want.

do --> can ; doing ; will.

has --> has_part_p ; has_attribute.

want --> want_to ; want_be ; want_obj.

at --> at(_obj,_loc).
not --> not(_A,_B).
can --> can(_Actor,_Action).
doing --> doing(_Actor,_Action).
will --> will(_Actor,_Action).
has_part_p --> has_part(_Actor,_Obj).
know --> know(_Actor,_mtransable).
has_attribute --> has_attribute(_Actor,_Attribute).
want_to --> want_to(_Actor,_Action).
want_be --> want_be(_Actor,_Attribute).
want_obj --> want_obj(_Actor,_Obj).

at(Obj,Loc) --> [at(Obj,Loc)],
        {p(pobj,Obj),p(loc,Loc),neq(Obj,Loc)}.

not(A,B) --> [not(A,B)],
        {neq(A,B)}.

can(A,G) --> [can(A,G)],
        {c(can(A,G))}.
		t(can(A,G)):-write_debug(t,can(A,G)).

doing(_Actor,_Action) --> [doing(_Actor,_Action)],
	{p(actor,_Actor),p(action,_Action),c(doing(_Actor,_Action))}.
		t(doing(_Actor,_Action)):-write_debug(t,doing(_Actor,_Action)).

will(_Actor,_Action) --> [will(_Actor,_Action)],
	{p(actor,_Actor),p(action,_Action),c(will(_Actor,_Action))}.
		t(will(_Actor,_Action)):-write_debug(t,will(_Actor,_Action)).

has_part(_Obj1,_Obj1) --> [has_part(_Obj1,_Obj2)],
	{p(pobj,_Obj1),p(pobj,_Obj2),neq(_Obj1,_Obj2),c(has_part(_Obj1,_Obj2))}.
		t(has_part(_Obj1,_Obj2)):-write_debug(t,has_part(_Obj1,_Obj2)).

know(_Actor,_mtransable) --> [know(_Actor,_mtransable)],
	{p(actor,_Actor),p(mtransable,_mtransable),c(know(_Actor,_mtransable))}.
		t(know(_Actor,_mtransable)):-write_debug(t,know(_Actor,_mtransable)).

has_attribute(_Actor,_Attribute) --> [has_attribute(_Actor,_Attribute)],
	{c(has_attribute(_Actor,_Attribute))}.
		t(has_attribute(_Actor,_Attribute)):-write_debug(t,has_attribute(_Actor,_Attribute)).

want_to(_Actor,_Action)--> [want_to(_Actor,_Action)],
        {c(want_to(_Actor,_Action))}.
                t(want_to(_Actor,_Action)):-write_debug(t,want_to(_Actor,_Action)).

want_be(_Actor,_Attribute)--> [want_be(_Actor,_Attribute)],
        {want_be(_Actor,_Attribute)}.
                t(want_be(_Actor,_Attribute)):-write_debug(t,want_be(_Actor,_Attribute)).

want_obj(_Actor,_Obj)--> [want_obj(_Actor,_Obj)],
        {want_obj(_Actor,_Obj)}.
                t(want_obj(_Actor,_Obj)):-write_debug(t,want_obj(_Actor,_Obj)).

/*
			% prototype Phrase %
Phrase--> [Phrase],
	{Phrase}.
		Phrase:-write_debug(try,Phrase).
*/



state --> [true(Cond)],{p(fact,Cond)}.
state --> [false(Cond)],{p(fact,Cond)}.
state --> [true(Cond)],{nonvar(Cond),c(fact(Cond))}.

fact --> [has(Obj,is(Attrib,Value))], {p(obj,Obj),c(bindable(Obj,Attrib,Value))}.
fact --> [at(PObj,Loc)], {p(pobj,PObj),p(loc,Loc)}.
fact --> ability.

ability --> [can(Actor,Action)], {p(actor,Actor),p(phys_conceptual_primitive,Action)}.

causation --> causationp.
causation --> causationp , causation .
%// causationp --> results ; enables ; initiates ; reason ; disable.
causationp -->[results(Action,State)],{p(state,State),p(action,Action)}. /* ^r */
causationp --> [enables(State,Action)],{p(state,State),p(action,Action)}. /* ^E */
causationp --> [initiates(State,Mental)],{p(state,State),p(mental_state,Mental)}. /* ^I */
causationp --> [initiates(Action,Mental)],{p(action,Action),p(mental_state,Mental)}. /* ^I */
causationp --> [reason(Mental,Action)],{p(phys_act,Action),p(mental_state,Mental)}. /* ^R */
causationp --> [disable(State,Action)],{p(state,State),p(action,Action)}. /* ^dE */


change --> change(_Obj,_Attrib,_OldValue,_NewValue).
change(Obj,Attrib,OldValue,NewValue) --> [change(Obj,Attrib,OldValue,NewValue)],
        {p(obj,Obj),bindable(Obj,Attrib,OldValue),bindable(Obj,Attrib,NewValue),neq(OldValue,NewValue)}.

/* now we undertake primitive scripts */

restraunt_script --> restraunt_script(entry), restraunt_script(order),  restraunt_script(eat), restraunt_script(leave).

restraunt_script(entry) --> 
		get_to_place(Actor,Restraunt),
		find_location_of_object(Actor,Table,Location),
		get_to_place(Actor,Location),
		change(Actor,position,_,sitting),
		{ p(actor,Actor),p(restraunt,Restraunt),p(called(table),Table),p(loc,Location)}.

get_to_place(Actor,Place) --> 
		attend(Actor,Sense,Place),
		mbuild(Actor,[goal([d-know(loc(Place))])]),
		move(Actor,Bparts),
		ptrans(Actor,Actor,Place),
		{p(actor,Actor),p(place,Place),p(sense_organ,Sense),p(bpart,Bparts)}.

get_obj(Actor,Object) --> 
		find_location_of_object(Actor,Object,Location),
		get_to_place(Actor,Location),
		pick_up(Actor,Object,Location),
		{p(actor,Actor),p(loc,Location),p(obj,Object)}.

find_location_of_object(Actor,Object,Location) -->
		attend(Actor,Sense,Location),
		at(Location,Object),
		{p(actor,Actor),p(loc,Location),p(obj,Object),p(sense_organ,Sense)}.

find_location_of_object(Actor,Object,Location) --> [
		at(Actor,ThisPlace),
		attend(Actor,Sense,ThisPlace),
		not(at(ThisPlace,Object)),
		get_to_place(Actor,NewPlace),
		find_location_of_object(Actor,Object,Location)
		],{p(actor,Actor),p(loc,Location),p(loc,NewPlace),p(loc,ThisPlace),neq(ThisPlace,NewPlace),p(sense_organ,Sense)}.



/* physical execution */

/*
ptrans(Actor,Actor,Place) :-!.

restraunt_script(2) --> [
		factual(at(Menu,Table)),
		
*/
		
		
% ?>		
