% <?
/*
	Learning Bot Main Driver

	SWI-Prolog Compatible Source Code

	bt Douglas Miles 1991-1999
	mailto:admin@vast1.com
	http://prolog.vast1.com
	
	nephrael@EFnet
	

*/

% override internal system error handling

:-	set_feature(debug_on_error, false),
	set_feature(report_error, false),
	set_feature(syntax_errors, false).



/*  give it a G look and feel */

:-op(1000,xfy,'&').
:-dynamic('<-'/2).
:-op(1200,xfx,'<-').
:-multifile('<-'/2).

'&'(X,Y) :- ','(X,Y). %% Conjuntive "then"

%%% the | "or" disjection is kerenl level

/* rebuild the systems meta-interpretor */

:-dynamic(true/1).
:-assert(true(_)).
:-assert(true(_)).

:-op(1100,fx,(/)).

(/):-!.

a <- b & c.
a <- d | e.
b <- d.
b <- e.
d <- true.
c <- true.
e <- true.
noun <- person | place | thing.
person <- living & breathing & noun.
bobo(X) <- bo(X) & bo(X).
boba(X) <- bo(X) & ba(X).


if( Rules ; Rules)<-
	learn_about(Rules,Rules).

if( GoalA & RulesA ; Rules)<- 
	learn_about([GoalA|RulesA],Rules),!.
if( GoalA & GoalB & RulesA ; Rules)<- 
	learn_about([GoalA,GoalB|RulesA],Rules),!.
if( GoalA & GoalB & GoalB & GoalC &  RulesA ; Rules)<- 
	learn_about([GoalA,GoalB,GoalC|RulesA],Rules),!.

succeed(true,true,_Proof_Tree,_Ancestors).
succeed((GoalA & GoalB),(RulesA & RulesB),Proof_Tree,Ancestors):-
	succeed(GoalA,RulesA,Proof_Tree,Ancestors) &
	succeed(GoalB,RulesB,Proof_Tree,Ancestors) .
succeed(GoalA,(GoalA:-RulesA),Proof_Tree,Ancestors) :-
	does_exist(GoalA:-Body) &
	succeed(Body,RulesA,Proof_Tree,[GoalA|Ancestors]).

succeed(true) <- (/).
succeed(A & B)<- (/) & 
	succeed(A) & 
	succeed(B).
succeed(A) <- note_call(A) & 
	does_exist(A<-Body) &
	succeeds(Body).
succeed(A) <- note_call(A) & 
	does_exist((A<-Goal)) & 
	succeeds(Goal) & 
	note_exit(A).
succeed(A) <- note_fail(A) &
	fail.

does_exist((Head <- Body)):- clause((Head <- Body),true).
does_exist((Head <- true('Term'))):- Head=..[OP|_],(current_functor(OP,1)).
does_exist1((Head <- Body)):-
        does_exist((Head <- Body)) & note_redo(Head).

built_in(Goal):-predicate_propetry(Goal,_).

learn_about(List, ToDo) :- !,write_debug(interesting,[studying,List,to,learn,ToDo]).

note_call(Goal):-write_debug(note,call(Goal)).
note_call(Goal):-write_debug(note,fail(Goal)) & fail.
note_exit(Goal):-write_debug(note,exit(Goal)).
note_redo(_Goal):-true.
note_redo(Goal):-write_debug(note,redo(Goal)) & fail.
 
	
succeed((/),true) <- (/).
succeed(true(X),true(X)) <- true & (/).

succeed(true,true) <- true.

succeed((GoalA & GoalB),(RulesA & RulesB)) <-
	succeed(GoalA,RulesA) &
	succeed(GoalB,RulesB) & 
	true('Conj/2').

succeed((GoalA , GoalB),(RulesA , RulesB)) <-
        succeed(GoalA,RulesA) ,
        succeed(GoalB,RulesB) ,
        true('Conj/2').

succeed((GoalA | GoalB),(RulesA | RulesB)) <-
        ((succeed(GoalA,RulesA) & true('Disj'),!) |
        (succeed(GoalB,RulesB) & true('Disj'),!)).

succeed((GoalA ; GoalB),(RulesA | RulesB)) <-
        ((succeed(GoalA,RulesA) & true('Disj'),!) ;
        (succeed(GoalB,RulesB) & true('Disj'),!)).

succeed(GoalA, (GoalA <- NewRule)) <-
	does_exist(GoalA, RulesA) &
	succeed(RulesA, NewRule) &
	true('Found').
	 
succ_with(Goal,OldRules,OldRules) <-
        does_exist(Goal) & (/). %%%& attempt(Goal) & true('System').

succ_with(Goal,OldRules,OldRules) <- 
	my_system(Goal,true) & (/). %%%%%% & attempt(Goal) & true('System').

succ_with(addQ(Goal),OldRules,NewRules) <- 
	(/) & if(addQ(Goal) & OldRules ; NewRules) & true('AddQ').

succ_with(delQ(Goal),OldRules,NewRules) <- 
	(/) & if(delQ(Goal) & OldRules ; NewRules) & true('DelQ').

succ_with(Goal,OldRules,NewRules) <- 
	kernel_t(Goal & OldRules & NewRules) & true('Kernel').


kernel_t(true(_X)  & OldRules & OldRules) <- (/).
kernel_t(true & OldRules & OldRules) <- (/).

kernel_t(GoalA & GoalB &  OldRules, NewRules ) <- 
	(/) &
	succeed_with(GoalA, OldRules, MoreRules) &
	succeed_with(GoalB, MoreRules, NewRules) & true('Conj').

kernel_t(GoalA & OldRules & NewRules) <-
	does_exist((GoalA <- CurrentG & OldRules)) & 
	succ_with(CurrentG, OldRules, NewRules) & true('Create').




/*  use our include librarys and source */

%:-	[dcgchat].
:-['../eggdrop_prolog/eggdrop.pl'].
%:-	load_foreign_library(douglas). /* see douglas.[c,h] */
%:-	[library(tokenization)].

/* these next 2 modules will not be loadeded separately ...yet... */
/*	use_module(library(eggdrop_features)),  silly irc stuff */
/*	use_module(library(call_backs)). this is the network kernel interface */
	 
/*  instead we will keep them here in this file so no battling Scopes */

/*  Main: Connect to the Remote Machine... start the Event Handler */

