/*
get_forms_with_atom(A,TNs)
keep_forms_with_atom(TNsIn,A,TNsOut)
remove_forms_with_atom(TNsIn,A,TNsOut)
get_forms_with_structure(S,TNs)
keep_forms_with_structure(TNsIn,S,TNsOut)
remove_forms_with_structure(TNsIn,S,TNsOut)
return_forms(TNs)
return_forms_to_html(TNs)
*/

%:-module(moo_browser,[invokeBrowserRequest/1]).

% :-include(moo_header).


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


% ===========================================================
% Search For Constant (Atom)
% ===========================================================
invokeBrowserRequest(Options):-memberchk(show='find',Options),!,
	getMooOption(word='instance',Word),
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_theory='PrologMOO',Context),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	disp_word_to_surface(Word,Context,Out),
	draw_update_enable(Context,Out).

disp_word_to_surface(Word,Context,_Ctx,Out):-
	retractall(pkids(_)),
	mooCache(PredR,SurfaceTODO,SURF,PROLOG,Context,AID,Maintainer,OnOff),
	getConstants(atomic,SURF,Atoms,_,_),memberchk(Word,Atoms),  
	writeAssertion(SurfaceTODO,SURF,PROLOG,Context,AID,Maintainer,OnOff),
	fail.	

disp_word_to_surface(Word,Context,Out):-!,setof(retract(pkids(INTID)),Out).

% ===========================================================
% Search For Tracking Number
% ===========================================================
invokeBrowserRequest(Options):-(memberchk(submit=editaid,Options);memberchk(dispasid=dispasid,Options)),!,  
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_theory='PrologMOO',Context),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	retractall(pkids(_)),!,
	disp_tn_to_surface(Context,AID,Out),
	draw_update_enable(Context,Out),!.
	
disp_tn_to_surface(Context,bogus,Out):-!.

disp_tn_to_surface(Context,TN,Out):-
	mooCache(PredR,Format,SURF,Prolog,Context,TN,Maintainer,OnOff), 
	writeAssertion(Format,SURF,Prolog,Context,TN,Maintainer,OnOff),
	fail.	

disp_tn_to_surface(Context,Out):- %isMooOption(disp_notes_nonuser=on),
	mooCache(PredR,Format,SURF,Prolog,Context,TN,Maintainer,OnOff),  
	writeAssertion(Format,SURF,Prolog,Context,TN,Maintainer,OnOff),
	fail.	

disp_tn_to_surface(Context,_,Out):-!,setof(retract(pkids(INTID)),Out).

aid_to_number(Number,Number):-number(AID),!.
aid_to_number(AID,Number):- atom_to_term(AID,Number,_),!.


% ===========================================================
% Search For State (Atom) (rejected,on,gaf).. etc
% ===========================================================
invokeBrowserRequest(Options):-memberchk(show='state',Options),!,
	getMooOption(word='disabled',Word),
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_theory='PrologMOO',Context),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	disp_word_to_surface(Word,Context,Out),
	draw_update_enable(Context,Out).

disp_status_to_surface(Status,Context,Out):-
	retractall(pkids(_)),
	mooCache(PredR,SurfaceTODO,SURF,Prolog,Context,AID,Maintainer,Status),
	writeAssertion(SurfaceTODO,SURF,Prolog,Context,AID,Maintainer,Status),
	fail.	

disp_status_to_surface(Status,Context,Out):-!,setof(retract(pkids(INTID)),Out).

% ===========================================================
% Search Disabled Assertion
% ===========================================================
invokeBrowserRequest(Options):-
	memberchk(cmd='Show Disabled',Options),!,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_theory='PrologMOO',Context),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	writeFmt('<H3><Font Color=Red>Listing Disabled Assertions...</Font></H3>',[]),!,
	show_disabled_assertions(Context,Ctx),!.
	
show_disabled_assertions(Context,Ctx):-
	mooCache(PredR,Form,Formula,Prolog,Context,AID,Maintainer,OnOff),not(OnOff=on),not(OnOff=uncanonicalized),
	writeAssertion(Form,Formula,Prolog,Context,AID,Maintainer,OnOff),
	fail.
	
show_disabled_assertions(Context,Ctx):-writeFmt('<H3><Font Color=Red>Done.</Font></H3>',[]).




