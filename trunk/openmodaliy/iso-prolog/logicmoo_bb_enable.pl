:-include('logicmoo_utils_header.pl').
% ==============================================
% Enable or Disable Prolog Memory
% ==============================================

% Assertions have these states

% Surface (on/disabled)
% Can #1 (on/disabled) (in_mem/out_mem)
% Can #2 (on/disabled) (in_mem/out_mem)

% A user makes an assertion into an inactive Context
% Surface (on)
% Can #1 (on) (out_mem)
% Can #2 (disabled) (out_mem)

% A user makes an assertion into an active Context
% Surface (on)
% Can #1 (on) (in_mem)
% Can #2 (disabled) (out_mem)

/*

So ... this means we have Three States of any Canonical assertion...  on/in_mem/disabled

*/
% ==============================================
% Enable + Canonizlize an Existing Surface Number
% ==============================================
enable_surf_number(SurfNumber):-
	 (mooCache(PredR,surface,Surf,Vars,Context,SurfNumber,Maintainer,OnOff)),!,
	 retractall(mooCache(PredR,wfs,_,_,Context,SurfNumber,_,_)),
	 logOnFailure(moo_invoke_accept_surface(tell,[canonicalize,untrusted],surface,Surf,Ctx,SurfNumber,Context,Vars,Maintainer,accept('Previous Assertion being Recanonicalized'))).


% ==============================================
% Disable and De-Canonizlize an Existing Surface Number
% ==============================================
disable_surf_number(SurfNumber):-
	 retract(mooCache(PredR,N,surface,Surface,Vars,Context,SurfNumber,Maintainer,OnOff)),!,
	 assert(mooCache(PredR,N,surface,Surface,Vars,Context,SurfNumber,Maintainer,disabled)),!,
	 disable_each_can_with_surf_id(SurfNumber).
	 
disable_each_can_with_surf_id(SurfNumber):-
	 mooCache(PredR,CanNumber,wfs,UCL,PROLOG,Context,SurfNumber,Maintainer,_),
	 once(disable_can_number(CanNumber)),fail.
disable_each_can_with_surf_id(SurfNumber):-!.

% ==============================================
% Enable or CAN ID and reflect in Prolog Memory
% ==============================================

enable_can_number(CanNumber):-
	 mooCache(PredR,CanNumber,wfs,UCL,PROLOG,Context,TN,Maintainer,on_mem).	 
	 
enable_can_number(CanNumber):-
	 mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,on),
	 isKnowledgeBaseLoaded(Context,Ctx),!,
	 enable_can_conj(PrologFormS).

enable_can_number(CanNumber):-
	 (mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,Off)),
	 isKnowledgeBaseLoaded(Context,Ctx),!,
	 retract(mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,Off)),
	 assert(mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,on_mem)),
	 enable_can_conj(PrologFormS).

enable_can_number(CanNumber):-
	 mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,on),
	 not(isKnowledgeBaseLoaded(Context,Ctx)),!.
	 
enable_can_number(CanNumber):-
	 retract(mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,_)),
	 not(isKnowledgeBaseLoaded(Context,Ctx)),!,
	 assert(mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,on)).


enable_can_number(CanNumber):-
	 mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,on_mem),
	 not(isKnowledgeBaseLoaded(Context,Ctx)),!. %TODO Problem
	 
enable_can_number(CanNumber):-!.

enable_can_conj(true):-!.
enable_can_conj((Prolog,Form)):-!,
	enable_can_conj(Prolog),
	enable_can_conj(Form),!.
enable_can_conj(PrologForm):-
	enable_can(PrologForm).

enable_can(PrologFormS):-
	logOnFailure(assert(PrologFormS,AssertID)).

	 
% ==============================================
% Disable CAN ID and reflect in Prolog Memory
% ==============================================

disable_can_number(CanNumber):-
	 retract(mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,on)),
	 assert(mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,disabled)),
	 sendNote(user,contentMananger,'Details of Disable',mooCache(PredR,CanNumber,wfs,UCL,prolog_code,Context,TN,Maintainer,disabled)),
	 disable_can_conj(PrologFormS).

disable_can_number(CanNumber):-
	 retract(mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,State)),
	 assert(mooCache(PredR,CanNumber,wfs,UCL,PrologFormS,Context,TN,Maintainer,disabled)),
	 disable_can_conj(PrologFormS).
disable_can_number(CanNumber):-!.
	 
disable_can_conj(true):-!.
disable_can_conj((Prolog,Form)):-!,
	disable_can_conj(Prolog),
	disable_can_conj(Form),!.
disable_can_conj(PrologForm):-
	disable_can(PrologForm).
	 
	 