main:-	
	/*Select callable name for the kernel */
	NickHandle = "SwiProlog",
	
	/* telnet in and create $stream(r|w) */
	egg_connect('vast1.com',65530,swiprolog,binpro,NickHandle),
	
	/* log its connection time */
	write_debug(interesting,[NickHandle,"has made it on-line."]),
	
	/* This is a Loop any improper exit codes will bring us back here */
	repeat, 

        /* We first execute the toploop by letting our threads have one cycle */
	eggdrop:egg_term(NickHandle,Event), 

	/* prototype the event as a "from" or a "clock" event */
	(Event=from(_,Who,say(_));Event=clock),
	
	not(Who=NickHandle),not(Who=[_,_,_,_|"rolog"]),

		
	/* Every event that enters Brings about a processing cycle */
	/* This Is Exempt from the actual event that just now happened */
	/* Its odd but i had to bind a toploop someplace so I did it Here */	
	        
	/* Ignore means its not part of the Calling event.. just a cycle */
	ignore((
	/* event/3  is a database predicate that gets a Current thread */
	/* we will bring it forground to log it */
                retract(event(TimeStamp,Where,Goal)),
		not(Goal=true),
		write_debug(true,trying(TimeStamp,Where,Goal)),
		(
                (
                integer(TimeStamp),
		Expiration is TimeStamp-1
                )
                ;
                true
                ),
    	/* Make sure its not corrupted */
                nonvar(Goal),

	/* give the thread a domain */
                console(Where),

	/* Ok lets parse the thread now (Execute it) */
                try_thread(event(Expiration,Where,Goal),event(NewExpires,Where,NewState)),
	
	/* Put it back into the Stack and log it */
		(
		(
		not(NewExpires=1),
		assertz_new(event(Expiration,Where,NewState)),
		write_debug(true,restacking(Where,NewState))
		)
		;
		true
		)
        )),

	/* execute the Real Thing in the Event Buffer */
	catch(Event,_,fail),
	
	/* this is the loop end fail=restart. */
	fail.

:-dynamic(clock/0). clock. 
:-dynamic(thread/2). 
:-dynamic(from/3),dynamic(hook_from/3).
from(Where,Who,What):-hook_from(Where,Who,What).
from(Place,Who,say(A)):-
	catch((
	Who="c9",atom_to_term(A,T,_V),
	console(Place),
	call(T)
	),_,fail).

from(Place,Who,say(P_Atom)):-
        /* if passes security checks - still not on hostmask-level */
        security_check(Place,Who,P_Atom),
        atom_to_prolog(P_Atom,Prolog,_Vars),
        console(Place),
        nonvar(Prolog),
	((once(catch(Prolog,_,fail)));
	(once(catch(try_lisp(Prolog),_,fail)))).

try_lisp(Any):-any_to_words(Any,Words),Prolog=..Words,Prolog.



	%asserta_new( event(5,Place,thread(P_Atom,true,unifyGlobals(Vars),Prolog) )).

/*
from(Place,Who,say(P_Atom)):-
	listen_to(Place,Who),
	any_to_words(P_Atom,Words),
	asserta_new( event(5,Place,thread(P_Atom,true,true,
			said(Place,Who,Words) ))).
*/

define(W):-any_to_words(W,Ws),wt(Ws,[X]),g(X,Y),post(Y).

listen_to("0","swiProlog"):-fail.
listen_to("nephrael",_):-!.
listen_to("nephryel",_):-!.

listen_to(_,"nephrael"):-!.
listen_to(_,"nephryel"):-!.

listen_to(X,X):-fail.
listen_to("#ai",_).
listen_to("#prolog",_).



engine([event,total],X):-predicate_property(event(_,_,_),number_of_clauses(X)).
engine([expire,events],X):-
	predicate_property(event(_,_,_),number_of_clauses(X)),
	post(["expiring ",X," events"]),
	retractall(event(_,_,_)).



said(_,_,_).


asserta_new(true):-!.
asserta_new(thread(_,true)):-!.
asserta_new(Prolog):-clause(Prolog,_),!.
asserta_new(Prolog):-asserta(Prolog),!.

assertz_new(true):-!.
assertz_new(thread(_,true)):-!.
assertz_new(Thread):-clause(Thread,_),!.
assertz_new(Thread):-assertz(Thread).
	
is_true_fact(X):-catch(X,_,fail).
is_untrue_fact(X):-catch(ignore(X),_,fail).

can_do(nop).
can_do(X):-is_true_fact(X).
can_do(X):-is_untrue_fact(X).

%TTTT


ab:-(a,b).

try_thread(true,true):-!.
try_thread(event(When,Where,Goal),event(When,Where,NewGoal)):-
	try_thread(Goal,NewGoal),!.

try_thread(shiftleft(RegisterA,RegisterB),shiftleft((RegisterA,Cmd),NewRegisterB)):-
	get_first_cmd(RegisterB,Cmd,NewRegisterB),!.

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,The_Rest),
        Cmd=fire,
        (do_execute(Static_Code,Declares,Completed);true),
        NewState=thread(Static_Code,Declares,(Completed,Cmd),The_Rest),!.

try_thread(thread(_Static_Code,_Declares,_Completed,(halt,_To_Do)),true):-!.

try_thread(thread(_Static_Code,_Declares,Completed,exit(!)),true):-!,
        (catch(Completed,_,fail);true).

try_thread(thread(_Static_Code,_Declares,Completed,(exit(!),_To_Do)),true):-!,
	(catch(Completed,_,fail);true).

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,The_Rest),
	Cmd=try_execute,
        ((do_execute(Static_Code,Declares,Completed),
	   true,NewState=thread(Static_Code,Declares,(Completed,Cmd),The_Rest)
		);(
	   NewState=thread(Static_Code,Declares,Completed,To_Do) /* fail */)).

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        Completed=true,
        get_first_cmd(To_Do,Cmd,The_Rest),
        can_do(Cmd),
        NewState=thread(Static_Code,Declares,Cmd,The_Rest),!.