% ===========================================================
% Show Assertion Updater
% ===========================================================
draw_update_enable(Context,[]):- writeFmt('Search yielded no Results.\n',[]).
draw_update_enable(Context,As):-!,
	format_o(
		'<HR>~w&nbsp;<INPUT type=submit name=submit value="Change"/>',select(ue,['Enable','Disable','Delete'])),!,
       %show_available_contexts_in_combobox(destination,[],Out),!,writeFmt('<BR><input type=radio name=CopyOrTransfer checked value=Transfer><B>Transfer</B></input>&nbsp;<input type=radio name=CopyOrTransfer value=Copy><B>Copy</B></input> selected assertions to ~w <input type=submit name=move value=Submit>',[Out]),!.
       	writeFmt('&nbsp;&nbsp;&nbsp;&nbsp;<A href="askInsert.jsp?theory=~w">Return to Ask/Insert</A>',[Context]).
		

% ===========================================================
% Show Assertions (Surface)
% ===========================================================

writeAssertion(surface,SURF,Vars,Context,AID,Maintainer,OnOff):- !,
	assert(pkids(INTID)),
	toMarkUp(html,SURF,Vars,SAtom),
	%toMarkUp(kif,SURF,Vars,KIF),
	%setMooOption(sf=KIF),
	on_to_check(on,OnOff,Checked),
	writeFmt('<hr><A HREF="askInsert.jsp?theory=~w&asid=~w&t=ea" title="Edit Assertion"><IMG border=0 src="pixmaps/cyan.gif" asrtid=~w></A><input type=chectheoryox class=assertionChecks  name=~w ~w/><nobr>',[Context,AID,AID,Checked]),
	writeFmt('<b>Surface</b> ID<font color=red>~w:~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font> Status: <font color=puple>~w</font>',[AID,Context,Maintainer,OnOff]),
	writeFmt('~w',[SAtom]),!.
	
% ===========================================================
% Show Assertions (HL)
% ===========================================================

writeAssertion(surface,WFS,Vars,Context,AID,Maintainer,OnOff):- %isMooOption(disp_notes_nonuser=on),
	assert(pkids(INTID)),
	toMarkUp(html,formula(WFS),Vars,SAtom),
	%toMarkUp(kif,SURF,Vars,KIF),
	%setMooOption(sf=KIF),
	on_to_check(on,OnOff,Checked),
	writeFmt('<hr><A HREF="askInsert.jsp?theory=~w&submit=editaid&asid=~w&t=ea" title="Show Source"><IMG border=0 src="pixmaps/purple.gif" asrtid=~w></A><input type=chectheoryox class=assertionChecks  name=~w ~w/><nobr>',[Context,AID,AID,Checked]),
	writeFmt('<b>~w</b> ID<font color=red>~w:~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font> Status: <font color=puple>~w</font>',['Surface',AID,Context,Maintainer,OnOff]),
	writeFmt('~w',[SAtom]),!.
/*
writeAssertion(skolem,WFS,Vars,Context,AID,Maintainer,OnOff):- %isMooOption(disp_notes_nonuser=on),
	assert(pkids(INTID)),
	toMarkUp(html,formula(WFS),Vars,SAtom),
	%toMarkUp(kif,SURF,Vars,KIF),
	%setMooOption(sf=KIF),
	on_to_check(on,OnOff,Checked),
	writeFmt('<hr><A HREF="askInsert.jsp?theory=~w&submit=editaid&asid=~w&t=ea" title="Show Source"><IMG border=0 src="pixmaps/purple.gif" asrtid=~w></A><input type=chectheoryox class=assertionChecks  name=~w ~w/><nobr>',[Context,AID,AID,Checked]),
	writeFmt('<b>~w</b> ID<font color=red>~w:~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>   Status: <font color=puple>~w</font>',['Skolem',AID,Context,OnOff]),
	writeFmt('~w',[SAtom]),!.
*/
writeAssertion(Head,Tail,Vars,Context,AID,Maintainer,OnOff):- %isMooOption(disp_notes_nonuser=on),
	assert(pkids(INTID)),
	toMarkUp(html,formula((Head:-Tail)),Vars,SAtom),
	%toMarkUp(kif,SURF,Vars,KIF),
	%setMooOption(sf=KIF),
	on_to_check(on,OnOff,Checked),
	writeFmt('<hr><A HREF="askInsert.jsp?theory=~w&submit=editaid&asid=~w&t=ea" title="Show Source"><IMG border=0 src="pixmaps/purple.gif" asrtid=~w></A><input type=chectheoryox class=assertionChecks  name=~w ~w/><nobr>',[Context,AID,AID,Checked]),
	writeFmt('<b>~w</b> ID<font color=red>~w:~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Status: <font color=puple>~w</font>',['Heuristic',AID,Context,OnOff]),
	writeFmt('~w',[SAtom]),!.


on_to_check(OnOff,OnOff,'Checked').
on_to_check(_,_,' ').






% ===============================================
% Read Chectheoryoxes and Make Calls
% ===============================================


