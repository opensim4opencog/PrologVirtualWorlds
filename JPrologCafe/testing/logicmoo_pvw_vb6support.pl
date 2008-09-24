% vb6support.prolog

end_of_file.
%//------------------------------------------------------------------------------

plquery(X):-catch((atom_to_term(X,Prolog,_),Prolog),Error,post(Error)).

getresult(X):-retract(result(X)).

%//------------------------------------------------------------------------------

check_cleanup(ToClean,Result):-
			raw_to_formed_event(ToClean,Result),
			read_the_news(Result),!.

check_cleanup(ToClean,unkchk(ToClean)):-!.


partition_events([ResultList],Events,News):-!,
			findall(X,(member(X,ResultList),X=is_news(_,_) ),News),
			findall(X,(member(X,ResultList),not(X=is_news(_,_)) ),Events).
%//------------------------------------------------------------------------------


read_the_news([]):-!.

read_the_news([SingleNewsItem|News]):-ignore(try(SingleNewsItem)),read_the_news(News).



/*
read_the_news(News):-
	forall(
	SingleNewsItem,
	(member(SingleNewsItem,News)),
	catch(SingleNewsItem,_Error,true)
	).
*/
%//------------------------------------------------------------------------------

factual(Exemplar,Thing):-Prolog=..[Exemplar,Thing],factual(Prolog).

factual(_Exemplar,Thing):-nonvar(Thing). %//Channel be more paranoid later

factual(X):-fact(X).

%//------------------------------------------------------------------------------

/*
Rules= 
		
	Event Form

	Invokation Form

	State Form

	Truisms Of Variables
*/


%//------------------------------------------------------------------------------


formed(Form,Propisition):-
	concat_atom('formed_',Form,Functor),
	Prolog=[Functor,Propisition],
	catch(Prolog),!.

formed(Datum):-
	member(Form,[event,invokation,state,truism,implication]),
	formed(Form,Datum),!.

formed(Datum):-!,catch(Datum).

%//------------------------------------------------------------------------------

formed_datatype(none):-fail.
formed_datatype(Type):-formed_datapair(Type=_).

formed_datapair(Type,DataType):-formed_datapair(Type=DataType).

formed_datapair(Type):-member(Type,
	[
	thing=thing,
	gesture=gesture,
	mtransable=mtransable,
	meshname=mtransable,
	mesh=mesh,
	color=long,
	file=mtransable,
	exemplar=mtransable,
	frame=frame,
	propname=mtransable,
	index=double,
	optionbits=long,
	pathmap=propertymap,
	file=mtransable,
	url=mtransable,
	text=mtransable,
	[]
	]).

whois_me('user').


%//------------------------------------------------------------------------------

	                                   
%//------------------------------------------------------------------------------

get_thingname(ThingName,ThingName):-atom(ThingName),!.
get_thingname(Thing,ThingName):-Thing=..[_,ThingName],!.
get_thingname(ThingName,ThingName):-!.

get_thingname(ThingName,ThingName,thing):-atom(ThingName),!.
get_thingname(Thing,ThingName,Exemplar):-Thing=..[Exemplar,ThingName],!.


%//------------------------------------------------------------------------------