try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
	call(Declares),
	get_first_cmd(To_Do,Cmd,The_Rest),
	is_true_fact(Cmd),
	NewState=thread(Static_Code,Declares,(Completed,Cmd),The_Rest),!.

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,The_Rest),
        expanded_clause(Cmd,Body),
        NewState=thread(Static_Code,Declares,Completed,(Body,The_Rest)),!.

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,The_Rest),
	Cmd=retry(Funct),
	(catch(Funct,_,fail),NewState=thread(Static_Code,Declares,(Completed,Cmd),The_Rest),!)
	;NewState=thread(Static_Code,Declares,Completed,To_Do),!.

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,The_Rest),
	Cmd=abort(Funct),
	catch(Funct,_,true),
	NewState=thread(Static_Code,Declares,(Completed,Cmd),The_Rest),!.

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,The_Rest),
        io_funtion(Cmd),
        NewState=thread(Static_Code,Declares,(Completed,Cmd),The_Rest),!.

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,The_Rest),
        can_do(Cmd),
        NewState=thread(Static_Code,Declares,(Completed,Cmd),The_Rest),!.

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,The_Rest),
        Cmd=(GoalA;GoalB),
	assertz_new(event(thread(Static_Code,Declares,Completed,(GoalA,The_Rest)))),
	assertz_new(event(thread(Static_Code,Declares,Completed,(GoalB,The_Rest)))),
        NewState=true,!.

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,The_Rest),
        Cmd=(GoalA,GoalB),
	NewState=thread(Static_Code,Declares,Completed,(GoalA,(GoalB,The_Rest))),!.

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,The_Rest),
	assertz_new(thread(null,thread(Cmd,Declares,Completed,Cmd))),
	assertz_new(thread(null,thread(Static_Code,Declares,Completed,The_Rest))),
	NewState=thread(Static_Code,Declares,Completed,To_Do),!.

try_thread(thread(Static_Code,Declares,Completed,To_Do),NewState):-
        call(Declares),
        get_first_cmd(To_Do,Cmd,_The_Rest),
        is_untrue_fact(Cmd),
        NewState=thread(Static_Code,Declares,Completed,To_Do),!.



try_thread(NoChange,NoChange):-!.

	/* this commad was called by the cammand above the last */
do_execute(Static_Code,Declares,Completed):-
	(Do_Code=((Declares,Completed))),!,
	   catch(
		(
		(
		Do_Code
		,
		write_debug(true,do_success(Static_Code,Declares,Completed))
		)
		;
		(
		write_debug(true,do_failed(Static_Code,Declares,Completed))
		,
		fail
		)
		),
		_ErrorCode,
			(
			 write_debug(true,error(_ErrorCode))
			  ,
			  fail
			     )
		             )
		             ,
			     !.


	/* retreived the left most command */
get_first_cmd(true,true, exit(!)):-!.
get_first_cmd((Cmd,Rest),Cmd,  Rest   ):-!.
get_first_cmd((Cmd) ,Cmd,  exit(!)):-!.

exit(X):-call(X).

	/* ok this is where the engine actually does it work */
ripe(Cmd):-Cmd=..[Op|_Args],ripe_ops(Op),!.
ripe(Cmd):-Cmd=..[Op|Args],checklist(nonvar,Args),must_be_bound_ops(Op),!.
ripe(Cmd):-predicate_property(Cmd,_T),!.

%ripe(Cmd):expanded_clause(Cmd,Expansion),get_first_cmd(Expansion,Exp_Op),ripe(Cmd),!.

ripe_ops(Op):-flow_ops(Op),!.
ripe_ops(Op):-compares(Op),!.
ripe_ops(Op):-io_ops(Op),!.
ripe_ops(Op):-fs_preds(Op),!.
ripe_ops(Op):-lists(Op),!.
ripe_ops(Op):-db_preds(Op),!.
ripe_ops(Op):-meta(Op),!.
ripe_ops(Op):-unused(Op),!.
	
flow_ops(Op):-member((Op/_),[true/0,!/0,'*->'/2,fail/0,exit/1,abort/0,halt/0,'->'/2]).

% math_ops(Op):-member((Op/_),[+/2,-/2,-/1,'*'/2,'/'/2,'//'/2]).

compares(Op):-member(Op,[<,>,=,'=..','=:=',=<,==,=@=,=\=,>=,>=,@<,@=.@>,@>=,\=,\==,\=@=,^]).

io_ops(Op):-member((Op/_),[write/1,disp/1,display/1,post/1,writeq/1,say/1,print/1,
				printf/1]),!.

db_preds(Op):-member((Op/_),[abolish/1,abolish/2,assert/1,assert/2,asserta/1,asserta/2,
				assertz/1,assertz/2]).

fs_preds(Op):-member((Op/_),[absolute_file_name/2,absolute_file_name/3,access_file/2,
				append/1]).

lists(Op):-member((Op/_),[append/3]).

meta(Op):-member((Op/_),[apply/2]).

unused(Op):-member((Op/_),[apropos/1,prolog/0]).
must_be_bound_ops(Op):-io_ops(Op),!.
must_be_bound_ops(Op):-member((Op/_),[not/1,call/1,bound/1,nonvar/1,integer/1]),!.

/*
to_do_once(Where,Who,P_Answer_Atom)),
	write_debug(true,stacking(Where,Who,P_Answer_Atom)).
*/


	/* this predicate is for 99% parsing goals */
atom_to_prolog(P_Answer_Atom,P_Answer,Vars):-
        /* Convert P_Answer_Atom to P_Answer, */
        /* catch will find U_P_Answer and FreeVars */
        catch(
                /* Parse Atom to U_P_Answere Term */
                atom_to_term(P_Answer_Atom,P_Answer,Vars),

                /* It will auto-break out here if our simple parse worked */
                _Skip_AnyErrors,
                (
                        /* we now must execute a pre-parser on P_Answer_Atom */
                        catch((
                        lets_try_new_syntax(P_Answer_Atom,P_Answer),

                        /* It Worked, now lets get Vars */
                        atom_to_term(_Useless,P_Answer,Vars)),_,fail)
                )
             ).
	/* one of the above Created P_Answer and Vars */
my_prolog(P_Answer_Atom):-!,
	
	/* Do we need to convert this to a term? */
	once(my_atom_to_term(P_Answer_Atom,P_Answer,Vars)),

	/* now we will "compile it"... (make a token langage) */
	
	/* Lets attempt to save our variables as they are now... */
	/* we will overwirite any previos settings */
	unify_Globals(Vars),

	/* write to true */
	trial(P_Answer,Vars),

	write_debug(true,
		[attempting,term(P_Answer_Atom,P_Answer,Vars)]),
	
	call_with_depth_limit(once(attempt(P_Answer,Vars)),40,_).

	/* Attempt to execute P_Answer if it executes all is well.. */
	/* it will prove we have adaquate data in the system.. */
	/* Honestly all we hope for Is some varibles will get instantiated */
	/* if none do thats ok, the system is made to handle it (It's Prolog!)*/
	
	
	/* this attempt is actaully to plant the trunk of our "proof tree" */
	/* the ';true' means "OR it is TRUE that some case exists..." */
	/* So if absolutley nothing has been found to work.. P_Answere */
	/* may have just been a generaliztion of what we already know */

