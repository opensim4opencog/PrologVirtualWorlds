
onuserconnect(X):-new_fact(is_actor(X)).

onuievent(onenterworld(X)):-new_fact(person(X)),new_fact(is_here(X)).

onuievent(ongeometrychanged(X)):-new_fact(is_object(X)).


onpropertychanged('global',X,100,""):-new_fact(is_object(X)).

onpropertychanged('global',"Exemplars",0,X):-new_fact(is_object(X)).

onpropertychanged('global',_ ,100,_):- ignore.
onpropertychanged('global',_ ,101,_):- ignore.
onpropertychanged('global',_,100 ):- ignore.
onpropertychanged('global',_,101 ):- ignore.

onpropertychanged(Who,"GeometryName",101,Skin):-new_fact(is_object(Who)),new_fact(object_desc(Who,Skin)).

onpropertychanged(Who,"Geometry",0,Skin):-new_fact(is_object(Who)),new_fact(object_desc(Who,Skin)).


onpropertychanged(Who,"Profile",0,ProfilePart):-new_fact(is_object(Who)),new_fact(is_property(ProfilePart)),
			new_fact(has_part(Who,ProfilePart) ).


onuievent(onleaveworld(X)):-old_fact(is_here(X)).

onpropertychanged(_,"IsConnected",101):-ignore.

onuievent(ongeometrychanged('Avatar')):-new_fact(is_object(X)).

onuievent(action(Room,_,[Who,Me],Text)):-append(_,"approaches you.",Text),
		new_fact(is_place(Room)),
		new_fact(is_object(Who)),new_fact(is_actor(Who)),
		new_fact(is_object(Me)),new_fact(is_actor(Me)),
		new_event(approach(Who,Me)).

onuievent(say(Room,Who,Text)):-
		new_fact(is_place(Room)),
		new_fact(is_object(Who)),new_fact(is_actor(Who)),
		new_event(say(Room,Who,Text)).


onpropertychanged(_,"Profile",1,"Ignoring"):-ignore.

onuievent(ignoredbyuser(Target,Who)):-
		new_fact(is_object(Target)),new_fact(is_actor(Target)),
		new_fact(is_object(Who)),new_fact(is_actor(Who)),
		new_event(ignored(Target,Who)).


onintialize(start):-ignore.




%onpropertychanged('Avatar',"Position",3)
%onpropertychanged('Avatar',"Direction",3)



%onuievent(say('RoomName','Avatar',"Action"))onuievent(emote('RoomName','Avatar',"Action"))

%onreport(vwPage,"Avatar whispers to FriendlyName, "Whisper"")

%onpropertychanged('Avatar',"Position",3)

%onpropertychanged('Avatar',"Direction",3)


formed_invokation_to_VBScript(Request,VBS):-
	Request=..[Verb,_Nill|Vargs],
	length(Vargs,Num),length(Args,Num),
	ProtoType=..[Verb,_Null|Args],
	formed_invokation(ExemplarObject,ProtoType,_ReturnValue),
	exemplar_to_vbs(ExemplarObject,VBSObj),
	try(concat_atom([VBSObj,Verb],'.',VBS1)),
	args_to_objects(Args,Vargs,Objects),
	try(concat_atom(Objects,',',VBSObjects)),
	try(concat_atom([VBS1,VBSObjects],' ',VBS)),!.

formed_invokation_to_VBScript(Prolog,VBScript):-
	Prolog=..[Method,_FirstArg],!,
	aformat(VBScript,'user.~w',[Method]).
	
formed_invokation_to_VBScript(say(_Avatar,Mtrans),VBScript):-
	is_list(Mtrans),
	concat_atom(Mtrans,' ',SayAtom),
	aformat(VBScript,'user.say "~w" ',[SayAtom]),
	!.

formed_invokation_to_VBScript(say(_Avatar,Mtrans),VBScript):-
	atom(Mtrans),
	aformat(VBScript,'user.say "~w" ',[Mtrans]),
	!.

