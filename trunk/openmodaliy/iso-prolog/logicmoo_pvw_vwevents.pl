%//------------------------------------------------------------------------------

%%%%%%%%%--------------Make these Recognised-----------------

%%  the take vbs==  user.invokemethod "take", user.container.findwithin("artifact")

%%[unk(oninvokemethod(light(light), [onboundarylistchanged], [0, 0]))]

%%%raw_to_formed_event(RAW,[unk(RAW)]):-!.

raw_to_formed_event(X,Y):-catch(raw_to_formed_event2(X,Y),_,fail).
%%% raw_to_formed_event(RAW,[unk(RAW)]):-!.

raw_to_formed_event2(end_of_file,[end_of_file]):-!.

raw_to_formed_event2(onpropertychanged(Thing, [PropertyName], Args),
	[onproperty(Thing,TheMethod)]):- 
	(TheMethod=..[setpropstr,Thing,PropertyName,Args]),!.
	
raw_to_formed_event2(onreport(_Method, [_Who, whispers, to, _ToWho|_Stuff]),
	[] ):-!. % Redundant

raw_to_formed_event2(oninvokemethod(Thing, [Method], ipropertylist([Int, Objects])),
	[oninvoke(Thing,TheMethod)]):-integer(Int),
	(TheMethod =.. [Method,Objects]),!.

raw_to_formed_event2(oninvokemethod(Thing, [Method], ipropertylist([Int,O|Objects])),
	[oninvoke(Thing,TheMethod)]):-integer(Int),
	(TheMethod =.. [Method,O|Objects]),!.

raw_to_formed_event2(oninvokemethod(Thing, [Method], ipropertylist(ToChopInt)),
        [oninvoke(Thing,TheMethod)]):-
        chop_last_int(ToChopInt,Chopped),
	(TheMethod =.. [Method|Chopped]),!.

raw_to_formed_event2(oninvokemethod(Thing, [Method], ipropertylist(Objects)),
	[oninvoke(Thing,TheMethod)]):-
	(TheMethod =.. [Method|Objects]),!.

raw_to_formed_event2(oninvokemethod(Invoker, [Method], PeerAction),
	[oninvoke(Invoker,TheMethod),oninvoke(Thing,ThePeerMethod),oninvoke(Thing,text(Text))]):-
	((PeerAction=[[PeerMethod], Text, [Thing, Object]],
	ThePeerMethod=..[PeerMethod,Thing,Object],
	TheMethod=[Method,Invoker,ThePeerMethod])),!.

raw_to_formed_event2(oninvokemethod(Thing, [Method], Object),
	[oninvoke(Thing,TheMethod)]):-
	(TheMethod =.. [Method,Object]),!.


raw_to_formed_event2(ontrace(Event),[ontrace(Event)]):-!.

raw_to_formed_event2(onuievent(Event),Events):-raw_to_formed_ui_event(Event,Events),!.
           

raw_to_formed_event2(onuserconnect(Thing),[oninvoke(Thing,TheMethod)]):-
	(TheMethod=..[onuserconnect,Thing]),!.
                                                
raw_to_formed_event2(onuserdisconnect(Thing),[oninvoke(Thing,TheMethod)]):-
	(TheMethod=..[onuserconnect,Thing]),!.


chop_last_int([],[]):-!.
chop_last_int(ToChopInt,Chopped):-!,last(Int,ToChopInt),integer(Int),append(Chopped,[Int],ToChopInt),Chopped=[_|_],!.
chop_last_int(Chopped,Chopped):-!.



%//------------------------------------------------------------------------------

raw_to_formed_ui_event(say(Location, Thing, empty(_G18462), Mtransable),[ onui(Location,oninvoke(Thing,say(Mtransable)))]):-!.

raw_to_formed_ui_event(say(Location, Thing, Distribution, Mtransable),[onui(Location,say(Thing,Mtransable,Distribution))]):-!.

raw_to_formed_ui_event(UI,[onui(Location,TheMethod)]):-
	UI =..[Method,Location,Thing,Distribution|Args],
	TheMethod=..[Method,Thing,Distribution|Args],!.


raw_to_formed_ui_event(A,[onui(A)]).
%//------------------------------------------------------------------------------