:-multifile(attempt/2).
:-multifile(trial/2).

attempt(true,_Vars):-!.

attempt(prolog,_Vars):-!.
attempt(Unbound,_Vars):-not(nonvar(Unbound)),!.

attempt((GoalA,GoalB),Vars):-!,
	attempt(GoalA,Vars),attempt(GoalB,Vars).

trial((GoalA,GoalB),_Vars):-
        trial(GoalA,_Vars),trial(GoalB,_Vars).

attempt((GoalA;GoalB),Vars):-!,
        attempt(GoalA,Vars);attempt(GoalB,Vars).

trial((GoalA;GoalB),_Vars):-
        trial(GoalA,_Vars);trial(GoalB,_Vars).

attempt((GoalA:-GoalB),_Vars):-!,create_new_clause((GoalA:-GoalB)).

trial((_GoalA:-_GoalB),_Vars):-true.

attempt(assert(GoalA:-GoalB),_Vars):-!,create_new_clause((GoalA:-GoalB)).
attempt(assert(Data),_Vars):-!,create_new_clause((Data:-true)).

trial(assert(_),_Vars):-true.

create_new_clause((Head:-Body)):-!,
	not(Head=Body),
	not(clause(Body,Head)),
	assert((Head:-Body)),
	write_debug(true,asserted((Head:-Body))).

attempt(display(Output),_Vars):-!,term_to_string(Output,Post),post(Post).
attempt(WriteOutput,_Vars):-
	WriteOutput=..[Write|Output],
	member(Write,[write,output,post,writeq,say,print,printf]),!,
	write_debug(true,output(Output)),
	post(Output).

trial(WriteOutput,_Vars):-
        WriteOutput=..[Write|Output],
        member(Write,[write,disp,display,post,writeq,say,print,printf]),!,
        nonvar(Output).

attempt(getVars(Vars),Vars):-!.
trial(getVars(Vars),Vars):-!.

attempt(true,_Vars):-!,write_debug(true,true).
attempt(!,_Vars):-!,write_debug(true,!).

/*
attempt(Goal,_Vars):-
	predicate_property(Goal,_),Goal,
	write_debug(true,Goal).
*/
expanded_clause(Head,Body):-get_clauses(Head,Body,_).

attempt(Goal,_Vars):-
	get_clauses(Goal,Expand,_),!,
	write_debug(true,expand(Goal,Expand)),
	attempt(Expand,_Vars).

get_clauses(Head,Body,1):-
	predicate_property(Head,number_of_clauses(1)),!,
	clause(Head,Body).

get_clauses(Head,(Body1;Body2),1):-!,
	get_clause(Head,Body1,1),
	get_clauses(Head,Body2,2).

get_clauses(Head,(Body1;Body2),X):-integer(X),X>1,
	get_clause(Head,Body1,X),X1 is X + 1,
	get_clauses(Head,Body2,X1),!.
	
get_clauses(Head,Body,X):-!,integer(X),X>1,
	get_clause(Head,Body,X).

get_clause(Head,Body,Index):-!,
	nth_clause(Head,Index,Ref),
	clause(Head,Body,Ref).

trial(Goal,_Vars):-
	predicate_property(Goal,_).

attempt(Goal,_Vars):-
	catch(
		Goal,
		Error,
		on_err(Error)
	),
	write_debug(true,success(Goal)).

on_err(error(existence_error(procedure, Pred),_)):-!,
	ask_about(Pred),
	write_debug(true,learn(Pred)),
	fail.

on_err(Error):-!,write_debug(true,Error),fail.

ask_about(Question):-post(["i do not understand ",Question]).

	
/* make term P_Answer into conjuntive list P_Answer_List */
	%term_to_codelist(P_Answer,P_Answer_List),
	
	/* Next we need to to have some usable Knowlege Base Data */
	/* Let merge that in as well... (this data is unlimited actualy) */
	/* Thats what makes this program unique (see use_network_kernel.c) */
	/* but this unlimited database might be too much for P_Answer_Plus */
	/* well just lets grab some previously cached network Q&A */
%	get_previously_cached_network_data(Cached_Net),
	
	/* we add our goal in front of the data  	*/
%	P_Answer_Plus_Plus=(Cached_Net,P_Answer_Plus),

	/* ok we have a list Datatype Now all or execution will be on a list */
	
	/* succeed if we can true P_Answer_Plus_Plus to Goal */
	/*          (which we weakly bind to true)     */
/*
	((Goal=true);true),
*/	
	/* in the worst case it will be lazy and bind Goal to true */
	/*                                                 */
	/*             Make a "proof tree"                 */
	/* 						   */
	/*						   */
/*
	succeed(P_Answer_Plus_Plus,Goal,Proof_Tree,RulesUsed),
*/	
	/* program is quite busy at this point */
	
	/* when its done .. it will msg the "solution" for now */
/*
	write_debug(true,
	succeed(P_Answer_Plus_Plus,Goal,Proof_Tree,RulesUsed)).
*/
		/* and now it loops back to the top of main */
	
	/* is alot more..:).. i am finishing up the proof compilier */
:-dynamic(get_previously_cached_network_data/1).

	get_previously_cached_network_data(true).


:-dynamic(varTable/2).
varTable('Poo',poo).

unify_Globals([]):-!.
unify_Globals(VarsIN):-
        checklist(unify_Globals1,VarsIN).

clearVar(Var):-retractall(varTable(Var,_)).
unboundVar(Var):-asserta(varTable(Var,_Unbound)).

unify_Globals1(Var=Val):-
	(	
	varTable(Var,Val),write_debug(true,varTable(Var,Val))
	);
	(
	nonvar(Var),nonvar(Val),asserta(varTable(Var,Val)),
		write_debug(true,asserta(varTable(Var,Val)))
	);
	(
	true %Var=Val,write_debug(true,Var=Val)
	).

:-op(700,xfx,':=').

