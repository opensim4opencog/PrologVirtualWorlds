end_of_file.
 %//<?
:-dynamic(f/1).
:-multifile(f/1).
:-dynamic(ff/1).
:-multifile(ff/1).

:-dynamic(c/1).
:-multifile(c/1).
:-dynamic(p/2).
:-multifile(p/2).
:-dynamic(t/1).
:-multifile(t/1).
:-dynamic(tt/2).
:-multifile(tt/2).
:-multifile(sayable/3).
/*
			Macros

f/1   fact that is in the system database and my be realivant and has no side effs (test that can bind variable)
t/1   test for an f.. and is a little weak.. must  (test that cannot bind variables)
i/1   test for an f only if all parts are bound.
c/1   call ( is an exution that may or may not bind variables )
p/2   phrase/term expansion (argument 1 and 2 are representative of the same casting)
a/1   assert is new as a t

semantic_name --> [is,represnted,by],{when,these,are,true}

*/


script --> scriptlet.
script --> scriptlet,[Script],
		{nonvar(Script),p(script,Script)}.
		
scriptlet --> prolog ; action ; state ; change ; test.

/* define prolog */

prolog --> [call(X)], {nonvar(X),catch(X,_,fail)}.

/* define action */
action --> conceptual_primitive.

conceptual_primitive --> phys_conceptual_primitive ;
		 mental_conceptual_primitive.

phys_conceptual_primitive --> ptrans ; atrans ; propel ; move ; grasp ; injest ; expel ; speak ; attend .
phys_act --> phys_conceptual_primitive.

mental_conceptual_primitive --> mtrans ; mbuild ; forget .

ptrans --> ptrans(_Avatar,_Thing,_Location).
atrans --> atrans(_Avatar,_Thing,_Location).
propel --> propel(_Avatar,_Thing,_Location,_Force).
move --> move(_Avatar,_Bpart).
grasp -->  grasp(_Avatar,_Bpart,_Thing). 
injest --> injest(_Avatar,_Thing). 
expel --> expel(_Avatar,_Thing,_Location). 
speak --> speak(_Avatar,_Text). 
attend --> attend(_Avatar,_SenseOrgan,_Location). 
mtrans --> mtrans(_Avatar,_Text,_Avatar2). 
forget --> forget(_Avatar,_Text).
mbuild --> mbuild(_Avatar,_Text).

ptrans(Avatar,Thing,Location) --> [ptrans(Avatar,Thing,Location)],{
	
    p(avatar,Avatar),
	p(pthing,Thing),
	p(location,Location),
	neq(location(Thing),Location),
	neq(Thing,Location),
	~p(thing_bodypart,Thing)
	}.

atrans(Avatar,Thing,Avatar2) --> [atrans(Avatar,Thing,Avatar2)],
	  {p(avatar,Avatar),p(athing,Thing),p(avatar,Avatar2),neq(Avatar,Avatar2)}.

atrans(Avatar,Thing,Avatar2) --> [atrans(Avatar,Thing,Avatar2)],
	  {p(avatar,Avatar),p(ownership,Thing),p(avatar,Avatar2),neq(Avatar,Avatar2)}.

propel(Avatar,Thing,Location,Force) --> [propel(Avatar,Thing,Location,Force)],
        {p(avatar,Avatar),p(pthing,Thing),p(location,Location),p(force,Force),neq(Thing,Location)}.

move(Avatar,Bpart) --> [move(Avatar,Bpart)],
        {p(avatar,Avatar),p(bodypart,Bpart),if(thing_part(Avatar,Bpart))}.

grasp(Avatar,Bpart,Thing) --> [grasp(Avatar,Bpart,Thing)],
        {p(avatar,Avatar),/* if(thing_part(Avatar,Bpart)), */p(grasper,Bpart),p(pthing,Thing),
		neq(Bpart,Thing)}.





peak(Avatar,Text) --> [speak(Avatar,Text)],
   
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            {p(avatar,Avatar),p(text,Text)}.

attend(Avatar,SenseOrgan,Location) --> [attend(Avatar,SenseOrgan,Location)],
        {p(avatar,Avatar),p(sense_organ,SenseOrgan),p(location,Location)}.

