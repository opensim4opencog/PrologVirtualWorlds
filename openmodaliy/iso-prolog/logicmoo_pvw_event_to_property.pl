end_of_file.
is_news(thing,Thing):- catch2(Thing =.. [Exemplar,ThingName]),
			is_news(exemplar,Exemplar),
			is_news(thingname,ThingName),
			is_news(thing_exemplar,[ThingName,Exemplar]),!.

is_news(exemplar,Exemplar):-
			assert_new(exemplar(Exemplar)),!.

is_news(thingname,Thing):-
			assert_new(thingname(Thing)),!.

is_news(thing_exemplar,[Thing,Exemplar]):-
			is_news(exemplar,Exemplar),
			is_news(thingname,Thing),
			catch2(Prolog=..[Exemplar,Thing]),
			assert_new(Prolog),
			assert_new(thing_exemplar(Thing,Exemplar)),!.

is_news(exemplar_prop_value,[Exemplar,PropertyName,Value]):-
			is_news(exemplar,Exemplar),
			is_news(propname,PropertyName),
			is_news(value,Value),
			is_news(exemplar_prop,[Exemplar,PropertyName]),
			assert_new(exemplar_prop_value(Exemplar,PropertyName,Value)),!.

is_news(propname,PropertyName):-
			assert_new(propname(PropertyName)),!.

is_news(value,Value):-
			assert_new(value(Value)),!.

is_news(mtransable,Text):-
			assert_new(mtransable(Text)),!.

is_news(Functor,Args):- %nonvar(Args),
			Prolog=..[Functor|Args],
			assert_new(Prolog),!.

is_news(Functor,Arg):- %nonvar(Arg),
			Prolog=..[Functor,Arg],
			assert_new(Prolog),!.

%//------------------------------------------------------------------------------