':='(Var,Val):-catch(Enum is Val,_,Enum=Val),nonvar(Var),
	asserta(varTable(Var,Val)).

:-op(700,xfx,':==').

':=='(Var,Val):-nonvar(Var),clearVar(Var),asserta(varTable(Var,Val)).


/*	answer_templates(P_Answer,P_AnswerT),*/

  /* post parser */
        	
	/* having 4 differrent Formated Calls Now.. lets log */

		/* ok lets thread these calls */


/*
	ok_lets_try_it_out(P_Answer,Result),post(Result))
	
	catch(Reformated_Answer,_,
                (ok_lets_try_it_out(P_Answer,Result),post(Result))).


*/
/* strange tcl and network messages can come via eggdrop:egg_term/2
   this is how we handle them.. they are not for us yet */

putlog(Weird_Stuff):-write_debug(minor,Weird_Stuff).

write_debug(true,Message):-!,
        any_to_string([true,Message],S),
	chars_to_atom(S,A),
	msg("0",A).

	% writeq(A),nl,ttyflush.

write_debug(Level,Message):-!,
	once(debug_to(Level,Place)),!,
	msg(Place,[Level,Message]),!.

:-dynamic(debug_to/2).
debug_to(interesting,"nephrael").
debug_to(_,"nephrael").

/* Most Predicates as "Dynamic" will be dumped into the "saved_states_file.pro"
   They are changed runtime and do just as good in an external database */

:-dynamic(ignore_it/1). /* ppl and source buffers DB predicates we ignore */
	ignore_it("jerks").
	ignore_it("#lamers").

:-dynamic(have_need_for/2). /* this system will stack unfulled commands here */
	have_need_for(step4,post("you are seeing this because step4 just complete")).

:-dynamic(goal_chain/2). /* these are execution lists to do */
	goal_chain(chain_name,[step1,step2,step3,step4]).
	goal_chain(succeed_now,[true]).

:-dynamic(answer_templates/2). /* this is used to see if arg1 is needed to be reformated */
	answer_templates(Unify_left_and_right,Unify_left_and_right).

:-dynamic(user_attribute/2).
	user_attribute("nephrael",_).
	user_attribute("nephryel",_).
	user_attribute("screwedup@jupe",prolog).
	user_attribute("screwedup",prolog).

	user_attribute(User,"#ai"):-member(User,["Phantal0o","c9",[_|"ie`"],[_|"otivez"],_,
		[_|"agos"],[_|"rbitz"],[_|"ispbliss"],[_|"sk"],
		[113,120|_],"rakta",[_|"yberTech"],[108,105,115,122|_]]).
 user_attribute(User,"#prolog"):-member(User,["Phantal0o","c9",[_|"ie`"],[_|"otivez"],_,
                [_|"agos"],[_|"rbitz"],[_|"ispbliss"],[_|"sk"],
                [113,120|_],"rakta",[_|"yberTech"],[108,105,115,122|_]]).



:-dynamic(good_source_of_learning/1). /* use these resource ("0"=network) */
	good_source_of_learning(Handle):-member(Handle,["nephrael","#ai","0","#lisp","#linux","#prolog"]).


:-dynamic(security_check/3). /* ACLs? */
	security_check(Place,Who,_Cmd):-!,
		(good_source_of_learning(Place),!;good_source_of_learning(Who)),
        	(user_attribute(Who,prolog),!;user_attribute(Who,Place)),
        	not(ignore_it(Who)),
		not(ignore_it(Place)),!.

:-dynamic(functcode/2).
	functcode(my_neg_add(A,B),[C is (A+B),return(-C)]).

lets_try_new_syntax(_,_):-!,fail.

lets_try_new_syntax(_P_Answer_Atom,said(Words)):-any_to_words(_P_Answer_Atom,Words).

ok_lets_try_it_out(Goal,true):-catch(Goal,_,fail).

ok_lets_try_it_out(P_Answer,Result):-
	is_it_system(P_Answer),!, /* yes! */
	use_the_system_then(P_Answer,Result).
 
ok_lets_try_it_out(P_Answer,Result):-
	not(is_it_system(P_Answer)),
	from_my_network(P_Answer),!, /* including C code */
	use_my_network(P_Answer,Result).

ok_lets_try_it_out(P_Answer,Result):-
	waiting_for_this(P_Answer),!,
	satisfy_something(P_Answer,Result).

ok_lets_try_it_out(P_Answer,Result):-
	not(is_it_system(P_Answer)),
	write_debug(learning,["new goal: ",P_Answer]),
	create_new_goal(P_Answer,Result).

is_it_system(P_Answer):-predicate_property(P_Answer,_),!.
is_it_system(P_Answer):-functcode(P_Answer,_),!.

:-op(800,xfy,':=:').

calleval(X):-my_system(X,Y),term_to_atom(Y,Z),post(Z).

my_system([nop],[]).
my_system([],[]):-!.
my_system([X|More],Final):-my_system(X,ReducedX),my_system([ReducedX|More],Final),!.
my_system([X|More],[Y|Final]):-my_system(X,Y),my_system(More,Final).
my_system(X,X):-integer(X),!.
my_system(eval(X),Y):-my_system(X,Y),!.
my_system(X,Y):-catch((Y is X),_,fail),!.
my_system(head([X|_]),X):-!.
my_system(tail([_|Y]),Y):-!.
my_system([X|More],[Y|Final]):-my_system(X,Y),my_system(More,Final).
my_system(A,A).


use_the_system_then((Step1,MoreSteps),Result):- 
	/* Conjunctive Compile */
        ok_lets_try_it_out(Step1,MoreSteps),
        ok_lets_try_it_out(MoreSteps,Result).

use_the_system_then((Step1;MoreSteps),Result):- /* Disjunctive Compile */
        ok_lets_try_it_out(Step1,MoreSteps);
        ok_lets_try_it_out(MoreSteps,Result).

use_the_system_then(X,Y):-my_system(X,Y).
use_the_system_then(true,_):-!.
use_the_system_then(return(Thread),Thread):-nonvar(Thread),!. /* No walking backwards */
use_the_system_then(write(Mesg),side_effects):-post(Mesg).
use_the_system_then(display(Term),side_effets):-nonvar(Term),!,
	term_to_atom(Term,Mesg),
	post(Mesg).