mtrans(Avatar,Text,Avatar2) --> [mtrans(Avatar,Text,Avatar2)],
        {p(avatar,Avatar),p(mtransable,Text),p(avatar,Avatar2),neq(Avatar,Avatar2)}.

forget(Avatar,Text) --> [forget(Avatar,Text)],
        {p(avatar,Avatar),p(mtransable,Text)}.

mbuild(Avatar,Text) --> [mbuild(Avatar,Text)],
        {p(avatar,Avatar),p(mtransable,Text)}.

ownership --> ownership(_Avatar,_Thing).
ownership(Avatar,Thing) --> [ownership(Avatar,Thing)],
        {p(avatar,Avatar),p(thing,Thing)}.

sayable(ownership(Avatar,Thing)) --> sayable(Avatar),[owns],sayable(Thing),
        {p(avatar,Avatar),p(thing,Thing)}.

sayable(ownership(Avatar,Thing)) --> sayable(Avatar),[does,own],sayable(Thing),
        {p(avatar,Avatar),p(thing,Thing)}.

sayable(Pred) --> sayable(Avatar),[Verb],sayable(Thing),
        	{nonvar(Pred),Pred=..[Verb,Avatar,Thing],
		 p(avatar,Avatar),p(thing,Thing)}.

infixme1(Pred) --> [A,Verb,B],
        	{nonvar(Pred),Pred=..[Verb,A,B]}.

infixme2(Pred) --> sayable(A),[Verb],sayable(B),
        	{nonvar(Pred),Pred=..[Verb,A,B]}.

infixme3(Pred) --> [AA,Verb,BB],
        	{Pred=..[Verb,A,B],p(sayable(A),[AA]),p(sayable(B),[BB])}.


% X --> [X],{c(X)}.

thing --> [thing(X)] , {if(thing(X))}.
thing --> pthing ; athing.

avatar --> bot.


pthing --> avatar.
pthing --> thing_bodypart.
pthing --> [pthing(X)], {if(pthing(X))}.
thing_bodypart --> [thing_bodypart(A,G)],{p(bodypart,[G]),p(avatar,[A])}.
athing --> [athing(X)], {if(athing(X))}.

location --> [location(X)], {if(location(X))}.
location --> thing_location.
thing_location --> [location(Thing)],{p(pthing,[Thing]),not(p(bodypart,Thing))}.

bodypart --> sense_organ.
bodypart --> limbus.
bodypart --> grasper.
bodypart --> organs.
sense_organ --> [sense_organ(X)], {if(sense_organ(X))}.
grasper --> [grasper(X)] , {if(grasper(X))}.
limbus --> [limbus(X)], {if(limbus(X))}.
organs --> [organs(X)], {if(organs(X))}.


avatar --> [avatar(X)],{if(avatar(X))}.
force --> [force(X)],{if(force(X))}.

% information --> tellable.
mtransable --> goal.
% mtransable --> [X], {nonvar(X),p(script,X)}.
mtransable --> phys_conceptual_primitive.
mtransable --> fact.

goal --> goal(_).
goal(Goal) --> [goal(Goal)],{p(fact,Goal)}.

text --> [text(Noise)],{if(text(Noise))}.

/* define state */

state --> at ; not ; do ; has ; want.
state --> [true(Cond)],{p(fact,Cond)}.
state --> [false(Cond)],{p(fact,Cond)}.

do --> can ; doing ; will.

has --> thing_part_p ; thing_prop.

want --> want_to ; want_be ; want_thing.

/*
at --> at(_thing,_location).
not --> not(_A,_B).
%can --> can(_Avatar,_Action).
will --> will(_Avatar,_Action).
doing --> doing(_Avatar,_Action).
thing_part_p --> thing_part(_Avatar,_Thing).
know --> know(_Avatar,_mtransable).
thing_prop --> thing_prop(_Avatar,_Attribute).
want_to --> want_to(_Avatar,_Action).
want_be --> want_be(_Avatar,_Attribute).
want_thing --> want_thing(_Avatar,_Thing).
*/

at --> at(_thing,_location).
at(Thing,Location) --> [at(Thing,Location)],
        {p(pthing,Thing),p(location,Location)}.

%not --> not(_A,_B).
not(A,B) --> [not(A,B)],
        {neq(A,B)}.

