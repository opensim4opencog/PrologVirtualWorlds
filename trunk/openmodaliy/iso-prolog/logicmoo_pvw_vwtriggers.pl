end_of_file.
% triggers




vbtest :-
        getpropstr(admin1,isconnected,TF),
        invoke(say([it,is,TF,that,' the admin',is,conencted])).

% Do nothing

oninvoke(ithing([[room], [20], [outside]]), oncontentsystemtell(ithing([[avatar], [11], [wizard]]), unset([nothing]), ME, [0])):-
	personalize(ME,YOU),
	eliza(YOU,OUT),
         expouse(OUT),!.	

oninvoke(_,_):-!.
onproperty(_,_):-!.

onui(_, say(sampleavatar(objectwizard), [do,Verb|Args], _)) :-
	catch(Method=..[Verb|Args]),       
        post(Method),
        invoke(say(Method)),
        invoke(Method,X),
        invoke(say(X)).

onui(_, say(sampleavatar(objectwizard), [run,Macro], _)) :-
	scriptFile(Macro,Script),
        invokelist(Script),!.
                
onui(_, say(sampleavatar(objectwizard), ME, _)) :-
	personalize(ME,YOU),
	eliza(YOU,OUT),
        expouse(OUT),!.

onui(_):- !.

expouse([OUT]):-catch2(OUT),!.
expouse([OUT]):-catch2(invoke(OUT)),!.
expouse(OUT):-invoke(say(OUT)),!.

meta(_).
 

%//------------------------------------------------------------------------------


scriptFile(testmacro,[
		say([creating]),
		create_instance(foo,thing),
		say([droping]),
		drop(foo),
		say([taking]),
		take(foo),
		say([droping]),
		drop(foo),
		say([destroying]),
		destroy_instance(foo),
		say([ending,my,macro,script])
	]).

scriptFile(testmacro2(Arg),[             %% test for ArgV Recocniton
		say([say,Arg]),
		say(' ends script'),
		say([ending,my,macro,script])
	]).