use_the_system_then(write_debug(Place,Mesg),side_effects):-!,write_debug(Place,Mesg).
use_the_system_then(prolog,true):-!.
use_the_system_then(Really_System,_):-catch(Really_System,Error,
			(write_debug(urgent,
			["Really_System: ",Really_System,Error]),
			fail)).

from_my_network(net_call(Request)):-is_it_system(Request).
from_my_network(net_result(Result,_Bindings)):-waiting_for_this(Result).

use_my_network(net_call(Request),true):-
	use_the_system_then(Request,Result),
	send_to_network(net_result(Request,Result)).

use_my_network(net_result(Result,Bindings)):-
	satisfy_something(Result,Bindings).

waiting_for_this(Answer):-have_need_for(Answer,_),!.

satisfy_something(Answer,Result):-
	have_need_for(Answer,Midpoint),
	ok_lets_try_it_out(Midpoint,Result),
	assertz(had_need_for(Answer,Midpoint)),
	retract(have_need_for(Answer,Midpoint)).

create_new_goal(Goal,So_I_Can_Proceed_At_This):-
	create_template(Goal,So_I_Can_Proceed_At_This),
	query_all_resource(Goal,So_I_Can_Proceed_At_This).
	
create_template(Goal,So_I_Can_Proceed_At_This):-  /* aready created, */
	have_need_for(Goal,So_I_Can_Proceed_At_This),!,
	write_debug(learning,[need,Goal,to,satisfiy,So_I_Can_Proceed_At_This]).

create_template(Goal,So_I_Can_Proceed_At_This):-
	asserta(have_need_for(Goal,So_I_Can_Proceed_At_This)),
	write_debug(learning,[need,Goal,to,satisfiy,So_I_Can_Proceed_At_This]).

query_all_resource(Goal,So_I_Can_Proceed_At_This):-
	query_each_resource(Goal,So_I_Can_Proceed_At_This).
query_all_resource(_Goal,_So_I_Can_Proceed_At_This).
	
query_each_resource(Goal,_So_I_Can_Proceed_At_This):-
	good_source_of_learning(Resource),
	locate_resource(Resource,Locate),
	msg(Locate,net_call(Goal)),fail.
	
locate_resource(Try_Calling_It_By_Its_Name,Try_Calling_It_By_Its_Name).

/*
:-dynamic(have_need_for/2). /* this system will stack unfulled commands here */
        have_need_for(step4,post("you are seeing this because step4 just complete")).

:-dynamic(goal_chain/2). /* these are execution lists to do */
        goal_chain(chain_name,[step1,step2,step3,step4]).

*/
/*
	
find_out_about(Goal,So_I_Can_Proceed_At_This):-
        create_need_for(Goal,So_I_Can_Proceed_At_This),
        ask_about_goal(Goal,So_I_Can_Proceed_At_This),
        create_need_for(Goal,So_I_Can_Proceed_At_This).

fail.


*/
	
/*	have_i_been_looking_for_this(P_Answer,I_Was_Wow),
	cann_someone_do_this_for_me(Possible
	(answer_templates(P_Answer,callback(Goal,Answer)),
        retract(have_need_for(Goal,So_I_Can_Do_This_Now)),
        assert((Goal:-Answer)),
        msg(Place,["thankyou, now I can try..",So_I_Can_Do_This_Now]),
        callt(So_I_Can_Do_This_Now)).
*/

/*
<nephrael> ok .. hrm i am going to write the call_back_hook for it
<nephrael> so if there is a non exestant call it will go to the OS to gewt it
<nephrael> so it searches for code.. if it dont find it it makes a new temlate it can fill in later
<jsr> ay eye
<nephrael> yes it will keep an agenda about filling it in
<nephrael> learning it
<nephrael> so if you asked it... be_human(X,Y).. it will try to fullfill that prototype
<nephrael> trying to created subtemplates until it has a good declaration :)
*/

clauses(Head,Body):-clausep(Head,Body).

clausep(Head,Body):-clause(Head,Body).
clausep(Head,Body):-predicate_property(Head,Body).
clausep(Head,Body):-clause((Head <- Body),true).





term_to_codelist((P:-ROLOG),CODELIST):-!,term_to_codelist1((P:-ROLOG),CODELIST).
term_to_codelist((:-PROLOG),CODELIST):-!,term_to_codelist1((:-PROLOG),CODELIST).
term_to_codelist(PROLOG,CODELIST):-!,term_to_codelist1((:-PROLOG),CODELIST).

term_to_codelist1([],[]):-!.

term_to_codelist1((A1 :- B1),AB):-!,
        term_to_codelist1(A1,A2),
        term_to_codelist1(B1,B2),
        append([define|A2],[as|B2],AB).

term_to_codelist1((A1,B1),AB):-!,
	term_to_codelist1(A1,A2),
	term_to_codelist1(B1,B2),
	append(A2,[and|B2],AB).

term_to_codelist1((A1;B1),AB):-!,
        term_to_codelist1(A1,A2),
        term_to_codelist1(B1,B2),
        append(A2,[or|B2],AB).

term_to_codelist1(:-CODE,['call'|LIST]):-(nonvar(CODE);nonvar(LIST)),!,
		term_to_codelist1(CODE,LIST).
term_to_codelist1(CODE,[learn|LIST]):-(nonvar(CODE);nonvar(LIST)),!,CODE=..LIST.

:-op(1200,fy,loglang).
:-op(1000,fy,solve).
solve(X):-Y is X,post(Y).

loglang(PROLOG):-term_to_codelist(PROLOG,CODE),
	term_to_atom(CODE,ATOM),post(ATOM).

% :-module(eggdrop_features,[callt/1,yn/1,nix/2,nix/1]).

callt(true):-!.
callt(prolog):-!,true.
callt(':-'(X,Y)):-assert((X:-Y)).
callt(write(X)):-post(X).
callt(display(X)):-term_to_atom(X,Y),post(Y).
callt(!):-!.
callt(','(X,More)):-try_to_see_if_you_have_to_stack(X,More).
callt(';'(X,More)):-try_to_see_if_you_have_to_stack(X,More);callt(More).
callt(X):-X.
callt(_):-fail.

try_to_see_if_you_have_to_stack(Goal,Stack):-
	predicate_property(Goal,_),!,
	call(Goal),
	callt(Stack).

try_to_see_if_you_have_to_stack(Goal,Stack):-
	find_out_about(Goal,Stack),!.
	

yn(X):-(X,post(yes));post(no).