disable_can(PrologFormS):-catch(erase(AssertID),_,true),fail.
disable_can(PrologForm):-!.

	
% ===========================================================
% Disable Assertion
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit='Disable Assertion',Options),!,
	getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(Agent,opt_theory='LogicMOO',Context),
	getMooOption(Agent,asid=_,AID),
	getMooOption(Agent,user='Web',Agent),
	getMooOption(Agent,interp='kif',Interp),
	say('<H3><Font Color=Red>Disabling....</Font></H3>',[]),
	disable_assertion(AID).
	
disable_assertion(AID):-
	retract(mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,on)),
	disable_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer),
	assertaClean(mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,disabled)),fail.
	
disable_assertion(AID):-say('<H3><Font Color=Red>Done.</Font></H3>',[]).

disable_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer):-
	toMarkup(html,SURF,Vars,SAtom),
	say('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
	say('<b>~w</b> ID<font color=red>~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[Form,AID,Context,Maintainer]),
	%format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',chectheoryox(AID,OnOff)),
	say('~w<br>',[SAtom]),!.

show_disable_assertions(Form,SURF,Vars,Context,AID,Maintainer,OnOff):-
	toMarkup(html,SURF,Vars,SAtom),
	say('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
	say('~w <b>~w</b> ID<font color=red>~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[OnOff,Form,AID,Context,Maintainer]),
	%format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',chectheoryox(AID,OnOff)),
	say('~w<hr>',[SAtom]),!.

% ===========================================================
% Enable Assertion
% ===========================================================
clientEvent(Channel,Agent,Options):-memberchk(submit='Enable Assertion',Options),!,
	getMooOption(Agent,opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(Agent,opt_theory='LogicMOO',Context),
	getMooOption(Agent,asid=_,AID),
	getMooOption(Agent,user='Web',Agent),
	getMooOption(Agent,interp='kif',Interp),
	say('<H3><Font Color=Red>Enabling....</Font></H3>',[]),
	enable_assertion(AID).
	
enable_assertion(AID):-
	retract(mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,_)),
	enable_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer),
	assertaClean(mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,on)),fail.
	
enable_assertion(AID):-say('<H3><Font Color=Red>Done.</Font></H3>',[]).

enable_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer):-
	toMarkup(html,SURF,Vars,SAtom),
	say('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
	say('<b>~w</b> ID<font color=red>~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[Form,AID,Context,Maintainer]),
	%format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',chectheoryox(AID,OnOff)),
	say('~w<br>',[SAtom]),!.
		

show_enable_assertions:-
	mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,on),
	show_enable_assertions(Form,SURF,Vars,Context,AID,Maintainer,OnOff),
	fail.
	
show_enable_assertions:-say('<H3><Font Color=Red>Done.</Font></H3>',[]).

show_enable_assertions(Form,SURF,Vars,Context,AID,Maintainer,OnOff):-
	toMarkup(html,SURF,Vars,SAtom),
	say('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
	say('~w <b>~w</b> ID<font color=red>~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[OnOff,Form,AID,Context,Maintainer]),
	%format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',chectheoryox(AID,OnOff)),
	say('~w<hr>',[SAtom]),!.



% ===============================================
% Read Chectheoryoxes and Make Calls
% ===============================================


% Disables
clientEvent(Channel,Agent,Options):-     
		memberchk(submit='Change',Options),
		memberchk(ue='Disable',Options),  %true,
		findall(N,(member(N=on,Options),number(N)),List),
		do_each_disable(List).

	do_each_disable([]):-!.
	do_each_disable([H|T]):-!,
		do_each_disable_id(H),
		do_each_disable(T),!.
	
	do_each_disable_id(ID):-mooCache(PredR,ID,surface,_,_,_,_,SurfNumber,_,_),!,disable_surf_number(SurfNumber).
	do_each_disable_id(ID):-mooCache(PredR,ID,wfs,_,_,_,_,_,_,_),!,disable_can_number(ID).


% Enables
clientEvent(Channel,Agent,Options):-   
		memberchk(submit='Change',Options),
		memberchk(ue='Enable',Options),   %true,
		findall(N,(member(N=on,Options),number(N)),List),
		do_each_enable(List).


	do_each_enable([]):-!.
	do_each_enable([H|T]):-!,
		do_each_enable_id(H),
		do_each_enable(T),!.
	
	do_each_enable_id(ID):-mooCache(PredR,ID,surface,_,_,_,_,SurfNumber,_,_),!,enable_surf_number(SurfNumber).
	do_each_enable_id(ID):-mooCache(PredR,ID,wfs,_,_,_,_,_,_,_),!,enable_can_number(ID).
	do_each_enable_id(ID):-!.

