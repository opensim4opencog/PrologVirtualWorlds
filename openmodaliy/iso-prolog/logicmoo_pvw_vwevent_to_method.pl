end_of_file.
%//------------------------------------------------------------------------------

% rev_invoke == Reverse Invoke
/*
		This predicate describes what Invokation Method was potentually used when a
		UI event was raised...
		  
*/


% //oninvoke(avatar(admin), emote, [empty(_G11593), ['sleeps.']])]),
rev_invoke(oninvoke(Thing, emote, [empty(_G11593), Emoticon]),
		[
		do([Thing,emote|Emoticon]),
		factual(thing,Thing),
		factual(emoticon,Emoticon)
		]).


% //oninvoke(bot(phebe), onpeertell, [avatar(admin), empty(_G11628), ['sleeps.'], 1])
rev_invoke(oninvoke(Thing, onpeertell, [Distribute, empty(_G11628), Emoticon, _ ] ),
		[
		do([Thing,tell,Distribute|Emoticon]),
		factual(thing,Thing),
		factual(thing,Distribute),
		factual(emoticon,Emoticon)
		]).

% //oninvoke(avatar(admin), say, [base=be:[p, pres, i0], manner=[how:[p, wh]], tsub=[you:[p, pers2, plur, anim, humn, person:[p]]]])
rev_invoke(oninvoke(Thing, say, Mtransable),
		[
		do([Thing,say|Mtransable]),
		factual(thing,Thing),
		factual(mtransable,Mtransable)
		]).


% //oninvoke(location('lobby location'), oncontentsystemtell, [avatar(admin), empty(_G11572), ['sleeps.'], 1])
rev_invoke(oninvoke(locationThing, oncontentsystemtell, [Thing, empty(_G11572), Emoticon, _]),
		[
		do([Thing,emote|Emoticon]),
		factual(location,locationThing),
		factual(thing,Thing),
		factual(emoticon,Emoticon)
		]).

%//------------------------------------------------------------------------------