nix(Request):-
	any_to_string(Request,String),
	atom_codes(CMD,String),
	nix(CMD,RESULT),
	post(RESULT).	

nix(CMD,RESULT) :-
        seeing(Old), see(pipe(CMD)),
        collect_wd(RESULT),
        seen, see(Old).

collect_wd([C|R]) :- get0(C), C \== -1, !, collect_wd(R).
collect_wd([]).

html_head(URL,Lines):-nix(["wget -O - ",URL,"|head ",-Lines]).
html_tail(URL,Lines):-nix(["wget -O - ",URL,"|tail ",-Lines]).
html_line(URL,Line):-nix(["wget -O - ",URL,"|head ",-Line,"|tail -1"]).
http_head(URL,Lines):-nix(["lynx -dump ",URL,"|head ",-Lines]).
http_tail(URL,Lines):-nix(["lynx -dump ",URL,"|tail ",-Lines]).
http_line(URL,Line):-nix(["lynx -dump ",URL,"|head ",-Line,"|tail -1"]).

:- op(255,xfy,'--->').
:- op(900, xfx, =>).
:- op(800, xfy, &).
:- op(300, xfx, :).


%:-use_module(library(lists)).
lfl(A):-any_to_words(A,X),phrase(sentence(P), X),post(P).
/*

%---------------------------------------------------------------
% Test data
%---------------------------------------------------------------
expr(Z) --> term(X), "+", expr(Y), {Z is X + Y}.
expr(Z) --> term(X), "-", expr(Y), {Z is X - Y}.
expr(X) --> term(X).

term(Z) --> number(X), "*", term(Y), {Z is X * Y}.
term(Z) --> number(X), "/", term(Y), {Z is X / Y}.
term(Z) --> number(Z).

number(C) --> "+", number(C).
number(C) --> "-", number(X), {C is -X}.
number(X) --> [C], {"0"=<C, C=<"9", X is C - "0"}.


imp --> [vp].
decl --> [np, vp].
decl --> [np, vp, pp].
decl --> [pp, np, vp].
decl --> [set].


np --> [set].
np --> [set, pp].

set --> [object].
set --> [object, object].
set --> [object, object, object].
set --> [object, object, object, object].

object --> [pn].
object --> [spec, noun ].
object --> [noun].

spec --> [det].
spec --> [adj].
spec --> [adj_sat].
spec --> [num].
spec --> [spec, spec].

vp  --> [iv].
vp  --> [tv, np].
vp  --> [tv, np, pp].
vp  --> [dv, np, pp].
vp  --> [adv, vp].
pp  --> [p, np].
*/

%list_to_terms([],true):-!.
list_to_terms([T1,T2,T3,T4,T5,T6],((T1,T2,T3,T4,T5,T6)) ).
list_to_terms([T1,T2,T3,T4,T5], ((T1,T2,T3,T4,T5)) ).
list_to_terms([T1,T2,T3,T4], ((T1,T2,T3,T4)) ).
list_to_terms([T1,T2,T3], ((T1,T2,T3)) ).

list_to_terms([T1,T2|T3L],T):-nonvar(T),T=( (T1,T2),T3 ),!,list_to_terms(T3L,T3).
list_to_terms([T1|T2T3L],T):-nonvar(T),T=( T1,(T2,T3) ),!,list_to_terms(T2T3L,(T2,T3)).
list_to_terms([T1],T1):-!.
list_to_terms([T1|L],(T1,TL) ):-list_to_terms(L,TL).


list_to_terms(List,(A,B)):-list_to_terms(A,AL),list_to_terms(B,BL),append(AL,BL,List).
list_to_terms(List,(A;B)):-list_to_terms(A,AL),list_to_terms(B,BL),append(AL,[or|BL],List).