formed_invokation_to_VBScript(Prolog,VBScript):-
	catch2(Prolog=..[Method,_Avatar,Object]),
	Method=drop,						%%user.invokemethod "drop", user.findwithin("bigbox")
	get_thingname(Object,ThingName),
	aformat(VBScript,'user.~w user.findwithin("~w")',[Method,ThingName]),
	!.

formed_invokation_to_VBScript(Prolog,VBScript):-
	catch2(Prolog=..[Method,_Avatar,Object,Exemplar]),
	Method=createinstance,						%%world.createinstance "bigbox","artifact"
	get_thingname(Object,ThingName),
	aformat(VBScript,'world.~w "~w","~w"',[Method,ThingName,Exemplar]),
	!.

formed_invokation_to_VBScript(Prolog,VBScript):-
	catch2(Prolog=..[Method,_Avatar,Object]),
	Method=createinstance,						%%world.createinstance "bigbox","artifact"
	get_thingname(Object,ThingName,Exemplar),
	aformat(VBScript,'world.~w "~w","~w"',[Method,ThingName,Exemplar]),
	!.

formed_invokation_to_VBScript(Prolog,VBScript):-
	catch2(Prolog=..[Method,_Avatar,Object]),
	get_thingname(Object,ThingName),
	aformat(VBScript,'user.~w user.container.findwithin("~w")',[Method,ThingName]),
	!.

formed_invokation_to_VBScript(Prolog,VBScript):-
	catch2(Prolog=..[Method,_Avatar,Object]),
	get_thingname(Object,ThingName),
	aformat(VBScript,'user.~w user.container.findwithin("~w")',[Method,ThingName]),
	!.







stack_prolog_for_VBScript(Prolog):-
     
	invokation_script_to_VBScript(Prolog,VBScript),
	post(please_exec(VBScript)).
        
notice_vbscript(please_exec(VBScript),VBScript):-!.

invokation_script_to_VBScript(Prolog,VBScript):-not(is_list(Prolog)),!,
	formed_invokation_to_VBScript(Prolog,VBScript).
	

invokation_script_to_VBScript(PrologList,VBScript):-
	invokation_script_to_VBScript1(PrologList,VBScriptList),
	concat_atom(VBScriptList,' : ',VBScript).

invokation_script_to_VBScript1([],[]).

invokation_script_to_VBScript1([Prolog|Tail],[VBScript|TailScript]):-
	formed_invokation_to_VBScript(Prolog,VBScript),
	invokation_script_to_VBScript1(Tail,TailScript).


exemplar_to_vbs(thing,'user').
exemplar_to_vbs(avatar,'user').
exemplar_to_vbs(world,'world').

args_to_objects([],[],[]).

args_to_objects([Arg|Args],[Varg|Vargs],[Object|Objects]):-
	arg_to_object(Arg,Varg,Object),
	args_to_objects(Args,Vargs,Objects).

arg_to_object(innerthing(_),X,Str):-
	get_thingname(X,Y),
	aformat(Str,' user.findwithin("~w") ',[Y]),!.

arg_to_object(thing(_),X,Str):-
	vb_reference_thing(X,Str),!.
	
arg_to_object(GG,X,Str):-
	member(GG,[gesture(_),mtransable(_)]),
	atom(X),!,
	aformat(Str,'"~w" ',[X]),!.

arg_to_object(GG,X,Str):-
	member(GG,[gesture(_),mtransable(_)]),
	concat_atom(X,' ',Y),
	aformat(Str,'"~w" ',[Y]),!.

arg_to_object(_,t,null):-!.

vb_reference_thing(Thing,Reference):-
	get_location_of_thing(Thing,Place),
	get_thingname(Thing,Name),
	aformat(Reference,' ~w.findwithin("~w") ',[Place,Name]).

:-dynamic(my_location_array/2).

get_location_of_thing(Thing,'user'):-
	get_thingname(Thing,Name),
	my_location_array(Name,me),!.

get_location_of_thing(Thing,'user'):-
	my_location_array(Thing,me),!.

get_location_of_thing(_Thing,Local):-
	my_location_array(me,Local),!.

get_location_of_thing(_Thing,'user.container'):-!.