%can --> can(_Avatar,_Action).
can(_Avatar,_Action) --> [can(_Avatar,_Action)],
        {p(avatar,_Avatar),p(action,_Action),if(can(_Avatar,_Action))}.

%doing --> doing(_Avatar,_Action).
doing(_Avatar,_Action) --> [doing(_Avatar,_Action)],
	{p(avatar,_Avatar),p(action,_Action),if(doing(_Avatar,_Action))}.

will --> will(_Avatar,_Action).
will(_Avatar,_Action) --> [will(_Avatar,_Action)],
	{p(avatar,_Avatar),p(action,_Action),c(will(_Avatar,_Action))}.
		t(will(_Avatar,_Action)):-writeDebug(t,will(_Avatar,_Action)).

thing_part_p --> thing_part(_Avatar,_Thing).
thing_part(_Thing1,_Thing1) --> [thing_part(_Thing1,_Thing2)],
	{p(pthing,_Thing1),p(pthing,_Thing2),neq(_Thing1,_Thing2),c(thing_part(_Thing1,_Thing2))}.
		t(thing_part(_Thing1,_Thing2)):-writeDebug(t,thing_part(_Thing1,_Thing2)).

know --> know(_Avatar,_mtransable).
know(_Avatar,_mtransable) --> [know(_Avatar,_mtransable)],
	{p(avatar,_Avatar),p(mtransable,_mtransable),c(know(_Avatar,_mtransable))}.
		t(know(_Avatar,_mtransable)):-writeDebug(t,know(_Avatar,_mtransable)).

thing_prop --> thing_prop(_Avatar,_Attribute).
thing_prop(_Avatar,_Attribute) --> [thing_prop(_Avatar,_Attribute)],
	{if(thing_prop(_Avatar,_Attribute))}.

want_to --> want_to(_Avatar,_Action).
want_to(_Avatar,_Action)--> [want_to(_Avatar,_Action)],
        {if(want_to(_Avatar,_Action))}.

want_be --> want_be(_Avatar,_Attribute).
want_be(_Avatar,_Attribute)--> [want_be(_Avatar,_Attribute)],
        {if(want_be(_Avatar,_Attribute))}.

want_thing --> want_thing(_Avatar,_Thing).
want_thing(_Avatar,_Thing)--> [want_thing(_Avatar,_Thing)],
        {if(want_thing(_Avatar,_Thing))}.

/*
			% prototype Phrase %
Phrase--> [Phrase],
	{Phrase}.
		Phrase:-writeDebug(try,Phrase).
*/

fact --> [has(Thing,is(Attrib,Value))], {p(thing,Thing),c(bindable(Thing,Attrib,Value))}.
fact --> [at(PThing,Location)], {p(pthing,PThing),p(location,Location)}.
fact --> ability.

ability --> [can(Avatar,Action)], {p(avatar,Avatar),p(phys_conceptual_primitive,Action)}.

causation --> causationp.
causation --> causationp , causation .
%// causationp --> results ; enables ; initiates ; reason ; disable.
causationp -->[results(Action,State)],{p(state,State),p(action,Action)}. /* ^r */
causationp --> [enables(State,Action)],{p(state,State),p(action,Action)}. /* ^E */
causationp --> [initiates(State,Mental)],{p(state,State),p(mental_state,Mental)}. /* ^I */
causationp --> [initiates(Action,Mental)],{p(action,Action),p(mental_state,Mental)}. /* ^I */
causationp --> [reason(Mental,Action)],{p(phys_act,Action),p(mental_state,Mental)}. /* ^R */
causationp --> [disable(State,Action)],{p(state,State),p(action,Action)}. /* ^dE */


change --> change(_Thing,_Attrib,_OldValue,_NewValue).
change(Thing,Attrib,OldValue,NewValue) --> [change(Thing,Attrib,OldValue,NewValue)],
        {
	p(thing,Thing),
	if(bindable(Thing,Attrib,OldValue)),
	if(bindable(Thing,Attrib,NewValue)),
	neq(OldValue,NewValue)
	}.

/* now we undertake primitive scripts */

restraunt_script --> 
	restraunt_script(entry),
 	restraunt_script(order),  
	restraunt_script(eat), 
	restraunt_script(leave).