% Disables
invokeBrowserRequest(Options):-     
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
invokeBrowserRequest(Options):-   
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
invokeBrowserRequest(Options):-memberchk(submit='Disable Assertion',Options),!,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_theory='PrologMOO',Context),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	writeFmt('<H3><Font Color=Red>Disabling....</Font></H3>',[]),
	disable_assertion(AID).
	
disable_assertion(AID):-
	retract(mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,on)),
	disable_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer),
	assertaClean(mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,disabled)),fail.
	
disable_assertion(AID):-writeFmt('<H3><Font Color=Red>Done.</Font></H3>',[]).

disable_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer):-
	toMarkUp(html,SURF,Vars,SAtom),
	writeFmt('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
	writeFmt('<b>~w</b> ID<font color=red>~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[Form,AID,Context,Maintainer]),
	%format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',chectheoryox(AID,OnOff)),
	writeFmt('~w<br>',[SAtom]),!.

show_disable_assertions(Form,SURF,Vars,Context,AID,Maintainer,OnOff):-
	toMarkUp(html,SURF,Vars,SAtom),
	writeFmt('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
	writeFmt('~w <b>~w</b> ID<font color=red>~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[OnOff,Form,AID,Context,Maintainer]),
	%format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',chectheoryox(AID,OnOff)),
	writeFmt('~w<hr>',[SAtom]),!.

% ===========================================================
% Enable Assertion
% ===========================================================
invokeBrowserRequest(Options):-memberchk(submit='Enable Assertion',Options),!,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_theory='PrologMOO',Context),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	writeFmt('<H3><Font Color=Red>Enabling....</Font></H3>',[]),
	enable_assertion(AID).
	
enable_assertion(AID):-
	retract(mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,_)),
	enable_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer),
	assertaClean(mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,on)),fail.
	
enable_assertion(AID):-writeFmt('<H3><Font Color=Red>Done.</Font></H3>',[]).

enable_assertion_disp(Form,SURF,Vars,Context,AID,Maintainer):-
	toMarkUp(html,SURF,Vars,SAtom),
	writeFmt('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
	writeFmt('<b>~w</b> ID<font color=red>~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[Form,AID,Context,Maintainer]),
	%format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',chectheoryox(AID,OnOff)),
	writeFmt('~w<br>',[SAtom]),!.
		

show_enable_assertions:-
	mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,on),
	show_enable_assertions(Form,SURF,Vars,Context,AID,Maintainer,OnOff),
	fail.
	
show_enable_assertions:-writeFmt('<H3><Font Color=Red>Done.</Font></H3>',[]).

show_enable_assertions(Form,SURF,Vars,Context,AID,Maintainer,OnOff):-
	toMarkUp(html,SURF,Vars,SAtom),
	writeFmt('<IMG src="pixmaps/bullet.gif" asrtid=~w><nobr>',[AID]),
	writeFmt('~w <b>~w</b> ID<font color=red>~w</font> in Context: <font color=green>~w</font>  CTX: <font color=green>~w</font>  Maintainer: <font color=green>~w</font>',[OnOff,Form,AID,Context,Maintainer]),
	%format_o('&nbsp;&nbsp;~w&nbsp;Enabled&nbsp;&nbsp;<br>',chectheoryox(AID,OnOff)),
	writeFmt('~w<hr>',[SAtom]),!.

% ===========================================================
% Edit Assertion
% ===========================================================
invokeBrowserRequest(Options):-memberchk(t='ea',Options),!,
	getMooOption(opt_ctx_assert='GlobalContext',Ctx),
	getMooOption(opt_theory='PrologMOO',Context),
	getMooOption(asid=_,AID),
	getMooOption(user='Web',User),
	getMooOption(interp='kif',Interp),
	mooCache(PredR,Form,SURF,Vars,Context,AID,Maintainer,_),
	%retractall(mooCache(PredR,_,_,_,Context,AID,_,_)),
	toMarkUp(kif,SURF,Vars,Formula),
	writeFmt('<textarea rows=6 cols=90 name="sf">~w</textarea><br>',[Formula]),
	writeFmt('<br>&nbsp;&nbsp;<INPUT type=submit name=submit value="Update Source">&nbsp;<input type=hidden name=editid value="~w">',[AID	]),
	writeFmt('&nbsp;&nbsp;&nbsp;&nbsp;<INPUT type=radio name="interp" value="kif" checked>KIF</INPUT><INPUT type=radio name="interp" value="ace" DISABLED>ACE</INPUT>&nbsp;&nbsp;<A href="askInsert.jsp?theory=~w">Cancel</a>',[Context]).