:-multifile(s/3).
:-multifile(vp/4).
:-multifile(vg/5).
:-multifile(pp/3).
/*
sent --> s(gap(0)).
sent --> np(gap(0)), s(gap(np)).
s(Gap) --> np(gap(0)), vp(Gap).
np(gap(0)) --> pn.
np(gap(0)) --> det, noun.
np(Gap) --> det, noun, pp(Gap).
np(gap(np)) --> [].
pp(Gap) --> p, np(Gap).
vp(gap(0)) --> iv.
vp(Gap) --> tv, np(Gap).
vp(Gap) --> datv, np(Gap), pp(gap(0)).
vp(Gap) --> datv, np(gap(0)), pp(Gap).

det --> [Det], {lex(Det,det)}.
p --> [P], {lex(P,p)}.
noun --> [N], {lex(N,noun)}.
verb --> [V], {lex(V,v)}.
pn --> [PN], {lex(PN,pn)}.
tv --> [TV], {lex(TV,tv)}.
datv --> [DV], {lex(DV,dv)}.
iv --> [IV],{lex(IV,iv)}.

wrap(A):-any_to_words(A,X),phrase(sentence(P),X),loglang(P).


lex(kim,np).
lex(sandy,np).
lex(lee,np).
lex(baby,noun).
lex(man,noun).
lex(woman,noun).
lex(telescope,noun).
lex(park,noun).
lex(a,det).
lex(the,det).
lex(to,p).
lex(with,p).
lex(in,p).
lex(died,iv).
lex(ate,tv).
lex(ate,tv).
lex(saw,tv).
lex(gave,tv).
lex(gave,dv).
lex(handed,dv).
lex(Word,noun):-w_n([Word],_).
lex(Word,adj):-w_a([Word],_).
lex(Word,adj_sat):-w_as([Word],_).
lex(Word,adv):-w_av([Word],_).
lex(Word,VT):-member(VT,[iv,tv,dv]),w_v([Word],_).
% Students visited the town. 

s(at(NP,VP))  -->  np(Num, NP), vp(Num, VP). 

%Noun Phrase 
% this student, a beer, the town 
np(Num, np(Det, N))  -->  detp(Num, Det), ng(Num,_, N). 
% students, beers, towns(plural countable) 
np(pl, np( N))  -->  ng(pl,c, N). 
% linguistics, money, meat (singular non-countable) 
np(sg, np( N))  -->  ng(sg,nc, N). 
% a cold beer 
% the students of Tartu 
np(Num, np(Det, N, PP))  -->  detp(Num, Det), ng(Num,_, N), pp(PP). 
% he, they 
np(Num, np(PersPron))  -->  pers_pron(Num, PersPron). 
% (he saw) her reading 
np(Num, np(Det, N, V))  -->  detp(Num, Det), noun(Num, _, N), verb(_, _, part1, V). 

% Noun Group 
% student, beer, town 
ng(Num,C, ng(N))  -->  noun(Num, C, N). 
% linguistics student, beer town 
ng(Num,C, ng(N1, N2))  -->  noun(_, _, N1), noun(Num, C, N2). 

%Determiner Phrase 
% a, the, this 
detp(Num, detp(Det))  -->  det(Num,Det). 
% my, his 
detp(Num, detp(Pron))  -->  poss_pron(Num,Pron). 
% one, two, four 
detp(Num, detp(NDet)) -->  num_det(Num,NDet). 
% those four 
detp(Num, detp(Det, NDet))  --> det(Num, Det), num_det(Num, NDet). 
% all the 
detg(Num, detg(QDet, Det))  -->  quant_det(QDet), det(Num, Det). 

% Verb Phrase 
% goes, were, is sleeping 
vp(Num, vp(V))  --> vg(Num, _, V). 
% give a book 
vp(Num, vp(V, NP)) --> vg(Num, _, V), np(_, NP). 
% give the book to the teacher 
p(Num, vp(V, NP, PP))  -->  vg(Num, _, V), np(_, NP), pp(PP). 
% give her a book for birthday 
vp(Num, vp(V, NP1, NP2, PP)) -->  vg(Num, _, V), np(_, NP1), np(_, NP2), pp(PP). 
% give her a book for birthday on Monday 
vp(Num, vp(V, NP, PP1, PP2))  --> vg(Num, _, V), np(_, NP), pp(PP1), pp(PP2). 

%Verb Group 
% visits, bought, go 
vg(Num, _, vg(V))  --> verb(Num, _, fin, V). 

% Auxiliary Phrase 
% is visiting, were bought 
vg(Num, _, vg(Auxgr, V)) --> auxgr(Num, Pers, _, _, NextForm, Auxgr), verb( Num, Pers, NextForm, V). 
% will be visited 
auxgr(Num, Pers, Form, Preced, EndForm, Str) --> aux(Num, Pers, Form, EndForm, Preced, Aux), { Str =.. [auxgr, Aux] }. 
% could have been visited 
auxgr(Num, Pers, Form, Preced, EndForm, Str) --> aux(Num, Pers, Form, NextForm, Preced, Aux), auxgr(_, _, NextForm, NextPreced, EndForm, Auxgr), { Preced < NextPreced, Str =.. [auxgr, Aux, Auxgr] }. 

% Predicative Phrase 
% (he) is a student 
vg(Num, _, vg(V, COP_NP)) -->  verb(Num, _, cop, V), cop_np(COP_NP). 
% (he) is ready to go 
vg(Num, _, vg(V, PRP))  -->  verb(Num, _, cop, V), prp(PRP). 
% (he) is student 
cop_np(np(N)) --> noun(sg, _,N). 
% (he) is an interesting teacher 
cop_np(cop_np(NP))  -->  np(_, NP). 
% (beer) is cold 
prp(A)  --> adj(A). 
% (he) is interesting 
prp(A) --> adjv(A). 

% Prepositional Phrase 
% to the mountains 
pp(pp(P, NP))  -->  prep(P), np(_, NP).  

% Adjective Phrase 
% ready to go 
adjv(adjv(A,InfMark,V)) -->  adj(A), inf_mark(InfMark), verb(_, _, base, V). 

adj --> [A], {w_a([A],_);w_as([A],_)}.

% imparitive(P) --> verb_phrase(X, P1).
:-op(100,fx,+).

s(s(NP,VP))-->np(NP,VP).
np(np(Det,Noun))-->det(Det),noun(Noun).
vp(vp(Verb,NP))-->verb(Verb),np(NP).
det(det(each))-->[each].
det(det(a))-->[a].
noun(noun(man))-->[man].
noun(noun(woman))-->[woman].
verb(verb(loves))-->[loves].
*/

sentence(P) --> noun_phrase(X, P1, P), verb_phrase(X, P1).


noun_phrase(X, P2, P) -->
	determiner(X, P2, P1, P),
	adjective(X, P3, P2, P1), 
	noun(X, P4), 
	rel_clause(X, P4, P3).

noun_phrase(X, P1, P) -->
        adjective(X, P2, P1, P), 
	noun(X, P3), 
	rel_clause(X, P3, P2).

noun_phrase(X, P1, P) -->
        determiner(X, P2, P1, P), noun(X, P3), rel_clause(X, P3, P2).
noun_phrase(X, P, P) --> name(X).

verb_phrase(X, P) --> trans_verb(X, Y, P1), noun_phrase(Y, P1, P).
verb_phrase(X, P) --> intrans_verb(X, P).

rel_clause(X, P1, P1&P2) --> [that], verb_phrase(X, P2).
rel_clause(_, P, P) --> [].

adjective(X,P1, P2, ATTRIB:(P1=>P2))--> [ADJ], 
	{att(ATTRIB,ADJ,X)}.

att(attrib(blue,X),blue,X):-!.
att(attrib(ADJ,X),ADJ,X):-w_a([ADJ],_).


determiner(X, P1, P2, all_known(X):(P1=>P2) ) --> [Y], 
	{ member(Y,[all,every])}.

determiner(X, P1, P2, do_exists(X):(P1&P2) ) --> [a].
determiner(X, P1, P2, do_exists(X):(P1&P2) ) --> [an].
determiner(X, P1, P2, do_exists(X):(P1&P2) ) --> [the].

determiner(X, P1, P2, some_of(X):(P1&P2) ) --> [some].
determiner(X, P1, P2, some_of(X):(P1&P2) ) --> [many].
determiner(X, P1, P2, some_of(X):(P1&P2) ) --> [most].
determiner(X, P1, P2, some_of(X):(P1&P2) ) --> [].

noun(X, man(X) ) --> [man].
noun(X, woman(X) ) --> [woman].
noun(X, Test ) --> [Y], {Test =.. [Y,X], w_n([Y],_)}.

name(john) --> [john].

trans_verb(X, Y, loves(X,Y) ) --> [loves].
trans_verb(X, Y, TV) --> [V], {TV=..[V,X,Y],w_v([V],_)}.
intrans_verb(X, lives(X) ) --> [lives].
intrans_verb(X, ITV ) --> [V], {ITV=..[V,X],w_v([V],_)}.

% ?>