walks(Avatar,Thing) --> sayable(Avatar),[walks,to],sayable(Thing),
                {
                p(ptrans,ptrans(Avatar,Avatar,location(Thing)))
                }.

walks(Avatar,Thing) --> sayable(Avatar),[walks,to],sayable(Thing),
                {
                p(ptrans,ptrans(Avatar,Avatar,Thing))
                }.

/*
% hard prototypes
sayable(avatar(Name),[Name]):-!.
sayable(location(avatar(Name)),[Name]):-!.
sayable(location(Name),[Name]):-!.
sayable(thing_part(Avatar,Part),[SayPart,of,SayAvatar]):-!,
	sayable(Part,SayPart),
	sayable(Avatar,SayAvatar).
sayable(pthing(Thing),[Thing]):-!.
sayable(sense_organ(Sense),Sense):-!.
sayable(Anything,[Anything]):-nonvar(Anything).
*/

% softer prototypes
sayable(avatar(Avatar)) --> [Avatar],
	{if(avatar(Avatar))}.
sayable(location(Name)) --> [at],sayable(Name), {nonvar(Name)}.
sayable(thing_part(Avatar,Part)) --> sayable(Part),[of],sayable(Avatar),
	{
	nonvar(Avatar),nonvar(Part)
	}.

	%	sayable(bodypart(Part)),[of],sayable(avatar(Avatar)),
	%		{if(bodypart(Part)),if(avatar(Avatar))}.
sayable(pthing(Thing))--> [Thing],
	{if(pthing(Thing))}.
sayable(athing(Thing))--> [Thing],
        {if(athing(Thing))}.

sayable(sense_organ(Sense)) --> [Sense],
	{if(sense_organ(Sense))}.
sayable(bodypart(Bpart))-->[Bpart],
	{if(bodypart(Bpart))}.

sayable(Anything) --> [Anything],{ground(Anything)}.

		
english --> walks(_,_).


restraunt_script(entry) --> 
		get_to_location(Avatar,Restraunt),
		find_location_of_thing(Avatar,Table,Location),
		get_to_location(Avatar,Location),
		change(Avatar,position,_,sitting),
			{ 
			p(avatar,Avatar),
			p(restraunt,Restraunt),
			p(called(table),Table),
			p(location,Location)
			}.

get_to_location(Avatar,Location) --> 
		attend(Avatar,Sense,Location),
	%	[mbuild(Avatar,[goal([d-know(location(Location))])])],
		move(Avatar,Bparts),
		ptrans(Avatar,Avatar,Location),
			{
			p(avatar,Avatar),
			p(location,Location),
			p(sense_organ,Sense),
			p(bodypart,Bparts)
			}.

get_thing(Avatar,Thing) --> 
		find_location_of_thing(Avatar,Thing,Location),
		get_to_location(Avatar,Location),
		pick_up(Avatar,Thing,Location),
		{p(avatar,Avatar),p(location,Location),p(thing,Thing)}.

find_location_of_thing(Avatar,Thing,Location) -->
		attend(Avatar,Sense,Location),
		at(Thing,Location),
		{p(avatar,Avatar),p(location,Location),p(thing,Thing),p(sense_organ,Sense)}.

find_location_of_thing(Avatar,Thing,Location) -->
		at(Avatar,ThisLocation),
		attend(Avatar,Sense,ThisLocation),
			not(at(ThisLocation,Thing)),
		get_to_location(Avatar,NewLocation),
		find_location_of_thing(Avatar,Thing,Location),
		{p(avatar,Avatar),p(location,Location),p(location,NewLocation),p(location,ThisLocation),neq(ThisLocation,NewLocation),p(sense_organ,Sense)}.

pick_up(Avatar,Thing,Location)-->
		at(Thing,Location),
		at(Avatar,Location),
		grasp(Avatar,Bpart,Thing),
		{p(grasper,Bpart)}.
		
		
%		,{p(avatar,Avatar),p(location,Location),p(thing,Thing),p(bpart,Bpart)}.

		

/* physical execution */

/*
ptrans(Avatar,Avatar,Location) :-!.

restraunt_script(2) --> [
		factual(at(Menu,Table)),
		
*/
		
		
% ?>		

